#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>
#include <stdexcept>

using namespace std;

class Exception : public runtime_error
{
public:
	Exception(const string& err) : runtime_error(err) { }
	string msg() const { return what(); }
};

#endif

#pragma once
