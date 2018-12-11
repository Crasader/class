#ifndef __iCasino_v2__LayerMainTop__
#define __iCasino_v2__LayerMainTop__

#include "cocos2d.h"
#include "../layers/LayerSettingMain.h"
#include "../layers/LayerSendMessage.h"
#include "cocos-ext.h"
#include "../layers/BagItem.h"
#include "../layergames/NativeHelper.h"
#include "../layers/LayerPromotion.h"
#include "../layers/LayerMenuRuongVip.h"
USING_NS_CC;
USING_NS_CC_EXT;

class LayerMainTop : public cocos2d::Layer
	, public PlayerCallBack
{
public:
	LayerMainTop();
	~LayerMainTop();

    void onScheduleThongBao(float dt);
    void onScheduleUpdateBagItem(float dt);

	void onButtonThoat(Ref* pSender);
	void onButtonQuaTang(Ref* pSender);
	void onButtonFriend(Ref* pSender);
	void onButtonRank(Ref* pSender);
	void onButtonNotify(Ref* pSender);
	void onButtonInbox(Ref* pSender);
	void onButtonSetting(Ref* pSender);
    void onButtonHotLineClicked(Ref* pSender);
	void createLabelThongBao(string _str);
    void createNotify(string _str);
    void onScheduleNotify(float dt);
    void onBtnThongBaoClick(Ref* pSender);
    void gotoRuongVip();
    void setValueMenuRuongVip(string value,string num);
	bool isCheckURL(string url);
    void onBtnGiaiDau(Ref* pSender);
    void onBtnGioVang(Ref* pSender);
    void onScheduleUpdateGioVang(float dt);
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;

    void onBtnExit(Ref* pSender);
    void onBtnSetting(Ref* pSender);
    void onBtnSuKien(Ref* pSender);
    
    void loadDatas();

    void onBtnProfile(Ref* pSender);
    void btnNapTien(Ref* pSender);
    void btnDoiThuong(Ref* pSender);
    void onBtnBack(Ref* pSender);
    void onButtonBack(Ref* pSender);
    void onButtonGif(Ref* pSender);
    Sprite* imgLogo;
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
	virtual bool init()override;
    
	CREATE_FUNC(LayerMainTop);

	cocos2d::ui::Button *btnSetting;
	Layout* pnlThongBao;
    Button* btnThoat;
    Sprite* imgPopMenu;
    Node* node;
    Button* btnBack;

    int currentParent = 1;//0 là main,1 là scene pickroom
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
    virtual void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)override;
private:
    LayerMenuRuongVip* menuRuong;
	Label* lblThongBao;
	BagItem* bagItem;
    Node* rootNode;
    BagItem* bagItem4Promotion;
    Button* btnThongBao;
    Button* btnGioVang;
	int currIndexNotification;
    string txtThongBao;
    Button* btnQuayThuong;
    bool isShowStatus;
	int numEvent;
    Text* _txtThongBao;
    ImageView* imgThongBao;
    Text* txtQuaTang;
    ImageView* imgThuMoi;
    Text* txtThuMoi;
    ImageView* imgSuKien;
    Text* txtSuKien;
    vector<string> stringNotify;
    RichText *richText = NULL;
    bool isSet = false;

};
#endif //__iCasino_v2__LayerMainTop__

