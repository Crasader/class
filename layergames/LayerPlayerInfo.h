//
//  LayerPlayerInfo.h
//  demo
//
//  Created by AlexDao on 5/24/14.
//
//

#ifndef demo_LayerPlayerInfo_h
#define demo_LayerPlayerInfo_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocosbuilder/CocosBuilder.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocosbuilder;

class LayerPlayerInfo
: public cocos2d::Layer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
, public PlayerCallBack
{
private:
	Label* lblAMF;
	Label* lblName;
	Label* lblSex;

	MenuItem* btnInviteFriend;
	MenuItem* btnUnFriend;
	MenuItem* btnKick;

	Node* nodeAvatar;

	string mFriendID;

	cocos2d::EventListener* _touchListener;
public:
    LayerPlayerInfo();
	~LayerPlayerInfo();
	CC_SYNTHESIZE(bool, isPlaying, Playing);

//	virtual void registerWithTouchDispatcher(void);
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void setFriendId(string friendID);
	void setAvatarImage(Sprite*);
	void setAvatarUrl(string url);
	void reloadAllDatas();
	void setIsBossRoom(bool isBoss);
    
    CREATE_FUNC(LayerPlayerInfo);
    
	// CCBSelectorResolver interfaces
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref * pTarget, const char* pSelectorName);
	virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
	virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
	virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(Ref * pTarget, const char* pSelectorName){
		return NULL;
	};
    // selector callbacks
	void onButtonInviteFriend(Ref* pSender);
	void onButtonUnFriend(Ref* pSender);
	void onButtonKick(Ref* pSender);
	void onButtonClose(Ref* pSender);
	//Server
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
};

class LayerPlayerInfoLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LayerPlayerInfoLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LayerPlayerInfo);
};

#endif
