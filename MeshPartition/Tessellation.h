#include <CoreMinimal.h>

#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/update/topology.h>
#include <vcg/complex/algorithms/update/flag.h>

namespace tsl
{
	using namespace XVerse;

	struct SimpleSubdivision
	{
		typedef vcg::tri::Allocator<XStaticMesh>::FaceIterator FaceIter;
		typedef vcg::tri::Allocator<XStaticMesh>::VertexIterator VertexIter;
		typedef vcg::tri::Allocator<XStaticMesh>::FacePointer FacePtr;

		struct Vert
		{
			Vert() {}
			Vert(const vcg::Point3f& P0, const vcg::Point3f& N0, const vcg::TexCoord2f& T0)
				:P(P0),N(N0),T(T0)
			{}

			Vert(const Vert& Rhs)
				:P(Rhs.P), N(Rhs.N), T(Rhs.T)
			{}

			Vert(Vert&& Rhs)
				:P(Rhs.P), N(Rhs.N), T(Rhs.T)
			{}

			vcg::Point3f P;
			vcg::Point3f N;
			vcg::TexCoord2f T;
		};

		static void Do(XStaticMesh& InMesh, XFace& InitialFace, float MaxTriArea)
		{
			std::queue<FacePtr> FaceQueue;

			// 初始三角形入队
			FaceQueue.push(&InitialFace);

			while (!FaceQueue.empty())
			{
				// 取出当前需要处理的三角形
				XFace* CurrentFace = FaceQueue.front();
				FaceQueue.pop();

				// 计算当前三角形面积
				float Area = ComputeArea(*CurrentFace);
				if (Area <= MaxTriArea)
				{
					// 面积已满足要求，无需细分
					continue;
				}

				// 获取顶点
				XVertex* V0 = CurrentFace->V(0);
				XVertex* V1 = CurrentFace->V(1);
				XVertex* V2 = CurrentFace->V(2);

				// 插值法线和 UV
				auto Interpolate = [](const auto& A, const auto& B, float Alpha) { return A + Alpha * (B - A); };
				auto InterpolateUV = [](const vcg::TexCoord2f& A, const vcg::TexCoord2f& B, float Alpha)
					{
						float Au = A.U();
						float Av = A.V();
						float Bu = B.U();
						float Bv = B.V();

						vcg::TexCoord2f Out;
						Out.U() = Au + Alpha * (Bu - Au);
						Out.V() = Av + Alpha * (Bv - Av);

						return Out;
					};

				// 计算中点位置
				vcg::Point3f PosM01 = (V0->P() + V1->P()) * 0.5f;
				vcg::Point3f PosM12 = (V1->P() + V2->P()) * 0.5f;
				vcg::Point3f PosM20 = (V2->P() + V0->P()) * 0.5f;

				// 计算中点法线
				vcg::Point3f NormalM01 = Interpolate(V0->N(), V1->N(), 0.5f).Normalize();
				vcg::Point3f NormalM12 = Interpolate(V1->N(), V2->N(), 0.5f).Normalize();
				vcg::Point3f NormalM20 = Interpolate(V2->N(), V0->N(), 0.5f).Normalize();

				// 计算中点 UV
				vcg::TexCoord2f UVM01 = InterpolateUV(V0->T(), V1->T(), 0.5f);
				vcg::TexCoord2f UVM12 = InterpolateUV(V1->T(), V2->T(), 0.5f);
				vcg::TexCoord2f UVM20 = InterpolateUV(V2->T(), V0->T(), 0.5f);

				int VertOffset = VertsToAdd.size();

				std::vector<Vert> V(6);
				V[0].P = V0->P();
				V[0].N = V0->N();
				V[0].T = V0->T();
				V[1].P = V1->P();
				V[1].N = V1->N();
				V[1].T = V1->T();
				V[2].P = V2->P();
				V[2].N = V2->N();
				V[2].T = V2->T();
				V[3].P = PosM01;
				V[3].N = NormalM01;
				V[3].T = UVM01;
				V[4].P = PosM12;
				V[4].N = NormalM12;
				V[4].T = UVM12;
				V[5].P = PosM20;
				V[5].N = NormalM20;
				V[5].T = UVM20;

				std::vector<int> I =
				{
					VertOffset + 0,VertOffset + 3,VertOffset + 5,
					VertOffset + 3,VertOffset + 1,VertOffset + 4,
					VertOffset + 5,VertOffset + 4,VertOffset + 2,
					VertOffset + 3,VertOffset + 4,VertOffset + 5
				};

				for (int i = 0; i < 6; ++i)
				{
					VertsToAdd.push_back(V[i]);
				}

				for (int i = 0; i < 12; ++i)
				{
					IdxToAdd.push_back(I[i]);
				}

				vcg::tri::Allocator<XStaticMesh>::DeleteFace(InMesh, *CurrentFace);

			}
		}

