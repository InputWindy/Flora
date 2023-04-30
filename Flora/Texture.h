#pragma once
#include <string>
#include "Core.h"
#include "Type.h"
#include "Serialization.h"
#include "Resource.h"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

struct FLORA_API FImage:public std::enable_shared_from_this<FImage>,public ISerialization
{
	friend class FTexture;
	friend class FOpenGLTexture;
public:
	FImage() = default;
	FImage(const std::string& root, const std::string& relative, bool b_hdr = false, bool b_flip = false);
	~FImage();

	void Load(bool b_hdr = false, bool b_flip = false);
	void Free();

	inline std::string GetRootPath()const { return root_path; };
	inline std::string GetDirectory()const { return directory; };
	inline std::string GetRelativePath()const { return relative_path; };
	inline std::string GetName()const { return name; };
	inline std::string GetExtension()const { return extension; };
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;
private:
	//=====================//
	// ¢Ùfull_path = root_path + directory + name + extension
	// ¢Úfull_path = root_path + relative_path
	//=====================//
	std::string root_path;
	std::string directory;
	std::string relative_path;
	std::string name;
	std::string extension;
private:
	void* data = nullptr;
	bool  hdr = false;
	bool  flip = false;
	int   w = 0;
	int   h = 0;
	int   comp = 0;
};

struct FLORA_API FTextureInfo
{
	//=============================//
	//		Inner Data Desc        //
	//=============================//
	uint8_t MaxMipLevel = 0;
	uint8_t MinMipLevel = 0;

	EWrapMode WrapModeR = EWrapMode_REPEAT;
	EWrapMode WrapModeS = EWrapMode_REPEAT;
	EWrapMode WrapModeT = EWrapMode_REPEAT;

	EFilterMode MinFilterMode = EFilterMode_LINEAR;
	EFilterMode MagFilterMode = EFilterMode_LINEAR;

	vec4 BorderColor = { 0,0,0,0 };
};

//.ftexture
class FLORA_API FTexture :public std::enable_shared_from_this<FTexture>,public ISerialization,public IResource
{
	friend class FOpenGLInterface;
protected:
	FTexture();
public:
	virtual ~FTexture() = default;

	static inline Ref<FTexture> GetWhite()   { return White; };
	static inline Ref<FTexture> GetBlack()   { return Black; };
	static inline Ref<FTexture> GetDefault() { return Default; };
public:
	//=======================//
	//		get	& set		 //
	//=======================//
	inline const string&	GetName()			 const { return Name; };
	inline uint32_t			GetWidth()			 const { return Width; };
	inline uint32_t			GetHeight()			 const { return Height; };
	inline uint32_t			GetDepth()			 const { return Depth; };
	inline ETextureTarget	GetTextureType()	 const { return TextureTarget; };
	inline EInternalFormat	GetInternalFormat()	 const { return InternalFormat; };

	//=======================//
	//		Texture Info	 //
	//=======================//
	inline uint8_t			GetMinMipLevel()const { return Info.MinMipLevel; };
	inline uint8_t			GetMaxMipLevel()const { return Info.MaxMipLevel; };

	inline EWrapMode        GetWrapModeR()const { return Info.WrapModeR; };
	inline EWrapMode        GetWrapModeS()const { return Info.WrapModeS; };
	inline EWrapMode        GetWrapModeT()const { return Info.WrapModeT; };

	inline EFilterMode      GetMinFilterMode()const { return Info.MinFilterMode; };
	inline EFilterMode      GetMagFilterMode()const { return Info.MagFilterMode; };

	inline vec4				GetBorderColor()const { return Info.BorderColor; };

	inline void	SetMinMipLevel(uint8_t level) { Info.MinMipLevel = level; };
	inline void	SetMaxMipLevel(uint8_t level) { Info.MaxMipLevel = level; };
						
	inline void SetWrapModeR(EWrapMode mode) { Info.WrapModeR = mode; };
	inline void SetWrapModeS(EWrapMode mode) { Info.WrapModeS = mode; };
	inline void SetWrapModeT(EWrapMode mode) { Info.WrapModeT = mode; };
							
	inline void SetMinFilterMode(EFilterMode mode) { Info.MinFilterMode = mode; };
	inline void SetMagFilterMode(EFilterMode mode) { Info.MagFilterMode = mode; };

	inline void	SetBorderColor(vec4 color) { Info.BorderColor = color; };
public:
	inline bool IsLoaded()const { return Image != nullptr; };
public:
	virtual uint32_t GetHandle()	const = 0;
	virtual uint32_t GetShortCut()	const = 0;

	virtual void Update() = 0;
	virtual void SetImageData(Ref<FImage> img) = 0;

	//cube map can not be serialized and displayed in the editor now ( but it work )
	[[deprecated]] virtual void SetCubeImageData(Ref<FImage> img, ETextureTarget target = ETextureTarget_MAX) = 0;
public:
	virtual bool Parse(IN FJson&)		final;
	virtual bool Serialize(OUT FJson&)	final;

	virtual void Register()final;
	virtual void Rename(const string& name) final;
protected:
	std::string  Name = "";

	uint32_t	 Width  = 0;
	uint32_t	 Height = 0;

	uint32_t	 Depth  = 0;

	ETextureTarget	TextureTarget  = ETextureTarget_2D;
	EInternalFormat InternalFormat = EInternalFormat_RGBA32F;

	FTextureInfo Info;

	uint32_t MultisampleNum = 4;
protected:
	Ref<FImage> Image;
protected:
	static inline Ref<FTexture> White   = nullptr;
	static inline Ref<FTexture> Black   = nullptr;
	static inline Ref<FTexture> Default = nullptr;
};