/**
* @brief Geometry Classes
* @file geom.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 16/07/2012
*
*/


#ifndef S9_GEOMETRY_HPP
#define S9_GEOMETRY_HPP

#include "common.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertex_types.hpp"

namespace s9 {
	
	/*
	 * Base Geometry. Used by the primitive
	 * \todo rather than use a dirty flag, register a listener or similar. Make implicit!
	 */
	 
	class DrawableGeometry {
	public:
		virtual bool isDirty(){ return false;}
		virtual void setDirty(bool b) {}
		virtual void* addr() {return NULL; }
		virtual bool isIndexed(){return false; }
		virtual uint32_t* indexaddr(){return NULL; }
		virtual uint32_t size() {return 0;}
		virtual uint32_t indexsize() {return 0;}
	};
	
	

	/*
	 * Basic Geometry Class - Passed up to the primitive. Created in various ways
	 * This is essentially a collection of types of vertices with addition and deletion
	 * taken care of. Likely, this will be expanded to take into account PCL Meshes 
	 * and similar and will become subclassed or specific
	 * 
	 * This class uses a single, flat array of types, interleaved
	 */
	
	template <class T>
	class Geometry : public DrawableGeometry{
	public:
		Geometry() {};
		
		Geometry(std::vector<glm::vec3> v, std::vector<glm::vec3> n) {};
		Geometry (std::vector<float_t> v, std::vector<float_t> n) {};
		Geometry(std::vector<float_t> v, std::vector<float_t> n, std::vector<float_t> t, std::vector<float_t> c) {};

	protected:
	
		struct SharedObj {
			std::vector<T> vBuffer;
			std::vector<uint32_t> vIndices;
			bool mDirty;
		};
		
		boost::shared_ptr<SharedObj> mObj;
	
	public:
		
		Geometry(std::vector<T> v) {
			mObj.reset(new SharedObj());
			mObj->vBuffer = v;
		};

		void createEmpty() {mObj.reset(new SharedObj()); };

		template<class U>
		U convert() {};

		std::vector<T> getBuffer() {return mObj->vBuffer; };
	
		virtual operator int() const { return mObj.use_count() > 0; };
	
		void* addr() { return &(mObj->vBuffer[0]); };
		uint32_t* indexaddr() { return &(mObj->vIndices[0]); };
		
		uint32_t size() { return mObj->vBuffer.size(); };
		uint32_t indexsize() { return mObj->vIndices.size(); };
		int elementsize() { return sizeof(T); };
		
		bool isIndexed() {return mObj->vIndices.size() > 0; };
		bool isDirty() {return mObj->mDirty;};
		void setDirty(bool b) {mObj->mDirty = b; };
		std::vector<uint32_t>  getIndices() {return mObj->vIndices; };
		void addVertex(T v) {mObj->push_back(v); setDirty(true); };
		void setVertex(T v, uint32_t p) { mObj->vBuffer[p] = v; setDirty(true); };
		void delVertex(uint32_t p) { mObj->vBuffer.erase( mObj->vBuffer.begin() + p); setDirty(true); };
		void addIndices(std::vector<uint32_t> idx) { mObj->vIndices = idx; };
	
	};




	// Specialist contructors for speed - basically, zipping from different vectors
	
	template<>
	inline Geometry<VertPNG>::Geometry(std::vector<glm::vec3> v, std::vector<glm::vec3> n) {
		mObj.reset(new SharedObj());
		
		if (v.size() != n.size()) { std::cerr << "S9Gear - Counts do not match" << std::endl; throw; return; }
		
		for (uint32_t i=0; i < v.size(); ++i){
			VertPNG png = {v[i],n[i]};
			mObj->vBuffer.push_back( png );
		}
	}
	
	template<>
	inline Geometry<VertPNF>::Geometry (std::vector<float_t> v, std::vector<float_t> n) {
		mObj.reset(new SharedObj());
		
		if (v.size() != n.size()) { std::cerr << "S9Gear - Counts do not match" << std::endl; throw; return; }
		
		for (uint32_t i=0; i < v.size(); i+=3){
			Float3 a = {v[i],v[i+1],v[i+2]}; 
			Float3 b = {n[i],n[i+1],n[i+2]};
			VertPNF p = {a,b}; 
			mObj->vBuffer.push_back(p);	
		}
	}
	

	template<>
	inline Geometry<VertPNCTF>::Geometry(std::vector<float_t> v, std::vector<float_t> n, std::vector<float_t> t, std::vector<float_t> c) {
		mObj.reset(new SharedObj());
		///\todo add size checking heres
		
		for (uint32_t i=0; i < v.size() / 3; i++){
			Float3 v0 = {v[i*3],v[i*3+1],v[i*3+2]};
			Float3 v1 = {n[i*3],n[i*3+1],n[i*3+2]};
			Float4 v2 = {c[i*4],c[i*4+1],c[i*4+2],c[i*4+3]};
			Float2 v3 = {t[i*2],t[i*2+1]};
			
			VertPNCTF p = {v0,v1,v2,v3};
			mObj->vBuffer.push_back(p);
		
		}
	}

	// Conversion templates - probaby inefficient ><

	/*
	 * Conversion from basic to 8 texture parameters - Quite specific
	 */
	
	template <>
	template <>
	inline Geometry<VertPNT8F> Geometry<VertPNF>::convert() {

		std::vector<VertPNT8F> vtemp;
		std::vector<VertPNF> vstart = this->mObj->vBuffer;
		for (std::vector<VertPNF>::iterator it = vstart.begin(); it != vstart.end(); it++) {
			VertPNF p = *it;
			VertPNT8F tp;
			tp.mP = p.mP;
			tp.mN = p.mN;

			for (int i = 0; i < 8; ++i){
				Float2 tt; tt.x = 0.0f; tt.y = 0.0f;
				tp.mT[i] = tt;
			}
			vtemp.push_back(tp);
		}

		Geometry<VertPNT8F> b (vtemp);
		b.addIndices(getIndices());

		return b;
	}

	/*
	 * Conversion from basic to full
	 */

	template <>
	template <>
	inline Geometry<VertPNCTF> Geometry<VertPNF>::convert() {
		std::vector<VertPNCTF> vtemp;
		std::vector<VertPNF> vstart = this->mObj->vBuffer;
		for (std::vector<VertPNF>::iterator it = vstart.begin(); it != vstart.end(); it++) {
			VertPNF p = *it;
			VertPNCTF tp;
			tp.mP = p.mP;
			tp.mN = p.mN;
			tp.mC.x = tp.mC.y = tp.mC.z = 0.5f;
			tp.mC.w = 1.0f;
			tp.mT.x = tp.mT.y = 0.0f;
			vtemp.push_back(tp);
		}

		Geometry<VertPNCTF> b (vtemp);
		b.addIndices(getIndices());
		return b;
	}

	// Handy Typedefs
	
	typedef Geometry<VertPNCTF> GeometryFullFloat;
	typedef Geometry<VertPNCTG> GeometryFullGLM;
	typedef Geometry<VertPNF> GeometryPNF;
	typedef Geometry<VertPNT8F> GeometryPNT8F;

}


#endif
