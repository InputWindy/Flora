#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
Ref<FCamera> FCamera::Generate(ECameraType type)
{
	switch (type)
	{
	case ECameraType::Perspective: return std::make_shared<FPerspectiveCamera>();
	case ECameraType::Orthographic: return std::make_shared<FOrthographicCamera>();
	case ECameraType::MAX:
		break;
	default:
		break;
	}
	assert(0,"Undefined Camera Type!");
	return nullptr;
}

FPerspectiveCamera::FPerspectiveCamera()
{
	Type = ECameraType::Perspective;
}

void FPerspectiveCamera::Update()
{
	Projection = glm::perspective(FOV, AspectRatio, NearClip, FarClip);
}

FOrthographicCamera::FOrthographicCamera()
{
	Type = ECameraType::Orthographic;
}

void FOrthographicCamera::Update()
{
	Projection = glm::ortho(Left, Right, Bottom, Top, NearClip, FarClip);
}

template<>
FPerspectiveCamera* FCamera::DynamicCast<FPerspectiveCamera>()
{
	return dynamic_cast<FPerspectiveCamera*>(this);
}

template<>
FOrthographicCamera* FCamera::DynamicCast<FOrthographicCamera>()
{
	return dynamic_cast<FOrthographicCamera*>(this);
}