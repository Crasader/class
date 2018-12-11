#ifndef _iCasino_v2_SceneGame_
#define _iCasino_v2_SceneGame_

#include "cocos2d.h"
#include "Common.h"
#include "cocos-ext.h"
#include "GameServer.h"
#include "../SceneManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SceneGame : public cocos2d::Layer, PlayerCallBack
{
private:
	struct StructFriendInfo{
		int uid;
		string aI;
		string aN;
		string amf;
	};
	int inviteCount;
	void autoInviteRoom();
protected:
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
public:
	SceneGame();
	~SceneGame();

	static SceneGame* createScene(int typeGame);

	void loadGameByTag(int typeGame);

	virtual bool init();
    void timeoutChatMaxtime(float dt);
    void startTimeOutChat();
	CREATE_FUNC(SceneGame);
};

#endif //_iCasino_v2_SceneGame_
