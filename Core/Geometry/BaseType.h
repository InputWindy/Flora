#pragma once
#include <string>
#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/clean.h>
#include <vcg/container/derivation_chain.h>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace XVerse
{
	typedef XVERSE_SCALAR XScalar;
	typedef vcg::Point2<XVERSE_SCALAR>     XPoint2;
	typedef vcg::Point3<XVERSE_SCALAR>     XPoint3;
	typedef vcg::Point4<XVERSE_SCALAR>     XPoint4;
	typedef vcg::Plane3<XVERSE_SCALAR>     XPlane3;
	typedef vcg::Segment2<XVERSE_SCALAR>   XSegment2;
	typedef vcg::Segment3<XVERSE_SCALAR>   XSegment3;
	typedef vcg::Box3<XVERSE_SCALAR>       XBox3;
	typedef vcg::Matrix44<XVERSE_SCALAR>   XMatrix44;
	typedef vcg::Matrix33<XVERSE_SCALAR>   XMatrix33;
	typedef vcg::Shot<XVERSE_SCALAR>       XShot;
	typedef vcg::Similarity<XVERSE_SCALAR> XSimilarity;
}

namespace vcg
{
	using namespace XVerse;

	namespace vertex
	{
		template <class T> class XPosition3 : public Coord<XPoint3, T> {
		public:	static void Name(std::vector<std::string>& name) { name.push_back(std::string("XPosition3")); T::Name(name); }
		};

		template <class T> class XNormal3 : public Normal<XPoint3, T> {
		public:	static void Name(std::vector<std::string>& name) { name.push_back(std::string("XNormal3")); T::Name(name); }
		};

		template <class T> class XTexCoord2Ocf : public TexCoordOcf<TexCoord2<XScalar, 1>, T> {
		public: static void Name(std::vector<std::string>& name) { name.push_back(std::string("XTexCoord2Ocf")); T::Name(name); }
		};

		template <class TT> class XQuality : public Quality<XScalar, TT> {
		public: static void Name(std::vector<std::string>& name) { name.push_back(std::string("XQuality")); TT::Name(name); }
		};
	}

	namespace face
	{
		template <class T> class XNormal3 : public NormalAbs<XPoint3, T> {
		public:  static void Name(std::vector<std::string>& name) { name.push_back(std::string("XNormal3")); T::Name(name); }
		};

		template <class T> class XQualityOcf : public QualityOcf<XScalar, T> {
		public:  static void Name(std::vector<std::string>& name) { name.push_back(std::string("XQualityOcf")); T::Name(name); }
		};

		template <class T> class XCurvatureDirOcf : public CurvatureDirOcf<CurvatureDirOcfBaseType<XScalar>, T> {
		public:	static void Name(std::vector<std::string>& name) { name.push_back(std::string("XCurvatureDirOcf")); T::Name(name); }
		};
	}
}

namespace XVerse
{
	// Forward declarations needed for creating the used types
	class XVertex;
	class XEdge;
	class XFace;

	// Declaration of the semantic of the used types
	class XUsedTypes : public vcg::UsedTypes < vcg::Use<XVertex>::AsVertexType,
		vcg::Use<XEdge   >::AsEdgeType,
		vcg::Use<XFace  >::AsFaceType > {};

	// The Main Vertex Class
	// Most of the attributes are optional and must be enabled before use.
	// Each vertex needs 40 byte, on 32bit arch. and 44 byte on 64bit arch.

	class XVertex : public vcg::Vertex< XUsedTypes,
		vcg::vertex::InfoOcf,           /*  4b */
		vcg::vertex::BitFlags,          /*  4b */
		vcg::vertex::XPosition3,         /* 12b */
		vcg::vertex::XNormal3,           /* 12b */
		vcg::vertex::XTexCoord2Ocf,      /*  0b */
		vcg::vertex::XQuality,          /*  4b */
		vcg::vertex::Color4b,           /*  4b */
		vcg::vertex::VFAdjOcf,           /*  0b */
		vcg::vertex::VFAdj,
		vcg::vertex::MarkOcf,           /*  0b */
		vcg::vertex::VEAdj
	> {};


	// The Main Edge Class
	class XEdge : public vcg::Edge<XUsedTypes,
		vcg::edge::BitFlags,          /*  4b */
		vcg::edge::EVAdj,
		vcg::edge::EEAdj,
		vcg::edge::VEAdj
	> {
	};

	// Each face needs 32 byte, on 32bit arch. and 48 byte on 64bit arch.
	class XFace : public vcg::Face< XUsedTypes,
		vcg::face::InfoOcf,              /* 4b */
		vcg::face::VertexRef,            /*12b */
		vcg::face::BitFlags,             /* 4b */
		vcg::face::XNormal3,             /*12b */
		vcg::face::QualityfOcf,          /* 0b */
		vcg::face::MarkOcf,              /* 0b */
		vcg::face::FFAdjOcf,             /* 0b */
		vcg::face::VFAdjOcf,             /* 0b */
		vcg::face::FEAdj,
		vcg::face::XCurvatureDirOcf,     /* 0b */
		vcg::face::WedgeTexCoordfOcf     /* 0b */
	> {};

	typedef vcg::tri::TriMesh< vcg::vertex::vector_ocf<XVertex>, vcg::face::vector_ocf<XFace>, std::vector<XEdge> > XTriMesh;

}

namespace XVerse
{
	namespace Wrap
	{
		/*
				int32: UpAxis => 1
				int32: UpAxisSign => 1
				int32: FrontAxis => 2
				int32: FrontAxisSign => 1
				int32: CoordAxis => 0
				int32: CoordAxisSign => 1
				int32: OriginalUpAxis => 2
				int32: OriginalUpAxisSign => 1
				float: UnitScaleFactor => 1
				float: OriginalUnitScaleFactor => 1
				int32: FrameRate => 6
				uint64: TimeSpanStart => 0
				uint64: TimeSpanStop => 0
				float: CustomFrameRate => -1
				string: SourceAsset_FormatVersion => 7400
				string: SourceAsset_Generator => FBX SDK/FBX Plugins version 2018.1
				string: SourceAsset_Format => Autodesk FBX Importer

				// different Up/Front value map to different coordinate
				// blender export fbx with forward(-Z)/Up(Y) : UpAxes(1)/FrontAxes(2)/CoordAxis(0), UpAxisSign(1)/FrontAxisSign(1)/CoordAxisSign(1)
				// UE export fbx: UpAxes(2)/FrontAxes(1)/CoordAxis(0), UpAxisSign(1)/FrontAxisSign(-1)/CoordAxisSign(1)
			*/
		struct GlobalSettings
		{
			GlobalSettings() = default;
			GlobalSettings(const GlobalSettings&) = default;

			int UpAxis = 1;
			int UpAxisSign = 1;
			int FrontAxis = 2;
			int FrontAxisSign = 1;
			int CoordAxis = 0;
			int CoordAxisSign = 1;
			int OriginalUpAxis = 2;
			int OriginalUpAxisSign = 1;
			double UnitScaleFactor = 1;
			double OriginalUnitScaleFactor = 1;
			int32_t FrameRate = 0;
			uint64_t TimeSpanStart = 0;
			uint64_t TimeSpanStop = 0;
			float CustomFrameRate = -1.0f;

			std::string SourceAssetFormatVersion = "";
			std::string SourceAssetGenerator = "";
			std::string SourceAssetFormat = "";
		};
	}
}