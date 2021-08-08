#include "Keyboard.h"

namespace Keyboard {
	std::vector<CorsairLedColor> GetAvailableKeyColors() {
		auto keyColorsSet = std::unordered_set<int>();
		for (int deviceIndex = 0, size = CorsairGetDeviceCount(); deviceIndex < size; deviceIndex++) {
			if (const auto ledPositions = CorsairGetLedPositionsByDeviceIndex(deviceIndex)) {
				for (auto i = 0; i < ledPositions->numberOfLed; i++) {
					const auto ledID = ledPositions->pLedPosition[i].ledId;
					keyColorsSet.insert(ledID);
				}
			}
		}
		std::vector<CorsairLedColor> keyColorVectors;
		keyColorVectors.reserve(keyColorsSet.size());
		for (const auto& ledID : keyColorsSet) keyColorVectors.push_back({ static_cast<CorsairLedId>(ledID), 0, 0, 0 });
		return keyColorVectors;
	}

	bool SetKeyColors(std::vector<CorsairLedColor>& keyLedColors, RGBColor color) {
		for (auto& ledColor : keyLedColors) {
			ledColor.r = color.r;
			ledColor.g = color.g;
			ledColor.b = color.b;
		}
		CorsairSetLedsColorsAsync(static_cast<int>(keyLedColors.size()), keyLedColors.data(), nullptr, nullptr);
		return true;
	}
}