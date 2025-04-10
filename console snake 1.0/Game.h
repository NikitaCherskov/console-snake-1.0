#pragma once

int CyclicSwitch(int val, int min, int max, bool increase);
void saveGame(int* bestscore);
void loadGame(int* bestscore);
bool IsKeyJustPressed(int key);
void textCopy(char* inpstr, const char* outstr, int inlenth);
void addCharToChar(char* canvas, const char str_for_add[]);
int mainProcess();