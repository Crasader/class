#include "LayerGameBauTom.h"
#include "ui/UIHelper.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Requests/LeaveRoomRequest.h"
#include "SceneManager.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
#include "../layergames/PhomMessDef.h"
#include "../layergames/ClientMsgDefs.h"
#include "../layers/LayerPopupThongBao.h"
#include "../layergames/CoinXocDia.h"
#include "../layergames/TaiXiuMessageDefs.h"
#include "../cotuong/gameUtils.h"
#include "../layers/LayerPopupInfo.h"
#include "cotuong/MyAnimation.h"
#include "../layers/LayerGuideInGame.h"
#include "Requests/LeaveRoomRequest.h"
#include "Requests/JoinRoomRequest.h"
#include "../scenes/SceneMain.h"
#define EXT_EVENT_LIST_WIN_NTF "lwntf"
#define EXT_FIELD_GAME_WIN_LIST "wl"
#define EXT_EVENT_RESET_BET_REG  "rbrq"
#define EXT_EVENT_RESET_BET_RES  "rbrs"
#define EXT_FIELD_GAME_BOSS_UID  "buid"
#define EXT_EVENT_RESET_BET_NTF  "rbntf"
#define TAG_OFFER 3737
LayerGameBauTom::LayerGameBauTom()
{
    this->bossName = "";
    this->bossNext = "";
    this->longpresstime = 0.5;
    this->isLoadedAvatar = true;
    this->mThreadQueueMsg = NULL;
}

LayerGameBauTom::~LayerGameBauTom()
{
    if (mThreadQueueMsg != NULL) delete mThreadQueueMsg;
    GameServer::getSingleton().removeListeners(this);
    //this->pThread1->stopEvent();
    this->unschedule(schedule_selector(LayerGameBauTom::updateEvent));

}

