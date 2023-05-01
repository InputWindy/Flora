#pragma once
#include "Core.h"
#include "Serialization.h"
#include "Camera.h"
#include "Transform.h"
#include <memory>

enum class EComponentType
{
	Mesh,
	Animation,
	Camera,
	Transform,
	Skeleton,
	Input,

	Max
};

class FGameObject;
class FLORA_API FComponent :public std::enable_shared_from_this<FComponent>, public ISerialization
{
protected:
	FComponent(FGameObject*,EComponentType);
public:
	virtual ~FComponent() = default;
public:
	inline EComponentType GetType() const { return Type; };
	inline void Enable(bool Enable) { bEnable = Enable; };

	template<typename T>
	T* DynamicCast();
public:
	virtual bool Parse(IN FJson&) = 0;
	virtual bool Serialize(OUT FJson&) = 0;
protected:
	EComponentType Type    = EComponentType::Max;
	FGameObject*   Owner   = nullptr;
	bool		   bEnable = true;
};

class FLORA_API FCameraComponent :public FComponent
{
public:
	FCameraComponent(FGameObject*);
	virtual ~FCameraComponent() = default;

	void SetCameraType(ECameraType);
	inline FCamera* GetCamera() { return Camera.get(); };
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;
private:
	Scope<FCamera> Camera;
};

class FLORA_API FTransformComponent :public FComponent
{
public:
	FTransformComponent(FGameObject*);
	virtual ~FTransformComponent() = default;

	inline FTransform& GetTransform() { return Transform; };

	void Update(const FTransform& Parent);
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;
private:
	FTransform Transform;
};

