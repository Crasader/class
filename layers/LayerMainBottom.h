#ifndef __iCasino_v2__LayerMainBottom__
#define __iCasino_v2__LayerMainBottom__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

#include "GameServer.h"
#include "../layergames/CircleAvatar.h"
#include "../layergames/LayerMenuMiniGame.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class LayerMainBottom : public Layer
	, public PlayerCallBack{
public:
	LayerMainBottom();
	~LayerMainBottom();

	void onButtonCharge(Ref* pSender);
	void onButtonDoiThuong(Ref* pSender);
	void onButtonChuyenTien(Ref* pSender);
	void onButtonUserInfo(Ref* pSender);
	void onGotoBaoDanh(Ref* pSender);
    void onButtonUngTien(Ref* pSender);
        void onBtnNapGold(Ref* pSender);
        void onBtnNapSilver(Ref* pSender);
	void onButtonAvatar();
        void onButtonMiniGame(Ref* pSender);
	void loadAllDatas();
        void reloadLang();
        void onScheduleThongBao(float dt);
        void onButtonSuKien(Ref* pSender);
        void onScheduleTX(float dt);
        void hideTXMenu();
        void showTXMenu();
        void delayTXMenu(float dt);
        void logOut();
        virtual bool init();
        bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
        virtual void onEnter() override;
        virtual void onExit() override;
	CREATE_FUNC(LayerMainBottom);
	int currScene = 0;
protected:
	//Server
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	void scheduleUpdateInfo(float dt);

private:
        Button* btnSuKien;
	Text* txtName;
	Text* txtLevel;
	Text* txtGold;
    Text* txtSilver;
        Text* txtVip;
	Sprite* spriteAvatar;
	ImageView* avtVip, *imgDoiThuong;
	LoadingBar* prgLevelProgress;
	Button* btnDoiThuong;
	Button* btnBaoDanh;
	Button* btnBaoDanhStatus;
	CircleAvatar* circleAvatar;
        Sprite* sprCoin;
    Button* btnUngTien;
        Button* btnPoker;
        Button* btnTaiXiu;
        Button* btnSapNgua;
        Button* btnCaoThap;
        Button* btnVongQuay;
        Animate* animN ;
	//so luot quay set cho quay thuong
	int luotquay = 0;
        Text* txtTimeTX;
        int timerTX = 60;
        Text* txtTX;
        Text* txtPK;
        Text* txtDD;
        Text* txtVQ;
        Text* txtCT;
        LayerMenuMiniGame* menu;
        
        Text* txtMoney;
        Text* lblMoneyVirtual;
};
#endif //__iCasino_v2__LayerMainBottom__
