/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RobotMotion.hpp $
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

#include "MotorControl.hpp"
#include "MotorEncoder.hpp"
#include "DeviceControl.hpp"
#include "Path.hpp"

#include "System/Threading/Thread.hpp"
using Bolitho::System::Runnable;
using Bolitho::System::Thread;

#include "System/Threading/ConditionVariable.hpp"
using Bolitho::System::ConditionVariable;

#include "Math/Matrix3.hpp"
using Bolitho::Math::Matrix3;

#include "Math/Vector2.hpp"
using Bolitho::Math::Vector2;

#include "Subsystem.hpp"

namespace NASA
{
  /* Enumeration for different states for RobotMotion */
  struct RobotMotionState { enum { PATHDRIVING = 128, MANUALDRIVING = 256, MANUALPATHDRIVING = 512  }; };

  class RobotServer;
  /* Controls and manages the position of the robot */
  class RobotMotion : public Subsystem
  {
    DeclareRuntimeType();
  public:
    RobotMotion(RobotServer& Server);

    /* Initializes the robot motion */
    virtual BOOL Initialize();

    virtual VOID Destroy();

    /* Stops all movement of the robot.  Cancels any pending move operations */
    VOID Stop();
    /* Pauses all movement of the robot.  Movement can be resumed with a call to 'Resume' */
    VOID Suspend();
    /* Resumes the execution of a previously suspended movement */
    VOID Resume();

    /* Begins driving directly to a particular location */
    VOID DriveDirectTo( Vector2F L );

    /* Begins driving a path */
    VOID SetActivePath(Ptr<Path> P);

    /* Sets the current speed and bias */
    VOID SetSpeedAndBias(FLOAT Speed, FLOAT Bias);

    VOID Drive(FLOAT Distance, FLOAT Bias);
    VOID Turn(FLOAT Angle);

    /* Returns the current position */
    Vector2F GetPosition() CONST
    { return m_Position; }

    /* Returns the current orientation */
    Vector2F GetPrimaryAxis() CONST
    { return m_Orientation; }

    /* Returns the current path */
    Ptr<Path> GetActivePath() CONST
    { return m_CurrentPath; }

    /* Returns the current status */
    DWORD GetStatus() CONST
    { return m_State; }

    /* Returns the encoder tick values for the left track */
    FLOAT GetTrackSpeed(SIZE_T i) CONST
    { return m_pMotorController->GetSpeed(i); }

    /* Returns the encoder tick values for the right track */
    UINT64 GetTrackTicks(SIZE_T i) CONST
    { return m_EncoderValue[i]; }

    /* Returns the current heading of the robot, in degrees */
    FLOAT GetHeading() CONST;

    /* Returns a rotation matrix representing the current robot orientation */
    Matrix3<FLOAT> GetRotation() CONST;
    
  protected:
    virtual INT ThreadMain();

    /* Returns a speed given a current tick value and a target tick value */
    FLOAT ComputeSpeed(UINT64 Start, UINT64 Current, UINT64 End, FLOAT MinimumSpeed, FLOAT MaximumSpeed, FLOAT Bias);

    /* The current position of the robot.  Updated by dead reackoning */
    Vector2F m_Position;
    /* The current primary axis of the robot.  Updated by dead reakoning */
    Vector2F m_Orientation;

    /* The maximum acceleration value for the robot */
    FLOAT m_MaximumAcceleration;

    /* The maximum/minimum speed of the robot */
    FLOAT m_MaximumDriveSpeed;
    FLOAT m_MinimumDriveSpeed;

    /* The maximum/minimum speed of the robot whilst turning */
    FLOAT m_MaximumTurnSpeed;
    FLOAT m_MinimumTurnSpeed;

    /* The speed boost value.  Used to overcome temporary motor stalls */
    FLOAT m_SpeedBoost;

    /* The "opcode" for path driving instructions */
    struct PathStepType
    { enum { DRIVE, TURN, SCAN }; };
    
    /* A basic path driving instruction */
    struct PATHSTEP
    {
      PATHSTEP()
      { Type = 0; Parameter = 0.0f; }
      PATHSTEP(DWORD Type, FLOAT Parameter)
      { this->Type = Type; this->Parameter = Parameter; }

      DWORD Type;
      FLOAT Parameter;
    };

    /* The sequence of instructions to follow a path */
    Array<PATHSTEP> m_CurrentPathSteps;
    /* The index of the currently executing instruction */
    SIZE_T m_CurrentPathStep;

    /* Computes the instructions nessesary to follow a given path from the current position */
    Array<PATHSTEP> ComputeSteps(Ptr<Path> P);

    /* Prepares to execute an instruction */
    VOID ExecuteInstruction(SIZE_T i);

    /* The current path that the robot will follow */
    Ptr<Path> m_CurrentPath;

    /* The Manual Driving Speed and Bias */
    FLOAT m_Speed;
    FLOAT m_Bias;

    /* The device driver for the main motor controller */
    Ptr<MotorController> m_pMotorController;

    /* The device driver for the motor encoders */
    Ptr<MotorEncoder> m_pLeftMotorEncoder;
    Ptr<MotorEncoder> m_pRightMotorEncoder;

    /* The accumulated encoder value */
    UINT64 m_EncoderValue[2];

    /* The path driving motor encoder start/targets */
    UINT64 m_EncoderStart[2];
    UINT64 m_EncoderTarget[2];

    /* The direction the tracks should move */
    INT m_TrackDirection[2];

    /* The relay controller board */
    RobotServer& m_RobotServer;

  };
}

