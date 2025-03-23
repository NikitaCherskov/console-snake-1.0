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



void PrintMenuUnit(const char* text, int number, int choosen) {
    if (choosen == number) {
        std::cout << ">" << text;
    }
    else {
        std::cout << " " << text;
    }
}





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
int rest(int a)
{
    if (a == polsiz)
    {
        a = 0;
    }
    else if(a == -1)
    {
        a = polsiz - 1;
    }
    return a;
}
int dead(block snakeb, int place[32][32], int l)
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
    std::cout << x_delta << ";" << y_delta;
    int x_next = snakeb.x + x_delta;
    int y_next = snakeb.y + y_delta;
    if (dbarrier == 1)
    {
        if (x_next == -1 || x_next == polsiz) {
            return 1;
        }
        if (y_next == -1 || y_next == polsiz) {
            return 1;
        }
    }
    return place[rest(y_next)][rest(x_next)];
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
    const char* skins[10] = {"|-|", "|=|", "|+|", "[-]", "[=]", "[+]", "(-)", "(=)" , "(+)", "111"};
    zapis(skin, skins[numskin - 1]);
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
        fullzapis(nameskin, ranks[numskin], 64);
    }
    else
    {
        fullzapis(nameskin, "Locked, need score: ", 64);
        char_to_int_adder(nameskin, need_record);
    }
    return;
}



void drawSettings(int vibor2, char skin[4], int bestscore, short need_record[10])
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
    PrintMenuUnit("size of place", 1, vibor2);
    std::cout << "\t\t< " << polsiz << " X " << polsiz << " >" << "\t" << setiskin[0] << "\n";
    PrintMenuUnit("speed of snake", 2, vibor2);
    std::cout << "\t\t< " << snakespeed << " m/s >" << "\t" << setiskin[1] << "\t" << name_skin << "\n";
    PrintMenuUnit("collision with edges", 3, vibor2);
    if (dbarrier == 1) {
        std::cout << "\t< YES >";
    }
    else {
        std::cout << "\t< NO >";
    }
    std::cout << "\t\t" << setiskin[2] << "\n";
    PrintMenuUnit("Skin of snake:", 4, vibor2);
    std::cout << "\t\t<" << numskin << ">\n";
    PrintMenuUnit("Save game\n", 5, vibor2);
    PrintMenuUnit("Clear save\n", 6, vibor2);
    PrintMenuUnit("back\n", 7, vibor2);
    std::cout << "\t\t\t\t";
    return;
}



void allmove(int vector, block snakeb[1024])
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
    for (int i = 0; i < polsiz; i++)
    {
        char_to_char_adder(canvas, "---");
    }
    char_to_char_adder(canvas, "--\tscore:");
    char_to_int_adder(canvas, score);
    char_to_char_adder(canvas, "\n");
    for (int i = 0; i < polsiz; i++)
    {
        char_to_char_adder(canvas, "|");
        for (int j = 0; j < polsiz; j++)
        {
            if (place[i][j] == 1)
            {
                char_to_char_adder(canvas, skin);
            }
            else if(place[i][j] == 2)
            {
                char_to_char_adder(canvas, "(*)");
            }
            else
            {
                char_to_char_adder(canvas, "   ");
            }
        }
        char_to_char_adder(canvas, "|\n");
    }
    for (int i = 0; i < polsiz; i++)
    {
        char_to_char_adder(canvas, "---");
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



const char* ep_ask = "You are sure exit?\n";
const char* cp_ask = "You are sure clear your save?\n";



void ask_print(int choose, const char* text)
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
        ask_print(choose, text);
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



int snake(char skin[4], bool* snake_exit)
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
                if (ask(ep_ask) == 1)
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
        col = dead(snakeb[0], place, vector);
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
    return bestscore >= need_record[numskin - 1];
}



