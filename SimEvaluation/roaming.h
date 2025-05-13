#pragma once 
#include <CoreMinimal.h>

struct XRoamingPath:public flora::ISerialization
{
	XRoamingPath()
	{}

	XRoamingPath(const std::string& path)
	{
		LoadFromFile(path);
	}

	struct CameraState
	{
		CameraState() {};
		CameraState(float xx, float yy, float zz, float yyaw, float ppitch)
		{
			x = xx;
			y = yy;
			z = zz;
			yaw = yyaw;
			pitch = ppitch;
		};
		float x, y, z;
		float pitch, yaw;
	};

	struct Path
	{
		std::vector<glm::vec3> Pos;
		std::vector<glm::ivec4> Tetrahedron;

		std::vector<float> Pitches;
		std::vector<float> Yaws;


		int Fov;

		std::string Key;
		std::string OnlinePathType;

		struct Hash 
		{
			size_t operator()(const glm::vec3& obj) const 
			{
				return (std::hash<float>()(obj.x) ^ std::hash<float>()(obj.y)) ^ std::hash<float>()(obj.z);
			}
		};

		struct Equal 
		{
			bool operator()(const glm::vec3& obj1, const glm::vec3& obj2) const 
			{
				return abs(obj1.x - obj2.x) < 1e-7 && abs(obj1.y - obj2.y) < 1e-7 && abs(obj1.z - obj2.z) < 1e-7;
			}
		};

		void UnInterpolate()
		{
			std::unordered_set<glm::vec3, Path::Hash, Path::Equal> TmpCameraPos;

			for (glm::vec3& var : Pos)
			{
				TmpCameraPos.insert(var);
			}

			std::vector<float>& yaw_list = Yaws;

			std::vector<float>& pitch_list = Pitches;

			for (glm::vec3 var : TmpCameraPos)
			{
				for (size_t i = 0; i < yaw_list.size(); i++)
				{
					for (size_t j = 0; j < pitch_list.size(); j++)
					{
						CameraStates.push_back({
								var.x,var.y,var.z,
								yaw_list[i],pitch_list[j]
							});
					}
				}
			}
		}

		void Interpolate()
		{
			std::unordered_set<glm::vec3, Path::Hash, Path::Equal> TmpCameraPos;

			for(glm::vec3& var : Pos)
			{
				TmpCameraPos.insert(var);
			}

			for(glm::ivec4& var : Tetrahedron)
			{
				const glm::vec3& P0 = Pos[var[0]];
				const glm::vec3& P1 = Pos[var[1]];
				const glm::vec3& P2 = Pos[var[2]];
				const glm::vec3& P3 = Pos[var[3]];

				glm::vec3 F0 = (P0 + P1 + P2); F0 /= 3;
				glm::vec3 F1 = (P0 + P1 + P3); F1 /= 3;
				glm::vec3 F2 = (P0 + P2 + P3); F2 /= 3;
				glm::vec3 F3 = (P1 + P2 + P3); F3 /= 3;

				glm::vec3 V0 = (P0 + P1 + P2 + P3); V0 /= 4;

				TmpCameraPos.insert({ F0 });
				TmpCameraPos.insert({ F1 });
				TmpCameraPos.insert({ F2 });
				TmpCameraPos.insert({ F3 });

				TmpCameraPos.insert({ V0 });
			}

			std::vector<float> yaw_list =
			{
				90,45,0,-45,-90
			};

			std::vector<float> pitch_list =
			{
				180,135,90,45,0,-45,-90,-135,-180
			};

			for (glm::vec3 var : TmpCameraPos)
			{
				for (size_t i = 0; i < 5; i++)
				{
					for (size_t j = 0; j < 9; j++)
					{
						CameraStates.push_back({
								var.x,var.y,var.z,
								yaw_list[i],pitch_list[j]
							});
					}
				}
			}
		}

		std::vector<CameraState> CameraStates;
	};

