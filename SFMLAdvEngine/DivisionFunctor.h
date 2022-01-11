#pragma once

#include <iostream>
#include "FunctorBase.h"

class DivisionFunctor : public FunctorBase {
public:
	DivisionFunctor() :FunctorBase(nullptr) {};

	virtual std::string GetName() { return "DivF"; };

	virtual float operator () (float val1, float val2) const {
		if (val2 == 0)
			return 1;
		else
			return val1 / val2;
	}
};
