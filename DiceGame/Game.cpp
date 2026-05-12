#include "Game.h"

// 游戏图片资源
IMAGE imgDice[6];       // 6个骰子面图片
IMAGE imgBackground;    // 背景图片
IMAGE imgWin;           // 胜利图片
IMAGE imgLose;          // 失败图片

int user;

// 绘制一个骰子面
void DrawDiceFace(IMAGE* img, int value)
{
    SetWorkingImage(img);           // 创建绘图设备

    setbkcolor(WHITE);              // 绘制骰子背景
    cleardevice();                  // 清空绘图设备
    setfillcolor(WHITE);            // 设置填充颜色
    fillrectangle(0, 0, 60, 60);    // 填充矩形 左上角 (left,top)，右下角 (right,bottom)

    setfillcolor(BLACK);            // 绘制骰子点数

    // 根据点数绘制不同的点
    switch (value) {
    case 1:
        fillcircle(30, 30, 6);
        break;
    case 2:
        fillcircle(15, 15, 6);
        fillcircle(45, 45, 6);
        break;
    case 3:
        fillcircle(15, 15, 6);
        fillcircle(30, 30, 6);
        fillcircle(45, 45, 6);
        break;
    case 4:
        fillcircle(15, 15, 6);
        fillcircle(45, 15, 6);
        fillcircle(15, 45, 6);
        fillcircle(45, 45, 6);
        break;
    case 5:
        fillcircle(15, 15, 6);
        fillcircle(45, 15, 6);
        fillcircle(15, 45, 6);
        fillcircle(45, 45, 6);
        fillcircle(30, 30, 6);
        break;
    case 6:
        fillcircle(15, 12, 6);
        fillcircle(15, 30, 6);
        fillcircle(15, 48, 6);
        fillcircle(45, 12, 6);
        fillcircle(45, 30, 6);
        fillcircle(45, 48, 6);
        break;
    }
    // 恢复默认绘图设备
    SetWorkingImage(NULL);
}

// 绘制背景图片
void DrawBackground() {
    SetWorkingImage(&imgBackground);//取地址符
    // 背景
    setbkcolor(RGB(200, 216, 255));
    cleardevice();
    // 绘制标题背景
    setfillcolor(RGB(76, 175, 80));
    fillrectangle(0, 0, 800, 100);
    // 绘制游戏区域背景
    setfillcolor(BLACK);
    fillrectangle(100, 100, 700, 500);
    // 绘制标题
    settextcolor(DARKGRAY);
    settextstyle(60, 0, _T("黑体"));
    outtextxy(280, 10, _T("骰子游戏"));
    // 恢复默认绘图设备
    SetWorkingImage(NULL);
}

// 绘制胜利图片   
void DrawWinImage() {
    SetWorkingImage(&imgWin);
    // 背景
    setbkcolor(GREEN);
    cleardevice();
    setfillcolor(GREEN);
    fillrectangle(0, 0, 200, 100);
    // 文字
    settextcolor(BLACK);
    settextstyle(40, 0, _T("黑体"));
    outtextxy(50, 30, _T("胜利"));
    // 恢复默认绘图设备
    SetWorkingImage(NULL);
}

// 绘制失败图片
void DrawLoseImage() {
    SetWorkingImage(&imgLose);
    // 背景
    setbkcolor(RED);
    cleardevice();
    setfillcolor(RED);
    fillrectangle(0, 0, 200, 100);
    // 文字
    settextcolor(BLACK);
    settextstyle(50, 0, _T("黑体"));
    outtextxy(50, 30, _T("失败"));
    // 恢复默认绘图设备
    SetWorkingImage(NULL);
}

// 初始化图形资源
void InitGraphics() {
    // 创建图片资源
    for (int i = 0; i < 6; i++) {
        imgDice[i].Resize(60, 60);
        DrawDiceFace(&imgDice[i], i + 1);
    }
    imgBackground.Resize(800, 600);
    DrawBackground();
    imgWin.Resize(200, 100);
    DrawWinImage();
    imgLose.Resize(200, 100);
    DrawLoseImage();
}

// 显示骰子滚动动画
void ShowRollingDice(int x, int y) {
    for (int i = 0; i < 10; i++) {
        // 随机显示骰子面来模拟滚动
        int face1 = rand() % 6;
        int face2 = rand() % 6;
        // 绘制骰子
        putimage(x, y, &imgDice[face1]);
        putimage(x + 70, y, &imgDice[face2]);
        // 延迟一段时间
        Sleep(100);
    }
}

// 绘制游戏界面
void DrawGameUI(int point) {
    // 绘制背景
    putimage(0, 0, &imgBackground);
    // 如果有目标点数，显示出来
    if (point > 0) {
        TCHAR pointStr[50];//声明一个​缓冲区数组，用于存储格式化后的字符串
        _stprintf_s(pointStr, _T("目标点数: %d"), point);
        settextstyle(50, 0, _T("黑体"));
        outtextxy(260, 120, pointStr);
    }
}

