/*
    Simple resource manager and base resource type (pure template class)
*/
#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <filesystem>

#define IMPL_RESOURCE(ResourceType) \
public:\
friend class XRHI;\
protected:\
void MakeKey()override\
{\
    Reference = '`';\
    Reference += FilePath;\
    Reference += '`';\
    Reference += #ResourceType;\
    Reference += "'";\
    Reference += std::filesystem::path(FilePath).stem().generic_string();\
\
    std::hash<std::string> szHash;\
    ID = szHash(Reference);\
}\
const std::string GetTypeString() const override { return #ResourceType; };

//Wrapper
namespace flora
{
    enum class EFileType;

    template<typename TBase, EFileType TFile>
    struct XImportSetting :public TBase
    {
        using BaseType = TBase;
        //using Type = TFile;
    };

    template<typename TBase, EFileType TFile>
    struct XExportSetting :public TBase
    {
        using BaseType = TBase;
        //using Type = TFile;
    };

    template<typename TSettings, typename TResource>
    struct XImporter
    {
        static std::shared_ptr<TResource> Exec(const std::string& Filepath, const TSettings& Settings);
    };

    template<typename TSettings, typename TResource>
    struct XExporter
    {
        static bool Exec(TResource& Resource, const std::string& Filepath, const TSettings& Settings);
    };
}

//template resource base type and resource manager
namespace flora
{
    typedef size_t Guid;

#ifndef DEFAULT_RESOURCE_MANAGER_TYPE
    #define DEFAULT_RESOURCE_MANAGER_TYPE XSimpleResourceManager
    class XSimpleResourceManager;
#else
    class DEFAULT_RESOURCE_MANAGER_TYPE;
#endif

    template<typename TResourceManager = DEFAULT_RESOURCE_MANAGER_TYPE>
    class IResource :public virtual std::enable_shared_from_this<IResource<TResourceManager>>
    {
    public:
        using ResourceManagerType = TResourceManager;
        using BaseType = IResource<ResourceManagerType>;
    protected:
        IResource() = default;
        IResource(const std::string& InFilePath) :FilePath(InFilePath) {  }

        IResource(const IResource&) = delete;
        IResource(IResource&&) = delete;

        void operator=(const IResource&) = delete;
        void operator=(IResource&&) = delete;
    public:
        virtual ~IResource() = default;
        virtual const std::string GetTypeString() const = 0;

        void Register()
        {
            MakeKey();
            auto& ResourceManager = ResourceManagerType::Get();
            if (!ResourceManager.Find(ID))
            {
                ResourceManager.ResourceMap.insert({ ID,this->shared_from_this() });
            }
        }

        void UnRegister()
        {
            auto& ResourceManager = ResourceManagerType::Get();
            if (ResourceManager.Find(ID))
            {
                ResourceManager.ResourceMap.erase(ID);
            }
        }

        template<template<typename, typename> class TImporter,typename TSettings>
        static auto Open(const std::string& Filepath, const TSettings & Settings = TSettings())
        {
            return TImporter<TSettings, IResource<TResourceManager>::BaseType>::Exec(Filepath, Settings);
        }

        template<template<typename,typename> class TExporter,typename TSettings,typename TResourceType>
        static bool Save(TResourceType& Resource,const std::string& Filepath, const TSettings & Settings = TSettings())
		{
			return TExporter<TSettings, TResourceType>::Exec(Resource,Filepath, Settings);
		}

        template<typename T>
        T* DynamicCast()
        {
            return dynamic_cast<T*>(this);
        }

        template<typename T>
        std::shared_ptr<T> DynamicPointerCast()
        {
            return std::dynamic_pointer_cast<T>(this->shared_from_this());
        }

		template<typename T>
		T* StaticCast()
		{
			return static_cast<T*>(this);
		}

        template<typename T>
        std::shared_ptr<T> StaticPointerCast()
        {
            return std::static_pointer_cast<T>(this->shared_from_this());
        }

		template<typename T>
		T* ReinterpretCast()
		{
			return reinterpret_cast<T*>(this);
		}
    public:
        uint32_t GetID()const { return ID; }
        std::string GetRef()const { return Reference; }
        std::string GetFilePath()const { return FilePath; }
    protected:
        virtual void MakeKey() = 0;
    protected:
        uint32_t ID;//Primary Key
        
        std::string Reference = "";//Primary Key(string)

        std::string FilePath;//Import path
    };

    class XSimpleResourceManager
    {
        friend class IResource<XSimpleResourceManager>;
    protected:
        XSimpleResourceManager() {}

    public:
        static XSimpleResourceManager& Get() noexcept(std::is_nothrow_constructible<XSimpleResourceManager>::value)
        {
            //Thread save singleton
            static XSimpleResourceManager instance;
            return instance;
        }

        virtual ~XSimpleResourceManager() noexcept = default;
        XSimpleResourceManager(const XSimpleResourceManager&) = delete;
        XSimpleResourceManager& operator=(const XSimpleResourceManager&) = delete;
    public:
        /// <summary>
        /// Find resource
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        IResource<XSimpleResourceManager>* Find(Guid UID)
        {
            IResource<XSimpleResourceManager>* Ret = nullptr;

            auto Iter = ResourceMap.find(UID);
            if (Iter != ResourceMap.end())
            {
                Ret = (*Iter).second.get();
            }

            return Ret;
        }

    protected:
        std::unordered_map<Guid, std::shared_ptr<IResource<XSimpleResourceManager>>> ResourceMap;
    };
}