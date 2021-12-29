#pragma once

#include <iostream>

class FunctorBase {
protected:
	float val;
public:
	FunctorBase() : val{ 0 } {};
	FunctorBase(float val2) :val{ val2 } {};
	const std::string var = "FunctorBase";

	virtual float operator () (float val1, float val2) {
		return val;

	}

	virtual float getVal() {
		return val;
	}


};


