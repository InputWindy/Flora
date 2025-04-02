#pragma once
#include <Common/Common.h>
#include <Geometry/BaseType.h>
#include <Common/Layout.h>
#include <memory>

namespace XVerse
{
	class XRHIVertexBuffer;
	class XRHIIndexBuffer;
	class XRHIVertexArray;
	class XRHIShaderStorageBuffer;

	class XModelMesh;

	class XStaticMesh : public XVerse::XTriMesh, public INoneCopyable,public std::enable_shared_from_this<XStaticMesh>
	{
		friend class XModelMesh;
	public:
		enum MeshElement
		{
			MM_NONE = 0x00000000,
			MM_VERTCOORD = 0x00000001,
			MM_VERTNORMAL = 0x00000002,
			MM_VERTFLAG = 0x00000004,
			MM_VERTCOLOR = 0x00000008,
			MM_VERTQUALITY = 0x00000010,
			MM_VERTMARK = 0x00000020,
			MM_VERTFACETOPO = 0x00000040,
			MM_VERTCURV = 0x00000080,
			MM_VERTCURVDIR = 0x00000100,
			MM_VERTRADIUS = 0x00000200,
			MM_VERTTEXCOORD = 0x00000400,
			MM_VERTNUMBER = 0x00000800,

			MM_FACEVERT = 0x00001000,
			MM_FACENORMAL = 0x00002000,
			MM_FACEFLAG = 0x00004000,
			MM_FACECOLOR = 0x00008000,
			MM_FACEQUALITY = 0x00010000,
			MM_FACEMARK = 0x00020000,
			MM_FACEFACETOPO = 0x00040000,
			MM_FACENUMBER = 0x00080000,
			MM_FACECURVDIR = 0x00100000,

			MM_WEDGTEXCOORD = 0x00200000,
			MM_WEDGNORMAL = 0x00400000,
			MM_WEDGCOLOR = 0x00800000,

			// 	Selection
			MM_VERTFLAGSELECT = 0x01000000,
			MM_FACEFLAGSELECT = 0x02000000,

			// Per Mesh Stuff....
			MM_CAMERA = 0x08000000,
			MM_TRANSFMATRIX = 0x10000000,
			MM_COLOR = 0x20000000,
			MM_POLYGONAL = 0x40000000,

			// unknown - will raise exceptions, to be avoided, here just for compatibility
			MM_UNKNOWN = 0x80000000,

			// geometry change (for filters that remove stuff or modify geometry or topology, but not touch face/vertex color or face/vertex quality)
			MM_GEOMETRY_AND_TOPOLOGY_CHANGE = 0x431e7be7,

			// everything - dangerous, will add unwanted data to layer (e.g. if you use MM_ALL it could means that it could add even color or quality)
			MM_ALL = 0xffffffff
		};
	public:
		XStaticMesh() {}
		XStaticMesh(const std::shared_ptr<XModelMesh> InOwner, const std::string& InName,
			glm::mat4 InCoordMatrix = glm::identity<glm::mat4>(),
			glm::mat4 InTransformToRoot = glm::identity<glm::mat4>())
			:Owner(InOwner), CoordMatrix(InCoordMatrix), TransformToRoot(InTransformToRoot), Name(InName) {}
		virtual ~XStaticMesh() = default;
	public:
		const std::string& GetStaticMeshName()const { return Name; }
		const XVerse::Wrap::GlobalSettings& GetMetaData()const;
	public:
		inline bool HasDataMask(const int InMaskToBeTested) const { return ((CurrentDataMask & InMaskToBeTested) != 0); }
		void UpdateDataMask();
		void UpdateDataMask(int InNeededDataMask);
		void ClearDataMask(int InUnneededDataMask);
		void Enable(int InOpeningFileMask);
		int DataMask() const;
		
		inline glm::mat4 GetTransformToRootMatrix()const { return TransformToRoot; }
		inline glm::mat4 GetCoordMatrix()const { return CoordMatrix; }

		inline std::string GetNodeName()const { return NodeName; }
		inline void SetNodeName(const std::string& InNodeName) { NodeName = InNodeName; }
		inline XModelMesh* GetOwner() { if (Owner.expired() == false)return Owner.lock().get(); else return nullptr; }

		//void SetData(float* aPos,float* aNor,float* aUV,int* Index,int numV,int numI);
	public:
		void UpdateBouding();

		int RemoveUnreferencedVertex();
		void RemoveDuplicate();
		void RemoveDegenerate();

		void CompactEveryVector();
		void RegenerateFaceNormal();
		void RegenerateVertexNormal();
		void RegenerateVertexNormalFromFace();
		void RegenerateEdge();

		void RegenerateMesh(const std::set<unsigned int>& InDeleteFaces);
	public:
		void SetData(std::byte* VertexBuffer, size_t VertexBufferSize, BufferLayout VertexLayout, unsigned int* IndexBuffer, int IndexNum);
	protected:
		/*
			Flags
		*/
		int CurrentDataMask = MM_VERTCOORD | MM_VERTNORMAL | MM_VERTFLAG | MM_FACEVERT | MM_FACENORMAL | MM_FACEFLAG;
	protected:
		glm::mat4 CoordMatrix = glm::mat4(1.0f); //fix coord system
		glm::mat4 TransformToRoot;//transform to localspace

		std::string Name;//mesh name
		std::string NodeName;//mesh node name
		

		std::weak_ptr<XModelMesh> Owner;
	};
}