/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RobotVision.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 280 $
 * Last Updated: $Date: 2007-12-03 09:56:28 -0500 (Mon, 03 Dec 2007) $
 * 
 * 
 * Copyright (c) 2005-2007, Matthew G Bolitho
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of The Johns Hopkins University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***************************************************************************************************/

#pragma once

#include "LADARControl.hpp"
#include "RobotMotion.hpp"
#include "Subsystem.hpp"

#if defined(PLATFORM_WINDOWS)
#include "Graphics/StillImageCapture.hpp"
using Bolitho::Graphics::StillImageCapture;
#endif

#include "Image.hpp"

namespace NASA
{
  class RobotServer;
  /* Controls and manages the vision system of the robot */
  class RobotVision : public Subsystem
  {
  public:
    RobotVision(RobotServer& RobotServer);

    virtual BOOL Initialize();
    virtual VOID Destroy();

    Ptr<Image> GetLatestImage();
    Ptr<Image> GetImage(UINT i);

  protected:
    virtual INT ThreadMain();

    UINT m_NextSequenceNumber;

    /* Aquire a new image from the camera */
    Ptr<Image> AquireImage();

    /* The latest aquired image */
    Ptr<Image> m_pLatestImage;

    /* The Robot Motion subsystem */
    RobotServer& m_RobotServer;
    
    #if defined(PLATFORM_WINDOWS)
    /* The Camera Still Image Capture device */
    Ptr<StillImageCapture> m_pImageCapture;
    #endif

  };
}

