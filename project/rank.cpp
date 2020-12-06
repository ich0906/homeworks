#include "rank.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Ranker
Ranker::Ranker(string name, size_t time) : name(name), time(time) {}
string Ranker::getName() const { return name; }
size_t Ranker::getTime() const { return time; }

bool Ranker::operator<(const Ranker &ranker) {
    return this->getTime() < ranker.getTime();
}

// RakerList
RankerList::RankerList() { // rank.txt 파일 읽어서 rank vector 생성
    const string rankFile = "./ranking.txt";
    system("touch ranking.txt");
    ifstream readFile(rankFile);

    if (!readFile.is_open()) {
        perror("ifstream open() error!");
        exit(-1);
    }

    string name, time_str, ignore;
    size_t time;

    // rank.txt 파일에 저장된 Ranker를 RankerList 벡터에 저장
    while (!readFile.eof()) {
        getline(readFile, name);
        if (name.size() == 0)
            break;
        getline(readFile, time_str);
        getline(readFile, ignore);
        time = (size_t)stoi(time_str);

        rankerList.push_back(Ranker(name, time));
    }

    readFile.close();
}

void RankerList::printRankerList() {
    string cmd;
    for (int i = 0; i < rankerList.size(); i++) {
        string cmd = "tput cup " + to_string(2 * i + 5) + " 18";
        system(cmd.c_str());

        if (i == 9)
            cout << i + 1 << "     " << rankerList[i].getName() << "      "
                 << rankerList[i].getTime() << endl;
        else
            cout << i + 1 << "      " << rankerList[i].getName() << "      "
                 << rankerList[i].getTime() << endl;
    }
}

void RankerList::rankerSort() { sort(rankerList.begin(), rankerList.end()); }

bool RankerList::isRanker(const Ranker &r) {
    if (rankerList.size() < 10) // 저장된 랭커가 10명 이하이면
        return true;            // r은 바로 새로운 랭커가 됨
    else
        return r.getTime() < rankerList[rankerList.size() - 1].getTime();
    // rankerList 벡터의 10등(마지막 랭커)보다 클리어 타임 빠르면 랭킹 순위권
}

void RankerList::addNewRanker(const Ranker &r) {
    // r이 새로운 ranker이면
    if (isRanker(r)) {
        if (rankerList.size() == 10) // 랭커가 10명으로 Full이면
            rankerList.pop_back();   // 기존 랭커 10등을 삭제

        rankerList.push_back(r); // r을 rankerList 벡터에 push
        rankerSort();            // rankerList 벡터 sort
    }
}

void RankerList::updateRankFile() {
    const string rankFile = "./ranking.txt";
    ofstream writeFile(rankFile);
    if (!writeFile.is_open()) {
        perror("ofstream() open() error!");
        exit(-1);
    }

    string name, time;

    for (size_t i = 0; i < rankerList.size(); i++) {
        name = rankerList[i].getName();
        writeFile.write(name.c_str(), name.size());
        writeFile.write("\n", strlen("\n"));

        time = to_string(rankerList[i].getTime());
        writeFile.write(time.c_str(), time.size());

        writeFile.write("\n\n", strlen("\n\n"));
    }

    writeFile.close();
}
