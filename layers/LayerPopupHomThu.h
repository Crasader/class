#ifndef __iCasino_v4__LayerPopupHomThu__
#define __iCasino_v4__LayerPopupHomThu__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameServer.h"
#include "mUtils.h"
#include "AllData.h"
#include "../Common.h"
#include "../layers/LayerPopupMailDetails.h"
#include "../layers/LayerSendMessage.h"
#include "../layergames/CircleAvatar.h"
#include "../layers/LayerPopupThongBao.h"

USING_NS_CC;
using namespace cocos2d::ui;
USING_NS_CC_EXT;

class LayerPopupHomThu : public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate, public PlayerCallBack
{
public:
    struct info{
        long id;
        string fromUser;
        string toUser;
        string subject;
        string datetime;
        string content;
        bool isRead;
        
        info(){
            id = 0;
            fromUser = "";
            toUser = "";
            datetime = "";
            content = "";
            isRead = false;
        }
    };
    
    cocos2d::Color3B _colorPress = cocos2d::Color3B(175, 8, 0);
    cocos2d::Color3B _colorNormal = cocos2d::Color3B(255, 236, 192);
    
    TableView* tblInbox;
    vector<info> lstInbox;
    vector<info> lstInboxs;
    vector<info> lstSends;
    vector<info> lstSent;
    
    /*Node *LayerPopupHomThu;*/
    Node *layerItemMailInboxDetail;
    
    Text *lblMailTitle, *lblMailContent, *lblDatetime, *lblSender, *lblSenderTitle,*txtNoMail;
    int currentTab = 1;
    
    LayerPopupHomThu();
    ~LayerPopupHomThu();
    
    virtual bool init();
    void loadAllDatas();
    void onEnter() override;
    void onExit() override;
    
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    void onBtnChildBack(Ref* pSender);
    void onBtnClose(Ref* pSender);
    
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    
    CREATE_FUNC(LayerPopupHomThu)
    
};

class LayerHomThuCell : public Layer{
public:
    CC_SYNTHESIZE(int, mIDMail, IDMail);
    
    std::string MyAI;
    std::string MyAal;
    Text *lblName, *lblDate, *lblMailTitle;
    CircleAvatar* circleAvatar;
    ImageView* imgUnread;
    Button* btnDelete;
    LayerHomThuCell();
    ~LayerHomThuCell();
    
    void onButtonDelete(Ref* pSender);
    virtual bool init();
    CREATE_FUNC(LayerHomThuCell)
    void setDatas(const LayerPopupHomThu::info& data,int currTab);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
};
#endif //__iCasino_v4__LayerPopupHomThu__
