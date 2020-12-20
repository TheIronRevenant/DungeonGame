#include "logger.hpp"

#include <fstream>
#include <chrono>
#include <ctime>

namespace DG
{
	std::stringstream log;
}

void DG::dumpLog()
{
	std::time_t logTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream timeCast;
	timeCast << logTime;
	std::string fileName = "log" + timeCast.str() + ".txt";

	std::ofstream ofs(fileName);
	ofs << DG::log.str();
	ofs.close();
}