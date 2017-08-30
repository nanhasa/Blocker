#pragma once

#include <exception>
#include <string>

// Custom exception to throw from callback functions
class TestException : public std::exception {
public:
	explicit TestException(std::string message) : m_message(message) {}

	const char* what() const throw() override
	{
		return m_message.c_str();
	}

private:
	std::string m_message;
};
