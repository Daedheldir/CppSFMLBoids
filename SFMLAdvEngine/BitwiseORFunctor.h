#pragma once

#include <iostream>
#include "FunctorBase.h"

class BitwiseORFunctor : public FunctorBase {
public:
	BitwiseORFunctor() :FunctorBase(nullptr) {};

	const std::string var = "BitwiseORFunctor";

	virtual float operator () (float val1, float val2) const {
		return static_cast<int>(val1) | static_cast<int>(val2);
	}
};
