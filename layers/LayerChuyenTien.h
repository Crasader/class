#ifndef __LayerChuyenTien_h__
#define __LayerChuyenTien_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "Requests/ExtensionRequest.h"
#include "../layergames/_Chat_.h"
#include "../layers/LayerPopupThongBao.h"
#include "LayerNapHao.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;

class LayerChuyenTien : public cocos2d::Layer
	, public PlayerCallBack
{
public:
	LayerChuyenTien();
	~LayerChuyenTien();

	virtual void onEnter() override;
	virtual void onExit() override;
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	void onBtnChuyenTienClicked(Ref* pSender);
	void onBtnLichSuClicked(Ref* pSender);
	void onBtnReceiverClicked(Ref* pSender);
	void onBtnClose(Ref* pSender);
	void onBtnOKConfirm(Ref* pSender);
    void onXacThucOTP(Node* sender, void* data);
    void onBtnDoiTien(Ref* pSender);
	void loadAllDatas();

	virtual bool init();
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void onScheduleUpdateCost(float dt);
    void createWebview();

	CREATE_FUNC(LayerChuyenTien);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	cocos2d::experimental::ui::WebView *webView;
	bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
	Button *btnChuyenTien, *btnLichSu,*btnDoiTien;
    LayerNapHao* doitien;
	Text *lblMoney, *lblMoneyPhi, *lblPhi, *lblSumMoney, *txtMinTrade;
	TextField *txtAcc, *txtValue,*txtContent;
	Button *btnReceiver;
	Node *nodeContentChuyenTien;
	Text *lblErrorNick, *lblErrorValue,*lblErrorContent;
	LayerPopupThongBao *layerThongBao;
    Text* lblSilver;
    Node* bg;
    int currTab;
private:
	EventListenerTouchOneByOne* listener;
	double mTax;
};
#endif // __LayerChuyenTien_h__
