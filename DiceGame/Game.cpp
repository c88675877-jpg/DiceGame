
#include "Game.h"

IMAGE imgDice[6];
IMAGE imgBackground;
IMAGE imgWin, imgLose;

// 按钮矩形区域 (用于鼠标点击)
struct Button {
    int x1, y1, x2, y2;
};

Button btnHelp, btnStart, btnExit;      // 主菜单按钮
Button btnRoll;                         // 游戏中"掷骰子"按钮

// 通用圆角面板
void DrawPanel(int x1, int y1, int x2, int y2, COLORREF fillColor, COLORREF borderColor) {
    setfillcolor(fillColor);
    setlinecolor(borderColor);
    fillroundrect(x1, y1, x2, y2, 12, 12);
}

// 绘制按钮
void DrawButton(int x1, int y1, int x2, int y2, COLORREF color, const TCHAR* text, int textSize = 28) {
    setfillcolor(color);
    setlinecolor(RGB(200, 200, 200));
    fillroundrect(x1, y1, x2, y2, 8, 8);
    setbkmode(TRANSPARENT);
    settextstyle(textSize, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 255));
    int tw = textwidth(text);
    int th = textheight(text);
    outtextxy(x1 + (x2 - x1 - tw) / 2, y1 + (y2 - y1 - th) / 2, text);
}

// 绘制单个骰子面
void DrawDiceFace(IMAGE* img, int value) {
    // 1. 把这张图片设置为 32位带透明通道（关键！）
    img->Resize(60, 60);  // 确保大小 60x60
    

    // 2. 设置绘图目标为这张图片
    SetWorkingImage(img);

    // 3. 清空整张图片为【完全透明】（不是黑色/白色！）
    DWORD* pBuffer = GetImageBuffer();
    for (int i = 0; i < 60 * 60; i++) {
        pBuffer[i] = 0x00000000; // ARGB = 全透明
    }
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

// 骰子滚动动画
void ShowRolling(int x, int y) {
    for (int i = 0; i < 12; i++) {
        putimage(x - 10, y - 10, 164, 84, &imgBackground, x - 10, y - 10, SRCCOPY);
        putimage(x, y, &imgDice[rand() % 6]);
        putimage(x + 80, y, &imgDice[rand() % 6]);
        Sleep(50 + i * 15);
    }
}

// 掷骰子并显示结果
int RollDice() {
    int d1 = rand() % 6, d2 = rand() % 6;
    int diceX = 328, diceY = 313;
    // 用背景图清除骰子区域和上一次的总和文字，避免黑色框和重叠
    putimage(diceX - 10, diceY - 10, 164, 84, &imgBackground, diceX - 10, diceY - 10, SRCCOPY);
    putimage(300, 390, 260, 60, &imgBackground, 300, 390, SRCCOPY);
    ShowRolling(diceX, diceY);
    // 动画结束后重新绘制最终骰子和背景，确保不消失
    putimage(diceX, diceY, &imgDice[d1]);
    putimage(diceX + 80, diceY, &imgDice[d2]);
    int sum = d1 + d2 + 2;
    TCHAR str[50];
    _stprintf_s(str, _T("总和: %d"), sum);
    setbkmode(TRANSPARENT);
    settextstyle(44, 0, _T("黑体"));
    settextcolor(RGB(255, 215, 0));
    outtextxy(340, 395, str);
    return sum;
}

// 绘制游戏界面 (showRollBtn 控制是否显示掷骰子按钮)
void DrawGameUI(int point, bool showRollBtn) {
    putimage(0, 0, &imgBackground);
    setbkmode(TRANSPARENT);

    // 目标点数提示
    if (point > 0) {
        setfillcolor(RGB(255, 193, 7));
        fillroundrect(250, 118, 550, 165, 10, 10);
        TCHAR str[50];
        _stprintf_s(str, _T("目标点数: %d"), point);
        settextstyle(36, 0, _T("黑体"));
        settextcolor(RGB(80, 40, 0));
        outtextxy(310, 125, str);
    }

    // 掷骰子按钮: 仅在游戏中显示，放在面板底部
    if (showRollBtn) {
        btnRoll = { 340, 470, 460, 520 };
        DrawButton(340, 470, 460, 520, RGB(46, 204, 113), _T("掷骰子"), 28);
    }
}

// 等待鼠标点击指定按钮
bool WaitForButtonClick(Button btn) {
    MOUSEMSG m;
    while (true) {
        m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN) {
            if (m.x >= btn.x1 && m.x <= btn.x2 && m.y >= btn.y1 && m.y <= btn.y2) {
                return true;
            }
        }
    }
}

