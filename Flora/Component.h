#pragma once
#include "Core.h"
#include "Serialization.h"
#include "Camera.h"
#include "Animation.h"
#include "Skeleton.h"
#include "Transform.h"
#include "Mesh.h"
#include <memory>
#include <functional>
#include <vector>
using std::vector;
using std::function;

enum class EComponentType
{
	Mesh,
	Animation,
	Camera,
	Transform,
	Script,

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
	inline T* DynamicCast() 
	{
		return dynamic_cast<T*>(this);
	};
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
	inline float GetMovementSpeed()const { return MovementSpeed; };
	inline float GetMouseSensitivity()const { return MouseSensitivity; };
	inline float GetMouseWheelSensitivity()const { return MouseWheelSensitivity; };
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;
private:
	float MovementSpeed = 2.5f;
	float MouseSensitivity = 0.1f;
	float MouseWheelSensitivity = 0.1f;

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

class FLORA_API FScriptComponent :public FComponent
{
	friend class FCameraObject;
	friend class FGameObject;
	using OnAwakeCallback   = std::function<void(void)>;
	using OnReleaseCallback = std::function<void(void)>;
	using OnUpdateCallback  = std::function<void(float)>;
public:
	FScriptComponent(FGameObject*);
	virtual ~FScriptComponent() = default;

	void BindAwakeCallback(OnAwakeCallback);
	void BindReleaseCallback(OnReleaseCallback);
	void BindUpdateCallback(OnUpdateCallback);
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;
private:
	OnAwakeCallback   OnAwake;
	OnReleaseCallback OnRelease;
	OnUpdateCallback  OnUpdate;
};

class FLORA_API FAnimationComponent :public FComponent
{
	friend class FGameObject;
public:
	FAnimationComponent(FGameObject*);
	virtual ~FAnimationComponent() = default;

	void AddAnimation();
	void ResetAnimation(const string&,Ref<FAnimation>);
	Ref<FAnimation>& operator[](const string&);
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;
private:
	unordered_map<string, Ref<FAnimation>> AnimationMap;
	Ref<FSkeleton> Skeleton = nullptr;
};

class FLORA_API FMeshComponent :public FComponent
{
	friend class FGameObject;
public:
	FMeshComponent(FGameObject*);
	virtual ~FMeshComponent() = default;

	Ref<FMesh>& operator[](uint32_t);
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;
private:
	vector<Ref<FMesh>> Meshes;
};



