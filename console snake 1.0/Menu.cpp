#include "Menu.h"
#include "TextUtils.h"
#include "Globals.h"
#include "Input.h"
#include "SaveLoad.h"
#include <iostream>
#pragma once

int CyclicSwitch(int val, int min, int max, bool increase) {
    if (increase) {
        val++;
    }
    else {
        val--;
    }
    if (val > max) {
        val = min;
    }
    if (val < min) {
        val = max;
    }
    return val;
}

void setRank(char* nameskin, int can, int need_record)
{
    const char* ranks[10] = {
        "Newbie",
        "Beginning",
        "Apprentice",
        "Advanced",
        "Professional",
        "Specialist",
        "Master",
        "Best master",
        "Legend",
        "***BINARY MONSTER***"
    };
    if (can == 1)
    {
        textCopy(nameskin, ranks[getSkinNumberLink()], 64);
    }
    else
    {
        textCopy(nameskin, "Locked, need score: ", 64);
        addIntToChar(nameskin, need_record);
    }
    return;
}

void PrintMenuUnit(const char* text, int number, int choosen) {
    if (choosen == number) {
        std::cout << ">" << text;
    }
    else {
        std::cout << " " << text;
    }
}

void drawSettings(int vibor2, char skin[4], int bestscore, short need_record[10])
{
    bool draw_skin = 0;
    char name_skin[64]{ "" };
    char setiskin[3][4];
    if (bestscore < need_record[getSkinNumberLink() - 1])
    {
        skinTextCopy(setiskin[0], "X X");
        skinTextCopy(setiskin[1], " X ");
        skinTextCopy(setiskin[2], "X X");
        draw_skin = 0;
    }
    else
    {
        skinTextCopy(setiskin[0], skin);
        skinTextCopy(setiskin[1], skin);
        skinTextCopy(setiskin[2], skin);
        draw_skin = 1;
    }
    setRank(name_skin, draw_skin, need_record[getSkinNumberLink() - 1]);
    std::cout << "SETTINGS:\n";
    PrintMenuUnit("size of place", 1, vibor2);
    std::cout << "\t\t< " << getFieldSizeLink() << " X " << getFieldSizeLink() << " >" << "\t" << setiskin[0] << "\n";
    PrintMenuUnit("speed of snake", 2, vibor2);
    std::cout << "\t\t< " << getSnakeSpeedLink() << " m/s >" << "\t" << setiskin[1] << "\t" << name_skin << "\n";
    PrintMenuUnit("collision with edges", 3, vibor2);
    if (getHasBarrierLink() == 1) {
        std::cout << "\t< YES >";
    }
    else {
        std::cout << "\t< NO >";
    }
    std::cout << "\t\t" << setiskin[2] << "\n";
    PrintMenuUnit("Skin of snake:", 4, vibor2);
    std::cout << "\t\t<" << getSkinNumberLink() << ">\n";
    PrintMenuUnit("Save game\n", 5, vibor2);
    PrintMenuUnit("Clear save\n", 6, vibor2);
    PrintMenuUnit("back\n", 7, vibor2);
    std::cout << "\t\t\t\t";
    return;
}

void askPrint(int choose, const char* text)
{
    system("cls");
    std::cout << text;
    PrintMenuUnit("No\n", 1, choose);
    PrintMenuUnit("Yes\n", 2, choose);
}

bool ask(const char* text)
{
    int choose = 1;
    while (true)
    {
        askPrint(choose, text);
        if (IsKeyJustPressed(87))
        {
            choose = CyclicSwitch(choose, 1, 2, false);
        }
        else if (IsKeyJustPressed(83))
        {
            choose = CyclicSwitch(choose, 1, 2, false);
        }
        else if (IsKeyJustPressed(13))
        {
            system("cls");
            return choose == 2;
        }
    }
    return false;
}

void settingsProcess(char* skin, int best_score, short* need_record) {
    system("cls");
    int choose = 1;
    bool exit = false;
    while (!exit)
    {
        setskin(skin);
        drawSettings(choose, skin, best_score, need_record);

        if (IsKeyJustPressed(87))
        {
            choose = CyclicSwitch(choose, 1, 7, false);
        }
        if (IsKeyJustPressed(83))
        {
            choose = CyclicSwitch(choose, 1, 7, true);
        }
        if (IsKeyJustPressed(13) && choose == 7)
        {
            exit = true;
        }
        if (IsKeyJustPressed(13) && choose == 6)
        {
            if (ask(getCPAsk()))
            {
                clearSave(&best_score);
            }
        }
        if (IsKeyJustPressed(13) && choose == 5)
        {
            saveGame(&best_score);
        }
        if (IsKeyJustPressed(68) || IsKeyJustPressed(65))
        {
            switch (choose)
            {
            case 1:
                getFieldSizeLink() = CyclicSwitch(getFieldSizeLink(), 16, 32, IsKeyJustPressed(68));
                break;
            case 2:
                getSnakeSpeedLink() = CyclicSwitch(getSnakeSpeedLink(), 2, 10, IsKeyJustPressed(68));
                break;
            case 3:
                getHasBarrierLink() = CyclicSwitch(getHasBarrierLink(), 0, 1, IsKeyJustPressed(68));
                break;
            case 4:
                getSkinNumberLink() = CyclicSwitch(getSkinNumberLink(), 1, 10, IsKeyJustPressed(68));
                break;
            }
        }
        system("cls");
    }
    if (skinCheck(best_score, need_record) == 0)
    {
        getSkinNumberLink() = skinMax(best_score, need_record);
        setskin(skin);
    }
    if (exit == true)
    {
        return;
    }
}

short skinMax(int bestscore, short need_record[10])
{
    short max_skin = 0;
    while (bestscore >= need_record[max_skin])
    {
        max_skin++;
    }
    return max_skin;
}

bool skinCheck(int bestscore, short need_record[10])
{
    return bestscore >= need_record[getSkinNumberLink() - 1];
}