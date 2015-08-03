/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RobotScanning.hpp $
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
#include "LADARMotorControl.hpp"
#include "RangeImage.hpp"
#include "Subsystem.hpp"

namespace NASA
{
  struct RobotScanningStatus
  { enum  { SCANPENDING = 128, SCANNING = 256 }; };

  class RobotServer;
  class RobotScanning : public Subsystem
  {
    DeclareRuntimeType();
  public:
    RobotScanning(RobotServer& RobotServer);

    /* Initialize the scanning subsystem */
    virtual BOOL Initialize();

    /* Destory the scanning subsystem */
    virtual VOID Destroy();

    /* A non-blocking scan */
    VOID EnqueueScan();

    /* A blocking scan */
    VOID AquireScan();

	// interrupt a scan which is in progress, and send back whatever data has been produced.
	VOID InterruptScan();

    DWORD GetStatus() CONST;

    /* Gets the last scan taken with the scanner */
    Ptr<RangeImage> GetLatestScan() CONST
    { return m_pLatestScan; }

    /* Gets a specific scan from the history */
    Ptr<RangeImage> GetScan(SIZE_T i) CONST;

    /* Gets the sequence number of the latest scan */
    SIZE_T GetLatestSequenceNumber() CONST
    { return m_NextSequenceNumber - 1; }

    Ptr<Map> GetMap()
    { return m_pMap; }

  public:
    /* Calibrates the position of the LADAR */
    VOID CalibratePosition();

    /* Aquires a new scan from the scanner */
    Ptr<RangeImage> InternalAquireScan();

    /* The Robot Motion subsystem */
    RobotServer& m_RobotServer;

    SIZE_T m_NextSequenceNumber;

    INT ThreadMain();

    /* The LADAR assembly step motor controller */
    Ptr<LADARMotorController> m_pLADARMotor;
    /* The LADAR controller */
    Ptr<LADAR> m_pLADAR;

    /* The latest scan aquired */
    Ptr<RangeImage> m_pLatestScan;

    /* The current map */
    Ptr<Map> m_pMap;

	/*Single Scan*/
	Buffer<WORD> SingleScan(UINT startPos);
  
	// Accessors and mutators
	INT ScanLines(){ return m_ScanLines; }
	INT FieldOfViewHorizontal(){ return m_FieldOfViewHorizontal; }
	INT ResolutionHorizontal(){ return m_ResolutionHorizontal; }
	FLOAT FieldOfViewVertical(){ return m_FieldOfViewVertical; }
	FLOAT BottomOfScan(){ return m_BottomOfScan; }
	FLOAT BottomOfScan(FLOAT x){ return m_BottomOfScan = x; }

	void HorizontalScanMode(UINT FieldOfView, UINT Resolution);
	void VerticalScanMode(float degreesPerScanLine, UINT ScanLines);

  protected:
	  // externally setable

	  /// number of scanlines to take in the image
	  INT m_ScanLines;
	  /// Yaw angle in view, in degrees, either 100 or 180
	  INT m_FieldOfViewHorizontal;
	  /// Angular distance b/w points horizontally, in 1/100ths of degrees
	  INT m_ResolutionHorizontal;  
		/// number of steps between each scanline
	  INT m_StepsPerScanLine;
	  /// Pitch at which to start scanning, moving upwards by FieldOfViewVertical
	  FLOAT m_BottomOfScan;

	  // defined by externally setable parameters
	  /// Pitch angle in view, in degrees 
	  FLOAT m_FieldOfViewVertical;
	  
	  
	  FLOAT m_UnitConversion;


	  boolean ScanInterrupted;
  };
}

