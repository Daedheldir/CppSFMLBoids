#pragma once

#include <iostream>
#include "FunctorBase.h"

class CosineFunctor : public FunctorBase {
public:
	CosineFunctor() : FunctorBase{ nullptr } {};
	virtual std::string GetName() { return "CosF"; };

	virtual float operator () (float val1, float val2) const {
		float output = std::cosf((1 + val1) / (1 + val2)) * 255;
		return output;
	}
};
