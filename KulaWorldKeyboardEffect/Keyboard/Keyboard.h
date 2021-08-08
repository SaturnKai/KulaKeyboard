#pragma once
#define CORSAIR_LIGHTING_SDK_DISABLE_DEPRECATION_WARNINGS
#include <CUESDK.h>
#include <vector>
#include <unordered_set>

namespace Keyboard {
	typedef struct {
		float r, g, b;
	} RGBColor;

	std::vector<CorsairLedColor> GetAvailableKeyColors();
	bool SetKeyColors(std::vector<CorsairLedColor>& keysLedColors, RGBColor color);
}