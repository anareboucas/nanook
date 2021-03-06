/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RobotMotion.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 303 $
 * Last Updated: $Date: 2008-02-08 11:17:35 -0500 (Fri, 08 Feb 2008) $
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
#include "RobotMotion.hpp"
#include "RobotServer.hpp"

#include "System/Configuration.hpp"
using Bolitho::System::Configuration;

#include "Math/Math.hpp"
using Bolitho::Math::Abs;
using Bolitho::Math::ACos;
using Bolitho::Math::Sign;
using Bolitho::Math::Sin;
using Bolitho::Math::Cos;
using Bolitho::Math::DEGREESTORADIANS;
using Bolitho::Math::RADIANSTODEGREES;
using Bolitho::Math::HALFPI;
using Bolitho::Math::TWOPI;
using Bolitho::Math::PI;

#include "Math/Vector3.hpp"
using Bolitho::Math::Vector3;

using Bolitho::IO::SerialPortException;

//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(NASA,RobotMotion,Object);
//----------------------------------------------------------------------------------------------------------------------------
RobotMotion::RobotMotion(RobotServer& RobotServer) : m_RobotServer(RobotServer)
{
  Trace0Enter("%p", &RobotServer);
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL RobotMotion::Initialize()
{
  Trace0Enter("");

  m_EncoderValue[0] = 0;
  m_EncoderValue[1] = 0;
  m_EncoderTarget[0] = 0;
  m_EncoderTarget[1] = 0;

  m_Orientation[0] = 0.0f;
  m_Orientation[1] = -1.0f;

  m_State = 0;

  m_CurrentPath = new Path();

  try
  {
    m_pMotorController = new MotorController();
    m_pMotorController->Initialize
      ( 
        Configuration::GetValue<String>("RobotMotion.MotorController.DeviceName"), 
        Configuration::GetValue<DWORD>("RobotMotion.MotorController.BaudRate")
      );
  }
  catch(SerialPortException& E)
  {
    #if ALLOW_MOTORCONTROLLER_FAILURE
    TraceWarning(this, "Failed to initialize controllers\n%s", E.ToString().ConstStr() ); 
    #else
    TraceError(this, "Failed to initialize controllers\n%s", E.ToString().ConstStr() ); 
    return FALSE;
    #endif
  }

  try
  {
    m_pLeftMotorEncoder = new MotorEncoder();
    m_pLeftMotorEncoder->Initialize
      ( 
        Configuration::GetValue<String>("RobotMotion.MotorEncoders.Left.DeviceName"), 
        Configuration::GetValue<DWORD>("RobotMotion.MotorEncoders.Left.BaudRate")
      );
  }
  catch(SerialPortException& E)
  {
    #if ALLOW_MOTORENCODER_FAILURE
    TraceWarning(this, "Failed to initialize left encoder\n%s", E.ToString().ConstStr()); 
    #else
    TraceError(this, "Failed to initialize left encoder\n%s", E.ToString().ConstStr()); 
    return FALSE;
    #endif
  }


  try
  {
    m_pRightMotorEncoder = new MotorEncoder();
    m_pRightMotorEncoder->Initialize
      ( 
        Configuration::GetValue<String>("RobotMotion.MotorEncoders.Right.DeviceName"), 
        Configuration::GetValue<DWORD>("RobotMotion.MotorEncoders.Right.BaudRate")
      );
  }
  catch(SerialPortException& E)
  {
    #if ALLOW_MOTORENCODER_FAILURE
    TraceWarning(this, "Failed to initialize right encoder\n%s", E.ToString().ConstStr()); 
    #else
    TraceError(this, "Failed to initialize right encoder\n%s", E.ToString().ConstStr()); 
    return FALSE;
    #endif
  }

  m_MaximumDriveSpeed = Configuration::GetValue<FLOAT>("RobotMotion.MaximumDriveSpeed");
  m_MinimumDriveSpeed = Configuration::GetValue<FLOAT>("RobotMotion.MinimumDriveSpeed");
  m_MaximumTurnSpeed = Configuration::GetValue<FLOAT>("RobotMotion.MaximumTurnSpeed");
  m_MinimumTurnSpeed = Configuration::GetValue<FLOAT>("RobotMotion.MinimumTurnSpeed");

  m_MaximumAcceleration = Configuration::GetValue<FLOAT>("RobotMotion.MaximumAcceleration");

  return Subsystem::Initialize();
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotMotion::Destroy()
{
  Trace0Enter("");

  m_pMotorController = 0;
  m_pLeftMotorEncoder = 0;
  m_pRightMotorEncoder = 0;

  Subsystem::Destroy();
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotMotion::Stop()
{
  Trace2Enter("");

  m_Lock.Lock();

  m_Speed = 0;
  m_Bias = 0;
  m_pMotorController->Stop();
  m_EncoderTarget[0] = m_EncoderValue[0];
  m_EncoderTarget[1] = m_EncoderValue[1];

  m_State &= ~(RobotMotionState::PATHDRIVING | RobotMotionState::MANUALDRIVING);

  m_Lock.Unlock();
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotMotion::Suspend()
{
  Trace1Enter("");

  m_Lock.Lock();

  m_State |= SubsystemState::PAUSED;

  m_Lock.Unlock();
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotMotion::Resume()
{
  Trace1Enter("");

  m_Lock.Lock();

  m_State &= ~SubsystemState::PAUSED;

  m_Lock.Unlock();
} 
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotMotion::DriveDirectTo(Vector2F L )
{
  Ptr<Path> P = new Path();
  P->Add(PathWaypoint(L));
  
  SetActivePath(P);
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotMotion::SetActivePath(Ptr<Path> P )
{
  m_Lock.Lock();
  m_State |= RobotMotionState::PATHDRIVING;
  m_State &= ~RobotMotionState::MANUALDRIVING;

  for (SIZE_T i=0; i<P->Count(); i++)
    TraceInfo(this, "Waypoint[%d] = {%f, %f} %d", i, P->GetWaypoint(i).Waypoint[0], P->GetWaypoint(i).Waypoint[1], P->GetWaypoint(i).ScanMode);
  
  m_CurrentPath = P;
  m_CurrentPathSteps = ComputeSteps(P);
  ExecuteInstruction(0);
  m_CurrentPathStep = 0;
  m_Lock.Unlock();
}
//----------------------------------------------------------------------------------------------------------------------------
INT RobotMotion::ThreadMain()
{
  FLOAT TestSpeed = Configuration::GetValue<FLOAT>("RobotMotion.InitializeTestSpeed");
  DWORD TestTime = Configuration::GetValue<DWORD>("RobotMotion.InitializeTestTime");

  TraceInfo(this, "Running Motor Controller Test");
  
  int initial[2];
  
  m_pMotorController->SetTrackSpeed(0, TestSpeed);
  m_pMotorController->SetTrackSpeed(1, TestSpeed);

  //Get the encoder delta value 5 times (to compensate for polling lag)
  /**/
  for(int i=0;i<5;i++){
	initial[0] = Abs(m_pLeftMotorEncoder->ReadEncoderDelta());
	initial[1] = Abs(m_pRightMotorEncoder->ReadEncoderDelta());
	Thread::Sleep(TestTime/5);
  }

  if(initial[0] == Abs(m_pLeftMotorEncoder->ReadEncoderDelta()) | initial[1] == Abs(m_pRightMotorEncoder->ReadEncoderDelta())){
	TraceInfo(this,"******************************************");
	TraceInfo(this,"*         THE MOTORS DIDN'T TURN         *");
	TraceInfo(this,"*                                        *");
	TraceInfo(this,"*	      1) CLOSE THIS PROGRAM          *");
	TraceInfo(this,"*     2) UNPLUG & PLUG THE USB CABLE     *");
	TraceInfo(this,"******************************************");
	return false;
  }
  /**/
  //Thread::Sleep(TestTime);
  
  m_pMotorController->SetTrackSpeed(0, -TestSpeed);
  m_pMotorController->SetTrackSpeed(1, -TestSpeed);
  Thread::Sleep(TestTime);

  m_pMotorController->SetTrackSpeed(0, TestSpeed);
  m_pMotorController->SetTrackSpeed(1, -TestSpeed);
  Thread::Sleep(TestTime);

  m_pMotorController->SetTrackSpeed(0, -TestSpeed);
  m_pMotorController->SetTrackSpeed(1, TestSpeed);
  Thread::Sleep(TestTime);

  m_pMotorController->Stop();


  FLOAT SpeedBoostDecay = Configuration::GetValue<FLOAT>("RobotMotion.SpeedBoostDecay");
  FLOAT SpeedBoost = Configuration::GetValue<FLOAT>("RobotMotion.SpeedBoost");
  INT64 BiasThreashold = Configuration::GetValue<INT>("RobotMotion.BiasThreashold"); 
  FLOAT MaximumBias = Configuration::GetValue<FLOAT>("RobotMotion.MaximumBias");
  FLOAT BiasFactor = Configuration::GetValue<FLOAT>("RobotMotion.BiasFactor");
  FLOAT TicksPerMeter = Configuration::GetValue<FLOAT>("RobotMotion.TicksPerMeter");
  FLOAT TicksPerDegree = Configuration::GetValue<FLOAT>("RobotMotion.TicksPerDegree");

  while(TRUE)
  {
    ResetWatchdogTimer();
    Thread::Sleep(Configuration::GetValue<DWORD>("RobotMotion.Delay"));

    m_Lock.Lock();

    INT Delta[2];
    Delta[0] = Abs(m_pLeftMotorEncoder->ReadEncoderDelta());
    Delta[1] = Abs(m_pRightMotorEncoder->ReadEncoderDelta());

    m_EncoderValue[0] += Delta[0];
    m_EncoderValue[1] += Delta[1];

    m_SpeedBoost -= SpeedBoostDecay;
    if (m_SpeedBoost < 0.0f)
      m_SpeedBoost = 0.0f;
    
    if ((m_State & SubsystemState::RUNNING) == 0)
    {
      m_Lock.Unlock();
      Stop();
      break;
    }

    if (m_State & RobotMotionState::MANUALDRIVING)
    {
      m_pMotorController->SetTrackSpeed(0, m_Speed + m_Bias);
      m_pMotorController->SetTrackSpeed(1, m_Speed - m_Bias);
    }
    else if (m_State & RobotMotionState::PATHDRIVING || m_State & RobotMotionState::MANUALPATHDRIVING)
    {
      BOOL Done[2];

      Done[0] = m_EncoderValue[0] >= m_EncoderTarget[0];
      Done[1] = m_EncoderValue[1] >= m_EncoderTarget[1];

      if (Delta[0] == 0 || Delta[1] == 0)
      {
        m_SpeedBoost += SpeedBoost;
        TraceWarning(this, "Stall detected. SpeedBoost is now %f", m_SpeedBoost);
      }

      INT64 TargetDelta = (m_EncoderTarget[1]-m_EncoderValue[1]) - (m_EncoderTarget[0]-m_EncoderValue[0]);
      if (Abs(TargetDelta) > BiasThreashold)
      {
        m_Bias = ((FLOAT)-TargetDelta / BiasFactor);

        if (m_Bias > MaximumBias)
          m_Bias = MaximumBias;
        if (m_Bias < -MaximumBias)
          m_Bias = -MaximumBias;

        //TraceInfo(this, "Bias: %f", m_Bias);
      }

      FLOAT MinimumSpeed = 0.0f;
      FLOAT MaximumSpeed = 0.0f;

      /* Update current position and orientation */
      if (m_TrackDirection[0] == m_TrackDirection[1])
      {
        MaximumSpeed = m_MaximumDriveSpeed;
        MinimumSpeed = m_MinimumDriveSpeed;

        Vector2F D = m_Orientation;
        D.Normalize();

        D *= Sign(m_TrackDirection[0]) * (Delta[0]+Delta[1]) / 2.0f / TicksPerMeter;
        m_Position += D;
      }
      else
      {
        FLOAT Theta = Sign(m_TrackDirection[0]) * (Delta[0]+Delta[1]) / 2.0f / TicksPerDegree * DEGREESTORADIANS;

        Vector2F D = m_Orientation;
        m_Orientation[0] = D[0] * Cos(Theta) - D[1] * Sin(Theta);
        m_Orientation[1] = D[0] * Sin(Theta) + D[1] * Cos(Theta);

        m_Orientation.Normalize();

        MaximumSpeed = m_MaximumTurnSpeed;
        MinimumSpeed = m_MinimumTurnSpeed;
      }

      /* Set track 0 speed */
      if (!Done[0])
        m_pMotorController->SetTrackSpeed(0, m_SpeedBoost + m_TrackDirection[0] * ComputeSpeed(m_EncoderStart[0], m_EncoderValue[0], m_EncoderTarget[0], MinimumSpeed, MaximumSpeed, m_Bias));
      else
        m_pMotorController->SetTrackSpeed(0, 0);

      /* Set track 1 speed */
      if (!Done[1])
        m_pMotorController->SetTrackSpeed(1, m_SpeedBoost + m_TrackDirection[1] * ComputeSpeed(m_EncoderStart[1], m_EncoderValue[1], m_EncoderTarget[1], MinimumSpeed, MaximumSpeed, -m_Bias));
      else
        m_pMotorController->SetTrackSpeed(1, 0);

      /* If we are done, perform next move from path */
      if (Done[0] && Done[1])
      {
        TraceInfo(this, "Step Complete.   Location: (%f, %f)  Orientation: (%f, %f)", m_Position[0], m_Position[1], m_Orientation[0], m_Orientation[1]);
        if (m_State & RobotMotionState::PATHDRIVING)
        {
          m_CurrentPathStep++;
          if (m_CurrentPathStep >= m_CurrentPathSteps.Length())
            m_State &= ~RobotMotionState::PATHDRIVING;
          else
            ExecuteInstruction(m_CurrentPathStep);
        }

        if (m_State & RobotMotionState::MANUALPATHDRIVING)
        {
          m_State &= ~RobotMotionState::MANUALPATHDRIVING;
        }

      }

    }
    else
    {
      //Trace("%I64u %I64u %I64u %I64u %I64d %I64d", m_EncoderValue[0], m_EncoderValue[1], m_EncoderTarget[0], m_EncoderTarget[1], m_EncoderTarget[0]-m_EncoderValue[0], m_EncoderTarget[1]-m_EncoderValue[1]);

      Stop();
      m_Lock.Unlock();
      continue;
    }

    m_Lock.Unlock();
  }

  TraceWarning(this, "RobotMotion::Main() has returned");
  return 0;  
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotMotion::ExecuteInstruction(SIZE_T i)
{
  Trace1Enter("%d", i);

  FLOAT TicksPerMeter = Configuration::GetValue<FLOAT>("RobotMotion.TicksPerMeter");
  FLOAT TicksPerDegree = Configuration::GetValue<FLOAT>("RobotMotion.TicksPerDegree");

  m_EncoderStart[0] = m_EncoderValue[0];
  m_EncoderStart[1] = m_EncoderValue[1];

  if (m_CurrentPathSteps[i].Type == PathStepType::DRIVE)
  {
    FLOAT Distance = m_CurrentPathSteps[i].Parameter;
    
    m_EncoderTarget[0] = m_EncoderValue[0] + (UINT64)(TicksPerMeter * Abs(Distance));
    m_EncoderTarget[1] = m_EncoderValue[1] + (UINT64)(TicksPerMeter * Abs(Distance));

    m_TrackDirection[0] = Sign(Distance);
    m_TrackDirection[1] = Sign(Distance);
  }
  else if (m_CurrentPathSteps[i].Type == PathStepType::TURN)
  {
    FLOAT Angle = m_CurrentPathSteps[i].Parameter;

    m_EncoderTarget[0] = m_EncoderValue[0] + (UINT64)(TicksPerDegree * Abs(Angle));
    m_EncoderTarget[1] = m_EncoderValue[1] + (UINT64)(TicksPerDegree * Abs(Angle));

    m_TrackDirection[0] = -Sign(Angle);
    m_TrackDirection[1] = +Sign(Angle);
  }
  else if (m_CurrentPathSteps[i].Type == PathStepType::SCAN)
  {
    m_RobotServer.GetScanningSubsystem()->AquireScan();

    m_EncoderTarget[0] = m_EncoderValue[0];
    m_EncoderTarget[1] = m_EncoderValue[1];
  }

  TraceInfo(this, "Encoder Targets (%I64u, %I64u)", m_EncoderTarget[0], m_EncoderTarget[1]);
  TraceInfo(this, "Track Directions (%d, %d)", m_TrackDirection[0], m_TrackDirection[1]);

}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotMotion::SetSpeedAndBias(FLOAT Speed, FLOAT Bias)
{
  Trace1Enter("%f, %f", Speed, Bias);

  m_Lock.Lock();

  m_State |= RobotMotionState::MANUALDRIVING;
  m_Speed = Speed;
  m_Bias = Bias;

  m_Lock.Unlock();
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotMotion::Drive(FLOAT Distance, FLOAT Bias)
{
  FLOAT TicksPerMeter = Configuration::GetValue<FLOAT>("RobotMotion.TicksPerMeter");

  Trace1Enter("%f, %f", Distance, Bias);

  m_Lock.Lock();

  m_State |= RobotMotionState::MANUALPATHDRIVING;
  m_State &= ~RobotMotionState::MANUALDRIVING;

  m_EncoderStart[0] = m_EncoderValue[0];
  m_EncoderStart[1] = m_EncoderValue[1];

  m_EncoderTarget[0] = m_EncoderValue[0] + (UINT64)(Abs(Distance) * TicksPerMeter);
  m_EncoderTarget[1] = m_EncoderValue[1] + (UINT64)(Abs(Distance) * TicksPerMeter);
  
  m_TrackDirection[0] = Sign(Distance);
  m_TrackDirection[1] = Sign(Distance);

  TraceInfo(this, "Encoder Targets (%I64u, %I64u)", m_EncoderTarget[0], m_EncoderTarget[1]);
  TraceInfo(this, "Track Directions (%d, %d)", m_TrackDirection[0], m_TrackDirection[1]);

  m_Lock.Unlock();
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotMotion::Turn(FLOAT Angle)
{
  FLOAT TicksPerDegree = Configuration::GetValue<FLOAT>("RobotMotion.TicksPerDegree");

  Trace1Enter("%f", Angle);

  m_Lock.Lock();

  m_State |= RobotMotionState::MANUALPATHDRIVING;
  m_State &= ~RobotMotionState::MANUALDRIVING;

  m_EncoderStart[0] = m_EncoderValue[0];
  m_EncoderStart[1] = m_EncoderValue[1];

  m_EncoderTarget[0] = m_EncoderValue[0] + (UINT64)(Abs(Angle) * TicksPerDegree);
  m_EncoderTarget[1] = m_EncoderValue[1] + (UINT64)(Abs(Angle) * TicksPerDegree);
  
  m_TrackDirection[0] = -Sign(Angle);
  m_TrackDirection[1] = +Sign(Angle);

  TraceInfo(this, "Encoder Targets (%I64u, %I64u)", m_EncoderTarget[0], m_EncoderTarget[1]);
  TraceInfo(this, "Track Directions (%d, %d)", m_TrackDirection[0], m_TrackDirection[1]);

  m_Lock.Unlock();
}
//----------------------------------------------------------------------------------------------------------------------------
FLOAT RobotMotion::ComputeSpeed(UINT64 StartTick, UINT64 CurrentTick, UINT64 EndTick, FLOAT MinimumSpeed, FLOAT MaximumSpeed, FLOAT Bias)
{
  Trace1Enter("%I64u, %I64u, %I64u, %f, %f", StartTick, CurrentTick, EndTick, MaximumSpeed, Bias);

  Assert(StartTick <= EndTick);
  Assert(CurrentTick >= StartTick);
  Assert(CurrentTick <= EndTick);

  UINT64 Length = EndTick-StartTick;
  INT64 i = CurrentTick-StartTick;
  INT64 Half = Length >> 1;

  FLOAT v = 0.0f;
  if (i < Half)
    v = m_MaximumAcceleration * i;
  else
    v = m_MaximumAcceleration * (-i + Length);

  if (v > MaximumSpeed)
    v = MaximumSpeed;
  if (v < MinimumSpeed)
    v = MinimumSpeed;

  v += Bias * (v / MaximumSpeed);

  Trace2Return("%f", v); 
  return v;
}
//----------------------------------------------------------------------------------------------------------------------------
Array<RobotMotion::PATHSTEP> RobotMotion::ComputeSteps(Ptr<Path> P)
{
  Trace1Enter("");

  Array<PATHSTEP> Steps;

  Vector2F Direction = m_Orientation;
  Direction.Normalize();
  
  Vector2F StartPosition = m_Position;

  for (SIZE_T i=0; i < P->Count(); i++)
  {
    Vector2F EndPosition = P->GetWaypoint(i).Waypoint;
    Vector2F LegDirection = Vector2F(EndPosition - StartPosition);
    FLOAT Distance = LegDirection.Length();
    FLOAT Angle = 0.0f;

    if (Distance > 0.0001)
    {
      LegDirection.Normalize();
      Vector2F PerpLegDirection = LegDirection.Perp();

      FLOAT Angle0 = ACos(LegDirection.Dot(Direction));
      FLOAT Angle1 = ACos(PerpLegDirection.Dot(Direction));

      if (Angle1 > HALFPI)
        Angle = Angle0;
      else
        Angle = -Angle0;
    }

    Angle *= RADIANSTODEGREES;

    Trace("Step %i:  Angle: %f  Distance: %f", i, Angle, Distance);

    Steps.Add(PATHSTEP(PathStepType::TURN, -Angle));
    Steps.Add(PATHSTEP(PathStepType::DRIVE, Distance));

    if (P->GetWaypoint(i).ScanMode > 0)
    {
      Steps.Add(PATHSTEP(PathStepType::SCAN, 0));
      Steps.Add(PATHSTEP(PathStepType::TURN, 90));
      Steps.Add(PATHSTEP(PathStepType::SCAN, 0));
      Steps.Add(PATHSTEP(PathStepType::TURN, 90));
      Steps.Add(PATHSTEP(PathStepType::SCAN, 0));
      Steps.Add(PATHSTEP(PathStepType::TURN, 90));
      Steps.Add(PATHSTEP(PathStepType::SCAN, 0));
      Steps.Add(PATHSTEP(PathStepType::TURN, 90));
    }
    
    StartPosition = EndPosition;
    Direction = LegDirection;
  }
  

  TraceInfo(this, "Compiled Path");
  for (SIZE_T i=0; i<Steps.Length(); i++)
  {
    if (Steps[i].Type == PathStepType::DRIVE)
      TraceInfo(this, "[%d] DRIVE %f", i, Steps[i].Parameter);
    if (Steps[i].Type == PathStepType::TURN)
      TraceInfo(this, "[%d] TURN %f", i, Steps[i].Parameter);
    if (Steps[i].Type == PathStepType::SCAN)
      TraceInfo(this, "[%d] SCAN %f", i, Steps[i].Parameter);
  }

  return Steps;
}
//----------------------------------------------------------------------------------------------------------------------------
FLOAT RobotMotion::GetHeading() CONST
{
  Trace2Enter("");

  FLOAT Angle0 = ACos(-m_Orientation[1]);
  FLOAT Angle1 = ACos(m_Orientation[0]);
  FLOAT Angle = 0.0f;

  if (Angle1 > HALFPI)
    Angle = TWOPI - Angle0;
  else
    Angle = Angle0;

  Angle *= RADIANSTODEGREES;
  
  Trace2Return("%f", Angle);
  return Angle;
}
//----------------------------------------------------------------------------------------------------------------------------
Matrix3<FLOAT> RobotMotion::GetRotation() CONST
{
  return Matrix3<FLOAT>::RotationY(-GetHeading() * DEGREESTORADIANS);
}
//----------------------------------------------------------------------------------------------------------------------------