		static void Init()
		{
			VertsToAdd.clear();
			IdxToAdd.clear();
		}

		static void Finalize(XStaticMesh& InMesh)
		{
			InMesh.CompactEveryVector();

			auto VertexIter = vcg::tri::Allocator<XVerse::XStaticMesh>::AddVertices(InMesh, VertsToAdd.size());

			InMesh.CompactEveryVector();

			for (size_t i = 0; i < IdxToAdd.size() / 3; i++)
			{
				Vert& V0 = VertsToAdd[IdxToAdd[i * 3 + 0]];
				Vert& V1 = VertsToAdd[IdxToAdd[i * 3 + 1]];
				Vert& V2 = VertsToAdd[IdxToAdd[i * 3 + 2]];

				(VertexIter + IdxToAdd[i * 3 + 0])->P() = VertsToAdd[IdxToAdd[i * 3 + 0]].P;
				(VertexIter + IdxToAdd[i * 3 + 0])->N() = VertsToAdd[IdxToAdd[i * 3 + 0]].N;
				(VertexIter + IdxToAdd[i * 3 + 0])->T() = VertsToAdd[IdxToAdd[i * 3 + 0]].T;
				(VertexIter + IdxToAdd[i * 3 + 1])->P() = VertsToAdd[IdxToAdd[i * 3 + 1]].P;
				(VertexIter + IdxToAdd[i * 3 + 1])->N() = VertsToAdd[IdxToAdd[i * 3 + 1]].N;
				(VertexIter + IdxToAdd[i * 3 + 1])->T() = VertsToAdd[IdxToAdd[i * 3 + 1]].T;
				(VertexIter + IdxToAdd[i * 3 + 2])->P() = VertsToAdd[IdxToAdd[i * 3 + 2]].P;
				(VertexIter + IdxToAdd[i * 3 + 2])->N() = VertsToAdd[IdxToAdd[i * 3 + 2]].N;
				(VertexIter + IdxToAdd[i * 3 + 2])->T() = VertsToAdd[IdxToAdd[i * 3 + 2]].T;

				vcg::tri::Allocator<XStaticMesh>::AddFace(InMesh,
					(VertexIter + IdxToAdd[i * 3 + 0])->Index(),
					(VertexIter + IdxToAdd[i * 3 + 1])->Index(),
					(VertexIter + IdxToAdd[i * 3 + 2])->Index()
				);
			}
			XVERSE_CORE_ERROR("Num Added Triangle : {0}", IdxToAdd.size() / 3)

				InMesh.CompactEveryVector();
		}

		static bool AreaBig(const XFace& Face, float MaxTriArea) {
			float Area = ComputeArea(Face);
			return Area >= MaxTriArea;
		}

		static bool Inside(const XFace& Face, glm::vec4 Sphere)
		{
			//const auto& Center = vec3(Sphere);
			const auto& V0 = Face.V(0)->P();
			const auto& V1 = Face.V(1)->P();
			const auto& V2 = Face.V(2)->P();

			return Interact(vec3(Sphere), Sphere.w, 
				glm::vec3(V0.X(), V0.Y(), V0.Z()),
				glm::vec3(V1.X(), V1.Y(), V1.Z()),
				glm::vec3(V2.X(), V2.Y(), V2.Z())
			);
		}

