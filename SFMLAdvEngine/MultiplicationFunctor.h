#pragma once

#include <iostream>
#include "FunctorBase.h"

class MultiplicationFunctor : public FunctorBase {
public:
	MultiplicationFunctor() : FunctorBase(nullptr) {};
	virtual std::string GetName() { return "MultF"; };

	virtual float operator () (float val1, float val2) const {
		return val1 * val2;
	}
};
