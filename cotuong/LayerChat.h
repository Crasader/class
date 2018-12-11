#ifndef LayerChat_h__
#define LayerChat_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "CommonChess.h"
#include "../GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class LayerChat : public cocos2d::Layer,
	public cocos2d::extension::TableViewDataSource,
	public cocos2d::extension::TableViewDelegate,
	public PlayerCallBack
{
public:
	virtual bool init() override;
	LayerChat();
	~LayerChat();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerChat);
	void setListChat(string _username, string _msg);
	vector<ChatInfoStruct> getListChat();

	void showChat();
	void hideChat();
protected:
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	cocos2d::ui::ScrollView* svChat;
	Button* btnEmotion;
	TextField* txtContent;
	Button* btnSend;
	Layout* pnlContent;
	TableView* tblChat;
	bool isBossRoom;
	bool isSpectator;
	bool isAllowChat;
	Sprite* bt_sound2_24;

	std::vector<std::string> listChats;
	std::vector<ChatInfoStruct> listStore;
	void initChatContent();
	void initEmotion();
	void onButtonPanelChat(Ref* pSender, Widget::TouchEventType type);
	void onButtonClose(Ref* pSender);
	void onButtonContentChat(Ref* pSender);
	void onButtonChat(Ref* pSender);
	void doChat(string txt, bool isEmo);

	void onButtonEmotion(Ref * pSender);
	bool checkSpectator(string listUser, string myName);
};

#endif // LayerChat_h__



