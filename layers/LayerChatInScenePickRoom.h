#ifndef LayerChatInScenePickRoom_h__
#define LayerChatInScenePickRoom_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "LayerChatInGame.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;

class LayerChatInScenePickRoom : public cocos2d::Layer,
	public PlayerCallBack,
	public TableViewDataSource, public TableViewDelegate
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
	Label* lblRoomName;
	Button* btnPlayNow;
	Button* btnSendChat;
	Layout* panelTable;
	boost::shared_ptr<Room> roomChat;
	LayerChatInGame *layerChatInGame;

	void onBtnSendChat(Ref* pSender, ui::Widget::TouchEventType type);
	void sendChat();
	void loadFirstCell();
	float getHeightForCell(string _strMsg);
    void onBtnCoBam(Ref* pSender, ui::Widget::TouchEventType type);
	TableViewCell* createCell4Chat(TableView* table, ssize_t idx);
    
    void onBtnBack(Ref* pSender);
    void onBtnHotLine(Ref* pSender);
    void onBtnSuKien(Ref* pSender);

	void parseChat(string json);
    void scheduleChat(float dt);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
	bool isImageExist(string url);
protected:
	void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
public:
	LayerChatInScenePickRoom();
	~LayerChatInScenePickRoom();

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual bool init();
	CREATE_FUNC(LayerChatInScenePickRoom);
	Button* getButtonPlayNow();
    void onBtnNapTien(Ref* pSender);
    void setScene(int type);

    Button* btnCoBam;
	void createPopupChat();
    void reload(float dt);
    Label* txtDieuKien;
    Layout* pnlThongBao;
    Button* btnBack;
    Button* btnHotLine;
    Button* btnSuKien;
    void loadAllDatas();
    void showNapTien(float dt);
    bool isJoin = false;
public:
    Sprite* sprKhuyenMai;
    TextBMFont* txtKhuyenMai;
};

#endif // LayerChatInScenePickRoom_h__


