#pragma once

#include <iostream>
#include "FunctorBase.h"

class BitwiseANDFunctor : public FunctorBase {
public:
	BitwiseANDFunctor() :FunctorBase(nullptr) {};
	virtual std::string GetName() { return "ANDF"; };

	virtual float operator () (float val1, float val2) const {
		return static_cast<float>((static_cast<int>(val1) & static_cast<int>(val2)));
	}
};
