/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/TemperatureControl.cpp $
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

#include "Robot.hpp"
#include "TemperatureControl.hpp"

#pragma warning(disable:4056)
#pragma warning(disable:4756)

//----------------------------------------------------------------------------------------------------------------------------
RobotClimateControl::RobotClimateControl()
{
  for (SIZE_T i=0; i<MAXIMUMZONECOUNT; i++)
  {
    m_CurrentTemperature[i] = MAXFLOAT / MINFLOAT;
    m_TargetTemperature[i] = MAXFLOAT / MINFLOAT;
    m_HeaterActive[i] = FALSE;
  }

}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotClimateControl::InitializeZone(UINT Zone, Ptr<ITemperatureSensor> pSensor, Ptr<IHeaterControl> pHeater, FLOAT TargetTemperature)
{
  m_pTemperatureSensor[Zone] = pSensor;
  m_pHeaterControl[Zone] = pHeater;
  m_TargetTemperature[Zone] = TargetTemperature;
}
//----------------------------------------------------------------------------------------------------------------------------
FLOAT RobotClimateControl::GetZoneTemperature(UINT Zone)
{
  return m_CurrentTemperature[Zone];
}
//----------------------------------------------------------------------------------------------------------------------------
FLOAT RobotClimateControl::GetZoneTargetTemperature(UINT Zone)
{
  return m_TargetTemperature[Zone];
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL RobotClimateControl::GetZoneHeaterActive(UINT Zone)
{
  return m_HeaterActive[Zone];
}
//----------------------------------------------------------------------------------------------------------------------------
INT RobotClimateControl::ThreadMain()
{
  while(TRUE)
  {
    Trace("Scanning Thermal Zones...");

    for (SIZE_T i=0; i<MAXIMUMZONECOUNT; i++)
    {
      if (m_pTemperatureSensor[i])
        m_CurrentTemperature[i] = m_pTemperatureSensor[i]->GetTemperature();

      Trace("Zone %d, Temperature %f, Heater %d", i, m_CurrentTemperature[i], m_CurrentTemperature[i] < m_TargetTemperature[i] );

      if (m_pHeaterControl[i])
      {
        if (m_CurrentTemperature[i] < m_TargetTemperature[i])
          m_HeaterActive[i] = TRUE;
        else
          m_HeaterActive[i] = FALSE;

        m_pHeaterControl[i]->SetActive(m_HeaterActive[i]);
      }
    }

    /* Sleep for one minute */
    Thread::Sleep(60000);
  }
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL RobotClimateControl::Initialize()
{
  return Subsystem::Initialize();
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotClimateControl::Destroy()
{
  Subsystem::Destroy();
}
//----------------------------------------------------------------------------------------------------------------------------



