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
	inline Ref<FCameraObject> GetMainCamera()  { return MainCamera; };

	void SetMainCamera(Ref<FCameraObject>);
public:
	template<typename TSubClass>
	inline Ref<FCameraObject> SpawnCamera(const char* Name)
	{
		uint32_t Id = ObjManager.NextId();
		ObjManager.RegisterObj(EGameObjectType_CameraObject, Id,
			std::make_shared<TSubClass>(Name, this,Id));
		return std::dynamic_pointer_cast<FCameraObject>(ObjManager.GetGameObjectByID(Id));
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
	struct FLORA_API FObjectManager
	{
	private:
		//all objects
		vector<Ref<FGameObject>> GameObjects;

		//category
		unordered_map<EGameObjectType, vector<uint32_t>> GameObjectsMap;
	public:
		uint32_t NextId();
		bool	 IsValidId(uint32_t);

		bool	 RegisterObj(EGameObjectType, uint32_t, Ref<FGameObject>);

		Ref<FGameObject> GetGameObjectByID(uint32_t);
		Ref<FGameObject> GetGameObjectByID(EGameObjectType,uint32_t);

		bool RemoveObj(uint32_t);
		bool RemoveObj(Ref<FGameObject>);
	};

	FObjectManager ObjManager;
private:
	//for render scene
	Ref<FCameraObject> MainCamera = nullptr;
};

