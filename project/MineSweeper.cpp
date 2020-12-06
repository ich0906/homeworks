#include "MineSweeper.hpp"
#include "rank.hpp"
#include "printFunc.hpp"
#include "Timer.hpp"
using namespace std;

Mine_Sweeper::Mine_Sweeper(int map_size) {
    vector<int> v1;
    for (int i = 0; i < map_size; ++i)
        v1.push_back(0);

    for (int i = 0; i < map_size; ++i)
        map.push_back(v1);

    vector<string> v2;
    vector<string> space;
    for (int i = 0; i < map_size; ++i) {
        v2.push_back("■ ");
        space.push_back("  ");
    }

    v2.push_back("  ");
    v2.push_back("  ");
    space.push_back("  ");
    space.push_back("  ");

    for (int i = 0; i < map_size; ++i)
        front.push_back(v2);

    front.push_back(space);
    front.push_back(space);

    fill_number_line();
    this->map_size = map_size;
}

void Mine_Sweeper::set_size(int sz)
{
    map.clear();
    front.clear();
    map_size=sz;

    vector<int> v1;
    for (int i = 0; i < map_size; ++i)
        v1.push_back(0);

    for (int i = 0; i < map_size; ++i)
        map.push_back(v1);

    vector<string> v2;
    vector<string> space;
    for (int i = 0; i < map_size; ++i) {
        v2.push_back("■ ");
        space.push_back("  ");
    }

    v2.push_back("  ");
    v2.push_back("  ");
    space.push_back("  ");
    space.push_back("  ");

    for (int i = 0; i < map_size; ++i)
        front.push_back(v2);

    front.push_back(space);
    front.push_back(space);

    fill_number_line();
}

void Mine_Sweeper::printMap() {
    for (int i = 0; i < front.size(); ++i) {
        for (int j = 0; j < front[i].size(); ++j){
            if(front[i][j]=="⚑ " || front[i][j]=="？")
            {
                printf("\x1b[1;40m\x1b[1;34m");
                cout << front[i][j];
                printf("\x1b[0m");
            }
            else if(front[i][j]=="✖ ")
            {
                printf("\x1b[1;40m\x1b[1;31m");
                cout << front[i][j];
                printf("\x1b[0m");
            }
            else
            {
                printf("\x1b[1;40m\x1b[1;37m");
                cout << front[i][j];
                printf("\x1b[0m");
                if(j == front[j].size() -1 && i == 5){    // if it's at the first row, last column
                    show_time(mine_time);
                }
            }
            
        }
        cout << endl;
    }
    /*
    cout<<endl;
    for (int i = 0; i < map_size; ++i)
        {
                for (int j = 0; j < map_size; ++j)
                {
                        cout << map[i][j] << " ";
                }
                cout << endl;
        }
    */
}

void Mine_Sweeper::spread_mine(int num) {
    vector<Point> pos;
    Point temp;

    for (int i = 0; i < num; ++i) {
        temp = get_rand_pos();

        for (int j = 0; j < pos.size(); ++j) {
            if (pos[j].x == temp.x && pos[j].y == temp.y) {
                temp = get_rand_pos();
                j = 0;
                continue;
            }
        }

        pos.push_back(temp);
    }

    for (int i = 0; i < pos.size(); ++i)
        map[pos[i].y][pos[i].x] = MINE;
}

Point Mine_Sweeper::get_rand_pos() {
    Point pos;
    pos.x = rand() % map_size;
    pos.y = rand() % map_size;
    return pos;
}

int Mine_Sweeper::count_mine(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if ((i == 0 && j == 0) || is_out_of_map(y + i, x + j))
                continue;
            //맵 넘어가거나 자기 자리인경우 건너뜀

            if (map[y + i][x + j] == MINE)
                count++;
        }
    }

    return count;
}

bool Mine_Sweeper::is_out_of_map(int x, int y) {
    if (x < 0 || x >= map_size || y < 0 || y >= map_size)
        return true;

    return false;
}

void Mine_Sweeper::make_mine_table(int num) {
    spread_mine(num);

    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j] != MINE)
                map[i][j] = count_mine(j, i);
        }
    }
}

void Mine_Sweeper::multi_boom(int x, int y) {
    if (is_out_of_map(x, y))
        return; //범위 밖이면 함수종료

    if (map[y][x] == 0) {
        if (map[y][x] != -1) //중간이 0이고 본곳이 아닌 경우
        {
            if (map[y][x] == 0)
                front[y][x] = "□ ";
            else
                front[y][x] = NUM[map[y][x]]; //중간 파고 본 위치 표시

            map[y][x] = -1;
        }

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                // 왼쪽 위부터 9개 맵 안쪽이며 안본 위치면
                if (!is_out_of_map(y + i, x + j) && map[y + i][x + j] != -1)
                    multi_boom(x + j, y + i);
            }
        }
    } else {
        if (map[y][x] != -1) // 0이 아니고 본 곳이 아닌 숫자면 파고 표시
        {
            front[y][x] = NUM[map[y][x]];
            map[y][x] = -1;
        }
    }
}

