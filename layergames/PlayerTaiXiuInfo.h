#ifndef PLAYERTAIXIUINFO_H
#define PLAYERTAIXIUINFO_H

#include <string>
#include <vector>
#include "TaiXiuBet.h"

using namespace std;

struct Struct4Call1 {
    TaiXiuBet* bet;
    long total;
};

class PlayerTaiXiuInfo
{
public:
    PlayerTaiXiuInfo();

    long SoTienThuc = 0;
    long SoTienAo = 0;
    long SoTienDatCuocClient = 0;
    long SoTienDatCuocServer = 0;
    vector<string> ArrayBet;

    void initMoney(const long& money);

    void reset ();

    void TruTienThuc(const long& money);

    void SetSoTienThuc(const long& money);
    void PushBet(string& bet);
    bool CheckBet(const std::string& bet);

    vector<string>& GetAllBet();
};

#endif // PLAYERTAIXIUINFO_H
