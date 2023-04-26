#pragma once
#include "Application.h"
extern void CreateApplication();
void main()
{
	CreateApplication();
	auto GEngine = FApplication::GetApp();
	if (GEngine)
	{
		GEngine->MainLoop();
		delete GEngine;
	}
}