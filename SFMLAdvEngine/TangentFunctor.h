#pragma once

#include <iostream>
#include "FunctorBase.h"

class TangentFunctor : public FunctorBase {
public:
	TangentFunctor() : FunctorBase{ nullptr } {};
	virtual std::string GetName() { return "TanF"; };

	virtual float operator () (float val1, float val2) const {
		float output = std::tanf((1 + val1) / (1 + val2)) * 255;
		return output;
	}
};
