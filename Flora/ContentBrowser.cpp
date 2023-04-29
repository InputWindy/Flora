#include "ContentBrowser.h"
#include "ResourceManager.h"
#include "Application.h"
#include <queue>
#include <glfw3.h>
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <thumbcache.h>
#include <tchar.h>
#include <shellapi.h>//SHGetFileInfo

#include <direct.h>
#include <string>
#include <memory>
#include <algorithm>
using namespace std;

HICON GetFileIcon(const TCHAR* path)
{
	CoInitialize(NULL);
	SHFILEINFO shfi;
	ZeroMemory(&shfi, sizeof(SHFILEINFO));
	HRESULT hr = SHGetFileInfo(path,
		0,
		&shfi,
		sizeof(shfi),
		SHGFI_ICON | SHGFI_SMALLICON | SHGFI_DISPLAYNAME);
	CoUninitialize();
	return shfi.hIcon;
}

FContentBrowserContext::FContentBrowserContext()
{

	RootPath = _getcwd(nullptr, 0);
	replace(RootPath.begin(), RootPath.end(), '\\', '/');

	CurrentPath = RootPath;

	FResourceManager& ResourceManager = FResourceManager::Get();
	Ref<FTexture> File = ResourceManager.FindObject<FTexture>(string("file"));
	Ref<FTexture> Directory = ResourceManager.FindObject<FTexture>(string("directory"));
	if (!File)
	{
		File = FApplication::GetRHI()->GenerateTexture("file", 0, 0, 0, ETextureTarget_2D, EInternalFormat_RGBA);
		File->SetImageData(std::make_shared<FImage>(RootPath, "/Icon/file.png", false, true));
	}
	if (!Directory)
	{
		Directory = FApplication::GetRHI()->GenerateTexture("directory", 0, 0, 0, ETextureTarget_2D, EInternalFormat_RGBA);
		Directory->SetImageData(std::make_shared<FImage>(RootPath, "/Icon/directory.png", false, true));
	}

	FileIcon = File;
	DirectoryIcon = Directory;
	Update();
}

FContentBrowserContext::~FContentBrowserContext()
{
}


void FContentBrowserContext::Update()
{
	using namespace std;
	{
		FTreeNode::Nodes.clear();

		std::filesystem::path Root(RootPath);
		std::filesystem::path Current(CurrentPath);

		queue<std::filesystem::path> q;
		q.push(Root);

		while (!q.empty())
		{
			filesystem::directory_entry entry(q.front()); q.pop();

			FTreeNode node;
			node.Obj = entry.path();
			node.NodeName = entry.path().filename().generic_string();
			//node.Icon = GetFileIcon(TEXT("C:\\Users\\InputWindy\\Desktop\\SandBox\\SandBox\\default.jpg"));

			if (!entry.is_directory())
			{
				node.ChildIdx = -1;
				node.ChildCount = -1;
			}
			else
			{
				int cnt = 0;
				node.ChildIdx = FTreeNode::Nodes.size() + 1 + q.size();
				for (auto& entry : filesystem::directory_iterator(entry)) { q.push(entry.path()); ++cnt; };
				node.ChildCount = cnt;
			}
			FTreeNode::Nodes.emplace_back(node);
		}
	}
}
