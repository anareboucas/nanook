/***************************************************************************************************
* File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RobotScanning.cpp $
* Author:       $Author: matthew $
* Revision:     $Revision: 295 $
* Last Updated: $Date: 2008-01-23 09:21:49 -0500 (Wed, 23 Jan 2008) $
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

#include "Robot.hpp"
#include "RobotScanning.hpp"
#include "RobotMotion.hpp"
#include "RobotServer.hpp"

#include "System/Process.hpp"
using Bolitho::System::Process;

#include "IO/File.hpp"
using Bolitho::IO::File;

#include "System/Configuration.hpp"
using Bolitho::System::Configuration;

using Bolitho::IO::SerialPortException;

#include "Debug.h"


//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(NASA,RobotScanning,Subsystem);
//----------------------------------------------------------------------------------------------------------------------------
RobotScanning::RobotScanning(RobotServer& RobotServer) : m_RobotServer(RobotServer)
{
	m_NextSequenceNumber = 1;
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL RobotScanning::Initialize()
{
	Trace0Enter("");

	try
	{
		m_pLADARMotor = new LADARMotorController();
		m_pLADARMotor->Initialize(Configuration::GetValue<String>("RobotScanning.StepMotorController.DeviceName"), Configuration::GetValue<DWORD>("RobotScanning.StepMotorController.BaudRate"));
	}
	catch(SerialPortException& E)
	{
#if ALLOW_LADARMOTORCONTROLLER_FAILURE
		TraceWarning(this, "Failed to initialize stepmotor controller\n%s", E.ToString().ConstStr() ); 
#else
		TraceError(this, "Failed to initialize stepmotor controller\n%s", E.ToString().ConstStr() ); 
		return FALSE;
#endif
	}

	try
	{
		m_pLADAR = new LADAR();
		m_pLADAR->Initialize(Configuration::GetValue<String>("RobotScanning.LADAR.DeviceName"), Configuration::GetValue<DWORD>("RobotScanning.LADAR.BaudRate"));
	}
	catch(SerialPortException& E)
	{
#if ALLOW_LADAR_FAILURE
		TraceWarning(this, "Failed to initialize LADAR\n%s", E.ToString().ConstStr() ); 
#else
		TraceError(this, "Failed to initialize LADAR\n%s", E.ToString().ConstStr() ); 
		return FALSE;
#endif
	}

	m_pMap = new Map(Configuration::GetValue<DWORD>("RobotScanning.Map.Width"), Configuration::GetValue<DWORD>("RobotScanning.Map.Height"), Configuration::GetValue<FLOAT>("RobotScanning.Map.Resolution"));

	m_pLADARMotor->SetSpeed(Configuration::GetValue<UINT>("RobotScanning.StepMotorController.Speed"));
	m_pLADARMotor->SetPosition(Configuration::GetValue<UINT>("RobotScanning.StepMotorController.InitialPosition"));

#if !defined(LADAR_SIMULATION)
	m_pLADAR->SetScanMode(180, 100);
	CalibratePosition();
#endif

	m_pLADAR->SetScanMode(Configuration::GetValue<UINT>("RobotScanning.LADAR.FieldOfView"), Configuration::GetValue<UINT>("RobotScanning.LADAR.Resolution"));

	m_pLADARMotor->MoveAndWait(Configuration::GetValue<INT>("RobotScanning.RestToIdle"));
	m_State = 0;

	// Read in initial scanning parameters
	m_ScanLines = Configuration::GetValue<INT>("RobotScanning.ScanLines");
	m_StepsPerScanLine = (Configuration::GetValue<INT>("RobotScanning.RestToIdle") * 2) / m_ScanLines; 
	m_UnitConversion = Configuration::GetValue<FLOAT>("RobotScanning.LADAR.DistanceUnitsPerMeter");
	m_BottomOfScan = 0;//-Configuration::GetValue<INT>("RobotScanning.RestToIdle");

	return Subsystem::Initialize();
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotScanning::CalibratePosition()
{
	WORD ThreasholdDistance = Configuration::GetValue<WORD>("RobotScanning.Calibration.ThreasholdDistance");
	INT StepSize = Configuration::GetValue<DWORD>("RobotScanning.Calibration.StepSize");
	INT StepsToStop = Configuration::GetValue<DWORD>("RobotScanning.Calibration.StepsToStop");

	Trace0Enter("Calibrating LADAR...");

	if (StepSize == 0)
	{
		TraceWarning(this, "Calibration Skipped.  StepSize == 0");
		return;
	}

	Buffer<WORD> Scan = m_pLADAR->RequestSingleScan();

	if (Scan.Length() == 0)
		return;

	if (Scan[0] < ThreasholdDistance)
		m_pLADARMotor->MoveAndWait(StepsToStop*2);

	SIZE_T HitCount = 0;
	while(TRUE)
	{
		if (Scan[0] < ThreasholdDistance)
		{
			HitCount++;
			if (HitCount > 4)
				break;
		}
		m_pLADARMotor->MoveAndWait(-StepSize);
		Scan = m_pLADAR->RequestSingleScan();
	}
	m_pLADARMotor->MoveAndWait(-StepsToStop);

}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotScanning::Destroy()
{
	Trace0Enter("");

	m_pLADARMotor = 0;
	m_pLADAR = 0;

	Subsystem::Destroy();
}
//----------------------------------------------------------------------------------------------------------------------------
Ptr<RangeImage> RobotScanning::InternalAquireScan()
{
	Trace1Enter("");

	ScanInterrupted = FALSE;
	// move down to the position of the bottom of the scan
	INT RestToIdle = Configuration::GetValue<INT>("RobotScanning.RestToIdle");
	whatis(DEBUG_SCANNING_1,m_BottomOfScan,%f);
	INT StartPos =(int) m_BottomOfScan*(Configuration::GetValue<INT>("RobotScanning.StepsPerDegree"));
	whatis(DEBUG_SCANNING_1,StartPos,%d);
	m_pLADARMotor->MoveAndWait( StartPos);
	whereis(DEBUG_SCANNING_1);

	// original movement
	//m_pLADARMotor->MoveAndWait(-Configuration::GetValue<INT>("RobotScanning.RestToIdle"));

	m_NextSequenceNumber++;

	// m_pLADAR->SamplesPerScanLine() should = m_ResolutionHorizontal
	RangeImage* pRangeImage = new RangeImage(m_NextSequenceNumber, m_pLADAR->SamplesPerScanLine(), m_ScanLines);

	SIZE_T i;
	for (i=0;( i<pRangeImage->Height())&& (!ScanInterrupted); i++)
	{
		printf("%d\n", i);
		Buffer<WORD> Scan = m_pLADAR->RequestSingleScan();
		FLOAT* pDestScanLine = pRangeImage->GetScanLine(i);

		for (SIZE_T j=0; j<pRangeImage->Width(); j++)
			pDestScanLine[j] = Scan[j] / m_UnitConversion;

		m_pLADARMotor->MoveAndWait(m_StepsPerScanLine);
	}

	// move back to the idle (rest?) position
	m_pLADARMotor->MoveAndWait(-StartPos - i*m_StepsPerScanLine);
	// original movement back
	//m_pLADARMotor->MoveAndWait(-Configuration::GetValue<INT>("RobotScanning.RestToIdle"));

	if(i!=pRangeImage->Height()){
		RangeImage * resized= new RangeImage(pRangeImage->SequenceNumber(),pRangeImage->Width(),i);
		for(unsigned int a =0;a<i;a++){
			for(unsigned int b=0;b<pRangeImage->Width();b++){
				resized->Set(a,b, pRangeImage->Get(a,b));
			}
		}
		pRangeImage->Release();
		// hopefully this doesn't cause a memory leak...
		pRangeImage = resized;



	}

	pRangeImage->AquiredOn() = DateTime::Now();

	Ptr<RobotMotion> pMotion = m_RobotServer.GetMotionSubsystem();
	pRangeImage->ScannerPosition() = pMotion->GetPosition();
	pRangeImage->ScannerPrimaryAxis() = pMotion->GetPrimaryAxis();
	pRangeImage->ScannerRotation() = pMotion->GetRotation();

	pRangeImage->HorizontalFieldOfView() = m_pLADAR->GetFieldOfView();
	pRangeImage->VerticalFieldOfView() =(float) i*m_StepsPerScanLine/(Configuration::GetValue<INT>("RobotScanning.StepsPerDegree"));
	pRangeImage->HorizontalBias() = 0.0f;
	pRangeImage->VerticalBias() = m_BottomOfScan+pRangeImage->VerticalFieldOfView()/2;
	pRangeImage->MaximumRange() = Configuration::GetValue<FLOAT>("RobotScanning.LADAR.MaximumRange") / m_UnitConversion;

#if !defined(SPACECUBE)
	String Filename = String::Format("Logs\\Robot.%d.%d.RangeImage", Process::GetCurrentProcessId(), m_NextSequenceNumber );
	TraceInfo(this, "Saving scan to file \"%s\"", Filename.ConstStr());

	pRangeImage->Write(File::Open(Filename));
#endif

	return pRangeImage;
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotScanning::AquireScan()
{
	Trace1Enter("");

	Ptr<RobotMotion> pMotion = m_RobotServer.GetMotionSubsystem();
	pMotion->Suspend();
	Ptr<RangeImage> pRangeImage = InternalAquireScan();
	pMotion->Resume();

	pRangeImage->Splat(m_pMap, Configuration::GetValue<FLOAT>("RobotScanning.Map.MaximumSplatHeight"));

#if !defined(SPACECUBE)
	String Filename = String::Format("Logs\\Robot.%d.%d.Map", Process::GetCurrentProcessId(), pRangeImage->SequenceNumber() );
	TraceInfo(this, "Saving map to file \"%s\"", Filename.ConstStr());
	m_pMap->Write(File::Open(Filename));
#endif

	m_Lock.Lock();
	m_pLatestScan = pRangeImage;
	m_Lock.Unlock();
}
//----------------------------------------------------------------------------------------------------------------------------
Buffer<WORD> RobotScanning::SingleScan(UINT startPos){
		  m_pLADARMotor->MoveAndWait(startPos*(Configuration::GetValue<INT>("RobotScanning.StepsPerDegree")));
		  Buffer<WORD> Scan = m_pLADAR->RequestSingleScan();
		  return Scan;
}
//----------------------------------------------------------------------------------------------------------------------------
INT RobotScanning::ThreadMain()
{
	Trace1Enter("");

	while (TRUE)
	{
		m_Lock.Lock();
		DWORD State = m_State;
		m_Lock.Unlock();

		if ((State & SubsystemState::RUNNING) == 0)
			break;

		if (State & RobotScanningStatus::SCANPENDING)
		{
			m_Lock.Lock();
			m_State |= RobotScanningStatus::SCANNING;
			m_State &= ~RobotScanningStatus::SCANPENDING;
			m_Lock.Unlock();

			Ptr<RobotMotion> pMotion = m_RobotServer.GetMotionSubsystem();
			pMotion->Suspend();
			Ptr<RangeImage> pRangeImage = InternalAquireScan();
			pMotion->Resume();

			pRangeImage->Splat(m_pMap, Configuration::GetValue<FLOAT>("RobotScanning.Map.MaximumSplatHeight"));

#if !defined(SPACECUBE)
			String Filename = String::Format("Logs\\Robot.%d.%d.Map", Process::GetCurrentProcessId(), pRangeImage->SequenceNumber() );
			TraceInfo(this, "Saving map to file \"%s\"", Filename.ConstStr());
			m_pMap->Write(File::Open(Filename));
#endif

			m_Lock.Lock();
			m_State = SubsystemState::RUNNING;
			m_pLatestScan = pRangeImage;
			m_Lock.Unlock();
		}

		Thread::Sleep(Configuration::GetValue<DWORD>("RobotScanning.Delay"));
	}

	TraceWarning(this, "RobotScanning::Main() has returned");
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotScanning::EnqueueScan()
{
	m_Lock.Lock();
	m_State |= RobotScanningStatus::SCANPENDING;
	m_Lock.Unlock();
}
//----------------------------------------------------------------------------------------------------------------------------
DWORD RobotScanning::GetStatus() CONST
{
	return m_State;
}
VOID RobotScanning::InterruptScan() {
	ScanInterrupted = true;

}

//----------------------------------------------------------------------------------------------------------------------------
void RobotScanning::HorizontalScanMode(UINT FieldOfView, UINT Resolution)
{ 
	// Let LADAR class validate input...
	m_pLADAR->SetScanMode(FieldOfView, Resolution);

	m_ResolutionHorizontal = Resolution; 
	m_FieldOfViewHorizontal = FieldOfView;
}
//----------------------------------------------------------------------------------------------------------------------------
void RobotScanning::VerticalScanMode(float DegreesPerScanLine, UINT ScanLines)
{ 
	// RobotScanning.RestToIdle gives motor steps for ~45 degree rotation
	m_ScanLines = ScanLines;

	m_StepsPerScanLine = (Configuration::GetValue<INT>("RobotScanning.StepsPerDegree")) * DegreesPerScanLine;
}
//----------------------------------------------------------------------------------------------------------------------------

