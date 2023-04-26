#pragma once
#include <string>
#include "Core.h"
#include "Type.h"
#include "Serialization.h"
#include "Resource.h"

using namespace std;

struct FLORA_API FImage
{
	friend class FOpenGLTexture;
public:
	FImage(const std::string& root, const std::string& relative);
	~FImage();

	void Load(bool b_hdr = false, bool b_flip = false);
	void Free();

	inline std::string GetRootPath()const { return root_path; };
	inline std::string GetDirectory()const { return directory; };
	inline std::string GetRelativePath()const { return relative_path; };
	inline std::string GetName()const { return name; };
	inline std::string GetExtension()const { return extension; };
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
	uint8_t MaxMipLevel = 0;		//max mip level
	uint8_t MinMipLevel = 0;		//max mip level

	EWrapMode WrapModeR = EWrapMode_REPEAT;	//sample method when uv is out of range
	EWrapMode WrapModeS = EWrapMode_REPEAT;	//sample method when uv is out of range
	EWrapMode WrapModeT = EWrapMode_REPEAT;	//sample method when uv is out of range

	EFilterMode MinFilterMode = EFilterMode_LINEAR;
	EFilterMode MagFilterMode = EFilterMode_LINEAR;
};

//.ftexture
class FLORA_API FTexture :public std::enable_shared_from_this<FTexture>,public ISerialization,public IResource
{
public:
	static Ref<FTexture> Generate(const char*, uint16_t, uint16_t, uint16_t, ETextureTarget, EInternalFormat, FTextureInfo);
	static Ref<FTexture> Generate(FImage, bool bMultisample = false, FTextureInfo TexInfo = FTextureInfo());
	static Ref<FTexture> Generate(uint32_t, uint32_t);
protected:
	FTexture();
	FTexture(const FTexture&) = default;
	FTexture(FTexture&&) noexcept = default;

	virtual void Reload() = 0;
public:
	/// <summary>
	/// release GPU resource
	/// </summary>
	virtual ~FTexture() = default;
public:
	//=======================//
	//		get	& set		 //
	//=======================//
	inline const string&	GetName()			 const { return Name; };
	inline int				GetWidth()			 const { return Width; };
	inline int				GetHeight()			 const { return Height; };
	inline int				GetDepth()			 const { return Depth; };
	inline ETextureTarget	GetTextureType()	 const { return TextureTarget; };

	inline EInternalFormat	GetInternalFormat()	 const { return InternalFormat; };
	inline FTextureInfo		GetTextureInfo()	 const { return Info; };
public:
	virtual uint32_t		GetHandle()			 const = 0;
	virtual uint32_t		GetShortCut()		 const = 0;

	virtual void			Update(FTextureInfo) = 0;

	virtual void			SetData(FImage) = 0;
	virtual void			SetData(ETextureTarget, FImage) = 0;
public:
	virtual bool Parse(IN FJson&)final;
	virtual bool Serialize(OUT FJson&) final;

	virtual void Register()final;
protected:
	//========================================//
	//		general field (don't change)      //
	//========================================//
	std::string  Name = "";

	int			 Width = 0;
	int			 Height = 0;

	int			 Depth = 0;//only for texture 3D

	ETextureTarget     TextureTarget = ETextureTarget_2D;
protected:
	std::string RootPath;
	std::string Directory;
	std::string RelativePath;
	std::string Extension;
	std::string CachePath;

	bool bHdr  = false;
	bool bFlip = false;
	bool bMutisample = false;
public:
	inline std::string GetRootPath()	const { return RootPath; };
	inline std::string GetDirectory()	const { return Directory; };
	inline std::string GetRelativePath()const { return RelativePath; };
	inline std::string GetExtension()	const { return Extension; };
	inline std::string GetCachePath()	const { return CachePath; };

	inline bool IsHdr()					const { return bHdr; };
	inline bool IsFlip()				const { return bFlip; };
	inline bool IsLoadedFromFile()		const { return RelativePath != ""; };
protected:
	FTextureInfo Info;
	EInternalFormat InternalFormat = EInternalFormat_RGBA32F;
};