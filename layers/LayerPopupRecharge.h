#ifndef __iCasino_v2__LayerPopupRecharge__
#define __iCasino_v2__LayerPopupRecharge__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "LayerPopupLuckyNumber.h"
#include "../Common.h"
#include "../layergames/NativeHelper.h"
#include "../data_manager.h"
#include "network/HttpClient.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "mIAP.h"
#endif

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocos2d::network;

class LayerItemSMS : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(LayerItemSMS);
	void setDatas(int loaithe, int menhgia, int tiennhanduoc);
	void setDataItem(int menhgia, int tiennhanduoc, string cuphap, int dauso);
	void onMenhGiaCallBack(Ref* target, SEL_CallFuncO listener);
	void onBtnMenhGiaClicked(Ref* pSender, ui::Widget::TouchEventType type);
	void onBtnNapClicked(Ref* pSender, ui::Widget::TouchEventType type);
private:
	Text* lblCuPhap;
	Text* lblDauSo;
	Text* lblQuyDoi;
	Button* btnNap;

	Text* lblMenhgia;
	Text* lblTiennhanduoc;
	Button* btnMenhGia;
	SEL_CallFuncO mCallBackListener;
	Ref *mCallBack;
};

class LayerPopupSMS : public Layer, public PlayerCallBack,
	public TableViewDataSource,
	public TableViewDelegate
{
public:
	virtual bool init();
	LayerPopupSMS();
	~LayerPopupSMS();

	virtual void onEnter() override;
	virtual void onExit() override;

	CREATE_FUNC(LayerPopupSMS);

	struct StructSMS{
		string content;
		string number;
		int money;
		int money_exchange;
	};

	struct TheSMS{
		int loaithe;
		int dauso;
		string cuphap;
		int menhgia;
		int tiennhanduoc;
	};

	vector<TheSMS> lst;
	vector<TheSMS> lstTheSMS;
	vector<string> smsString;

	Layout* pnlSelect;
	Layout* pnlListTheSms;
	Layout* pnlCuphap;
	Text* lblError;
	Text* lblNote;
	TableView* tblTheSms;

	Button* btnMobi;
	Button* btnViettel;
	Button* btnVina;
	Text* lblUserSelect;
	int current_tag;

	void onButtonMobi(Ref* pSender);
	void onButtonViettel(Ref* pSender);
	void onButtonVina(Ref* pSender);
	void onButtonBackSelectClicked(Ref* pSender);
	void onButtonBackCuphapClicked(Ref* pSender);
	void onButtonNap1Selected(Ref* pSender);
	void onButtonNap2Selected(Ref* pSender);
	void onMenhGiaClick(Ref* pSender);
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
private:
	int currTabSMS;
	enum{
		tagIndexListSMS_10k = 0,
		tagIndexListSMS_15k = 1,
		tagIndexListSMS_3k = 2
	};

	vector<StructSMS> lstSMS;
	Button* btn10K;
	Text* txt10k;

	Button* btn15K;
	Text* txt15k;

	Button* btn3K;
	Text* txt3k;

	Text* lblMoneyReceived;
	Text* lblNapnhanh;
	Text* lblDauSo;
	Text* lblCuphap;
	Text* lblPromotionSMS;
	Text* lblFastCharge;
	Text* lblNoList;

	//listSMS-p2
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
	void getAllDataSms();
	vector<string> tachChuoi(string _str);

	Button* btnUserNap;
	TextField* txtTennguoinhan;
	Button* btnNapForAnother;
	Button* btnBackSelect;
	Text* lblQuyDoi;
	Text* lblCuphapNap;
	Text* lblDauSonap;
	Text* lblNguoiNhan;
	Button* btnNap;
	Button* btnBackCuPhap;
	//end p2 variable
	Size cellSize;

	void onBtnTyGia(Ref* pSender);
	void onBtnNap10k(Ref* pSender);
	void onBtnNap15k(Ref* pSender);
	void onBtnNap3k(Ref* pSender);
	void onBtnNapTien(Ref* pSender);
	void loadAllDatas();
	void setInfo(int tag, string title);
	void gotoReconnect(float dt);
};

class LayerPopupPurcharse : public Layer
{
public:
	virtual bool init();
	LayerPopupPurcharse();
	~LayerPopupPurcharse();
	CREATE_FUNC(LayerPopupPurcharse);
private:
	void onButtonBronze(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonSilver(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonGold(Ref* pSender, ui::Widget::TouchEventType type);
	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	mIAP* iap;
#endif
};

class LayerPopupRecharge : public cocos2d::Layer,
	public PlayerCallBack
{
public:
	LayerPopupRecharge();
	~LayerPopupRecharge();

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void onBtnIAP(Ref* pSender);
	void onBtnClose(Ref* pSender);
	void onBtnSMS(Ref* pSender);
	void onBtnTyGia(Ref* pSender);
	void onBtnTheCao(Ref* pSender);
	void onBtnGiftCode(Ref* pSender);
	void onBtnHistory(Ref* pSender);
	void onBtnVIP(Ref* pSender);


	void onButtonVip(Ref* pSender);
	void onButtonMobi(Ref* pSender);
	void onButtonViettel(Ref* pSender);
	void onButtonVina(Ref* pSender);
	void onButtonOK(Ref* pSender);
	void onBtnNhapGiftCode(Ref* pSender);
	void onBtnHotLine(Ref* pSender);
    void onButtonGiftCode(Ref* pSender);
	void showLocalPayment(bool isShowInApp, bool isShowLocal);

	void gotoCard();
	void gotoSMS();

	void loadAllDatas();
	void scheduleButtonOK(float dt);

	virtual bool init();

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	//
    void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	CREATE_FUNC(LayerPopupRecharge);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	cocos2d::experimental::ui::WebView *webView;
	bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif

private:
	cocos2d::EventListener* _touchListener;
	int gameID;
	int const TAG_CHILD = 111;
	//Node tab
	Node* nodeTheCao;
	Node* nodeGiftCode;
	Node* nodeHistory;
	//tab button
	Button* btnSMS;
	Button* btnTheCao;
	Button* btnIAP;
	Button* btnVIP;
	Layout* pnlWebView;
	//ui in node SMS
	LayerPopupSMS* layerSMS;
	//ui in node IAP
	LayerPopupPurcharse *layerIAP;
	//ui in node the cao
	Text* lblPromotion;
	Text* lblErrorSeri;
	Text* lblErrorMaThe;
	Text* lblMoneyReceived;
	Text* lblPhoneHotLine;

	Button* btnMobi;
	Button* btnViettel;
	Button* btnVina;
	Button* btnGiftCode;
	Button* btnHistory;
	Button* btnTyGia;
	Button* btnHotLine;
	Button* btnOK;
	Button* btnCSVIP;
	TextField* txtMaThe;
	TextField* txtSoThe;
	TextField* txtGiftCode;
	Text* lblReportGiftCode;
	ListView* lsvHistories;

	int currTab;
	std::string strHotLine;
    bool isShowLocalGlobal;
};


#endif //__iCasino_v2__LayerPopupRecharge__

