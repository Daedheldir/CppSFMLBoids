#pragma once

#include <iostream>
#include "FunctorBase.h"

class LeftShiftFunctor : public FunctorBase {
public:
	LeftShiftFunctor() : FunctorBase{ nullptr } {};
	const std::string var = "LeftShiftFunctor";

	virtual float operator () (float val1, float val2) const {
		return static_cast<float>(static_cast<int>(val1) << static_cast<int>(val2));
	}
};
#pragma once
