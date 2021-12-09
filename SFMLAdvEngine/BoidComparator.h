#pragma once
#include "BoidAgentData.h"
#include "MathAdditions.h"

class BoidComparator {
public:
	bool operator() (const BoidAgentData& a, const BoidAgentData& b) const
	{
		float mag_a = mathAdditions::VectorSqrMagnitude(a.position);
		float mag_b = mathAdditions::VectorSqrMagnitude(b.position);
		return  mag_a < mag_b;
	};
};