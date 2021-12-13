#include <iostream>
#include "FunctorBase.h"

class AdditionFunctor : public FunctorBase {
public:
	AdditionFunctor() {};
	AdditionFunctor(int val2) :FunctorBase{ val2 } {};

	int operator () (float val1, float val2) {
		return val1 + val2;
	}
};

