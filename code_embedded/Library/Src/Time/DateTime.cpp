/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Time/DateTime.cpp $
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





#include "CodeLibrary.hpp"
#include "Time/DateTime.hpp"

using namespace Bolitho;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
CONST INT DateTime::TicksPerSecond = 10 * 1000 * 1000;
CONST SHORT DateTime::MonthDayInYear[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
LPCTSTR DateTime::ShortMonthNames[13] = { TEXT(""), TEXT("Jan"), TEXT("Feb"), TEXT("Mar"), TEXT("Apr"), TEXT("May"), TEXT("Jun"), TEXT("Jul"), TEXT("Aug"), TEXT("Sep"), TEXT("Oct"), TEXT("Nov"), TEXT("Dec") };
LPCTSTR DateTime::LongMonthNames[13] = { TEXT(""), TEXT("January"), TEXT("February"), TEXT("March"), TEXT("April"), TEXT("May"), TEXT("June"), TEXT("July"), TEXT("August"), TEXT("September"), TEXT("October"), TEXT("November"), TEXT("December") };
LPCTSTR DateTime::ShortDayNames[7] = { TEXT("Mon"), TEXT("Tue"), TEXT("Wed"), TEXT("Thu"), TEXT("Fri"), TEXT("Sat"), TEXT("Sun") };
LPCTSTR DateTime::LongDayNames[7] = { TEXT("Monday"), TEXT("Tuesday"), TEXT("Wednesday"), TEXT("Thursday"), TEXT("Friday"), TEXT("Saturday"), TEXT("Sunday") };
//-----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,DateTime,Object);
//-----------------------------------------------------------------------------------------------------------------------------
DateTime::DateTime()
{
  m_Ticks = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime::DateTime(ULONGLONG Ticks)
{
  m_Ticks = Ticks;
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime::DateTime(time_t T)
{
  m_Ticks = (ULONGLONG)T;
  m_Ticks += 11644473600;
  m_Ticks *= TicksPerSecond;
}
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
DateTime::DateTime(FILETIME Ft)
{
  m_Ticks = Ft.dwLowDateTime + ((ULONGLONG)Ft.dwHighDateTime << 32);
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::Now()
{
  FILETIME Ft;
  GetSystemTimeAsFileTime( &Ft );
  return DateTime(Ft);
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::NowUTC()
{
  FILETIME Ft;
  GetSystemTimeAsFileTime( &Ft );
  return DateTime(Ft);
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_LINUX)
DateTime::DateTime( struct timeval T )
{
  m_Ticks = (ULONGLONG)T.tv_sec;
  m_Ticks += 11644473600;
  m_Ticks *= 1000000;
  m_Ticks += (ULONGLONG)T.tv_usec;
  m_Ticks *= 100;
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::Now()
{
  return DateTime(time(0));
}
//-----------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::NowUTC()
{
  return DateTime(time(0));
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Year() CONST
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  return P.Year;
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Month() CONST
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  return P.Month;
}

//-----------------------------------------------------------------------------------------------------------------------------
CONST TCHAR* DateTime::MonthName() CONST
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  return LongMonthNames[P.Month];
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Day() CONST
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  return P.Day;
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::DayOfYear() CONST
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  return P.DayOfYear;
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::DayOfWeek() CONST
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  return P.DayOfWeek;
}

//-----------------------------------------------------------------------------------------------------------------------------
CONST TCHAR* DateTime::DayOfWeekName() CONST
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  return LongDayNames[P.Day];
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Hour() CONST
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  return P.Hour;
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Minute() CONST
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  return P.Minute;
}

//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::Second() CONST
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  return P.Second;
}

//-----------------------------------------------------------------------------------------------------------------------------
ULONGLONG DateTime::Ticks() CONST
{
  return m_Ticks;
}
//-----------------------------------------------------------------------------------------------------------------------------
SHORT DateTime::NanoSecond() CONST
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  return P.Nano;
}

//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddNanoSeconds( INT N )
{
  m_Ticks += (N / 100);
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddMicroSeconds( INT N )
{
  m_Ticks += (N * 10);
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddMilliSeconds( INT N )
{
  m_Ticks += (N * 10000);
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddSeconds( INT N )
{
  m_Ticks += (LONGLONG)TicksPerSecond * (LONGLONG)N;
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddMinutes( INT N )
{
  m_Ticks += (LONGLONG)TicksPerSecond * 60L * N;
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddHours( INT N )
{
  m_Ticks += (LONGLONG)TicksPerSecond * 3600L * N;
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddDays( INT N )
{
  m_Ticks += (LONGLONG)TicksPerSecond * 86400L * N;
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddMonths( INT N )
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  P.Month += N;
  ConvertTimeToInternal( P, m_Ticks );
}
//-----------------------------------------------------------------------------------------------------------------------------
VOID DateTime::AddYears( INT N )
{
  DateTimeParts P;
  ConvertInternalToTime( m_Ticks, P );
  P.Year += N;
  ConvertTimeToInternal( P, m_Ticks );
}


//-----------------------------------------------------------------------------------------------------------------------------
BOOL DateTime::ConvertTimeToInternal( CONST DateTimeParts& SrcTime, ULONGLONG& DestTime )
{
  ULONGLONG nDate;
  int iDays = SrcTime.Day;
  INT nHour = SrcTime.Hour;
  INT nMinute = SrcTime.Minute;
  INT nSecond = SrcTime.Second;
  INT nHundredsNano = (SrcTime.Nano+50) / 100;
  INT nMilliSecond = 0;//SrcTime.Milli;
  INT nMicroSecond = 0;//SrcTime.Micro;
  
  // Validate year and month
  if (SrcTime.Year > 29000 || SrcTime.Year < -29000 ||
      SrcTime.Month < 1 || SrcTime.Month > 12)
    return false;
  
  //  Check for leap year
  BOOL IsLeapYear = ((SrcTime.Year & 3) == 0) &&
    ((SrcTime.Year % 100) != 0 || (SrcTime.Year % 400) == 0);
  
  /*int nDaysInMonth = 
    anMonthDayInYear[SrcTime.nMonth] - anMonthDayInYear[SrcTime.nMonth-1] +
    ((bIsLeapYear && SrcTime.nDay == 29 && SrcTime.nMonth == 2) ? 1 : 0);*/
  
  // Adjust time and frac time
  nMicroSecond += nHundredsNano / 10;
  //nHundredsNano %= 10;
  nMilliSecond += nMicroSecond / 1000;
  nMicroSecond %= 1000;
  nSecond += nMilliSecond / 1000;
  nMilliSecond %= 1000;
  nMinute += nSecond / 60;
  nSecond %= 60;
  nHour += nMinute / 60;
  nMinute %= 60;
  iDays += nHour / 24;
  nHour %= 24;
  
  //It is a valid date; make Jan 1, 1AD be 1
  nDate = SrcTime.Year*365L + SrcTime.Year/4 - SrcTime.Year/100 + SrcTime.Year/400 + MonthDayInYear[SrcTime.Month-1] + iDays;
  
  //  If leap year and it's before March, subtract 1:
  if (SrcTime.Month <= 2 && IsLeapYear)
    --nDate;
  
  //  Offset so that 01/01/1601 is 0
  nDate -= 584754L;
  
  // Change nDate to seconds
  nDate *= 86400L;
  nDate += (nHour * 3600L) + (nMinute * 60L) + nSecond;
  
  // Change nDate to hundreds of nanoseconds
  nDate *= 10000000L;
  nDate += nHundredsNano;
  
  DestTime = nDate;
  
  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------
BOOL DateTime::ConvertInternalToTime( ULONGLONG SrcTime, DateTimeParts& DestTime )
{
  ULONGLONG nTempTime;
  ULONGLONG nDaysAbsolute;     // Number of days since 1/1/0
  ULONGLONG nSecsInDay;        // Time in seconds since midnight
  ULONGLONG nMinutesInDay;     // Minutes in day
  
  ULONGLONG n400Years;         // Number of 400 year increments since 1/1/0
  ULONGLONG n400Century;       // Century within 400 year block (0,1,2 or 3)
  ULONGLONG n4Years;           // Number of 4 year increments since 1/1/0
  ULONGLONG n4Day;             // Day within 4 year block
  //  (0 is 1/1/yr1, 1460 is 12/31/yr4)
  ULONGLONG n4Yr;              // Year within 4 year block (0,1,2 or 3)
  BOOL bLeap4 = true;     // TRUE if 4 year block includes leap year
  ULONGLONG nHNanosThisDay;
  
  nTempTime = SrcTime;
  nHNanosThisDay      = (long)(nTempTime % 10000000L);
  nTempTime						/= 10000000L;
  nSecsInDay          = (long)(nTempTime % 86400L);
  nTempTime						/= 86400L;
  nDaysAbsolute       = (long)(nTempTime);
  nDaysAbsolute				+= 584754L;	//  Add days from 1/1/0 to 01/01/1601
  
  // Calculate the day of week (sun=1, mon=2...)
  //   -1 because 1/1/0 is Sat.  +1 because we want 1-based
  DestTime.DayOfWeek = (int)((nDaysAbsolute - 1) % 7L);
  
  
  // Leap years every 4 yrs except centuries not multiples of 400.
  n400Years = (long)(nDaysAbsolute / 146097L);
  
  // Set nDaysAbsolute to day within 400-year block
  nDaysAbsolute %= 146097L;
  
  // -1 because first century has extra day
  n400Century = (long)((nDaysAbsolute - 1) / 36524L);
  
  // Non-leap century
  if (n400Century != 0)
  {
    // Set nDaysAbsolute to day within century
    nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;
    
    // +1 because 1st 4 year increment has 1460 days
    n4Years = (long)((nDaysAbsolute + 1) / 1461L);
    
    if (n4Years != 0)
      n4Day = (long)((nDaysAbsolute + 1) % 1461L);
    else
    {
      bLeap4 = false;
      n4Day = (long)nDaysAbsolute;
    }
  }
  else
  {
    // Leap century - not special case!
    n4Years = (long)(nDaysAbsolute / 1461L);
    n4Day = (long)(nDaysAbsolute % 1461L);
  }
  
  if (bLeap4)
  {
    // -1 because first year has 366 days
    n4Yr = (n4Day - 1) / 365;
    
    if (n4Yr != 0)
      n4Day = (n4Day - 1) % 365;
  }
  else
  {
    n4Yr = n4Day / 365;
    n4Day %= 365;
  }

  // n4Day is now 0-based day of year. Save 1-based day of year, year number
  DestTime.DayOfYear = (int)n4Day + 1;
  DestTime.Year = (INT)(n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr);
  
  // Handle leap year: before, on, and after Feb. 29.
  if (n4Yr == 0 && bLeap4)
  {
    // Leap Year
    if (n4Day == 59)
    {
      /* Feb. 29 */
      DestTime.Month  = 2;
      DestTime.Day = 29;
      goto DoTime;
    }
    
    // Pretend it's not a leap year for month/day comp.
    if (n4Day >= 60)
      --n4Day;
  }
  
  // Make n4DaY a 1-based day of non-leap year and compute
  //  month/day for everything but Feb. 29.
  ++n4Day;
  
  // Month number always >= n/32, so save some loop time */
  for (DestTime.Month = (INT)(n4Day >> 5) + 1;
       n4Day > MonthDayInYear[DestTime.Month]; DestTime.Month++);
  
  DestTime.Day = (int)(n4Day - MonthDayInYear[DestTime.Month-1]);
  
 DoTime:
  if (nSecsInDay == 0)
    DestTime.Hour = DestTime.Minute = DestTime.Second = 0;
  else
  {
    DestTime.Second = (SHORT)(nSecsInDay % 60L);
    nMinutesInDay = nSecsInDay / 60L;
    DestTime.Minute = (SHORT)(nMinutesInDay % 60);
    DestTime.Hour   = (SHORT)(nMinutesInDay / 60);
  }

  if (nHNanosThisDay == 0)
    DestTime.Nano = 0;
  else
  {
    DestTime.Nano = (SHORT)(nHNanosThisDay * 100L);
  }
  
  return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
String DateTime::ToString() CONST
{
  DateTimeParts Date;
  ConvertInternalToTime( m_Ticks, Date );
  
  return String::Format(TEXT("%02d/%02d/%04d %02d:%02d:%02d"), Date.Day, Date.Month, Date.Year, Date.Hour, Date.Minute, Date.Second );
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
String DateTime::ToString( LPCTSTR Format ) CONST
{
  DateTimeParts Date;
  ConvertInternalToTime( m_Ticks, Date );
  
  TCHAR String[1024];
  ZeroMemory( String, 1024*sizeof(TCHAR) );
  
  LPCTSTR Ch = Format;
  while (Ch != 0)
    {
    switch (*Ch)
    {
    case 'd':
      break;
    case 'm':
      break;
    case 'y':
      break;
    case 'h':
      break;
    case 'n':
      break;
    case 's':
      break;
    case 'z':
      break;
    case 't':
      break;

    };
    Ch++;
  }
  
	return String;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
TimeSpan DateTime::operator - (CONST DateTime& D1) CONST
{
  return TimeSpan( m_Ticks - D1.m_Ticks );
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
DateTime DateTime::operator +  (CONST TimeSpan& dT) CONST
{
  return DateTime( m_Ticks + dT.m_Ticks ); 
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
DateTime& DateTime::operator += (CONST TimeSpan& dT)
{
  m_Ticks += dT.m_Ticks;
  return *this;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,TimeSpan,Object);
//---------------------------------------------------------------------------------------------------------------------------------------------------
TimeSpan::TimeSpan()
{
  m_Ticks = 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
TimeSpan::TimeSpan( LONGLONG Ticks )
{
  m_Ticks = Ticks;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
TimeSpan::TimeSpan( FILETIME T )
{
  m_Ticks = MAKELONGLONG(T.dwLowDateTime, T.dwHighDateTime);
}
#endif
//---------------------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------------------------------
TimeSpan TimeSpan::operator+ (CONST TimeSpan& T)
{
  return TimeSpan(m_Ticks+T.m_Ticks);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
TimeSpan TimeSpan::operator- (CONST TimeSpan& T)
{
  return TimeSpan(m_Ticks-T.m_Ticks);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
LONGLONG TimeSpan::Ticks() CONST
{
  return m_Ticks;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalNanoSeconds() CONST
{
  return m_Ticks * 100.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalMicroSeconds() CONST
{
  return m_Ticks / 10.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalMilliSeconds() CONST
{
  return m_Ticks / 10.0f / 1000.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalSeconds() CONST
{
  return m_Ticks / (FLOAT)DateTime::TicksPerSecond;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalMinutes() CONST
{
  return m_Ticks / (FLOAT)DateTime::TicksPerSecond / 60.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalHours() CONST
{
  return m_Ticks / (FLOAT)DateTime::TicksPerSecond / 60.0f / 60.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT TimeSpan::TotalDays() CONST
{
  return m_Ticks / (FLOAT)DateTime::TicksPerSecond / 60.0f / 60.0f / 24.0f;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------------------------
IO::BinaryStream& Bolitho::operator << (IO::BinaryStream& S, DateTime x)
{
  S.Write(x.Ticks());  
  return S;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
IO::BinaryStream& Bolitho::operator >> (IO::BinaryStream& S, DateTime& x)
{
  x = DateTime(S.ReadUInt64());
  return S;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------------------------------
IO::BinaryStream& Bolitho::operator << (IO::BinaryStream& S, TimeSpan x)
{
  S.Write(x.Ticks());
  return S;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
IO::BinaryStream& Bolitho::operator >> (IO::BinaryStream& S, TimeSpan& x)
{
  x = TimeSpan(S.ReadInt64());
  return S;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------

