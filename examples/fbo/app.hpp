/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef FBOAPP_HPP
#define FBOAPP_HPP

#include "s9/s9gear.hpp"
#include "s9/common.hpp"
#include "s9/gl/shapes.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw_app.hpp"
#include "s9/gl/fbo.hpp"

#include <anttweakbar/AntTweakBar.h>

 
namespace s9 {

	/*
 	 * An application that shows how to deal with an FBO and draw to the screen
 	 */

	class FBOApp : public VisualApp{
	public:
		void init();
		void display(double_t dt);

		// Event handling - you can choose which to override
		void fireEvent(MouseEvent e);
		void fireEvent(KeyboardEvent e);
		void fireEvent(ResizeEvent e);
		
	protected:
		gl::Quad mTestQuad;
		gl::Quad mHudQuad;
		gl::Shader mShader;
		gl::Shader mFBOShader;
		gl::FBO mFBO;

		InertiaCam<OrbitCamera> mCamera;
		ScreenCamera mScreenCamera;
		
	};
}

#endif
