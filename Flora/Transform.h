#pragma once
#include "Core.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
using glm::vec3;
using glm::mat4;
class FLORA_API FTransform
{
public:
	//right hand coordinate (defines root coordinate system)
	const static inline vec3 WorldCoordRight  = vec3{ 1.,0.,0. };//px
	const static inline vec3 WorldCoordUp	  = vec3{ 0.,1.,0. };//py
	const static inline vec3 WorldCoordFront  = vec3{ 0.,0.,1. };//pz
	const static inline vec3 WorldCoordOrigin = vec3{ 0.,0.,0. };//o

	const inline vec3& GetLocalPosition() { return LocalPosition; };
	const inline vec3& GetLocalRotation() { return LocalRotation; };
	const inline vec3& GetLocalScale()	  { return LocalScale   ; };

	const inline vec3& GetWorldPosition() { return TransformToRoot * glm::vec4(LocalPosition, 1.0f); };
	const inline vec3& GetWorldRotation() { return TransformToRoot * glm::vec4(LocalRotation, 1.0f); };
	const inline vec3& GetWorldScale()    { return TransformToRoot * glm::vec4(LocalScale   , 1.0f); };

	inline mat4& GetModelMatrix()		  { return TransformToRoot; };
	inline mat4& GetLocalModelMatrix()	  { return TransformToParent; };
	inline mat4& MatrixToLocalSpace()	  { return InvTransformToRoot; };
public:
	void Update(const FTransform& Parent);
	void ToLocalSpace(vec3& v);
public:
	void MoveWorldForward(float ds);
	void MoveWorldBack(float ds);
	void MoveWorldLeft(float ds);
	void MoveWorldRight(float ds);
	void MoveWorldUp(float ds);
	void MoveWorldDown(float ds);

	void MoveWorldX(float dx);
	void MoveWorldY(float dy);
	void MoveWorldZ(float dz);
	void MoveWorld(vec3 ds);

	void MoveToWorldPos(vec3 WorldPos);
public:
	void MoveLocalForward(float ds);
	void MoveLocalBack(float ds);
	void MoveLocalLeft(float ds);
	void MoveLocalRight(float ds);
	void MoveLocalUp(float ds);
	void MoveLocalDown(float ds);

	void MoveLocalX(float dx);
	void MoveLocalY(float dy);
	void MoveLocalZ(float dz);
	void MoveLocal(vec3 ds);

	void MoveToLocalPos(vec3 LocalPos);
public:
	void RotateToLocal(vec3 Angle);
	void RotateLocal(vec3 dAngle);
private:
	vec3 LocalPosition = vec3{ 0,0,0 };
	vec3 LocalRotation = vec3{ 0,0,0 };
	vec3 LocalScale    = vec3{ 1,1,1 };

	mat4 TransformToParent = mat4(1.0f);
	mat4 TransformToRoot = mat4(1.0f);
	mat4 InvTransformToRoot = mat4(1.0f);
};

