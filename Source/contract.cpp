#include <cstdio>
#include <cstdlib>

#include "contract.h"

void abortProgram(const char* file, const int line, const char* condition, const char* type) {
	printf("%s condition (%s) failed on line %d in file %s", type, condition, line, file);
	exit(EXIT_FAILURE);
}