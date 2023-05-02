#pragma once
#include "GameObject.h"
#include "CameraObject.h"
using std::string;
class FLORA_API FScene:public std::enable_shared_from_this<FScene>,public ISerialization
{
	friend class FSceneManager;
public:
	FScene(const string& name);
	~FScene() = default;

	inline const string&	  GetName()const   { return SceneName; };
	inline Ref<FCameraObject> GetDebugCamera() { return DebugCamera; };
public:
	template<typename TSubClass>
	inline void SpawnDebugCamera(const char* Name)
	{
		DebugCamera = make_shared<TSubClass>(Name, this);
	};
public:
	void Rename(const string&);
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;
private:
	void BeginUpdate();
	void Update();
	void EndUpdate();

	void Awake();
	void Release();
	void ProcessInputEvent();
private:
	string SceneName;
private:
	Ref<FCameraObject> DebugCamera;

	//total objects
	unordered_map<uint32_t, Ref<FGameObject>> GameObjects;


};

