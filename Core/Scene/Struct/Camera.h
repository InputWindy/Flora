#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Common/Common.h>
#include <Common/Serialize.h>

namespace XVerse
{
	using glm::vec3;
	using glm::mat4;
	using std::shared_ptr;

	enum class ECameraType
	{
		CT_PERSPECTIVE,
		CT_ORTHOGRAPHIC,

		CT_MAX_COUNT
	};

	class XPerspectiveCamera;
	class XOrthographicCamera;

	struct XFrustum
	{
		//ortho
		XFrustum(float l, float r, float b, float t, float n, float f);

		//perspective
		XFrustum(float fov, float aspect, float near, float far);

		glm::vec3 Corners[8] = {};
		glm::vec4 Planes[6] = {};
	};

	class XCamera :public ISerialization
	{
	protected:
		XCamera() = default;
	public:
		virtual ~XCamera() = default;

	public:
		virtual ECameraType GetType()const = 0;

		inline glm::mat4 ProjectionMatrix()const { return Projection; }

		virtual void Update() = 0;

		virtual XFrustum GetFrustum()const = 0;
	protected:
		glm::mat4 Projection = glm::identity<mat4>();
	};

	class XOrthographicCamera :public XCamera
	{
	public:
		XOrthographicCamera() { Update(); }
		virtual ~XOrthographicCamera() = default;

		virtual ECameraType GetType()const { return ECameraType::CT_ORTHOGRAPHIC; }
	public:
		inline float GetLeftClipPlane()const { return Left; }
		inline float GetRightClipPlane()const { return Right; }
		inline float GetBottonClipPlane()const { return Bottom; }
		inline float GetTopClipPlane()const { return Top; }
		inline float GetNearClipPlane()const { return NearClip; }
		inline float GetFarClipPlane()const { return FarClip; }

		inline void SetLeftClipPlane(float f) { Left = f; }
		inline void SetRightClipPlane(float f) { Right = f; }
		inline void SetBottonClipPlane(float f) { Bottom = f; }
		inline void SetTopClipPlane(float f) { Top = f; }
		inline void SetNearClipPlane(float f) { NearClip = f; }
		inline void SetFarClipPlane(float f) { FarClip = f; }

		virtual void Update() final;
		virtual XFrustum GetFrustum()const final;
	public:
		virtual bool Parse(const json11::Json&);
		virtual bool Serialize(json11::Json&);
	protected:
		//define frustum
		float Left = 30.f;
		float Right = 30.f;
		float Bottom = 30.f;
		float Top = 30.f;
		float NearClip = 0.1f;
		float FarClip = 1000.0f;
	};

	class XPerspectiveCamera :public XCamera
	{
	public:
		XPerspectiveCamera() { Update(); }
		virtual ~XPerspectiveCamera() = default;

		virtual ECameraType GetType()const { return ECameraType::CT_PERSPECTIVE; }

		inline float GetFOV()const { return Fov; }
		inline float GetAspectRatio()const { return AspectRatio; }
		inline float GetNearClipPlane()const { return NearClip; }
		//inline float GetFarClipPlane()const { return FarClip; }

		inline void SetFOV(float f) { Fov = f; Update(); }
		inline void SetAspectRatio(float f) { AspectRatio = f; Update(); }
		inline void SetNearClipPlane(float f) { NearClip = f; Update(); }
		inline void SetFarClipPlane(float f) { FarClip = f; Update(); }

		virtual void Update() final;
		virtual XFrustum GetFrustum()const final;
	public:
		virtual bool Parse(const json11::Json&);
		virtual bool Serialize(json11::Json&);
	protected:
		//define frustum
		float Fov = 45.0f;
		float AspectRatio = 1.778f;
		float NearClip = 0.1f;
		float FarClip = -1.f;
	};
}