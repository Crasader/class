#ifndef PLAYERCHANISME_H
#define PLAYERCHANISME_H

#include "PlayerChan.h"

class PlayerChanIsMe : public PlayerChan
{
private:

public:
    PlayerChanIsMe();
    ~PlayerChanIsMe();

    // Xoe quat.
    void RefreshListCards();

    // Check Co the
    bool CheckU(const CardOrg& card);

    void ActionDiscardByServer(const CardOrg& card);
    void ActionTraCuaByServer(const CardOrg& card);
    void ActionChuyenBaiByServer(const CardOrg& card);
    void ActionAnCuaTrenByServer(const CardOrg& card);
    void ActionChuyenBaiChiuByServer(const CardOrg& card);

    //
    int ActionDisCardByClient();
    int ActionChiuCardByClient();
    int ActionTakeCardByClient(const CardOrg& lastCard);

    //
    void ScaleCardsHand_WhenU();

    //
    void ChiuSuccess(const int& rescode);
    void TakeSuccess(const int& rescode);
    void DisSuccess(const int& rescode);

    //
    int SetListCards(std::string& listcards);

    void RotateListCards();

    void BeforeBocBai();

    //
    void StartTurn();
    void FinishTurn();

    virtual void RePainTrenTay();

    void SetListCardFirst(string& lstCards);

private:
    void CardTouch(Ref *pSender, ui::Widget::TouchEventType type);
};

#endif // PLAYERCHANISME_H
