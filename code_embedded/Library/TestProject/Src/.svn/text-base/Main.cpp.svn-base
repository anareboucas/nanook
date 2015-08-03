/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/TestProject/Src/Main.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1168 $
 * Last Updated: $Date: 2007-09-29 11:47:34 -0400 (Sat, 29 Sep 2007) $
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




#define LIBRARYAPI_STATIC

#include "CodeLibrary.hpp"
#include "IO/StdIO.hpp"
#include "System/ConsoleApplication.hpp"
#include "Debug/Trace.hpp"
#include "IO/File.hpp"

//#include "Graphics/Image.hpp"
//#include "Graphics/Raster.hpp"
#include "Graphics/Grid.hpp"


using namespace Bolitho;
using namespace Bolitho::IO;
using namespace Bolitho::System;
using namespace Bolitho::Graphics;

class TestApplication : public ConsoleApplication
{
	public: 
    virtual INT Main();
};


struct TestTransform
{
  FLOAT operator() (FLOAT Value)
  {
    return Value * 2.0f;
  }
};

struct TestFilter
{
  FLOAT Accumulate(UINT N, ...)
  {
    Assert(N == 9);
    FLOAT* pValues[9];
    
    va_list Values;
    va_start(Values, N);
    for (SIZE_T i=0; i<9; i++)
      pValues[i] = va_arg(Values, FLOAT*);
    va_end(Values);

    return *pValues[4] * 2.0f;
  }
};

ImplementApplicationEntryPoint(TestApplication)

INT TestApplication::Main()
{
  Trace0Enter("");

  Grid<FLOAT> G("Test.Grid", 8192, 8192);

  Timer T;
  T.Start();

  TestTransform XF;
  G.Transform(XF);
  Trace("TimeTaken: %f", T.ElapsedSeconds());

  return 0;
}



/*
Image I(512,512);
Raster<Image,Color> R(I);

R.FillTriangle(PointF(10,10), PointF(10,100), PointF(100,10), Brush<Color>(Color::Blue));

I.Save("Foo.Bmp");
*/

