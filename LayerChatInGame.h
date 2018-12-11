//
//  LayerChatInGame.hpp
//  iCasino_v4
//
//  Created by Tuan Anh on 1/19/16.
//
//


#ifndef LayerChatInGame2_h__
#define LayerChatInGame2_h__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocosbuilder/CocosBuilder.h"
#include "cocostudio/CocoStudio.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerChatInGame : public Layer,
	public PlayerCallBack
{
public:
	LayerChatInGame();
	~LayerChatInGame();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual bool init();
	void doChat(string txt, bool isEmo);
	void show();
	void hide();
	void onBtnSend(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonClicked(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
	void reportResultGame(string& result);
	CREATE_FUNC(LayerChatInGame);
	CC_SYNTHESIZE(bool, _ispick, IsPickRoom);//neu la scene pick room thi cach gui bna tin chat se khac
private:
	struct ChatItem
	{
		string username;
		string message;
	};
	bool isShouldHide;
	TextField* txtChat;
	PageView* pvContent;
	PageView* pvEmo;
	ImageView* bgImage;
	vector<string> listChat;

	void createPageView();
	void createPageViewEmo();
	void createListChat();
	void showToast(string msg);
	void getListChatByID(int gameid);
	bool checkSpamChat(string txt);
};

/*moc3-add*/
class LayerMocChatInGame : public Layer, PlayerCallBack{
public:
	LayerMocChatInGame();
	~LayerMocChatInGame();
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual bool init();
	void doChat(string txt, bool isEmo);
	//	void show();
	void hide();
	void onBtnSend(Ref* pSender, ui::Widget::TouchEventType type);
	void processButtonClicked(Ref* pSender, ui::Widget::TouchEventType type);
	void processButtonEmoTouched(Ref* pSender, ui::Widget::TouchEventType type);
	void processButtonTextTouched(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
	void reportResultGame(string& result);
	CREATE_FUNC(LayerMocChatInGame);
	CC_SYNTHESIZE(bool, _ispick, IsPickRoom);//neu la scene pick room thi cach gui bna tin chat se khac
private:
	struct ChatItem
	{
		string username;
		string message;
	};

	//	bool isShouldHide;
	TextField* txtChat;
	PageView* pvContent;
	PageView* pvEmo;
	Button *btnEmo, *btnText;
	//	ImageView* bgImage;
	vector<string> listChat;
	//
	void createPageView();
	void createPageViewEmo();
	void createListChat();
	void showToast(string msg);
	void getListChatByID(int gameid);
	bool checkSpamChat(string txt);
	int countSpace(std::string _string);
};
/*moc3-end change*/
#endif