void settingsProcess(char* skin, int best_score, short* need_record) {
    system("cls");
    int vibvect = 1;
    int vibor2 = 1;
    bool exit = false;
    while (!exit)
    {
        setskin(skin);
        drawSettings(vibor2, skin, best_score, need_record);

        if (IsKeyJustPressed(87))
        {
            vibor2 = CyclicSwitch(vibor2, 1, 7, false);
        }
        if (IsKeyJustPressed(83))
        {
            vibor2 = CyclicSwitch(vibor2, 1, 7, true);
        }
        if (IsKeyJustPressed(13) && vibor2 == 7)
        {
            exit = true;
        }
        if (IsKeyJustPressed(13) && vibor2 == 6)
        {
            if (ask(cp_ask))
            {
                clear(&best_score);
            }
        }
        if (IsKeyJustPressed(13) && vibor2 == 5)
        {
            sav(&best_score);
        }
        if (IsKeyJustPressed(68) || IsKeyJustPressed(65))
        {
            switch (vibor2)
            {
            case 1:
                polsiz = CyclicSwitch(polsiz, 16, 32, IsKeyJustPressed(68));
                break;
            case 2:
                snakespeed = CyclicSwitch(snakespeed, 2, 10, IsKeyJustPressed(68));
                break;
            case 3:
                dbarrier = CyclicSwitch(dbarrier, 0, 1, IsKeyJustPressed(68));
                break;
            case 4:
                numskin = CyclicSwitch(numskin, 1, 10, IsKeyJustPressed(68));
                break;
            }
        }
        system("cls");
        vibvect = 1;
    }
    if (skin_check(best_score, need_record) == 0)
    {
        numskin = skin_max(best_score, need_record);
        setskin(skin);
    }
    if (exit == true)
    {
        return;
    }
}



int main()
{
    bool snakeexit = 0;
    short need_record[10]{ 0, 50, 100, 200, 300, 400, 550, 700, 850, 1024 };
    char skin[4] = "";
    int best_score = 0;
    int score = 0;
    int vibor = 1;
    bool exit = false;
    lod(&best_score);
    srand(time(0));
    setskin(skin);
    while (1)
    {
        exit = false;
        while (!exit)
        {
            std::cout << "Your best score: " << best_score << "\n\nMENU:\n";

            const char* menu_units[5]{"Play\n", "Settings\n", "Help\n", "Authors\n", "Save and exit\n"};
            for (int i = 0; i < 5; i++)
            {
                PrintMenuUnit(menu_units[i], i + 1, vibor);
            }

            if (IsKeyJustPressed(87))
            {
                vibor = CyclicSwitch(vibor, 1, 5, false);
            }
            if (IsKeyJustPressed(83))
            {
                vibor = CyclicSwitch(vibor, 1, 5, true);
            }
            if (IsKeyJustPressed(13))
            {
                if (vibor == 1)
                {
                    score = snake(skin, &snakeexit);
                }
                if (vibor == 2)
                {
                    settingsProcess(skin, best_score, need_record);
                }
                if (vibor == 3)
                {
                    system("cls");
                    std::cout << "Help:\n";
                    std::cout << "(w, a, s, d) - control\n";
                    std::cout << "(Enter) - interaction\n";
                    std::cout << ">Back\n";
                    WaitKeyPress(13);
                }
                if (vibor == 4)
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
                if (vibor == 5)
                {
                    sav(&best_score);
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
            const char* owr_txt_1 = "XXXX XXXX XX XX XXXX   XXXX  X  X XXXX X XX \n";
            const char* owr_txt_2 = "X    X  X X X X X      X  X  X  X X    XX  X\n";
            const char* owr_txt_3 = "X XX XXXX X X X XXXX   X  X  X  X XXXX X    \n";
            const char* owr_txt_4 = "X  X X  X X X X X      X  X  X  X X    X    \n";
            const char* owr_txt_5 = "XXXX X  X X X X XXXX   XXXX   XX  XXXX X    \n";
            std::cout << owr_txt_1 << owr_txt_2 << owr_txt_3 << owr_txt_4 << owr_txt_5;
            if (score >= polsiz * polsiz)
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