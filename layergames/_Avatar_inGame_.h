
#ifndef __iCasino_v2___Avatar_inGame___
#define __iCasino_v2___Avatar_inGame___

#include <iostream>

#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "AllData.h"
#include "_Avatar_.h"
#include "_Chat_.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;
using namespace cocos2d::ui;

class LayerAvatarInGame : public Layer
{
public:
	LayerAvatarInGame();
	~LayerAvatarInGame();

    virtual bool init();
    CREATE_FUNC(LayerAvatarInGame)
    
    void setIcon (const int& pos, char *url);
    void setFlag (const int& pos, bool isShow);
    void setName (const int& pos, string name);
    void setMoney(const int& pos, string money);
	void setMoney(const int& pos, int money);
	void setMoney(const int& pos, double money);
    void setReady(const int& pos, bool isReady);
	void setVisibleLayerInvite(const int& pos, bool isShow);
    
    void resetAll();
    
    void setUnReadyAllUser();

    void setListUserByTienLen(string listUser);
    
    Avatar* getUserByPos(const int& pos);

    //
    int getPosByName(string name);
    int getPosByAccountID(string aI);
    //
    
    void runTimer(const int& posUser);
	void runTimerWithTime(const int& posUser, const float& time);
    void stopAllTimer();

	bool isSpectator();
	bool isStartedGame();

    void stopAllBlinkAvatar();
    void SetIsSpect(bool isSpect);

    int getUserCount();

    void ShowBiBao(const int& pos, bool state);

    void SetEndGame();

    void SetMeIsBoss(bool isBoss);

    bool SetListSpectToEmpty(std::string& list, const std::string& myAI);
    Avatar* GetEmptyAvatar();
    void ResetListSpect();

    //
    void SetEmotionInPos(const int& pos, string& chat);

    void showIconLeaveGame(bool state);

    void checkSameIp();
private:
    AvatarChanNormal* playerMe;
    AvatarChanNormal* playerLeft;
    AvatarChanNormal* playerRight;
    AvatarChanNormal* playerTop;
    AvatarChanNormal* playerBottom;

    bool isSpector;
    string myAI;
};

#endif
