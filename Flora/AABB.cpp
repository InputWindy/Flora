#include "AABB.h"
using namespace std;
using glm::mat4;

void FAABB::AddPoint(const vec3& Point)
{
	Min.x = min(Min.x, Point.x);
	Min.y = min(Min.y, Point.y);
	Min.z = min(Min.z, Point.z);

	Max.x = max(Max.x, Point.x);
	Max.y = max(Max.y, Point.y);
	Max.z = max(Max.z, Point.z);

	Center = Min + Max;
	Center.x /= 2;
	Center.y /= 2;
	Center.z /= 2;

	Extend = glm::abs(Max - Min);
	Extend.x /= 2;
	Extend.y /= 2;
	Extend.z /= 2;
};

void FAABB::Merge(const FAABB& Other)
{
	Min.x = min(Min.x, Other.Min.x);
	Min.y = min(Min.y, Other.Min.y);
	Min.z = min(Min.z, Other.Min.z);

	Max.x = max(Max.x, Other.Max.x);
	Max.y = max(Max.y, Other.Max.y);
	Max.z = max(Max.z, Other.Max.z);

	Center = Min + Max;
	Center.x /= 2;
	Center.y /= 2;
	Center.z /= 2;

	Extend = glm::abs(Max - Min);
	Extend.x /= 2;
	Extend.y /= 2;
	Extend.z /= 2;
}

glm::mat4 FAABB::GetTransform()const
{
	mat4 ret =
	{
		Extend.x,0,0,0,
		0,Extend.y,0,0,
		0,0,Extend.z,0,
		Center.x,Center.y,Center.z,1
	};
	return std::move(ret);
}
;