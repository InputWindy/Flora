#include <Scene/Struct/Camera.h>
#include <limits>
#include <Common/Log.h>

using namespace glm;
using namespace XVerse;

void XVerse::XOrthographicCamera::Update()
{
	Projection = glm::ortho(Left, Right, Bottom, Top, NearClip, FarClip);
}

XFrustum XVerse::XOrthographicCamera::GetFrustum() const
{
	return XFrustum(Left, Right, Bottom, Top, NearClip, FarClip);
}

bool XVerse::XOrthographicCamera::Parse(const json11::Json& In)
{
	Left = In["Left"].number_value();
	Right = In["Right"].number_value();
	Bottom = In["Bottom"].number_value();
	Top = In["Top"].number_value();
	NearClip = In["NearClip"].number_value();
	FarClip = In["FarClip"].number_value();
	Update();
	return true;
}

bool XVerse::XOrthographicCamera::Serialize(json11::Json& Out)
{
	Out = json11::Json::object
	{
		{ "Left", Left},
		{ "Right", Right},
		{ "Bottom", Bottom},
		{ "Top", Top},
		{ "NearClip", NearClip},
		//{ "FarClip", FarClip},
	};
	return true;
}

void XVerse::XPerspectiveCamera::Update()
{
	if (FarClip < 0)
		Projection = glm::infinitePerspective(glm::radians(Fov), AspectRatio, NearClip);
	else
	{
		Projection = glm::perspective(glm::radians(Fov), AspectRatio, NearClip, FarClip);
	}
}

XFrustum XVerse::XPerspectiveCamera::GetFrustum() const
{
	return XFrustum(Fov, AspectRatio, NearClip, FarClip);
}

bool XVerse::XPerspectiveCamera::Parse(const json11::Json& In)
{
	Fov = In["Fov"].number_value();
	AspectRatio = In["AspectRatio"].number_value();
	NearClip = In["NearClip"].number_value();
	//FarClip = In["FarClip"].number_value();
	Update();
	return true;
}

bool XVerse::XPerspectiveCamera::Serialize(json11::Json& Out)
{
	Out = json11::Json::object
	{
		{ "Fov", Fov},
		{ "AspectRatio", AspectRatio },
		{ "NearClip", NearClip},
		//{ "FarClip", FarClip},
	};
	return true;
}

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <iostream>

// 定义一个平面结构
struct Plane 
{
	glm::vec3 normal;
	float distance;

	Plane() {};

	Plane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) 
	{
		normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
		distance = -glm::dot(normal, p1);
	}

	vec4 ToVec4()
	{
		return vec4(normal, distance);
	}
};

// 计算视锥体的顶点
void CalculateFrustumVertices(float fov, float aspect, float near, float far, glm::vec3* vertices) 
{
	float tanHalfFov = tan(fov / 2.0f);

	// 近平面
	float nearHeight = 2.0f * tanHalfFov * near;
	float nearWidth = nearHeight * aspect;

	// 远平面
	float farHeight = 2.0f * tanHalfFov * far;
	float farWidth = farHeight * aspect;

	// 近平面顶点
	vertices[0] = glm::vec3( -nearWidth / 2.0f, -nearHeight / 2.0f,near);
	vertices[1] = glm::vec3(  nearWidth / 2.0f, -nearHeight / 2.0f,near);
	vertices[2] = glm::vec3(  nearWidth / 2.0f,  nearHeight / 2.0f,near);
	vertices[3] = glm::vec3( -nearWidth / 2.0f, nearHeight / 2.0f, near);

	// 远平面顶点
	vertices[4] = glm::vec3(-farWidth / 2.0f, -farHeight / 2.0f,far);
	vertices[5] = glm::vec3( farWidth / 2.0f, -farHeight / 2.0f,far);
	vertices[6] = glm::vec3( farWidth / 2.0f,  farHeight / 2.0f,far);
	vertices[7] = glm::vec3(-farWidth / 2.0f,  farHeight / 2.0f, far);
}

// 计算视锥体的平面
void CalculateFrustumPlanes(const glm::vec3* vertices, vec4* out) 
{
	Plane planes[6] = {};

	planes[0] = Plane(vertices[0], vertices[1], vertices[2]);
	planes[1] = Plane(vertices[5], vertices[4], vertices[7]);
	planes[2] = Plane(vertices[4], vertices[0], vertices[3]);
	planes[3] = Plane(vertices[1], vertices[5], vertices[6]);
	planes[4] = Plane(vertices[3], vertices[2], vertices[6]);
	planes[5] = Plane(vertices[4], vertices[5], vertices[1]);

	out[0] = planes[0].ToVec4();
	out[1] = planes[1].ToVec4();
	out[2] = planes[2].ToVec4();
	out[3] = planes[3].ToVec4();
	out[4] = planes[4].ToVec4();
	out[5] = planes[5].ToVec4();
}

XVerse::XFrustum::XFrustum(float l, float r, float b, float t, float n, float f)
{
	XVERSE_CORE_ERROR("Undefined Ortho Frustum");
	throw std::runtime_error("Undefined Ortho Frustum");
}

XVerse::XFrustum::XFrustum(float fov, float aspect, float near, float far)
{
	CalculateFrustumVertices(fov,aspect,near,far,Corners);
	CalculateFrustumPlanes(Corners, Planes);
}
