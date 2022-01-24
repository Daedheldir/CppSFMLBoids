#pragma once

class GPParameters {
	GPParameters() = delete;

public:

	enum class PopulationUpdateModifiers {
		DEFAULT,
		DEFAULT_WITH_SEPARATE_COLORS_COMPARISON,
		DEFAULT_WITH_WEIGHTED_COLORS_COMPARISON,
		SEPARATE_COLORS,
		WARPED_PERCEPTION
	};
	static constexpr float BRUSH_SIZE_LERP{ 0.00f };

	static constexpr float BOID_DISCARD_PERCENTAGE{ 0.95f };

	static constexpr float MUTATION_RATE_TREE{ 0.2f };
	static constexpr float MUTATION_RATE_RAND{ 0.5f };
	static constexpr unsigned int MUTATION_NODES_COUNT{ 1 };
	static constexpr float MUTATION_RANGE_MIN{ -32.0f };
	static constexpr float MUTATION_RANGE_MAX{ 32.0f };

	static constexpr float IMAGE_FADE_DISTANCE{ 25.0f };
	static constexpr bool ENABLE_IMAGE_FADING{ false };

	static const PopulationUpdateModifiers POPULATION_UPDATE_METHOD = PopulationUpdateModifiers::DEFAULT;
};