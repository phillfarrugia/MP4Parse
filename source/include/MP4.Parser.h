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

#ifndef _MP4_PARSER_
#pragma once
#define _MP4_PARSER_

#include "mp4.h"
#include "MP4.BinaryStream.h"
#include "MP4.AtomFactory.h"
#include "atoms.h"

namespace MP4
{
    class ParserException : public std::exception
    {
        private:
            
            
            
        protected:
            
            unsigned int code;
            
        public:
            
            enum
            {
                NoFileName  = 0x00
            };
            
            ParserException( void );
            ParserException( unsigned int c );
            virtual const char * what( void ) const throw();
    };
    
    class Parser
    {
        private:
            
            
            
        protected:
        
            AtomFactory  * _atomFactory;
            BinaryStream * _stream;
            File         * _file;
            bool           _verboseLogging;
        
            MP4::Atom* parseNextAtom();
        
        public:
            
            Parser( void );
            Parser( char * filename );
            ~Parser( void );
        
            Atom * getRootAtom( void );
            void prettyPrint( bool verbose );
            void prettyPrintAtom( Atom * atom, int depth, bool verbose );
        
            std::string getBytes( Atom * atom );
        
    };
}

#endif /* _MP4_PARSER_ */
