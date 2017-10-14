#include "Utility/contract.h"

#include <cstdio>
#include <cstdlib>

#include "Utility/logger.h"
#include "utility.h"

void abortProgram(const char* file, const int line, const char* condition, const char* type)
{
	static Logger log("Contract");
	log.fatal(std::string(type) + " condition " + std::string(condition) + 
		" failed on line " + utility::toStr(line) + " in file " + std::string(file));
	exit(EXIT_FAILURE);
}
