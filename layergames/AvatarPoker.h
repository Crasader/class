#ifndef AVATARPOKER_H
#define AVATARPOKER_H

#include "_Avatar_.h"
#include "XiToHelper.h"

class AvatarPoker : public Avatar
{
private:
    Label* lblTypeBet;
    Sprite* flag;
    Sprite* typePlayer;
    Sprite* betType;
    
public:
    ~AvatarPoker();
    AvatarPoker();
    

    bool init();

    static AvatarPoker* create();

    void showTypeBet(const int& type);
    void setPosDeal(int type);
    void setTypePlayer(int type);
    void hideTypeBet();

    Sprite* getFlag();

    void ResetAll();

    void SetInfo(boost::shared_ptr<XiToInfoPlayer> info);
    void reloadCoin();

    virtual void showChat(string& mess);
    int TypeBet = -1;
    
    void showLoseEffect();
    void showNormalEffect();
    void showEffectWin();
};

#endif // AvatarPoker_H
