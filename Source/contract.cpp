#include "contract.h"

#include <cstdio>
#include <cstdlib>

void abort_program(const char* file, const int line, const char* condition, const char* type)
{
	printf("%s condition (%s) failed on line %d in file %s", type, condition, line, file);
	exit(EXIT_FAILURE);
}