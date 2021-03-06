/*******************************************************************************
 * Copyright (c) 2011, Jean-David Gadina - www.xs-labs.com
 * Distributed under the Boost Software License, Version 1.0.
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/
 
/* $Id$ */

#include "MP4.Parser.h"
#include "atoms.h"
#include <stack>
#include <stdio.h>
#include "hex.h"

using namespace MP4;

ParserException::ParserException( void )
{
    this->code = -1;
}

ParserException::ParserException( unsigned int c )
{
    this->code = c;
}

const char * ParserException::what( void ) const throw()
{
    switch( this->code )
    {
        case ParserException::NoFileName:
            
            return "No input file";
    }
    
    return "Unknown exception";
}

Parser::Parser( void )
{
    ParserException e = ParserException( ParserException::NoFileName );
    
    throw e;
}

Parser::Parser( char * filename )
{
    MP4::Atom          * atom;
    MP4::ContainerAtom * parentAtom;
    int                  parentDepth = 0;
    
    std::stack<MP4::ContainerAtom*> previousParents;
   
    _atomFactory = new AtomFactory();
    this->_stream = new BinaryStream( filename );
    this->_file   = new MP4::File();
    FILE * pFile;
    long size = 0;
    this->_verboseLogging = false;
    
    pFile = fopen (filename,"rb");
    if(pFile==NULL) perror ("Error opening file");
    else
    {
        fseek (pFile, 0, SEEK_END);   // non-portable
        size=ftell (pFile);
        fclose (pFile);
    }
    this->_file->setDataLength(size);
    
    if( _verboseLogging )
    {
        std::cout << "File size = " << size <<  "\n";
    }
    
    parentAtom = this->_file;
    
    while( !this->_stream->eof() )
    {
        atom = parseNextAtom();
        
        if ( atom == nullptr ) {
            break;
        }
        
        while( parentAtom->lengthOfChildren() + atom->getLength() > parentAtom->getDataLength()) {
            if( parentDepth == 0 ) {
                break;
            }
            parentAtom = previousParents.top();
            previousParents.pop();
            parentDepth--;
            
            if( _verboseLogging )
            {
                std::cout << " ---- Resetting parent to " << parentAtom->getType() << " at " << atom->getType() << "\n";
            }
        }
        if( _verboseLogging )
        {
            uint64_t remaining = parentAtom->getDataLength() - parentAtom->lengthOfChildren() + atom->getLength();
            std::cout << "Adding child " << atom->getType() << " to parent " << parentAtom->getType() << "\n";
            std::cout << "pA->loc = " << parentAtom->lengthOfChildren() << " a.l = " << atom->getLength() << " pA.gdl = " << parentAtom ->getDataLength() << "\n";
            std::cout << "       remaining = " << remaining << "\n";
        }
        parentAtom->addChild(atom);
        
        ContainerAtom *containerAtom = dynamic_cast<ContainerAtom*>( atom );
        if( containerAtom ) {
            previousParents.push(parentAtom);
            parentAtom = containerAtom;
            if( _verboseLogging )
            {
                std::cout << " ---- Setting parent to " << parentAtom->getType() << " \n";
            }
            parentDepth++;
        }
        
        // Seems to only happen in some AVC1 atoms
        // Sometimes after COLOR/PASP child atoms they have an extra 4 bytes.
        uint64_t diff = parentAtom->getDataLength() - parentAtom->lengthOfChildren();
        if ( diff > 0 && diff < 8 ) {
            std::cout << "Not enough space ( " << diff << " bytes ) left in " << parentAtom->getType() << " for child, ignoring\n";
            _stream->ignore( diff );
        }
    }
}

MP4::Atom* Parser::parseNextAtom()
{
    std::streampos atomStart = this->_stream->tellg();
    
    uint64_t length = this->_stream->readBigEndianUnsignedInteger();
    if ( this->_stream->eof() ) {
        return nullptr;
    }
    
    uint64_t dataLength = 0;
    char type[ 5 ];
    memset( type, 0, 5 );
    this->_stream->read( ( char * )type, 4 );
    
    uint64_t headerLength = 8;
    
    if( length == 1 )
    {
        headerLength += 8;
        dataLength = this->_stream->readBigEndianUnsignedLong() - 16;
    }
    else
    {
        dataLength = length - 8;
    }
    
    if( _verboseLogging )
    {
        std::cout << "Found " << type << " atom, length = " << dataLength << "\n";
    }
    
    Atom * atom = _atomFactory->instantiateAtom( type );
    std::streampos start = _stream->tellg();
    atom->processData( this->_stream, dataLength );
    uint64_t read = _stream->tellg() - start;
    
    atom->setHeaderLength( headerLength + read );
    atom->setDataLength( dataLength - read );
    
    std::streampos atomEnd = this->_stream->tellg();
    
    atom->setStreamPos(atomStart, atomEnd);
    
    return atom;
}

Parser::~Parser( void )
{
    delete this->_stream;
    delete this->_file;
}

Atom* Parser::getRootAtom( void )
{
    return _file;
}

void Parser::prettyPrint( bool verbose )
{
    prettyPrintAtom( _file, 0, verbose );
}


void Parser::prettyPrintAtom( Atom * atom, int depth, bool verbose )
{
    if( depth != 0 ) {
        std::cout << std::string(depth, '-') << " " << atom->getType() << " ( " << atom->getName() << " )\n";
        if( verbose ) {
            std::cout << atom->getContent() << "\n";
        }
    }
    
    ContainerAtom *containerAtom = dynamic_cast<ContainerAtom*>( atom );
    if( containerAtom ) {
        std::vector<MP4::Atom*> children = containerAtom->getChildren();
        for( std::vector<MP4::Atom*>::iterator it = children.begin(); it != children.end(); ++it )
        {
            prettyPrintAtom( ( *it ), depth+1, verbose );
        }
    }
}

std::string Parser::getBytes( Atom * atom )
{
    this->_stream->clear();
    this->_stream->seekg(atom->getStartStreamPos());
    
    std::ostringstream prefix;
    std::ostringstream byteStream;
    std::ostringstream stringStream;
    
    uint64_t read = 0;
    while( this->_stream->tellg() != atom->getEndStreamPos() ) {
        if( read > 2500 ) {
            prefix << "Truncated to 2500 bytes\n\n";
            break;
        }
        char s[1];
        memset(s, 0, 1);
        this->_stream->read((char *)s, 1);
        
        byteStream << hex(s[0]) << " ";
        
        if ( s[0] == '\0' ) {
            stringStream << '.';
        } else {
            stringStream << s[0];
        }
        read++;
    }
    
    return prefix.str() + "Bytes:\n" + byteStream.str() + "\n\nString:\n" + stringStream.str();
}
