#pragma once

#include <iostream>
#include "FunctorBase.h"

class MultiplicationFunctor : public FunctorBase {
public:
	MultiplicationFunctor() : FunctorBase(nullptr) {};
	const std::string var = "MultiplicationFunctor";

	virtual float operator () (float val1, float val2) const {
		return val1 * val2;
	}
};
