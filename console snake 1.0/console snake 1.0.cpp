// Console Snake 0.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <string>
#include <Windows.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unordered_map>
int polsiz = 16;
int snakespeed = 2;
int dbarrier = 0;
int numskin = 1;
int squareps;



class cell
{
public:
    int x;
    int y;
    cell(int a = 0, int b = 0);
    void set(int a, int b);
};
void cell::set(int a, int b)
{
    x = a;
    y = b;
}
cell::cell(int a, int b)
{
    x = a;
    y = b;
}
//Реализация класса block ниже ------------------------
class block
{
public:
    block(int a = 0, int b = 0);
    bool active;
    int x;
    int y;
    int lastx;
    int lasty;
    void set(int, int);
    void move(int, int);
    void spawn(int, int);
};
void block::set(int lr, int du)
{
    lastx = x;
    lasty = y;
    x = lr;
    y = du;
}
void block::move(int lr, int du)
{
    lastx = x;
    lasty = y;
    if (x == polsiz - 1 && lr == 1)
    {
        x = 0;
    }
    else
    {
        if (x == 0 && lr == -1)
        {
            x = polsiz - 1;
        }
        else
        {
            x = x + lr;
        }
    }
    if (y == polsiz - 1 && du == 1)
    {
        y = 0;
    }
    else
    {
        if (y == 0 && du == -1)
        {
            y = polsiz - 1;
        }
        else
        {
            y = y + du;
        }
    }
}
block::block(int a, int b)
{
    active = 0;
    x = a;
    y = b;
    lastx = x;
    lasty = y;
}
void block::spawn(int a, int b)
{
    active = 1;
    x = a;
    y = b;
}
//-----------------------------------------------------

//----------------новые функции-----------------
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
        if (IsKeyPressed(key)) {
            return;
        }
    }
}
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
//-----------------------------------------------------

//---------------сохранения и загрузки-----------------
void sav(int* bestscore)
{
    std::ofstream saving;
    saving.open("save.snake", std::ios::out | std::ios::binary);
    saving.write((const char*)&polsiz, sizeof polsiz);
    saving.write((const char*)&snakespeed, sizeof snakespeed);
    saving.write((const char*)&dbarrier, sizeof dbarrier);
    saving.write((const char*)&numskin, sizeof numskin);
    saving.write((const char*)bestscore, sizeof *bestscore);
    saving.close();
}
void lod(int* bestscore)
{
    std::ifstream loading;
    loading.open("save.snake", std::ios::in | std::ios::binary);
    loading.read((char*)&polsiz, sizeof polsiz);
    loading.read((char*)&snakespeed, sizeof snakespeed);
    loading.read((char*)&dbarrier, sizeof dbarrier);
    loading.read((char*)&numskin, sizeof numskin);
    loading.read((char*)bestscore, sizeof * bestscore);
    loading.close();
}
void clear(int* bestscore)
{
    std::fstream clear;
    clear.open("save.snake", std::ios::trunc | std::ios::binary);
    clear.close();
    polsiz = 16;
    snakespeed = 2;
    dbarrier = 0;
    numskin = 1;
    *bestscore = 0;
}
//-----------------------------------------------------


