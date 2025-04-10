#include <windows.h>
#include <iostream>
#include <cassert>
#include "Tests.h"
using namespace std;
#define assertm(exp, msg) assert((void(msg), exp))



void cyclicSwitchTest() {
	int val1 = 5;
	val1 = CyclicSwitch(val1, 0, 7, true);
	int val2 = 5;
	val2 = CyclicSwitch(val2, 0, 5, true);
	int val3 = 0;
	val3 = CyclicSwitch(val3, -2, 7, false);
	int val4 = 0;
	val4 = CyclicSwitch(val4, 0, 7, false);

	const char* msg = "The CyclicSwitch() does not work correctly!\n";
	assertm(val1 == 6 && val2 == 0 && val3 == -1 && val4 == 7, msg);
}



void saveLoadTest() {
	int old_best_score = 100;
	int new_best_score = 0;
	saveGame(&old_best_score);
	loadGame(&new_best_score);

	const char* msg = "The saveGame() or loadGame() does not work correctly!\n";
	assertm(old_best_score == new_best_score, msg);
}



void keyPressTest() {
	WORD vkCode = 0x41;
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = vkCode;
	input.ki.dwFlags = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	SendInput(1, &input, sizeof(INPUT));

	bool is_key_just_pressed = IsKeyJustPressed(vkCode);

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));

	const char* msg = "The IsKeyJustPressed() does not work correctly!\n";
	assertm(is_key_just_pressed == true, msg);
}



void textCopyTest() {
	char text[64];
	textCopy(text, "text1", 6);
	string str(text);
	const char* msg = "The textCopy() does not work correctly!\n";
	assertm(str == "text1", msg);
}



void addCharToCharTest() {
	char* canvas = new char[64]{""};
	addCharToChar(canvas, "text1");
	addCharToChar(canvas, "text2");
	string str(canvas);
	const char* msg = "The addCharToChar() does not work correctly!\n";
	assertm(str == "text1text2", msg);
}



void startTests() {
	std::cout << "tests started\n";
	cyclicSwitchTest();
	std::cout << "checkpoint #1 passed\n";
	saveLoadTest();
	std::cout << "checkpoint #2 passed\n";
	keyPressTest();
	std::cout << "checkpoint #3 passed\n";
	textCopyTest();
	std::cout << "checkpoint #4 passed\n";
	addCharToCharTest();
	std::cout << "tests passed\n";
}