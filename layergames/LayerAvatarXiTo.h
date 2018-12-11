//
//  LayerAvatarXiTo.h
//  iCasinov2
//
//  Created by DauA on 5/28/14.
//
//

#ifndef __iCasinov2__LayerAvatarXiTo__
#define __iCasinov2__LayerAvatarXiTo__

#include <iostream>
#include "cocos2d.h"
#include "AvatarXiTo.h"
#include "GameServer.h"

using namespace cocos2d;
using namespace std;

class LayerAvatarXiTo : public Layer,public PlayerCallBack
{
public:
    LayerAvatarXiTo();
    ~LayerAvatarXiTo();
    virtual bool init();
    CREATE_FUNC(LayerAvatarXiTo)

    void SetIcon(const int& pos, const char *url);
    void SetFlag(const int& pos, bool isShow);
    void SetName(const int& pos, const char *name = "");
    void SetMoney(const int& pos, const char *money = "");

	void ResetAvatar(Avatar* avatar);

    void ResetAll();
    void StopAllTimer();
	void RunTimer(const int& pos);

	void UnReadyAllUser();

    AvatarXiTo* GetUserByPos(const int& pos);

	void ShowChatByPos(const int& pos, const string& mes);
    void ShowTypeBetInPos(const int& pos, const string& mes);

    void hideAllBetType();

    void SetIsBoss(const int& pos, bool isBoss = false);

    bool SetListSpectToEmpty(string& list, const string& myAI);
    
    void	OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);


    void showIconLeaveGame(bool state);
    void checkSameIp();

private:
    AvatarXiTo* User_IsMe;
    AvatarXiTo* User_Left_Bottom;
    AvatarXiTo* User_Right_Bottom;
    AvatarXiTo* User_Left_Top;
    AvatarXiTo* User_Right_Top;

    AvatarXiTo* GetEmptyAvatar();
    void ResetListSpect();
    vector<Avatar*> lstSpec;
};

#endif /* defined(__iCasinov2__LayerAvatarXiTo__) */
