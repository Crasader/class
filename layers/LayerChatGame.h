#ifndef LayerChatGame_h__
#define LayerChatGame_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "../GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;
using namespace std;

class LayerChatGame :
public Layer,
public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate,
public EditBoxDelegate,
public PlayerCallBack

{
private:
    struct structChat{
        string Name;
        string Content;
        bool isadm;
        bool isnotify;
        structChat(string _name, string _content,bool _isadm, bool _isnotify) :
        Name(_name), Content(_content),isadm(_isadm),isnotify(_isnotify){};
    };
	TextField* txtNoiDung;
	TableView* tblChat;
	vector<structChat> lstChat;
	Button* btnSendChat;
	Layout* panelTable;
    Layout* pnlChat;
    cocos2d::EventListener* _touchListener;

    ImageView* imgBg;
    ImageView* imgNoti;
    Text* txtCount;
    int countMessage = 0;
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)override;

    void showNoti(int count);
	void onBtnSendChat(Ref* pSender);
    void onBtnOpen(Ref* pSender, ui::Widget::TouchEventType type);
    

	void sendChat();
	void loadFirstCell();
	float getHeightForCell(string _strMsg);
	TableViewCell* createCell4Chat(TableView* table, ssize_t idx);
	void parseChat(string json);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
	bool isImageExist(string url);
    virtual void editBoxEditingDidBegin (EditBox *editBox)override;
    virtual void editBoxEditingDidEnd (EditBox *editBox)override;
    virtual void editBoxReturn (EditBox *editBox)override;

protected:
	void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
public:
	LayerChatGame();
	~LayerChatGame();
    void loadAllDatas();

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual bool init();
	CREATE_FUNC(LayerChatGame);
};

#endif // LayerChatGame_h__


