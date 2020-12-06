#ifndef __PRINT_FUNC_HPP__
#define __PRINT_FUNC_HPP__

#include "rank.hpp"

#define UI_WIDTH 60
#define UI_HEIGHT 30
#define BUTTON_WIDTH_GAP 21
#define EASY_MAP 9
#define NORMAL_MAP 16
#define HARD_MAP 22
#define EASY_MINE 10
#define NORMAL_MINE 40
#define HARD_MINE 99
#define MAIN_MENU_NUM 3

enum { GAME_START=0, RANKING, HELP, EXIT,
       EASY_MODE=0,NORMAL_MODE,HARD_MODE,START_BACK};

void runSystem();   
void runStart();
void show_UI_cursor(int x,int y);
void delete_UI_cursor(int x,int y);

void printBox();
void printContent(string str);
void printUI_Main();
void printUI_Start();
void downCursor(int num);
void upCursor(int num);
void printTitle();
void printRanking();
void printHelp();


void main_switch();
bool start_switch();
void help_UI_printing();
void ranking_UI_printing();

int cursor=0;
#endif
