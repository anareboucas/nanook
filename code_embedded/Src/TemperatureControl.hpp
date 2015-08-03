/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/TemperatureControl.hpp $
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

#include "Subsystem.hpp"

namespace NASA
{
  /* The interface used by temperature sensor devices */
  class ITemperatureSensor : public Object
  {
  public:
    virtual FLOAT GetTemperature() = 0;
  };

  /* The interface used to control a heating device */
  class IHeaterControl : public Object
  {
  public:
    virtual VOID SetActive(BOOL Active) = 0;
  };

  /* Manages the environmental subsystem of the robot */
  class RobotClimateControl : public Subsystem
  {
  public:
    RobotClimateControl();

    virtual BOOL Initialize();
    virtual VOID Destroy();

    /* Initializes a zone with a sensor, heater, and target temperature.
    * If pHeater is NULL, no temperature control will be performed, and TargetTemperature is ignored.
    */
    VOID InitializeZone(UINT Zone, Ptr<ITemperatureSensor> pSensor, Ptr<IHeaterControl> pHeater, FLOAT TargetTemperature);

    /* Gets the temperature for a thermal zone */
    FLOAT GetZoneTemperature(UINT Zone);

    /* Gets the target temperature for a thermal zone */
    FLOAT GetZoneTargetTemperature(UINT Zone);

    /* Gets the heater temperature for a thermal zone */
    BOOL GetZoneHeaterActive(UINT Zone);

  protected:
    virtual INT ThreadMain();

    static CONST SIZE_T MAXIMUMZONECOUNT = 8;

    FLOAT m_CurrentTemperature[MAXIMUMZONECOUNT];
    FLOAT m_TargetTemperature[MAXIMUMZONECOUNT];
    BOOL m_HeaterActive[MAXIMUMZONECOUNT];

    Ptr<ITemperatureSensor> m_pTemperatureSensor[MAXIMUMZONECOUNT];
    Ptr<IHeaterControl> m_pHeaterControl[MAXIMUMZONECOUNT];

    DWORD m_Flags;
  };


#if defined(TEST_CLIMATECONTROL)
  namespace Test
  {
    class TemperatureSensor : public ITemperatureSensor, public IHeaderControl
    {
    public:
      virtual FLOAT GetTemperature();
      virtual VOID SetMode(DWORD Mode);

    private:
      VOID ComputeNewTemperature();
      Timer m_ElapsedTime; 
      FLOAT m_Temperature;
    };
  }
#endif
}

