/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Graphics/ImageImpl.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1261 $
 * Last Updated: $Date: 2007-11-15 06:18:15 -0500 (Thu, 15 Nov 2007) $
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


//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
CONST PixelType& Image::Pixel(UINT i, UINT j) CONST
{
  LPCBYTE p = (LPCBYTE)m_pPixelData + i*m_Width*GetPixelSize() + j*GetPixelSize();
  return *(CONST PixelType*)p;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
PixelType& Image::Pixel(UINT i, UINT j)
{
  LPBYTE p = (LPBYTE)m_pPixelData + i*m_Width*GetPixelSize() + j*GetPixelSize();
  return *(PixelType*)p;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class P>
VOID Image::Clamp()
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<P>(i,j).Clamp();

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class P>
VOID Image::Clamp(typename P::ColorChannelType Low, typename P::ColorChannelType High)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<P>(i,j).Clamp(Low, High);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class P>
VOID Image::Normalize()
{
  if (Width() == 0 || Height() == 0)
    return;

  P::ColorChannelType MinimumColor = Pixel<P>(0,0)[0];
  P::ColorChannelType MaximumColor = Pixel<P>(0,0)[0];
 
  for (UINT i=0; i<Height(); i++)
  {
    for (UINT j=0; j<Width(); j++)
    {
      for (UINT k=0; k<P::Channels; k++)
      {
        MinimumColor = Min(MinimumColor, Pixel<P>(i,j)[k]);
        MaximumColor = Max(MaximumColor, Pixel<P>(i,j)[k]);
      }
    }
  }

  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      for (UINT k=0; k<P::Channels; k++)
        Pixel<P>(i,j)[k] = (P::ColorChannelType)((Pixel<P>(i,j)[k] - MinimumColor) / (FLOAT)(MaximumColor - MinimumColor));
  
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class P>
Math::Histogram1<typename P::ColorChannelType> Image::GetColorDistribution(UINT Bins)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementTemplateRuntimeType(Bolitho::Graphics,TImage,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::TImage() : Image()
{
  SetPixelFormat(PixelType::PixelFormat);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::TImage(LPCTSTR Filename) : Image(Filename, PixelType::PixelFormat)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::TImage(UINT Width, UINT Height) : Image(Width, Height, PixelType::PixelFormat)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::TImage(UINT Width, UINT Height, CONST PixelType* pImageData ) : Image(Width, Height, PixelType::PixelFormat, pImageData)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::TImage(CONST Image& I) : Image(I.Width, I.Height, I.PixelFormat, I.m_pPixelData)
{
  SetPixelFormat(PixelType::PixelFormat);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator = (CONST Image& I)
{
  Resize(I.m_Width, I.m_Height);
  if (m_pImageData)
    CopyMemory(m_pImageData, I.m_pImageData, sizeof(Pixel)*m_Width*m_Height);
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::operator CONST PixelType* () CONST
{
  return (CONST PixelType*)m_pImageData;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>::operator PixelType* ()
{
  return (PixelType*)m_pImageData;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
CONST PixelType& TImage<PixelType>::operator() (UINT i, UINT j) CONST
{
  return Pixel<PixelType>(i,j);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
PixelType& TImage<PixelType>::operator() (UINT i, UINT j)
{
  return Pixel<PixelType>(i,j);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(USE_LUA)
template<class PixelType>
Lua::Bind::Scope TImage<PixelType>::GenerateBindings( LPCTSTR Name )
{
  using namespace Lua::Bind;

  return 
    Class<TImage<Pixel>>(Name)
      .Def( Constructor<>() )
      .Def( Constructor<UINT,UINT>() )

      .Property( "Width", &TImage<Pixel>::Width )
      .Property( "Height", &TImage<Pixel>::Height )

      .Def( "GetPixel", &TImage<Pixel>::GetPixel )
      .Def( "SetPixel", &TImage<Pixel>::SetPixel )

      .Def( "Export", &TImage<Pixel>::Export );
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
VOID TImage<PixelType>::Load(LPCTSTR Filename)
{
  Image::Load(Filename, PixelType::PixelFormat);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator += (CONST TImage& Image)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) += Image.Pixel<PixelType>(i,j);

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator += (PixelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) += X;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator -= (CONST TImage& Image)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) -= Image.Pixel<PixelType>(i,j);

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator -= (PixelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) -= X;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator *= (CONST TImage& Image)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) *= Image.Pixel<PixelType>(i,j);

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator *= (PixelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) *= X;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator *= (FLOAT X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) *= X;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator /= (CONST TImage& Image)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) /= Image.Pixel<PixelType>(i,j);

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator /= (PixelType X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) /= X;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class PixelType>
TImage<PixelType>& TImage<PixelType>::operator /= (FLOAT X)
{
  for (UINT i=0; i<Height(); i++)
    for (UINT j=0; j<Width(); j++)
      Pixel<PixelType>(i,j) /= X;

  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

