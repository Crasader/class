//
//  BaiMauBinh.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 4/20/17.
//
//

#include "BaiMauBinh.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../mUtils.h"
#include "LayerBoBaiMauBinh.h"
#include "AvatarInGame52La.h"
#include "MauBinhLayerXepBai.h"
#include "LayerGameMauBinh.h"
#include "Avatar52La.h"
using namespace cocos2d::ui;

BaiMauBinh::BaiMauBinh()
{
}


BaiMauBinh::~BaiMauBinh()
{
}

bool BaiMauBinh::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    this->rootNode = CSLoader::getInstance()->createNode("BaiMauBinh.csb");
    this->setContentSize(this->rootNode->getContentSize());
    ui::Helper::doLayout(this->rootNode);
    
    this->addChild(this->rootNode);
    
    
    //Chi 1
    ImageView* chi1_1 = static_cast<ImageView*>(this->rootNode->getChildByName("chi1_1"));
    ImageView* chi1_2 = static_cast<ImageView*>(this->rootNode->getChildByName("chi1_2"));
    ImageView* chi1_3 = static_cast<ImageView*>(this->rootNode->getChildByName("chi1_3"));
    ImageView* chi1_4 = static_cast<ImageView*>(this->rootNode->getChildByName("chi1_4"));
    ImageView* chi1_5 = static_cast<ImageView*>(this->rootNode->getChildByName("chi1_5"));
    
    this->lstImage.push_back(chi1_1);
    this->lstImage.push_back(chi1_2);
    this->lstImage.push_back(chi1_3);
    this->lstImage.push_back(chi1_4);
    this->lstImage.push_back(chi1_5);
    
    //Chi 2
    ImageView* chi2_1 = static_cast<ImageView*>(this->rootNode->getChildByName("chi2_1"));
    ImageView* chi2_2 = static_cast<ImageView*>(this->rootNode->getChildByName("chi2_2"));
    ImageView* chi2_3 = static_cast<ImageView*>(this->rootNode->getChildByName("chi2_3"));
    ImageView* chi2_4 = static_cast<ImageView*>(this->rootNode->getChildByName("chi2_4"));
    ImageView* chi2_5 = static_cast<ImageView*>(this->rootNode->getChildByName("chi2_5"));
    
    this->lstImage.push_back(chi2_1);
    this->lstImage.push_back(chi2_2);
    this->lstImage.push_back(chi2_3);
    this->lstImage.push_back(chi2_4);
    this->lstImage.push_back(chi2_5);
    
    
    //Chi 3
    ImageView* chi3_1 = static_cast<ImageView*>(this->rootNode->getChildByName("chi3_1"));
    ImageView* chi3_2 = static_cast<ImageView*>(this->rootNode->getChildByName("chi3_2"));
    ImageView* chi3_3 = static_cast<ImageView*>(this->rootNode->getChildByName("chi3_3"));
    this->lstImage.push_back(chi3_1);
    this->lstImage.push_back(chi3_2);
    this->lstImage.push_back(chi3_3);
    
    
    //Chi 1
    ImageView* chi1_1_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi1_1_0"));
    ImageView* chi1_2_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi1_2_0"));
    ImageView* chi1_3_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi1_3_0"));
    ImageView* chi1_4_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi1_4_0"));
    ImageView* chi1_5_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi1_5_0"));
    
    this->lstImageSochi.push_back(chi1_1_0);
    this->lstImageSochi.push_back(chi1_2_0);
    this->lstImageSochi.push_back(chi1_3_0);
    this->lstImageSochi.push_back(chi1_4_0);
    this->lstImageSochi.push_back(chi1_5_0);
    
    //Chi 2
    ImageView* chi2_1_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi2_1_0"));
    ImageView* chi2_2_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi2_2_0"));
    ImageView* chi2_3_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi2_3_0"));
    ImageView* chi2_4_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi2_4_0"));
    ImageView* chi2_5_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi2_5_0"));
    
    this->lstImageSochi.push_back(chi2_1_0);
    this->lstImageSochi.push_back(chi2_2_0);
    this->lstImageSochi.push_back(chi2_3_0);
    this->lstImageSochi.push_back(chi2_4_0);
    this->lstImageSochi.push_back(chi2_5_0);
    
    
    //Chi 3
    ImageView* chi3_1_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi3_1_0"));
    ImageView* chi3_2_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi3_2_0"));
    ImageView* chi3_3_0 = static_cast<ImageView*>(this->rootNode->getChildByName("chi3_3_0"));
    this->lstImageSochi.push_back(chi3_1_0);
    this->lstImageSochi.push_back(chi3_2_0);
    this->lstImageSochi.push_back(chi3_3_0);
    
    
    this->txtStatus = static_cast<Text*>(this->rootNode->getChildByName("txtStatus"));
    this->txtStatus->setVisible(false);
    
    this->imgChi1 = static_cast<ImageView*>(this->rootNode->getChildByName("imgChi1"));
    this->imgChi1->setVisible(false);
    this->imgChi1->setLocalZOrder(15);
    
    this->imgChi2 = static_cast<ImageView*>(this->rootNode->getChildByName("imgChi2"));
    this->imgChi2->setVisible(false);
    this->imgChi2->setLocalZOrder(15);
    
    this->imgChi3 = static_cast<ImageView*>(this->rootNode->getChildByName("imgChi3"));
    this->imgChi3->setVisible(false);
    this->imgChi3->setLocalZOrder(15);
    
    
    
    
    for (int i = 0; i < 13; i ++){
        Sprite* sp = Sprite::createWithSpriteFrameName("card_back.png");
        sp->setAnchorPoint(this->lstImage[i]->getAnchorPoint());
        sp->setPosition(this->lstImage[i]->getPosition());
        sp->setRotation(this->lstImage[i]->getRotationSkewX());
        this->lstPointOrg.push_back(this->lstImage[i]->getPosition());
        sp->setScale(0.75);
        this->rootNode->addChild(sp);
        this->lstCard.push_back(sp);
        sp->setLocalZOrder(30/ ((i)/5+1)  + (i)%5 );
        //        this->lstCard[i]->setRotationSkewX(this->lstImage[i]->getRotationSkewX());
        //        this->lstCard[i]->setRotationSkewY(this->lstImage[i]->getRotationSkewY());
        this->lstSkewXOrg.push_back(this->lstImage[i]->getRotationSkewX());
        this->lstSkewYOrg.push_back(this->lstImage[i]->getRotationSkewY());
        sp->setVisible(false);
    }
    this->txtSoChi = static_cast<Text*>(this->rootNode->getChildByName("txtSoChi"));
    this->txtSoChi->setVisible(false);
    this->resetGame();
    return true;
}
vector<Point> BaiMauBinh::getListPos(){
    return this->lstPointOrg;
}
vector<float> BaiMauBinh::getListSkewX(){
    return this->lstSkewXOrg;
}
vector<float> BaiMauBinh::getListSkewY(){
    return this->lstSkewYOrg;
}
void BaiMauBinh::runActionXepBai(vector<Point> lstPoint){
    for (int i = 0 ; i < this->lstCard.size(); i++){
        this->lstCard[i]->runAction(MoveTo::create(0.2, Vec2(lstPoint[i].x-710 - 80*(i%5),lstPoint[i].y-55-100*(i/5))));
        this->lstCard[i]->setRotation(0);
        this->lstCard[i]->runAction(ScaleTo::create(0.2, 0.75));
    }
    this->runAction(Sequence::create(DelayTime::create(0.2),CallFunc::create([this](){ this->setVisible(false);}), NULL));
}
void BaiMauBinh::setIsme(bool isme){
    this->isMe = isme;
}
bool BaiMauBinh::getIsme(){
    return this->isMe;
}
void BaiMauBinh::swapAnimation(float dt){
    //return;
    if (!this->isMe && this->isXepBai)
    {
        int from = rand() %12;
        int to = rand() %12;
        
        
        
        float orgSkewXFrom = this->lstCard[from]->getRotationSkewX();
        float orgSkewXTo = this->lstCard[to]->getRotationSkewX();
        
        auto orgPosFrom = this->lstCard[from]->getPosition();
        auto orgPosTo = this->lstCard[to]->getPosition();
        
        float orgZOderFrom = this->lstCard[from]->getLocalZOrder();
        float orgZOderTo = this->lstCard[to]->getLocalZOrder();
        
        this->lstCard[from]->setLocalZOrder(orgZOderTo);
        this->lstCard[to]->setLocalZOrder(orgZOderFrom);
        
        this->lstCard[from]->setRotation(orgSkewXTo);
        this->lstCard[to]->setRotation(orgSkewXFrom);

        this->lstCard[from]->runAction(MoveTo::create(0.3, orgPosTo));
        this->lstCard[to]->runAction(MoveTo::create(0.3, orgPosFrom));
        std::swap(this->lstCard[from], this->lstCard[to]);
        this->runAction(Sequence::create(DelayTime::create(0.5),CallFunc::create(CC_CALLBACK_0(BaiMauBinh::reloadPos, this)), NULL));
    }
}
void BaiMauBinh::reloadPos(){
    //return;
    for (int i = 0 ; i < this->lstCard.size(); i++){
        this->lstCard[i]->setScale(0.75);
        this->lstCard[i]->setPosition(this->lstImage[i]->getPosition());
        this->lstCard[i]->setRotation(this->lstImage[i]->getRotationSkewX());
    }
}
void BaiMauBinh::updateStatus(bool isDone){
    if (isDone){
        this->txtStatus->setString(dataManager.GetSysString(941));
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(BaiMauBinh::swapAnimation),this);
        this->isXepBai = false;
    }
    else{
        Director::getInstance()->getScheduler()->schedule(schedule_selector(BaiMauBinh::swapAnimation),this, 2, false);
        
        this->txtStatus->setString(dataManager.GetSysString(917));
        this->isXepBai = true;
        
    }
}
void BaiMauBinh::dealCard(string listCard){
    auto lst = mUtils::splitString(listCard,';');
    for (int i = 0; i < this->lstCard.size(); i ++){
        if (!this->isMe){
            this->lstCard[i]->setVisible(false);
        }
        else{
            this->lstCard[i]->setVisible(false);
            //this->lstCard[i]->setVisible(true);
        }
    }
    
    
    CallFunc* callfuncCard = CallFunc::create(CC_CALLBACK_0(BaiMauBinh::showCard, this));
    this->runAction(Sequence::create(DelayTime::create(0.15*13),callfuncCard,NULL));
    
    CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(BaiMauBinh::XepBaiAnimation, this));
    this->runAction(Sequence::create(DelayTime::create(0.15*14),callfunc,NULL));
    
}
void BaiMauBinh::showCard(){
    LayerBoBaiMauBinh* parent = (LayerBoBaiMauBinh*)this->getParent();
    if (parent == NULL)
        return;
    parent->XoaBaiChia();
    
    for (int i = 0; i < this->lstCard.size(); i ++){
        if (!this->isMe){
            //this->lstCard[i]->setPosition(this->lstImage[i]->getPosition());
            this->lstCard[i]->setVisible(true);
            this->lstCard[i]->setRotation(this->lstImage[i]->getRotationSkewX());
        }
        else{
            this->lstCard[i]->setVisible(true);
            //this->lstCard[i]->setPosition(this->lstImage[i]->getPosition());
            this->lstCard[i]->setRotation(this->lstImage[i]->getRotationSkewX());
            this->lstCard[i]->setVisible(true);
        }
    }
    
}
void BaiMauBinh::reloadBoBai(string listCard){
    this->currListCard = listCard;
    if (this->isMe){
        auto lst = mUtils::splitString(listCard,';');
        
        for (int i = 0; i < this->lstCard.size(); i ++){
            this->lstCard[i]->setVisible(true);
            this->lstCard[i]->setSpriteFrame(this->getSrcCard(lst[i]));
        }
    }
    this->txtStatus->setVisible(false);
}
string BaiMauBinh::getListBoBai(){
    return this->currListCard;
}
void BaiMauBinh::latbai(){
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(BaiMauBinh::swapAnimation),this);
    this->reloadPos();
    auto lst = mUtils::splitString(this->currListCard,';');
    for (int i = 0; i < this->lstCard.size(); i ++){
        this->lstCard[i]->setColor(Color3B::WHITE);
        this->lstCard[i]->setVisible(true);
        this->lstCard[i]->setSpriteFrame(this->getSrcCard(lst[i]));
    }
    this->txtStatus->setVisible(false);
    this->runAction(ScaleTo::create(0.2, 1.3));
}
void BaiMauBinh::XepBaiAnimation(){
    this->txtStatus->setVisible(true);
    this->updateStatus(false);
    if (this->isMe){
        for (int i = 0; i < this->lstCard.size(); i ++){
            this->lstCard[i]->setVisible(false);
            this->txtStatus->setVisible(false);
        }
    }
}
string BaiMauBinh::getSrcCard(string cardtype){
    string url = "card_";
    auto lstchi = mUtils::splitString(cardtype,'_');
    if (lstchi.size()<2)
        return "";
    int num = atoi(lstchi[0].c_str());
    int type = atoi(lstchi[1].c_str());
    switch (num) {
            
        case 11:
            url += "j";
            break;
        case 12:
            url += "q";
            break;
        case 13:
            url += "k";
            break;
        case 14:
            url += "a";
            break;
        default:
            url += StringUtils::format("%d",num);
            break;
    }
    
    url += "_";
    
    switch (type) {
        case 1:
            url += "h.png"; // cơ
            break;
        case 2:
            url += "d.png"; // rô
            break;
        case 3:
            url += "c.png"; // tép
            break;
        case 4:
            url += "s.png"; // bích
            break;
        default:
            break;
    }
    return url;
    
}

