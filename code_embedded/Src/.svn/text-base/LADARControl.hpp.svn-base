/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/LADARControl.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 304 $
 * Last Updated: $Date: 2008-02-19 14:18:31 -0500 (Tue, 19 Feb 2008) $
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

#include "RangeImage.hpp"

#include "IO/File.hpp"
using Bolitho::IO::File;
using Bolitho::IO::FileMode;
using Bolitho::IO::FileAccess;

#include "IO/SerialPort.hpp"
using Bolitho::IO::SerialPort;
using Bolitho::Buffer;

#if defined(LADAR_SIMULATION)
#pragma pack(push, 1)

namespace NASA
{
  /* The header on scanner telegrams */
  struct TELEGRAMHEADER
  {
    BYTE Stx;
    BYTE Address;
    WORD PacketLength;
    BYTE Response;
  };

  /* The scanner status information */
  struct SCANNERSTATUS
  {
    CHAR SoftwareVersion[7]; // A
    BYTE OperatingMode; // B
    BYTE Status; // C
    WORD Reserved1; // D
    BYTE DeviceType; // E
    WORD PollutionValues[8]; // F
    WORD RefPollutionValues[4]; // G
    WORD CalibrationPollutionValues[8]; // H
    WORD CalibrationRefPollutionValues[4]; // I
    WORD MotorRevolutions; // J
    WORD Reserved2; // K
    WORD ReferenceScale1Dark100; // L
    WORD Reserved3; // M
    WORD ReferenceScale2Dark100; // N
    WORD ReferenceScale1Dark66; // O
    WORD Reserved4; // P
    WORD ReferenceScale2Dark66; // Q
    WORD SignalAmplitude; // R
    WORD CurrentAngle; // S
    WORD PeakThreshhold; // R
    WORD AngleOfMeasurement; // U
    WORD CalibrationSignalAmplitude; // V
    WORD TargetStopThreshold; // W
    WORD TargetPeakThreshold; // X
    WORD ActualStopThreshold; // Y
    WORD ActualPeakThreshold; // Z
    BYTE Reserved5; // A1
    BYTE MeasuringMode; // A2
    WORD ReferenceTargetSingle; // A3
    WORD ReferenceTargetMean; // A4
    WORD ScanningAngle; // A5
    WORD AngularResolution; // A6
    BYTE RestartMode; // A7
    WORD RestartTime; // A8
    CHAR OffsetFieldSet2; // A9
    BYTE Reserved7; // B1
    WORD BaudRate; // B2
    BYTE EvaluationNumber;
    BYTE PermanentBaudRate;
    BYTE LMSAddress;
    BYTE ActiveFieldSet;
    BYTE MeasurementUnit;
    BYTE LaserState;
    CHAR Reserved8[7];
  };

#pragma pack(pop)

  class LIBRARY_API LADARException : public Exception
  {
  public:
    LADARException(LPCTSTR Message) : Exception(0)
    {
      m_Description = Bolitho::CharTraits<TCHAR>::Duplicate(Message);
    }

    LADARException( CONST LADARException& E ) : Exception(E)
    {
      m_Description = Bolitho::CharTraits<TCHAR>::Duplicate(E.m_Description);
    }

    virtual String Description() CONST
    { return m_Description; }

    virtual ~LADARException()
    {
      if (m_Description)
        free(m_Description);
    }

  private:
    TCHAR* m_Description;
  };


  /* A "driver" class for the LADAR.  All calls between the rest of the system and the LADAR scanner should be made through this class */ 
  class LADAR : public Object
  {
    DeclareRuntimeType();
  public:
    LADAR();

    /* Reset the LADAR */
    VOID Reset();

    /* Initialize the LADAR for use.  Connects via the device specified in 'DeviceName' */
    VOID Initialize(LPCTSTR DeviceName, DWORD Baud);

    /* Sets the horizontal field of view of the LADAR.  Valid values are 180 and 100 */
    /* Sets the horizontal resolution of the LADAR.  Valid values are 100, 50 and 25 */
    BOOL SetScanMode(UINT FieldOfView, UINT Resolution);

    /* Requests a single scan line */
    Buffer<WORD> RequestSingleScan();

    /* Requests an averaged scan line */
    Buffer<WORD> RequestMeanScan(UINT N);

    /* Get the scanner status */
    SCANNERSTATUS GetStatus();

    /* Gets the number of samples per scan line at the current field of view and resolution */
    SIZE_T SamplesPerScanLine() CONST
    { return m_SamplesPerScanLine; }

    FLOAT GetFieldOfView() CONST
    { return (FLOAT)m_HorizontalAngle; }


    BOOL RequestBaudRate(DWORD Baud);
    VOID MakeBaudRatePermanent();

  private:
    UINT m_HorizontalResolution;
    UINT m_HorizontalAngle;
    SIZE_T m_SamplesPerScanLine;

    RangeImage m_TestData;
    UINT m_TestDataRowIndex;
  };
}
#else
#pragma pack(push, 1)

namespace NASA
{
  /* The header on scanner telegrams */
  struct TELEGRAMHEADER
  {
    BYTE Stx;
    BYTE Address;
    WORD PacketLength;
    BYTE Response;
  };

