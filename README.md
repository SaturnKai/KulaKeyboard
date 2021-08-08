Kula World Keyboard Effect is an experimental program I wrote in C++ that reads the ePSXe Emulator's memory to determine what type of level you're currently in inside of Kula World or Roll Away. Once the level type is determined, my program utilizes the Corsair CUE Software Development Kit to change the LED lighting effects on your Corsair keyboard.

Download Instructions:
-----

 - Download the Repository
 - Extract the .ZIP file and compile using Visual Studio
 - Run the newly compiled "Kula World Keyboard Effect.exe" executable

Program Instructions:
-----

When executing the program, it will attempt to perform a handshake with the ICUE Corsair Keyboard Software in order to establish a connection. If this step fails, you either don't have a compatible keyboard, or the "Enable SDK" option isn't enabled in your ICUE Software settings. Next, it will attempt to attach a process handle to the ePSXe emulator. If this step fails, ePSXe either isn't open or set up correctly. Next, you will be prompted to enter the correct game version. The current supported games are Kula World and Roll Away. After that, the program will constantly read the game's memory to determine what type of level you're on, and will use the Corsair CUE Software Development Kit to affect your keyboard with custom lighting effects accordingly using multithreading.

Lighting Effects:
-----

 - Regular Level: Solid blue effect
 - Bonus Level: Rapid rainbow effect
 - Hidden Level: Solid white effect

Screenshot:
-----

![Screenshot 1](https://www.satial.xyz/projects/jdg6yq7gFUGSDX2b.png)
