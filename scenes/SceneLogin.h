#ifndef __iCasino_v2__SceneLogin__
#define __iCasino_v2__SceneLogin__
#include "layers/LayerPopupThongBao.h"
#include "cocos2d.h"
#include "../Common.h"
#include "cocos-ext.h"
#include "GameServer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layers/LayerPopupForgetPassword.h"
#include "../layers/LayerPopupThongBao.h"
#include "../layergames/NativeHelper.h"
#include "network/HttpClient.h"
#include "layergames/LayerLoading.h"
#include "layers/LayerLogin.h"
#include "layergames/ChanUtils.h"
#include "../data_manager.h"
#include "../mUtils.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

//struct SocialNetworkAcc{
//	std::string LT;
//	std::string uid;
//	std::string name;
//	std::string email;
//	std::string urlAvatar;
//	std::string urlWallPicture;
//
//public:
//	SocialNetworkAcc() :
//		LT(""),
//		uid(""),
//		name(""),
//		email(""),
//		urlAvatar(""),
//		urlWallPicture("")
//	{
//	};
//	void clear(){
//		LT = "";
//		uid = "";
//		name = "";
//		email = "";
//		urlAvatar = "";
//		urlWallPicture = "";
//	}
//};

class SceneLogin : public cocos2d::Layer
	, public PlayerCallBack
{
public:
	enum {
		loginType,
		registType,
		facebookType,
		gmailType,
		quickType
	};
	SceneLogin();
	~SceneLogin();
	int currLoginType;
	static SceneLogin* instance;

	static cocos2d::Scene* createScene();

	static SceneLogin* getInstance();
	static bool isInstanceExisted();
	static void DestroyInstance();

	//SocialNetworkAcc socialInfo;
	virtual bool init();
	CREATE_FUNC(SceneLogin);

	void readInfo();
	void saveInfo();

	void gotoLogin(float dt);
	void gotoRegister();
	void gotoForgotPass();
	void doLogin(const int& type = 0);
	//LayerLogin
	void onButtonHotLineClicked(Ref* pSender);
	void onButtonLogin(Ref* pSender);
	void onButtonAutoLogin(Ref* pSender);
	void onButtonQuickPlay(Ref* pSender);
	void onButtonLoginFacebook(Ref* pSender);
	void onButtonLoginGPlus(Ref* pSender);
	void onButtonGotoCreateAcc(Ref* pSender);
	void backToLogin(Ref* pSender);
	void onButtonForgetPassword(Ref* pSender);
	void onButtonOtherClicked(Ref* pSender);

	void showFade();
	void hideFade();
    void onEnter();
    void onExit();

	//LayerCreateAcc
	void onButtonCreateAcc(Ref* pSender);

	void timeoutLoadingCallback();
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
	void showPopupThongbao();

	void scheduleUpdateEditbox(float dt);

	void scheduleUpdateContactInfo(float dt);

	void getVersionIOS(float dt);

	void hideToUp(float dt);

	void getPartnerInfo();
	void setBonusFacebook(float dt);
	void showMaintain(float dt);
	void showUpdateInfo(float dt);

	void setShouldAuto(bool isShould);
	void onLoadingFinish();

	void timeoutChatMaxtime(float dt);
	void startTimeOutChat();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	cocos2d::experimental::ui::WebView *webView;
	bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
	Button* btnLoginFace;

private:
	bool isShow;
	bool isShouldAuto;
	static bool instanceFlag;
	Layer* mCurrentNode;
	Layout* pnlLoading;
	Layout* pnlRegist;
	Layout* pnlLogin;
	ImageView* imgAutoLogin;
	Layout* pnlFade;
	ImageView* imgFade;
	ImageView* imgFadeRegister;
	Node* rootNode;
	Text* txtMoney;
	int const tagChildNode = 113;

	string partnerID;
	string srcID;

	TextField* txtUserName;
	TextField* txtPassword;

	TextField* txtRegUserName;
	TextField* txtRegPassword;
	TextField* txtReRegUserName;

	Text* lblHotLine;
	Button* btnHotLine;
	Button* btnLoginGPlus;
	Label* labelDNB;
	Sprite* logo;
	Sprite* iconPhone;
	void onPopupCallBack(Ref *pSender);

	//Button dong ý popup bảo trì
	void onButtonDongy(Ref* pSender);
	//Layer loading -- MINHDV
	LayerLoading *layerloading;

	void createLayerLoading();
protected:
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxConnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxLogin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxLogout(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxLoginError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

private:
	//update get device id for win32 - 29/11/2016
	std::string getDeviceID();
};
#endif //__iCasino_v2__SceneLogin__

