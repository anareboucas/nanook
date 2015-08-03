/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Graphics/OpenGL/Light.cpp $
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




#include "CodeLibrary.hpp"

#if defined(USE_OPENGL)

#include "Graphics/OpenGL/Light.hpp"
#include "Graphics/OpenGL/Context.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Graphics;
using namespace Bolitho::Graphics::OpenGL;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics::OpenGL,Light,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Light::Light( UINT LightId )
{
	m_LightId = GL_LIGHT0 + LightId;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Light::SetAmbient( CONST ColorF& C )
{
	glLightfv( m_LightId, GL_AMBIENT, C );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Light::SetDiffuse( CONST ColorF& C )
{
	glLightfv( m_LightId, GL_DIFFUSE, C );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Light::SetSpecular( CONST ColorF& C )
{
	glLightfv( m_LightId, GL_SPECULAR, C );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Light::SetPosition( CONST Vector4<FLOAT>& V )
{
	glLightfv( m_LightId, GL_POSITION, V );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Light::SetPosition( CONST Vector3<FLOAT>& V )
{
	SetPosition( Vector4<FLOAT>(V[0], V[1], V[2], 1 ) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Light::SetDirection( CONST Math::Vector3<FLOAT>& V )
{
	glLightfv( m_LightId, GL_SPOT_DIRECTION, V );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Light::Enable()
{
	glEnable( m_LightId );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Light::Disable()
{
	glDisable( m_LightId );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#endif