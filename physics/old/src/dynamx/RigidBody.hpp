#ifndef  RIGIDBODY_H
#define  RIGIDBODY_H

#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>

#include "SDL/SDL_opengl.h" 

#include <dynamx/Core.hpp>
#include <dynamx/Point3.hpp>
#include <dynamx/Vector3.hpp>
#include <dynamx/Matrix3.hpp>
#include <dynamx/Matrix4.hpp>
#include <dynamx/RigidBodyGeometry.hpp>
#include <dynamx/ObjLoader.hpp>
#include <dynamx/Vertex.hpp>
#include <dynamx/Face.hpp>
#include <dynamx/AABB.hpp>
#include <dynamx/OdeSolver.hpp>

using namespace std;
using namespace boost;

namespace dynamx
{
	/*
	 * =====================================================================================
	 *        Class: RigidBody
	 *  Description:
	 * =====================================================================================
	 */
	class RigidBody
	{
		public:
			RigidBody();

			virtual ~RigidBody();

			void Integrate(real timestep);

			void StateToArray(real* y);

			void ArrayToState(real* y);

			void CalculateInternals();

			void ClearAccumulators();

			void AddForceAtBodyPoint(const Vector3& force, const Point3& point);

			void AddForceAtPoint(const Vector3& force, const Point3& point);

			void TransformInertiaTensor(Matrix3* iitWorld
					, const Matrix3& iitBody
					, const Matrix3& rotMat) const;

			//Util fns.
			void GetPointInLocalSpace(const Point3& point, Point3* result) const;
			void GetPointInWorldSpace(const Point3& point, Point3* result) const;

			//Drawing functions
			void LoadModel(std::string filename);
			void DrawMesh();
			virtual void VDraw();

			/*-----------------------------------------------------------------------------
			 *  Getters and Setters
			 *-----------------------------------------------------------------------------*/
			Point3 GetPos() const { return m_Pos; }
			//Matrix3& GetOrientation() { return m_Orientation; }
			Quaternion& GetOrientation() { return m_Orientation; }
			RigidBodyGeometry GetTransformedGeom() const { return m_TransformedGeom; }
			Vector3 GetLinearVel() const { return m_LinearVel; }
			Vector3 GetAngularVel() const { return m_AngularVel; }
			shared_ptr<AABB> GetAABB() { return m_AABB; }
			Matrix3 GetInverseInertiaTensor() { return m_InverseInertiaTensor; }
			Matrix3 GetInverseInertiaTensorWorld() { return m_InverseInertiaTensorWorld; }
			real GetInverseMass() const { return m_InverseMass; }
			//XXX:Fix this
			real GetMass() const { if(m_InverseMass==0){ return 1000000000; }else {return 1/m_InverseMass;}}
			Vector3 GetForceAccum() const { return m_ForceAccum; }
			Vector3 GetTorqueAccum() const { return m_TorqueAccum; }
			void SetForceAccum(const Vector3 val) { m_ForceAccum = val; }
			void SetTorqueAccum(const Vector3 val) { m_TorqueAccum = val; }

//			void SetOrientation(const Matrix3& val) { m_Orientation = val; }
			void SetOrientation(const Quaternion& val) { m_Orientation = val; }
			void SetLinearVel(const Vector3& val) { m_LinearVel = val; }
			void SetPos(const Point3& val) { m_Pos = val; }
			void SetAngularVel(const Vector3& val) { m_AngularVel = val; }

			/* 
			Vector3 GetV() const { return m_V; }
			void SetV(const Vector3& val) { m_V = val; }
			Vector3 GetOmega() const { return m_Omega; }
			void SetOmega(const Vector3& val) { m_Omega = val; }
			*/

			Vector3 GetAngularMomentum() const
			{
				return m_AngularMomentum;
			}

			void SetAngularMomentum(const Vector3& val)
			{
				m_AngularMomentum = val;
			}

			Vector3 GetLinearMomentum() const
			{
				return m_LinearMomentum;
			}

			void SetLinearMomentum(const Vector3& val)
			{
				m_LinearMomentum = val;
			}

			//These are a bit different, we are setting their inverses as they are more useful.
			void SetMass(real mass) { assert(mass>0); m_InverseMass = 1/mass; }
			void SetInertiaTensor(const Matrix3& val) { val.GetInverse(&m_InverseInertiaTensor); }

			void CreateList();

		protected:
			//State vals
			Point3 m_Pos; //x(t)
//			Matrix3 m_Orientation;
			Quaternion m_Orientation; //R(t)
			Vector3 m_LinearVel; //v(t)
			Vector3 m_AngularVel; //w(t)

			//Derived quantities
			Vector3 m_LinearMomentum;
			Vector3 m_AngularMomentum;
//			Vector3 m_Omega;
//			Vector3 m_V;

			real m_InverseMass;

			//Inverse of inertia tensor
			Matrix3 m_InverseInertiaTensor;

			//Internal vals calculated at every loop, basically cached values.
			Matrix4 m_Transform;
			Matrix3 m_InverseInertiaTensorWorld;

			//Will use these to work out Linear and angular momentum at the next integration step
			Vector3 m_ForceAccum; //P(t)
			Vector3 m_TorqueAccum; //L(t)

			//Damping
			real m_LinearDamping;
			real m_AngularDamping;

			//Geometry
			RigidBodyGeometry m_Geom;
			RigidBodyGeometry m_TransformedGeom;

			//Collision Detection
			//Broadphase
			shared_ptr<AABB> m_AABB;

			//Debug
			bool m_DebugDraw;

			//Drawing
			GLuint m_RigidBodyList;

			Vector3 m_ConstantAccel;

			OdeSolver m_Solver;

		private:
	};
}

#endif   // RIGIDBODY_H