bool LayerGameBauTom::init()
{
    if (!Layer::init())
        return false;
    Size sizeScreen = Director::getInstance()->getVisibleSize();
    string filename = "LayerMainBauTom.csb";
    if (dataManager.getAppConfig().isChip){
        filename = "LayerMainBauTom2.csb";
    }
        
    auto rootNode = CSLoader::getInstance()->createNode(filename);
    rootNode->setContentSize(sizeScreen);
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    this->currTypeMoney = SceneManager::getSingleton().getCurrRoomType();
    
    this->pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
    this->initAllVariables();
    this->initButton();
    this->initButtonBet();
    this->initPanelHis();
    this->initPanelUser();
    this->initAllListAvatar();
    this->initPanelChat();
    this->initTable();
    //this->initBetString();
    this->resetGame();
    
    this->mThreadQueueMsg = new ThreadQueueMsg();
    this->mThreadQueueMsg->setFunCallBack(this, callfunc_selector(LayerGameBauTom::OnExtensionResponseProcess));
//    this->pThread1 = new ThreadFunction();
//    this->pThread1->runEvent(callfunc_selector(LayerGameBauTom::updateEvent), this);
    this->schedule(schedule_selector(LayerGameBauTom::updateEvent));
    
    if (dataManager.getAppConfig().isChip){
        this->lstImg = {"ResBautom/500-chip.png","ResBautom/1K-chip.png","ResBautom/5K-chip.png","ResBautom/10K-chip.png","ResBautom/50K-chip.png","ResBautom/100K-chip.png","ResBautom/500K-chip.png","ResBautom/1M-chip.png"};
}else{
     this->lstImg = {"ResBautom/500.png","ResBautom/1k.png","ResBautom/5k.png","ResBautom/10k.png","ResBautom/50k.png","ResBautom/100k.png","ResBautom/500k.png","ResBautom/1000k.png"};
}
//
    
    this->currTypeMoney = SceneManager::getSingleton().getCurrRoomType();
    GameServer::getSingleton().addListeners(this);
    SceneManager::getSingleton().showLoading();
    return true;
}
void LayerGameBauTom::animationBetOther(int money,string bettype,int showtype){
    Vec2 orginPos;
    int type = 1;
    Vec2 beginPos;
    vector<string> listUser = mUtils::splitString(this->listUserPlay, ';');
    int max = (int)listUser.size()-1;
    int random = 0;
    if (listUser.size()>=5){
        random = rand()%4;
    }else if (listUser.size() > 1 && listUser.size() < 5){
        random = rand()% max;
    }
    if (max ==0){
        random = 0;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (random == 4){
        beginPos = Vec2(WIDTH_DESIGN -27, visibleSize.height-280);
    }else{
        if (random < this->listAvatar.size())
            beginPos = this->listAvatar[random]->getPosition();
    }
    if (bettype == "TOM"){
        orginPos = this->btnTom->getPosition();
        type = 1;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
    }
    if (bettype == "CUA"){
        orginPos = this->btnCua->getPosition();
        type = 2;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
    }
    if (bettype == "CA"){
        orginPos = this->btnCa->getPosition();
        type = 3;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
    }
    if (bettype == "GA"){
        orginPos = this->btnGa->getPosition();
        type = 4;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
    }
    if (bettype == "BAU"){
        orginPos = this->btnBau->getPosition();
        type = 5;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
    }
    if (bettype == "HUOU"){
        orginPos = this->btnHo->getPosition();
        type = 6;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(beginPos);
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
    }
}
void LayerGameBauTom::animationBetWhenReturn(int money,string bettype,int showtype){
    Vec2 orginPos;
    int type = 1;
    if (bettype == "TOM"){
        orginPos = this->btnTom->getPosition();
        type = 1;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
    }
    if (bettype == "CUA"){
        orginPos = this->btnCua->getPosition();
        type = 2;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            a->setRotation(randomRotate);
            
            a->setPosition(Vec2(randX,randY));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
    }
    if (bettype == "CA"){
        orginPos = this->btnCa->getPosition();
        type = 3;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            a->setRotation(randomRotate);
            
            a->setPosition(Vec2(randX,randY));
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
    }
    if (bettype == "GA"){
        orginPos = this->btnGa->getPosition();
        type = 4;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
    }
    if (bettype == "BAU"){
        orginPos = this->btnBau->getPosition();
        type = 5;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
    }
    if (bettype == "HUOU"){
        orginPos = this->btnHo->getPosition();
        type = 6;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
      
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(Vec2(randX,randY));
            a->setRotation(randomRotate);
            
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            a->setRotation(randomRotate);
            
            a->setPosition(Vec2(randX,randY));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
    }
    
}
void LayerGameBauTom::rollbackBeInt(int money,string bettype){
    int type = 1;
    vector<Sprite*> curList;
    if (bettype == "TOM"){
        type = 1;
        curList = this->listImgTom;
    }
    if (bettype == "CUA"){
        type = 2;
        curList = this->listImgCua;
        
    }
    if (bettype == "CA"){
        type = 3;
        curList = this->listImgCa;
        
    }
    if (bettype == "GA"){
        type = 4;
        curList = this->listImgGa;
        
    }
    if (bettype == "BAU"){
        type = 5;
        curList = this->listImgBau;
        
    }
    if (bettype == "HUOU"){
        type = 6;
        curList = this->listImgHo;
        
    }
    
    for (int i = 0 ; i < curList.size(); i++){
        if (curList[i]->getTag() == (money+type) && curList[i]->isVisible() && curList[i]->getPositionX() > this->nodeAvatarPlayer->getPositionX()){
            this->currRolBackSprite = curList[i];
            this->runAction(Sequence::create(MoveTo::create(0.2, this->getPosition()),CallFunc::create([=]{this->currRolBackSprite->setVisible(false);}), NULL));
        }
    }
    
}
void LayerGameBauTom::animationBet(int money,string bettype,int showtype){
    Vec2 orginPos;
    int type = 1;
    if (bettype == "TOM"){
        orginPos = this->btnTom->getPosition();
        type = 1;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(this->btnBet1000k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(this->btnBet500k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(this->btnBet100k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(this->btnBet50k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(this->btnBet10k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(this->btnBet5k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(this->btnBet1k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgTom.push_back(a);
            if (showtype == 1){
                this->listImgMeTom.push_back(a);
            }
        }
    }
    if (bettype == "CUA"){
        orginPos = this->btnCua->getPosition();
        type = 2;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(this->btnBet1000k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(this->btnBet500k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(this->btnBet100k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(this->btnBet50k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(this->btnBet10k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(this->btnBet5k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(this->btnBet1k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCua.push_back(a);
            if (showtype == 1){
                this->listImgMeCua.push_back(a);
            }
        }
    }
    if (bettype == "CA"){
        orginPos = this->btnCa->getPosition();
        type = 3;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(this->btnBet1000k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(this->btnBet500k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(this->btnBet100k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(this->btnBet50k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(this->btnBet10k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(this->btnBet5k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(this->btnBet1k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgCa.push_back(a);
            if (showtype == 1){
                this->listImgMeCa.push_back(a);
            }
        }
    }
    if (bettype == "GA"){
        orginPos = this->btnGa->getPosition();
        type = 4;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(this->btnBet1000k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(this->btnBet500k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(this->btnBet100k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(this->btnBet50k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(this->btnBet10k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(this->btnBet5k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(this->btnBet1k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgGa.push_back(a);
            if (showtype == 1){
                this->listImgMeGa.push_back(a);
            }
        }
    }
    if (bettype == "BAU"){
        orginPos = this->btnBau->getPosition();
        type = 5;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(this->btnBet1000k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(this->btnBet500k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(this->btnBet100k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(this->btnBet50k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(this->btnBet10k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(this->btnBet5k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(this->btnBet1k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgBau.push_back(a);
            if (showtype == 1){
                this->listImgMeBau.push_back(a);
            }
        }
    }
    if (bettype == "HUOU"){
        orginPos = this->btnHo->getPosition();
        type = 6;
        Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
        int randX = startPos.x +rand() % 100;
        int randY = startPos.y +rand() % 30;
        
        int million = money/1000000;
        
        for (int i = 0; i < million;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[7]);
            a->setLocalZOrder(101);
            a->setTag(1000000+type);
            a->setPosition(this->btnBet1000k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=1000000*million;
        
        int _500k = money/500000;
        for (int i = 0; i < _500k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[6]);
            a->setLocalZOrder(101);
            a->setTag(500000+type);
            a->setPosition(this->btnBet500k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=500000*_500k;
        
        int _100k = money/100000;
        for (int i = 0; i < _100k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[5]);
            a->setLocalZOrder(101);
            a->setTag(100000+type);
            a->setPosition(this->btnBet100k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=100000*_100k;
        
        int _50k = money/50000;
        for (int i = 0; i < _50k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[4]);
            a->setLocalZOrder(101);
            a->setTag(50000+type);
            
            a->setPosition(this->btnBet50k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=50000*_50k;
        
        int _10k = money/10000;
        for (int i = 0; i < _10k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[3]);
            a->setLocalZOrder(101);
            a->setTag(10000+type);
            
            a->setPosition(this->btnBet10k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=10000*_10k;
        
        int _5k = money/5000;
        for (int i = 0; i < _5k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[2]);
            a->setLocalZOrder(101);
            a->setTag(5000+type);
            
            a->setPosition(this->btnBet5k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=5000*_5k;
        
        int _1k = money/1000;
        for (int i = 0; i < _1k;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[1]);
            a->setLocalZOrder(101);
            a->setTag(1000+type);
            
            a->setPosition(this->btnBet1k->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=1000*_1k;
        
        int _500 = money/500;
        for (int i = 0; i < _500;i++){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(1*i),RotateTo::create(0.01, randomRotate, randomRotate),MoveTo::create(0.2, Vec2(randX,randY)), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
        money-=500*_500;
        
        if (money>0){
            int randomRotate = rand() % 360;
            
            Sprite* a = Sprite::create(this->lstImg[0]);
            a->setLocalZOrder(101);
            a->setTag(500+type);
            
            a->setPosition(this->btnBet500->getPosition());
            a->runAction(Sequence::create(DelayTime::create(0),MoveTo::create(0.3, Vec2(randX,randY)),RotateTo::create(0.01, randomRotate, randomRotate), NULL));
            this->pnlMain->addChild(a);
            this->listImgMoney.push_back(a);
            this->listImgHo.push_back(a);
            if (showtype == 1){
                this->listImgMeHo.push_back(a);
            }
        }
    }
}
void LayerGameBauTom::onEnter()
{
    Layer::onEnter();
}

void LayerGameBauTom::onExit()
{
    Layer::onExit();
    this->unschedule(schedule_selector(LayerGameBauTom::updateTimeMoQuy));
    this->stopAllActions();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

void LayerGameBauTom::initButton()
{
    Button* btnQuit = (Button*)this->pnlMain->getChildByName("btnQuit");
    Button* btnHis = (Button*)this->pnlMain->getChildByName("btnHis");
    Button* btnChat = (Button*)this->pnlMain->getChildByName("btnChat");
    Button* btnDsCai = (Button*)this->pnlMain->getChildByName("btnDsCai");
    this->btnCamera = (Button*)this->pnlMain->getChildByName("btnCamera");
    this->btnCuopCai = (Button*)this->pnlMain->getChildByName("btnCuopCai");
    Button* btnNhacai = (Button*)this->pnlMain->getChildByName("btnNhacai");
    this->btnCuopCai->setEnabled(false);
    Button* btnCoin = (Button*)this->pnlMain->getChildByName("btnCoin");
    
    auto lblRoom_0 = static_cast<Text*>(this->pnlMain->getChildByName("lblRoom_0"));
    lblRoom_0->setString(dataManager.GetSysString(43));
    
    auto lblRoom_0_1 = static_cast<Text*>(this->pnlMain->getChildByName("lblRoom_0_1"));
    lblRoom_0_1->setString(dataManager.GetSysString(42));
    
    auto lblRoom_0_0 = static_cast<Text*>(this->pnlMain->getChildByName("lblRoom_0_0"));
    lblRoom_0_0->setString(dataManager.GetSysString(44));
    
    auto lblRoom_0_0_0 = static_cast<Text*>(this->pnlMain->getChildByName("lblRoom_0_0_0"));
    lblRoom_0_0_0->setString(dataManager.GetSysString(45));
    
    this->btnCamera->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonCamera, this));
    btnQuit->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonQuit, this));
    btnHis->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonHis, this));
    this->btnCuopCai->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonCuopCai, this));
    btnChat->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonChat, this));
    btnDsCai->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonDsCai, this));
    btnNhacai->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonDsCai, this));
    btnCoin->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonCoin, this));
    if (dataManager.getAppConfig().is24h) {
        btnChat->setVisible(false);
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    //    auto imgNhaCai = Sprite::create("ResXD/nha-cai-lamcai.png");
    //    imgNhaCai->setAnchorPoint(Vec2(0.5,1));
    //    imgNhaCai->setPosition(Vec2(WIDTH_DESIGN /2, visibleSize.height));
    //    imgNhaCai->setTexture(StringUtils::format("design/ResXD/%snha-cai-lamcai.png",SceneManager::getSingleton().getFolderLang().c_str()));
    //    this->addChild(imgNhaCai);
    
    auto userIcon = Sprite::create("ResXD/use-icon.png");
    userIcon->setAnchorPoint(Vec2(1,1));
    userIcon->setPosition(Vec2(WIDTH_DESIGN -27, visibleSize.height-280));
    this->pnlMain->addChild(userIcon);
    
    this->lblUserCount = Label::createWithTTF("", "__Roboto-Bold.ttf", 45);
    this->pnlMain->addChild(this->lblUserCount);
    this->lblUserCount->setAnchorPoint(Vec2(0.5,1));
    this->lblUserCount->setPosition(Vec2(userIcon->getPositionX() - userIcon->getContentSize().width/2, visibleSize.height-385));
    
    
    auto btnXepHang = Button::create();
    btnXepHang->loadTextureNormal("design/ResBautom/moc3-popup-menu-rank.png");
    btnXepHang->loadTexturePressed("design/ResBautom/moc3-popup-menu-rank.png");
    
    this->pnlMain->addChild(btnXepHang);
    btnXepHang->setAnchorPoint(Vec2(1,1));
    
    btnXepHang->setPosition(Vec2(WIDTH_DESIGN -27, visibleSize.height-150));
    btnXepHang->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onBtnXepHang, this));
    this->chat = LayerChatGame::create();
    this->pnlMain->addChild(this->chat,2000);
    this->chat->setPosition(Vec2(1100, 0));
    
    Button* btnHuongDan = static_cast<Button*>(this->pnlMain->getChildByName("btnHuongDan"));
    if (btnHuongDan) {
        btnHuongDan->addTouchEventListener(CC_CALLBACK_2(LayerGameBauTom::onBtnHuongDan, this));
        btnHuongDan->setPressedActionEnabled(true);
    }
    this->txtTimerHuThuong = static_cast<Text*>(this->pnlMain->getChildByName("txtTimer"));
    this->txtTimerHuThuong->setString("---");
    this->txtHuThuong = static_cast<Text*>(this->pnlMain->getChildByName("txtHuThuong"));
    this->txtHuThuong->setString("---");
    
    Button* btnHuThuong = (Button*)this->pnlMain->getChildByName("btnHuThuong");
    btnHuThuong->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onBtnHuThuong, this));
    
    this->btnChuyenDongTien = (Button*)this->pnlMain->getChildByName("btnChuyenDongTien");
    this->btnChuyenDongTien->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onBtnChuyenDongTien, this));
    if (this->currTypeMoney ==0 ){
        this->btnChuyenDongTien->loadTextures("ResBautom/dongmoc-min.png", "ResBautom/dongmoc-min.png");
        
    }else{
        this->btnChuyenDongTien->loadTextures("ResBautom/donghao-min.png", "ResBautom/donghao-min.png");
    }
//    this->txtCheat = (TextField*)this->pnlMain->getChildByName("txtCheat");
//    Button* btnSet = static_cast<Button*>(this->pnlMain->getChildByName("btnSet"));
//    if (btnSet) {
//        btnSet->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onBtnCheat, this));
//    }
}
void LayerGameBauTom::onBtnCheat(Ref* btnSet){
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL || this->txtCheat->getString() == "")
        return;
    //send smartfox sever msg
    boost::shared_ptr<SFSObject> params(new SFSObject());
    params->PutUtfString("rg", this->txtCheat->getString());
    boost::shared_ptr<IRequest> req2(new ExtensionRequest("stv", params, lastRoom));
    GameServer::getSingleton().Send(req2);
}
void LayerGameBauTom::onBtnChuyenDongTien(Ref* sender){
    this->isJoin = false;
   // log("chuyen dong tien");
    
    this->setStateButtonBet(-1);
    this->currBetvalue = 0;
    
    boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (NULL == lastRoom)
    {
        //SceneManager::getSingleton().gotoPickGame(this->gameID);
        SceneManager::getSingleton().gotoMain();
        
    }
    else
    {
        this->resetGame();
        SceneManager::getSingleton().setIsChuyenTab(true);
        boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
        GameServer::getSingleton().Send(request);
        
    }
    SceneManager::getSingleton().showLoading();
    
}
void LayerGameBauTom::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    // Get the user parameter of the event
    
    boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    // Get the room parameter of the event
    boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
    boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);
    //
    if (ptrNotifiedUser == NULL || ptrNotifiedRoom == NULL) return;
    
    if (ptrNotifiedRoom->GroupId()->compare(SERVICE_CHAT_ROOM) == 0
        || ptrNotifiedRoom->Id() == 118 || ptrNotifiedRoom->GroupId()->compare("200") == 0
        || ptrNotifiedRoom->Id() == 200)
        return;
    //if it is me
    if (!ptrNotifiedUser->IsItMe()){
        log ("return do khong phai minh");
        return;
    }
    
    log ("return do joinj roi");

    if (this->isJoin) return;
    log ("return do currTypeMoney = -1");

    if (this->currTypeMoney == -1) return;
    log ("return do currTypeMoney = khong phai bau tom");

    if (SceneManager::getSingleton().getGameID() != kGameBauTom) return;
    log ("return do currTypeMoney = khong phai bau tom");

    if (this->currTypeMoney == 0){
        boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(kGameBauTom));
        for (int i = 0; i < list_room->size(); i++)
        {
            boost::shared_ptr<Room> room = list_room->at(i);
            if (room != NULL)
            {
                if (room->GetVariable("roomType") != NULL){
                    int roomType = (int)*room->GetVariable("roomType")->GetIntValue();
                    
                    if (roomType == 1){
                        this->currTypeMoney = 1;
                        SceneManager::getSingleton().setCurrRoomType(this->currTypeMoney);
                        boost::shared_ptr<long> lastRoomID(new long(-1));
                        boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
                        if (lastRoom)
                            lastRoomID = boost::shared_ptr<long>(new long(lastRoom->Id()));
                       // log("Room Tai Xiu != NULL");
                        boost::shared_ptr<IRequest> request(new JoinRoomRequest(room, "", lastRoomID, false));
                        GameServer::getSingleton().Send(request);
                    }
                }
            }
        }
    }else{
        boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(kGameBauTom));
        for (int i = 0; i < list_room->size(); i++)
        {
            boost::shared_ptr<Room> room = list_room->at(i);
            if (room != NULL)
            {
                if (room->GetVariable("roomType") != NULL){
                    int roomType = (int)*room->GetVariable("roomType")->GetIntValue();
                    if (roomType == 0){
                        this->currTypeMoney = 0;
                        SceneManager::getSingleton().setCurrRoomType(this->currTypeMoney);
                        
                        boost::shared_ptr<long> lastRoomID(new long(-1));
                        boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
                        if (lastRoom)
                            lastRoomID = boost::shared_ptr<long>(new long(lastRoom->Id()));
                       // log("Room Tai Xiu != NULL");
                        boost::shared_ptr<IRequest> request(new JoinRoomRequest(room, "", lastRoomID, false));
                        GameServer::getSingleton().Send(request);
                    }
                }
            }
        }
    }
}
void LayerGameBauTom::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
   // log("join con me no room rồi");
    if (this->chat != NULL)
        this->chat->loadAllDatas();

    if (this->currTypeMoney ==0 ){
        this->btnChuyenDongTien->loadTextures("ResBautom/dongmoc-min.png", "ResBautom/dongmoc-min.png");
        
    }else{
        this->btnChuyenDongTien->loadTextures("ResBautom/donghao-min.png", "ResBautom/donghao-min.png");
    }
    this->updateInfoMe();
}
void LayerGameBauTom::onBtnHuThuong(Ref* pSender){
    this->removeChildByTag(2390);
    LayerPopupNoHuBauTom* xh = LayerPopupNoHuBauTom::create();
    xh->setTimeNoHu(this->timeNoHu);
    xh->setTag(2390);
    this->addChild(xh,700000);
    
}
void LayerGameBauTom::onBtnHuongDan(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        LayerGuideInGame* layer = LayerGuideInGame::create();
        string url = GUIDE_LINK;
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
            url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+GUIDE_P2_LINK;
        layer->setLink(StringUtils::format("%s%d", url.c_str(), kGameBauTom));
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(layer, 1000);
        
        layer->setTag(1504);
    }
}
void LayerGameBauTom::onBtnXepHang(Ref* pSender){
    LayerBangXepHangTo* xh = LayerBangXepHangTo::create();
    xh->loadTopByType();
    this->addChild(xh,700000);
    
}
void LayerGameBauTom::SelectCoinValue(long money)
{
    
    int type = -1;
    double per = money/20;
    if (per >=25  && per < 2000)
    {
        type = 0;
    }
    
    if (per >=25  && per < 2000)
    {
        type =  0;
    }
    else if (per >= 2000 && per < 5000){
        type =  1;
    }
    else if (per >= 5000 && per < 10000) {
        type =  2;
    }
    
    //10k
    else if (per >= 10000 && per < 50000) {
        type =  3;
    }
    
    //50k
    else if (per >= 50000 && per < 100000) {
        type =  4;
    }
    
    else if (per >= 100000 && per < 500000) {
        type =  5;
    }
    
    else if (per >= 500000 && per < 1000000) {
        type =  6;
    }
    
    else if (per >= 1000000 && per < 5000000) {
        type =  7;
    }
    else if (per >= 5000000){
        type =  7;
    }
    
    if (type < 0) return;
    this->currBetvalue = this->listBetvalue[type];
    this->setStateButtonBet(type+77);
    
}
void LayerGameBauTom::initButtonBet()
{
    
    this->initBetString();
    
    this->btnBet500 = (Button*)this->pnlMain->getChildByName("btnBet1");
    this->spActive500 = (Sprite*)this->btnBet500->getChildByName("sprAct");
    this->btnBet1k = (Button*)this->pnlMain->getChildByName("btnBet2");
    this->spActive1k = (Sprite*)this->btnBet1k->getChildByName("sprAct");
    this->btnBet5k = (Button*)this->pnlMain->getChildByName("btnBet3");
    this->spActive5k = (Sprite*)this->btnBet5k->getChildByName("sprAct");
    this->btnBet10k = (Button*)this->pnlMain->getChildByName("btnBet4");
    this->spActive10k = (Sprite*)this->btnBet10k->getChildByName("sprAct");
    this->btnBet50k = (Button*)this->pnlMain->getChildByName("btnBet5");
    this->spActive50k = (Sprite*)this->btnBet50k->getChildByName("sprAct");
    this->btnBet100k = (Button*)this->pnlMain->getChildByName("btnBet6");
    this->spActive100k = (Sprite*)this->btnBet100k->getChildByName("sprAct");
    this->btnBet500k = (Button*)this->pnlMain->getChildByName("btnBet7");
    this->spActive500k = (Sprite*)this->btnBet500k->getChildByName("sprAct");
    this->btnBet1000k = (Button*)this->pnlMain->getChildByName("btnBet8");
    this->spActive1000k = (Sprite*)this->btnBet1000k->getChildByName("sprAct");
    
    
    this->spActive1000k->setVisible(false);
    this->spActive500k->setVisible(false);
    this->spActive100k->setVisible(false);
    this->spActive50k->setVisible(false);
    this->spActive10k->setVisible(false);
    this->spActive5k->setVisible(false);
    this->spActive1k->setVisible(false);
    this->spActive500->setVisible(false);
    
    this->btnBet500->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonBet, this));
    this->btnBet1k->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonBet, this));
    this->btnBet5k->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonBet, this));
    this->btnBet10k->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonBet, this));
    this->btnBet50k->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonBet, this));
    this->btnBet100k->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonBet, this));
    this->btnBet500k->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonBet, this));
    this->btnBet1000k->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonBet, this));
    //////////////////////////////////////////////////////////////////////////
    
    
    this->btnTom = (Button*)this->pnlMain->getChildByName("btnTom");
    this->spActiveTom = (Sprite*)this->btnTom->getChildByName("Sprite_14");
    this->spX2Tom = (Sprite*)this->pnlMain->getChildByName("sprx2Tom");
    this->spX3Tom = (Sprite*)this->pnlMain->getChildByName("sprx3Tom");
    this->spX2Tom->setLocalZOrder(102);
    this->spX3Tom->setLocalZOrder(102);
    
    
    this->btnCua = (Button*)this->pnlMain->getChildByName("btnCua");
    this->spActiveCua = (Sprite*)this->btnCua->getChildByName("Sprite_15");
    this->spX2Cua = (Sprite*)this->pnlMain->getChildByName("sprx2Cua");
    this->spX3Cua = (Sprite*)this->pnlMain->getChildByName("sprx3Cua");
    this->spX2Cua->setLocalZOrder(102);
    this->spX3Cua->setLocalZOrder(102);
    
    this->btnCa = (Button*)this->pnlMain->getChildByName("btnCa");
    this->spActiveCa = (Sprite*)this->btnCa->getChildByName("Sprite_16");
    this->spX2Ca = (Sprite*)this->pnlMain->getChildByName("sprx2Ca");
    this->spX3Ca = (Sprite*)this->pnlMain->getChildByName("sprx3Ca");
    this->spX2Ca->setLocalZOrder(102);
    this->spX3Ca->setLocalZOrder(102);
    
    this->btnGa = (Button*)this->pnlMain->getChildByName("btnGa");
    this->spActiveGa = (Sprite*)this->btnGa->getChildByName("Sprite_17");
    this->spX2Ga = (Sprite*)this->pnlMain->getChildByName("sprx2Ga");
    this->spX3Ga = (Sprite*)this->pnlMain->getChildByName("sprx3Ga");
    this->spX2Ga->setLocalZOrder(102);
    this->spX3Ga->setLocalZOrder(102);
    
    this->btnHo = (Button*)this->pnlMain->getChildByName("btnHo");
    this->spActiveHo = (Sprite*)this->btnHo->getChildByName("Sprite_19");
    this->spX2Ho = (Sprite*)this->pnlMain->getChildByName("sprx2Ho");
    this->spX3Ho = (Sprite*)this->pnlMain->getChildByName("sprx3Ho");
    this->spX2Ho->setLocalZOrder(102);
    this->spX3Ho->setLocalZOrder(102);
    
    this->btnBau = (Button*)this->pnlMain->getChildByName("btnBau");
    this->spActiveBau = (Sprite*)this->btnBau->getChildByName("Sprite_18");
    this->spX2Bau = (Sprite*)this->pnlMain->getChildByName("sprx2Bau");
    this->spX3Bau = (Sprite*)this->pnlMain->getChildByName("sprx3Bau");
    this->spX2Bau->setLocalZOrder(102);
    this->spX3Bau->setLocalZOrder(102);
    
    
    
    this->imgMoneyTom = (ImageView*)this->pnlMain->getChildByName("imgMoneyTom");
    
    this->imgMoneyCa = (ImageView*)this->pnlMain->getChildByName("imgMoneyCa");
    this->imgMoneyBau = (ImageView*)this->pnlMain->getChildByName("imgMoneyBau");
    this->imgMoneyCua = (ImageView*)this->pnlMain->getChildByName("imgMoneyCua");
    this->imgMoneyGa = (ImageView*)this->pnlMain->getChildByName("imgMoneyGa");
    this->imgMoneyHo = (ImageView*)this->pnlMain->getChildByName("imgMoneyHo");
    
    this->imgMoneyTom->setLocalZOrder(102);
    this->imgMoneyBau->setLocalZOrder(102);
    this->imgMoneyCua->setLocalZOrder(102);
    this->imgMoneyGa->setLocalZOrder(102);
    this->imgMoneyHo->setLocalZOrder(102);
    this->imgMoneyCa->setLocalZOrder(102);
    
    this->lblTom = (Text*)this->imgMoneyTom->getChildByName("lblMeTom");
    this->lblTotalTom = (Text*)this->imgMoneyTom->getChildByName("lblAllTom");
    
    this->lblCua = (Text*)this->imgMoneyCua->getChildByName("lblMeCua");
    this->lblTotalCua = (Text*)this->imgMoneyCua->getChildByName("lblAllCua");
    
    this->lblCa = (Text*)this->imgMoneyCa->getChildByName("lblMeCa");
    this->lblTotalCa = (Text*)this->imgMoneyCa->getChildByName("lblAllCa");
    
    this->lblGa = (Text*)this->imgMoneyGa->getChildByName("lblMeGa");
    this->lblTotalGa = (Text*)this->imgMoneyGa->getChildByName("lblAllGa");
    
    this->lblBau = (Text*)this->imgMoneyBau->getChildByName("lblMeBau");
    this->lblTotalBau = (Text*)this->imgMoneyBau->getChildByName("lblAllBau");
    
    this->lblHo = (Text*)this->imgMoneyHo->getChildByName("lblMeHo");
    this->lblTotalHo = (Text*)this->imgMoneyHo->getChildByName("lblAllHo");
    
    this->btnTom->addTouchEventListener(CC_CALLBACK_2(LayerGameBauTom::onButtonBauTom, this));
    this->btnCua->addTouchEventListener(CC_CALLBACK_2(LayerGameBauTom::onButtonBauTom, this));
    this->btnCa->addTouchEventListener(CC_CALLBACK_2(LayerGameBauTom::onButtonBauTom, this));
    this->btnGa->addTouchEventListener(CC_CALLBACK_2(LayerGameBauTom::onButtonBauTom, this));
    this->btnBau->addTouchEventListener(CC_CALLBACK_2(LayerGameBauTom::onButtonBauTom, this));
    this->btnHo->addTouchEventListener(CC_CALLBACK_2(LayerGameBauTom::onButtonBauTom, this));
    
    
    //    this->lblTom->setColor(Color3B::BLACK);
    //    this->lblTotalTom->setColor(Color3B::BLACK);
    //    this->lblCa->setColor(Color3B::BLACK);
    //    this->lblCa->setColor(Color3B::BLACK);
    //    this->lblCua->setColor(Color3B::BLACK);
    //    this->lblTotalCua->setColor(Color3B::BLACK);
    //    this->lblBau->setColor(Color3B::BLACK);
    //    this->lblTotalBau->setColor(Color3B::BLACK);
    //    this->lblGa->setColor(Color3B::BLACK);
    //    this->lblTotalGa->setColor(Color3B::BLACK);
    //    this->lblHo->setColor(Color3B::BLACK);
    //    this->lblTotalHo->setColor(Color3B::BLACK);
    
    //////////////////////////////////////////////////////////////////////////
    
    this->btnReBet = (Button*)this->pnlMain->getChildByName("btnReBet");
    this->btnReBet->loadTextures(StringUtils::format("design/ResBautom/%sbtn-datlai.png",SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("design/ResBautom/%sbtn-datlai-disable.png",SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("design/ResBautom/%sbtn-datlai-disable.png",SceneManager::getSingleton().getFolderLang().c_str()));
    
    
    
    
    this->btnX2 = (Button*)this->pnlMain->getChildByName("btnX2");
    this->btnX3 = (Button*)this->pnlMain->getChildByName("btnX3");
    this->btnX2->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonX2, this));
    this->btnX3->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonX3, this));
    this->btnReBet->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonReBet, this));
    
    this->timerBau = this->createTimerFromSprite(this->spActiveBau);
    this->btnBau->addChild(this->timerBau);
    this->timerCa = this->createTimerFromSprite(this->spActiveCa);
    this->btnCa->addChild(this->timerCa);
    this->timerGa = this->createTimerFromSprite(this->spActiveGa);
    this->btnGa->addChild(this->timerGa);
    this->timerCua = this->createTimerFromSprite(this->spActiveCua);
    this->btnCua->addChild(this->timerCua);
    this->timerHo = this->createTimerFromSprite(this->spActiveHo);
    this->btnHo->addChild(this->timerHo);
    this->timerTom = this->createTimerFromSprite(this->spActiveTom);
    this->btnTom->addChild(this->timerTom);
}

