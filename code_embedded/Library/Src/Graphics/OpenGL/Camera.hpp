/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Graphics/OpenGL/Camera.hpp $
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




#pragma once

#include "UI/Window.hpp"
#include "Utility/SizePointRect.hpp"
#include "Math/Vector3.hpp"
#include "Math/Matrix4.hpp"

namespace Bolitho
{
	namespace Graphics
	{
		namespace OpenGL
		{
			class LIBRARY_API Camera : public Object
			{
				DeclareRuntimeType();
			private:
				Point m_MousePosition;
				WPARAM m_MouseState;

				BOOL m_DoSpin;
				Math::Vector3<FLOAT> m_SpinCenter;
				Math::Vector3<FLOAT> m_SpinAxis;
				FLOAT m_SpinSpeed;

				FLOAT m_FieldOfView;
				FLOAT m_PixelScale;

				FLOAT m_SurfaceDepth;
				FLOAT m_ClickDepth;
				
				PointF m_TrackballPosition;
				FLOAT m_TrackballScreenSize;

				Math::Vector3<FLOAT> m_SceneCenter;
				FLOAT m_SceneSize;

				Math::Matrix4<FLOAT> m_View;
											
				BOOL ReadDepth(Point P, Math::Vector3<FLOAT>& V) CONST;

				VOID StartSpin();
				Math::Vector3<FLOAT> MouseToTrackball(PointF P);

				VOID Rotate(Point P, Math::Matrix4<FLOAT>& xf);
				VOID MoveXY(Point P, Math::Matrix4<FLOAT>& xf);
				VOID MoveZ(Point P, Math::Matrix4<FLOAT>& xf);
				
				//VOID Wheel(UINT updown, Math::Matrix4<FLOAT>& xf);
				
				VOID MouseClick(Point P);

			public:
				VOID SetFieldOfView(FLOAT FieldOfView);

				LRESULT HandleMessage( Ptr<UI::Window> pWindow, UINT Message, WPARAM wParam, LPARAM lParam );

				//VOID Mouse(Point P, UINT b, CONST Math::Vector3<FLOAT>& SceneCenter, FLOAT SceneSize, Math::Matrix4<FLOAT>& xf);
				
        //VOID StopSpin();
				//BOOL AutoSpin(Math::Matrix4<FLOAT>& xf);

				VOID Initialize(CONST Math::Vector3<FLOAT>& SceneCenter, FLOAT SceneSize);
				VOID ResetView();

				CONST Math::Matrix4<FLOAT>& GetViewMatrix() CONST
				{ return m_View; }
				VOID SetViewMatrix( CONST Math::Matrix4<FLOAT>& M )
				{ m_View = M; }
				
				Camera(FLOAT FieldOfView = 0.7f);
				~Camera() {}

			};
		}
	}
}
