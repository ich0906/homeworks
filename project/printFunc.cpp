#include "printFunc.hpp"
#include "Timer.hpp"
#include "MineSweeper.hpp"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void runSystem()
{   
    system("clear");
    cursor=0;
    char key;
    setColor();
    printUI_Main();
    show_UI_cursor(15,13+4*(cursor));

    while(1)
    {
        key=getch();
        
        switch(key)
        {
        case 'w':
            delete_UI_cursor(15,13+4*(cursor));
            if(cursor>0)cursor--;
            show_UI_cursor(15,13+4*(cursor));
            break;
        case 's':
            delete_UI_cursor(15,13+4*(cursor));
            if(cursor>=MAIN_MENU_NUM)cursor=MAIN_MENU_NUM;
            else cursor++;
            show_UI_cursor(15,13+4*(cursor));
            break;
        case '\n':
            delete_UI_cursor(15,13+4*(cursor));
            main_switch();
            break;
        }
        deleteColor();
    }
    
}

void runStart()
{   
    system("clear");
    cursor=0;
    char key;
    bool end=false;
    setColor();
    printUI_Start();
    show_UI_cursor(15,13+4*(cursor));

    while(1)
    {
        key=getch();
        setColor();
        switch(key)
        {
        case 'w':
            delete_UI_cursor(15,13+4*(cursor));
            if(cursor>0)cursor--;
            show_UI_cursor(15,13+4*(cursor));
            break;
        case 's':
            delete_UI_cursor(15,13+4*(cursor));
            if(cursor>=MAIN_MENU_NUM)cursor=MAIN_MENU_NUM;
            else cursor++;
            show_UI_cursor(15,13+4*(cursor));
            break;
        case '\n':
            delete_UI_cursor(15,13+4*(cursor));
            end=start_switch();
            break;
        }
        deleteColor();
        if(end)break;
    }
    printUI_Main();
    show_UI_cursor(15,13+4*(cursor));
    
}

void show_UI_cursor(int x,int y)
{
    string cmd="tput cup ";
    cmd+=(to_string(y)+" ");
    cmd+=(to_string(x)+";");
    cmd+="echo \"\033[1;40m\033[1;31m\"▸ \"\033[0m\"";
    system(cmd.c_str());
    deleteColor();
}

void delete_UI_cursor(int x,int y)
{
    string cmd="tput cup ";
    cmd+=(to_string(y)+" ");
    cmd+=(to_string(x)+";");
    cmd+="echo \"\033[1;40m\033[1;37m\"    \"\033[0m\"";
    system(cmd.c_str());
    deleteColor();
}

void main_switch()
{
    switch(cursor)
    {
    case GAME_START:
        runStart();
        break;
    case RANKING:
        printRanking();
        break;
    case HELP:
        printHelp();
        break;
    case EXIT:
        exit(0);
        break;
    }
    deleteColor();
}

bool start_switch()
{
    Mine_Sweeper mine(0);
    char re='y';

    switch(cursor)
    {
    case EASY_MODE:
        while(re=='y')
        {
            mine.set_size(EASY_MAP);
            if(!Timer(mine,EASY_MINE))//mine.play_game(EASY_MINE); 
                cout<<"Wanna replay? (y/n):";
            re=getch();
        }
        ///////////
        break;

    case NORMAL_MODE:
        while(re=='y')
        {
            mine.set_size(NORMAL_MAP);
            if(!Timer(mine,NORMAL_MINE)) //mine.play_game(NORMAL_MINE);
                cout<<"Wanna replay? (y/n):";
            re=getch();
        }
        break;

    case HARD_MODE:
        while(re=='y')
        {
            mine.set_size(HARD_MAP);
            if(!Timer(mine,HARD_MINE)) //mine.play_game(HARD_MINE);
                cout<<"Wanna replay? (y/n):";
            re=getch();
        }
        break;

    case START_BACK:
        break;
    }
    return true;
}

void help_UI_printing()
{
    //HELP MENU
}

void ranking_UI_printing()
{
    //RANK MENU
}

void printBox() {
    setColor();
    for (int i = 0; i < UI_HEIGHT; ++i) {
        for (int j = 0; j < UI_WIDTH; ++j) {
            if (i == 0 && j == 0)
                cout << "┏";
            else if (i == 0 && j == UI_WIDTH - 1)
                cout << "┓";
            else if (i == UI_HEIGHT - 1 && j == 0)
                cout << "┗";
            else if (i == UI_HEIGHT - 1 && j == UI_WIDTH - 1)
                cout << "┛";
            else if (i == 0 || i == UI_HEIGHT - 1)
                cout << "━";
            else if (j == 0 || j == UI_WIDTH - 1)
                cout << "┃";
            else
                cout << " ";
        }
        cout << endl;
    }
    deleteColor();
}

