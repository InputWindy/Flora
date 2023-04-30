#pragma once
#include "Transform.h"
#include <vector>
#include <memory>
enum class ECameraType
{
	Perspective,
	Orthographic,

	MAX
};

class FLORA_API FCamera
{
protected:
	FCamera() = default;
public:
	virtual ~FCamera() = default;
	
	template<typename T>
	T* DynamicCast();
	
	static Ref<FCamera> Generate(ECameraType Type);
	
public:
	inline ECameraType GetType() { return Type; };
	inline const auto& GetProjection()const { return Projection; };

	inline void SetNearClip(float nc) { NearClip = nc; };
	inline void SetFarClip(float fc) { FarClip = fc; };

	inline float GetNearClip()const { return NearClip; };
	inline float GetFarClip()const { return FarClip; };
public:
	virtual void Update() = 0;
protected:
	glm::mat4   Projection = glm::mat4(1.0f);
	ECameraType Type = ECameraType::MAX;

	float NearClip = 0.1f, FarClip = 10000.0f;
};


class FLORA_API FPerspectiveCamera :public FCamera
{
public:
	FPerspectiveCamera();
	virtual ~FPerspectiveCamera() = default;

	virtual void Update()final;
public:
	inline float GetFOV()const { return FOV; };
	inline float GetAspectRatio()const { return AspectRatio; };

	inline void SetFOV(float fov) { FOV = fov; };
	inline void SetAspectRatio(float asr) { AspectRatio = asr; };
private:
	float FOV = 45.0f, AspectRatio = 1.778f;
};

class FLORA_API FOrthographicCamera :public FCamera
{
public:
	FOrthographicCamera() = default;
	virtual ~FOrthographicCamera() = default;

	virtual void Update()final;
public:
	inline float GetLeft()const { return Left; };
	inline float GetRight()const { return Right; };
	inline float GetBottom()const { return Bottom; };
	inline float GetTop()const { return Top; };

	inline void GetLeft(float l) { Left = l; };
	inline void GetRight(float r) { Right = r; };
	inline void GetBottom(float b) { Bottom = b; };
	inline void GetTop(float t) { Top = t; };
private:
	float Left = 30.f, Right = 30.f, Bottom = 30.f, Top = 30.f;
};


