
#ifndef __RANK_HPP__
#define __RANK_HPP__

#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

class Ranker {
  public:
    Ranker(string name, size_t time);
    string getName() const;
    size_t getTime() const;

    string setName(string newName){name=newName;}
    bool operator<(const Ranker &ranker);
    // RankerList 클래스의 rakerSort()를 위한 비교 연산자 오버라이딩

  private:
    string name;
    size_t time;
};

class RankerList {
  public:
    RankerList();
    void printRankerList();
    void rankerSort();
    bool isRanker(const Ranker &r);
    // r이 ranking 순위권에 드는지 확인
    void addNewRanker(const Ranker &r);
    // r이 새로운 ranker이면, rankerList 벡터에 push하고 정렬
    void updateRankFile();
    // rankerList 벡터의 내용을 ranking.txt 파일에 저장(갱신)

  private:
    vector<Ranker> rankerList;
};

#endif
