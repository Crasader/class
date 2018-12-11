#ifndef CHANORDERALG_H
#define CHANORDERALG_H

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class ChanCa{
private:
    int SoChan;
    int SoCa;
    int SoBaDau;
    int SoQue;

    int cardnu;
    vector<int> cardsu;
public:
    ChanCa(const int& cardnu);
    ~ChanCa();
    void PushSuite(const int& cardsu);

    int& GetCardNumer();

    void Calculator();

    int GetCa();
    int GetChan();
    int GetBaDau();
    int GetQue();

    void Prints();
};

class ChanOrderAlg
{
    vector<ChanCa*> lstChanCa;
public:
    ChanOrderAlg();
    ~ChanOrderAlg();
    void Push(const int& nu, const int& su);

    void Calculator();
    bool CheckU();
private:
    ChanCa* GetChanCa(const int& nu);
};

#endif // CHANORDERALG_H