void LayerGameBauTom::initPanelHis()
{
    this->pnlHis = (Layout*)this->pnlMain->getChildByName("pnlHis");
    auto Sprite_21 = static_cast<Sprite*>(this->pnlHis->getChildByName("Sprite_21"));
    Sprite_21->setTexture(StringUtils::format("design/ResBautom/%spopup-lichsu.png",SceneManager::getSingleton().getFolderLang().c_str()));
    this->btnCloseHis = (Button*)this->pnlHis->getChildByName("btnCloseHis");
    this->pnlTable = (Layout*)this->pnlMain->getChildByName("pnlTable");
    this->pnlHis->setLocalZOrder(1);
    this->btnCloseHis->addClickEventListener(CC_CALLBACK_1(LayerGameBauTom::onButtonCloseHis, this));
    
    this->tableViewHis = TableView::create(this, this->pnlTable->getContentSize());
    this->tableViewHis->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tableViewHis->setPosition(this->pnlTable->getPosition());
    this->tableViewHis->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    this->tableViewHis->setTag(37);//table pickroom
    this->pnlMain->addChild(this->tableViewHis);
    this->tableViewHis->reloadData();
    this->tableViewHis->setDelegate(this);
    this->hidePanelHis(true);
    this->firstPosHis = this->pnlHis->getPosition();
    this->HideAllSuggestion();
    
}

void LayerGameBauTom::initPanelUser()
{
    this->nodeAvatarCai = (Node*)this->pnlMain->getChildByName("nodeCai");
    this->nodeAvatarPlayer = (Node*)this->pnlMain->getChildByName("nodeCai_0");
    this->lblUserMoney = (Text*)this->pnlMain->getChildByName("lblUserMoney");
    this->lblUserName = (Text*)this->pnlMain->getChildByName("lblUserName");
    this->lblRoom = (Text*)this->pnlMain->getChildByName("lblRoom");
    this->lblRoomMoney = (Text*)this->pnlMain->getChildByName("lblRoomMoney");
    this->lblBossMoney = (Text*)this->pnlMain->getChildByName("lblBossMoney");
    this->lblBet = (Text*)this->pnlMain->getChildByName("lblBet");
    this->lblBetEat = (Text*)this->pnlMain->getChildByName("lblBetEat");
    
    this->lblRoomMoney->setVisible(false);
    this->lblRoom->setString(dataManager.GetSysString(18));
    
    //user info
    this->avatarCai = CircleAvatar::create();
    this->avatarCai->setPosition(this->nodeAvatarCai->getPosition());
    this->pnlMain->addChild(this->avatarCai);
    this->avatarCai->setClickCallBack(CC_CALLBACK_0(LayerGameBauTom::avatarCaiCallBack, this));
    
    this->avatarMe = CircleAvatar::create();
    this->avatarMe->setPosition(this->nodeAvatarPlayer->getPosition());
    this->avatarMe->setScale(0.8);
    this->pnlMain->addChild(this->avatarMe);
    this->avatarMe->setClickCallBack(CC_CALLBACK_0(LayerGameBauTom::avatarMeCallBack, this));
    this->myName = GameUtils::getMyName();
    string namePlayer = GameUtils::getNamePlayer(this->myName);
    this->lblUserName->setString(namePlayer);
    this->updateInfoMe();
    this->canBeBoss = false;
}

void LayerGameBauTom::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    if (0 != cmd->compare(EXT_EVENT_START)
        && 0 != cmd->compare(EXT_EVENT_END)
        && 0 != cmd->compare(EXT_EVENT_GAME_RESULT)
        && 0 != cmd->compare(EXT_EVENT_GAME_BET_ERROR_NOTIFY)
        && 0 != cmd->compare(EXT_EVENT_GAME_INFO_NTF)
        && 0 != cmd->compare(EXT_EVENT_GAME_HIS_NTF)
        && 0 != cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_GAME_STATE_NTF)
        && 0 != cmd->compare(EXT_EVENT_GAME_BET_NTF)
        && 0 != cmd->compare("updateinfome")
        && 0 != cmd->compare(EXT_EVENT_REGISTER_BOSS_RES)
        && 0 != cmd->compare(EXT_EVENT_UNREGISTER_BOSS_RES)
        && 0 != cmd->compare(EXT_EVENT_GET_LIST_BOSS_RES)
        && 0 != cmd->compare(EXT_EVENT_NEXT_BOSS_NTF)
        && 0 != cmd->compare(EXT_EVENT_DEL_BOSS_NTF)
        && 0 != cmd->compare(EXT_EVENT_OFFER_BOSS_NTF)
        && 0 != cmd->compare(EXT_EVENT_LIST_WIN_NTF)
        && 0 != cmd->compare(EXT_EVENT_RESET_BET_RES)
        && 0 != cmd->compare(EXT_EVENT_RESET_BET_NTF)
        && 0 != cmd->compare(EXT_EVENT_TCC_OPEN_QUY_THUONG_NTF)
        && 0 != cmd->compare(EXT_EVENT_TCC_JACKPOT_NTF)){
        return;
    }
    pObjs->PutUtfString("_cmd_", *cmd);
    //only add to queues
    mThreadQueueMsg->addQueue(pObjs);
}

void LayerGameBauTom::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
    boost::shared_ptr<Room> room = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
    if (room != NULL)
    {
        boost::shared_ptr<RoomVariable> listUserPtr = room->GetVariable(EXT_FIELD_LIST_USER);
        if (listUserPtr)
        {
            this->listUserPlay = listUserPtr->GetStringValue()->c_str();
            this->updateListUser();
        }
        this->SetRoomConfig();
        
        boost::shared_ptr<RoomVariable> bossVar = room->GetVariable(EXT_FIELD_GAME_BOSS_UID);
        if (bossVar)
        {
            boost::shared_ptr<string> bossPtr = bossVar->GetStringValue();
            // 			if (bossPtr->compare("") != 0)
            // 			{
            this->updateInfoCai(bossPtr->c_str());
            //}
        }
    }
}

void LayerGameBauTom::onButtonQuit(Ref* pSender)
{
    boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (NULL == lastRoom)
    {
        SceneManager::getSingleton().gotoMain();
        SceneMain::getSingleton().prepareToPickGame();

    }
    else
    {
        SceneManager::getSingleton().setIsChuyenTab(false);
        SceneManager::getSingleton().setCurrRoomType(-1);
        boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
        GameServer::getSingleton().Send(request);
        SceneMain::getSingleton().prepareToPickGame();
    }
}

void LayerGameBauTom::onButtonHis(Ref* pSender)
{
    this->hidePanelHis(!this->isHide);
}

void LayerGameBauTom::onButtonCamera(Ref* pSender)
{
    this->btnCamera->setEnabled(false);
    if (!this->_filename.empty())
        Director::getInstance()->getTextureCache()->removeTextureForKey(this->_filename);
    long current_time = utils::getTimeInMilliseconds();
    this->_filename = StringUtils::format("bvscreenshot_%ld.png", current_time);
    utils::captureScreen(CC_CALLBACK_2(LayerGameBauTom::afterCaptured, this), this->_filename);
}

void LayerGameBauTom::onButtonCuopCai(Ref* pSender)
{
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    boost::shared_ptr<SFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_REGISTER_BOSS_REG, params, lastRoom));
    GameServer::getSingleton().Send(req2);
    this->btnCuopCai->setEnabled(false);
}

void LayerGameBauTom::onButtonMenu(Ref* pSender)
{
    
}

void LayerGameBauTom::onButtonCloseHis(Ref* pSender)
{
    this->hidePanelHis(!this->isHide);
}

void LayerGameBauTom::afterCaptured(bool succeed, const std::string& outputFile)
{
    if (succeed)
    {
        this->btnCamera->setEnabled(true);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        _filename = outputFile;
        IOS::saveScreenShot(outputFile);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        NativeHelper::saveScreenShot(_filename.c_str());
#endif
        SceneManager::getSingleton().showToast(dataManager.GetSysString(1));
        
    }
    else
    {
        this->btnCamera->setEnabled(true);
       // log("Capture screen failed.");
        SceneManager::getSingleton().showToast(dataManager.GetSysString(2));
    }
}

void LayerGameBauTom::initPanelChat()
{
    this->layerChatInGame = LayerMocChatInGame::create();
    this->layerChatInGame->setIsPickRoom(false);
    this->layerChatInGame->setPosition(Vec2(10, 10));
    this->addChild(this->layerChatInGame);
    this->layerChatInGame->hide();
}

void LayerGameBauTom::OnExtensionResponseProcess()
{
    this->param = this->mThreadQueueMsg->deQueue();
    boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");
   // log("CMD bautom = %s", cmd->c_str());
    if (0 == cmd->compare("hbc"))
    {
        return;
    }
    else if (0 == cmd->compare(EXT_EVENT_START))
    {
        this->event_EXT_EVENT_START();
    }
    else if (0 == cmd->compare(EXT_EVENT_END))
    {
        this->event_EXT_EVENT_END();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT))
    {
        this->event_EXT_EVENT_GAME_RESULT();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_BET_ERROR_NOTIFY))
    {
        this->event_EXT_EVENT_GAME_BET_ERROR_NOTIFY();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_INFO_NTF))
    {
        this->event_EXT_EVENT_GAME_INFO_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_HIS_NTF))
    {
        this->event_EXT_EVENT_GAME_HIS_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF))
    {
        this->event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_STATE_NTF))
    {
        this->event_EXT_EVENT_GAME_STATE_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_BET_NTF))
    {
        this->event_EXT_EVENT_GAME_BET_NTF();
    }
    else if (0 == cmd->compare("updateinfome"))
    {
        if (SceneManager::getSingleton().getCurrRoomType() == 0){
            boost::shared_ptr<double> amfptr = param->GetDouble("amf");
            boost::shared_ptr<string> uidptr = param->GetUtfString("uid");
            if (uidptr && amfptr)
            {
                string uid = uidptr->c_str();
                if (0 == uid.compare(this->myName))
                {
                    this->amfUserClient = *amfptr;
                }
                this->lblUserMoney->setString(formatMoneySymbol(this->amfUserClient));
            }
        }
        else{
            boost::shared_ptr<double> amfptr = param->GetDouble("amfs");
            boost::shared_ptr<string> uidptr = param->GetUtfString("uid");
            if (uidptr && amfptr)
            {
                string uid = uidptr->c_str();
                if (0 == uid.compare(this->myName))
                {
                    this->amfUserClient = *amfptr;
                }
                this->lblUserMoney->setString(formatMoneySymbol(this->amfUserClient));
            }
        }
    }
    else if (0 == cmd->compare(EXT_EVENT_REGISTER_BOSS_RES)){
        //log("rc= %d - rd= %s", rd, rd.c_str());
        this->event_EXT_EVENT_REGISTER_BOSS_RES();
    }
    else if (0 == cmd->compare(EXT_EVENT_UNREGISTER_BOSS_RES)){
        int rc = (int)*this->param->GetInt("rc");
        std::string rd = *this->param->GetUtfString("rd");
        //log("rc= %d - rd= %s", rd, rd.c_str());
    }
    else if (0 == cmd->compare(EXT_EVENT_GET_LIST_BOSS_RES)){
        // 		std::string lstDkCai = *this->param->GetUtfString("lstRegister");
        // 		log("lst= %s", lstDkCai.c_str());
    }
    else if (0 == cmd->compare(EXT_EVENT_NEXT_BOSS_NTF)){
        this->event_EXT_EVENT_NEXT_BOSS_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_DEL_BOSS_NTF)){
        boost::shared_ptr<string> uidPtr = this->param->GetUtfString(EXT_FIELD_USERID);
        if (uidPtr != NULL)
        {
            if (0 == this->bossName.compare(uidPtr->c_str()))
            {
                this->updateInfoCai(dataManager.GetSysString(18));
            }
            if (0 == this->myName.compare(uidPtr->c_str()))
                SceneManager::getSingleton().showToast(dataManager.GetSysString(3));
            if (this->bossNext.compare(uidPtr->c_str()) == 0)
                this->bossNext = "";
        }
    }
    else if (0 == cmd->compare(EXT_EVENT_OFFER_BOSS_NTF)){
        this->event_EXT_EVENT_OFFER_BOSS_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_LIST_WIN_NTF)){
        this->event_EXT_EVENT_LIST_WIN_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_RESET_BET_RES))
    {
        for (int i = 0; i < this->listBetInfoClient.size(); i++)
        {
            this->listBetInfoClient[i].userbet = 0;
        }
        
        this->updateInfoMe();
    }
    else if (0 == cmd->compare(EXT_EVENT_RESET_BET_NTF))
    {
        boost::shared_ptr<string> gbt = this->param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
        if (gbt != NULL)
        {
           // log("gbt = %s", gbt->c_str());
            for (int i = 0; i < this->listBetInfoClient.size(); i++)
            {
                this->listBetInfoClient[i].totalbet = 0;
            }
            this->ReloadGameWhenReturn(gbt->c_str());
            for (int i = 0; i < this->listBetInfoClient.size(); i++)
            {
                string bettype = this->listBetInfoClient[i].bettype;
                int totalBet = this->listBetInfoClient[i].totalbet;
                int betval = this->listBetInfoClient[i].userbet;
                
                this->setMoneyBetBautom(totalBet, betval, bettype);
            }
            
        }
    }
    else if (0 == cmd->compare(EXT_EVENT_TCC_OPEN_QUY_THUONG_NTF)){
        auto top = this->param->GetUtfString(EXT_FIELD_GAME_HIS_QUY_THUONG);
        boost::shared_ptr<long> timeOpenHu = this->param->GetInt(EXT_FIELD_GAME_TIME_OPEN_EVENT);
        
        if (timeOpenHu){
            this->timeNoHu = (int)*timeOpenHu;
            if (this->timeNoHu>0){
                this->schedule(schedule_selector(LayerGameBauTom::updateTimeMoQuy),1);
            }else{
                this->txtTimerHuThuong->setString("00:00:00:00");
            }
        }
        if (top){
            if (top->compare("|") == 0) return;
            this->removeChildByTag(2390);
            LayerPopupNoHuBauTom* xh = LayerPopupNoHuBauTom::create();
            xh->showNoHuThuong(*top);
            this->addChild(xh,700000);
            xh->setTag(2390);
        }
    }
    else if (0 == cmd->compare(EXT_EVENT_TCC_JACKPOT_NTF)){
        auto huthuong = this->param->GetLong(EXT_FIELD_GAME_JACKPOT_VALUE);
        if (huthuong){
            this->txtHuThuong->setString(mUtils::convertMoneyEx(*huthuong));
        }
    }
}


void LayerGameBauTom::onButtonBet(Ref* pSender)
{
    int _tag = ((Button*)pSender)->getTag();// 77 - 83
    this->currBetvalue = this->listBetvalue[_tag - 77];
    this->setStateButtonBet(_tag);
}

void LayerGameBauTom::updatepress(float delta)
{
    this->press += delta;
    if (this->press >= this->longpresstime && this->lastTag != -1){
        
        if (this->lastTag != this->OldPressTag){
            this->OldPressTag = this->lastTag;
            string bettype[6] = { "TOM", "CUA", "CA", "GA", "BAU", "HUOU" };
            string betTypeCurrent = bettype[this->lastTag - 107];
            if (betTypeCurrent == "TOM"){
                this->imgMoneyTom->setVisible(true);
            }
            if (betTypeCurrent == "CUA"){
                this->imgMoneyCua->setVisible(true);
            }
            if (betTypeCurrent == "CA"){
                this->imgMoneyCa->setVisible(true);
            }
            if (betTypeCurrent == "GA"){
                this->imgMoneyGa->setVisible(true);
            }
            if (betTypeCurrent == "BAU"){
                this->imgMoneyBau->setVisible(true);
            }
            if (betTypeCurrent == "HUOU"){
                this->imgMoneyHo->setVisible(true);
            }
        }
    }
}
void LayerGameBauTom::onButtonBauTom(Ref *sender, ui::Widget::TouchEventType pType)
{
    if (SceneMain::getSingleton().chatting == true) return;

    if (pType == ui::Widget::TouchEventType::BEGAN){
        
        int _tag = ((Button*)sender)->getTag();// 107 - 112
        this->lastTag = _tag;
        this->press = 0;
        this->schedule(schedule_selector(LayerGameBauTom::updatepress),0.1);
        
    }
    
    if (pType == ui::Widget::TouchEventType::ENDED) {
        this->unschedule(schedule_selector(LayerGameBauTom::updatepress));
        this->HideAllSuggestion();
        
       // log("long press = %f", this->press);
        if (this->press >= this->longpresstime)
        {
            return;
        }
        
        int _tag = ((Button*)sender)->getTag();// 107 - 112
        this->lastTag = _tag;
        string bettype[6] = { "TOM", "CUA", "CA", "GA", "BAU", "HUOU" };
        if (this->CurrentGameState == false)
        {
            SceneManager::getSingleton().showToast(dataManager.GetSysString(4));
            return;
        }
        
        if (this->meIsBoss == true)
        {
            SceneManager::getSingleton().showToast(dataManager.GetSysString(5));
            return;
        }
        
        if (this->currBetvalue == 0)
        {
            SceneManager::getSingleton().showToast(dataManager.GetSysString(6));
            return;
        }
        
        if (!this->validateBet(this->currBetvalue))
        {
            SceneManager::getSingleton().showToast(dataManager.GetSysString(7));
            return;
        }
        string betTypeCurrent = bettype[_tag - 107];
        int countBet = 0;
        bool isAllow = false;
        for (int i = 0; i < this->listBetInfoClient.size(); i++)
        {
            if (this->listBetInfoClient[i].userbet > 0)
            {
                countBet++;
                if (this->listBetInfoClient[i].bettype.compare(betTypeCurrent) == 0 && countBet <= 3)
                    isAllow = true;
            }
        }
        if (countBet >= 10 && isAllow == false)
        {
            SceneManager::getSingleton().showToast(dataManager.GetSysString(8));
            return;
        }
        this->animationBet(this->currBetvalue,betTypeCurrent,1);
        this->updateBetInfoClient(betTypeCurrent, this->currBetvalue, this->currBetvalue);
        this->amfUserClient -= this->currBetvalue;
        this->lblUserMoney->setString(formatMoneySymbol(this->amfUserClient));
        DiceInfo infoBet = this->listBetInfoClient[_tag - 107];
        this->setMoneyBetBautom(infoBet.totalbet, infoBet.userbet, betTypeCurrent);
        
        boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
        if (lastRoom == NULL)
            return;
        //send smartfox sever msg
        boost::shared_ptr<SFSObject> params(new SFSObject());
        params->PutUtfString(EXT_FIELD_GAME_BET_VALUE, StringUtils::format("%d", this->currBetvalue));
        params->PutUtfString(EXT_FIELD_GAME_BET_TYPE, betTypeCurrent);
        boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
        GameServer::getSingleton().Send(req2);
        
        this->lastBetType = betTypeCurrent;
    }
    
    if (pType == ui::Widget::TouchEventType::CANCELED){
        this->unschedule(schedule_selector(LayerGameBauTom::updatepress));
        this->HideAllSuggestion();
    }
}
void LayerGameBauTom::HideAllSuggestion()
{
    
    this->imgMoneyTom->setVisible(false);
    this->imgMoneyCua->setVisible(false);
    this->imgMoneyCa->setVisible(false);
    this->imgMoneyGa->setVisible(false);
    this->imgMoneyBau->setVisible(false);
    this->imgMoneyHo->setVisible(false);
    this->OldPressTag = -1;
}



