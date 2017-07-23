#pragma once
#include <map>
#include <string>

namespace ps3d
{
	class Settings
	{
		std::map<std::string, std::string> settingsMap;
		void skipBOM(std::ifstream &in);
	public:
		explicit Settings(std::string fileName);
		bool has(std::string key);
		std::string get(std::string key);
	};
}
