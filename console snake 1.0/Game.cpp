#include "Game.h"
#include "Globals.h"
#include "Cell.h"
#include "Block.h"
#include "Input.h"
#include "SaveLoad.h"
#include "TextUtils.h"
#include "Menu.h"
#include <string>
#include <ctime>
#include <iostream>
#include <cstdlib>

//----------------новые функции-----------------


//-----------------------------------------------------
void spawnLocationSelection(int* x, int* y, int place[32][32])
{
    int randcell;
    int i;
    int a;
    int b;
    int amount = 0;
    cell* emty_place = new cell[getFieldSizeLink() * getFieldSizeLink()];
    i = 0;
    a = 0;
    while (a < getFieldSizeLink())
    {
        b = 0;
        while (b < getFieldSizeLink())
        {
            if (place[a][b] == 0)
            {
                emty_place[amount].set(a, b);
                amount++;
            }
            b++;
        }
        a++;
    }
    randcell = rand() % amount;
    *y = emty_place[randcell].x;
    *x = emty_place[randcell].y;
    delete[] emty_place;
}

//Обработка смерти-------------------------------------
int cycleOnField(int a)
{
    if (a == getFieldSizeLink())
    {
        a = 0;
    }
    else if (a == -1)
    {
        a = getFieldSizeLink() - 1;
    }
    return a;
}

int checkCollide(block snakeb, int place[32][32], int l)
{
    int x_delta = 0;
    int y_delta = 0;
    if (l == 1) {
        y_delta = -1;
    }
    else if (l == 2) {
        x_delta = 1;
    }
    else if (l == 3) {
        y_delta = 1;
    }
    else {
        x_delta = -1;
    }
    int x_next = snakeb.x + x_delta;
    int y_next = snakeb.y + y_delta;
    if (getHasBarrierLink() == 1)
    {
        if (x_next == -1 || x_next == getFieldSizeLink()) {
            return 1;
        }
        if (y_next == -1 || y_next == getFieldSizeLink()) {
            return 1;
        }
    }
    return place[cycleOnField(y_next)][cycleOnField(x_next)];
}

//-----------------------------------------------------


void moveAll(int vector, block snakeb[1024])
{
    int x_move = 0;
    int y_move = 0;
    if (vector == 1) {
        y_move = -1;
    }
    if (vector == 2) {
        x_move = 1;
    }
    if (vector == 3) {
        y_move = 1;
    }
    if (vector == 4) {
        x_move = -1;
    }
    int i = 1;
    snakeb[0].move(x_move, y_move);
    while (snakeb[i].active)
    {
        snakeb[i].set(snakeb[i - 1].lastx, snakeb[i - 1].lasty);
        i++;
    }
}

