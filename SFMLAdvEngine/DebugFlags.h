#pragma once

class DebugFlags {
	DebugFlags() = delete;

public:

	enum class PopulationUpdateModifiers {
		DEFAULT,
		SEPARATE_COLORS,
		WARPED_PERCEPTION
	};
	static constexpr float BRUSH_SIZE_LERP{ 0.2f };
	static const PopulationUpdateModifiers POPULATION_UPDATE_METHOD = PopulationUpdateModifiers::DEFAULT;
};