void LayerGameBauTom::onButtonReBet(Ref* pSender)
{
    if (this->CurrentGameState == false)
    {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(4));
        return;
    }
    if (this->meIsBoss == true)
    {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(5));
        return;
    }
    
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    boost::shared_ptr<SFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_RESET_BET_REG, params, lastRoom));
    GameServer::getSingleton().Send(req2);
}

void LayerGameBauTom::onButtonX2(Ref* pSender)
{
    if (this->CurrentGameState == false)
    {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(4));
        return;
    }
    
    if (this->lastBetType.compare("") == 0)
    {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(9));
        return;
    }
    if (this->currBetvalue == 0)
    {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(6));
        return;
    }
    int betX2 = this->currBetvalue * 2;
    if (!this->validateBet(betX2))
    {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(7));
        return;
    }
    this->updateBetInfoClient(this->lastBetType, betX2, betX2);
    this->amfUserClient -= betX2;
    this->lblUserMoney->setString(formatMoneySymbol(this->amfUserClient));
    DiceInfo infoBet = this->listBetInfoClient[this->lastTag - 107];
    this->setMoneyBetBautom(infoBet.totalbet, infoBet.userbet, this->lastBetType);
    
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    //send smartfox sever msg
    boost::shared_ptr<SFSObject> params(new SFSObject());
    params->PutUtfString(EXT_FIELD_GAME_BET_VALUE, StringUtils::format("%d", betX2));
    params->PutUtfString(EXT_FIELD_GAME_BET_TYPE, this->lastBetType);
    boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
    GameServer::getSingleton().Send(req2);
}

void LayerGameBauTom::onButtonX3(Ref* pSender)
{
    if (this->CurrentGameState == false)
    {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(4));
        return;
    }
    
    if (this->lastBetType.compare("") == 0)
    {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(9));
        return;
    }
    if (this->currBetvalue == 0)
    {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(6));
        return;
    }
    int betX2 = this->currBetvalue * 3;
    if (!this->validateBet(betX2))
    {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(7));
        return;
    }
    this->updateBetInfoClient(this->lastBetType, betX2, betX2);
    this->amfUserClient -= betX2;
    this->lblUserMoney->setString(formatMoneySymbol(this->amfUserClient));
    DiceInfo infoBet = this->listBetInfoClient[this->lastTag - 107];
    this->setMoneyBetBautom(infoBet.totalbet, infoBet.userbet, this->lastBetType);
    
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    //send smartfox sever msg
    boost::shared_ptr<SFSObject> params(new SFSObject());
    params->PutUtfString(EXT_FIELD_GAME_BET_VALUE, StringUtils::format("%d", betX2));
    params->PutUtfString(EXT_FIELD_GAME_BET_TYPE, this->lastBetType);
    boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
    GameServer::getSingleton().Send(req2);
}

void LayerGameBauTom::updateEvent(float dt)
{
    if (this->mThreadQueueMsg == NULL) return;
    if (this->mThreadQueueMsg->isQueueEmpty())
        return;
    this->mThreadQueueMsg->run();
}

void LayerGameBauTom::initTable()
{
    this->mDiaNoc = (ImageView*)this->pnlMain->getChildByName("imgBat");
    this->mDiaNoc->setLocalZOrder(102);
    for (int i = 0; i < 3; i++)
    {
        ImageView* img = (ImageView*)this->mDiaNoc->getChildByName(StringUtils::format("img%d", i + 1));
        if (img)
        {
            this->arrResult.push_back(img);
        }
    }
    
    Node *nodeTimer = (Node *)this->pnlMain->getChildByName("nodeTimer");
    
    this->bgTimer = Sprite::create("moc3-xocdia-timer.png");
    this->bgTimer->setPosition(nodeTimer->getPosition());
    this->pnlMain->addChild(this->bgTimer);
    
    this->txtTimer = Label::createWithTTF("", kBoldFont, 40);
    this->txtTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->txtTimer->setColor(Color3B(255, 255, 255));
    if (SceneManager::getSingleton().isNagaWin){
        this->txtTimer->setColor(Color3B::BLACK);

    }
    this->txtTimer->setPosition(Vec2(this->bgTimer->getContentSize().width / 2, this->bgTimer->getContentSize().height / 2));
    if (SceneManager::getSingleton().isNagaWin){
        this->txtTimer->setPosition(Vec2(this->bgTimer->getContentSize().width / 2-3, this->bgTimer->getContentSize().height / 2+5));

    }
    this->bgTimer->addChild(this->txtTimer);
}

void LayerGameBauTom::resetGame()
{
    this->unschedule(schedule_selector(LayerGameBauTom::updateTimer));
    this->bgTimer->setVisible(false);
    this->lblTom->setString(dataManager.GetSysString(513)+": 0");
    this->lblBau->setString(dataManager.GetSysString(513)+": 0");
    this->lblCa->setString(dataManager.GetSysString(513)+": 0");
    this->lblHo->setString(dataManager.GetSysString(513)+": 0");
    this->lblGa->setString(dataManager.GetSysString(513)+": 0");
    this->lblCua->setString(dataManager.GetSysString(513)+": 0");
    
    
    this->timerBau->setVisible(false);
    this->timerCua->setVisible(false);
    this->timerTom->setVisible(false);
    this->timerCa->setVisible(false);
    this->timerGa->setVisible(false);
    
    this->spActiveHo->setVisible(false);
    this->spActiveBau->setVisible(false);
    this->spActiveCua->setVisible(false);
    this->spActiveTom->setVisible(false);
    this->spActiveCa->setVisible(false);
    this->spActiveGa->setVisible(false);
    
    this->stopAllActions();
    this->spX2Ca->setVisible(false);
    this->spX2Cua->setVisible(false);
    this->spX2Tom->setVisible(false);
    this->spX2Bau->setVisible(false);
    this->spX2Ho->setVisible(false);
    this->spX2Ga->setVisible(false);
    
    this->spX3Ca->setVisible(false);
    this->spX3Cua->setVisible(false);
    this->spX3Tom->setVisible(false);
    this->spX3Bau->setVisible(false);
    this->spX3Ho->setVisible(false);
    this->spX3Ga->setVisible(false);
    
    
    this->lblTotalBau->setString(dataManager.GetSysString(746)+": 0");
    this->lblTotalCa->setString(dataManager.GetSysString(746)+": 0");
    this->lblTotalCua->setString(dataManager.GetSysString(746)+": 0");
    this->lblTotalGa->setString(dataManager.GetSysString(746)+": 0");
    this->lblTotalHo->setString(dataManager.GetSysString(746)+": 0");
    this->lblTotalTom->setString(dataManager.GetSysString(746)+": 0");
    
    this->lblBet->setString("0");
    this->lblBetEat->setString("0");
    this->totalBetEat = 0;
    
    this->resetButtonBautom();
//    if (this->lastBetType.compare("") == 0)
//    {
//
//    }
//
    this->resultGame = "";
    this->listwin = "";
    this->mDiaNoc->loadTexture("dia-1.png");
    this->avatarMe->showAnimationWin(false);
    for (int i = 0; i < this->listBetInfoClient.size(); i++)
    {
        this->listBetInfoClient[i].totalbet = 0;
        this->listBetInfoClient[i].userbet = 0;
    }
    for (int i = 0; i < this->listAvatar.size(); i++)
    {
        this->listAvatar[i]->showAnimationWin(false);
    }
    for (int i = 0; i < this->arrResult.size(); i++)
    {
        this->arrResult[i]->setVisible(false);
    }
    for (int i = 0 ; i < this->listImgMoney.size();i++){
        this->listImgMoney[i]->removeFromParent();
        this->listImgMoney[i]=NULL;
        
    }
    this->listImgMoney.clear();
    
    LayerAnimationXocDia* a = (LayerAnimationXocDia*)this->pnlMain->getChildByTag(TAG_ANIM_END_GAME);
    if (a){
        a->stopAllActions();
        a->setCallBackListener(NULL);
        
    }
    
    LayerAnimationXocDia* b = (LayerAnimationXocDia*)this->pnlMain->getChildByTag(TAG_ANIM_START_GAME);
    if (b){
        b->stopAllActions();
        b->setCallBackListener(NULL);
    }
    this->pnlMain->removeChildByTag(TAG_ANIM_END_GAME);
    this->pnlMain->removeChildByTag(TAG_ANIM_START_GAME);
    
    //    this->lblTom->setColor(Color3B::BLACK);
    //    this->lblTotalTom->setColor(Color3B::BLACK);
    //    this->lblCa->setColor(Color3B::BLACK);
    //    this->lblTotalCa->setColor(Color3B::BLACK);
    //    this->lblCua->setColor(Color3B::BLACK);
    //    this->lblTotalCua->setColor(Color3B::BLACK);
    //    this->lblBau->setColor(Color3B::BLACK);
    //    this->lblTotalBau->setColor(Color3B::BLACK);
    //    this->lblGa->setColor(Color3B::BLACK);
    //    this->lblTotalGa->setColor(Color3B::BLACK);
    //    this->lblHo->setColor(Color3B::BLACK);
    //    this->lblTotalHo->setColor(Color3B::BLACK);
    
    
    this->listImgMove.clear();
    
    for (int i = 0 ; i < this->listImgWin.size();i++){
        if (this->listImgWin[i]){
            this->listImgWin[i]->removeFromParent();
            this->listImgWin[i]=NULL;
        }
        
    }
    this->listImgWin.clear();
    this->listImgCua.clear();
    this->listImgCa.clear();
    this->listImgHo.clear();
    this->listImgGa.clear();
    this->listImgBau.clear();
    this->listImgTom.clear();
    this->listImgMeCua.clear();
    this->listImgMeCa.clear();
    this->listImgMeHo.clear();
    this->listImgMeGa.clear();
    this->listImgMeBau.clear();
    this->listImgMeTom.clear();
    this->listCheckRemove.clear();
    this->listImgMeWin.clear();
}
void LayerGameBauTom::initBetString()
{
    if (this->listBetvalue.size() == 0)
    {
        int currRoomType = SceneManager::getSingleton().getCurrRoomType();
        if (currRoomType == 0)//moc
        {
            this->listBetvalue.push_back(500);
            this->listBetvalue.push_back(1000);
            this->listBetvalue.push_back(5000);
            this->listBetvalue.push_back(10000);
            this->listBetvalue.push_back(50000);
            this->listBetvalue.push_back(100000);
            this->listBetvalue.push_back(500000);
            this->listBetvalue.push_back(1000000);
        }
        else if (currRoomType == 1){ //hao
            this->listBetvalue.push_back(500);
            this->listBetvalue.push_back(1000);
            this->listBetvalue.push_back(5000);
            this->listBetvalue.push_back(10000);
            this->listBetvalue.push_back(50000);
            this->listBetvalue.push_back(100000);
            this->listBetvalue.push_back(500000);
            this->listBetvalue.push_back(1000000);
        }
    }
    
    //    this->btnBet1k->setTitleText(StringUtils::format("%s", formatMoneyPicker(this->listBetvalue[0]).c_str()));
    //    this->btnBet5k->setTitleText(StringUtils::format("%s", formatMoneyPicker(this->listBetvalue[1]).c_str()));
    //    this->btnBet10k->setTitleText(StringUtils::format("%s", formatMoneyPicker(this->listBetvalue[2]).c_str()));
    //    this->btnBet50k->setTitleText(StringUtils::format("%s", formatMoneyPicker(this->listBetvalue[3]).c_str()));
    //    this->btnBet100k->setTitleText(StringUtils::format("%s", formatMoneyPicker(this->listBetvalue[4]).c_str()));
    //    this->btnBet500k->setTitleText(StringUtils::format("%s", formatMoneyPicker(this->listBetvalue[5]).c_str()));
    //    this->btnBet1000k->setTitleText(StringUtils::format("%s", formatMoneyPicker(this->listBetvalue[6]).c_str()));
    
}

void LayerGameBauTom::updateTimer(float dt)
{
    if (this->TimerCountDown > 0) {
        this->TimerCountDown--;
        
        string str = boost::to_string(this->TimerCountDown);
        this->txtTimer->setString(str);
        txtTimer->setVisible(true);
        
        if (this->TimerCountDown > 5) {
            if (0 == this->TimerCountDown % 5) {
                //this->Update5sStep(0);
            }
        }
        else if (this->TimerCountDown < 5 && this->TimerCountDown > 1) {
            if (0 == this->TimerCountDown % 2) {
                //this->Update5sStep(0);
            }
        }
        else {
            //this->Update5sStep(0);
        }
    }
    else {
        this->unschedule(schedule_selector(LayerGameBauTom::updateTimer));
        this->TimerCountDown = this->PlayTurnTime;
    }
}

void LayerGameBauTom::UpdateTimeEndGame()
{
    this->CurrentGameState = false;
    //Dung dem timer
    this->txtTimer->setVisible(false);
    this->bgTimer->setVisible(false);
    string str = boost::to_string(this->PlayTurnTime);
    this->txtTimer->setString(str);
    this->unschedule(schedule_selector(LayerGameBauTom::updateTimer));
    
    // Dem nguoc thoi gian den van tiep theo.
    //this->lblCountNextTime->setVisible(false);
    this->TimerCountNextTime = this->BetweentwoPlayTime;
    this->schedule(schedule_selector(LayerGameBauTom::updateToNextTurn),1);
}

void LayerGameBauTom::updateToNextTurn(float dt)
{
    if (this->TimerCountNextTime > 0){
        this->txtTimer->setString(StringUtils::format("%d", this->TimerCountNextTime));
        this->txtTimer->setVisible(true);
        bgTimer->setVisible(true);
        this->TimerCountNextTime--;
    }
    else
    {
        this->unschedule(schedule_selector(LayerGameBauTom::updateToNextTurn));
        this->txtTimer->setVisible(false);
        this->bgTimer->setVisible(false);
        this->TimerCountNextTime = this->BetweentwoPlayTime;
    }
}

void LayerGameBauTom::UpdateTimeStartGame()
{
    this->unschedule(schedule_selector(LayerGameBauTom::updateToNextTurn));
    // Dem timer thoi gian van choi
    this->TimerCountDown = this->PlayTurnTime;
    this->schedule(schedule_selector(LayerGameBauTom::updateTimer),1);
    // 	this->bgTimer->setVisible(true);
    // 	this->txtTimer->setVisible(true);
}

void LayerGameBauTom::setStateButtonBet(int _tag)
{
    
    //reset all button;
    this->spActive1000k->setVisible(false);
    this->spActive500k->setVisible(false);
    this->spActive100k->setVisible(false);
    this->spActive50k->setVisible(false);
    this->spActive10k->setVisible(false);
    this->spActive5k->setVisible(false);
    this->spActive1k->setVisible(false);
    this->spActive500->setVisible(false);
    
    
    if (!dataManager.getAppConfig().isChip){
        this->btnBet500->setPositionY(231);
        this->btnBet1k->setPositionY(231);
        this->btnBet5k->setPositionY(231);
        this->btnBet10k->setPositionY(231);
        this->btnBet50k->setPositionY(231);
        this->btnBet100k->setPositionY(231);
        this->btnBet500k->setPositionY(231);
        this->btnBet1000k->setPositionY(231);
    }
    else{
        this->btnBet500->setPositionY(176);
        this->btnBet1k->setPositionY(176);
        this->btnBet5k->setPositionY(176);
        this->btnBet10k->setPositionY(176);
        this->btnBet50k->setPositionY(176);
        this->btnBet100k->setPositionY(176);
        this->btnBet500k->setPositionY(176);
        this->btnBet1000k->setPositionY(176);
    }
    
    
    switch (_tag)
    {
        case 77:
            this->spActive500->setVisible(true);
            this->btnBet500->setPositionY(this->btnBet500->getPositionY()+19);
            break;
        case 78:
            this->spActive1k->setVisible(true);
            this->btnBet1k->setPositionY(this->btnBet1k->getPositionY()+19);
            
            break;
        case 79:
            this->spActive5k->setVisible(true);
            this->btnBet5k->setPositionY(this->btnBet5k->getPositionY()+19);
            
            break;
        case 80:
            this->spActive10k->setVisible(true);
            this->btnBet10k->setPositionY(this->btnBet10k->getPositionY()+19);
            
            break;
        case 81:
            this->spActive50k->setVisible(true);
            this->btnBet50k->setPositionY(this->btnBet50k->getPositionY()+19);
            
            break;
        case 82:
            this->spActive100k->setVisible(true);
            this->btnBet100k->setPositionY(this->btnBet100k->getPositionY()+19);
            
            break;
        case 83:
            this->spActive500k->setVisible(true);
            this->btnBet500k->setPositionY(this->btnBet500k->getPositionY()+19);
            break;
        case 84:
            this->spActive1000k->setVisible(true);
            this->btnBet1000k->setPositionY(this->btnBet1000k->getPositionY()+19);
            
            break;
        default:
            break;
    }
}