bool intersec(int lr, int ud, block snakeb[1024])
{
    int i = 0;
    while (snakeb[i].active == 1)
    {
        if (snakeb[i].x == lr && snakeb[i].y == ud)
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int painter(int place[32][32], int score, char skin[4])
{
    char* canvas = new char[3500]{ "" };
    for (int i = 0; i < getFieldSizeLink(); i++)
    {
        addCharToChar(canvas, "---");
    }
    addCharToChar(canvas, "--\tscore:");
    addIntToChar(canvas, score);
    addCharToChar(canvas, "\n");
    for (int i = 0; i < getFieldSizeLink(); i++)
    {
        addCharToChar(canvas, "|");
        for (int j = 0; j < getFieldSizeLink(); j++)
        {
            if (place[i][j] == 1)
            {
                addCharToChar(canvas, skin);
            }
            else if (place[i][j] == 2)
            {
                addCharToChar(canvas, "(*)");
            }
            else
            {
                addCharToChar(canvas, "   ");
            }
        }
        addCharToChar(canvas, "|\n");
    }
    for (int i = 0; i < getFieldSizeLink(); i++)
    {
        addCharToChar(canvas, "---");
    }
    addCharToChar(canvas, "--\n");
    std::cout << canvas;
    delete[] canvas;
    return 0;
}

void spawnSnake(block* snakeb, int length) {
    int halfplace = getFieldSizeLink() / 2;
    for (int i = 0; i < length; i++)
    {
        snakeb[i].spawn(halfplace, halfplace + i);
    }
}

void createPlace(int place[32][32]) {
    for (int b = 0; b < 32; b++)
    {
        for (int a = 0; a < 32; a++)
        {
            place[b][a] = 0;
        }
    }
}

void addSnakeOnPlace(block* snakeb, int place[32][32]) {
    for (int i = 0; snakeb[i].active == 1; i++)
    {
        place[snakeb[i].y][snakeb[i].x] = 1;
    }
}

int snakeProcess(char skin[4], bool* snake_exit)
{
    int score = 0;
    int timetoblock = 1000 / getSnakeSpeedLink();
    int length = 3;
    int eat_resp = 1;
    int col = 0;
    int lr = 0;
    int ud = 0;
    int vector = 1;
    clock_t t1;
    int place[32][32];
    block* snakeb = new block[getFieldSizeLink() * getFieldSizeLink()];
    spawnSnake(snakeb, length);
    std::cout << "\n";
    while (1)
    {
        createPlace(place);
        addSnakeOnPlace(snakeb, place);
        if (eat_resp == 1) //если еда была в прошлый раз съедена, переразместить еду и удлинится
        {
            spawnLocationSelection(&lr, &ud, place);
            eat_resp = 0;
        }
        place[ud][lr] = 2;
        system("cls");
        painter(place, score, skin);
        t1 = clock();
        std::cout << "\n";
        while ((clock() - t1) < timetoblock)
        {
            if (IsKeyPressed(87) && (snakeb[0].y - snakeb[1].y) != 1)
            {
                vector = 1;
            }
            else if (IsKeyPressed(68) && (snakeb[1].x - snakeb[0].x) != 1)
            {
                vector = 2;
            }
            else if (IsKeyPressed(83) && (snakeb[1].y - snakeb[0].y) != 1)
            {
                vector = 3;
            }
            else if (IsKeyPressed(65) && (snakeb[0].x - snakeb[1].x) != 1)
            {
                vector = 4;
            }
            else if (IsKeyPressed(27))
            {
                if (ask(getEPAsk()) == 1)
                {
                    *snake_exit = 1;
                    return score;
                }
                else
                {
                    painter(place, score, skin);
                }
            }
        }
        col = checkCollide(snakeb[0], place, vector);
        if (col == 1)
        {
            return score;
        }
        if (col == 2) //что происходит если еда была съедена
        {
            timetoblock = timetoblock * 0.98;
            score++;
            length++;
            snakeb[length - 1].active = 1;
            eat_resp = 1;
        }
        col = 0;
        moveAll(vector, snakeb);
    }
    delete[] snakeb;
    return score;
}

void printGameOver() {
    const char* owr_txt_1 = "XXXX XXXX XX XX XXXX   XXXX  X  X XXXX X XX \n";
    const char* owr_txt_2 = "X    X  X X X X X      X  X  X  X X    XX  X\n";
    const char* owr_txt_3 = "X XX XXXX X X X XXXX   X  X  X  X XXXX X    \n";
    const char* owr_txt_4 = "X  X X  X X X X X      X  X  X  X X    X    \n";
    const char* owr_txt_5 = "XXXX X  X X X X XXXX   XXXX   XX  XXXX X    \n";
    std::cout << owr_txt_1 << owr_txt_2 << owr_txt_3 << owr_txt_4 << owr_txt_5;
}

int mainProcess() {
    bool snakeexit = 0;
    short needed_score[10]{ 0, 50, 100, 200, 300, 400, 550, 700, 850, 1024 };
    char skin[4] = "";
    int best_score = 0;
    int score = 0;
    int choose = 1;
    bool exit = false;
    loadGame(&best_score);
    srand(time(0));
    setskin(skin);
    while (1)
    {
        exit = false;
        while (!exit)
        {
            std::cout << "Your best score: " << best_score << "\n\nMENU:\n";

            const char* menu_units[5]{ "Play\n", "Settings\n", "Help\n", "Authors\n", "Save and exit\n" };
            for (int i = 0; i < 5; i++)
            {
                PrintMenuUnit(menu_units[i], i + 1, choose);
            }

            if (IsKeyJustPressed(87))
            {
                choose = CyclicSwitch(choose, 1, 5, false);
            }
            if (IsKeyJustPressed(83))
            {
                choose = CyclicSwitch(choose, 1, 5, true);
            }
            if (IsKeyJustPressed(13))
            {
                if (choose == 1)
                {
                    score = snakeProcess(skin, &snakeexit);
                }
                if (choose == 2)
                {
                    settingsProcess(skin, best_score, needed_score);
                }
                if (choose == 3)
                {
                    system("cls");
                    std::cout << "Help:\n";
                    std::cout << "(w, a, s, d) - control\n";
                    std::cout << "(Enter) - interaction\n";
                    std::cout << ">Back\n";
                    WaitKeyPress(13);
                }
                if (choose == 4)
                {
                    system("cls");
                    std::cout << "Authors:\n";
                    std::cout << "Programming - Nikita\n";
                    std::cout << "Graphic - Nikita\n";
                    std::cout << "Animations - Nikita\n";
                    std::cout << "Physics - Nikita\n";
                    std::cout << "Voice acting - Nikita\n";
                    std::cout << ">Back\n";
                    WaitKeyPress(13);
                }
                if (choose == 5)
                {
                    saveGame(&best_score);
                    return 0;
                }
            }
            system("cls");
        }
        if (score > best_score)
        {
            best_score = score;
        }
        system("cls");
        if (snakeexit == 1)
        {
            std::cout << "You been exit, your score: " << score << "\n\n>Continue";
        }
        else
        {
            printGameOver();
            if (score >= getFieldSizeLink() * getFieldSizeLink())
            {
                std::cout << "\n\nYou won! You have reached the maximum value on this place!";
            }
            std::cout << "\n\nYour score : " << score << "\n\n>Continue";
        }
        snakeexit = 0;
        WaitKeyPress(13);
        system("cls");
    }
    return 0;
}