/*******************************************************************************
 * Copyright (c) 2016, Rory Hool
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

#ifndef _MP4_MP4A_H_
#define _MP4_MP4A_H_
#pragma once

#include "mp4.h"
#include "MP4.ContainerAtom.h"
#include "MP4.BinaryStream.h"

namespace MP4
{
    class MP4A : public ContainerAtom
    {
    private:
        
        
    protected:
        int _dataReferenceIndex;
        int _channelCount;
        int _sampleSize;
        long _sampleRate;
        int _soundVersion;
        int _compressionId;
        int _packetSize;
        long _samplesPerPacket;
        long _bytesPerPacket;
        long _bytesPerFrame;
        long _bytesPerSample;
        
        int _reserved1;
        long _reserved2;
        
        std::vector<uint8_t> _soundVersion2Data;
        
        //byte[] soundVersion2Data;
        
        
    public:
        
        
        MP4A( void ) : ContainerAtom( ( char * )"MP4A" )
        {}
        
        std::string getName() { return "Audio Sample Entry"; }
        std::string getContent( void );
        void processData( MP4::BinaryStream * stream, uint64_t length );
    };
}

#endif /* _MP4_MP4A_H_ */
