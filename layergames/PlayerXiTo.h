#ifndef PLAYERXITO_H
#define PLAYERXITO_H

#include <vector>
#include "cocos2d.h"
#include "mUtils.h"
#include "_Card_.h"
#include "FrameBet.h"
#include "cocos-ext.h"
#include "XiToHelper.h"

using namespace cocos2d;
using namespace std;

class PlayerXiTo : public Layer
{
private:

protected:
    std::string aI;

    float WithOrig;
    float HeiOrig;

    float LeftCard;
    float BottomCard;

    float WidtdCard;
    float HeightCard;
    std::vector<Card*> ArrayCards;

    //
    int CurrentTypeBet; // To 1/4, X2, 1/2
    float CurrentValueBet;
    float TotalBet;

    Label* lblVicType;
	ImageView *bgVicType;

    bool IsFold;
    string sListCardMe;

    vector<Label*>arrAnimMoney;

protected:
    void RestoreCard(string &listcards, bool hasopen);
    void ShowMoneyCallFunc(Ref *sender, std::string data);
public:
    PlayerXiTo();
    ~PlayerXiTo();

    bool init();

    void AddCards(Card* pCard);
    void RemoveAllCards();

    void ReceiveCardFirst();
    virtual void ReceiveCard(const std::string& allc, const string& url);

    //
    virtual void RefreshListCards(Ref* sender, std::string& listID);

    bool CheckCardRunning();

    //
    Vec2 PosCard();

    // Set lai id cho list cards
    void SetIdListCards(string& lc);

    // Hien gia tri bo bai tren tay: thung, sanh
    void DisplayValueListCards(string& lc);

    //Getter, Seter
    void SetPosBeginCard(const float& left, const float& bottom);
    Vec2 GetPosBeginCard();
    void SetSizeCard(const Size& size);

    float GetWitdCard() const;
    float GetHeightCard() const;
    float GetLeftCard() const;
    float GetBottomCard() const;

    int GetCurrentTypeBet() const;
    float GetCurrentValueBet() const;
    float GetTotalBet() const;

    void SetAI(const string& aI);
    string GetAI() const;

    Vec2 GetVicTypePosition();

    //Show Money
    void ShowMoney(const string& amf, bool b = true);
    void BoCuoc();

    //
    void ResetTurn();

    void ShowVicType(const string& type);

    //
    virtual void MoveDealCard(const int& idCard) = 0;
    virtual void UpBai() = 0;
    virtual void CreateFrameBet(const float&x, const float& y, const int& sign) = 0;
    virtual void SetBet(const long& bettype, const double& betvalues) = 0;
    virtual void HideFrameBet() = 0;
    virtual void RestoreListCard(std::string& listcards, bool hasopen) = 0;
};

class PlayerXiToIsMe : public PlayerXiTo
{
private:
    Sprite* spFrameBetMe;
    Label* lblBetMe;
public:
    PlayerXiToIsMe();
    ~PlayerXiToIsMe();
    bool init();
    CREATE_FUNC(PlayerXiToIsMe)
    int DisplayTwoCard(string& listdeal);
    void MoveDealCard(const int& idCard);
    void RestoreListCard(std::string& listcards, bool hasopen);
    void UpBai();

    //
    void CreateFrameBet(const float&x, const float& y, const int& sign);
    void SetBet(const long& bettype, const double& betvalues);
    void HideFrameBet();

    void RefreshListCards(Ref* sender, std::string& listID);

    // Set tien to truoc server.
    void SetBetBeforeServer(const double& bet);
};

class PlayerXiToNormal : public PlayerXiTo{
private:
    FrameBet* frameBet;
public:
    PlayerXiToNormal();
    ~PlayerXiToNormal();
    bool init();
    CREATE_FUNC(PlayerXiToNormal)
    void MoveDealCard(const int& idCard);
    void RestoreListCard(std::string& listcards, bool hasopen);
    void UpBai();

    //
    void CreateFrameBet(const float&x, const float& y, const int& sign);
    void SetBet(const long& bettype, const double& betvalues);
    void HideFrameBet();
};



#endif // PLAYERXITO_H
