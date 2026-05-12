#pragma once
#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

// ÓÎÏ·×´Ì¬Ã¶¾Ù
enum GameStatus { WIN, LOSE, CONTINUE };

void DrawDiceFace(IMAGE* img, int value);
void DrawBackground();
void DrawWinImage();
void DrawLoseImage();
void InitGraphics();
void ShowRollingDice(int x, int y);
void DrawGameUI(int point = 0);
int RollDice();
GameStatus Game();
void Help();
int ShowMainMenu();
void Run(); 
