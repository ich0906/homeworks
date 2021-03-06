#ifndef __CALO_UI_H__
#define __CALO_UI_H__

#include "food_information.h"
#include "drawing_graph.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <conio.h>

using namespace std;

#define EDGE_WIDTH 100
#define EDGE_HEIGHT 31

enum class KEY { UP, DOWN, LEFT, RIGHT, ENTER, ESC, NONE };

const string TOP_LEFT = "��";
const string TOP_RIGHT = "��";
const string BOTTOM_LEFT = "��";
const string BOTTOM_RIGHT = "��";
const string VERTICAL = "��";
const string HORIZONTAL = "��";

const pair<int, int> menu_1 = make_pair(40, 17);
const pair<int, int> menu_2 = make_pair(40, 20);
const pair<int, int> menu_3 = make_pair(40, 23);

//Move Cursor to (x, y)
void init_calo_ui();
void gotoxy(int x, int y);
void gotoxy(pair<int, int> coord);
void goto_origin();
void print_edge(int height = EDGE_HEIGHT, int width = EDGE_WIDTH);
void print_main_menu();
void print_textbox(pair<int, int> lu, string msg); //ru: right up, ld: left down
int cursor(vector<pair<int, int>> sels);
void draw_graph(string name, bool gender);
Food input_food_info(wstring food_name);
void print_user_choice(string person_name, bool gender);
void print_user_sel();
void print_user_create();
void print_user_del();
KEY get_key();

#endif