#ifndef __LayerDoiThuong_h__
#define __LayerDoiThuong_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../layergames/NativeHelper.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;
class LayerItemDoiThuong : public Layer
	, public PlayerCallBack{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemDoiThuong);
    void setDatas(int loaiMang,int txtMoneyReceive,int txtMoney);
    void onDoiThuongCallBack(Ref* target, SEL_CallFuncO listener);
    void onBtnDoiThuong(Ref* pSender, ui::Widget::TouchEventType type);
private:
    Sprite* sprNhaMang;
    Text* lblMoney;
    Text* lblMoneyReceive;
    Button* btnDoiThuong;
    SEL_CallFuncO mCallBackListener;
    Ref *mCallBack;
};
class LayerItemVatPham : public Layer
, public PlayerCallBack{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemVatPham);
    void setDatas(string urlVatpham,string txtName,int txtCost);
    void onDoiThuongCallBack(Ref* target, SEL_CallFuncO listener);
    void onBtnDoiThuong(Ref* pSender, ui::Widget::TouchEventType type);
private:
    ImageView* imgVatPham;
    Text* lblName;
    Text* lblCost;
    Button* btnDoiThuong;
    SEL_CallFuncO mCallBackListener;
    Ref *mCallBack;
};
class LayerDoiThuong :
public cocos2d::Layer,
public TableViewDataSource,
public TableViewDelegate,
public PlayerCallBack

{
public:
	LayerDoiThuong();
	~LayerDoiThuong();

	virtual void onEnter() override;
	virtual void onExit() override;
    void loadTheCao();
	virtual bool init();
	CREATE_FUNC(LayerDoiThuong);

	void loadAllDatas();
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    void onDoiThuongClick(Ref* pSender);
    void  popUpCallBack(Ref* pSender);
    void onXacThucOTP(Node* sender, void* data);
    void onBtnClose(Ref* pSender,ui::Widget::TouchEventType type);
    void onBtnTheCao(Ref* pSender,ui::Widget::TouchEventType type);
    void onBtnVatPham(Ref* pSender,ui::Widget::TouchEventType type);
    void onBtnLichSu(Ref* pSender,ui::Widget::TouchEventType type);
    void onBtnXacThucSoDienThoai(Ref* pSender, ui::Widget::TouchEventType type);
    void createWebView();
    void onBtnQuyDinhDoiThuong(Ref* pSender, ui::Widget::TouchEventType type);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
    struct TheCao{
        int type;
        int cost;
        int menhgia;
    };
    struct VatPham{
        int id;
        string desc;
        string img;
        int cost;
        string name;
    };
    
    vector<TheCao>listTheCaos;
    vector<VatPham>listVatPhams;
	//int gameId;

private:
	EventListenerTouchOneByOne* listener;
    Text* lblMoney;
    Button* btnTheCao;
    Button* btnVatPham;
    Button* btnLichSu;
    Node* nodeChuY;
    Text* lblChuy;
    Layout* bg;
    TableView* tblPhanThuong;
	double mTax;
    int currTab;
    int tagVatPham;
    Layout* pnlWeb;
};
#endif // __LayerDoiThuong_h__