bool Mine_Sweeper::is_game_over(int num_of_mine) {
    int count = 0;
    for (int i = 0; i < map_size; ++i) {
        for (int j = 0; j < map_size; ++j) {
            if (map[i][j] == -1)
                count++;
        }
    }

    if (count == (map_size * map_size - num_of_mine))
        return true;
    else
        return false;
}

bool Mine_Sweeper::play_game(int num_of_mine, pid_t child_pid) {  // to send signal, we need child pid
    
    int HintCount;
    if (num_of_mine == EASY_MINE)
         HintCount = 2;
    else if (num_of_mine == NORMAL_MINE)
         HintCount = 5;
    else if (num_of_mine == HARD_MINE)
        HintCount = 10;
    
    system("clear");
    make_mine_table(num_of_mine);
    printMap();
    Print_HintCount(HintCount,num_of_mine);

    int x = 0, y = 0;
    int status = -1;
    bool is_over = false;
    
    while (true) {
        if (is_out_of_map(y, x)) {
            cout << "out of range!" << endl;
            continue;
        }
        show_cursor(x, y);

        status = acting(x, y);

        if (status == DIG) {
            switch (map[y][x]) {
            case 0: // add multi boom
                multi_boom(x, y);
                break;

            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                front[y][x] = NUM[map[y][x]]; // change field shape
                map[y][x] = -1;           // turn into -1 after choosed
                break;

            case MINE:
                front[y][x] = "✖ ";
                system("clear");
                show_mine();
                printMap();
                cout << "GAME OVER" << endl;
                is_over = true;
                break;
            }
        } else if (status == FLAG) {
            if (front[y][x] == "■ ")
                front[y][x] = "⚑ ";
            else if (front[y][x] == "⚑ ")
                front[y][x] = "？";
            else if(front[y][x] == "？")
                front[y][x] = "■ ";
        }
        else if (status == QUESTION_MARK) { 
            if (HintCount > 0) {
                //Mark Hint in the map
                if(front[y][x] ==  "✖ ")
                {

                }
                else if ( map[y][x] == MINE ){
                    front[y][x] =  "✖ ";
                    HintCount--;
                }
                else {
                    if(front[y][x] == "■ " || front[y][x] == "⚑ " || front[y][x] == "？"){
                        if(map[y][x]==0){
                            multi_boom(x,y);
                            HintCount--;
                        }
                        else{
                            front[y][x]=NUM[map[y][x]];
                            map[y][x]=-1;
                            HintCount--;
                        }
                    }  
                }
            }
        }

        if(is_over){
            send_end(child_pid);// game is in child process, so get the parent's pid
            return false;
        }
        show_cursor(x, y);
        if (is_game_over(num_of_mine)) {
            system("clear");
            show_mine();
            printMap();
            cout << "GAME CLEAR" << endl;
            send_end(child_pid);
            return true;
        }

        system("clear");
        printMap();
        Print_HintCount(HintCount,num_of_mine);
    }
     
}

void Mine_Sweeper::Print_HintCount (int count, int num) {
    string temp = "tput cup ";
    if (num == EASY_MINE){
        temp+= "11 0";
    }
    else if (num == NORMAL_MINE) {
        temp+="18 0";
    }
    else if (num == HARD_MINE) {
        temp+="24 0";
    }
    system(temp.c_str());
    cout << "Hint: " << count << endl;
}

void Mine_Sweeper::show_mine()
{
    for(int i=0;i<map_size;++i)
        for(int j=0;j<map_size;++j)
            if(map[i][j]==MINE)front[i][j]="✖ ";
}

void Mine_Sweeper::fill_number_line() {
    int count = 0;
    for (int i = 0; i < front[0].size() - 2; ++i) {
        front[front.size() - 1][i] = NUM[count];
        front[i][front.size() - 1] = NUM[count++];

        if (count > 9)
            count = 0;
    }
}

void Mine_Sweeper::show_cursor(int x, int y) {
    string cmd = "tput cup ";
    cmd += to_string(y);
    cmd += " ";
    cmd += to_string(2 * x);
    system(cmd.c_str());
}

int Mine_Sweeper::acting(int &x, int &y) {
    char cmd;
    cmd = getch();

    switch (cmd) {
    case 'w':
        if (y > 0)
            y--;
        break;
    case 's':
        if (y < map_size - 1)
            y++;
        break;
    case 'a':
        if (x > 0)
            x--;
        break;
    case 'd':
        if (x < map_size - 1)
            x++;
        break;

    case '\n':
        return DIG;

    case 'i':
        return FLAG;

     case 'o':
        return QUESTION_MARK;
    }
    return 0;
}

int getch(void) {
    int ch;
    struct termios buf, save;
    tcgetattr(0, &save);
    buf = save;
    buf.c_lflag &= ~(ICANON | ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}
