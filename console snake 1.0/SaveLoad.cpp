#include "SaveLoad.h"
#include "Globals.h"
#include <fstream>

void saveGame(int* bestscore)
{
    std::ofstream saving;
    saving.open("save.snake", std::ios::out | std::ios::binary);
    saving.write((const char*)&getFieldSizeLink(), sizeof getFieldSizeLink());
    saving.write((const char*)&getSnakeSpeedLink(), sizeof getSnakeSpeedLink());
    saving.write((const char*)&getHasBarrierLink(), sizeof getHasBarrierLink());
    saving.write((const char*)&getSkinNumberLink(), sizeof getSkinNumberLink());
    saving.write((const char*)bestscore, sizeof * bestscore);
    saving.close();
}

void loadGame(int* bestscore)
{
    std::ifstream loading;
    loading.open("save.snake", std::ios::in | std::ios::binary);
    loading.read((char*)&getFieldSizeLink(), sizeof getFieldSizeLink());
    loading.read((char*)&getSnakeSpeedLink(), sizeof getSnakeSpeedLink());
    loading.read((char*)&getHasBarrierLink(), sizeof getHasBarrierLink());
    loading.read((char*)&getSkinNumberLink(), sizeof getSkinNumberLink());
    loading.read((char*)bestscore, sizeof * bestscore);
    loading.close();
}

void clearSave(int* bestscore)
{
    std::fstream clear;
    clear.open("save.snake", std::ios::trunc | std::ios::binary);
    clear.close();
    getFieldSizeLink() = 16;
    getSnakeSpeedLink() = 2;
    getHasBarrierLink() = 0;
    getSkinNumberLink() = 1;
    *bestscore = 0;
}