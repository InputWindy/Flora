#pragma once
#include "Core.h"
#include "Texture.h"
#include <filesystem>
#include <direct.h>
#include <windows.h>
struct FLORA_API FContentBrowserContext
{
	static inline FContentBrowserContext& Get() { static FContentBrowserContext msContentBrowser; return msContentBrowser; };

	FContentBrowserContext();
	~FContentBrowserContext();

	struct FLORA_API FTreeNode
	{
		std::filesystem::path Obj;

		std::string NodeName = "";
		int ChildIdx = -1;
		int ChildCount = -1;

		HICON Icon;

		static inline std::vector<FTreeNode> Nodes;
		static inline std::vector<FTreeNode>& GetTreeNodes() { return Nodes; };
	};

	static inline Ref<FTexture> FileIcon = nullptr;
	static inline Ref<FTexture> DirectoryIcon = nullptr;

	static inline std::string RootPath;
	static inline std::string CurrentPath;

	void Update();
};

