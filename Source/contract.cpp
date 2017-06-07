#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <exception>

#include "contract.h"

void abort_program(const char* file, const int line, const char* condition, const char* type)
{
	printf("%s condition (%s) failed on line %d in file %s", type, condition, line, file);
	exit(EXIT_FAILURE);
}