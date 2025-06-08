#include "TextUtils.h"
#include "Globals.h"
#include <string>

void textCopy(char* inpstr, const char* outstr, int inlenth)
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

void skinTextCopy(char skin[4], const char setch[4])
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
    const char* skins[10] = { "|-|", "|=|", "|+|", "[-]", "[=]", "[+]", "(-)", "(=)" , "(+)", "111" };
    skinTextCopy(skin, skins[getSkinNumberLink() - 1]);
}

void addIntToChar(char* canvas, int toadd)
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

void addCharToChar(char* canvas, const char str_for_add[])
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
    canvas[i] = '\0';
}