	private:
		// 计算三角形面积
		static float ComputeArea(const XFace& Face)
		{
			const auto& V0 = Face.V(0)->P();
			const auto& V1 = Face.V(1)->P();
			const auto& V2 = Face.V(2)->P();

			// 面积公式：| (V1 - V0) × (V2 - V0) | / 2
			auto CrossProduct = (V1 - V0) ^ (V2 - V0);
			return 0.5f * std::sqrt(CrossProduct.SquaredNorm());
		}

		static bool Interact(const glm::vec3& P,const float R, const glm::vec3& V0, const glm::vec3& V1, const glm::vec3& V2)
		{
			// 检查到平面距离
			glm::vec3 N = glm::normalize(glm::cross(V1 - V0, V2 - V0));
			glm::vec3 C = (V0 + V1 + V2) / vec3(3, 3, 3);

			glm::vec3 CP = P - C;
			float H = glm::dot(N, CP);

			if (H < 0)
			{
				N = -N;
				H = -H;
			}

			if (H > R)return false;

			// 检查点是否在球内
			if (glm::length(V0 - P) < R || glm::length(V1 - P) < R || glm::length(V2 - P) < R) {
				return true;
			}

			// 检查边是否在球内
			// --边V0--V1
			glm::vec3 S = V1 - V0;
			float t = glm::dot(P - V0, S) / glm::dot(S, S);
			float t_clamped = glm::clamp(t, 0.0f, 1.0f);
			glm::vec3 cloest = V0 + t_clamped * S;
			if (glm::length(cloest - P) < R) {
				return true;
			}
			// --边V1--V2
			S = V2 - V1;
			t = glm::dot(P - V1, S) / glm::dot(S, S);
			t_clamped = glm::clamp(t, 0.0f, 1.0f);
			cloest = V1 + t_clamped * S;
			if (glm::length(cloest - P) < R) {
				return true;
			}
			// --边V2--V0
			S = V0 - V2;
			t = glm::dot(P - V2, S) / glm::dot(S, S);
			t_clamped = glm::clamp(t, 0.0f, 1.0f);
			cloest = V2 + t_clamped * S;
			if (glm::length(cloest - P) < R) {
				return true;
			}

			// 检查面是否在球内
			//glm::vec3 F = C + P - N * H;
			glm::vec3 F = P - N * H;

			glm::vec3 N1 = glm::cross(glm::normalize(V0 - F), glm::normalize(V1 - V0));
			glm::vec3 N2 = glm::cross(glm::normalize(V1 - F), glm::normalize(V2 - V1));
			glm::vec3 N3 = glm::cross(glm::normalize(V2 - F), glm::normalize(V0 - V2));

			float Cos1 = glm::dot(N1, N2);
			float Cos2 = glm::dot(N2, N3);
			float Cos3 = glm::dot(N3, N1);

			return  Cos1 * Cos2 > 0 && Cos1 * Cos3 > 0 && Cos2 * Cos3 > 0;
		}

	private:
		static inline std::vector<Vert> VertsToAdd;
		static inline std::vector<int> IdxToAdd;
	};

	template<typename TProcesser>
	static bool Tessellation(XStaticMesh* InMesh, glm::vec4 BoundingSphere, float MaxTriArea)
	{
		if (!InMesh) return false;

		XStaticMesh& Mesh = *InMesh;

		TProcesser::Init();

		for (int i = 0;i < Mesh.face.size();++i)
		{
			XFace& Face = Mesh.face[i];
			if (TProcesser::AreaBig(Face, MaxTriArea) && TProcesser::Inside(Face, BoundingSphere))
			{
				TProcesser::Do(Mesh, Face, MaxTriArea);
				//XVERSE_CORE_ERROR("ok")
			}
		}

		TProcesser::Finalize(Mesh);

		Mesh.RemoveDuplicate();
		Mesh.RemoveDegenerate();
		Mesh.RemoveUnreferencedVertex();

		Mesh.RegenerateFaceNormal();
		Mesh.RegenerateVertexNormalFromFace();
		Mesh.CompactEveryVector();

		Mesh.UpdateBouding();

		return true;
	}
}