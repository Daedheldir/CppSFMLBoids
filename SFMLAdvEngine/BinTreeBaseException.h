#ifndef _BINTREEBASEEXCEPTION_H_
#define _BINTREEBASEEXCEPTION_H_

#include <string>
#include <stdexcept>

using namespace std;

class BinTreeBaseException : public runtime_error
{
public:
	BinTreeBaseException(const string& err) : runtime_error(err) { }
	string msg() const { return what(); }
};

#endif

#pragma once

