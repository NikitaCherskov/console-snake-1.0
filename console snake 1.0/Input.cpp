#include "Input.h"
#include <Windows.h>
#include <unordered_map>

std::unordered_map<int, bool> previousKeyState;
bool IsKeyJustPressed(int key) {
    bool isKeyPressed = (GetAsyncKeyState(key) & 0x8000) != 0;
    bool wasKeyPressed = previousKeyState[key];
    previousKeyState[key] = isKeyPressed;
    return isKeyPressed && !wasKeyPressed;
}



bool IsKeyPressed(int key) {
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}



void WaitKeyPress(int key) {
    while (true) {
        if (IsKeyJustPressed(key)) {
            return;
        }
    }
}



void WaitKeyJustPress(int key) {
    while (true) {
        if (IsKeyJustPressed(key)) {
            return;
        }
    }
}