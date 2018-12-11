//
//  LayerCaoThapPopupGame.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 11/14/16.
//
//

#ifndef LayerCaoThapPopupGame_hpp
#define LayerCaoThapPopupGame_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "ThreadQueueMsg.h"
#include "../GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class ItemKetQuaCaoThap : public cocos2d::Layer{
public:
    ItemKetQuaCaoThap();
    ~ItemKetQuaCaoThap();
    virtual bool init();
    CREATE_FUNC(ItemKetQuaCaoThap);
    void setDatas(string pCard, string pType);
private:
    ImageView* imgCard;
    Text* txtCard;

};
class LayerCaoThapPopupGame : public cocos2d::Layer,public PlayerCallBack
{
    public:
    LayerCaoThapPopupGame();
    ~LayerCaoThapPopupGame();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onBtnClose(Ref *sender);
    void onBtnSuKien(Ref *sender);
    void onBtnPlayPhienMoi(Ref *sender);
    void onBtnEndGame(Ref *sender);
    void onBtnHuongDan(Ref *sender);
    void onBtnLichSu(Ref *sender);
    void onBtnXepHang(Ref *sender);

    void onBtnMucCuoc1000(Ref *sender);
    void onBtnMucCuoc5000(Ref *sender);
    void onBtnMucCuoc10000(Ref *sender);
    
    void onBtnBetCao(Ref *sender);
    void onBtnBetThap(Ref *sender);
    
    void onBtnSet(Ref *sender);
    
    void onGameReset();
    
    void StartTimer();
    
    void PlayAnimlatBai();
    
    void CountDownTimer(float dt);
    void GetQuy(float dt);
    
    void ShowQuanBaiLat(string strCard,string strType);
    
    std::string getURL_byID_TienLen(string pid, string pType);
    
    void updateAnimLatBat(float dt);
    void StopAnimlatBai();
    void ShowLoseGame(string card,string type);
    void StopTimer();
    
    void updateEvent(float dt);
    void OnExtensionResponseProcess();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    
    void onBtnSilver(Ref *sender);
    void onBtnGold(Ref *sender);
    
    void getMinMoney();
    
    //
    void event_EXT_EVENT_GAME_STATE_NTF();
    void event_EXT_EVENT_GAME_START();
    void event_EXT_EVENT_GAME_RESULT();
    void event_EXT_EVENT_GAME_END();
    
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(LayerCaoThapPopupGame);
    private:
    EventListenerTouchOneByOne* listener;
    ThreadQueueMsg* mQueueMsg;
    
    boost::shared_ptr<ISFSObject> param;
    
    Size sizeAdd;
    
    Layout* pnlBg;
    
    //
    Text* txtPhien;
    Text* txtTongHu;
    
    Button* btnPlayPhienMoi;
    Button* btnEndGame;
    
    Button* btnMucCuoc1000;
    Button* btnMucCuoc5000;
    Button* btnMucCuoc10000;
    
    Button* btnBetCao;
    Button* btnBetThap;
    
    int currMucCuoc = 0;
    
    Sprite* sprOpenCard;
    
    Text* txtTienBetCao;
    Text* txtTienNhan;
    Text* txtTienBetThap;
    ImageView* imgshowtiencao;
    ImageView* imgshowtien;
    ImageView* imgshowtienthap;
    
    Text* txtThongBao;
    ImageView* imgStatus;
    
    Text* txtTimer;
    
    //Cheat
    TextField* txtFieldCheat;
    
    ImageView* imgbgQuanAt1;
    ImageView* imgbgQuanAt2;
    ImageView* imgbgQuanAt3;
    
    vector<ImageView*> lstA;
    
    int currTimer = 120;
    
    int currTimerAnimLatBai = 0;
    
    int TAG_RESULT_MONEY = 150;
    
    bool isGameState = false;
    
    int currMyMoney = 0;
    
    boost::shared_ptr<long> ibqt;
    boost::shared_ptr<long> gwv;
    boost::shared_ptr<string> glstc;
    
    
    //new
    
    Button* btnGold;
    Button* btnSilver;
    int currTypeMoney = 0;
    int MinBetValue = 0;

    string PATH_CT = "design/LayerGameCaoThap/";
};

#endif /* LayerPlayGameMiniPoker_hpp */
