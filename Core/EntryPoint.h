#pragma once
#include <Application/Application.h>
#include <Common/Log.h>

#include <exception>
#include <stdexcept>

int main(int argc, char* argv[])
{
	try
	{
		XVerse::IApp* GApp = XVerse::CreatApp(argc, argv);

		GApp->StartUp();
		GApp->Run();
		GApp->Shutdown();

		delete GApp;
	}
	catch (const std::exception& e)
	{
		//XVERSE_CORE_ERROR("Fatal Error:{0}", e.what());
		std::cerr << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}

	return 0;
}