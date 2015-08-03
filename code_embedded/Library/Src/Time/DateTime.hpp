/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Time/DateTime.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1265 $
 * Last Updated: $Date: 2007-11-15 16:11:48 -0500 (Thu, 15 Nov 2007) $
 * 
 * 
 * Copyright (c) 2004-2007, Matthew G Bolitho
 * All rights reserved.
 *
 *
 * Microsoft Reference License (Ms-RL)
 *   
 * This license governs use of the accompanying software. If you use the software, you accept 
 * this license. If you do not accept the license, do not use the software.
 *
 * 1. Definitions
 *  - The terms "reproduce," "reproduction" and "distribution" have the same meaning here as under 
 *    U.S. copyright law.
 *  - "You" means the licensee of the software.
 *  - "Your company" means the company you worked for when you downloaded the software.
 *  - "Reference use" means use of the software within your company as a reference, in read only 
 *    form, for the sole purposes of debugging your products, maintaining your products, or 
 *    enhancing the interoperability of your products with the software, and specifically excludes 
 *    the right to distribute the software outside of your company.
 *  - "Licensed patents" means any Licensor patent claims which read directly on the software as 
 *    distributed by the Licensor under this license.
 * 
 * 2. Grant of Rights
 *  (A) Copyright Grant- Subject to the terms of this license, the Licensor grants you a non-transferable, 
 *      non-exclusive, worldwide, royalty-free copyright license to reproduce the software for reference use.
 *  (B) Patent Grant- Subject to the terms of this license, the Licensor grants you a non-transferable,
 *      non-exclusive, worldwide, royalty-free patent license under licensed patents for reference use.
 * 
 * 3. Limitations
 *  (A) No Trademark License - This license does not grant you any rights to use the Licensor's name
 *      logo, or trademarks.
 *  (B) If you begin patent litigation against the Licensor over patents that you think may apply 
 *      to the software (including a cross-claim or counterclaim in a lawsuit), your license to the 
 *      software ends automatically.
 *  (C) The software is licensed "as-is." You bear the risk of using it. The Licensor gives no express 
 *      warranties, guarantees or conditions. You may have additional consumer rights under your local 
 *      laws which this license cannot change. To the extent permitted under your local laws, the 
 *      Licensor excludes the implied warranties of merchantability, fitness for a particular purpose 
 *      and non-infringement.
 *
 ***************************************************************************************************/





#pragma once

namespace Bolitho
{
  class DateTime;

  class LIBRARY_API TimeSpan : public Object
  {
    friend class DateTime;

    DeclareRuntimeType();
  public:
    TimeSpan();
    TimeSpan( LONGLONG Ticks );
		
    #if defined(PLATFORM_WINDOWS)
    TimeSpan( FILETIME T );
    #endif

		TimeSpan operator+ (CONST TimeSpan& T);
		TimeSpan operator- (CONST TimeSpan& T);

    LONGLONG Ticks() CONST;
    
    FLOAT TotalNanoSeconds() CONST;
    FLOAT TotalMicroSeconds() CONST;
    FLOAT TotalMilliSeconds() CONST;
    FLOAT TotalSeconds() CONST;
    FLOAT TotalMinutes() CONST;
    FLOAT TotalHours() CONST;
    FLOAT TotalDays() CONST;

  protected:
    LONGLONG m_Ticks;
  };

  IO::BinaryStream& operator << (IO::BinaryStream& S, TimeSpan x);
  IO::BinaryStream& operator >> (IO::BinaryStream& S, TimeSpan& x);
  
  class LIBRARY_API DateTime : public Object
  {
    DeclareRuntimeType();
  public:
    enum DayOfWeek
    {
      SUNDAY = 0,
      MONDA = 1,
      TUESDAY = 2,
      WEDNESDAY = 3,
      THURSDAY = 4,
      FRIDAY = 5,
      SATURDAY = 6
    };

    enum MonthOfYear
    {
      JANUARY = 1,
      FEBRUARY = 2,
      MARCH = 3,
      APRIL = 4,
      MAY = 5,
      JUNE = 6,
      JULY = 7,
      AUGUST = 8,
      SEPTEMBER = 9,
      OCTOBER = 10,
      NOVEMBER = 11,
      DECEMBER = 12
    };

  public:
    DateTime();
    DateTime( ULONGLONG Ticks );
    DateTime( time_t T );

    #if defined(PLATFORM_WINDOWS)
    DateTime( FILETIME Ft );
    #elif defined(PLATFORM_LINUX)
    DateTime( struct timeval );
    #endif

    TimeSpan  operator -  (CONST DateTime& d1) CONST;
    DateTime  operator +  (CONST TimeSpan& dT) CONST;
    DateTime& operator += (CONST TimeSpan& dT);

    SHORT Year() CONST;
    SHORT Month() CONST;
    SHORT Day() CONST;
    SHORT DayOfYear() CONST;
    SHORT DayOfWeek() CONST;
    SHORT Hour() CONST;
    SHORT Minute() CONST;
    SHORT Second() CONST;
    SHORT NanoSecond() CONST;

    ULONGLONG Ticks() CONST;

    operator ULONGLONG () CONST
    { return Ticks(); }

    CONST TCHAR* DayOfWeekName() CONST;
    CONST TCHAR* MonthName() CONST;

    static CONST INT TicksPerSecond;

    VOID AddNanoSeconds( INT N );
    VOID AddMicroSeconds( INT N );
    VOID AddMilliSeconds( INT N );
    VOID AddSeconds( INT N );
    VOID AddMinutes( INT N );
    VOID AddHours( INT N );
    VOID AddDays( INT N );
    VOID AddMonths( INT N );
    VOID AddYears( INT N );

    static DateTime NowUTC();
    static DateTime Now();

    virtual String ToString() CONST;
    virtual String ToString( LPCTSTR Format ) CONST;

    static BOOL IsLeapYear(INT Year)
    { return (Year % 4 == 0 && Year % 100 != 0) || (Year % 400 == 0); }

    static CONST INT HOURSPERDAY = 24;
    static CONST INT MINUTESPERHOUR = 60;
    static CONST INT MINUTESPERDAY = 24 * 60;
    static CONST INT SECONDSPERMINUTE = 60;
    static CONST INT SECONDSPERHOUR = 3600;
    static CONST INT SECONDSPERDAY = 3600 * 24;

  protected:
    ULONGLONG m_Ticks;

    struct DateTimeParts
    {
      SHORT Year;
      SHORT Month;
      SHORT Day;
      SHORT Hour;
      SHORT Minute;
      SHORT Second;
      SHORT Nano;
      SHORT DayOfYear;
      SHORT DayOfWeek;
    };

    static BOOL ConvertTimeToInternal( CONST DateTimeParts& SrcTime, ULONGLONG& DestTime );
    static BOOL ConvertInternalToTime( ULONGLONG SrcTime, DateTimeParts& DestTime );

    static CONST SHORT MonthDayInYear[];
    static CONST TCHAR* ShortMonthNames[];
    static CONST TCHAR* LongMonthNames[];
    static CONST TCHAR* ShortDayNames[];
    static CONST TCHAR* LongDayNames[];

  };
  
  IO::BinaryStream& operator << (IO::BinaryStream& S, DateTime x);
  IO::BinaryStream& operator >> (IO::BinaryStream& S, DateTime& x);

}

#if defined(PLATFORM_LINUX)
INLINE LONG GetTickCount()
{
  tms tm;
  return times(&tm);
}
#endif

