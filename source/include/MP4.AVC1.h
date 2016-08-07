//
//  MP4.AVC1.hpp
//  MP4Parse
//
//  Created by Hool, Rory on 8/4/16.
//  Copyright © 2016 RoryHool. All rights reserved.
//

#ifndef _MP4_AVC1_H_
#define _MP4_AVC1_H_
#pragma once

#include "mp4.h"
#include "MP4.ContainerAtom.h"
#include "MP4.BinaryStream.h"

namespace MP4
{
    class AVC1 : public ContainerAtom
    {
        private:
        
        
        protected:
        
        
        public:
        
            AVC1( void ) : ContainerAtom( ( char * )"AVC1" )
            {}
        
            std::string description( int depth );
            void processData( MP4::BinaryStream * stream, size_t length );
            int getLength( void );
        
    };
}

#endif /* MP4_AVC1_hpp */