void spawn_location_selection(int* x, int* y, int place[32][32])
{
    int randcell;
    int i;
    int a;
    int b;
    int amount = 0;
    cell* emty_place = new cell[polsiz * polsiz];
    i = 0;
    a = 0;
    while (a < polsiz)
    {
        b = 0;
        while (b < polsiz)
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
int rest(int a, int b)
{
    a = a + b;
    if (a == polsiz)
    {
        a = 0;
    }
    else
    {
        if (a == -1)
        {
            a = polsiz - 1;
        }
    }
    return a;
}
int dead(block snakeb, int place[32][32], int l, int a, int b)
{
    if (dbarrier == 1)
    {
        if ((place[rest(snakeb.y, -1)][snakeb.x] == 1 && l == 1) || (snakeb.y == 0 && l == 1))
        {
            return 1;
        }
        else
        {
            if (place[rest(snakeb.y, -1)][snakeb.x] == 2 && l == 1)
            {
                return 2;
            }
        }
        if ((place[snakeb.y][rest(snakeb.x, 1)] == 1 && l == 2) || (snakeb.x == polsiz - 1 && l == 2))
        {
            return 1;
        }
        else
        {
            if (place[snakeb.y][rest(snakeb.x, 1)] == 2 && l == 2)
            {
                return 2;
            }
        }
        if ((place[rest(snakeb.y, 1)][snakeb.x] == 1 && l == 3) || (snakeb.y == polsiz - 1 && l == 3))
        {
            return 1;
        }
        else
        {
            if (place[rest(snakeb.y, 1)][snakeb.x] == 2 && l == 3)
            {
                return 2;
            }
        }
        if ((place[snakeb.y][rest(snakeb.x, -1)] == 1 && l == 4) || (snakeb.x == 0 && l == 4))
        {
            return 1;
        }
        else
        {
            if (place[snakeb.y][rest(snakeb.x, -1)] == 2 && l == 4)
            {
                return 2;
            }
        }
    }
    else
    {
        if (place[rest(snakeb.y, -1)][snakeb.x] == 1 && l == 1)
        {
            return 1;
        }
        else
        {
            if (place[rest(snakeb.y, -1)][snakeb.x] == 2 && l == 1)
            {
                return 2;
            }
        }
        if (place[snakeb.y][rest(snakeb.x, 1)] == 1 && l == 2)
        {
            return 1;
        }
        else
        {
            if (place[snakeb.y][rest(snakeb.x, 1)] == 2 && l == 2)
            {
                return 2;
            }
        }
        if (place[rest(snakeb.y, 1)][snakeb.x] == 1 && l == 3)
        {
            return 1;
        }
        else
        {
            if (place[rest(snakeb.y, 1)][snakeb.x] == 2 && l == 3)
            {
                return 2;
            }
        }
        if (place[snakeb.y][rest(snakeb.x, -1)] == 1 && l == 4)
        {
            return 1;
        }
        else
        {
            if (place[snakeb.y][rest(snakeb.x, -1)] == 2 && l == 4)
            {
                return 2;
            }
        }
    }
    return 0;
}
//-----------------------------------------------------
void fullzapis(char* inpstr, const char* outstr, int inlenth)
{
    int inlf = 0;
    while (*outstr != '\0' && inlf < inlenth - 1)
    {
        *inpstr = *outstr;
        inlf++;
        inpstr++;
        outstr++;
    }
    while (inlf < inlenth)
    {
        *inpstr = '\0';
        inlf++;
        inpstr++;
    }
}
void zapis(char skin[4], const char setch[4])
{
    int i = 0;
    while (i < 4)
    {
        skin[i] = setch[i];
        i++;
    }
    return;
}
void setskin(char skin[4])
{
    int i = 0;
    while (i < 4)
    {
        skin[i] = '\0';
        i++;
    }
    switch (numskin)
    {
    case 1:
    {
        zapis(skin, "|-|");
    }
    break;
    case 2:
    {
        zapis(skin, "|=|");
    }
    break;
    case 3:
    {
        zapis(skin, "|+|");
    }
    break;
    case 4:
    {
        zapis(skin, "[-]");
    }
    break;
    case 5:
    {
        zapis(skin, "[=]");
    }
    break;
    case 6:
    {
        zapis(skin, "[+]");
    }
    break;
    case 7:
    {
        zapis(skin, "(-)");
    }
    break;
    case 8:
    {
        zapis(skin, "(=)");
    }
    break;
    case 9:
    {
        zapis(skin, "(+)");
    }
    break;
    case 10:
    {
        zapis(skin, "111");
    }
    break;
    }
    return;
}
void char_to_int_adder(char* canvas, int toadd)
{
    std::string char_num = { "" };
    int i = 0;
    int i2 = 0;
    char_num = std::to_string(toadd);
    while (canvas[i] != '\0')
    {
        i++;
    }
    while (char_num[i2] != '\0')
    {
        canvas[i] = char_num[i2];
        i++;
        i2++;
    }
}
void setrank(char* nameskin, int can, int need_record)
{
    if (can == 1)
    {
        switch (numskin)
        {
        case 1:
        {
            fullzapis(nameskin, "Newbie", 64);
        }
        break;
        case 2:
        {
            fullzapis(nameskin, "Beginning", 64);
        }
        break;
        case 3:
        {
            fullzapis(nameskin, "Apprentice", 64);
        }
        break;
        case 4:
        {
            fullzapis(nameskin, "Advanced", 64);
        }
        break;
        case 5:
        {
            fullzapis(nameskin, "Professional", 64);
        }
        break;
        case 6:
        {
            fullzapis(nameskin, "Specialist", 64);
        }
        break;
        case 7:
        {
            fullzapis(nameskin, "Master", 64);
        }
        break;
        case 8:
        {
            fullzapis(nameskin, "Best master", 64);
        }
        break;
        case 9:
        {
            fullzapis(nameskin, "Legend", 64);
        }
        break;
        case 10:
        {
            fullzapis(nameskin, "***BINARY MONSTER***", 64);
        }
        break;
        }
    }
    else
    {
        fullzapis(nameskin, "Locked, need score: ", 64);
        char_to_int_adder(nameskin, need_record);
    }
    return;
}
void setpainter(int vibor2, char skin[4], int bestscore, short need_record[10]) //Рисует Меню настроек ББББББББББББББj
{
    bool can = 0;
    char name_skin[64]{ "" };
    char setiskin[3][4];
    if (bestscore < need_record[numskin - 1])
    {
        zapis(setiskin[0], "X X");
        zapis(setiskin[1], " X ");
        zapis(setiskin[2], "X X");
        can = 0;
    }
    else
    {
        zapis(setiskin[0], skin);
        zapis(setiskin[1], skin);
        zapis(setiskin[2], skin);
        can = 1;
    }
    setrank(name_skin, can, need_record[numskin - 1]);
    std::cout << "SETTINGS:\n";
    if (vibor2 == 1)
        std::cout << ">size of place";
    else
        std::cout << " size of place";
    std::cout << "\t\t< " << polsiz << " X " << polsiz << " >" << "\t" << setiskin[0] << "\n";
    if (vibor2 == 2)
        std::cout << ">speed of snake";
    else
        std::cout << " speed of snake";
    std::cout << "\t\t< " << snakespeed << " m/s >" << "\t" << setiskin[1] << "\t" << name_skin << "\n";
    if (vibor2 == 3)
        std::cout << ">collision with edges";
    else
        std::cout << " collision with edges";
    if (dbarrier == 1)
    {
        std::cout << "\t< YES >";
    }
    else
    {
        std::cout << "\t< NO >";
    }
    std::cout << "\t\t" << setiskin[2] << "\n";
    if (vibor2 == 4)
        std::cout << ">Skin of snake:";
    else
        std::cout << " Skin of snake:";
    std::cout << "\t\t<" << numskin << ">\n";
    if (vibor2 == 5)
        std::cout << ">Save game\n";
    else
        std::cout << " Save game\n";
    if (vibor2 == 6)
        std::cout << ">Clear save\n";
    else
        std::cout << " Clear save\n";
    if (vibor2 == 7)
        std::cout << ">back\n";
    else
        std::cout << " back\n";
    std::cout << "\t\t\t\t";
    return;
}

void allmove(int vector, block snakeb[1024])
{
    int i = 1;
    if (vector == 1)
    {
        snakeb[0].move(0, -1);
        while (snakeb[i].active)
        {
            snakeb[i].set(snakeb[i - 1].lastx, snakeb[i - 1].lasty);
            i++;
        }
    }
    if (vector == 2)
    {
        snakeb[0].move(1, 0);
        while (snakeb[i].active)
        {
            snakeb[i].set(snakeb[i - 1].lastx, snakeb[i - 1].lasty);
            i++;
        }
    }
    if (vector == 3)
    {
        snakeb[0].move(0, 1);
        while (snakeb[i].active)
        {
            snakeb[i].set(snakeb[i - 1].lastx, snakeb[i - 1].lasty);
            i++;
        }
    }
    if (vector == 4)
    {
        snakeb[0].move(-1, 0);
        while (snakeb[i].active)
        {
            snakeb[i].set(snakeb[i - 1].lastx, snakeb[i - 1].lasty);
            i++;
        }
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
void char_to_char_adder(char* canvas, const char str_for_add[])
{
    int i = 0;
    int i2 = 0;
    while (canvas[i] != '\0')
    {
        i++;
    }
    while (str_for_add[i2] != '\0')
    {
        canvas[i] = str_for_add[i2];
        i++;
        i2++;
    }
}
int painter(int place[32][32], int score, char skin[4])
{
    char* canvas = new char[3500]{ "" };
    int a = 0;
    int b = 0;
    int c = 0;
    while (c < polsiz)
    {
        char_to_char_adder(canvas, "---");
        c++;
    }
    char_to_char_adder(canvas, "--\tscore:");
    char_to_int_adder(canvas, score);
    char_to_char_adder(canvas, "\n");
    while (b < polsiz)
    {
        a = 0;
        char_to_char_adder(canvas, "|");
        while (a < polsiz)
        {
            if (place[b][a] == 1)
            {
                char_to_char_adder(canvas, skin);
            }
            else
            {
                if (place[b][a] == 2)
                {
                    char_to_char_adder(canvas, "(*)");
                }
                else
                {
                    char_to_char_adder(canvas, "   ");
                }
            }
            a++;
        }
        char_to_char_adder(canvas, "|\n");
        b++;
    }
    c = 0;
    while (c < polsiz)
    {
        char_to_char_adder(canvas, "---");
        c++;
    }
    char_to_char_adder(canvas, "--\n");
    std::cout << canvas;
    delete[] canvas;
    return 0;
}
bool long_press(int a, int* dolgnaz)
{
    if (GetAsyncKeyState(a) == -32768 && *dolgnaz != a)
    {
        *dolgnaz = a;
        return 1;
    }
    else
    {
        return 0;
    }
}
void ask_ep(int vibor)
{
    system("cls");
    std::cout << "You are sure exit?\n";
    if (vibor == 1)
    {
        std::cout << ">";
    }
    else
    {
        std::cout << " ";
    }
    std::cout << "No\n";
    if (vibor == 2)
    {
        std::cout << ">";
    }
    else
    {
        std::cout << " ";
    }
    std::cout << "Yes\n";
}
bool ask_exit(int* dolgnaz)
{
    *dolgnaz = 0;
    short vibor = 1;
    ask_ep(vibor);
    while (1)
    {
        if (GetAsyncKeyState(87) == 0 && *dolgnaz == 1)
        {
            *dolgnaz = 0;
        }
        if (GetAsyncKeyState(83) == 0 && *dolgnaz == 2)
        {
            *dolgnaz = 0;
        }
        if (GetAsyncKeyState(13) == 0 && *dolgnaz == 3)
        {
            *dolgnaz = 0;
        }
        if (GetAsyncKeyState(87) == -32768 && GetAsyncKeyState(83) == 0 && *dolgnaz != 1)
        {
            vibor--;
            if (vibor < 1)
                vibor = 2;
            *dolgnaz = 1;
            ask_ep(vibor);
        }
        if (GetAsyncKeyState(83) == -32768 && GetAsyncKeyState(87) == 0 && *dolgnaz != 2)
        {
            vibor++;
            if (vibor > 2)
                vibor = 1;
            *dolgnaz = 2;
            ask_ep(vibor);
        }
        if (GetAsyncKeyState(13) == -32768 && *dolgnaz != 3)
        {
            *dolgnaz = 3;
            system("cls");
            if (vibor == 1)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    return 0;
}
void ask_cp(int vibor)
{
    system("cls");
    std::cout << "You are sure clear your save?\n";
    if (vibor == 1)
    {
        std::cout << ">";
    }
    else
    {
        std::cout << " ";
    }
    std::cout << "No\n";
    if (vibor == 2)
    {
        std::cout << ">";
    }
    else
    {
        std::cout << " ";
    }
    std::cout << "Yes\n";
}
bool ask_clear(int* dolgnaz)
{
    short vibor = 1;
    ask_cp(vibor);
    while (1)
    {
        if (GetAsyncKeyState(87) == 0 && *dolgnaz == 1)
        {
            *dolgnaz = 0;
        }
        if (GetAsyncKeyState(83) == 0 && *dolgnaz == 2)
        {
            *dolgnaz = 0;
        }
        if (GetAsyncKeyState(13) == 0 && *dolgnaz == 3)
        {
            *dolgnaz = 0;
        }
        if (GetAsyncKeyState(87) == -32768 && GetAsyncKeyState(83) == 0 && *dolgnaz != 1)
        {
            vibor--;
            if (vibor < 1)
                vibor = 2;
            *dolgnaz = 1;
            ask_ep(vibor);
        }
        if (GetAsyncKeyState(83) == -32768 && GetAsyncKeyState(87) == 0 && *dolgnaz != 2)
        {
            vibor++;
            if (vibor > 2)
                vibor = 1;
            *dolgnaz = 2;
            ask_ep(vibor);
        }
        if (GetAsyncKeyState(13) == -32768 && *dolgnaz != 3)
        {
            *dolgnaz = 3;
            system("cls");
            if (vibor == 1)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    return 0;
}
int snake(char skin[4], bool* snake_exit, int* dolgnaz)
{
    squareps = polsiz * polsiz;
    bool first_launch = 1;
    int score = 0;
    int timetoblock = 1000 / snakespeed;
    int halfplace = polsiz / 2;
    int lenth = 3;
    int eat_resp = 1;
    int col = 0;
    int lr = 0;
    int ud = 0;
    int vector = 1;
    clock_t t1;
    int i;
    int a;
    int b;
    int place[32][32];
    block* snakeb = new block[squareps];
    i = 0;
    while (i < lenth) //спавн змейки
    {
        snakeb[i].spawn(halfplace, halfplace + i);
        i++;
    }
    std::cout << "\n";
    while (1)
    {

        b = 0;
        while (b < 32) //создание поля
        {
            a = 0;
            while (a < 32)
            {
                place[b][a] = 0;
                a++;
            }
            b++;
        }

        i = 0;
        while (snakeb[i].active == 1) //размещение созданных блоков змейки на поле
        {
            place[snakeb[i].y][snakeb[i].x] = 1;
            i++;
        }
        if (eat_resp == 1) //если еда была в прошлый раз съедена, переразместить еду и удлинится
        {
            spawn_location_selection(&lr, &ud, place);
            eat_resp = 0;
        }
        place[ud][lr] = 2;
        system("cls");
        painter(place, score, skin);
        t1 = clock();
        std::cout << "\n";
        while ((clock() - t1) < timetoblock)
        {
            if (GetAsyncKeyState(87) == -32768 && (snakeb[0].y - snakeb[1].y) != 1)
            {
                vector = 1;
            }
            else
            {
                if (GetAsyncKeyState(68) == -32768 && (snakeb[1].x - snakeb[0].x) != 1)
                {
                    vector = 2;
                }
                else
                {
                    if (GetAsyncKeyState(83) == -32768 && (snakeb[1].y - snakeb[0].y) != 1)
                    {
                        vector = 3;
                    }
                    else
                    {
                        if (GetAsyncKeyState(65) == -32768 && (snakeb[0].x - snakeb[1].x) != 1)
                        {
                            vector = 4;
                        }
                        else
                        {
                            if (GetAsyncKeyState(27) == -32768)
                            {
                                if (ask_exit(dolgnaz) == 1)
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
                    }
                }
            }
        }
        col = dead(snakeb[0], place, vector, ud, lr);
        if (col == 1)
        {
            return score;
        }
        if (col == 2) //что происходит если еда была съедена
        {
            timetoblock = timetoblock * 0.98;
            score++;
            lenth++;
            snakeb[lenth - 1].active = 1;
            eat_resp = 1;
        }
        col = 0;
        allmove(vector, snakeb);
    }
    delete[] snakeb;
    return score;
}
short skin_max(int bestscore, short need_record[10])
{
    short max_skin = 0;
    while (bestscore >= need_record[max_skin])
    {
        max_skin++;
    }
    return max_skin;
}
bool skin_check(int bestscore, short need_record[10])
{
    if (bestscore < need_record[numskin - 1])
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
int main()
{
    bool snakeexit = 0;
    short need_record[10]{ 0, 50, 100, 200, 300, 400, 550, 700, 850, 1024 };
    char skin[4] = "";
    int bestscore = 0;
    int score = 0;
    int vibvect = 1;
    int dubleexit;
    int vibor = 1;
    int vibor2 = 1;
    int st;
    int dolgnaz = 0;
    lod(&bestscore);
    srand(time(0));
    setskin(skin);
    while (1)
    {
        st = 0;
        while (st != 1)
        {
            std::cout << "Your best score: " << bestscore << "\n\nMENU:\n";

            std::string menu_units[5]{"Play", "Settings", "Help", "Authors", "Save and exit"};
            for (int i = 0; i < 5; i++)
            {
                if (i == vibor - 1)
                {
                    std::cout << ">";
                }
                else
                {
                    std::cout << " ";
                }
                std::cout << menu_units[i] << "\n";
            }

            while (1)
            {
                if (GetAsyncKeyState(13) == 0 && dolgnaz == 3)
                {
                    dolgnaz = 0;
                }
                if (GetAsyncKeyState(87) == 0 && dolgnaz == 1)
                {
                    dolgnaz = 0;
                }
                if (GetAsyncKeyState(83) == 0 && dolgnaz == 2)
                {
                    dolgnaz = 0;
                }

                if (IsKeyJustPressed(87))
                {
                    vibor--;
                    if (vibor < 1)
                        vibor = 5;
                    break;
                }
                if (IsKeyJustPressed(83))
                {
                    vibor++;
                    if (vibor > 5)
                        vibor = 1;
                    break;
                }
                if (IsKeyJustPressed(13))
                {
                    if (vibor == 1)
                    {
                        st = 1;
                        break;
                    }
                    if (vibor == 2) //Вход в настройки
                    {
                        dubleexit = 0;
                        system("cls");
                        vibor2 = 1;
                        vibvect = 1;
                        while (dubleexit == 0)
                        {
                            setskin(skin);
                            setpainter(vibor2, skin, bestscore, need_record);
                            while (1) //Ждет нажатия клавиш в настройках
                            {
                                if (IsKeyJustPressed(87))
                                {
                                    vibvect = 1;
                                    break;
                                }
                                if (IsKeyJustPressed(83))
                                {
                                    vibvect = 3;
                                    break;
                                }
                                if (IsKeyJustPressed(68))
                                {
                                    vibvect = 2;
                                    break;
                                }
                                if (IsKeyJustPressed(65))
                                {
                                    vibvect = 4;
                                    break;
                                }
                                if (IsKeyJustPressed(13))
                                {
                                    vibvect = 5;
                                    break;
                                }
                            }
                            if (vibvect == 1)
                            {
                                vibor2 = CyclicSwitch(vibor2, 1, 7, false);
                            }
                            if (vibvect == 3)
                            {
                                vibor2 = CyclicSwitch(vibor2, 1, 7, true);
                            }
                            if (vibvect == 5 && vibor2 == 7)
                            {
                                dubleexit = 1;
                            }
                            if (vibvect == 5 && vibor2 == 6)
                            {
                                if (ask_clear(&dolgnaz))
                                {
                                    clear(&bestscore);
                                }
                            }
                            if (vibvect == 5 && vibor2 == 5)
                            {
                                sav(&bestscore);
                            }
                            if (vibvect == 2 || vibvect == 4)
                            {
                                switch (vibor2)
                                {
                                case 1:
                                    polsiz = CyclicSwitch(polsiz, 16, 32, vibvect == 2);
                                    break;
                                case 2:
                                    snakespeed = CyclicSwitch(snakespeed, 2, 10, vibvect == 2);
                                    break;
                                case 3:
                                    dbarrier = CyclicSwitch(dbarrier, 0, 1, vibvect == 2);
                                    break;
                                case 4:
                                    numskin = CyclicSwitch(numskin, 1, 10, vibvect == 2);
                                    break;
                                }
                            }
                            //вот здесь начинается обработка выбора и скроллов
                            system("cls");
                            vibvect = 1;
                        }
                        if (skin_check(bestscore, need_record) == 0)
                        {
                            numskin = skin_max(bestscore, need_record);
                            setskin(skin);
                        }
                        if (dubleexit == 1)
                        {
                            dolgnaz = 3;
                            dubleexit = 0;
                            break;
                        }
                    }
                    if (vibor == 3)
                    {
                        system("cls");
                        std::cout << "Help:\n(w, a, s, d) - control\n(Enter) - interaction\n>Back\n";
                        WaitKeyPress(13);
                    }
                    if (vibor == 4)
                    {
                        system("cls");
                        std::cout << "Authors:\nProgramming - Nikitos\nGraphic - Nikitos\nAnimations - Nikitos\nPhysics - Nikitos\nVoice acting - Nikitos\n>Back\n";
                        WaitKeyPress(13);
                    }
                    if (vibor == 5)
                    {
                        sav(&bestscore);
                        return 0;
                    }
                }
            }
            system("cls");
        }
        score = snake(skin, &snakeexit, &dolgnaz);
        if (score > bestscore)
        {
            bestscore = score;
        }
        system("cls");
        if (snakeexit == 1)
        {
            std::cout << "You been exit, your score: " << score << "\n\n>Continue";
        }
        else
        {
            if (score >= polsiz * polsiz)
            {
                std::cout << "XXXX XXXX XX XX XXXX   XXXX  X  X XXXX X XX \nX    X  X X X X X      X  X  X  X X    XX  X\nX XX XXXX X X X XXXX   X  X  X  X XXXX X    ";
                std::cout << "\nX  X X  X X X X X      X  X  X  X X    X    \nXXXX X  X X X X XXXX   XXXX   XX  XXXX X    \n\nYou won! You have reached the maximum value on this place!" << "\n\nYour score: ";
                std::cout << score << "\n\n>Continue";
            }
            else
            {
                std::cout << "XXXX XXXX XX XX XXXX   XXXX  X  X XXXX X XX \nX    X  X X X X X      X  X  X  X X    XX  X\nX XX XXXX X X X XXXX   X  X  X  X XXXX X    ";
                std::cout << "\nX  X X  X X X X X      X  X  X  X X    X    \nXXXX X  X X X X XXXX   XXXX   XX  XXXX X    \n\nYour score: " << score << "\n\n>Continue";
            }
        }
        snakeexit = 0;
        while (1)
        {
            if (GetAsyncKeyState(13) == 0)
            {
                dolgnaz = 0;
            }
            if (GetAsyncKeyState(13) == -32768 && dolgnaz != 3)
            {
                dolgnaz = 3;
                dubleexit = 1;
                break;
            }
        }
        system("cls");
    }
    return 0;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
