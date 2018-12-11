#ifndef __iCasino_v2__AvatarInGameChan_TVDA__
#define __iCasino_v2__AvatarInGameChan_TVDA__

#include "cocos2d.h"
#include "AvatarChan.h"

using namespace cocos2d;
using namespace std;


class AvatarInGameChan : public Layer
{
public:
	AvatarInGameChan();
	~AvatarInGameChan();
	bool init();
	CREATE_FUNC(AvatarInGameChan);

	void SetIcon (const int& pos, char *url);
	void SetName (const int& pos, string name);
	void SetMoney(const int& pos, string money);
	void SetMoney(const int& pos, int money);
	void SetMoney(const int& pos, double money);
	void SetReady(const int& pos, bool isReady);
	void SetVisibleLayerInvite(const int& pos, bool isShow);

	void UnReadyAllUsers();
	void ResetGame();
	void ResetAll();

	AvatarChan* GetUserByPos(const int& pos);

	void RunTimer(const int& posUser);
	void RunTimerWithTime(const int& posUser, const float& time);
	void StopAllTimers();

	void StopAllBlinkAvatar();
};

#endif