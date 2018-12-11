#ifndef BOBAICHAN_H
#define BOBAICHAN_H

#include <vector>
#include "CardChan.h"
#include "ChanUtils.h"

using namespace std;

class BoBaiChan
{
private:
    static BoBaiChan* instance;
    BoBaiChan();
    std::vector<CardChan*> arrayAllCards;
    vector<CardOrg> arrayCanChiu;
    int mark;
public:
    ~BoBaiChan();
    static BoBaiChan* GetInstance();
    static void Destroy();

    CardChan* PopCard();
    void BackCard(CardChan* pCard);
    void ResetAllCards();

    std::vector<CardChan*>& GetArrayCards();

    void SetCardCanChiu(std::string& ls);
    void RemoveElemsChiu(const CardOrg& card);
    bool CheckCanChiu(const CardOrg& card);

    void ShowCardU(const int& cardnu, const int& cardsu);
    void ShowU(const int& id);
};

#endif // BOBAICHAN_H
