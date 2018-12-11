#ifndef LayerBottomInGame_h__
#define LayerBottomInGame_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "../GameServer.h"
#include "LayerPopupAlert.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerBottomInGame : public cocos2d::Layer,
	public PlayerCallBack
{
public:
	virtual bool init() override;
	LayerBottomInGame();
	~LayerBottomInGame();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerBottomInGame);
	void setShowSetting(bool isShow);
	void checkShowSetting();
protected:
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	enum TAG_ALERT{
		TAG_LEAVE_GAME,
		TAG_PEACE_REQUEST,//yeu cau cau hoa
		TAG_PEACE_RESPONSE,//phan hoi cau hoa
		TAG_LOOSE,
	};
	Sprite* spVisitor;
	Button* btnBack, *btnSound, *btnPeace, *btnSetting, *btnLose;
	bool isShowSetting;//bien kiem tra xem co cho phep cai dat trong phong hay khong - chi chu phong default la duoc bat
	string listUser;

	void onButtonBack(Ref* pSender);
	void onButtonSetting(Ref* pSender);
	void onButtonPeace(Ref* pSender);
	void onButtonLose(Ref* pSender);
	void onButtonSound(Ref* pSender);

	void event_EXT_EVENT_GAME_LEAVE_RES(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_FOR_PEACE_NTF(boost::shared_ptr<ISFSObject> param);
	void event_EXT_EVENT_PRE_CHANGE_SETTING_RESPONSE(boost::shared_ptr<ISFSObject> param);
	void createLayerNotification(string title, string content_mess, int tag);
	void callBackFromAlert(LayerPopupAlert* sender, int tag);

	void showSpectatorMode(bool isSpec);
	bool checkSpectator(string listUser,string myName);
};

#endif // LayerBottomInGame_h__



