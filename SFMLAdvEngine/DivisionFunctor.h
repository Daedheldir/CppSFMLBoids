#include <iostream>
#include "FunctorBase.h"

class DivisionFunctor : public FunctorBase {
public:
	DivisionFunctor() {};
	DivisionFunctor(float val2) :FunctorBase{ val2 } {};
	const std::string var = "DivisionFunctor";

	virtual float operator () (float val1, float val2) {
		if (val2 == 0)
			return 1;
		else
			return val1 / val2;
	}
};
