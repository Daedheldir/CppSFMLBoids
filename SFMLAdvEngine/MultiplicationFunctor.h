#include <iostream>
#include "FunctorBase.h"

class MultiplicationFunctor : public FunctorBase {
public:
	MultiplicationFunctor() {};
	MultiplicationFunctor(float val2) : FunctorBase{ val2 } {};
	const std::string var = "MultiplicationFunctor";

	virtual float operator () (float val1, float val2) {
		return val1 * val2;
	}
};