float BaiMauBinh::getRotationXByID(int i){
    return this->lstImage[i]->getRotationSkewX();
}
float BaiMauBinh::getRotationYByID(int i){
    return this->lstImage[i]->getRotationSkewY();
}
Vec2 BaiMauBinh::getPosByID(int i){
    Vec2 pos = Vec2(this->lstImage[i]->getPositionX(),this->lstImage[i]->getPositionY());
    return pos;
}
void BaiMauBinh::soChiEffect(int chi, string rs){
    //this->reloadPos();
    //    for (int i = 0 ; i < this->lstCard.size(); i++){
    //        this->lstCard[i]->setPosition(this->lstPointOrg[i]);
    //        this->lstCard[i]->setRotationSkewX(this->lstSkewXOrg[i]);
    //        this->lstCard[i]->setRotationSkewY(this->lstSkewYOrg[i]);
    //
    //    }
    
    this->runAction(ScaleTo::create(0.15, 1));
    this->updateStatus(true);
    this->txtStatus->setVisible(false);

    auto lst = mUtils::splitString(this->currListCard,';');
    if (lst.size() == 0) return;
    auto lstketqua = mUtils::splitString(rs,';');
    if (chi == 1){
        for (int i = 0; i < this->lstImage.size(); i ++){
            this->lstCard[i]->setColor(Color3B(150, 150, 150));
            this->lstCard[i]->runAction(ScaleTo::create(0.3, 0.75));
            this->lstCard[i]->runAction(RotateTo::create(0.2, this->lstImage[i]->getRotationSkewX(), this->lstImage[i]->getRotationSkewY()));
            this->lstCard[i]->runAction(MoveTo::create(0.3, this->lstImage[i]->getPosition()));
            this->lstCard[i]->setLocalZOrder(30/ ((i)/5+1)  + (i)%5 );
        }
        for (int i = 0; i < 5; i ++){
            this->lstCard[i]->setColor(Color3B::WHITE);
            
            // if (!this->isMe){
            int skewAmount = 20;
            auto firstSkew2 = SkewBy::create(0.1, 0, skewAmount);
            Sprite* pCard = this->lstCard[i];
            auto str = this->getSrcCard(lst[i]);
            auto initFrame2 = CallFunc::create([pCard, str](){pCard->setSpriteFrame(str); });
            // flip the skew value after the initial half of the animation for a more credible 3d effect
            auto instantFlipSkew2 = SkewTo::create(0, 0, -skewAmount);
            auto resetSkew2 = SkewTo::create(0.1, 0, 0);
            auto delayTime2 = DelayTime::create(0.1);
            pCard->runAction(Sequence::create(DelayTime::create(0.3),ScaleTo::create(0.2,1.25),NULL));
            //                pCard->setRotationSkewX(0);
            //                pCard->setRotationSkewY(0);
            pCard->runAction(RotateTo::create(0.2, 0, 0));
            pCard->setLocalZOrder(105);
            
            pCard->runAction(MoveTo::create(0.2, this->lstImageSochi[i]->getPosition()));
            // var firtRotate = cc.Rotate3.create(0.5,cc.math.vec3(0,90,0));
            //pCard->runAction(Sequence::create(delayTime2, firstSkew2, instantFlipSkew2, initFrame2, resetSkew2, NULL));
            pCard->runAction(Sequence::create( initFrame2,NULL));
            
            // }
           // log("rs 1: %s",rs.c_str());
            if (atoi(lstketqua[0].c_str()) != 0){
                this->imgChi1->setVisible(true);
                this->imgChi1->loadTexture(this->getStrBoBai(atoi(lstketqua[0].c_str())));
                
            }
            else{
                this->imgChi1->loadTexture(this->getStrBoBai(atoi(lstketqua[2].c_str())));
                this->imgChi1->setVisible(true);
            }
        }
        this->txtSoChi->setVisible(false);
        int sochi = atoi(lstketqua[3].c_str());
        if (sochi > 0){
            this->txtSoChi->setString("+"+lstketqua[3]+ " "+dataManager.GetSysString(921));
        }else{
            this->txtSoChi->setString(lstketqua[3]+ " "+dataManager.GetSysString(921));
            
            
        }
        CallFunc* showChi = CallFunc::create(CC_CALLBACK_0(BaiMauBinh::showChiAvatar, this,(sochi)));
        this->runAction(Sequence::create(DelayTime::create(0.6),showChi, NULL));
    }
    if (chi == 2){
        for (int i = 0; i < this->lstImage.size(); i ++){
            this->lstCard[i]->setColor(Color3B(150, 150, 150));
            this->lstCard[i]->runAction(ScaleTo::create(0.3, 0.75));
            this->lstCard[i]->runAction(RotateTo::create(0.2, this->lstImage[i]->getRotationSkewX(), this->lstImage[i]->getRotationSkewY()));
            this->lstCard[i]->runAction(MoveTo::create(0.3, this->lstImage[i]->getPosition()));
            this->lstCard[i]->setLocalZOrder(30/ ((i)/5+1)  + (i)%5 );
            
        }
        for (int i = 5; i < 10; i ++){
            this->lstCard[i]->setColor(Color3B::WHITE);
            
            // if (!this->isMe){
            int skewAmount = 20;
            auto firstSkew2 = SkewBy::create(0.1, 0, skewAmount);
            Sprite* pCard = this->lstCard[i];
            auto str = this->getSrcCard(lst[i]);
            auto initFrame2 = CallFunc::create([pCard, str](){pCard->setSpriteFrame(str); });
            // flip the skew value after the initial half of the animation for a more credible 3d effect
            auto instantFlipSkew2 = SkewTo::create(0, 0, -skewAmount);
            auto resetSkew2 = SkewTo::create(0.1, 0, 0);
            auto delayTime2 = DelayTime::create(0.1);
            pCard->runAction(RotateTo::create(0.2, 0, 0));
            
            pCard->runAction(Sequence::create(DelayTime::create(0.3),ScaleTo::create(0.2,1.25),NULL));
            pCard->setLocalZOrder(105);
            pCard->runAction(MoveTo::create(0.2, this->lstImageSochi[i]->getPosition()));
            // var firtRotate = cc.Rotate3.create(0.5,cc.math.vec3(0,90,0));
            //pCard->runAction(Sequence::create(delayTime2, firstSkew2, instantFlipSkew2, initFrame2, resetSkew2, NULL));
            pCard->runAction(Sequence::create( initFrame2,NULL));
            
            // var firtRotate = cc.Rotate3.create(0.5,cc.math.vec3(0,90,0));
            // pCard->runAction(Sequence::create(delayTime2, firstSkew2, instantFlipSkew2, initFrame2, resetSkew2, NULL));
            // }
            
           // log("rs 2: %s",rs.c_str());
            
            if (atoi(lstketqua[0].c_str()) != 0){
                this->imgChi1->setVisible(false);
                this->imgChi2->setVisible(true);
                
                this->imgChi2->loadTexture(this->getStrBoBai(atoi(lstketqua[0].c_str())));
                
            }
            else{
                this->imgChi1->setVisible(false);
                this->imgChi2->setVisible(true);
                this->imgChi2->loadTexture(this->getStrBoBai(atoi(lstketqua[2].c_str())));
                
            }
        }
        this->txtSoChi->setVisible(false);
        int sochi = atoi(lstketqua[3].c_str());
        if (sochi > 0){
            this->txtSoChi->setString("+"+lstketqua[3]+ " "+dataManager.GetSysString(921));
        }else{
            this->txtSoChi->setString(lstketqua[3]+ " "+dataManager.GetSysString(921));
            
            
        }
        CallFunc* showChi = CallFunc::create(CC_CALLBACK_0(BaiMauBinh::showChiAvatar, this,(sochi)));
        this->runAction(Sequence::create(DelayTime::create(0.6),showChi, NULL));
    }
    if (chi == 3){
        for (int i = 0; i < this->lstImage.size(); i ++){
            this->lstCard[i]->setColor(Color3B(150, 150, 150));
            this->lstCard[i]->runAction(ScaleTo::create(0.3, 0.75));
            this->lstCard[i]->runAction(RotateTo::create(0.2, this->lstImage[i]->getRotationSkewX(), this->lstImage[i]->getRotationSkewY()));
            this->lstCard[i]->runAction(MoveTo::create(0.3, this->lstImage[i]->getPosition()));
            this->lstCard[i]->setLocalZOrder(30/ ((i)/5+1)  + (i)%5 );
            
        }
        for (int i = 10; i < 13; i ++){
            this->lstCard[i]->setColor(Color3B::WHITE);
            // if (!this->isMe){
            int skewAmount = 20;
            auto firstSkew2 = SkewBy::create(0.1, 0, skewAmount);
            Sprite* pCard = this->lstCard[i];
            pCard->runAction(Sequence::create(DelayTime::create(0.3),ScaleTo::create(0.2,1.25),NULL));
            
            auto str = this->getSrcCard(lst[i]);
            auto initFrame2 = CallFunc::create([pCard, str](){pCard->setSpriteFrame(str); });
            // flip the skew value after the initial half of the animation for a more credible 3d effect
            auto instantFlipSkew2 = SkewTo::create(0, 0, -skewAmount);
            auto resetSkew2 = SkewTo::create(0.1, 0, 0);
            auto delayTime2 = DelayTime::create(0.1);
            pCard->runAction(RotateTo::create(0.2, 0, 0));
            
            pCard->setLocalZOrder(105);
            
            pCard->runAction(MoveTo::create(0.2, this->lstImageSochi[i]->getPosition()));
            // var firtRotate = cc.Rotate3.create(0.5,cc.math.vec3(0,90,0));
            //pCard->runAction(Sequence::create(delayTime2, firstSkew2, instantFlipSkew2, initFrame2, resetSkew2, NULL));
            pCard->runAction(Sequence::create( initFrame2,NULL));
            
            // var firtRotate = cc.Rotate3.create(0.5,cc.math.vec3(0,90,0));
            //pCard->runAction(Sequence::create(delayTime2, firstSkew2, instantFlipSkew2, initFrame2, resetSkew2, NULL));
            // }
            
           // log("rs 3: %s",rs.c_str());
            
            if (atoi(lstketqua[0].c_str()) != 0){
                this->imgChi1->setVisible(false);
                this->imgChi2->setVisible(false);
                
                this->imgChi3->setVisible(true);
                
                this->imgChi3->loadTexture(this->getStrBoBai(atoi(lstketqua[0].c_str())));
                
            }
            else{
                this->imgChi3->setVisible(true);
                this->imgChi1->setVisible(false);
                this->imgChi2->setVisible(false);
                this->imgChi3->loadTexture(this->getStrBoBai(atoi(lstketqua[2].c_str())));
                
            }
        }
        this->txtSoChi->setVisible(false);
        int sochi = atoi(lstketqua[3].c_str());
        if (sochi > 0){
            this->txtSoChi->setString("+"+lstketqua[3] + " "+dataManager.GetSysString(921));
        }else{
            this->txtSoChi->setString(lstketqua[3]+ " "+dataManager.GetSysString(921));
            
        }
        CallFunc* showChi = CallFunc::create(CC_CALLBACK_0(BaiMauBinh::showChiAvatar, this,(sochi)));
        this->runAction(Sequence::create(DelayTime::create(0.6),showChi, NULL));
    }
}
void BaiMauBinh::showChiAvatar(int chi){
    LayerBoBaiMauBinh* parent = (LayerBoBaiMauBinh*)this->getParent();
    if (parent == NULL)
        return;
    
    LayerGameMauBinh* game = (LayerGameMauBinh*)parent->getParent();
    if (game == NULL)
        return;
    LayerAvatarInGame52La* layerAvatar = (LayerAvatarInGame52La*)game->getChildByTag(100);
    if (layerAvatar == NULL)
        return;
    double muccuoc = 0;
    double phe = 0;
    if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() != NULL){
        int id = std::atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0, 3).c_str());
        boost::shared_ptr<RoomVariable> _phe = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("fee");
        if (_phe != NULL){
            string paramString = _phe->GetStringValue()->c_str();
            phe = atof(paramString.c_str());
        }
        boost::shared_ptr<RoomVariable> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params");
        if (param != NULL){
            string paramString = param->GetStringValue()->c_str();
            vector<string> arrInfo = mUtils::splitString(paramString, '@');
            string money = arrInfo.at(0);
            muccuoc = atol(money.c_str());
            int roomID = 101;
            
            
        }
        
        double money = muccuoc*chi;
        std::string res;
        if (money > 0)
            res = "+" + StringUtils::format("%f",money-money*phe);
        else
            res = StringUtils::format("%f",money);
        auto avatar = layerAvatar->getUserByPos(this->currPos);
        if (avatar)
            avatar->showNumberSoChi(res);
        
    }
    
}
void BaiMauBinh::LatBaiCuoiVan(){
    auto lst = mUtils::splitString(this->currListCard,';');
    if (lst.size()== 0) return;
    for (int i = 0; i < this->lstImage.size(); i ++){
        this->lstCard[i]->setColor(Color3B::WHITE);
        this->lstCard[i]->runAction(ScaleTo::create(0.3, 0.75));
        this->lstCard[i]->setRotation(this->lstImage[i]->getRotationSkewX());
        this->lstCard[i]->runAction(MoveTo::create(0.3, this->lstImage[i]->getPosition()));
        this->lstCard[i]->setLocalZOrder(30/ ((i)/5+1)  + (i)%5 );
    }
    
    this->txtStatus->setVisible(false);
    this->txtSoChi->setVisible(false);
    this->imgChi3->setVisible(false);
    this->imgChi2->setVisible(false);
    this->imgChi1->setVisible(false);
    this->runAction(ScaleTo::create(0.3,1.5));
}
void BaiMauBinh::setChi(string chi){
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(BaiMauBinh::swapAnimation),this);
    this->txtSoChi->setVisible(false);
    int sochi = atoi(chi.c_str());
    if (sochi > 0){
        this->txtSoChi->setString("+"+chi + " "+dataManager.GetSysString(921));
    }else{
        this->txtSoChi->setString(chi+ " "+dataManager.GetSysString(921));
        
    }
    if (chi == ""){
        this->txtSoChi->setVisible(false);
        return;
    }
    this->showChiAvatar(sochi);
}
void BaiMauBinh::setPos(int _pos){
    this->currPos = _pos;
}
string BaiMauBinh::getStrBoBai(int code){
    if (this->isMe){
        if (mUtils::isSoundOn())
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(this->getSoundPath(code).c_str());
    }
    switch (code) {
        case GAME_CARD_TYPE_SAM_CHI_CUOI:
            return "mb_sam_chi_cuoi.png";
        case GAME_CARD_TYPE_CU_LU_CHI_GIUA:
            return "mb_cu_lu_chi_giua.png";
        case GAME_CARD_TYPE_TU_QUY_CHI_DAU:
            return "mb_tu_quy_chi_dau.png";
        case GAME_CARD_TYPE_TU_QUY_CHI_GIUA:
            return "mb_tu_quy_chi_giua.png";
        case GAME_CARD_TYPE_THUNG_PHA_SANH_CHI_DAU:
            return "mb_thung_pha_sanh_chi_dau.png";
        case GAME_CARD_TYPE_THUNG_PHA_SANH_CHI_GIUA:
            return "mb_thung_pha_sanh_chi_giua.png";
        case GAME_CARD_TYPE_MAU_THAU:
            return "mb_mau_thau.png";
        case GAME_CARD_TYPE_DOI:
            return "mb_doi.png";
        case GAME_CARD_TYPE_THU:
            return "mb_thu.png";
        case GAME_CARD_TYPE_SAM_CO:
            return "mb_sam_chi.png";
        case GAME_CARD_TYPE_SANH:
            return "mb_sanh.png";
        case GAME_CARD_TYPE_THUNG:
            return "mb_thung.png";
        case GAME_CARD_TYPE_CU_LU:
            return "mb_cu_lu.png";
        case GAME_CARD_TYPE_TU_QUY:
            return "mb_tu_quy.png";
        case GAME_CARD_TYPE_THUNG_PHA_SANH:
            return "mb_thung_pha_sanh.png";
    }
    
}
string BaiMauBinh::getSoundPath(int code){
    switch (code) {
        case GAME_CARD_TYPE_SAM_CHI_CUOI:
            return "SamChiCuoi.mp3";
        case GAME_CARD_TYPE_CU_LU_CHI_GIUA:
            return "CuLuChiGiua.mp3";
        case GAME_CARD_TYPE_TU_QUY_CHI_DAU:
            return "TuQuyChiDau.mp3";
        case GAME_CARD_TYPE_TU_QUY_CHI_GIUA:
            return "TuQuyChiGiua.mp3";
        case GAME_CARD_TYPE_THUNG_PHA_SANH_CHI_DAU:
            return "ThungPhaSanhChiDau.mp3";
        case GAME_CARD_TYPE_THUNG_PHA_SANH_CHI_GIUA:
            return "ThungPhaSanhChiGiua.mp3";
        case GAME_CARD_TYPE_MAU_THAU:
            return "MauThau.mp3";
        case GAME_CARD_TYPE_DOI:
            return "Doi.mp3";
        case GAME_CARD_TYPE_THU:
            return "Thu.mp3";
        case GAME_CARD_TYPE_SAM_CO:
            return "SamCo.mp3";
        case GAME_CARD_TYPE_SANH:
            return "Sanh.mp3";
        case GAME_CARD_TYPE_THUNG:
            return "Thung.mp3";
        case GAME_CARD_TYPE_CU_LU:
            return "CuLu.mp3";
        case GAME_CARD_TYPE_TU_QUY:
            return "TuQuy.mp3";
    }
}
void BaiMauBinh::resetGame(){
    this->updateStatus(true);
    this->stopAllActions();
    for (int i = 0; i < this->lstImage.size(); i ++){
        this->lstCard[i]->stopAllActions();
        this->lstCard[i]->setSpriteFrame("card_back.png");
        this->lstCard[i]->setColor(Color3B::WHITE);
        this->lstCard[i]->setPosition(this->lstImage[i]->getPosition());
        this->lstCard[i]->setRotation(this->lstImage[i]->getRotationSkewX());
        this->lstCard[i]->setLocalZOrder(30/ ((i)/5+1)  + (i)%5 );
    }
    this->txtSoChi->setVisible(false);
    this->imgChi1->setVisible(false);
    this->imgChi2->setVisible(false);
    this->imgChi3->setVisible(false);
    this->txtStatus->setString(dataManager.GetSysString(917));
    this->txtStatus->setVisible(false);
    this->isMe = false;
    this->currListCard = "";
    this->isXepBai = false;
    this->setScale(1);
    
}
void BaiMauBinh::bocuoc(){
    this->txtStatus->setVisible(false);
}
void BaiMauBinh::onEnter()
{
    Layer::onEnter();
}

void BaiMauBinh::onExit()
{
    Layer::onExit();
}
