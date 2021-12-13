#include <iostream>
#include "FunctorBase.h"

class AdditionFunctor : public FunctorBase {
public:
	AdditionFunctor() {};
	AdditionFunctor(int val2) :FunctorBase{ val2 } {};

	int operator () (int val2) {
		return val + val2;
	}
};

