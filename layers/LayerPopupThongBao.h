#ifndef LayerPopupThongBao_h__
#define LayerPopupThongBao_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
class LayerPopupThongBao : public Layer,
	public	PlayerCallBack
{
public:
	LayerPopupThongBao();
	~LayerPopupThongBao();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void onButtonHuy(Ref* pSender);
	void onButtonDongy(Ref* pSender);
	void setMessage(string msg);
	void setContentMess(string msg);
	void showAButton();
    void setPopupClose();
	Button* getBtnHuy();
	Button* getBtnOk();
	Text* getLblTitle();

	void setCallBackFromButton(Ref* target, SEL_CallFuncO listener, int tag);

	virtual bool init();
	CREATE_FUNC(LayerPopupThongBao);
protected:
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	cocos2d::EventListener* _touchListener;
	Text* lblMessage;
	Text* lblTitle;
	Button *btn_huy, *btn_dongy;
	SEL_CallFuncO mCallBackListener;
	Ref *mCallBack;
	int tagcallback;
	void sendRequestCountMess(float dt);
    bool isThoat = false;
};

class LayerPopupWebView :public Layer, public PlayerCallBack{
public:
	LayerPopupWebView();
	~LayerPopupWebView();
	virtual bool init();
	CREATE_FUNC(LayerPopupWebView);
	void setWebView(string urlWeb);//set link de hien thi webview
	void setWebFile(string pathFile);
private:
	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	cocos2d::experimental::ui::WebView *webView;
	bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
	void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
};
class LayerPopupDanhSachCai :public Layer,
	public PlayerCallBack, public TableViewDataSource,
	public TableViewDelegate{
public:
	LayerPopupDanhSachCai();
	~LayerPopupDanhSachCai();
	virtual bool init();
	virtual bool init(std::string _lstCaiReg);
	//CREATE_FUNC(LayerPopupDanhSachCai);
	static LayerPopupDanhSachCai* create();
	static LayerPopupDanhSachCai* create(std::string _lstCaiReg);

	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
	void event_EXT_EVENT_GET_LIST_BOSS_RES(boost::shared_ptr<ISFSObject> param);
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	Button* getButtonOk();
	Button* getButtonCancel();
private:
	Button *btnOk;
	Button *btnCancel;
	TableView* tblLstBoss;
	vector<std::string> lstBoss;

};

class LayerItemBoss : public Layer{
public:
	LayerItemBoss();
	~LayerItemBoss();
	CREATE_FUNC(LayerItemBoss);
	virtual bool init();

	void setData(string _username, int idx);
private:
	Text* lblPos;
	Text* lblName;
	Text* lblStatus;
};

class LayerPopupActiveCai : public Layer{
public:
	LayerPopupActiveCai();
	~LayerPopupActiveCai();
	virtual bool init();
	CREATE_FUNC(LayerPopupActiveCai);
	Button* getButtonOk();
	Button* getButtonExit();
private:
	Button *btnOk;
	Button *btnExit;

	void processButtonExitTouched(Ref* pSender, ui::Widget::TouchEventType type);
	void processButtonCupCaiTouched(Ref* pSender, ui::Widget::TouchEventType type);
};
#endif // LayerPopupThongBao_h__
