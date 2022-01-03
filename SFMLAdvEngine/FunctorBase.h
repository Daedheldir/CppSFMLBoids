#pragma once

#include <iostream>

class FunctorBase {
protected:
	float* val;
public:
	FunctorBase(float* val2) :val{ val2 } {};
	virtual ~FunctorBase() {};
	const std::string var = "FunctorBase";

	virtual float operator ()(float val1, float val2) const {
		return getVal();
	}

	virtual float getVal() const {
		return (val == nullptr) ? 0 : *val;
	}
};