void LayerGameBauTom::updateListUser()
{
    vector<string> listUser = mUtils::splitString(this->listUserPlay, ';');
    this->lblUserCount->setString(StringUtils::format("%lu", listUser.size()));
    
    if (listUser.size() < 1)
        return;
    if (listUser.size() == 1)
    {
        if (listUser[0].compare(this->myName) == 0)
        {
            for (int i = 0; i < this->listAvatar.size(); i++)
            {
                this->listAvatar[i]->setVisible(false);
            }
            return;
        }
    }
    
    for (int i = 0; i < listUser.size(); i++)
    {
        if (listUser[i].compare(this->myName) == 0)
        {
            listUser.erase(listUser.begin() + i);
            break;
        }
    }
    
    for (int i = 0; i < this->listAvatar.size(); i++)
    {
        this->listAvatar[i]->setVisible(false);
    }
    
    int maxSize = listUser.size() > 4 ? 4 : listUser.size();
    if (maxSize<=0) return;
    vector<int> avatarChosse;
    while (avatarChosse.size()<maxSize) {
        if (avatarChosse.size() >=maxSize)
            break;
        int randomxy = rand() % maxSize;
        bool isShouldShow = true;
        for (int i = 0; i< avatarChosse.size();i++){
            if (avatarChosse[i] == randomxy){
                isShouldShow = false;
            }
        }
        if (isShouldShow){
            this->listAvatar[avatarChosse.size()]->setVisible(true);
            this->listAvatar[avatarChosse.size()]->setName(listUser[randomxy]);
            string urlAvatar = GameUtils::getAvatarUser(listUser[randomxy]);
            this->listAvatar[avatarChosse.size()]->setAvatarImage(urlAvatar);
            avatarChosse.push_back(randomxy);
        }
        
    }
}

void LayerGameBauTom::updateInfoMe()
{
    int currRoomType = SceneManager::getSingleton().getCurrRoomType();
    double amfUser = 0;
    if (currRoomType == 0)
        amfUser = GameUtils::getAmfGoldUser();
    else if (currRoomType == 1)
        amfUser = GameUtils::getAmfSilverUser(this->myName);
    this->amfUserClient = amfUser;
    // 	if (this->isLoadedAvatar == false)
    // 	{
    this->lblUserMoney->setString(formatMoneySymbol(amfUser));
    string urlAvatar = GameUtils::getAvatarUser(this->myName);
    this->avatarMe->setAvatarImage(urlAvatar);
    this->isLoadedAvatar = true;
    //	}
}

void LayerGameBauTom::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    if (ptrEventParamValueUser == NULL) return;
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    if (ptrNotifiedUser == NULL) return;
    if (ptrNotifiedUser->IsItMe())
    {
        string uid = *ptrNotifiedUser->Name();

        if (this->CurrentGameState == false)
        {
            this->isLoadedAvatar = false;
            this->updateInfoMe();
           
        }else{
            int currRoomType = SceneManager::getSingleton().getCurrRoomType();
            double amfUser = 0;
            if (currRoomType == 0){
                amfUser = GameUtils::getAmfGoldUser();
                boost::shared_ptr<ISFSObject> params(new SFSObject());
                params->PutDouble("amf", amfUser);
                params->PutUtfString("uid", uid);
                params->PutUtfString("_cmd_", "updateinfome");
                this->mThreadQueueMsg->addQueue(params);
            }
            else if (currRoomType == 1){
                amfUser = GameUtils::getAmfSilverUser(this->myName);
                boost::shared_ptr<ISFSObject> params(new SFSObject());
                params->PutDouble("amfs", amfUser);
                params->PutUtfString("uid", uid);
                params->PutUtfString("_cmd_", "updateinfome");
                this->mThreadQueueMsg->addQueue(params);
            }
        }
    }
    //update for boss
    if (ptrNotifiedUser->Name()->compare(this->bossName) == 0)
    {
        this->updateInfoCai(this->bossName);
    }
}

bool LayerGameBauTom::validateBet(int betValue)
{
    if (betValue > this->amfUserClient)
        return false;
    
    return true;
}

void LayerGameBauTom::setMoneyBetBautom(int totalBet, int meBet, string tagBetType)
{
    if (tagBetType.compare("TOM") == 0)
    {
        this->lblTom->setString(dataManager.GetSysString(513)+": " + mUtils::convertMoneyEx(meBet));
        this->lblTotalTom->setString(dataManager.GetSysString(746)+": " + mUtils::convertMoneyEx(totalBet));
    }
    else if (tagBetType.compare("CUA") == 0)
    {
        this->lblCua->setString(dataManager.GetSysString(513)+": " + mUtils::convertMoneyEx(meBet));
        this->lblTotalCua->setString(dataManager.GetSysString(746)+": " + mUtils::convertMoneyEx(totalBet));
    }
    else if (tagBetType.compare("CA") == 0)
    {
        this->lblCa->setString(dataManager.GetSysString(513)+": " + mUtils::convertMoneyEx(meBet));
        this->lblTotalCa->setString(dataManager.GetSysString(746)+": " + mUtils::convertMoneyEx(totalBet));
    }
    else if (tagBetType.compare("GA") == 0)
    {
        this->lblGa->setString(dataManager.GetSysString(513)+": " + mUtils::convertMoneyEx(meBet));
        this->lblTotalGa->setString(dataManager.GetSysString(746)+": " + mUtils::convertMoneyEx(totalBet));
    }
    else if (tagBetType.compare("BAU") == 0)
    {
        this->lblBau->setString(dataManager.GetSysString(513)+": " + mUtils::convertMoneyEx(meBet));
        this->lblTotalBau->setString(dataManager.GetSysString(746)+": " + mUtils::convertMoneyEx(totalBet));
    }
    else if (tagBetType.compare("HUOU") == 0)
    {
        this->lblHo->setString(dataManager.GetSysString(513)+": " + mUtils::convertMoneyEx(meBet));
        this->lblTotalHo->setString(dataManager.GetSysString(746)+": " + mUtils::convertMoneyEx(totalBet));
    }
    int totalUserBet = 0;
    for (int i = 0; i < this->listBetInfoClient.size(); i++)
    {
        totalUserBet += this->listBetInfoClient[i].userbet;
    }
    this->lblBet->setString(mUtils::convertMoneyEx(totalUserBet));
}

void LayerGameBauTom::initAllVariables()
{
    this->BetweentwoPlayTime = 15;
    this->TimerCountDown = 45;
    this->TimerCountNextTime = 15;
    this->listUserPlay = "";
    this->currBetvalue = 0;
    this->PlayTurnTime = 45;
    this->CurrentGameState = false;
    
    this->listBetInfoClient.push_back(DiceInfo("TOM", 0, 0));
    this->listBetInfoClient.push_back(DiceInfo("CUA", 0, 0));
    this->listBetInfoClient.push_back(DiceInfo("CA", 0, 0));
    this->listBetInfoClient.push_back(DiceInfo("GA", 0, 0));
    this->listBetInfoClient.push_back(DiceInfo("BAU", 0, 0));
    this->listBetInfoClient.push_back(DiceInfo("HUOU", 0, 0));
    
}

void LayerGameBauTom::updateBetInfoClient(string bettype, int total, int meBet)
{
    for (int i = 0; i < this->listBetInfoClient.size(); i++)
    {
        if (this->listBetInfoClient[i].bettype.compare(bettype) == 0){
            this->listBetInfoClient[i].totalbet += total;
            this->listBetInfoClient[i].userbet += meBet;
            break;
        }
    }
}

void LayerGameBauTom::ReloadGameWhenReturn(string gameBetType)
{
    //gbt = TOM:0:100;CUA:0:0;CA:0:0;BAU:0:0;HUOU:0:0;GA:0:0
    vector<string> listInfos = mUtils::splitString(gameBetType, ';');
   // log("RESULT = %s", gameBetType.c_str());
    for (int i = 0; i < listInfos.size(); i++)
    {
        vector<string> infos = mUtils::splitString(listInfos[i], ':');
        if (infos.size() == 3)
        {
            string betType = infos[0];
            int totalBet = atoi(infos[2].c_str());
            int meBet = atoi(infos[1].c_str());
            if (meBet>0){
                this->animationBetWhenReturn(meBet,betType,1);
                this->animationBetWhenReturn(totalBet-meBet,betType,1);
            }
            else{
                this->animationBetWhenReturn(totalBet,betType,1);
            }
            
            this->updateBetInfoClient(betType, totalBet, meBet);
            this->setMoneyBetBautom(totalBet, meBet, betType);
        }
    }
}

void LayerGameBauTom::SetRoomConfig()
{
    boost::shared_ptr<Room> lstRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lstRoom != NULL){
        boost::shared_ptr<RoomVariable> playtime = lstRoom->GetVariable("playtime");
        if (playtime != NULL){
            this->PlayTurnTime = *playtime->GetIntValue() / 1000;
        }
        boost::shared_ptr<RoomVariable> end = lstRoom->GetVariable("end");
        if (end != NULL){
            this->BetweentwoPlayTime = *end->GetIntValue() / 1000;
        }
        
        boost::shared_ptr<RoomVariable> activebets = lstRoom->GetVariable("activebets");
        //        this->listBetvalue.clear();
        //        if (activebets != NULL) {
        //            std::string betStr = *activebets->GetStringValue();
        //            auto arr = mUtils::splitString(betStr, ',');
        //           // log("list value bet: %s", betStr.c_str());
        //            for (auto i = 0; i < arr.size(); ++i) {
        //                this->listBetvalue.push_back(atoi(arr.at(i).c_str()));
        //            }
        //            this->initBetString();
        //        }
    }
}

void LayerGameBauTom::initAllListAvatar()
{
    for (int i = 0; i < 4; i++)
    {
        CircleAvatar* avatar = CircleAvatar::create();
        avatar->setName("");
        Node* avatarNode = (Node*)this->pnlMain->getChildByName(StringUtils::format("nodeCai_0_%d", i + 1));
        avatar->setPosition(avatarNode->getPosition());
        avatar->setVisible(false);
        this->pnlMain->addChild(avatar);
        this->listAvatar.push_back(avatar);
        avatar->setClickCallBackWithParam(CC_CALLBACK_2(LayerGameBauTom::avatarUserCallBack, this));
    }
    //init last avatar
    Node* avatarNode = (Node*)this->pnlMain->getChildByName("nodeCai_0_6");
    CircleAvatar* avatarLast = CircleAvatar::create();
    avatarLast->setName("");
    avatarLast->setPosition(avatarNode->getPosition());
    avatarLast->setVisible(false);
    this->pnlMain->addChild(avatarLast);
    this->listAvatar.push_back(avatarLast);
    
    Sprite* avatarBlur = Sprite::create("avatar_blur.png");
    avatarBlur->setPosition(0, 0);
    avatarLast->addChild(avatarBlur);
    
    
}

void LayerGameBauTom::rollbackBetInfoClient(string bettype, int betvalue)
{
    for (int i = 0; i < this->listBetInfoClient.size(); i++)
    {
        if (this->listBetInfoClient[i].bettype.compare(bettype) == 0){
            this->listBetInfoClient[i].totalbet -= betvalue;
            this->listBetInfoClient[i].userbet -= betvalue;
            this->setMoneyBetBautom(this->listBetInfoClient[i].totalbet, this->listBetInfoClient[i].userbet, bettype);
            break;
        }
    }
}
void LayerGameBauTom::event_EXT_EVENT_START()
{
    boost::shared_ptr<long> ttpptr = param->GetInt(EXT_FIELD_GAME_TOTAL_PLAYER);
    if (ttpptr)
    {
        string t = StringUtils::format("%s %d",dataManager.GetSysString(19).c_str(), *ttpptr);
        
    }
    this->resetGame();
    this->bgTimer->setVisible(false);
    this->CurrentGameState = true;
    this->UpdateTimeStartGame();
    this->scheduleOnce(schedule_selector(LayerGameBauTom::showAnimation), 0);
    //this->schedule(schedule_selector(LayerGameBauTom::showAnimation), this, 0.5, 1, 0.5, false);

}

void LayerGameBauTom::event_EXT_EVENT_END()
{
    this->UpdateTimeEndGame();
    boost::shared_ptr<string> rgptr = param->GetUtfString(EXT_FIELD_RESULT_GAME);
    boost::shared_ptr<string> rt10ptr = param->GetUtfString(EXT_FIELD_RESULT_TOP_10);
    boost::shared_ptr<long> gbv = this->param->GetInt(EXT_FIELD_GAME_BET_VALUE);
    boost::shared_ptr<long> gtbv = this->param->GetInt(EXT_FIELD_GAME_TOTAL_BET_VALUE);
    boost::shared_ptr<string> gbce = this->param->GetUtfString(EXT_FIELD_GAME_BET_CAN_EAT);
    
    if (gtbv != NULL)
    {
        this->totalBetEat = *gtbv;
    }
    //set result game
    if (rgptr != NULL)
    {
       // log("RESULT = %s", rgptr->c_str());
        this->mDiaNoc->loadTexture("dia_xocdia.png");
        this->resultGame = *rgptr;
        vector<string> listRes = mUtils::splitString(*rgptr, '_');
        for (int i = 0; i < listRes.size(); i++)
        {
            string res = this->getResImageByBetType(listRes[i]);
            this->arrResult[i]->setVisible(true);
            this->arrResult[i]->loadTexture(res);
        }
    }
    
    LayerAnimationXocDia *animate = LayerAnimationXocDia::create(Vec2(this->mDiaNoc->getPositionX(), this->mDiaNoc->getPositionY()), 1);
    animate->setCallBackListener(CC_CALLBACK_0(LayerGameBauTom::callBackAnimation, this));
    this->pnlMain->addChild(animate, 399, TAG_ANIM_END_GAME);
}

void LayerGameBauTom::event_EXT_EVENT_GAME_RESULT()
{
    boost::shared_ptr<long> gbv = this->param->GetInt(EXT_FIELD_GAME_BET_VALUE);
    boost::shared_ptr<long> gtbv = this->param->GetInt(EXT_FIELD_GAME_TOTAL_BET_VALUE);
    boost::shared_ptr<string> gbce = this->param->GetUtfString(EXT_FIELD_GAME_BET_CAN_EAT);
    
    boost::shared_ptr<string> rgptr = param->GetUtfString(EXT_FIELD_RESULT_GAME);
    boost::shared_ptr<string> rt10ptr = param->GetUtfString(EXT_FIELD_RESULT_TOP_10);
    
    if (gbv != NULL)
    {
        this->totalBetEat = *gbv;
        
    }
    //set result game
    // 	if (rgptr != NULL)
    // 	{
    // 		log("RESULT = %s", rgptr->c_str());
    // 		this->mDiaNoc->loadTexture("dia_xocdia.png");
    // 		this->resultGame = *rgptr;
    // 		vector<string> listRes = mUtils::splitString(*rgptr, '_');
    // 		for (int i = 0; i < listRes.size(); i++)
    // 		{
    // 			string res = this->getResImageByBetType(listRes[i]);
    // 			this->arrResult[i]->setVisible(true);
    // 			this->arrResult[i]->loadTexture(res);
    // 		}
    // 	}
}

void LayerGameBauTom::event_EXT_FIELD_GAME_BET_EXCHANGE()
{
    
}

void LayerGameBauTom::event_EXT_EVENT_GAME_INFO_NTF()
{
    boost::shared_ptr<string> ghtp = this->param->GetUtfString(EXT_FIELD_GAME_HISTORY_TOP_PLAYER);
    boost::shared_ptr<string> ghs = this->param->GetUtfString(EXT_FIELD_GAME_HISTORY_RESULT);
    //CUA_TOM_BAU;BAU_HUOU_BAU;GA_BAU_GA;BAU_TOM_BAU;TOM_HUOU_TOM;HUOU_CUA_TOM;CUA_TOM_CA;CUA_TOM_CA;CUA_GA_HUOU;CUA_TOM_CA;
    if (ghs)
    {
       // log("Game His = %s", ghs->c_str());
        this->listHis.clear();
        this->listHis = mUtils::splitString(*ghs, ';');
        
        std::reverse(this->listHis.begin(), this->listHis.end());
        this->tableViewHis->reloadData();
    }
}

void LayerGameBauTom::event_EXT_EVENT_GAME_HIS_NTF()
{
    
}

