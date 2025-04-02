#include <Common/Common.h>
#include <filesystem>
#include <iostream>
#include <cmath>
using namespace std;
bool XVerse::GlobalUtils::CreateDirectoryRecursive(const std::string& path)
{
	std::filesystem::path dir_path(path);

	if (std::filesystem::exists(dir_path))
	{
		if (std::filesystem::is_directory(dir_path)) {
			return true;
		}
		else {
			return false;
		}
	}

	if (CreateDirectoryRecursive(dir_path.parent_path().generic_string()))
	{
		return std::filesystem::create_directory(dir_path);
	}
	return false;
}

void XVerse::PrintProgressBar(float cc, int allNum)
{
	float percentage = cc * 100.0f / allNum;
	const int barWidth = 50;
	int progress = barWidth * percentage / 100;

	std::cout << "[";
	for (int i = 0; i < barWidth; ++i) {
		if (i < progress) {
			std::cout << "=";
		}
		else {
			std::cout << " ";
		}
	}
	std::cout.flush();
	std::cout << "] " << std::setprecision(2) << percentage << "%.";
	std::cout << std::fixed << std::setprecision(0);
	std::cout << "[" << cc << "/" << allNum << "]" << "\r";
	std::cout.flush();
}
int XVerse::MinPowerOfTwo(int x)
{
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x + 1;
}

std::pair<int, int> XVerse::CalGroupNum(int ClusterNum) 
{
	int PreservedGroupNum = MinPowerOfTwo(ClusterNum);

	int Exp = log2(PreservedGroupNum);

	int GroupX = pow(2, Exp / 2);
	int GroupY = pow(2, Exp / 2 + 1);

	return std::pair<int, int>(GroupX, GroupY);
}
