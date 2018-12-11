#ifndef AVATARXITO_H
#define AVATARXITO_H

#include "_Avatar_.h"
#include "XiToHelper.h"

class AvatarXiTo : public Avatar
{
private:
    Label* lblTypeBet;
    Sprite* flag;

public:
    ~AvatarXiTo();
    AvatarXiTo();

    bool init();

    static AvatarXiTo* create();

    void showTypeBet(const string& type);
    void hideTypeBet();

    Sprite* getFlag();

    void ResetAll();

    void SetInfo(boost::shared_ptr<XiToInfoPlayer> info);
    void reloadCoin();

    virtual void showChat(string& mess);
};

#endif // AVATARXITO_H
