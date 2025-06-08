#pragma once

int CyclicSwitch(int val, int min, int max, bool increase);
void setRank(char* nameskin, int can, int need_record);
void PrintMenuUnit(const char* text, int number, int choosen);
void drawSettings(int vibor2, char skin[4], int bestscore, short need_record[10]);
void askPrint(int choose, const char* text);
bool ask(const char* text);
void settingsProcess(char* skin, int best_score, short* need_record);
short skinMax(int bestscore, short need_record[10]);
bool skinCheck(int bestscore, short need_record[10]);