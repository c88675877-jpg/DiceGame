// Game.cpp
#include "Game.h"

IMAGE imgDice[6];
IMAGE imgBackground;
IMAGE imgWin, imgLose;

void DrawPanel(int x1, int y1, int x2, int y2, COLORREF fillColor, COLORREF borderColor) {
    setfillcolor(fillColor);
    setlinecolor(borderColor);
    fillroundrect(x1, y1, x2, y2, 12, 12);
}

void DrawDiceFace(IMAGE* img, int value) {
    SetWorkingImage(img);
    cleardevice();
    setbkmode(TRANSPARENT);
    setfillcolor(RGB(250, 250, 252));
    setlinecolor(RGB(200, 200, 210));
    fillroundrect(0, 0, 60, 60, 10, 10);
    setfillcolor(RGB(200, 50, 50));
    switch (value) {
    case 1: fillcircle(30, 30, 7); break;
    case 2: fillcircle(17, 17, 6); fillcircle(43, 43, 6); break;
    case 3: fillcircle(17, 17, 6); fillcircle(30, 30, 6); fillcircle(43, 43, 6); break;
    case 4: fillcircle(17, 17, 6); fillcircle(43, 17, 6); fillcircle(17, 43, 6); fillcircle(43, 43, 6); break;
    case 5: fillcircle(17, 17, 6); fillcircle(43, 17, 6); fillcircle(17, 43, 6); fillcircle(43, 43, 6); fillcircle(30, 30, 6); break;
    case 6: fillcircle(17, 14, 6); fillcircle(17, 30, 6); fillcircle(17, 46, 6); fillcircle(43, 14, 6); fillcircle(43, 30, 6); fillcircle(43, 46, 6); break;
    }
    SetWorkingImage(NULL);
}

void DrawBackground() {
    SetWorkingImage(&imgBackground);
    cleardevice();
    setbkcolor(RGB(25, 55, 109));
    cleardevice();
    setfillcolor(RGB(46, 125, 50));
    fillroundrect(20, 15, 780, 95, 20, 20);
    setbkmode(TRANSPARENT);
    settextstyle(56, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 230));
    outtextxy(280, 25, _T("骰子游戏"));
    DrawPanel(60, 115, 740, 575, RGB(30, 40, 65), RGB(100, 160, 230));
    SetWorkingImage(NULL);
}

void DrawResultImage(IMAGE* img, const TCHAR* text, COLORREF bgStart, COLORREF bgEnd) {
    SetWorkingImage(img);
    cleardevice();
    for (int i = 0; i < 100; i++) {
        COLORREF c = RGB(
            GetRValue(bgStart) + (GetRValue(bgEnd) - GetRValue(bgStart)) * i / 100,
            GetGValue(bgStart) + (GetGValue(bgEnd) - GetGValue(bgStart)) * i / 100,
            GetBValue(bgStart) + (GetBValue(bgEnd) - GetBValue(bgStart)) * i / 100);
        setlinecolor(c);
        line(0, i, 220, i);
    }
    setlinecolor(RGB(100, 230, 120));
    roundrect(0, 0, 219, 99, 12, 12);
    setbkmode(TRANSPARENT);
    settextstyle(40, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 230));
    outtextxy(50, 28, text);
    SetWorkingImage(NULL);
}

void InitGraphics() {
    srand((unsigned)time(NULL));
    for (int i = 0; i < 6; i++) {
        imgDice[i].Resize(64, 64);
        DrawDiceFace(&imgDice[i], i + 1);
    }
    imgBackground.Resize(800, 600);
    DrawBackground();
    imgWin.Resize(220, 100);
    DrawResultImage(&imgWin, _T("胜利!"), RGB(46, 160, 67), RGB(30, 120, 50));
    imgLose.Resize(220, 100);
    DrawResultImage(&imgLose, _T("失败"), RGB(210, 60, 50), RGB(160, 30, 25));
}

// 骰子滚动动画（居中位置）
void ShowRolling(int x, int y) {
    for (int i = 0; i < 12; i++) {
        // 清除区域：扩大边距，覆盖动画全程
        putimage(x - 10, y - 10, 164, 84, &imgBackground, x - 10, y - 10, SRCCOPY);
        putimage(x, y, &imgDice[rand() % 6]);
        putimage(x + 80, y, &imgDice[rand() % 6]);
        Sleep(50 + i * 15);
    }
}

// 掷骰子并显示结果（居中显示）
int RollDice() {
    int d1 = rand() % 6, d2 = rand() % 6;
    // 骰子起始坐标 (328,313) 在游戏区域居中
    int diceX = 328, diceY = 313;
    // 清除区域 (318,303) 到 (482,377) 完全覆盖骰子+边距
    clearrectangle(318, 303, 482, 377);
    ShowRolling(diceX, diceY);
    putimage(diceX, diceY, &imgDice[d1]);
    putimage(diceX + 80, diceY, &imgDice[d2]);
    int sum = d1 + d2 + 2;
    TCHAR str[50];
    _stprintf_s(str, _T("总和: %d"), sum);
    setbkmode(TRANSPARENT);
    settextstyle(44, 0, _T("黑体"));
    settextcolor(RGB(255, 215, 0));
    outtextxy(340, 395, str);   // 位于骰子下方居中
    return sum;
}

