#ifndef _BINTREEBASEEXCEPTION_H_
#define _BINTREEBASEEXCEPTION_H_

#include <string>
#include <stdexcept>

class BinTreeBaseException : public std::runtime_error
{
public:
	BinTreeBaseException(const std::string& err) : std::runtime_error(err) { }
	std::string msg() const { return what(); }
};

#endif
