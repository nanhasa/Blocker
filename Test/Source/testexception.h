#pragma once

#include <exception>
#include <string>

// Custom exception to throw from callback functions
class TestException : public std::exception {
public:
	TestException(std::string message) : m_message(message) {}

	virtual const char* what() const throw() {
		return m_message.c_str();
	}

private:
	std::string m_message;
};
