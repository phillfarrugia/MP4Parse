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

#include "MP4.ELST.h"

using namespace MP4;
          
ELST::ELST( void )
{
    this->_type.append( "ELST" );
}

std::string ELST::getContent( void )
{
    std::ostringstream o;
    
    o << "Entry Count: " << this->_entryCount << "\n";
    o << "Entries:\n";
    o << "Segment Duration   Time   Rate\n";
    long count = _entries.size();
    long omitted = 0;
    if( count > 20 )
    {
        omitted = _entries.size() - 20;
        count = 20;
    }
    for(int i = 0; i < count; i++)
    {
        o << "    " << _entries[i].segmentDuration << "          " << _entries[i].mediaTime << "      " << _entries[i].mediaRate << "\n";
    }
    if( omitted > 0 )
    {
        o << "  " << omitted << " entries omitted\n";
    }
    
    return o.str();
}

void ELST::processData( MP4::BinaryStream * stream, uint64_t length )
{
    FullBox::processData( stream, length );
    _entryCount = stream->readBigEndianUnsignedInteger();
    
    for( uint32_t i = 0; i < _entryCount; i++ )
    {
        Entry entry;
        if( _version == 1 ) {
            entry.segmentDuration = stream->readBigEndianUnsignedLong();
            entry.mediaTime = stream->readUnsignedLong();
        } else {
            entry.segmentDuration = stream->readBigEndianUnsignedInteger();
            entry.mediaTime = stream->readUnsignedInteger();
        }
        entry.mediaRate = stream->readBigEndianUnsignedInteger() / 65536;
        
        _entries.push_back(entry);
    }
}
