#include "Settings.h"
#include <fstream>
#include <vector>
#include <sstream>

void ps3d::Settings::skipBOM(std::ifstream &in)
{
	char test[3] = { 0 };
	in.read(test, 3);
	if (static_cast<unsigned char>(test[0]) == 0xEF &&
		static_cast<unsigned char>(test[1]) == 0xBB &&
		static_cast<unsigned char>(test[2]) == 0xBF)
	{
		return;
	}
	in.seekg(0);
}

ps3d::Settings::Settings(std::string fileName)
{
	std::ifstream stream(fileName);
	skipBOM(stream);
	std::string line;
	while(!stream.eof())
	{
		getline(stream, line);
		if (line.empty() || line.at(0) == '#') continue; //if comment or empty skip
		std::vector<std::string> lineVector;
		std::stringstream ss(line);
		std::string word;
		while (std::getline(ss, word, ' ')) { //splits the line to parts
			lineVector.push_back(word);
		}
		settingsMap[lineVector[0]] = lineVector[1];
	}
}

bool ps3d::Settings::has(std::string key)
{
	return settingsMap.find(key) != settingsMap.end();
}

std::string ps3d::Settings::get(std::string key)
{
	return settingsMap[key];
}