/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Graphics/Image.hpp $
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

#pragma once

#include "IO/File.hpp"
#include "IO/Path.hpp"
#include "Graphics/Color.hpp"
#include "Lua/Binding.hpp"
#include "Com/Guid.hpp"
#include "Com/ComException.hpp"
#include "Com/ComPointer.hpp"
#include "IO/Path.hpp"
#include "Math/Histogram.hpp"

namespace Bolitho
{
  namespace Graphics
  {
    class LIBRARY_API ImageException : public UserException
    {
      DeclareRuntimeType();
    public:
      ImageException( CONST Object* Sender, LPCTSTR Description ) : UserException(Sender)
      {
        m_Description = CharTraits<TCHAR>::Duplicate(Description);
      }

      ImageException( CONST ImageException& E ) : UserException(E)
      {
        m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
      }

      virtual ~ImageException()
      {
        if (m_Description)
          FreeMemory(m_Description);
        m_Description = 0;
      }

      virtual String Description() CONST
      {
        return m_Description;
      }

    private:
      TCHAR* m_Description;
    };

    /* The format of an image container on disk */
    struct ImageFormat 
    { 
      enum 
      { 
        UNKNOWN = 0x00000000, 
        BMP, 
        PNG, 
        JPEG, 
        TIFF, 
        WDP 
      };
    };

    class Image : public Object
    {
      DeclareRuntimeType();
    public:
      /* Constructs an empty image */
      Image();
      /* Construct a Width*Height image, with PixelFormat */
      Image(UINT Width, UINT Height, DWORD PixelFormat);
      /* Construct a Width*Height image, with PixelFormat.  Copy data from pImageData */
      Image(UINT Width, UINT Height, DWORD PixelFormat, LPVOID pImageData);
      /* Construct an image as a copy of another image */
      Image(CONST Image& I);
      /* Construct an image by loading Filename from disk */
      Image(LPCTSTR Filename);
      /* Construct an image by loading Filename from disk with PixelFormat */
      Image(LPCTSTR Filename, DWORD PixelFormat);

      virtual ~Image();

      Image& operator = (CONST Image& I);

      /* Resizes the image, retaining existing the pixel format */
      VOID Resize(UINT Width, UINT Height);
      /* Resizes the image, selecting a new pixel format */
      VOID Resize(UINT Width, UINT Height, DWORD PixelFormat);

      /* Returns the width of the image */
      UINT Width() CONST
      { return m_Width; }
      /* Returns the height of the image */
      UINT Height() CONST
      { return m_Height; }
      /* Returns format of a pixel */
      DWORD PixelFormat() CONST
      { return m_PixelFormat; }

      /* Returns the size of a single pixel, in bytes */
      UINT GetPixelSize() CONST
      { return GetPixelSize(m_PixelFormat); }
      /* Returns the number of color channels */
      UINT GetPixelChannels() CONST
      { return GetPixelChannels(m_PixelFormat); }

      /* Converts to a new pixel format */
      VOID SetPixelFormat(DWORD PixelFormat);

      /* Pixel access */
      template<class P>
      CONST P& Pixel(UINT i, UINT j) CONST;
      template<class P>
      P& Pixel(UINT i, UINT j);

      /* Saves the image on disk with a certain container and pixel format */
      VOID Save(LPCTSTR Filename, DWORD ContainerFormat = ImageFormat::UNKNOWN, DWORD PixelFormat = PixelFormat::UNKNOWN) CONST;
      /* Loads an image from disk with a specified pixel format */
      VOID Load(LPCTSTR Filename, DWORD PixelFormat = PixelFormat::UNKNOWN);

      /* Returns the native pixel format of an image on disk */
      static DWORD GetImagePixelFormat(LPCTSTR Filename);

      #if defined(USE_LUA)
      /* Generate Lua script bindings for this class */
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
      #endif

      /* Returns the size (in bytes) of a pixel in a given PixelFormat */
      static UINT GetPixelSize(DWORD PixelFormat)
      { return (PixelFormat & 0xff) / 8; }
      /* Returns the number of color channels of a pixel in a given PixelFormat */
      static UINT GetPixelChannels(DWORD PixelFormat)
      { return (PixelFormat & 0xf0000000) >> 28; }

      /* Clamps all color values in the image */
      template<class P>
      VOID Clamp();
      template<class P>
      VOID Clamp(typename P::ColorChannelType Low, typename P::ColorChannelType High);

      /* Normalizes all color values */
      template<class P>
      VOID Normalize();

      template<class P>
      Math::Histogram1<typename P::ColorChannelType> GetColorDistribution(UINT Bins);



    protected:

      /* Converts a scanline of pixels from one format to another */
      static VOID ConvertScanLine(LPVOID pOutData, DWORD OutPixelFormat, LPCVOID pInData, DWORD InPixelFormat, UINT Width);

      /* The width of the image, in pixels */
      UINT m_Width;
      /* The height of the image, in pixels */
      UINT m_Height;
      /* The format of the pixel data */
      DWORD m_PixelFormat;
      /* The pixel data */
      LPVOID m_pPixelData;
    private:
          
    };

    template<class PixelType>
    class TImage : public Image
    {
      DeclareRuntimeType();
    public:
      TImage();
      TImage(UINT Width, UINT Height);
      TImage(UINT Width, UINT Height, CONST PixelType* pImageData);
      TImage(CONST Image& I);
      TImage(LPCTSTR Filename);

      TImage& operator= (CONST Image& I);

      operator CONST PixelType* () CONST;
      operator PixelType* ();

      CONST PixelType& operator() (UINT i, UINT j) CONST;
      PixelType& operator() (UINT i, UINT j);

      /* Loads an image from disk with a specified pixel format */
      VOID Load(LPCTSTR Filename);

      VOID Normalize()
      { Image::Normalize<PixelType>(); }
      
      VOID Clamp()
      { Image::Clamp<PixelType>(); }
      VOID Clamp(typename PixelType::ColorChannelType Low, typename PixelType::ColorChannelType High)
      { Image::Clamp<PixelType>(Low, High); }
      
      TImage& operator += (CONST TImage& Image);
      TImage& operator += (PixelType X);

      TImage& operator -= (CONST TImage& Image);
      TImage& operator -= (PixelType X);

      TImage& operator *= (CONST TImage& Image);
      TImage& operator *= (PixelType X);
      TImage& operator *= (FLOAT X);

      TImage& operator /= (CONST TImage& Image);
      TImage& operator /= (PixelType X);
      TImage& operator /= (FLOAT X);

    private:
      VOID SetPixelFormat(DWORD PixelFormat)
      { Image::SetPixelFormat(PixelFormat); }

    };

		typedef TImage<ColorRGBA32> ImageRGBA32;
		typedef TImage<ColorRGB32> ImageRGB32;
		typedef TImage<ColorRGB24> ImageRGB24;
		typedef TImage<ColorBGRA32> ImageBGRA32;
		typedef TImage<ColorBGR32> ImageBGR32;
		typedef TImage<ColorBGR24> ImageBGR24;
		typedef TImage<ColorRGBA128F> ImageRGBA128F;
		typedef TImage<ColorRGB128F> ImageRGB128F;
    typedef TImage<ColorR8> ImageR8;
    typedef TImage<ColorR32F> ImageR32F;

    typedef ImageBGRA32 ImageB;
		typedef ImageRGBA128F ImageF;

    typedef ImageR8 ImageLumB;
    typedef ImageR32F ImageLumF;

		#include "ImageImpl.hpp"
  }
}