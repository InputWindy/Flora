#pragma once
#include "Core.h"
#include "Type.h"
#include "Serialization.h"
#include "Resource.h"
#include "AABB.h"
#include <memory>
#include <string>

//===========================//
//		vertex attributes    //
//===========================//
struct FLORA_API FVertex
{
	FVertex() = default;
	FVertex(float x, float y, float z)
	{
		Position = { x,y,z };
	}
	/*location = 0*/ glm::vec3  Position;						
	/*location = 1*/ glm::vec3  Tangent;						
	/*location = 2*/ glm::vec3  Bitangent;						
	/*location = 3*/ glm::vec3  Normal;							
	/*location = 4*/ glm::ivec4 BoneIds;
	/*location = 5*/ glm::vec4  BoneWeights;
	/*location = 6*/ glm::vec3  UV0;
	/*location = 7*/ glm::vec3  UV1;
	/*location = 8*/ glm::vec3  UV2;
	/*location = 9*/ glm::vec3  UV3;
};

class FLORA_API FMesh:public std::enable_shared_from_this<FMesh>,public IResource,public ISerialization
{
	friend class FRenderCommand;
	friend class FOpenGLInterface;
	friend class FResourceManager;
protected:
	FMesh();
public :
	virtual ~FMesh() = default;

	inline const std::string& GetName() const { return MeshName; };
	inline size_t			  GetVertexNum()const { return VertexNum; };
	inline size_t			  GetIndexNum()const { return IndexNum; };
	inline FAABB			  GetOBB()const { return OBB; };

	void GetAABB(const glm::mat4& ModelMatrix,FAABB& InOut);

	inline const Ref<FMesh> GetCube()const { return Cube; };
	inline const Ref<FMesh> GetSphere()const { return Sphere; };
	inline const Ref<FMesh> GetCone()const { return Cone; };
	inline const Ref<FMesh> GetPlane()const { return Plane; };
	inline const Ref<FMesh> GetCylinder()const { return Cylinder; };
	inline const Ref<FMesh> GetCubeFrame()const { return CubeFrame; };
public:
	virtual void SetData(const std::vector<FVertex>&, const std::vector<uint32_t>&,const FAABB&) = 0;
	virtual void Draw(EDrawMode) = 0;
	virtual void DrawInstanced(EDrawMode,int) = 0;
public:
	//resource interface
	virtual void Register() final;
	virtual void Rename(const string& name) final;

	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;
protected:
	std::string MeshName;
	size_t VertexNum = 0;
	size_t IndexNum  = 0;

	//local space aabb
	FAABB OBB;
protected:
	std::string ModelResourcePath;
	std::string ModelMeshName;
private:
	static inline Ref<FMesh> Cube = nullptr;
	static inline Ref<FMesh> Sphere = nullptr;
	static inline Ref<FMesh> Cone = nullptr;
	static inline Ref<FMesh> Plane = nullptr;
	static inline Ref<FMesh> Cylinder = nullptr;
	static inline Ref<FMesh> CubeFrame = nullptr;
	static inline Ref<FMesh> Screen = nullptr;
};