  /* The scanner status information */
  struct SCANNERSTATUS
  {
    CHAR SoftwareVersion[7]; // A
    BYTE OperatingMode; // B
    BYTE Status; // C
    WORD Reserved1; // D
    BYTE DeviceType; // E
    WORD PollutionValues[8]; // F
    WORD RefPollutionValues[4]; // G
    WORD CalibrationPollutionValues[8]; // H
    WORD CalibrationRefPollutionValues[4]; // I
    WORD MotorRevolutions; // J
    WORD Reserved2; // K
    WORD ReferenceScale1Dark100; // L
    WORD Reserved3; // M
    WORD ReferenceScale2Dark100; // N
    WORD ReferenceScale1Dark66; // O
    WORD Reserved4; // P
    WORD ReferenceScale2Dark66; // Q
    WORD SignalAmplitude; // R
    WORD CurrentAngle; // S
    WORD PeakThreshhold; // R
    WORD AngleOfMeasurement; // U
    WORD CalibrationSignalAmplitude; // V
    WORD TargetStopThreshold; // W
    WORD TargetPeakThreshold; // X
    WORD ActualStopThreshold; // Y
    WORD ActualPeakThreshold; // Z
    BYTE Reserved5; // A1
    BYTE MeasuringMode; // A2
    WORD ReferenceTargetSingle; // A3
    WORD ReferenceTargetMean; // A4
    WORD ScanningAngle; // A5
    WORD AngularResolution; // A6
    BYTE RestartMode; // A7
    WORD RestartTime; // A8
    CHAR OffsetFieldSet2; // A9
    BYTE Reserved7; // B1
    WORD BaudRate; // B2
    BYTE EvaluationNumber;
    BYTE PermanentBaudRate;
    BYTE LMSAddress;
    BYTE ActiveFieldSet;
    BYTE MeasurementUnit;
    BYTE LaserState;
    CHAR Reserved8[7];
  };

#pragma pack(pop)

  class LIBRARY_API LADARException : public Exception
  {
  public:
    LADARException(LPCTSTR Message) : Exception(0)
    {
      m_Description = Bolitho::CharTraits<TCHAR>::Duplicate(Message);
    }

    LADARException( CONST LADARException& E ) : Exception(E)
    {
      m_Description = Bolitho::CharTraits<TCHAR>::Duplicate(E.m_Description);
    }

    virtual String Description() CONST
    { return m_Description; }

    virtual ~LADARException()
    {
      if (m_Description)
        free(m_Description);
    }

  private:
    TCHAR* m_Description;
  };


  /* A "driver" class for the LADAR.  All calls between the rest of the system and the LADAR scanner should be made through this class */ 
  class LADAR : public Object
  {
    DeclareRuntimeType();
  public:
    LADAR();

    /* Reset the LADAR */
    VOID Reset();

    /* Initialize the LADAR for use.  Connects via the device specified in 'DeviceName' */
    VOID Initialize(LPCTSTR DeviceName, DWORD Baud);

    /* Sets the horizontal field of view of the LADAR.  Valid values are 180 and 100 */
    /* Sets the horizontal resolution of the LADAR.  Valid values are 100, 50 and 25 */
    BOOL SetScanMode(UINT FieldOfView, UINT Resolution);

    /* Requests a single scan line */
    Buffer<WORD> RequestSingleScan();

    /* Requests an averaged scan line */
    Buffer<WORD> RequestMeanScan(UINT N);

    /* Get the scanner status */
    SCANNERSTATUS GetStatus();

    /* Gets the number of samples per scan line at the current field of view and resolution */
    SIZE_T SamplesPerScanLine() CONST
    { return m_SamplesPerScanLine; }

    FLOAT GetFieldOfView() CONST
    { return (FLOAT)m_HorizontalAngle; }

    /* Requests a change in baud rate */
    BOOL RequestBaudRate(DWORD Baud);
    VOID MakeBaudRatePermanent();

	/**
	 * Tries to determine at what baud rate the LADAR is currently running by requesting scans at each possible rate.
	 * Returns the first rate at which a response is received.
	 */
	DWORD LADAR::DetermineBaudRate();
	/**
	 * Tries to request a single scan from the LADAR, but ignores the data and instead 
	 * returns 1 if it gets any response and 0 if it times out.
	 */
	DWORD LADAR::TestSingleScan();

  private:
    /* The port used to communicate with the scanner */
    #if defined(SPACECUBE)
    Ptr<File> m_pLADARPort;
    #else    
    Ptr<SerialPort> m_pLADARPort;
    #endif

    /* Parses a response telegram from the scanner */
    VOID SendTelegram(BYTE Command, CONST Buffer<BYTE>& Data);
    VOID SendTelegram(BYTE Command, LPCBYTE pData, SIZE_T Length);
    Buffer<BYTE> ReadTelegram(BOOL WaitForSTX = FALSE);
    WORD ComputeCrc16(LPCBYTE pData, SIZE_T Length); 

    UINT m_HorizontalResolution;
    UINT m_HorizontalAngle;

    SIZE_T m_SamplesPerScanLine;

  };
}
#endif
