/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Graphics/Image.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1198 $
 * Last Updated: $Date: 2007-10-22 20:05:46 -0400 (Mon, 22 Oct 2007) $
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
#include "Debug/Trace.hpp"
#include "Graphics/Image.hpp"

using namespace Bolitho;
using namespace Bolitho::Com;
using namespace Bolitho::IO;
using namespace Bolitho::Graphics;

#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics, ImageException, UserException);
ImplementRuntimeType(Bolitho::Graphics, Image, Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
Image::Image()
{
  m_Width = 0;
  m_Height = 0;
  m_pPixelData = 0;
  m_PixelFormat = PixelFormat::UNKNOWN;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Image::Image(UINT Width, UINT Height, DWORD PixelFormat)
{
  m_Width = 0;
  m_Height = 0;
  m_pPixelData = 0;
  m_PixelFormat = PixelFormat::UNKNOWN;

  Resize(Width, Height, PixelFormat);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Image::Image(UINT Width, UINT Height, DWORD PixelFormat, LPVOID pImageData)
{
  m_Width = 0;
  m_Height = 0;
  m_pPixelData = 0;
  m_PixelFormat = PixelFormat::UNKNOWN;

  Resize(Width, Height, PixelFormat);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Image::Image(CONST Image& I)
{
  m_Width = 0;
  m_Height = 0;
  m_pPixelData = 0;
  m_PixelFormat = PixelFormat::UNKNOWN;

  Resize(I.m_Width, I.m_Height, I.m_PixelFormat);
  CopyMemory(m_pPixelData, I.m_pPixelData, m_Width*m_Height*GetPixelSize());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Image::Image(LPCTSTR Filename)
{
  m_Width = 0;
  m_Height = 0;
  m_pPixelData = 0;
  m_PixelFormat = PixelFormat::UNKNOWN;
  Load(Filename);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Image::Image(LPCTSTR Filename, DWORD PixelFormat)
{
  m_Width = 0;
  m_Height = 0;
  m_pPixelData = 0;
  m_PixelFormat = PixelFormat::UNKNOWN;
  Load(Filename, PixelFormat);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Image::~Image()
{
  if (m_pPixelData)
    FreeMemory(m_pPixelData);
  m_pPixelData = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Image& Image::operator = (CONST Image& I)
{
  Resize(I.m_Width, I.m_Height, I.m_PixelFormat);
  CopyMemory(m_pPixelData, I.m_pPixelData, m_Width*m_Height*GetPixelSize());
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Image::Resize(UINT Width, UINT Height, DWORD PixelFormat)
{
  if (m_pPixelData)
    FreeMemory(m_pPixelData);

  m_Width = Width;
  m_Height = Height;
  m_PixelFormat = PixelFormat;
  m_pPixelData = AllocateMemory(m_Width*m_Height*GetPixelSize());

  ZeroMemory(m_pPixelData, m_Width*m_Height*GetPixelSize());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Image::Resize(UINT Width, UINT Height)
{
  if (m_pPixelData)
    FreeMemory(m_pPixelData);

  m_Width = Width;
  m_Height = Height;
  if (Width*Height*GetPixelSize() > 0)
    m_pPixelData = AllocateMemory(m_Width*m_Height*GetPixelSize());

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Image::SetPixelFormat(DWORD PixelFormat)
{
  if (m_pPixelData == 0)
  {
    m_PixelFormat = PixelFormat;
    return;
  }

  LPVOID pNewPixelData = AllocateMemory(m_Width*m_Height*GetPixelSize(PixelFormat));

  for (UINT i=0; i<m_Height; i++)
    ConvertScanLine( (LPBYTE)pNewPixelData + i*m_Width*GetPixelSize(PixelFormat), PixelFormat, (LPBYTE)m_pPixelData + i*m_Width*GetPixelSize(m_PixelFormat), m_PixelFormat, m_Width);

  FreeMemory(m_pPixelData);
  m_pPixelData = pNewPixelData;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------


#define TryFormat(x)                                                            \
{                                                                               \
  for (UINT i=0; i<PixelFormatCount; i++)                                       \
  if (IsEqualGUID(pPixelFormats[i], x))                                         \
  return x;                                                                     \
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
WICPixelFormatGUID ChooseBestPixelFormat(DWORD PixelFormat, UINT PixelFormatCount, WICPixelFormatGUID* pPixelFormats)
{
  switch (PixelFormat)
  {
  case PixelFormat::RGBA32:
    TryFormat(GUID_WICPixelFormat32bppBGRA);
    TryFormat(GUID_WICPixelFormat32bppBGR);
    TryFormat(GUID_WICPixelFormat24bppRGB);
    TryFormat(GUID_WICPixelFormat24bppBGR);
    break;
  case PixelFormat::RGB32:
    TryFormat(GUID_WICPixelFormat32bppBGR);
    TryFormat(GUID_WICPixelFormat24bppRGB);
    TryFormat(GUID_WICPixelFormat24bppBGR);
    TryFormat(GUID_WICPixelFormat32bppBGRA);
    break;
  case PixelFormat::RGB24:
    TryFormat(GUID_WICPixelFormat24bppRGB);
    TryFormat(GUID_WICPixelFormat24bppBGR);
    TryFormat(GUID_WICPixelFormat32bppBGR);
    TryFormat(GUID_WICPixelFormat32bppBGRA);
    break;

  case PixelFormat::BGRA32:
    TryFormat(GUID_WICPixelFormat32bppBGRA);
    TryFormat(GUID_WICPixelFormat32bppBGR);
    TryFormat(GUID_WICPixelFormat24bppBGR);
    TryFormat(GUID_WICPixelFormat24bppRGB);
    break;
  case PixelFormat::BGR32:
    TryFormat(GUID_WICPixelFormat32bppBGR);
    TryFormat(GUID_WICPixelFormat24bppBGR);
    TryFormat(GUID_WICPixelFormat24bppRGB);
    TryFormat(GUID_WICPixelFormat32bppBGRA);
    break;
  case PixelFormat::BGR24:
    TryFormat(GUID_WICPixelFormat24bppBGR);
    TryFormat(GUID_WICPixelFormat24bppRGB);
    TryFormat(GUID_WICPixelFormat32bppBGR);
    TryFormat(GUID_WICPixelFormat32bppBGRA);
    break;

  case PixelFormat::RGBA128F:
    TryFormat(GUID_WICPixelFormat128bppRGBAFloat);
    TryFormat(GUID_WICPixelFormat32bppBGRA);
    TryFormat(GUID_WICPixelFormat32bppBGR);
    TryFormat(GUID_WICPixelFormat24bppBGR);
    TryFormat(GUID_WICPixelFormat24bppRGB);
    break;
  case PixelFormat::RGB128F:
    TryFormat(GUID_WICPixelFormat128bppRGBAFloat);
    TryFormat(GUID_WICPixelFormat32bppBGR);
    TryFormat(GUID_WICPixelFormat32bppBGRA);
    TryFormat(GUID_WICPixelFormat24bppBGR);
    TryFormat(GUID_WICPixelFormat24bppRGB);
    break;
  case PixelFormat::RGB96F:
    TryFormat(GUID_WICPixelFormat128bppRGBAFloat);
    TryFormat(GUID_WICPixelFormat24bppBGR);
    TryFormat(GUID_WICPixelFormat24bppRGB);
    TryFormat(GUID_WICPixelFormat32bppBGR);
    TryFormat(GUID_WICPixelFormat32bppBGRA);
    break;

  case PixelFormat::R8:
    TryFormat(GUID_WICPixelFormat8bppGray);
    TryFormat(GUID_WICPixelFormat32bppBGRA);
    TryFormat(GUID_WICPixelFormat24bppBGR);
    TryFormat(GUID_WICPixelFormat24bppRGB);
    break;
  case PixelFormat::R32F:
    TryFormat(GUID_WICPixelFormat32bppGrayFloat);
    TryFormat(GUID_WICPixelFormat128bppRGBAFloat);
    TryFormat(GUID_WICPixelFormat8bppGray);
    TryFormat(GUID_WICPixelFormat32bppBGRA);
    TryFormat(GUID_WICPixelFormat24bppBGR);
    TryFormat(GUID_WICPixelFormat24bppRGB);
    break;
  }

  return GUID_WICPixelFormatUndefined;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#define MatchFormat(x, y)                                                       \
{                                                                               \
  if (IsEqualGUID(Format, x))                                                   \
  return y;                                                                     \
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD ChooseBestPixelFormat(WICPixelFormatGUID& Format)
{
  MatchFormat(GUID_WICPixelFormat128bppRGBAFloat, PixelFormat::RGBA128F);
  MatchFormat(GUID_WICPixelFormat24bppBGR, PixelFormat::BGR24);
  MatchFormat(GUID_WICPixelFormat24bppRGB, PixelFormat::RGB24);
  MatchFormat(GUID_WICPixelFormat32bppBGR, PixelFormat::BGR32);
  MatchFormat(GUID_WICPixelFormat32bppBGRA, PixelFormat::BGRA32);
  MatchFormat(GUID_WICPixelFormat32bppGrayFloat, PixelFormat::R32F);
  MatchFormat(GUID_WICPixelFormat8bppGray, PixelFormat::R8);

  return PixelFormat::UNKNOWN;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT GetPixelFormatStride(UINT Width, WICPixelFormatGUID& Format)
{
  if (IsEqualGUID(Format, GUID_WICPixelFormat32bppBGRA))
    return (Width * 32 + 7) / 8;
  if (IsEqualGUID(Format, GUID_WICPixelFormat32bppBGR))
    return (Width * 32 + 7) / 8;
  if (IsEqualGUID(Format, GUID_WICPixelFormat24bppBGR))
    return (Width * 24 + 7) / 8;
  if (IsEqualGUID(Format, GUID_WICPixelFormat128bppRGBAFloat))
    return (Width * 16);
  if (IsEqualGUID(Format, GUID_WICPixelFormat8bppGray))
    return Width;

  Assert(FALSE);
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Image::Save(LPCTSTR Filename, DWORD FileFormat, DWORD PixelFormat) CONST
{
  if (FileFormat == ImageFormat::UNKNOWN)
  {
    if (Path::ExtensionMatches(Filename, TEXT("PNG")))
      FileFormat = ImageFormat::PNG;
    if (Path::ExtensionMatches(Filename, TEXT("BMP")))
      FileFormat = ImageFormat::BMP;
    if (Path::ExtensionMatches(Filename, TEXT("JPEG")) || Path::ExtensionMatches(Filename, TEXT("JPG")))
      FileFormat = ImageFormat::JPEG;
    if (Path::ExtensionMatches(Filename, TEXT("TIFF")))
      FileFormat = ImageFormat::TIFF;
    if (Path::ExtensionMatches(Filename, TEXT("WDP")))
      FileFormat = ImageFormat::WDP;
  }

  ComPtr<IWICImagingFactory> pFactory;
  ComPtr<IWICBitmapEncoder> pEncoder;
  ComPtr<IWICBitmapFrameEncode> pBitmapFrame;
  ComPtr<IPropertyBag2> pPropertyBag;
  ComPtr<IWICStream> pStream;

  VerifyComResult(pFactory.CreateInstance(CLSID_WICImagingFactory));
  VerifyComResult(pFactory->CreateStream(&pStream));
  VerifyComResult(pStream->InitializeFromFilename(StringW(Filename), GENERIC_WRITE));

  switch(FileFormat)
  {
  case ImageFormat::BMP:
    VerifyComResult(pFactory->CreateEncoder(GUID_ContainerFormatBmp, NULL, &pEncoder)); break;
  case ImageFormat::PNG:
    VerifyComResult(pFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder)); break;
  case ImageFormat::JPEG:
    VerifyComResult(pFactory->CreateEncoder(GUID_ContainerFormatJpeg, NULL, &pEncoder)); break;
  case ImageFormat::TIFF:
    VerifyComResult(pFactory->CreateEncoder(GUID_ContainerFormatTiff, NULL, &pEncoder)); break;
  case ImageFormat::WDP:
    VerifyComResult(pFactory->CreateEncoder(GUID_ContainerFormatWmp, NULL, &pEncoder)); break;
  };
  
  VerifyComResult(pEncoder->Initialize(pStream, WICBitmapEncoderNoCache));
  
  ComPtr<IWICBitmapEncoderInfo> pEncoderInfo;
  VerifyComResult(pEncoder->GetEncoderInfo(&pEncoderInfo));

  UINT PixelFormatCount = 0;
  VerifyComResult(pEncoderInfo->GetPixelFormats(0, 0, &PixelFormatCount));
  GUID* pPixelFormats = new GUID[PixelFormatCount];
  VerifyComResult(pEncoderInfo->GetPixelFormats(PixelFormatCount, pPixelFormats, &PixelFormatCount));
  WICPixelFormatGUID FormatGUID = ChooseBestPixelFormat(m_PixelFormat, PixelFormatCount, pPixelFormats);
  DWORD OutFormat = ChooseBestPixelFormat(FormatGUID);
  delete[] pPixelFormats;

  VerifyComResult(pEncoder->CreateNewFrame(&pBitmapFrame, &pPropertyBag));
  VerifyComResult(pBitmapFrame->Initialize(pPropertyBag));
  VerifyComResult(pBitmapFrame->SetSize(m_Width, m_Height));
  VerifyComResult(pBitmapFrame->SetPixelFormat(&FormatGUID));
  UINT Stride = GetPixelFormatStride(m_Width, FormatGUID);

  BYTE* pScanLine = new BYTE[Stride];
  for (UINT i=0; i<m_Height; i++)
  {
    ConvertScanLine(pScanLine, OutFormat, (LPBYTE)m_pPixelData + i*m_Width*GetPixelSize(m_PixelFormat), m_PixelFormat, m_Width);
    VerifyComResult(pBitmapFrame->WritePixels(1, Stride, Stride, pScanLine));
  }
  delete[] pScanLine;

  VerifyComResult(pBitmapFrame->Commit());
  VerifyComResult(pEncoder->Commit());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Image::Load(LPCTSTR Filename, DWORD SpecifiedPixelFormat)
{
  ComPtr<IWICImagingFactory> pFactory;
  ComPtr<IWICBitmapDecoder> pDecoder;
  ComPtr<IWICBitmapFrameDecode> pBitmapFrame;

  VerifyComResult(pFactory.CreateInstance(CLSID_WICImagingFactory));
  VerifyComResult(pFactory->CreateDecoderFromFilename(StringW(Filename), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder));
  VerifyComResult(pDecoder->GetFrame(0, &pBitmapFrame));  

  UINT Width, Height;
  VerifyComResult(pBitmapFrame->GetSize(&Width, &Height));
  WICPixelFormatGUID FormatGUID;
  VerifyComResult(pBitmapFrame->GetPixelFormat(&FormatGUID));

  DWORD InputPixelFormat = ChooseBestPixelFormat(FormatGUID);
  DWORD InternalPixelFormat = SpecifiedPixelFormat;
  if (InternalPixelFormat == PixelFormat::UNKNOWN)
    InternalPixelFormat = InputPixelFormat;

  Resize(Width, Height, InternalPixelFormat);

  UINT Stride = GetPixelFormatStride(m_Width, FormatGUID);
  BYTE* pScanLine = new BYTE[Stride];
  for (UINT i=0; i<m_Height; i++)
  {
    WICRect rc;
    rc.X = 0;
    rc.Y = i;
    rc.Width = m_Width;
    rc.Height = 1;
    
    VerifyComResult(pBitmapFrame->CopyPixels(&rc, Stride, Stride, pScanLine));

    ConvertScanLine( (LPBYTE)m_pPixelData + i*m_Width*GetPixelSize(InternalPixelFormat), InternalPixelFormat, pScanLine, InputPixelFormat, m_Width);
  }
  delete[] pScanLine;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Image::GetImagePixelFormat(LPCTSTR Filename)
{
  ComPtr<IWICImagingFactory> pFactory;
  ComPtr<IWICBitmapDecoder> pDecoder;
  ComPtr<IWICBitmapFrameDecode> pBitmapFrame;

  VerifyComResultStatic(pFactory.CreateInstance(CLSID_WICImagingFactory));
  VerifyComResultStatic(pFactory->CreateDecoderFromFilename(StringW(Filename), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder));
  VerifyComResultStatic(pDecoder->GetFrame(0, &pBitmapFrame));  

  WICPixelFormatGUID FormatGUID;
  VerifyComResultStatic(pBitmapFrame->GetPixelFormat(&FormatGUID));

  return ChooseBestPixelFormat(FormatGUID);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#pragma optimize("", off)
VOID Image::ConvertScanLine(LPVOID pOutData, DWORD OutPixelFormat, LPCVOID pInData, DWORD InPixelFormat, UINT Width)
{
  if (OutPixelFormat == PixelFormat::RGBA32 && InPixelFormat == PixelFormat::RGBA32)
    CopyMemory(pOutData, pInData, Width*4);

  if (OutPixelFormat == PixelFormat::RGB32 && InPixelFormat == PixelFormat::RGBA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::RGB24 && InPixelFormat == PixelFormat::RGBA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[4*i];
      pTypedOutData[3*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[4*i+2];
    }
  }

  if (OutPixelFormat == PixelFormat::BGRA32 && InPixelFormat == PixelFormat::RGBA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::BGR32 && InPixelFormat == PixelFormat::RGBA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::BGR24 && InPixelFormat == PixelFormat::RGBA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[3*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[4*i];
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA128F && InPixelFormat == PixelFormat::RGBA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((pTypedInData[4*i+3]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB128F && InPixelFormat == PixelFormat::RGBA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((pTypedInData[4*i+3]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB96F && InPixelFormat == PixelFormat::RGBA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[3*i + 0] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
      pTypedOutData[3*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[3*i + 2] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R8 && InPixelFormat == PixelFormat::RGBA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[1*i + 0] = (BYTE)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R32F && InPixelFormat == PixelFormat::RGBA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[1*i + 0] = (FLOAT)(((BYTE)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f)) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA32 && InPixelFormat == PixelFormat::RGB32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::RGB32 && InPixelFormat == PixelFormat::RGB32)
    CopyMemory(pOutData, pInData, Width*4);

  if (OutPixelFormat == PixelFormat::RGB24 && InPixelFormat == PixelFormat::RGB32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[4*i];
      pTypedOutData[3*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[4*i+2];
    }
  }

  if (OutPixelFormat == PixelFormat::BGRA32 && InPixelFormat == PixelFormat::RGB32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::BGR32 && InPixelFormat == PixelFormat::RGB32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::BGR24 && InPixelFormat == PixelFormat::RGB32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[3*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[4*i];
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA128F && InPixelFormat == PixelFormat::RGB32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((pTypedInData[4*i+3]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB128F && InPixelFormat == PixelFormat::RGB32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((pTypedInData[4*i+3]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB96F && InPixelFormat == PixelFormat::RGB32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[3*i + 0] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
      pTypedOutData[3*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[3*i + 2] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R8 && InPixelFormat == PixelFormat::RGB32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[1*i + 0] = (BYTE)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R32F && InPixelFormat == PixelFormat::RGB32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[1*i + 0] = (FLOAT)(((BYTE)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f)) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA32 && InPixelFormat == PixelFormat::RGB24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[3*i];
      pTypedOutData[4*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[3*i+2];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::RGB32 && InPixelFormat == PixelFormat::RGB24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[3*i];
      pTypedOutData[4*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[3*i+2];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::RGB24 && InPixelFormat == PixelFormat::RGB24)
    CopyMemory(pOutData, pInData, Width*3);

  if (OutPixelFormat == PixelFormat::BGRA32 && InPixelFormat == PixelFormat::RGB24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[3*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[3*i];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::BGR32 && InPixelFormat == PixelFormat::RGB24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[3*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[3*i];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::BGR24 && InPixelFormat == PixelFormat::RGB24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[3*i+2];
      pTypedOutData[3*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[3*i];
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA128F && InPixelFormat == PixelFormat::RGB24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[3*i]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[3*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[3*i+2]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((255) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB128F && InPixelFormat == PixelFormat::RGB24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[3*i]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[3*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[3*i+2]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((255) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB96F && InPixelFormat == PixelFormat::RGB24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[3*i + 0] = (FLOAT)((pTypedInData[3*i]) / 255.0f);
      pTypedOutData[3*i + 1] = (FLOAT)((pTypedInData[3*i+1]) / 255.0f);
      pTypedOutData[3*i + 2] = (FLOAT)((pTypedInData[3*i+2]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R8 && InPixelFormat == PixelFormat::RGB24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[1*i + 0] = (BYTE)(((FLOAT)pTypedInData[3*i] + (FLOAT)pTypedInData[3*i+1] + (FLOAT)pTypedInData[3*i+2]) / 3.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R32F && InPixelFormat == PixelFormat::RGB24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[1*i + 0] = (FLOAT)(((BYTE)(((FLOAT)pTypedInData[3*i] + (FLOAT)pTypedInData[3*i+1] + (FLOAT)pTypedInData[3*i+2]) / 3.0f)) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA32 && InPixelFormat == PixelFormat::BGRA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::RGB32 && InPixelFormat == PixelFormat::BGRA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::RGB24 && InPixelFormat == PixelFormat::BGRA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[3*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[4*i];
    }
  }

  if (OutPixelFormat == PixelFormat::BGRA32 && InPixelFormat == PixelFormat::BGRA32)
    CopyMemory(pOutData, pInData, Width*4);

  if (OutPixelFormat == PixelFormat::BGR32 && InPixelFormat == PixelFormat::BGRA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::BGR24 && InPixelFormat == PixelFormat::BGRA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[4*i];
      pTypedOutData[3*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[4*i+2];
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA128F && InPixelFormat == PixelFormat::BGRA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((pTypedInData[4*i+3]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB128F && InPixelFormat == PixelFormat::BGRA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((pTypedInData[4*i+3]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB96F && InPixelFormat == PixelFormat::BGRA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[3*i + 0] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
      pTypedOutData[3*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[3*i + 2] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R8 && InPixelFormat == PixelFormat::BGRA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[1*i + 0] = (BYTE)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R32F && InPixelFormat == PixelFormat::BGRA32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[1*i + 0] = (FLOAT)(((BYTE)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f)) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA32 && InPixelFormat == PixelFormat::BGR32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::RGB32 && InPixelFormat == PixelFormat::BGR32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::RGB24 && InPixelFormat == PixelFormat::BGR32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[4*i+2];
      pTypedOutData[3*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[4*i];
    }
  }

  if (OutPixelFormat == PixelFormat::BGRA32 && InPixelFormat == PixelFormat::BGR32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::BGR32 && InPixelFormat == PixelFormat::BGR32)
    CopyMemory(pOutData, pInData, Width*4);

  if (OutPixelFormat == PixelFormat::BGR24 && InPixelFormat == PixelFormat::BGR32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[4*i];
      pTypedOutData[3*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[4*i+2];
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA128F && InPixelFormat == PixelFormat::BGR32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((pTypedInData[4*i+3]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB128F && InPixelFormat == PixelFormat::BGR32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((pTypedInData[4*i+3]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB96F && InPixelFormat == PixelFormat::BGR32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[3*i + 0] = (FLOAT)((pTypedInData[4*i+2]) / 255.0f);
      pTypedOutData[3*i + 1] = (FLOAT)((pTypedInData[4*i+1]) / 255.0f);
      pTypedOutData[3*i + 2] = (FLOAT)((pTypedInData[4*i]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R8 && InPixelFormat == PixelFormat::BGR32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[1*i + 0] = (BYTE)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R32F && InPixelFormat == PixelFormat::BGR32)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[1*i + 0] = (FLOAT)(((BYTE)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f)) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA32 && InPixelFormat == PixelFormat::BGR24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[3*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[3*i];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::RGB32 && InPixelFormat == PixelFormat::BGR24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[3*i+2];
      pTypedOutData[4*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[3*i];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::RGB24 && InPixelFormat == PixelFormat::BGR24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[3*i+2];
      pTypedOutData[3*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[3*i];
    }
  }

  if (OutPixelFormat == PixelFormat::BGRA32 && InPixelFormat == PixelFormat::BGR24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[3*i];
      pTypedOutData[4*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[3*i+2];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::BGR32 && InPixelFormat == PixelFormat::BGR24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[3*i];
      pTypedOutData[4*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[3*i+2];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::BGR24 && InPixelFormat == PixelFormat::BGR24)
    CopyMemory(pOutData, pInData, Width*3);

  if (OutPixelFormat == PixelFormat::RGBA128F && InPixelFormat == PixelFormat::BGR24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[3*i+2]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[3*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[3*i]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((255) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB128F && InPixelFormat == PixelFormat::BGR24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[3*i+2]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[3*i+1]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[3*i]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((255) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB96F && InPixelFormat == PixelFormat::BGR24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[3*i + 0] = (FLOAT)((pTypedInData[3*i+2]) / 255.0f);
      pTypedOutData[3*i + 1] = (FLOAT)((pTypedInData[3*i+1]) / 255.0f);
      pTypedOutData[3*i + 2] = (FLOAT)((pTypedInData[3*i]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R8 && InPixelFormat == PixelFormat::BGR24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[1*i + 0] = (BYTE)(((FLOAT)pTypedInData[3*i] + (FLOAT)pTypedInData[3*i+1] + (FLOAT)pTypedInData[3*i+2]) / 3.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R32F && InPixelFormat == PixelFormat::BGR24)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[1*i + 0] = (FLOAT)(((BYTE)(((FLOAT)pTypedInData[3*i] + (FLOAT)pTypedInData[3*i+1] + (FLOAT)pTypedInData[3*i+2]) / 3.0f)) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA32 && InPixelFormat == PixelFormat::RGBA128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[4*i]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[4*i+2]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((pTypedInData[4*i+3]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB32 && InPixelFormat == PixelFormat::RGBA128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[4*i]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[4*i+2]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((pTypedInData[4*i+3]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB24 && InPixelFormat == PixelFormat::RGBA128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = (BYTE)((pTypedInData[4*i]) * 255);
      pTypedOutData[3*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[3*i + 2] = (BYTE)((pTypedInData[4*i+2]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGRA32 && InPixelFormat == PixelFormat::RGBA128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[4*i+2]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[4*i]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((pTypedInData[4*i+3]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGR32 && InPixelFormat == PixelFormat::RGBA128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[4*i+2]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[4*i]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((pTypedInData[4*i+3]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGR24 && InPixelFormat == PixelFormat::RGBA128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = (BYTE)((pTypedInData[4*i+2]) * 255);
      pTypedOutData[3*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[3*i + 2] = (BYTE)((pTypedInData[4*i]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA128F && InPixelFormat == PixelFormat::RGBA128F)
    CopyMemory(pOutData, pInData, Width*16);

  if (OutPixelFormat == PixelFormat::RGB128F && InPixelFormat == PixelFormat::RGBA128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::RGB96F && InPixelFormat == PixelFormat::RGBA128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[4*i];
      pTypedOutData[3*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[4*i+2];
    }
  }

  if (OutPixelFormat == PixelFormat::R8 && InPixelFormat == PixelFormat::RGBA128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[1*i + 0] = (BYTE)(((FLOAT)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f)) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::R32F && InPixelFormat == PixelFormat::RGBA128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[1*i + 0] = (FLOAT)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA32 && InPixelFormat == PixelFormat::RGB128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[4*i]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[4*i+2]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((pTypedInData[4*i+3]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB32 && InPixelFormat == PixelFormat::RGB128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[4*i]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[4*i+2]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((pTypedInData[4*i+3]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB24 && InPixelFormat == PixelFormat::RGB128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = (BYTE)((pTypedInData[4*i]) * 255);
      pTypedOutData[3*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[3*i + 2] = (BYTE)((pTypedInData[4*i+2]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGRA32 && InPixelFormat == PixelFormat::RGB128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[4*i+2]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[4*i]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((pTypedInData[4*i+3]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGR32 && InPixelFormat == PixelFormat::RGB128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[4*i+2]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[4*i]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((pTypedInData[4*i+3]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGR24 && InPixelFormat == PixelFormat::RGB128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = (BYTE)((pTypedInData[4*i+2]) * 255);
      pTypedOutData[3*i + 1] = (BYTE)((pTypedInData[4*i+1]) * 255);
      pTypedOutData[3*i + 2] = (BYTE)((pTypedInData[4*i]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA128F && InPixelFormat == PixelFormat::RGB128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[4*i];
      pTypedOutData[4*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[4*i+2];
      pTypedOutData[4*i + 3] = pTypedInData[4*i+3];
    }
  }

  if (OutPixelFormat == PixelFormat::RGB128F && InPixelFormat == PixelFormat::RGB128F)
    CopyMemory(pOutData, pInData, Width*16);

  if (OutPixelFormat == PixelFormat::RGB96F && InPixelFormat == PixelFormat::RGB128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[4*i];
      pTypedOutData[3*i + 1] = pTypedInData[4*i+1];
      pTypedOutData[3*i + 2] = pTypedInData[4*i+2];
    }
  }

  if (OutPixelFormat == PixelFormat::R8 && InPixelFormat == PixelFormat::RGB128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[1*i + 0] = (BYTE)(((FLOAT)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f)) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::R32F && InPixelFormat == PixelFormat::RGB128F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[1*i + 0] = (FLOAT)(((FLOAT)pTypedInData[4*i] + (FLOAT)pTypedInData[4*i+1] + (FLOAT)pTypedInData[4*i+2]) / 3.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA32 && InPixelFormat == PixelFormat::RGB96F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[3*i]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[3*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[3*i+2]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((1.0f) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB32 && InPixelFormat == PixelFormat::RGB96F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[3*i]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[3*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[3*i+2]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((1.0f) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB24 && InPixelFormat == PixelFormat::RGB96F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = (BYTE)((pTypedInData[3*i]) * 255);
      pTypedOutData[3*i + 1] = (BYTE)((pTypedInData[3*i+1]) * 255);
      pTypedOutData[3*i + 2] = (BYTE)((pTypedInData[3*i+2]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGRA32 && InPixelFormat == PixelFormat::RGB96F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[3*i+2]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[3*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[3*i]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((1.0f) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGR32 && InPixelFormat == PixelFormat::RGB96F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[3*i+2]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[3*i+1]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[3*i]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((1.0f) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGR24 && InPixelFormat == PixelFormat::RGB96F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = (BYTE)((pTypedInData[3*i+2]) * 255);
      pTypedOutData[3*i + 1] = (BYTE)((pTypedInData[3*i+1]) * 255);
      pTypedOutData[3*i + 2] = (BYTE)((pTypedInData[3*i]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA128F && InPixelFormat == PixelFormat::RGB96F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[3*i];
      pTypedOutData[4*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[3*i+2];
      pTypedOutData[4*i + 3] = 1.0f;
    }
  }

  if (OutPixelFormat == PixelFormat::RGB128F && InPixelFormat == PixelFormat::RGB96F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[3*i];
      pTypedOutData[4*i + 1] = pTypedInData[3*i+1];
      pTypedOutData[4*i + 2] = pTypedInData[3*i+2];
      pTypedOutData[4*i + 3] = 1.0f;
    }
  }

  if (OutPixelFormat == PixelFormat::RGB96F && InPixelFormat == PixelFormat::RGB96F)
    CopyMemory(pOutData, pInData, Width*12);

  if (OutPixelFormat == PixelFormat::R8 && InPixelFormat == PixelFormat::RGB96F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[1*i + 0] = (BYTE)(((FLOAT)(((FLOAT)pTypedInData[3*i] + (FLOAT)pTypedInData[3*i+1] + (FLOAT)pTypedInData[3*i+2]) / 3.0f)) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::R32F && InPixelFormat == PixelFormat::RGB96F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[1*i + 0] = (FLOAT)(((FLOAT)pTypedInData[3*i] + (FLOAT)pTypedInData[3*i+1] + (FLOAT)pTypedInData[3*i+2]) / 3.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA32 && InPixelFormat == PixelFormat::R8)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[i];
      pTypedOutData[4*i + 1] = pTypedInData[i];
      pTypedOutData[4*i + 2] = pTypedInData[i];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::RGB32 && InPixelFormat == PixelFormat::R8)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[i];
      pTypedOutData[4*i + 1] = pTypedInData[i];
      pTypedOutData[4*i + 2] = pTypedInData[i];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::RGB24 && InPixelFormat == PixelFormat::R8)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[i];
      pTypedOutData[3*i + 1] = pTypedInData[i];
      pTypedOutData[3*i + 2] = pTypedInData[i];
    }
  }

  if (OutPixelFormat == PixelFormat::BGRA32 && InPixelFormat == PixelFormat::R8)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[i];
      pTypedOutData[4*i + 1] = pTypedInData[i];
      pTypedOutData[4*i + 2] = pTypedInData[i];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::BGR32 && InPixelFormat == PixelFormat::R8)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[i];
      pTypedOutData[4*i + 1] = pTypedInData[i];
      pTypedOutData[4*i + 2] = pTypedInData[i];
      pTypedOutData[4*i + 3] = 255;
    }
  }

  if (OutPixelFormat == PixelFormat::BGR24 && InPixelFormat == PixelFormat::R8)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[i];
      pTypedOutData[3*i + 1] = pTypedInData[i];
      pTypedOutData[3*i + 2] = pTypedInData[i];
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA128F && InPixelFormat == PixelFormat::R8)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[i]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[i]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[i]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((255) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB128F && InPixelFormat == PixelFormat::R8)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = (FLOAT)((pTypedInData[i]) / 255.0f);
      pTypedOutData[4*i + 1] = (FLOAT)((pTypedInData[i]) / 255.0f);
      pTypedOutData[4*i + 2] = (FLOAT)((pTypedInData[i]) / 255.0f);
      pTypedOutData[4*i + 3] = (FLOAT)((255) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB96F && InPixelFormat == PixelFormat::R8)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[3*i + 0] = (FLOAT)((pTypedInData[i]) / 255.0f);
      pTypedOutData[3*i + 1] = (FLOAT)((pTypedInData[i]) / 255.0f);
      pTypedOutData[3*i + 2] = (FLOAT)((pTypedInData[i]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::R8 && InPixelFormat == PixelFormat::R8)
    CopyMemory(pOutData, pInData, Width*1);

  if (OutPixelFormat == PixelFormat::R32F && InPixelFormat == PixelFormat::R8)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPBYTE pTypedInData = (LPBYTE)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[1*i + 0] = (FLOAT)((pTypedInData[i]) / 255.0f);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA32 && InPixelFormat == PixelFormat::R32F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((1.0f) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB32 && InPixelFormat == PixelFormat::R32F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((1.0f) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGB24 && InPixelFormat == PixelFormat::R32F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[3*i + 1] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[3*i + 2] = (BYTE)((pTypedInData[i]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGRA32 && InPixelFormat == PixelFormat::R32F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((1.0f) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGR32 && InPixelFormat == PixelFormat::R32F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[4*i + 0] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 1] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 2] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[4*i + 3] = (BYTE)((1.0f) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::BGR24 && InPixelFormat == PixelFormat::R32F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[3*i + 0] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[3*i + 1] = (BYTE)((pTypedInData[i]) * 255);
      pTypedOutData[3*i + 2] = (BYTE)((pTypedInData[i]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::RGBA128F && InPixelFormat == PixelFormat::R32F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[i];
      pTypedOutData[4*i + 1] = pTypedInData[i];
      pTypedOutData[4*i + 2] = pTypedInData[i];
      pTypedOutData[4*i + 3] = 1.0f;
    }
  }

  if (OutPixelFormat == PixelFormat::RGB128F && InPixelFormat == PixelFormat::R32F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[4*i + 0] = pTypedInData[i];
      pTypedOutData[4*i + 1] = pTypedInData[i];
      pTypedOutData[4*i + 2] = pTypedInData[i];
      pTypedOutData[4*i + 3] = 1.0f;
    }
  }

  if (OutPixelFormat == PixelFormat::RGB96F && InPixelFormat == PixelFormat::R32F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPFLOAT pTypedOutData = (LPFLOAT)pOutData;
      pTypedOutData[3*i + 0] = pTypedInData[i];
      pTypedOutData[3*i + 1] = pTypedInData[i];
      pTypedOutData[3*i + 2] = pTypedInData[i];
    }
  }

  if (OutPixelFormat == PixelFormat::R8 && InPixelFormat == PixelFormat::R32F)
  {
    for (UINT i=0; i<Width; i++)
    {
      LPFLOAT pTypedInData = (LPFLOAT)pInData;
      LPBYTE pTypedOutData = (LPBYTE)pOutData;
      pTypedOutData[1*i + 0] = (BYTE)((pTypedInData[i]) * 255);
    }
  }

  if (OutPixelFormat == PixelFormat::R32F && InPixelFormat == PixelFormat::R32F)
    CopyMemory(pOutData, pInData, Width*4);

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#pragma optimize("", on)