	virtual bool Parse(const json11::Json& In)
	{
		Paths.clear();

		for (auto& Item : In["data"].object_items())
		{
			Path P;
			P.Key = Item.first;
			P.Fov = Item.second["fov"].int_value();
			P.OnlinePathType = Item.second["online_path_type"].string_value();
			for (auto& Point : Item.second["camera_points"].array_items())
			{
				P.Pos.push_back({
					Point["x"].number_value(),
					Point["y"].number_value(),
					Point["z"].number_value()
					});
			}

			for (auto& Id : Item.second["index"].array_items())
			{
				const auto& T = Id.array_items();
				P.Tetrahedron.push_back(
					{
						T[0].number_value(),
						T[1].number_value(),
						T[2].number_value(),
						T[3].number_value()
					});
			}

			for (auto& Pitch : Item.second["pitches"].array_items())
			{
				P.Pitches.push_back(Pitch.number_value());
			}

			for (auto& Yaw : Item.second["yaws"].array_items())
			{
				P.Yaws.push_back(Yaw.number_value());
			}

			auto UnInterpolatedPath = P; UnInterpolatedPath.UnInterpolate();
			auto InterpolatedPath = P; InterpolatedPath.Interpolate();

			UnInterpolatedPaths.push_back(UnInterpolatedPath);
			Paths.push_back(InterpolatedPath);
		}

		return true;
	}

	virtual bool Serialize(json11::Json& Out) { return true; }

	const std::vector<Path>& GetRoamingPath()
	{
		static std::vector<Path> PP;

		PP.clear();

		for (const Path& P : Paths)
		{
			if (P.OnlinePathType == "VRIBR3DPanoramaFirst")
			{
				PP.emplace_back(P);
			}
		}

		return PP;
	}

	const std::vector<Path>& GetBaseColorPath()
	{
		static std::vector<Path> PP;

		PP.clear();

		for (const Path& P : Paths)
		{
			if (P.OnlinePathType == "IBR3DRoamingFirst")
			{
				PP.emplace_back(P);
			}
		}

		return PP;
	}

	const std::vector<Path>& GetAllPath()
	{
		return UnInterpolatedPaths;
	}

	void CalculateLatLong(const glm::vec3& A, const glm::vec3& B, float& latitude, float& longitude,float& r) 
	{
		glm::vec3 vec = A - B;
		r = glm::length(vec);

		// ����γ��
		latitude = std::asin(vec.y / r);

		// ���㾭��
		longitude = std::atan2(vec.z, vec.x);

		// ת��Ϊ��
		latitude = glm::degrees(latitude) + 90;
		longitude = glm::degrees(longitude) + 180;
	}

	std::vector<CameraState> ChooseCameraStates(int SliceX,int SliceY,const glm::vec3& WorldPos)
	{
		std::vector<CameraState> Cameras;


		using namespace glm;

		float StrideX = 360 / SliceX, StrideY = 180 / SliceY;
		
		// TODO:[latitude][longitude]
		std::vector<std::vector<float>> MinDist(SliceY, std::vector<float>(SliceX, std::numeric_limits<float>::max()));
		std::vector<std::vector<bool>> bFill(SliceY, std::vector<bool>(SliceX, false));
		std::vector<std::vector<CameraState>> TmpStates(SliceY, std::vector<CameraState>(SliceX, CameraState{}));

		for (const Path& P : GetRoamingPath())
		{
			for (const CameraState& Stat : P.CameraStates)
			{
				float lat, lon, r;
				CalculateLatLong(vec3(Stat.x, Stat.y, Stat.z), WorldPos, lat, lon, r);

				int idy = glm::floor(lat / StrideY);
				int idx = glm::floor(lon / StrideX);

				//flora_CORE_ERROR("{0},{1}", idy, idx);

				if (r < MinDist[idy][idx])
				{
					MinDist[idy][idx] = r;
					bFill[idy][idx] = true;
					TmpStates[idy][idx] = Stat;
				}

				//Cameras.emplace_back(Stat);
			}
		}


		for (int i = 0; i < bFill.size(); ++i)
		{
			for (int j = 0; j < bFill[i].size(); ++j)
			{
				if (bFill[i][j])
				{
					Cameras.emplace_back(TmpStates[i][j]);
				}
			}
		}

		return Cameras;
	}

	std::vector<Path> Paths;
	std::vector<Path> UnInterpolatedPaths;
};
