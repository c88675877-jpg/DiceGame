#pragma once
#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>

enum GameStatus { WIN, LOSE, CONTINUE };

void InitGraphics();
void DrawGameUI(int point = 0);  // 声明带默认参数
void Run();