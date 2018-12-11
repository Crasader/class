#ifndef PLAYERCHANNORMAL_H
#define PLAYERCHANNORMAL_H

#include "PlayerChan.h"

class PlayerChanNormal : public PlayerChan
{
public:
    PlayerChanNormal();
    ~PlayerChanNormal();

    void ActionDiscardByServer(const CardOrg& card);
    void ActionTraCuaByServer(const CardOrg& card);
    void ActionChuyenBaiByServer(const CardOrg& card);
    void ActionAnCuaTrenByServer(const CardOrg& card);
    void ActionChuyenBaiChiuByServer(const CardOrg& card);

    int ActionDisCardByClient();
    int ActionChiuCardByClient();
    int ActionTakeCardByClient(const CardOrg& lastCard);

    int SetListCards(std::string& listcards);
};

#endif // PLAYERCHANNORMAL_H
