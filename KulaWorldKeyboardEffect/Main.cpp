#include <CUESDK.h>

#include <iostream>
#include <Windows.h>
#include <thread>

#include "Keyboard/Keyboard.h"
#include "Memory/Memory.h"
#include "Memory/Offsets.h"

enum GameVersion {
	KULAWORLD, ROLLAWAY
};

enum LevelType {
	REGULAR, BONUS, HIDDEN
};

HANDLE processHandle;
DWORD processID;
DWORD_PTR moduleBaseAddress;

LevelType currentLevelType = REGULAR;
GameVersion currentGameVersion = KULAWORLD;

Keyboard::RGBColor currentRainbowColor;

void SetRainbowColor(int red, int green, int blue) {
	while (currentRainbowColor.r != red || currentRainbowColor.g != green || currentRainbowColor.b != blue) {
		if (currentRainbowColor.r < red) currentRainbowColor.r += 1;
		if (currentRainbowColor.r > red) currentRainbowColor.r -= 1;

		if (currentRainbowColor.g < green) currentRainbowColor.g += 1;
		if (currentRainbowColor.g > green) currentRainbowColor.g -= 1;

		if (currentRainbowColor.b < blue) currentRainbowColor.b += 1;
		if (currentRainbowColor.b > blue) currentRainbowColor.b -= 1;

		Sleep(1);
	}
}

void RainbowEffect() {
	SetRainbowColor(255, 0, 0);
	SetRainbowColor(0, 255, 0);
	SetRainbowColor(0, 0, 255);
	SetRainbowColor(255, 255, 0);
	SetRainbowColor(80, 0, 80);
	SetRainbowColor(0, 255, 255);
}

void KeyboardEffect(std::vector<CorsairLedColor>& keyLedColors) {
	switch (currentLevelType) {
	case REGULAR: Keyboard::SetKeyColors(keyLedColors, Keyboard::RGBColor{ 0, 0, 255 }); break;
	case BONUS: Keyboard::SetKeyColors(keyLedColors, currentRainbowColor); break;
	case HIDDEN: Keyboard::SetKeyColors(keyLedColors, Keyboard::RGBColor{ 255, 255, 255 }); break;
	default: Keyboard::SetKeyColors(keyLedColors, Keyboard::RGBColor{ 255, 0, 0 }); break;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

void SetCurrentLevelType() {
	int currentBall;
	if (currentGameVersion == KULAWORLD)currentBall = Memory::ReadProcMem(processHandle, moduleBaseAddress + KULAWORLD_CURRENT_BALL);
	else currentBall = Memory::ReadProcMem(processHandle, moduleBaseAddress + ROLLAWAY_CURRENT_BALL);

	switch (currentBall) {
	case 10: case 11: case 12:currentLevelType = BONUS; break;
	case 13: currentLevelType = HIDDEN; break;
	default:currentLevelType = REGULAR; break;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

int main() {
	SetConsoleTitle(TEXT("Kula World Keyboard Effect - Developed By: SaturnKai"));
	CorsairPerformProtocolHandshake();
	if (const auto error = CorsairGetLastError()) {
		std::cout << "ERROR: Failed to Perform Corsair Handshake: " << error << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}

	std::cout << "INFO: Successfully Performed Corsair Handshake!" << std::endl;

	auto keyLedColors = Keyboard::GetAvailableKeyColors();
	if (keyLedColors.empty()) {
		std::cout << "ERROR: Failed to Obtain Key Color Values!" << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}

	std::cout << "INFO: Successfully Obtained Key Color Values!" << std::endl;

	if (!Memory::AttachProcessHandle((char*)"ePSXe.exe", processHandle, processID, moduleBaseAddress)) {
		std::cout << "ERROR: Failed to Attach to ePSXe Process!" << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}

	std::cout << "INFO: Successfully Attached to ePSXe Process - PID: " << processID << std::endl;

	std::cout << "\nINPUT: Please Select Your Correct Version\n [1] Kula World\n [2] Roll Away\n\n > ";
	try
	{
		char option;
		std::cin >> option;
		switch (option) {
		case '1':currentGameVersion = KULAWORLD; break;
		case '2':currentGameVersion = ROLLAWAY; break;
		default:
			std::cout << "\nERROR: Invalid Selection!" << std::endl;
			std::cin.ignore();
			std::cin.get();
			return EXIT_FAILURE;
			break;
		}
	}
	catch (const std::exception&)
	{
		std::cout << "\nERROR: Invalid Selection!" << std::endl;
		std::cin.ignore();
		std::cin.get();
		return EXIT_FAILURE;
	}

	std::cout << "\nINFO: Successfully Selected Game Version!" << std::endl;

	std::atomic_bool continueEffect{ true };

	std::thread setLevelTypeThread([&continueEffect] {
		while (continueEffect) {
			SetCurrentLevelType();
		}
	});

	std::thread keyboardEffectThread([&keyLedColors, &continueEffect] {
		while (continueEffect) {
			KeyboardEffect(keyLedColors);
		}
	});

	std::thread rgbRainbowThread([&continueEffect] {
		while (continueEffect) {
			RainbowEffect();
		}
	});

	std::cout << "\nINPUT: Press Any Key to Exit..." << std::endl;
	std::cin.ignore();
	std::cin.get();
	std::cout << "INFO: Exiting..." << std::endl;

	continueEffect = false;
	setLevelTypeThread.join();
	keyboardEffectThread.join();
	rgbRainbowThread.join();

	return EXIT_SUCCESS;
}