#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Common/Serialize.h>

namespace flora
{
	struct XTransform:public ISerialization
	{
		/*UE World Coordinate
					   z
					   |
					   |       ↑
					   |	    | Up
					   |	    |------→
					   |	   /  Front
					   * -----/-------------x
					  /      /
					 /      / Right
					/     ↙
				   /
				 y

			left-handed rotation

			PX:0,0,0
			NX:0,0,-180
			PY:0,0,90
			NY:0,0,-90
			PZ:0,90,0
			NZ:0,-90,0
		*/
		XTransform() = default;
		XTransform(glm::vec3 InPos) :LocalPosition(InPos) {}

		//right hand coordinate (defines root coordinate system)
		const static inline glm::vec3 WorldRight() { return glm::vec3{ 0.,1.,0. }; };//py
		const static inline glm::vec3 WorldUp() { return glm::vec3{ 0.,0.,1. }; };//pz
		const static inline glm::vec3 WorldFront() { return glm::vec3{ 1.,0.,0. }; };//px
		const static inline glm::vec3 WorldOrigin() { return glm::vec3{ 0.,0.,0. }; };//o

		//const static inline glm::vec3 WorldRight() { return glm::vec3{ 1.,0.,0. } }//py
		//const static inline glm::vec3 WorldUp() { return glm::vec3{ 0.,1.,0. } }//pz
		//const static inline glm::vec3 WorldFront() { return glm::vec3{ 0.,0.,-1. } }//px
		//const static inline glm::vec3 WorldOrigin() { return glm::vec3{ 0.,0.,0. } }//o

		glm::mat4 ModelMatrix()const;
		glm::mat4 ViewMatrix()const;//view matrix
		glm::mat4 LookAtMatrix(const glm::vec3& LookAtPos)const;

		void LookAtPX();
		void LookAtNX();
		void LookAtPY();
		void LookAtNY();
		void LookAtPZ();
		void LookAtNZ();


		inline glm::vec3 GetFrontVector()const { return Front; }
		inline glm::vec3 GetRightVector()const { return Right; }
		inline glm::vec3 GetUpVector()const { return Up; }
			   
		inline glm::vec3 GetLocalPosition()const { return LocalPosition; }
		inline glm::vec3 GetLocalRotation()const { return LocalRotation; }
		inline glm::vec3 GetLocalScale()const { return LocalScale; }

		inline glm::vec3& GetLocalPosition() { return LocalPosition; }
		inline glm::vec3& GetLocalRotation() { return LocalRotation; }
		inline glm::vec3& GetLocalScale() { return LocalScale; }
			  
		inline XTransform& SetPosition(glm::vec3 InPos) { LocalPosition = InPos; return *this; }
		//row pitch yaw	   
		inline XTransform& SetRotation(glm::vec3 InRot) { LocalRotation = InRot;UpdateLocalVectors(); return *this; }
		inline XTransform& SetScale(glm::vec3 InScale) { LocalScale = InScale; return *this; }

		inline float GetMovementSpeed()const { return MovementSpeed; }
		inline float GetMouseSensitivity()const { return MouseSensitivity; }

		inline float& GetMovementSpeed() { return MovementSpeed; }
		inline float& GetMouseSensitivity() { return MouseSensitivity; }

		inline XTransform& SetMovementSpeed(float F) { MovementSpeed = F; return *this; }
		inline XTransform& SetMouseSensitivity(float F) { MouseSensitivity = F; return *this; }

	public:
		virtual void MoveForward(float deltaTime);
		virtual void MoveBackward(float deltaTime);
		virtual void MoveLeft(float deltaTime);
		virtual void MoveRight(float deltaTime);
		virtual void MoveUp(float deltaTime);
		virtual void MoveDown(float deltaTime);
		virtual void Rotate(float deltax, float deltay, float deltaz, bool constrainYaw = true);
	public:
		virtual bool Parse(const json11::Json& In);
		virtual bool Serialize(json11::Json& Out);
	public:
		void UpdateLocalVectors();
	private:
		//XTransform settings
		float MovementSpeed = 2.5f;
		float MouseSensitivity = 0.1f;

		glm::vec3 Front = XTransform::WorldFront();
		glm::vec3 Right = XTransform::WorldRight();
		glm::vec3 Up = XTransform::WorldUp();
		
		glm::vec3 LocalPosition = glm::vec3{ 0,0,0 };
		glm::vec3 LocalRotation = glm::vec3{ 0,0,0 };//row pitch yaw
		glm::vec3 LocalScale = glm::vec3{ 1,1,1 };
	};

}