void printContent(string str) {
    setColor();
    for (int i = 0; i < BUTTON_WIDTH_GAP; ++i) {
        if (i == 0)
            cout << "┃";
        else
            cout << " ";
    }
    cout << "┏━━━━━━━━━━━━━━━┓\n";
    for (int i = 0; i < BUTTON_WIDTH_GAP; ++i) {
        if (i == 0)
            cout << "┃";
        else
            cout << " ";
    }
    cout << "┃   " << str << "  ┃\n";
    for (int i = 0; i < BUTTON_WIDTH_GAP; ++i) {
        if (i == 0)
            cout << "┃";
        else
            cout << " ";
    }
    cout << "┗━━━━━━━━━━━━━━━┛\n";
    deleteColor();
}

void printUI_Main() {
    system("clear");
    setColor();
    printBox();
    upCursor(23);
    printTitle();
    downCursor(4);
    printContent("Game Start");
    downCursor(1);
    printContent(" Ranking  ");
    downCursor(1);
    printContent("   Help   ");
    downCursor(1);
    printContent("   Exit   ");
    downCursor(5);
    deleteColor();
}

void printUI_Start() {
    system("clear");
    setColor();
    printBox();
    upCursor(23);
    printTitle();
    downCursor(4);
    printContent("   Easy   ");
    downCursor(1);
    printContent("  Normal  ");
    downCursor(1);
    printContent("   Hard   ");
    downCursor(1);
    printContent("   Back   ");
    downCursor(5);
    deleteColor();
}

void downCursor(int num) { printf("\x1b[%dB", num); }

void upCursor(int num) { printf("\x1b[%dA", num); }

void printTitle() {
    setColor();
    for (int i = 0; i < 21; ++i) {
        if (i == 0)
            cout << "┃";
        else
            cout << " ";
    }
    cout << "●  MINE SWEEPER ●" << endl;
    deleteColor();
}

void printRanking() {
    RankerList rankerList;
    rankerList.rankerSort();
    rankerList.updateRankFile();

    system("clear");
    setColor();
    printBox();
    system("tput cup 2 23"); setColor();
    cout << "●  Ranking ●" << endl;

    rankerList.printRankerList();

    system("tput cup 26 35");
    cout<<"< Press b to go back >"<<endl;
    deleteColor();

    char re = 'a';
    while(re != 'b'){
        re=getch();
    }
    system("clear");
    printUI_Main();
}

void printHelp() {
    system("clear");
    setColor();
    printBox();
    system("tput cup 2 23"); setColor();
    cout << "●   Help   ●" << endl;

    system("tput cup 4 1");
    cout << "● 게임 방법 ● " << endl;
    system("tput cup 6 1");
    cout << "1. 맵 안에는 빈 공간, 숫자, 지뢰가 있는데, "<< endl;
    system("tput cup 7 4");
    cout << "Enter키를 눌러 땅을 파면 확인할 수 있다." << endl;
    system("tput cup 8 1");
    cout << "2. 숫자는 그 칸 주위 8칸에 존재하는 지뢰 개수를 의미한다." << endl;
    system("tput cup 9 1");
    cout << "3. 지뢰가 있다고 생각되는 곳에 깃발을 세워 표시한다." << endl;
    system("tput cup 10 1");
    cout << "4. 지뢰의 위치를 알고 싶다면 Hint를 사용하여 확인한다." << endl;
    system("tput cup 11 1");
    cout << "5. 지뢰가 있는 땅을 파거나 지뢰를 다 찾으면 게임이 끝난다." << endl;

    system("tput cup 14 1");
    cout << "● 난이도 ● " << endl;
    system("tput cup 16 1");
    cout << "EASY MODE : 지뢰 10개 | 힌트 2개" << endl;
    system("tput cup 17 1");
    cout << "NORMAL MODE: 지뢰 40개 | 힌트 5개" << endl;
    system ("tput cup 18 1");
    cout << "HARD MODE: 지뢰 99개 | 힌트 10개" << endl;

    system("tput cup 21 1");
    cout << "● 조작키 ● " << endl;
    system("tput cup 23 1");
    cout << "W: 위 쪽  A: 왼 쪽  S: 아래 쪽  D: 오른 쪽" << endl;
    system("tput cup 24 1");
    cout << "Enter: 땅 파기  i: 깃발 세우기  o: 힌트" << endl; 
    system("tput cup 26 35");
    cout<<"< Press b to go back >"<<endl;
    deleteColor();
    char re = 'a';
    while(re != 'b')
    {
        re=getch();
    }
    system("clear");
    printUI_Main();
}

