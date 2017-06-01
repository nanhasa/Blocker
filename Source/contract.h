#pragma once

/**
 * \brief Prints error details and exits program
 * \param file Filepath and name of the file where the error occured
 * \param line Line number in the file where the error occured
 * \param condition The condition that failed
 * \param type Contract type that failed
 */
extern void abort_program(const char* file, const int line, const char* condition, const char* type);

#ifdef _DEBUG

#define ASSERT(condition, type) if (!(condition)) abort_program(__FILE__, __LINE__, #condition, type)
#define REQUIRE(condition) ASSERT(condition, "REQUIRE")
#define ENSURE(condition) ASSERT(condition, "ENSURE")
#define INVARIANT(condition) ASSERT(condition, "INVARIANT")

#else

#define ASSERT(ignore) ((void) 0)
#define REQUIRE(ignore) ((void) 0)
#define ENSURE(ignore) ((void) 0)
#define INVARIANT(ignore) ((void) 0)

#endif
