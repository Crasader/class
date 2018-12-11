#ifndef QUEUETAIXIU_H
#define QUEUETAIXIU_H

#include <string>
#include <iostream>
#include <vector>
using namespace std;

class BetInfo {
public:
    BetInfo();
    ~BetInfo();

    string& GetType();
    long& GetTotalBet();
    long& GetPlayerBet();

    void SetType(const string& type);
    void SetTotalBet(const long& val);
    void SetPlayerBet(const long& val);

private:
    std::string mType;
    long mTotalBet;
    long mPlayerBet;
};

class QueueTaiXiu
{
public:
    QueueTaiXiu();
    ~QueueTaiXiu();

    void Push(const string& type, const long& total, const long& playerbet);
    void RemoveAll();

    vector<BetInfo*>& GetArrayBetInfo();

private:
    BetInfo* GetBetInfo(const string& type);
    vector<BetInfo*> pArrayInfo;
};

#endif // QUEUETAIXIU_H
