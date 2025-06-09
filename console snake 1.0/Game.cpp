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
    int random_cell;
    int i;
    int amount = 0;
    cell* empty_place = new cell[getFieldSizeLink() * getFieldSizeLink()];
    i = 0;
    for (int a = 0; a < getFieldSizeLink(); a++)
    {
        for (int b = 0; b < getFieldSizeLink(); b++)
        {
            if (place[a][b] == 0)
            {
                empty_place[amount].set(a, b);
                amount++;
            }
        }
    }
    random_cell = rand() % amount;
    *y = empty_place[random_cell].x;
    *x = empty_place[random_cell].y;
    delete[] empty_place;
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
    for (int i = 0; snakeb[i].active == 1; i++)
    {
        if (snakeb[i].x == lr && snakeb[i].y == ud)
        {
            return 0;
        }
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
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            place[i][j] = 0;
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
    int game_score = 0;
    int step_duration = 1000 / getSnakeSpeedLink();
    int snake_length = 3;
    bool required_food_reposition = true;
    int collider_type = 0;
    int food_pos_x = 0;
    int food_pos_y = 0;
    int move_vector = 1;
    clock_t clock_mark;
    int place[32][32];
    block* snake_block = new block[getFieldSizeLink() * getFieldSizeLink()];
    spawnSnake(snake_block, snake_length);
    std::cout << "\n";
    while (true)
    {
        createPlace(place);
        addSnakeOnPlace(snake_block, place);
        if (required_food_reposition == true) //если еда была в прошлый раз съедена, переразместить еду и удлинится
        {
            spawnLocationSelection(&food_pos_x, &food_pos_y, place);
            required_food_reposition = false;
        }
        place[food_pos_y][food_pos_x] = 2;
        system("cls");
        painter(place, game_score, skin);
        clock_mark = clock();
        std::cout << "\n";
        while ((clock() - clock_mark) < step_duration)
        {
            if (IsKeyPressed(87) && (snake_block[0].y - snake_block[1].y) != 1)
            {
                move_vector = 1;
            }
            else if (IsKeyPressed(68) && (snake_block[1].x - snake_block[0].x) != 1)
            {
                move_vector = 2;
            }
            else if (IsKeyPressed(83) && (snake_block[1].y - snake_block[0].y) != 1)
            {
                move_vector = 3;
            }
            else if (IsKeyPressed(65) && (snake_block[0].x - snake_block[1].x) != 1)
            {
                move_vector = 4;
            }
            else if (IsKeyPressed(27))
            {
                if (ask(getEPAsk()) == 1)
                {
                    *snake_exit = 1;
                    return game_score;
                }
                else
                {
                    painter(place, game_score, skin);
                }
            }
        }
        collider_type = checkCollide(snake_block[0], place, move_vector);
        if (collider_type == 1)
        {
            return game_score;
        }
        if (collider_type == 2) //что происходит если еда была съедена
        {
            step_duration = step_duration * 0.98;
            game_score++;
            snake_length++;
            snake_block[snake_length - 1].active = 1;
            required_food_reposition = true;
        }
        collider_type = 0;
        moveAll(move_vector, snake_block);
    }
    delete[] snake_block;
    return game_score;
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
    bool required_exit = 0;
    short needed_score[10]{ 0, 50, 100, 200, 300, 400, 550, 700, 850, 1024 };
    char skin[4] = "";
    int best_score = 0;
    int game_score = 0;
    int menu_choise = 1;
    loadGame(&best_score);
    srand(time(0));
    setskin(skin);
    while (true)
    {
        while (true)
        {
            std::cout << "Your best score: " << best_score << "\n\nMENU:\n";

            const char* menu_units[5]{ "Play\n", "Settings\n", "Help\n", "Authors\n", "Save and exit\n" };
            for (int i = 0; i < 5; i++)
            {
                PrintMenuUnit(menu_units[i], i + 1, menu_choise);
            }

            if (IsKeyJustPressed(87))
            {
                menu_choise = CyclicSwitch(menu_choise, 1, 5, false);
            }
            if (IsKeyJustPressed(83))
            {
                menu_choise = CyclicSwitch(menu_choise, 1, 5, true);
            }
            if (IsKeyJustPressed(13))
            {
                if (menu_choise == 1)
                {
                    game_score = snakeProcess(skin, &required_exit);
                    break;
                }
                if (menu_choise == 2)
                {
                    settingsProcess(skin, best_score, needed_score);
                }
                if (menu_choise == 3)
                {
                    system("cls");
                    std::cout << "Help:\n";
                    std::cout << "(w, a, s, d) - control\n";
                    std::cout << "(Enter) - interaction\n";
                    std::cout << ">Back\n";
                    WaitKeyPress(13);
                }
                if (menu_choise == 4)
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
                if (menu_choise == 5)
                {
                    saveGame(&best_score);
                    return 0;
                }
            }
            system("cls");
        }
        if (game_score > best_score)
        {
            best_score = game_score;
        }
        system("cls");
        if (required_exit == 1)
        {
            std::cout << "You been exit, your score: " << game_score << "\n\n>Continue";
        }
        else
        {
            printGameOver();
            if (game_score >= getFieldSizeLink() * getFieldSizeLink())
            {
                std::cout << "\n\nYou won! You have reached the maximum value on this place!";
            }
            std::cout << "\n\nYour score : " << game_score << "\n\n>Continue";
        }
        required_exit = 0;
        WaitKeyPress(13);
        system("cls");
    }
    return 0;
}