void DrawGameUI(int point) {
    putimage(0, 0, &imgBackground);
    setbkmode(TRANSPARENT);
    if (point > 0) {
        setfillcolor(RGB(255, 193, 7));
        fillroundrect(250, 118, 550, 165, 10, 10);
        TCHAR str[50];
        _stprintf_s(str, _T("目标点数: %d"), point);
        settextstyle(36, 0, _T("黑体"));
        settextcolor(RGB(80, 40, 0));
        outtextxy(310, 125, str);
    }
}

GameStatus Game() {
    DrawGameUI();
    int sum = RollDice();
    int point = 0;
    GameStatus status;
    if (sum == 7 || sum == 11) status = WIN;
    else if (sum == 2 || sum == 3 || sum == 12) status = LOSE;
    else {
        status = CONTINUE;
        point = sum;
        DrawGameUI(point);
    }

    while (status == CONTINUE) {
        setbkmode(TRANSPARENT);
        settextstyle(24, 0, _T("宋体"));
        settextcolor(RGB(150, 180, 220));
        outtextxy(290, 430, _T("按任意键继续投掷..."));
        _getch();
        putimage(0, 0, &imgBackground);
        DrawGameUI(point);
        sum = RollDice();
        if (sum == point) status = WIN;
        else if (sum == 7) status = LOSE;
    }
    putimage(290, 400, (status == WIN) ? &imgWin : &imgLose);
    _getch();
    return status;
}

void Help() {
    DrawGameUI();
    DrawPanel(100, 150, 700, 520, RGB(20, 30, 55), RGB(80, 140, 210));
    setbkmode(TRANSPARENT);
    settextstyle(40, 0, _T("黑体"));
    settextcolor(RGB(255, 215, 0));
    outtextxy(280, 165, _T("游戏规则"));
    settextstyle(22, 0, _T("宋体"));
    settextcolor(RGB(200, 210, 230));
    outtextxy(150, 235, _T("1. 第一次掷出 7 或 11，玩家获胜"));
    outtextxy(150, 270, _T("2. 第一次掷出 2、3 或 12，玩家失败"));
    outtextxy(150, 305, _T("3. 掷出其他数字，该数字成为目标点数"));
    outtextxy(150, 340, _T("4. 继续掷骰子，直到掷出目标点数 (获胜) 或 7 (失败)"));
    outtextxy(270, 470, _T("按任意键返回..."));
    _getch();
}

int ShowMainMenu() {
    DrawGameUI();
    setbkmode(TRANSPARENT);
    settextstyle(44, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(300, 160, _T("主菜单"));
    const TCHAR* items[] = { _T("1. 游戏帮助"), _T("2. 开始游戏"), _T("3. 退出游戏") };
    COLORREF colors[] = { RGB(52,152,219), RGB(46,204,113), RGB(231,76,60) };
    int btnX = 260, btnW = 280, btnH = 50, gap = 15;
    for (int i = 0; i < 3; i++) {
        int y = 240 + i * (btnH + gap);
        setfillcolor(colors[i]);
        fillroundrect(btnX, y, btnX + btnW, y + btnH, 10, 10);
        settextstyle(28, 0, _T("黑体"));
        settextcolor(RGB(255, 255, 255));
        outtextxy(btnX + (btnW - textwidth(items[i])) / 2, y + 12, items[i]);
    }
    settextstyle(20, 0, _T("宋体"));
    settextcolor(RGB(120, 150, 190));
    outtextxy(300, 480, _T("请选择 (1-3):"));
    return _getch() - '0';
}

void Run() {
    initgraph(800, 600, SHOWCONSOLE);
    InitGraphics();
    int total = 0, wins = 0;
    int choice;
    do {
        choice = ShowMainMenu();
        switch (choice) {
        case 1: Help(); break;
        case 2: {
            GameStatus res = Game();
            total++; if (res == WIN) wins++;
            DrawGameUI();
            DrawPanel(200, 200, 600, 400, RGB(20, 30, 55), RGB(100, 160, 230));
            setbkmode(TRANSPARENT);
            TCHAR str[100];
            _stprintf_s(str, _T("胜利: %d 次"), wins);
            settextstyle(28, 0, _T("黑体"));
            settextcolor(RGB(80, 230, 140));
            outtextxy(260, 280, str);
            _stprintf_s(str, _T("总计: %d 次"), total);
            settextcolor(RGB(200, 210, 230));
            outtextxy(260, 320, str);
            if (total > 0) {
                _stprintf_s(str, _T("胜率: %d%%"), wins * 100 / total);
                settextcolor(RGB(255, 215, 0));
                outtextxy(260, 360, str);
            }
            outtextxy(300, 440, _T("按任意键继续..."));
            _getch();
            break;
        }
        case 3:
            DrawGameUI();
            DrawPanel(200, 200, 600, 420, RGB(20, 30, 55), RGB(100, 160, 230));
            setbkmode(TRANSPARENT);
            TCHAR final[100];
            _stprintf_s(final, _T("最终战绩: %d 胜 / %d 局"), wins, total);
            settextstyle(28, 0, _T("黑体"));
            settextcolor(RGB(200, 210, 230));
            outtextxy(240, 285, final);
            outtextxy(280, 390, _T("按任意键退出..."));
            _getch();
            break;
        }
    } while (choice != 3);
    closegraph();
}