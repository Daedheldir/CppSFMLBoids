#pragma once
#include <cmath>
namespace mathAdditions {
	template<typename T>
	const T VectorMagnitude(const sf::Vector2<T>& vec)
	{
		return std::sqrt(VectorSqrMagnitude(vec));
	}

	template<typename T>
	const T VectorSqrMagnitude(const sf::Vector2<T>& vec)
	{
		return vec.x * vec.x + vec.y * vec.y;
	}

	template <typename T>
	const sf::Vector2<T> ClampVectorMagnitude(const sf::Vector2<T>& vec, const T magnitude)
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
	const sf::Vector2<T> NormalizeVector(const sf::Vector2<T>& vec)
	{
		return ClampVectorMagnitude(vec, 1.0f);
	}

	template <typename T>
	T MapValue(const T& val, const std::pair<T, T>& in_range, const std::pair<T, T>& out_range) {
		return static_cast<T>(out_range.first + ((val - in_range.first) * (out_range.second - out_range.first)) / (in_range.second - in_range.first));
	}
}