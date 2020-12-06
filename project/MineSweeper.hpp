#ifndef __MINE_SWEEPER_HPP__
#define __MINE_SWEEPER_HPP__

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <termio.h>
#include <unistd.h>
#include <vector>
using namespace std;

#define MAP_SIZE 9
#define MINE 9
#define PUSH_ENTER 10
#define DIG 1
#define FLAG 2
#define QUESTION_MARK 3

string NUM[10]={"０","１","２","３","４","５","６","７","８","９"};

int getch(void);

class Point {
  public:
    int x;
    int y;
};


class MineSweeper {
  public:
  private:
    vector<Point> mines;
};


////////// GAME_PROGRAMMING

class Mine_Sweeper {
  public:
    Mine_Sweeper(int map_size = MAP_SIZE);
    void printMap();
    bool is_out_of_map(int x, int y);
    void spread_mine(int num); // spread mine at random position
    Point get_rand_pos();
    int count_mine(int x, int y);
    void make_mine_table(int num);   // fill map with num of mines
    bool play_game(int num_of_mine, pid_t child_pid); // input number of mine and start game
    void change_map(int x, int y, int value) { map[y][x] = value; }
    void change_front(int x, int y, string value) { front[y][x] = value; }
    void multi_boom(int x, int y); // check near spot also 0
    bool is_game_over(int num_of_mine);
    void show_mine();

    int acting(int &x, int &y);
    void show_cursor(int x, int y);
    void fill_number_line(); // fill front's border with numbers

    void set_size(int sz);

    void Print_HintCount (int count,int num);

  private:
    vector<vector<int>> map;
    vector<vector<string>> front;
    int map_size;
};

#endif