GameStatus Game() {
    DrawGameUI(0, true);
    int sum = RollDice();
    int point = 0;
    GameStatus status;
    if (sum == 7 || sum == 11) status = WIN;
    else if (sum == 2 || sum == 3 || sum == 12) status = LOSE;
    else {
        status = CONTINUE;
        point = sum;
        // 只更新目标点数提示和按钮，保留骰子和总和显示
        setfillcolor(RGB(255, 193, 7));
        fillroundrect(250, 118, 550, 165, 10, 10);
        TCHAR str[50];
        _stprintf_s(str, _T("目标点数: %d"), point);
        setbkmode(TRANSPARENT);
        settextstyle(36, 0, _T("黑体"));
        settextcolor(RGB(80, 40, 0));
        outtextxy(310, 125, str);
        btnRoll = { 340, 470, 460, 520 };
        DrawButton(340, 470, 460, 520, RGB(46, 204, 113), _T("掷骰子"), 28);
    }

    while (status == CONTINUE) {
        WaitForButtonClick(btnRoll);
        sum = RollDice();
        if (sum == point) status = WIN;
        else if (sum == 7) status = LOSE;
        else {
            // 本轮未分胜负，保留骰子结果，只重绘按钮
            btnRoll = { 340, 470, 460, 520 };
            DrawButton(340, 470, 460, 520, RGB(46, 204, 113), _T("掷骰子"), 28);
        }
    }

    // 显示结果面板: 在当前画面(含骰子和总和)上叠加结果
    DrawPanel(260, 220, 540, 380, RGB(20, 30, 55), (status == WIN) ? RGB(80, 230, 140) : RGB(231, 76, 60));
    setbkmode(TRANSPARENT);
    if (status == WIN) {
        settextstyle(44, 0, _T("黑体"));
        settextcolor(RGB(80, 230, 140));
        outtextxy(340, 240, _T("胜利!"));
        settextstyle(24, 0, _T("宋体"));
        settextcolor(RGB(200, 230, 200));
        outtextxy(300, 300, _T("恭喜，你赢了!"));
    }
    else {
        settextstyle(44, 0, _T("黑体"));
        settextcolor(RGB(231, 76, 60));
        outtextxy(340, 240, _T("失败"));
        settextstyle(24, 0, _T("宋体"));
        settextcolor(RGB(230, 200, 200));
        outtextxy(310, 300, _T("运气不好，再试试!"));
    }
    Button btnBack = { 330, 340, 470, 375 };
    DrawButton(330, 340, 470, 375, RGB(52, 152, 219), _T("返回菜单"), 24);
    WaitForButtonClick(btnBack);

    return status;
}

void Help() {
    DrawGameUI();
    DrawPanel(100, 140, 700, 530, RGB(20, 30, 55), RGB(80, 140, 210));

    setbkmode(TRANSPARENT);
    settextstyle(40, 0, _T("黑体"));
    settextcolor(RGB(255, 215, 0));
    outtextxy(280, 160, _T("游戏规则"));

    // 分割线
    setlinecolor(RGB(80, 140, 210));
    line(130, 210, 670, 210);

    settextstyle(22, 0, _T("宋体"));
    settextcolor(RGB(200, 210, 230));
    outtextxy(150, 230, _T("1. 第一次掷出 7 或 11，玩家获胜"));
    outtextxy(150, 265, _T("2. 第一次掷出 2、3 或 12，玩家失败"));
    outtextxy(150, 300, _T("3. 掷出其他数字，该数字成为目标点数"));
    outtextxy(150, 335, _T("4. 继续掷骰子，直到掷出"));
    outtextxy(170, 360, _T("目标点数 (获胜) 或 7 (失败)"));

    // 返回按钮: 面板内部底部居中
    Button btnHelpBack = { 300, 470, 500, 510 };
    DrawButton(300, 470, 500, 510, RGB(52, 152, 219), _T("返回主菜单"), 28);
    WaitForButtonClick(btnHelpBack);
}