void LayerGameBauTom::event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF()
{
    SceneManager::getSingleton().showToast(dataManager.GetSysString(10));
    SceneManager::getSingleton().showPopupMoneyCharge();
}
void LayerGameBauTom::updateTimeMoQuy(float dt){
    
    int _time = this->timeNoHu--;
    if (this->timeNoHu<=0){
        this->txtTimerHuThuong->setString("00:00:00:00");
        this->unschedule(schedule_selector(LayerGameBauTom::updateTimeMoQuy));
        return;
    }
    int _day = _time / (60 * 60 * 24);
    int _hour = (_time / (60 * 60)) % 24;
    int _min = (_time / (60)) % 60;
    int _se = _time % 60;
    string strH = "";
    if (_hour<10){
        strH = StringUtils::format("0%d",_hour);
    }else{
        strH = StringUtils::format("%d",_hour);
    }
    string strM = "";
    if (_min<10){
        strM = StringUtils::format("0%d",_min);
    }else{
        strM = StringUtils::format("%d",_min);
    }
    
    string strS = "";
    if (_se<10){
        strS = StringUtils::format("0%d",_se);
    }else{
        strS = StringUtils::format("%d",_se);
    }
    this->txtTimerHuThuong->setString(StringUtils::format("%dd:%s:%s:%s",_day,strH.c_str(),strM.c_str(),strS.c_str()));
}
void LayerGameBauTom::event_EXT_EVENT_GAME_STATE_NTF()
{
   // log("event_EXT_EVENT_GAME_STATE_NTF");
    this->isJoin = true;
    
    boost::shared_ptr<long> gtime = this->param->GetInt(EXT_FIELD_GAME_TIME);
    boost::shared_ptr<string> gbt = this->param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
    
    boost::shared_ptr<long> gstate = this->param->GetInt(EXT_FIELD_GAME_PLAY_STATE);
    
    boost::shared_ptr<string> gbex = this->param->GetUtfString(EXT_FIELD_GAME_BET_EXCHANGE);
    boost::shared_ptr<string> bossPtr = this->param->GetUtfString(EXT_FIELD_GAME_BOSS_UID);
    boost::shared_ptr<long> gendtime = this->param->GetInt("gendtime");
    boost::shared_ptr<string> glastrs = this->param->GetUtfString(EXT_FIELD_RESULT_GAME);
    
    boost::shared_ptr<long> timeOpenHu = this->param->GetInt(EXT_FIELD_GAME_TIME_OPEN_EVENT);
    auto huthuong = this->param->GetLong(EXT_FIELD_GAME_JACKPOT_VALUE);

    if (timeOpenHu){
        this->timeNoHu = *timeOpenHu;
        if (this->timeNoHu>0){
            this->schedule(schedule_selector(LayerGameBauTom::updateTimeMoQuy),1);
        }else{
            this->txtTimerHuThuong->setString("00:00:00:00");
        }
    }
    
    if (huthuong){
        this->txtHuThuong->setString(mUtils::convertMoneyEx(*huthuong));
    }
    
    if (bossPtr != NULL)
    {
        if (bossPtr->compare("") != 0)
        {
            this->updateInfoCai(bossPtr->c_str());
        }
    }
    // Thoi gian con lai cua van choi
    if (gtime != NULL) {
        this->TimerCountDown = *gtime;
        this->txtTimer->setString(StringUtils::format("%d", this->TimerCountDown));
    }
    
    if (gbex != NULL) {
       // log("gbex = %s ", gbex->c_str());
    }
    this->resetGame();
    if (gstate != NULL) {
       // log("gstate = %ld ", *gstate);
        if (0 == *gstate) {
            this->bgTimer->setVisible(true);
            this->txtTimer->setString(StringUtils::format("%d", this->TimerCountDown));
            this->CurrentGameState = false;
            this->schedule(schedule_selector(LayerGameBauTom::updateTimer),1);
            if (glastrs){
                this->timerHo->setVisible(true);
                this->timerBau->setVisible(true);
                this->timerCua->setVisible(true);
                this->timerTom->setVisible(true);
                this->timerCa->setVisible(true);
                this->timerGa->setVisible(true);
                
                this->spActiveHo->setVisible(true);
                this->spActiveBau->setVisible(true);
                this->spActiveCua->setVisible(true);
                this->spActiveTom->setVisible(true);
                this->spActiveCa->setVisible(true);
                this->spActiveGa->setVisible(true);
                this->resultGame = *glastrs;
                if (this->resultGame == "" || this->resultGame == " ") return;
                vector<string> listRes = mUtils::splitString(this->resultGame, '_');
                if (listRes.size() == 3)
                {
                    this->setResultGame(listRes[0]);
                    this->setResultGame(listRes[1]);
                    this->setResultGame(listRes[2]);
                }
                this->mDiaNoc->loadTexture("dia_xocdia.png");
                for (int i = 0; i < listRes.size(); i++)
                {
                    string res = this->getResImageByBetType(listRes[i]);
                    this->arrResult[i]->setVisible(true);
                    this->arrResult[i]->loadTexture(res);
                }
            }
        }
        else {
            this->bgTimer->setVisible(true);
            this->CurrentGameState = true;
            this->schedule(schedule_selector(LayerGameBauTom::updateTimer),1);
        }
    }
    
    if (gbt != NULL) {
       // log("gbt = %s ", gbt->c_str());
        this->ReloadGameWhenReturn(*gbt);
    }
    this->SelectCoinValue(this->amfUserClient);
    SceneManager::getSingleton().hideLoading();

}

void LayerGameBauTom::event_EXT_EVENT_GAME_BET_NTF()
{
    boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
    boost::shared_ptr<string> gbv = this->param->GetUtfString(EXT_FIELD_GAME_BET_VALUE);
    boost::shared_ptr<string> gbt = this->param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
    boost::shared_ptr<long> gtbv = this->param->GetInt(EXT_FIELD_GAME_TOTAL_BET_VALUE);
    boost::shared_ptr<long> gtbvc = this->param->GetInt(EXT_FIELD_GAME_TOTAL_BET_VALUE_CELL);
    boost::shared_ptr<long> ttp = this->param->GetInt(EXT_FIELD_GAME_TOTAL_PLAYER);
    if (gbt != NULL && gbv != NULL && uid != NULL)
    {
        int betValue = atoi(gbv->c_str());
        if (this->myName.compare(uid->c_str()) == 0)
        {
            //this->updateBetInfoClient(gbt->c_str(), betValue, 0);
        }
        else{
            this->animationBetOther(betValue,gbt->c_str(),2);
            this->updateBetInfoClient(gbt->c_str(), betValue, 0);
            int totalBet = 0;
            int betval = 0;
            for (int i = 0; i < this->listBetInfoClient.size(); i++)
            {
                if (this->listBetInfoClient[i].bettype.compare(gbt->c_str()) == 0)
                {
                    totalBet = this->listBetInfoClient[i].totalbet;
                    betval = this->listBetInfoClient[i].userbet;
                    break;
                }
            }
            this->setMoneyBetBautom(totalBet, betval, gbt->c_str());
        }
    }
}

void LayerGameBauTom::event_EXT_EVENT_GAME_BET_ERROR_NOTIFY()
{
    boost::shared_ptr<long> errc = this->param->GetInt(EXT_FIELD_ERROR_CODE);
    boost::shared_ptr<string> bettypePtr = param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
    boost::shared_ptr<string> betvaluePtr = param->GetUtfString(EXT_FIELD_GAME_BET_VALUE);
    if (errc != NULL) {
       // log("bet errc = %ld", *errc);
        if (*errc == RESCODE_NOT_ENOUGH_MONEY) {
            SceneManager::getSingleton().showToast(dataManager.GetSysString(10));
        }
        else if (*errc == RESCODE_GAME_IS_ENDED)
        {
            SceneManager::getSingleton().showToast(dataManager.GetSysString(4));
            this->updateInfoMe();
        }
    }
    
    if (bettypePtr != NULL && betvaluePtr != NULL)
    {
        int betvalue = atoi(betvaluePtr->c_str());
        string bettype = bettypePtr->c_str();
        this->rollbackBetInfoClient(bettype, betvalue);
        this->rollbackBeInt(betvalue,bettype);
    }
}

void LayerGameBauTom::event_EXT_EVENT_OFFER_BOSS_NTF()
{
    this->removeChildByTag(TAG_OFFER);
    LayerPopupThongBao *popupThongBao = LayerPopupThongBao::create();
    popupThongBao->setMessage(dataManager.GetSysString(17));
    popupThongBao->setContentMess(dataManager.GetSysString(16));
    popupThongBao->setTag(TAG_OFFER);
    popupThongBao->setCallBackFromButton(this, callfuncO_selector(LayerGameBauTom::onPopupCallBack), 1);
    this->addChild(popupThongBao);
    this->btnCuopCai->setEnabled(true);
    this->btnCuopCai->setBright(true);
    this->canBeBoss = true;
}

void LayerGameBauTom::event_EXT_EVENT_REGISTER_BOSS_RES()
{
    //public static final String EXT_FIELD_ERROR_STRING = "eexp";
    //public static final String EXT_FIELD_ERROR_CODE = "errc";
    //public static final int RESCODE_NOT_ENOUGH_MONEY = 29;
    //public static final int RESCODE_GAME_IS_PLAYING=31;
    this->removeChildByTag(TAG_OFFER);
    boost::shared_ptr<long> rcPtr = this->param->GetInt(EXT_FIELD_ERROR_CODE);
    boost::shared_ptr<string> rdPtr = this->param->GetUtfString(EXT_FIELD_ERROR_STRING);
    if (rcPtr != NULL)
    {
        int rc = *rcPtr;
        if (rc == 0)
            this->btnCuopCai->setEnabled(false);
        if (rc == 29)
        {
            //SceneManager::getSingleton().showToast(dataManager.GetSysString(11));
        }
        else if (rc == 31)
        {
            this->btnCuopCai->setEnabled(true);
            //SceneManager::getSingleton().showToast("Game đang diễn ra");
        }
    }
    if (rdPtr != NULL)
    {
        SceneManager::getSingleton().showToast(rdPtr->c_str());
    }
}

void LayerGameBauTom::event_EXT_EVENT_NEXT_BOSS_NTF()
{
    boost::shared_ptr<string> infoNextCaiPtr = this->param->GetUtfString("uid");
    if (infoNextCaiPtr != NULL)
    {
        
        this->bossNext = infoNextCaiPtr->c_str();
        if (this->CurrentGameState == false)
        {
            double amfBoss = GameUtils::getAmfGoldUser(infoNextCaiPtr->c_str());
            if (SceneManager::getSingleton().getCurrRoomType() == 1)
                amfBoss = GameUtils::getAmfSilverUser(infoNextCaiPtr->c_str());
            this->lblBossMoney->setString(mUtils::convertMoneyEx(amfBoss));
            string urlAvatarCai = GameUtils::getAvatarUser(infoNextCaiPtr->c_str());
            this->updateInfoCai(infoNextCaiPtr->c_str());
        }
        SceneManager::getSingleton().showToast(StringUtils::format("%s %s",dataManager.GetSysString(14).c_str(), GameUtils::getNamePlayer(infoNextCaiPtr->c_str()).c_str()));
    }
}
void LayerGameBauTom::event_EXT_EVENT_LIST_WIN_NTF()
{
    boost::shared_ptr<string> listWinPtr = this->param->GetUtfString(EXT_FIELD_GAME_WIN_LIST);
    if (listWinPtr)
    {
        this->listwin = *listWinPtr;
    }
}

std::string LayerGameBauTom::getResImageByBetType(string bettype)
{
    if (bettype.compare("HUOU") == 0)
    {
        return "design/ResBautom/cop.png";
    }
    else if (bettype.compare("CA") == 0)
    {
        return "design/ResBautom/ca.png";
    }
    else if (bettype.compare("CUA") == 0)
    {
        return "design/ResBautom/cua.png";
    }
    else if (bettype.compare("GA") == 0)
    {
        return "design/ResBautom/ga.png";
    }
    else if (bettype.compare("TOM") == 0)
    {
        return "design/ResBautom/tom.png";
    }
    else if (bettype.compare("BAU") == 0)
    {
        return "design/ResBautom/bau.png";
    }
    return "default_tomcuaca.png";
}

void LayerGameBauTom::showAnimation(float dt)
{
    bool isSoundOn = mUtils::isSoundOn();
    if (isSoundOn)
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("xocdia.mp3", true);
    LayerAnimationXocDia *animate = LayerAnimationXocDia::create(Vec2(mDiaNoc->getPositionX(), mDiaNoc->getPositionY()), 2);
    animate->setCallBackListener(CC_CALLBACK_0(LayerGameBauTom::callBackAnimation2, this));
    this->pnlMain->addChild(animate, 399, TAG_ANIM_START_GAME);
    SceneManager::getSingleton().showToast(dataManager.GetSysString(12));
}
void LayerGameBauTom::callBackAnimation2()
{
    this->bgTimer->setVisible(true);
}
void LayerGameBauTom::callBackAnimation()
{
    //this->resultGame = "GA_CA_GA";
    vector<string> listRes = mUtils::splitString(this->resultGame, '_');
    this->timerHo->setVisible(true);
    this->timerBau->setVisible(true);
    this->timerCua->setVisible(true);
    this->timerTom->setVisible(true);
    this->timerCa->setVisible(true);
    this->timerGa->setVisible(true);
    
    this->spActiveHo->setVisible(true);
    this->spActiveBau->setVisible(true);
    this->spActiveCua->setVisible(true);
    this->spActiveTom->setVisible(true);
    this->spActiveCa->setVisible(true);
    this->spActiveGa->setVisible(true);
    
    if (listRes.size() == 3)
    {
        this->setResultGame(listRes[0]);
        this->setResultGame(listRes[1]);
        this->setResultGame(listRes[2]);
    }
    this->showAnimationThuTien();
    this->runAction(Sequence::create(DelayTime::create(1),CallFunc::create(CC_CALLBACK_0(LayerGameBauTom::showAnimationTraTien, this)), NULL));
    this->runAction(Sequence::create(DelayTime::create(2.5+this->listImgWin.size()*0.05),CallFunc::create(CC_CALLBACK_0(LayerGameBauTom::showAnimationTraTienUser, this)), NULL));
    
    this->runAction(Sequence::create(DelayTime::create(4.5+this->listImgWin.size()*0.05+0.05*this->listImgMeWin.size()),CallFunc::create(CC_CALLBACK_0(LayerGameBauTom::showMoneyWin, this)), NULL));
    
}

