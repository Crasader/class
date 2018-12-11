#ifndef LayerChatRoom_h__
#define LayerChatRoom_h__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameServer.h"
#include "../Common.h"
#include "../layergames/CircleAvatar.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class ListRoomCell : public Layer{
public:

	ImageView* bgPressed;
	virtual bool init();
	void setDatas(string roomName);

	CREATE_FUNC(ListRoomCell);
private:
	Text* labelNameRoom;
	
};
class ListContentCell : public Layer{
public:
	virtual bool init();
	void setDatas(string name, string content,int accviptype,string avturl,bool isMe);
	CREATE_FUNC(ListContentCell);
private:
	Text* lblContent;
	Text* lblName;
    ImageView* avtVip;
    CircleAvatar* circleAvatar;
    ImageView* avtIMG;
};
class LayerChatRoom : 
	public Layer,
	public TableViewDataSource,
	public TableViewDelegate,
	public PlayerCallBack{
public:
	LayerChatRoom();
	~LayerChatRoom();
	
	TableViewCell* createCell4Left(TableView *table, int idx);
	TableViewCell* createCell4Right(TableView *table, int idx);

	void loadDatas();
	void loadFirst();

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override;
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override;
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	virtual bool init();

	void onBtnBack(Ref* pSender);
	void onBtnSendChat(Ref* pSender);

	CREATE_FUNC(LayerChatRoom);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
private:
	enum tagTableView{
		tagTableLeft = 0,
		tagTableRight = 1
	};
	struct ChatItem
	{
		string name;
		string message;
        int accviptype;
        string urlavt;
	};
	vector<ChatItem> lstChats;
	TableView* tblLeft;
	TableView* tblRight;
	TextField* txtNoiDung;
	Text* lblRoomName;
	Layout* panelLeft;
	Layout* panelRight;
};

#endif // LayerChatRoom_h__
