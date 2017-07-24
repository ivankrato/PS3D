#pragma once
#include <map>
#include <string>

namespace ps3d
{
	class Settings
	{
		std::map<std::string, std::string> settingsMap;
		/**
		 * \brief skips UTF8 byte order mark
		 * \param in ifstream of configuration file
		 */
		void skipBOM(std::ifstream &in);
	public:
		/**
		 * \brief constructor
		 * \param fileName name of the configuraiton file
		 */
		explicit Settings(std::string fileName);
		/**
		 * \brief checks if something is configured
		 * \param key configuration key to check
		 * \return true if configured
		 */
		bool has(std::string key);
		/**
		 * \param key configuration key to get
		 * \return value of the configuration 
		 */
		std::string get(std::string key);
	};
}
