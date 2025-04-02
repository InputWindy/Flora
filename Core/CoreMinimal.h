#pragma once
//Common module
#include <Common/ForwardDeclare.h>
#include <Common/Common.h>
#include <Common/Layout.h>
#include <Common/Log.h>
#include <Common/Refl.hpp>
#include <Common/Serialize.h>

//Resource module ( standalone module )
#include <Resource/ResourceManager.h>

//Geometry module ( standalone math module )
#include <Geometry/BaseType.h>
#include <Geometry/StaticMesh.h>

//Resource type ( depend on Common & Geometry & ResourceManager)
#include <Resource/Struct/ModelMesh.h>
#include <Resource/Struct/Material.h>
#include <Resource/Struct/Texture.h>

//Render module (depend on Common & Resource type)
#include <Render/Proxy/ProxyBase.h>
#include <Render/Renderer.h>
#include <Render/ExampleRenderer.h>
#include <Render/RHI/RHI.h>
#include <Render/Proxy/Continer/MaterialFieldContainer.h>
#include <Render/Proxy/Continer/StaticMeshFieldContainer.h>

//Scene module (depend on xxx)
#include <Scene/Scene.h>
#include <Scene/Actor.h>
#include <Scene/Component.h>
#include <Scene/Struct/Camera.h>
#include <Scene/Struct/Transform.h>

//Window module ( standalone module)
#include <Window/Window.h>

//Editor Interface
#include <GUI/EditorBase.h>

//Application ( frame work )
#include <Application/Application.h>

//libigl & vcglib
//#include <vcglib/wrap/igl/lscm_parametrization.h>


//Third patry
#ifdef WITH_GUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_node_editor.h>
#include <ImGuizmo.h>
#include <implot.h>
#endif

#include <argparse/argparse.hpp>

//std
#include <string.h>
#include <exception>
#include <stdint.h>