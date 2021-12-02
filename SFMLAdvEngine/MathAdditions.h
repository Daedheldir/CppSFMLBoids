#pragma once
#include <cmath>
namespace mathAdditions {
	template<typename T>
	T VectorMagnitude(sf::Vector2<T> vec)
	{
		auto R_x = static_cast<T>(vec.x);
		auto R_y = static_cast<T>(vec.y);

		return std::sqrt(R_x * R_x + R_y * R_y);
	}

	template<typename T>
	T VectorSqrMagnitude(sf::Vector2<T> vec)
	{
		auto R_x = static_cast<T>(vec.x);
		auto R_y = static_cast<T>(vec.y);

		return R_x * R_x + R_y * R_y;
	}

	template <typename T>
	sf::Vector2<T> ClampVectorMagnitude(sf::Vector2<T> vec, T magnitude)
	{
		T max = std::abs(vec.x) > std::abs(vec.y) ? std::abs(vec.x) : std::abs(vec.y);
		T multiplier;
		float vecSqrMag = VectorSqrMagnitude(vec);
		if (vecSqrMag > magnitude)
			multiplier = magnitude / vecSqrMag;
		else
			multiplier = 1;

		return sf::Vector2<T>(vec.x * multiplier, vec.y * multiplier);
	}

	template <typename T>
	sf::Vector2<T> NormalizeVector(sf::Vector2<T> vec)
	{
		return ClampVectorMagnitude(vec, 1.0f);
	}

	template <typename T>
	T MapValue(T val, std::pair<T, T> in_range, std::pair<T, T> out_range) {
		return static_cast<T>(out_range.first + ((val - in_range.first) * (out_range.second - out_range.first)) / (in_range.second - in_range.first));
	}
}