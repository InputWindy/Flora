#pragma once
#include "Flora.h"

struct EditorLayer :public FLayer
{
	virtual void OnAwake();
	virtual void OnUpdate();
	virtual void OnGUI();

private:
	virtual bool MainRegion();
	virtual bool RenderGizmos();
private:
	//=======================//
	//		Window Item      //
	//=======================//
	virtual void Console();				//checked
	virtual void ContentBrowser();		//checked
	virtual void GameScene();			//checked
	virtual void EditorSettings();		//checked
	virtual void ResourceViewer();		//checked

	virtual void Hierarchy();
	virtual void Inspector();
	virtual void LevelViewer();
	virtual void GameObjectViewer();

private:
	bool bOpenContentBrowser = true;
	bool bOpenConsole = true;
	bool bOpenGameScene = true;
	bool bOpenResourceViewer = true;
	
	bool bOpenEditorSettings = false;
	bool bOpenHierarchy = false;
	bool bOpenInspector = false;
	bool bOpenLevelViewer = false;
	bool bOpenGameObjectViewer = false;
};
