/**
* @brief Shapes Classes
* @file shapes.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/07/2012
*
*/

#include "shapes.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign; 

namespace s9{

	GeometryFullFloat makeQuad(float_t w, float_t h) {
		
		vector<size_t> indices;
		vector<float_t> verts;
		vector<float_t> texcoords;
		vector<float_t> colours;
		vector<float_t> normals;
		
		// Quad for Camera drawing
		indices += 0,3,1,3,2,1;
		verts += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w, h,0.0f,
			0.0f, h,0.0f;
			
		normals += 0.0f,0.0f,1.0f,
			0.0f, 0.0f,1.0f, 
			0.0f, 0.0f,1.0f,
			0.0f, 0.0f,1.0f;
		
		texcoords += 0.0, h,
			w, h,
			w, 0.0,
			0.0,0.0;
		
		colours += 1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f;
		
		GeometryFullFloat geom(verts,normals,texcoords,colours);
		
		geom.addIndex(indices);

		return geom;
	}

	
	GeometryFullFloat makeTriangle(float_t w, float_t h){
		
		vector<size_t> indices;
		vector<float_t> verts;
		vector<float_t> texcoords;
		vector<float_t> colours;
		vector<float_t> normals;
		
		// Quad for Camera drawing
		verts += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w/2, h,0.0f;
			
		normals += 0.0f,0.0f,1.0f,
			0.0f, 0.0f,1.0f, 
			0.0f, 0.0f,1.0f;

		texcoords += 0.0, h,
			w, 0.0,
			w/2, h;

		colours += 1.0f,0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f;
			
		GeometryFullFloat geom(verts,normals,texcoords,colours);
		
		return geom;
	}
	
}