int ShowMainMenu() {
    DrawGameUI();
    setbkmode(TRANSPARENT);

    // 标题
    settextstyle(44, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(300, 170, _T("主菜单"));

    // 按钮: 加大间距，整体居中在面板内 (面板 y: 115~575，可用中心区域 y: 220~520)
    int btnX = 260, btnW = 280, btnH = 55, gap = 25;
    int startY = 260;
    btnHelp  = { btnX, startY,              btnX + btnW, startY + btnH };
    btnStart = { btnX, startY + btnH + gap,  btnX + btnW, startY + 2 * btnH + gap };
    btnExit  = { btnX, startY + 2 * (btnH + gap), btnX + btnW, startY + 3 * btnH + 2 * gap };

    DrawButton(btnHelp.x1,  btnHelp.y1,  btnHelp.x2,  btnHelp.y2,  RGB(52, 152, 219),  _T("游戏帮助"), 28);
    DrawButton(btnStart.x1, btnStart.y1, btnStart.x2, btnStart.y2, RGB(46, 204, 113),  _T("开始游戏"), 28);
    DrawButton(btnExit.x1,  btnExit.y1,  btnExit.x2,  btnExit.y2,  RGB(231, 76, 60),   _T("退出游戏"), 28);

    // 底部提示
    settextstyle(20, 0, _T("宋体"));
    settextcolor(RGB(120, 150, 190));
    outtextxy(310, startY + 3 * btnH + 2 * gap + 40, _T("请用鼠标点击按钮"));

    // 等待鼠标点击并返回对应选项
    while (true) {
        MOUSEMSG m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN) {
            if (m.x >= btnHelp.x1 && m.x <= btnHelp.x2 && m.y >= btnHelp.y1 && m.y <= btnHelp.y2)
                return 1;
            else if (m.x >= btnStart.x1 && m.x <= btnStart.x2 && m.y >= btnStart.y1 && m.y <= btnStart.y2)
                return 2;
            else if (m.x >= btnExit.x1 && m.x <= btnExit.x2 && m.y >= btnExit.y1 && m.y <= btnExit.y2)
                return 3;
        }
    }
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
            total++;
            if (res == WIN) wins++;

            // 战绩统计面板
            DrawGameUI();
            DrawPanel(200, 180, 600, 440, RGB(20, 30, 55), RGB(100, 160, 230));
            setbkmode(TRANSPARENT);

            settextstyle(36, 0, _T("黑体"));
            settextcolor(RGB(255, 215, 0));
            outtextxy(310, 195, _T("对局统计"));

            setlinecolor(RGB(80, 140, 210));
            line(220, 240, 580, 240);

            TCHAR str[100];
            _stprintf_s(str, _T("胜利: %d 次"), wins);
            settextstyle(28, 0, _T("黑体"));
            settextcolor(RGB(80, 230, 140));
            outtextxy(260, 265, str);

            _stprintf_s(str, _T("总计: %d 次"), total);
            settextcolor(RGB(200, 210, 230));
            outtextxy(260, 310, str);

            if (total > 0) {
                _stprintf_s(str, _T("胜率: %d%%"), wins * 100 / total);
                settextcolor(RGB(255, 215, 0));
                outtextxy(260, 355, str);
            }

            Button btnContinue = { 340, 400, 460, 435 };
            DrawButton(340, 400, 460, 435, RGB(52, 152, 219), _T("继续"), 28);
            WaitForButtonClick(btnContinue);
            break;
        }
        case 3:
            DrawGameUI();
            DrawPanel(200, 200, 600, 420, RGB(20, 30, 55), RGB(100, 160, 230));
            setbkmode(TRANSPARENT);

            settextstyle(36, 0, _T("黑体"));
            settextcolor(RGB(255, 215, 0));
            outtextxy(300, 215, _T("游戏结束"));

            setlinecolor(RGB(80, 140, 210));
            line(220, 260, 580, 260);

            {
                TCHAR finalStr[100];
                _stprintf_s(finalStr, _T("最终战绩: %d 胜 / %d 局"), wins, total);
                settextstyle(28, 0, _T("黑体"));
                settextcolor(RGB(200, 210, 230));
                outtextxy(240, 290, finalStr);
            }

            settextcolor(RGB(150, 170, 200));
            settextstyle(24, 0, _T("宋体"));
            outtextxy(320, 340, _T("谢谢游玩!"));

            Button btnExitConfirm = { 340, 380, 460, 415 };
            DrawButton(340, 380, 460, 415, RGB(231, 76, 60), _T("退出"), 28);
            WaitForButtonClick(btnExitConfirm);
            break;
        }
    } while (choice != 3);
    closegraph();
}
