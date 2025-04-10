#include <windows.h>
#include <iostream>
#include "Tests.h"
using namespace std;



string cyclicSwitchTest() {
	int val1 = 5;
	val1 = CyclicSwitch(val1, 0, 7, true);
	int val2 = 5;
	val2 = CyclicSwitch(val2, 0, 5, true);
	int val3 = 0;
	val3 = CyclicSwitch(val3, -2, 7, false);
	int val4 = 0;
	val4 = CyclicSwitch(val4, 0, 7, false);

	if (val1 != 6 || val2 != 0 || val3 != -1 || val4 != 7) {
		return "The CyclicSwitch() does not work correctly!\n";
	}
	return "The CyclicSwitch() works correctly\n";
}



string saveLoadTest() {
	int old_best_score = 100;
	int new_best_score = 0;
	saveGame(&old_best_score);
	loadGame(&new_best_score);

	if (old_best_score != new_best_score) {
		return "The saveGame() or loadGame() does not work correctly!\n";
	}
	return "The saveGame() and loadGame() works correctly\n";
}



string keyPressTest() {
	WORD vkCode = 0x41;
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = vkCode;
	input.ki.dwFlags = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	SendInput(1, &input, sizeof(INPUT));

	string ret;
	if (IsKeyJustPressed(vkCode) == false) {
		ret = "The IsKeyJustPressed() does not work correctly!\n";
	}
	else {
		ret = "The IsKeyJustPressed() works correctly\n";
	}

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));

	return ret;
}



string textCopyTest() {
	char text[64];
	textCopy(text, "text1", 6);
	string str(text);
	if (str != "text1") {
		return "The textCopy() does not work correctly!\n";
	}
	return "The textCopy() works correctly\n";
}



string addCharToCharTest() {
	char* canvas = new char[64]{""};
	addCharToChar(canvas, "text1");
	addCharToChar(canvas, "text2");
	string str(canvas);
	if (str != "text1text2") {
		return "The addCharToChar() does not work correctly!\n";
	}
	return "The addCharToChar() works correctly\n";
}



void startTests() {
	std::cout << cyclicSwitchTest();
	std::cout << saveLoadTest();
	std::cout << keyPressTest();
	std::cout << textCopyTest();
	std::cout << addCharToCharTest();
}