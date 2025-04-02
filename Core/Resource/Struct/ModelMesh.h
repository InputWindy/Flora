#pragma once
#include <Common/Refl.hpp>
#include <Common/Serialize.h>
#include <Common/GlobalType.h>
#include <Geometry/StaticMesh.h>
#include <Resource/ResourceManager.h>
#include <vcglib/wrap/io_trimesh/io_mask.h>

namespace XVerse
{
	/// <summary>
	/// Assimp for example
	/// </summary>
	struct AssimpIFlags
	{
		bool bJoinIdenticalVertices = false;
		bool bPreTransformVertices = false;
		bool bOptimizeMeshes = false;
		bool bRemoveRedundantMaterials = false;
		bool bSortByPType = false;
	};

	struct AssimpOFlags
	{
		bool bExportUV0 = false;
	};

	//IO Settings
	using AssimpImportSettings = XImportSetting<AssimpIFlags, EFileType::FT_MODEL>;
	using AssimpExportSettings = XExportSetting<AssimpOFlags, EFileType::FT_MODEL>;

	class XModelMesh : public XVerse::IResource<>,ISerialization
	{
		IMPL_RESOURCE(ModelMesh)
	public:
		friend class XImporter<AssimpImportSettings, XModelMesh::BaseType>;
		friend class XExporter<AssimpExportSettings, XModelMesh>;

		class MeshNode;

	public:
		XModelMesh() = default;
		XModelMesh(const std::string& InFilePath,const XVerse::Wrap::GlobalSettings, std::vector<std::shared_ptr<XStaticMesh>>, std::vector<std::weak_ptr<MeshNode>>,std::shared_ptr<XModelMesh::MeshNode>);//��һ��StaticMesh������ModelMeshĬ�ϲ����κ����˽ṹ������ʱҲû��Node�ṹ
		virtual ~XModelMesh() = default;
	public:
		void UpdateFaceFaceAdj();
		void UpdateStaticMeshes();
		void UpdateStaticMeshesOnlyNode();
		const XVerse::Wrap::GlobalSettings& GetMetaData()const { return IOSettings; }
		std::shared_ptr<XVerse::XModelMesh::MeshNode> AllocateNode();
	protected:
		/*
			MetaData
		*/
		XVerse::Wrap::GlobalSettings IOSettings;//fbx�ļ����뵼��MetaData

	public:
		struct MeshNode:public std::enable_shared_from_this<MeshNode>
		{
			MeshNode() = default;

			/*
				Topology
			*/
			std::weak_ptr<MeshNode> Parent;
			std::vector<std::shared_ptr<MeshNode>> Children;

			/*
				Transformation
			*/
			glm::mat4 TransformToParent = glm::identity<glm::mat4>();//�任�����ڵ㣬����ԭʼģ�͵����˽ṹ

			/*
				Node Info
			*/
			std::string Name;

			std::vector<std::shared_ptr<XStaticMesh>> NodeMeshes = {};

			glm::mat4 TransformToRoot()
			{
				glm::mat4 TransformToRootNode = TransformToParent;

				if (Parent.expired())return TransformToRootNode;

				if (std::shared_ptr<XModelMesh::MeshNode> ParentNode = Parent.lock())
				{
					TransformToRootNode = ParentNode->TransformToRoot() * TransformToRootNode;
				}
				return TransformToRootNode;
			};

			static void GetMeshes(std::shared_ptr<MeshNode> InNode,std::vector<std::shared_ptr<XStaticMesh>>& InoutMeshes, std::vector<std::weak_ptr<MeshNode>>& InoutMeshNodes)
			{
				if (InNode)
				{
					InoutMeshes.insert(InoutMeshes.end(), InNode->NodeMeshes.begin(), InNode->NodeMeshes.end());
					InoutMeshNodes.insert(InoutMeshNodes.end(), InNode->NodeMeshes.size(), InNode->weak_from_this());

					for (size_t Idx = 0; Idx < InNode->Children.size(); Idx++)
					{
						GetMeshes(InNode->Children[Idx],InoutMeshes, InoutMeshNodes);
					}
				}
			}

			void SetOwner(std::shared_ptr<XModelMesh> Model)
			{
				for (auto Mesh : NodeMeshes)
				{
					Mesh->Owner = Model;
				}

				for (auto Child : Children)
				{
					Child->SetOwner(Model);
				}
			}

			std::shared_ptr<XStaticMesh> AllocateStaticMesh()
			{
				std::string MeshName = Name + '_' + std::to_string(NodeMeshes.size()).c_str();

				std::shared_ptr<XStaticMesh> NewMesh = std::make_shared<XStaticMesh>(nullptr, MeshName, glm::identity<glm::mat4>(), TransformToParent);
				NodeMeshes.push_back(NewMesh);

				NewMesh->Enable(vcg::tri::io::Mask::IOM_VERTCOORD | vcg::tri::io::Mask::IOM_VERTNORMAL | vcg::tri::io::Mask::IOM_VERTTEXCOORD | vcg::tri::io::Mask::IOM_VERTCOLOR);

				return NewMesh;
			}
			
			void AppendStaticMesh(std::shared_ptr<XStaticMesh> NewMesh)
			{
				NodeMeshes.push_back(NewMesh);
			}
		};
	public:
		const std::shared_ptr<MeshNode> GetTopology()const { return Root; }
		const std::vector<std::shared_ptr<XStaticMesh>> GetMeshes()const { return Meshes; }
		const std::vector<std::weak_ptr<MeshNode>> GetMeshNodes()const { return MeshNodes; }
	public:
		void SetMetaData(const Wrap::GlobalSettings& Settings) { IOSettings = Settings; };

		static std::shared_ptr<MeshNode> AllocateNode(const std::string& NodeName, std::shared_ptr<MeshNode> ParentNode, glm::mat4 TransformToParent = glm::identity<glm::mat4>())
		{
			std::shared_ptr<MeshNode> NewNode = std::make_shared<MeshNode>();
			NewNode->Name = NodeName;
			NewNode->Parent = ParentNode;
			NewNode->TransformToParent = TransformToParent;

			if (ParentNode)
			{
				ParentNode->Children.push_back(NewNode);
			}

			return NewNode;
		};

		void SetTopology(std::shared_ptr<MeshNode> RootNode)
		{
			if (!RootNode)return;

			Root = RootNode;
			Root->SetOwner(this->shared_from_this()->DynamicPointerCast<XModelMesh>());

			MeshNode::GetMeshes(Root, Meshes, MeshNodes);

			UpdateStaticMeshesOnlyNode();
		};

		std::vector<std::weak_ptr<MeshNode>>& GetMeshNode() { return MeshNodes; }

	protected:
		std::shared_ptr<MeshNode> Root = nullptr;
		std::vector<std::shared_ptr<XStaticMesh>> Meshes;
		std::vector<std::weak_ptr<MeshNode>> MeshNodes;
	};
}