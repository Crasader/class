//
//  LayerPopupDoiAvatar.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/27/16.
//
//

#ifndef LayerPopupDoiAvatar_hpp
#define LayerPopupDoiAvatar_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../GameServer.h"
#include "Requests/ExtensionRequest.h"
#include "../layergames/CircleAvatar.h"
#include "../ImageDownloader.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class ChangeAvatarMoc;
class LayerPopupDoiAvatar: public cocos2d::Layer, public TableViewDataSource,
public TableViewDelegate,public PlayerCallBack{
public:
    LayerPopupDoiAvatar();
    ~LayerPopupDoiAvatar();
    virtual bool init();
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    void hideLayer();
    void showLayer();
    void onBtnClose(Ref* pSender);
    void changeAvatarCallBack(Ref *pSend);
    void onChangeAvatarClick(Ref* pSender, ui::Widget::TouchEventType type);


    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerPopupDoiAvatar);
private:
  
    TableView* tableListAvatar;
    vector<ChangeAvatarMoc*> arrAvatarChange;

    
};
class ChangeAvatarMoc : public Layer{
private:
    std::string url;
    SEL_CallFuncO mCallBackListener;
    Ref *mCallBack;
    Sprite* iconCheck;
    CircleAvatar* circleAvatar;
    bool isChoose;
    
public:
    ChangeAvatarMoc(const string& url);
    ~ChangeAvatarMoc();
    
    static ChangeAvatarMoc* create(const string& url);
    
    bool init();
    void setIcon(string& url);
    string& getUrl () ;
    bool getIsChoose ();
    void visibleIconCheck(bool state) ;
    void onCallBack(Ref *target, SEL_CallFuncO listener);
    void onAvatarTouches(Ref* pSender, ui::Widget::TouchEventType type);
    ImageFromUrl *gImgDownloadProcess;
};


#endif /* LayerPopupDoiAvatar_hpp */
