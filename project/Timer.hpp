#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include "MineSweeper.hpp"
#include "printFunc.hpp"

#ifdef __cplusplus
extern "C"{
#endif

    #include<stdio.h>
    #include<stdlib.h>
    #include<signal.h>
    #include<unistd.h>
    #include<string.h>
    #include<sys/types.h>
    #include<sys/wait.h>   

    int mine_time = 0; // global variable

    void get_sig(int s) {
        if(s == SIGUSR1)
            exit(1);
    }

    void send_end(pid_t p) {
        if ( kill(p, SIGUSR1) != 0 ) {
            printf("kill() error!\n");
        }
    }

    void inc_time(int t){
        if(t == SIGUSR2){
            ++mine_time;
        }
    }

    void setColor(){printf("\x1b[1;40m\x1b[1;37m");}
    void deleteColor(){printf("\x1b[0m");}

    bool Timer(Mine_Sweeper mine, int mode) {
        RankerList rankerList;
        rankerList.rankerSort();
        rankerList.updateRankFile();
        string name=" ";
        char ch=' ';

        pid_t pid, main_pid;
        main_pid = getpid();
        mine_time = 0;

        pid = fork();
        int status = 0;
        bool is_clear;
        if(pid == 0) { 
            signal(SIGUSR1, get_sig);
            while(true) { 
                sleep(1);
                kill(main_pid, SIGUSR2);
            }
        }
        else if(pid > 0) {
            signal(SIGUSR2, inc_time);
            is_clear=mine.play_game(mode, pid);

            if(!is_clear)
                return false;

            Ranker player(name,mine_time);
            if(is_clear && rankerList.isRanker(player))
            {
                system("tput cup 20 1"); setColor();
                cout<<"You are ranker! Would you register?(y/n) "; cin>>ch;

                if(ch=='y')
                {
                    system("tput cup 21 10"); setColor();
                    cout<<"Name: "; cin>>name;

                    Ranker ranker(name,mine_time);
                    rankerList.addNewRanker(ranker);
                    rankerList.updateRankFile();
                }
                deleteColor();
                return true;
            }

        }
        else {
            printf("fork() error!");
        }
    }
    
    void show_time(int t) {
    for(int i = 0 ; i < 10; i++) {
        if(i == 8){
            setColor();
            cout<<"[ TIME: "<< t <<" ]";
            deleteColor();
        }
        else
            printf(" ");
    }
    }
    

#ifdef __cplusplus
}
#endif

#endif