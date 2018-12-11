//
//  LayerAvatarPoker.h
//  iCasinov2
//
//  Created by DauA on 5/28/14.
//
//

#ifndef __iCasinov2__LayerAvatarPoker__
#define __iCasinov2__LayerAvatarPoker__

#include <iostream>
#include "cocos2d.h"
#include "AvatarPoker.h"
#include "GameServer.h"

using namespace cocos2d;
using namespace std;

class LayerAvatarPoker : public Layer,public PlayerCallBack
{
public:
    LayerAvatarPoker();
    ~LayerAvatarPoker();
    virtual bool init();
    CREATE_FUNC(LayerAvatarPoker)

    void SetIcon(const int& pos, const char *url);
    void SetFlag(const int& pos, bool isShow);
    void SetName(const int& pos, const char *name = "");
    void SetMoney(const int& pos, const char *money = "");

	void ResetAvatar(Avatar* avatar);

    void ResetAll();
    void StopAllTimer();
	void RunTimer(const int& pos);

	void UnReadyAllUser();

    AvatarPoker* GetUserByPos(const int& pos);

	void ShowChatByPos(const int& pos, const string& mes);
    void ShowTypeBetInPos(const int& pos, const int& type);
    void ShowTypePlayer(const int& pos,int type);
    void hideAllBetType();

    void SetIsBoss(const int& pos, bool isBoss = false);

    bool SetListSpectToEmpty(string& list, const string& myAI);
    
    void	OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);


    void showIconLeaveGame(bool state);
    void checkSameIp();

private:
    AvatarPoker* User_IsMe;
    AvatarPoker* User_Left_Bottom;
    AvatarPoker* User_Right_Bottom;
    AvatarPoker* User_Left_Top;
    AvatarPoker* User_Right_Top;

    AvatarPoker* GetEmptyAvatar();
    void ResetListSpect();
    vector<Avatar*> lstSpec;
};

#endif /* defined(__iCasinov2__LayerAvatarPoker__) */