// 投掷骰子
int RollDice() {
    int die1 = rand() % 6;    // 0-5 的随机数
    int die2 = rand() % 6;
    // 显示骰子滚动动画
    ShowRollingDice(330, 200);
    // 显示最终骰子结果
    putimage(330, 200, &imgDice[die1]);
    putimage(400, 200, &imgDice[die2]);
    // 显示总和
    TCHAR sumStr[50];
    _stprintf_s(sumStr, _T("总和: %d"), die1 + die2 + 2);
    settextstyle(50, 0, _T("黑体"));
    outtextxy(310, 280, sumStr);
    return die1 + die2 + 2;
}

GameStatus Game() {
    srand(static_cast<unsigned int>(time(0)));  // 随机数种子
    DrawGameUI();      // 绘制初始界面

    // 第一次投掷
    int sum = RollDice();
    GameStatus status;
    int myPoint = 0;
    user = _getch();
    // 检查第一次投掷结果
    switch (sum) {
    case 7: case 11:
        status = WIN;
        break;
    case 2: case 3: case 12:
        status = LOSE;
        break;
    default:
        status = CONTINUE;
        myPoint = sum;
        DrawGameUI(myPoint); // 更新界面显示目标点数
    }

    // 继续投掷
    while (status == CONTINUE) {
        // 提示用户按任意键继续
        settextstyle(30, 0, _T("宋体"));
        outtextxy(270, 350, _T("按任意键继续投掷..."));
        user = _getch();
        clearrectangle(270, 280, 600, 400);
        sum = RollDice();
        if (sum == myPoint) {
            status = WIN;
        }
        else if (sum == 7) {
            status = LOSE;
        }
    }


    // 显示结果
    if (status == WIN) {
        putimage(300, 390, &imgWin);
    }
    else {
        putimage(300, 390, &imgLose);
    }

    // 等待用户查看结果
    user = _getch();

    return status;
}

// 显示帮助信息
void Help() {
    DrawGameUI();

    settextstyle(20, 10, _T("黑体"));
    outtextxy(150, 150, _T("游戏规则:"));
    outtextxy(150, 180, _T("1. 第一次掷出7或11，玩家获胜"));
    outtextxy(150, 210, _T("2. 第一次掷出2, 3或12，玩家失败"));
    outtextxy(150, 240, _T("3. 第一次掷出其他数字，该数字成为目标点数"));
    outtextxy(150, 270, _T("4. 继续掷骰子，直到掷出目标点数(获胜)或7(失败)"));

    outtextxy(280, 350, _T("按任意键返回主菜单..."));
    user = _getch();
}

// 显示主菜单
int ShowMainMenu() {
    DrawGameUI();

    settextstyle(40, 0, _T("黑体"));
    outtextxy(320, 150, _T("主菜单"));

    settextstyle(30, 0, _T("宋体"));
    outtextxy(300, 250, _T("1. 游戏帮助"));
    outtextxy(300, 280, _T("2. 开始游戏"));
    outtextxy(300, 310, _T("3. 退出游戏"));

    outtextxy(300, 350, _T("请选择(1-3): "));

    // 获取用户输入
    int choice = _getch() - '0';
    return choice;
}

void Run() {
    // 初始化图形窗口
    initgraph(800, 600, SHOWCONSOLE);

    // 初始化图形资源
    InitGraphics();

    int totalCount = 0;
    int winCount = 0;
    int choice;

    do {
        choice = ShowMainMenu();

        switch (choice) {
        case 1:
            Help();
            break;
        case 2: {
            GameStatus result = Game();
            totalCount++;
            if (result == WIN) {
                winCount++;
            }

            // 显示统计信息
            DrawGameUI();
            TCHAR stats[100];
            _stprintf_s(stats, _T("当前战绩: 胜利 %d 次，共 %d 次"), winCount, totalCount);
            settextstyle(30, 0, _T("黑体"));
            outtextxy(200, 250, stats);
            outtextxy(300, 300, _T("按任意键继续..."));
            user = _getch();
            break;
        }
        case 3:
            // 显示退出信息
            DrawGameUI();
            TCHAR finalStats[100];
            _stprintf_s(finalStats, _T("最终战绩: 胜利 %d 次，共 %d 次"), winCount, totalCount);
            settextstyle(30, 0, _T("宋体"));
            outtextxy(200, 250, finalStats);
            outtextxy(250, 350, _T("谢谢游玩! 按任意键退出..."));
            user = _getch();
            break;
        default:
            // 无效选择
            DrawGameUI();
            outtextxy(300, 250, _T("无效选择!"));
            outtextxy(300, 300, _T("按任意键继续..."));
            user = _getch();
        }
    } while (choice != 3);

    // 关闭图形窗口
    closegraph();
}