void LayerGameBauTom::setResultGame(string bettype)
{
    auto actionTimer = RepeatForever::create(ProgressFromTo::create(1, 100, 0));
    if (bettype.compare("HUOU") == 0)
    {
        this->countHuou++;
        if (this->countHuou == 2)
        {
            this->spX3Ho->setVisible(false);
            this->spX2Ho->setVisible(true);
        }
        else if (this->countHuou == 3)
        {
            this->spX3Ho->setVisible(true);
            this->spX2Ho->setVisible(false);
        }
        this->timerHo->setVisible(false);
        this->spActiveHo->setVisible(false);
    }
    else if (bettype.compare("CA") == 0)
    {
        this->countCa++;
        if (this->countCa == 2)
        {
            this->spX3Ca->setVisible(false);
            this->spX2Ca->setVisible(true);
        }
        else if (this->countCa == 3)
        {
            this->spX3Ca->setVisible(true);
            this->spX2Ca->setVisible(false);
        }
        this->timerCa->setVisible(false);
        this->spActiveCa->setVisible(false);
    }
    else if (bettype.compare("CUA") == 0)
    {
        
        this->countCua++;
        if (this->countCua == 2)
        {
            this->spX3Cua->setVisible(false);
            this->spX2Cua->setVisible(true);
        }
        else if (this->countCua == 3)
        {
            this->spX3Cua->setVisible(true);
            this->spX2Cua->setVisible(false);
        }
        this->spActiveCua->setVisible(false);
        this->timerCua->setVisible(false);
    }
    else if (bettype.compare("GA") == 0)
    {
        
        this->countGa++;
        if (this->countGa == 2)
        {
            this->spX3Ga->setVisible(false);
            this->spX2Ga->setVisible(true);
        }
        else if (this->countGa == 3)
        {
            this->spX3Ga->setVisible(true);
            this->spX2Ga->setVisible(false);
        }
        this->spActiveGa->setVisible(false);
        this->timerGa->setVisible(false);
        //this->timerGa->runAction(RepeatForever::create(actionTimer));
    }
    else if (bettype.compare("TOM") == 0)
    {
        
        this->countTom++;
        if (this->countTom == 2)
        {
            this->spX3Tom->setVisible(false);
            this->spX2Tom->setVisible(true);
        }
        else if (this->countTom == 3)
        {
            this->spX3Tom->setVisible(true);
            this->spX2Tom->setVisible(false);
        }
        this->spActiveTom->setVisible(false);
        this->timerTom->setVisible(false);
        //this->timerTom->runAction(RepeatForever::create(actionTimer));
    }
    else if (bettype.compare("BAU") == 0)
    {
        
        this->countBau++;
        if (this->countBau == 2)
        {
            this->spX3Bau->setVisible(false);
            this->spX2Bau->setVisible(true);
        }
        else if (this->countBau == 3)
        {
            this->spX3Bau->setVisible(true);
            this->spX2Bau->setVisible(false);
        }
        this->spActiveBau->setVisible(false);
        this->timerBau->setVisible(false);
        //this->timerBau->runAction(RepeatForever::create(actionTimer));
    }
}
void LayerGameBauTom::showAnimationTraTien(){
    
    
    if (this->listImgMove.size()<=0){
        Sprite* a = Sprite::create("ResBautom/500.png");
        this->pnlMain->addChild(a);
        this->listImgMove.push_back(a);
        a->setVisible(false);
        int randomY = rand() % 80;
        a->setPosition(Vec2(682,582+randomY));
    }
    
    vector<string> listRes = mUtils::splitString(this->resultGame, '_');
    for (int i = 0;i< listRes.size(); i++){
        if (listRes[i]=="TOM"){
            Vec2 orginPos = this->btnTom->getPosition();
            Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            
            for (int j = 0; j < this->listImgTom.size();j++){
                int randomRotate = rand() % 360;
                int itemhide = rand() % this->listImgMove.size();
                
                if (this->listImgMove.size()>0)
                    this->listImgMove[itemhide]->setVisible(false);
                Sprite* a = Sprite::create(this->listImgTom[j]->getResourceName());
                a->setRotation(randomRotate);
                a->setTag(this->listImgTom[j]->getTag());
                a->setLocalZOrder(101);
                a->setPosition(this->listImgMove[itemhide]->getPosition());
                this->pnlMain->addChild(a);
                a->runAction(Sequence::create(DelayTime::create(j*0.05),MoveTo::create(0.4, Vec2(randX,randY)),NULL));
                this->listImgWin.push_back(a);
            }
        }
        if (listRes[i]=="CUA"){
            Vec2 orginPos = this->btnCua->getPosition();
            Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            
            for (int j = 0; j < this->listImgCua.size();j++){
                int randomRotate = rand() % 360;
                int itemhide = rand() % this->listImgMove.size();
                
                if (this->listImgMove.size()>0)
                    this->listImgMove[itemhide]->setVisible(false);
                Sprite* a = Sprite::create(this->listImgCua[j]->getResourceName());
                a->setRotation(randomRotate);
                a->setTag(this->listImgCua[j]->getTag());
                
                a->setLocalZOrder(101);
                a->setPosition(this->listImgMove[itemhide]->getPosition());
                this->pnlMain->addChild(a);
                a->runAction(Sequence::create(DelayTime::create(j*0.05),MoveTo::create(0.4, Vec2(randX,randY)),NULL));
                this->listImgWin.push_back(a);
            }
        }
        if (listRes[i]=="CA"){
            Vec2 orginPos = this->btnCa->getPosition();
            Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            
            for (int j = 0; j < this->listImgCa.size();j++){
                int randomRotate = rand() % 360;
                int itemhide = rand() % this->listImgMove.size();
                
                if (this->listImgMove.size()>0)
                    this->listImgMove[itemhide]->setVisible(false);
                Sprite* a = Sprite::create(this->listImgCa[j]->getResourceName());
                a->setRotation(randomRotate);
                a->setTag(this->listImgCa[j]->getTag());
                
                a->setLocalZOrder(101);
                a->setPosition(this->listImgMove[itemhide]->getPosition());
                this->pnlMain->addChild(a);
                a->runAction(Sequence::create(DelayTime::create(j*0.05),MoveTo::create(0.4, Vec2(randX,randY)),NULL));
                this->listImgWin.push_back(a);
            }
        }
        if (listRes[i]=="GA"){
            Vec2 orginPos = this->btnGa->getPosition();
            Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            
            for (int j = 0; j < this->listImgGa.size();j++){
                int randomRotate = rand() % 360;
                int itemhide = rand() % this->listImgMove.size();
                
                if (this->listImgMove.size()>0)
                    this->listImgMove[itemhide]->setVisible(false);
                Sprite* a = Sprite::create(this->listImgGa[j]->getResourceName());
                a->setRotation(randomRotate);
                a->setTag(this->listImgGa[j]->getTag());
                a->setLocalZOrder(101);
                a->setPosition(this->listImgMove[itemhide]->getPosition());
                this->pnlMain->addChild(a);
                a->runAction(Sequence::create(DelayTime::create(j*0.05),MoveTo::create(0.4, Vec2(randX,randY)),NULL));
                this->listImgWin.push_back(a);
            }
        }
        if (listRes[i]=="BAU"){
            Vec2 orginPos = this->btnBau->getPosition();
            Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            
            for (int j = 0; j < this->listImgBau.size();j++){
                int randomRotate = rand() % 360;
                int itemhide = rand() % this->listImgMove.size();
                
                if (this->listImgMove.size()>0)
                    this->listImgMove[itemhide]->setVisible(false);
                Sprite* a = Sprite::create(this->listImgBau[j]->getResourceName());
                a->setRotation(randomRotate);
                a->setLocalZOrder(101);
                a->setTag(this->listImgBau[j]->getTag());
                
                a->setPosition(this->listImgMove[itemhide]->getPosition());
                this->pnlMain->addChild(a);
                a->runAction(Sequence::create(DelayTime::create(j*0.05),MoveTo::create(0.4, Vec2(randX,randY)),NULL));
                this->listImgWin.push_back(a);
            }
        }
        if (listRes[i]=="HUOU"){
            Vec2 orginPos = this->btnHo->getPosition();
            Vec2 startPos = Vec2(orginPos.x-60,orginPos.y-60);
            int randX = startPos.x +rand() % 100;
            int randY = startPos.y +rand() % 30;
            
            for (int j = 0; j < this->listImgHo.size();j++){
                int randomRotate = rand() % 360;
                int itemhide = rand() % this->listImgMove.size();
                
                if (this->listImgMove.size()>0)
                    this->listImgMove[itemhide]->setVisible(false);
                
                Sprite* a = Sprite::create(this->listImgHo[j]->getResourceName());
                a->setRotation(randomRotate);
                a->setTag(this->listImgHo[j]->getTag());
                
                a->setLocalZOrder(101);
                a->setPosition(this->listImgMove[itemhide]->getPosition());
                this->pnlMain->addChild(a);
                a->runAction(Sequence::create(DelayTime::create(j*0.05),MoveTo::create(0.4, Vec2(randX,randY)),NULL));
                this->listImgWin.push_back(a);
            }
        }
    }
}
void LayerGameBauTom::showMoneyWin(){
    for (int i = 0; i < this->listImgMeWin.size(); i++){
        this->listImgMeWin[i]->setVisible(false);
    }
    string money = mUtils::convertMoneyEx(this->totalBetEat);
    this->lblBetEat->setString(money);
    if (this->totalBetEat > 0)
    {
        string font_name = "font_win-font.fnt";
        Label* labelResult = Label::createWithBMFont(font_name, StringUtils::format("+%s", money.c_str()), TextHAlignment::CENTER);
        labelResult->setScale(0.8);
        labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        labelResult->setPosition(this->nodeAvatarPlayer->getPosition());
        this->pnlMain->addChild(labelResult);
        labelResult->runAction(Sequence::create(
                                                MoveBy::create(1, Vec2(0, labelResult->getContentSize().height + 10)),
                                                DelayTime::create(2),
                                                FadeOut::create(0.2), RemoveSelf::create(true), nullptr));
    }
    if (this->currTypeMoney == 0){
        if (this->totalBetEat > dataManager.getAppConfig().notifyZen){
            
            string msg = "";
            msg = StringUtils::format("%s|Kla Klouk|%s|%s|%s|%s|%s/%s|Kla Klouk|%s|%s|%s|%s|%s/%s|Fish Prawn Crab|%s|%s|%s|%s|%s/%s|螃蟹|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(983).c_str(),mUtils::convertMoneyEx(this->totalBetEat).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9811).c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9831).c_str(),mUtils::convertMoneyEx(this->totalBetEat).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9812).c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9832).c_str(),mUtils::convertMoneyEx(this->totalBetEat).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9813).c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9833).c_str(),mUtils::convertMoneyEx(this->totalBetEat).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str());
            SceneManager::getSingleton().sendEventNotify(msg);
            
        }
    }else{
        if (this->totalBetEat > dataManager.getAppConfig().notifyGold){
            
            string msg = "";
            msg = StringUtils::format("%s|Kla Klouk|%s|%s|%s|%s|%s/%s|Kla Klouk|%s|%s|%s|%s|%s/%s|Fish Prawn Crab|%s|%s|%s|%s|%s/%s|螃蟹|%s|%s|%s|%s|%s",dataManager.GetSysString(981).c_str(),dataManager.GetSysString(982).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(983).c_str(),mUtils::convertMoneyEx(this->totalBetEat).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9811).c_str(),dataManager.GetSysString(9821).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9831).c_str(),mUtils::convertMoneyEx(this->totalBetEat).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9812).c_str(),dataManager.GetSysString(9822).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9832).c_str(),mUtils::convertMoneyEx(this->totalBetEat).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str(),dataManager.GetSysString(9813).c_str(),dataManager.GetSysString(9823).c_str(),GameUtils::getNamePlayer(SceneManager::getSingleton().getMyName()).c_str(),dataManager.GetSysString(9833).c_str(),mUtils::convertMoneyEx(this->totalBetEat).c_str(),SceneManager::getSingleton().getCoin(this->currTypeMoney).c_str());
            SceneManager::getSingleton().sendEventNotify(msg);
        }
    }
    vector<string> listWin = mUtils::splitString(this->listwin, ',');
    for (int i = 0; i < listWin.size(); i++)
    {
        for (int j = 0; j < this->listAvatar.size(); j++)
        {
            if (this->listAvatar[j]->getName().compare(listWin[i]) == 0)
            {
                this->listAvatar[j]->showAnimationWin(true);
            }
        }
        
    }
    
    for (int i = 0; i < listWin.size(); i++)
    {
        if (listWin[i].compare(this->myName) == 0)
        {
            this->avatarMe->showAnimationWin(true);
            break;
        }
    }
    
    this->updateInfoMe();
    
}
void LayerGameBauTom::showAnimationTraTienUser(){
    vector<string> listRes = mUtils::splitString(this->resultGame, '_');
    for (int k = 0;k<listRes.size();k++){
        if (listRes[k] == "BAU"){
            if (this->listImgMeBau.size()>0){
                if (this->listImgMeWin.size() < this->listImgMeBau.size()){
                    for (int i = 0; i < this->listImgMeBau.size();i++){
                        this->listImgMeBau[i]->runAction(Sequence::create(DelayTime::create(i*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                        this->listImgMeWin.push_back(this->listImgMeBau[i]);
                    }
                }
                
                int i = 0;
                while (i<listImgMeBau.size()) {
                    if (i==listImgMeBau.size())
                        break;
                    for (int j= 0; j< this->listImgWin.size();j++){
                        if (this->listImgWin[j]->getTag() == this->listImgMeBau[i]->getTag() && this->listImgWin[j]->getPositionX()>this->nodeAvatarPlayer->getPositionX()){
                            this->listImgWin[j]->runAction(Sequence::create(DelayTime::create((i+1)*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                            this->listImgMeWin.push_back(this->listImgWin[j]);
                            this->listImgWin.erase(this->listImgWin.begin()+j);
                            i++;
                            break;
                        }
                    }
                    i++;
                }
            }
        }
        if (listRes[k] == "CUA"){
            if (this->listImgMeCua.size()>0){
                if (this->listImgMeWin.size() < this->listImgMeCua.size()){
                    for (int i = 0; i < this->listImgMeCua.size();i++){
                        this->listImgMeCua[i]->runAction(Sequence::create(DelayTime::create(i*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                        this->listImgMeWin.push_back(this->listImgMeCua[i]);
                    }
                }
                
                int i = 0;
                while (i<listImgMeCua.size()) {
                    if (i==listImgMeCua.size())
                        break;
                    for (int j= 0; j< this->listImgWin.size();j++){
                        if (this->listImgWin[j]->getTag() == this->listImgMeCua[i]->getTag() && this->listImgWin[j]->getPositionX()>this->nodeAvatarPlayer->getPositionX()){
                            this->listImgWin[j]->runAction(Sequence::create(DelayTime::create((i+1)*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                            this->listImgMeWin.push_back(this->listImgWin[j]);
                            this->listImgWin.erase(this->listImgWin.begin()+j);
                            i++;
                            break;
                        }
                    }
                    i++;

                }
            }
        }
        if (listRes[k] == "TOM"){
            if (this->listImgMeTom.size()>0){
                if (this->listImgMeWin.size() < this->listImgMeTom.size()){
                    for (int i = 0; i < this->listImgMeTom.size();i++){
                        this->listImgMeTom[i]->runAction(Sequence::create(DelayTime::create(i*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                        this->listImgMeWin.push_back(this->listImgMeTom[i]);
                    }
                }
                
                int i = 0;
                while (i<listImgMeTom.size()) {
                    if (i==listImgMeTom.size())
                        break;
                    for (int j= 0; j< this->listImgWin.size();j++){
                        if (this->listImgWin[j]->getTag() == this->listImgMeTom[i]->getTag() && this->listImgWin[j]->getPositionX()>this->nodeAvatarPlayer->getPositionX()){
                            this->listImgWin[j]->runAction(Sequence::create(DelayTime::create((i+1)*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                            this->listImgMeWin.push_back(this->listImgWin[j]);
                            this->listImgWin.erase(this->listImgWin.begin()+j);
                            i++;
                            break;
                        }
                    }
                    i++;
                }
            }
        }
        if (listRes[k] == "CA"){
            if (this->listImgMeCa.size()>0){
                if (this->listImgMeWin.size() < this->listImgMeCa.size()){
                    for (int i = 0; i < this->listImgMeCa.size();i++){
                        this->listImgMeCa[i]->runAction(Sequence::create(DelayTime::create(i*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                        this->listImgMeWin.push_back(this->listImgMeCa[i]);
                    }
                }
                
                int i = 0;
                while (i<listImgMeCa.size()) {
                    if (i==listImgMeCa.size())
                        break;
                    for (int j= 0; j< this->listImgWin.size();j++){
                        if (this->listImgWin[j]->getTag() == this->listImgMeCa[i]->getTag() && this->listImgWin[j]->getPositionX()>this->nodeAvatarPlayer->getPositionX()){
                            this->listImgWin[j]->runAction(Sequence::create(DelayTime::create((i+1)*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                            this->listImgMeWin.push_back(this->listImgWin[j]);
                            this->listImgWin.erase(this->listImgWin.begin()+j);
                            i++;
                            break;
                        }
                    }
                    i++;

                }
            }
        }
        if (listRes[k] == "GA"){
            if (this->listImgMeGa.size()>0){
                if (this->listImgMeWin.size() < this->listImgMeGa.size()){
                    for (int i = 0; i < this->listImgMeGa.size();i++){
                        this->listImgMeGa[i]->runAction(Sequence::create(DelayTime::create(i*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                        this->listImgMeWin.push_back(this->listImgMeGa[i]);
                    }
                }
                
                int i = 0;
                while (i<listImgMeGa.size()) {
                    if (i==listImgMeGa.size())
                        break;
                    for (int j= 0; j< this->listImgWin.size();j++){
                        if (this->listImgWin[j]->getTag() == this->listImgMeGa[i]->getTag() && this->listImgWin[j]->getPositionX()>this->nodeAvatarPlayer->getPositionX()){
                            this->listImgWin[j]->runAction(Sequence::create(DelayTime::create((i+1)*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                            this->listImgMeWin.push_back(this->listImgWin[j]);
                            this->listImgWin.erase(this->listImgWin.begin()+j);
                            i++;
                            break;
                        }
                    }
                    i++;

                }
            }
        }
        if (listRes[k] == "HUOU"){
            if (this->listImgMeHo.size()>0){
                if (this->listImgMeWin.size() < this->listImgMeHo.size()){
                    for (int i = 0; i < this->listImgMeHo.size();i++){
                        this->listImgMeHo[i]->runAction(Sequence::create(DelayTime::create(i*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                        this->listImgMeWin.push_back(this->listImgMeHo[i]);
                    }
                }
                
                int i = 0;
                while (i<listImgMeHo.size()) {
                    if (i==listImgMeHo.size())
                        break;
                    for (int j= 0; j< this->listImgWin.size();j++){
                        if (this->listImgWin[j]->getTag() == this->listImgMeHo[i]->getTag() && this->listImgWin[j]->getPositionX()>this->nodeAvatarPlayer->getPositionX()){
                            this->listImgWin[j]->runAction(Sequence::create(DelayTime::create((i+1)*0.05),MoveTo::create(0.25, this->nodeAvatarPlayer->getPosition()), NULL));
                            this->listImgMeWin.push_back(this->listImgWin[j]);
                            this->listImgWin.erase(this->listImgWin.begin()+j);
                            i++;
                            break;
                        }
                    }
                    i++;

                }
            }
        }
    }
}
void LayerGameBauTom::showAnimationThuTien(){
    //bettype ="CUA";
    for (int i = 0; i< this->listImgMoney.size();i++){
        string tag = StringUtils::format("%d",this->listImgMoney[i]->getTag());
        if (!this->checkImgWin(tag)){
            int randomY = rand() % 80;
            this->listImgMoney[i]->runAction(MoveTo::create(0.4, Vec2(682,582+randomY)));
            this->listImgMove.push_back(this->listImgMoney[i]);
        }
    }
}
bool LayerGameBauTom::checkImgWin(string tag){
    vector<string> listRes = mUtils::splitString(this->resultGame, '_');
    bool isWin = false;
    for (int i = 0;i<listRes.size();i++){
        if (StringUtils::format("%d",this->getTypeByBet(listRes[i])).compare(&tag.back()) == 0){
            isWin = true;
        }
    }
    return isWin;
}
int LayerGameBauTom::getTypeByBet(string tag){
    int type = 1;
    string bettype = tag;
    vector<Sprite*> curList;
    if (bettype == "TOM"){
        type = 1;
        curList = this->listImgTom;
    }
    if (bettype == "CUA"){
        type = 2;
        curList = this->listImgCua;
        
    }
    if (bettype == "CA"){
        type = 3;
        curList = this->listImgCa;
        
    }
    if (bettype == "GA"){
        type = 4;
        curList = this->listImgGa;
        
    }
    if (bettype == "BAU"){
        type = 5;
        curList = this->listImgBau;
        
    }
    if (bettype == "HUOU"){
        type = 6;
        curList = this->listImgHo;
        
    }
    return type;
}
void LayerGameBauTom::resetButtonBautom()
{
    this->countBau = 0;
    this->countCa = 0;
    this->countCua = 0;
    this->countGa = 0;
    this->countHuou = 0;
    this->countTom = 0;
    
    this->spActiveHo->setVisible(false);
    this->spActiveHo->stopAllActions();
    this->spX2Ho->setVisible(false);
    this->spX3Ho->setVisible(false);
    
    this->spActiveBau->setVisible(false);
    this->spActiveBau->stopAllActions();
    this->spX2Bau->setVisible(false);
    this->spX3Bau->setVisible(false);
    
    this->spActiveCua->setVisible(false);
    this->spActiveCua->stopAllActions();
    this->spX2Cua->setVisible(false);
    this->spX3Cua->setVisible(false);
    
    this->spActiveGa->setVisible(false);
    this->spActiveGa->stopAllActions();
    this->spX2Ga->setVisible(false);
    this->spX3Ga->setVisible(false);
    
    this->spActiveTom->setVisible(false);
    this->spActiveTom->stopAllActions();
    this->spX2Tom->setVisible(false);
    this->spX3Tom->setVisible(false);
    
    this->spActiveCa->setVisible(false);
    this->spActiveCa->stopAllActions();
    this->spX2Ca->setVisible(false);
    this->spX3Ca->setVisible(false);
    
    this->timerGa->stopAllActions();
    this->timerGa->setVisible(false);
    this->timerTom->stopAllActions();
    this->timerTom->setVisible(false);
    this->timerBau->stopAllActions();
    this->timerBau->setVisible(false);
    this->timerCa->stopAllActions();
    this->timerCa->setVisible(false);
    this->timerHo->stopAllActions();
    this->timerHo->setVisible(false);
    this->timerCua->stopAllActions();
    this->timerCua->setVisible(false);
}

void LayerGameBauTom::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    
}

cocos2d::Size LayerGameBauTom::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(260, 65);
}

cocos2d::extension::TableViewCell* LayerGameBauTom::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    string res = this->listHis[idx];
    int _cTag = 123;
    Size sizeItem = Size(350, 220);
    int cols = 3;
    if (!cell) {
        cell = new TableViewCell();
        cell->setAnchorPoint(Vec2(0, 0));
        cell->autorelease();
        
    }
    else
    {
        cell->removeAllChildrenWithCleanup(true);
    }
    
    LayerItemHis *itemHis = LayerItemHis::create();
    itemHis->setAnchorPoint(Vec2::ZERO);
    itemHis->setPosition(Vec2::ZERO);
    itemHis->setDataDice(idx+1,res);
    itemHis->setTag(_cTag + idx);
    cell->addChild(itemHis);
    return cell;
}

ssize_t LayerGameBauTom::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return this->listHis.size();
}

void LayerGameBauTom::hidePanelHis(bool _isHide)
{
    this->isHide = _isHide;
    float desPos = this->firstPosHis.x;
    if (_isHide)
        desPos = this->firstPosHis.x - 600;
    MoveTo* actionMove = MoveTo::create(0.3, Vec2(desPos, this->firstPosHis.y));
    this->pnlHis->runAction(actionMove);
}

void LayerGameBauTom::onButtonChat(Ref* pSender)
{
    this->layerChatInGame->setVisible(true);
}

void LayerGameBauTom::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueSender = (*ptrEventParams)["sender"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueSender);
    boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
    boost::shared_ptr<string> ptrNotifiedMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);
    if (dataManager.getAppConfig().is24h)
    {
        return;
    }
    //
    if (ptrNotifiedMessage == NULL || ptrNotifiedUser == NULL) return;
   // log("ptrNotifiedMessage: %s", ptrNotifiedMessage->c_str());
    this->showChatByName(ptrNotifiedUser->Name()->c_str(), ptrNotifiedMessage->c_str());
}

void LayerGameBauTom::showChatByName(string nameChat, string message)
{
    if (nameChat.compare(this->myName) == 0)
    {
        this->avatarMe->showChat(message);
    }
    else{
        for (int i = 0; i < this->listAvatar.size(); i++)
        {
            if (this->listAvatar[i]->getName().compare(nameChat) == 0)
            {
                this->listAvatar[i]->showChat(message);
            }
        }
    }
}

void LayerGameBauTom::onPopupCallBack(Ref* pSender)
{
    int tagCB = ((Node*)pSender)->getTag();
    switch (tagCB)
    {
        case 1:
        {
            boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
            if (lastRoom == NULL)
                return;
            boost::shared_ptr<SFSObject> params(new SFSObject());
            boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_REGISTER_BOSS_REG, params, lastRoom));
            GameServer::getSingleton().Send(req2);
        }
            break;
        default:
            break;
    }
    this->removeChildByTag(TAG_OFFER);
}

void LayerGameBauTom::avatarCaiCallBack()
{
    if (this->bossName.compare("") == 0)
        return;
    
    string userAvatar = GameUtils::getAvatarUser(this->bossName);
    string userIp = GameUtils::getUserIP(this->bossName);
    auto layerInfo = LayerPopupInfo::create();
    layerInfo->setFriendID(this->bossName);
    layerInfo->setUserIP(userIp);
    layerInfo->setAvatarUrl(userAvatar);
    layerInfo->setIsBossRoom(false);//bo nut chu phong
    layerInfo->reloadAllDatas();
    layerInfo->setIsPlaying(GameUtils::isStartedGame());
    this->addChild(layerInfo);
}

void LayerGameBauTom::avatarMeCallBack()
{
    string userAvatar = GameUtils::getAvatarUser(this->myName);
    string userIp = GameUtils::getUserIP(this->myName);
    auto layerInfo = LayerPopupInfo::create();
    layerInfo->setFriendID(this->myName);
    layerInfo->setUserIP(userIp);
    layerInfo->setAvatarUrl(userAvatar);
    layerInfo->setIsBossRoom(false);//bo nut chu phong
    layerInfo->reloadAllDatas();
    layerInfo->setIsPlaying(GameUtils::isStartedGame());
    this->addChild(layerInfo);
}

void LayerGameBauTom::OnSmartFoxUserCountChange(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (room != NULL)
    {
        boost::shared_ptr<RoomVariable> listUserPtr = room->GetVariable(EXT_FIELD_LIST_USER);
        if (listUserPtr)
        {
            this->listUserPlay = listUserPtr->GetStringValue()->c_str();
            this->updateListUser();
        }
    }
}

void LayerGameBauTom::onButtonDsCai(Ref* pSender)
{
    LayerPopupListCai* popupCai = LayerPopupListCai::create();
    if (this->canBeBoss == false)
        popupCai->disableButtonRegister();
    this->addChild(popupCai);
}

void LayerGameBauTom::avatarUserCallBack(CircleAvatar* sender, string callback)
{
    string userAvatar = GameUtils::getAvatarUser(callback);
    string userIp = GameUtils::getUserIP(callback);
    auto layerInfo = LayerPopupInfo::create();
    layerInfo->setFriendID(callback);
    layerInfo->setUserIP(userIp);
    layerInfo->setAvatarUrl(userAvatar);
    layerInfo->setIsBossRoom(false);//bo nut chu phong
    layerInfo->reloadAllDatas();
    layerInfo->setIsPlaying(GameUtils::isStartedGame());
    this->addChild(layerInfo);
}

ProgressTimer* LayerGameBauTom::createTimerFromSprite(Sprite *inputSprite)
{
    ProgressTimer* outputTimer = ProgressTimer::create(inputSprite);
    outputTimer->setAnchorPoint(inputSprite->getAnchorPoint());
    outputTimer->setPosition(inputSprite->getPosition());
    outputTimer->setPercentage(0);
    outputTimer->setType(ProgressTimer::Type::RADIAL);
    
    return outputTimer;
}

void LayerGameBauTom::updateInfoCai(string _bossName)
{
    this->bossName = _bossName;
    double amfBoss = GameUtils::getAmfGoldUser(this->bossName);
    if (SceneManager::getSingleton().getCurrRoomType() == 1)
        amfBoss = GameUtils::getAmfSilverUser(this->bossName);
    if (amfBoss == 0)
    {
        this->lblBossMoney->setString(dataManager.GetSysString(15));
    }
    else{
        this->lblBossMoney->setString(mUtils::convertMoneyEx(amfBoss));
    }
    string urlAvatarCai = GameUtils::getAvatarUser(this->bossName);
    this->avatarCai->setAvatarImage(urlAvatarCai);
    
    if (this->myName.compare(this->bossName) == 0)
    {
        this->meIsBoss = true;
        this->canBeBoss = true;
    }
    else
    {
        this->meIsBoss = false;
        this->canBeBoss = false;
    }
}

void LayerGameBauTom::onButtonCoin(Ref* pSender)
{
    SceneManager::getSingleton().showPopupMoneyCharge();
}

LayerItemHis::LayerItemHis()
{
    
}

LayerItemHis::~LayerItemHis()
{
    
}

bool LayerItemHis::init()
{
    if (!Layer::init())
        return false;
    auto root = CSLoader::getInstance()->createNode("LayerItemHis.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    this->setContentSize(root->getContentSize());
    this->setAnchorPoint(Vec2::ZERO);
    auto pnlBg =  (Layout*)root->getChildByName("pnlBg");
    pnlBg->setSwallowTouches(false);
    this->img1 = (ImageView*)pnlBg->getChildByName("img1");
    this->img2 = (ImageView*)pnlBg->getChildByName("img2");
    this->img3 = (ImageView*)pnlBg->getChildByName("img3");
    this->imghis = (ImageView*)pnlBg->getChildByName("imghis");
    this->txtSTT = (Text*)pnlBg->getChildByName("txtSTT");
    return true;
}

void LayerItemHis::setDataDice(int idx,string _result)
{
    vector<string> listRes = mUtils::splitString(_result, '_');
    if (listRes.size()<=0) return;
   // log("RES IMAGE = %s", _result.c_str());
    //log("Res get = %s/%s/%s", listRes[0].c_str(), listRes[1].c_str(), listRes[2].c_str());
    this->img1->loadTexture(this->getResImageByBetType(listRes[0]));
    this->img2->loadTexture(this->getResImageByBetType(listRes[1]));
    this->img3->loadTexture(this->getResImageByBetType(listRes[2]));
    if (idx == 1){
        this->imghis->loadTexture("ResBautom/bg-hisphiennow.png");
    }else{
        this->imghis->loadTexture("ResBautom/bg-hisphien.png");
    }
    this->txtSTT->setString(StringUtils::format("#%d",idx));
}
std::string LayerItemHis::getResImageByBetType(string bettype)
{
    if (bettype.compare("HUOU") == 0)
    {
        return "design/ResBautom/ho-his.png";
    }
    else if (bettype.compare("CA") == 0)
    {
        return "design/ResBautom/ca-his.png";
    }
    else if (bettype.compare("CUA") == 0)
    {
        return "design/ResBautom/cua-his.png";
    }
    else if (bettype.compare("GA") == 0)
    {
        return "design/ResBautom/ga-his.png";
    }
    else if (bettype.compare("TOM") == 0)
    {
        return "design/ResBautom/tom-his.png";
    }
    else if (bettype.compare("BAU") == 0)
    {
        return "design/ResBautom/bau-his.png";
    }
    return "";
}

LayerItemListCai::LayerItemListCai()
{
    
}

LayerItemListCai::~LayerItemListCai()
{
    
}

bool LayerItemListCai::init()
{
    if (!Layer::init())
        return false;
    auto root = CSLoader::getInstance()->createNode("LayerItemListCai.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    this->setContentSize(root->getContentSize());
    this->setAnchorPoint(Vec2::ZERO);
    
    this->lblName = (Text*)root->getChildByName("lblName");
    this->lblStatus = (Text*)root->getChildByName("lblStatus");
    this->lblTitle = (Text*)root->getChildByName("lblTitle");
    
    return true;
}

void LayerItemListCai::setData(string _username, int idx)
{
    string _statusStr = dataManager.GetSysString(569);
    Color3B colorStatus = Color3B::WHITE;
    if (idx == 0)
    {
        _statusStr = dataManager.GetSysString(570);
        colorStatus = Color3B(255, 216, 0);
    }
    this->lblStatus->setString(_statusStr);
    this->lblStatus->setColor(colorStatus);
    this->lblName->setString(GameUtils::getNamePlayer(_username));
    this->lblTitle->setString(StringUtils::format("%d", idx + 1));
}

LayerPopupListCai::LayerPopupListCai()
{
    this->isCanBeBoss = true;
    GameServer::getSingleton().addListeners(this);
}

LayerPopupListCai::~LayerPopupListCai()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupListCai::init()
{
    if (!Layer::init())
        return false;
    auto root = CSLoader::getInstance()->createNode("LayerPopupListCai.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    this->setContentSize(root->getContentSize());
    this->setAnchorPoint(Vec2::ZERO);
    
    Layout* pnlMain = (Layout*)root->getChildByName("pnlMain");
    auto Text_1 = static_cast<Text*>(pnlMain->getChildByName("Text_1"));
    Text_1->setString(dataManager.GetSysString(17));
    auto title_1 = static_cast<Sprite*>(pnlMain->getChildByName("title_1"));
    title_1->setTexture(StringUtils::format("design/ResBautom/%stitle.png",SceneManager::getSingleton().getFolderLang().c_str()));
    this->btnRegister = (Button*)pnlMain->getChildByName("btnAccept");
    this->btnRegister->setTitleText(dataManager.GetSysString(889));
    
    Button* btnCancel = (Button*)pnlMain->getChildByName("btnCancel");
    btnCancel->setTitleText(dataManager.GetSysString(890));
    Layout* pnlTable = (Layout*)pnlMain->getChildByName("pnlTable");
    
    this->tableViewHis = TableView::create(this, pnlTable->getContentSize());
    this->tableViewHis->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tableViewHis->setPosition(Vec2(0, 0));
    this->tableViewHis->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    this->tableViewHis->setTag(37);//table pickroom
    pnlTable->addChild(this->tableViewHis);
    this->tableViewHis->reloadData();
    this->tableViewHis->setDelegate(this);
    
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom != NULL)
    {
        boost::shared_ptr<SFSObject> params(new SFSObject());
        boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_GET_LIST_BOSS_REG, params, lastRoom));
        GameServer::getSingleton().Send(req2);
    }
    
    this->btnRegister->addClickEventListener(CC_CALLBACK_1(LayerPopupListCai::onButtonRegisterBoss, this));
    btnCancel->addClickEventListener(CC_CALLBACK_1(LayerPopupListCai::onButtonClose, this));
    
    return true;
}

void LayerPopupListCai::onButtonRegisterBoss(Ref* pSender)
{
    if (this->isCanBeBoss == false)
    {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(13));
        return;
    }
    
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    boost::shared_ptr<SFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_REGISTER_BOSS_REG, params, lastRoom));
    GameServer::getSingleton().Send(req2);
    this->removeFromParentAndCleanup(true);
}

void LayerPopupListCai::onButtonClose(Ref* pSender)
{
    this->removeFromParent();
}

void LayerPopupListCai::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    if (ptrEvetnParams == NULL) return;
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    if (ptrEventParamValueCmd == NULL) return;
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    if (cmd == NULL) return;
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    if (ptrEventParamValueParams == NULL)
        return;
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    if (param == NULL) return;
    if (cmd->compare(EXT_EVENT_GET_LIST_BOSS_RES) == 0)
    {
        //public static final String EXT_FIELD_GAME_BOSS_LIST = "bl";
        boost::shared_ptr<string> bosslistPtr = param->GetUtfString("bl");
        if (bosslistPtr != NULL)
        {
            this->listCai.clear();
            vector<string> listBoss = mUtils::splitString(*bosslistPtr, ',');
            for (int i = 0; i < listBoss.size(); i++)
            {
                if (listBoss[i].compare(" ") == 0)
                    continue;
                this->listCai.push_back(listBoss[i]);
            }
            
            this->tableViewHis->reloadData();
        }
    }
    else if (0 == cmd->compare(EXT_EVENT_OFFER_BOSS_NTF)){
        this->isCanBeBoss = true;
    }
}

void LayerPopupListCai::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    
}

cocos2d::Size LayerPopupListCai::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(1470, 150);
}

cocos2d::extension::TableViewCell* LayerPopupListCai::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    string res = this->listCai[idx];
    int _cTag = 123;
    Size sizeItem = Size(350, 220);
    int cols = 3;
    if (!cell) {
        cell = new TableViewCell();
        cell->setAnchorPoint(Vec2(0, 0));
        cell->autorelease();
        cell->setTag(_cTag);
        
        LayerItemListCai *itemHis = LayerItemListCai::create();
        itemHis->setAnchorPoint(Vec2::ZERO);
        itemHis->setPosition(Vec2::ZERO);
        itemHis->setData(res, idx);
        itemHis->setTag(_cTag + idx);
        cell->addChild(itemHis);
    }
    else
    {
        LayerItemListCai* itemHis = (LayerItemListCai*)cell->getChildByTag(_cTag + idx);
        if (itemHis)
        {
            itemHis->setData(res, idx);
        }
    }
    
    
    return cell;
}

ssize_t LayerPopupListCai::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return this->listCai.size();
}

void LayerPopupListCai::disableButtonRegister()
{
    this->isCanBeBoss = false;
}
