#pragma once

/**
 * \brief abort_program
 * \param file
 * \param line
 * \param condition
 * \param type
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
