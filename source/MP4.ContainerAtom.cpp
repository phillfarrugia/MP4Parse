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

#include "MP4.ContainerAtom.h"

using namespace MP4;

ContainerAtom::ContainerAtom( char * type )
{
    this->_type = type;
    
    std::transform( this->_type.begin(), this->_type.end(), this->_type.begin(), ::toupper );
}

ContainerAtom::~ContainerAtom( void )
{
    for( std::vector<MP4::Atom*>::iterator it = _children.begin(); it != _children.end(); ++it )
    {
        delete ( *it );
    }

}

void ContainerAtom::addChild( Atom * a )
{
    if( a == NULL )
    {
        return;
    }
    
    this->_children.push_back( a );
}

bool ContainerAtom::hasChildren( void )
{
    return _children.size() > 0;
}

unsigned int ContainerAtom::numberOfChildren( void )
{
    return _children.size();
}

int ContainerAtom::lengthOfChildren( void )
{
    int length = 0;
    for( std::vector<MP4::Atom*>::iterator it = _children.begin(); it != _children.end(); ++it )
    {
        length += ( *it )->getLength();
    }
    return length;
}

std::string ContainerAtom::description( int depth )
{
    std::string s;
    
    s += std::string(depth, '-') + this->_type + "\n";
    
    for( std::vector<MP4::Atom*>::iterator it = _children.begin(); it != _children.end(); ++it )
    {
       s.append( ( *it )->description( depth + 1 ) );
    }
    
    return s;
}

Atom* ContainerAtom::findChild( const std::string &type )
{
    if ( this->_type == type )
    {
      return this;
    }
    
    for( std::vector<MP4::Atom*>::iterator it = _children.begin(); it != _children.end(); ++it )
    {
        ContainerAtom *containerAtom = dynamic_cast< ContainerAtom* >( *it );
        if ( containerAtom )
        {
            return containerAtom->findChild( type );
        }
        else
        {
            if ( ( *it )->getType() == type )
            {
                return ( *it );
            }
        }
    }

    return NULL;
}
