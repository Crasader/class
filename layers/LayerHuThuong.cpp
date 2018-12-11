//
//  LayerHuThuong.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/25/16.
//
//

#include "LayerHuThuong.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "LayerPopupMission.h"
#include "../data_manager.h"
#include "../SceneManager.h"
#include "LayerPopupThongBao.h"
#include "LayerPopupMission.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "PluginFacebook/PluginFacebook.h"
#include "mFaceBook.h"
#include "../data_manager.h"
#endif
USING_NS_CC;
using namespace cocos2d::ui;

LayerHuThuong::LayerHuThuong()
{
    GameServer::getSingleton().addListeners(this);
}


LayerHuThuong::~LayerHuThuong()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerHuThuong::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    rootNode = CSLoader::getInstance()->createNode("LayerHuThuong.csb");
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    

    imgbg = static_cast<ImageView*>(pnlBg->getChildByName("Image_1"));
    
    Text* txtMoney = static_cast<Text*>(pnlBg->getChildByName("Text_1"));
    txtMoney->setVisible(false);

    labelResult = Label::createWithBMFont("font_win-font.fnt", "", TextHAlignment::CENTER);
    labelResult->setScale(1.3);
    labelResult->setAnchorPoint(txtMoney->getAnchorPoint());
    labelResult->setPosition(Vec2(txtMoney->getPosition().x,txtMoney->getPosition().y-10));
    pnlBg->addChild(labelResult);

    rootNode->setVisible(false);
    
    isGetBonus = false;
    

    this->isNoHu = false;
   // this->setMoney("200,000");
    
    return true;
}
void LayerHuThuong::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp("e", cmd->c_str()) == 0 || strcmp("endntf", cmd->c_str()) == 0){
        if (SceneManager::getSingleton().getGameID() == kGameTaiXiu || SceneManager::getSingleton().getGameID() == kGameXocDia || SceneManager::getSingleton().getGameID() == kGameQuayThuong ) {
            return;
        }
        if (isGetBonus){
            //moneyBonus = 1000;
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutInt("evl", moneyBonus);
            boost::shared_ptr<IRequest> request(new ExtensionRequest("oe", params));
            GameServer::getSingleton().Send(request);
        }
    }
    
    else if (strcmp("dsteggcmd", cmd->c_str()) == 0){
        if (param->GetInt("evl") == NULL) return;
        moneyBonus = *param->GetInt("evl");
        isGetBonus = true;
    }
    else if (0 == cmd->compare("eensn")){
        boost::shared_ptr<string> aI = param->GetUtfString("aI");
        boost::shared_ptr<string> content = param->GetUtfString("content");
        if (aI == NULL) return;
        if (content == NULL) return;
        if (0 == content->compare("")){
            return;
        }
        if (0 == aI->compare(SceneManager::getSingleton().getMyName())){
            this->isNoHu = true;
        }
    }
    else if (0 == cmd->compare(EXT_EVENT_CAOTHAP_GAME_RESULT)) {
        boost::shared_ptr<long> gwv= param->GetInt("gwv");
        if (gwv){
            if (*gwv>0){
                if (this->isNoHu){
                    this->runAction(Sequence::create( DelayTime::create(3), CallFunc::create(CC_CALLBACK_0(LayerHuThuong::setMoney, this,mUtils::convertMoneyEx((long long)*gwv))),NULL));
                    this->isNoHu = false;
                }
            }else{
                this->isNoHu = false;
            }
        }
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT_PKMINI)) {
        boost::shared_ptr<long> gwv= param->GetInt("gwv");
        if (gwv){
            if (*gwv>0){
                if (this->isNoHu){
                    this->runAction(Sequence::create( DelayTime::create(3), CallFunc::create(CC_CALLBACK_0(LayerHuThuong::setMoney, this,mUtils::convertMoneyEx((long long)*gwv))),NULL));
                    this->isNoHu = false;
                }
            }else{
                this->isNoHu = false;
            }
        }
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT_TXMINI)) {
        boost::shared_ptr<long> gwv= param->GetInt("gwv");
        if (gwv){
            if (*gwv>0){
                if (this->isNoHu){
                    this->runAction(Sequence::create( DelayTime::create(3), CallFunc::create(CC_CALLBACK_0(LayerHuThuong::setMoney, this,mUtils::convertMoneyEx((long long)*gwv))),NULL));
                    this->isNoHu = false;
                }
            }else{
                this->isNoHu = false;
            }
        }
    }
    else if (strcmp("resoe", cmd->c_str()) == 0){
         if (param->GetInt("rc") == NULL) return;
        int rc = *param->GetInt("rc");
        if (rc == 0){
            rootNode->setVisible(true);
            setMoney(mUtils::convertMoneyEx((long long)moneyBonus));
            isGetBonus = false;
            moneyBonus = 0;
        }
        
    }
    else if (0 == cmd->compare(EXT_EVENT_VQ_ROTATE_RESP)) {
        boost::shared_ptr<string> grs = param->GetUtfString(EXT_FIELD_GAME_VQ_RESULT);
        //    boost::shared_ptr<string> lst7lucky = this->param->GetUtfString("lst7");
        //    boost::shared_ptr<long> has7 = this->param->GetInt("has7");
        
        if (grs!= NULL){
            string rs = *grs;
            auto lst = mUtils::splitString(rs, ';');
            if (lst.size()<6) return;
            bool isNoHu = false;
            for (int k = 6; k < lst.size() ; k++){
                auto lstphanthuong = mUtils::splitString(lst[k], ',');
                /*#define MAT_LUOT_VQMM 5
                 #define NO_HU_VQMM 1
                 #define HAU_TO_EXP 2
                 #define HAU_TO_GOLD 0
                 #define HAU_TO_SILVER 1*/
                if (atoi(lstphanthuong[1].c_str()) > 0){
                    isNoHu = true;
                }
            }
            if (isNoHu){
                this->runAction(Sequence::create( DelayTime::create(11), CallFunc::create(CC_CALLBACK_0(LayerHuThuong::setMoney, this,mUtils::convertMoneyEx(atol(lst[1].c_str())))),NULL));
            }
        }
    }
}
void LayerHuThuong::setMoney(string money){
    
    for (int i = 0; i < 5; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 960 - i * 300;
        float posY = -200;
        int delay = rand() % 3;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);
        
    }
    
    for (int i = 5; i < 10; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 960 + (i-5) * 300;
        float posY = -200;
        int delay = rand() % 3+3;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setLocalZOrder(1);
        coin->setTag(i);
    }
    
    for (int i = 10; i < 15; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 960 + (i-10) * 300;
        float posY = 1080+200;
        int delay = rand() % 3+1;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);
        
    }
    for (int i = 15; i < 20; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 960 - (i-15) * 300;
        float posY = 1080+200;
        int delay = rand() % 3+2;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);
        
    }
    for (int i = 20; i < 25; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = -200;
        float posY = 300 *(i-20);
        int delay = rand() % 3+4;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);
        
    }
    for (int i = 25; i < 30; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 1920+200;
        float posY = 300 *(i-25);
        int delay = rand() % 5+3;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);
    }
    for (int i = 30; i < 35; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 1920+200;
        float posY = 350 *(i-30);
        int delay = rand() % 8+5;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);
    }
    
    for (int i = 0; i < 5; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 960 - i * 300;
        float posY = -200;
        int delay = rand() % 3;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);

    }
    
    for (int i = 5; i < 10; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 960 + (i-5) * 300;
        float posY = -200;
        int delay = rand() % 3;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setLocalZOrder(1);
        coin->setTag(i);
    }
    
    for (int i = 10; i < 15; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 960 + (i-10) * 300;
        float posY = 1080+200;
        int delay = rand() % 3;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);

    }
    for (int i = 15; i < 20; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 960 - (i-15) * 300;
        float posY = 1080+200;
        int delay = rand() % 3;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);

    }
    for (int i = 20; i < 25; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = -200;
        float posY = 300 *(i-20);
        int delay = rand() % 3;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);

    }
    for (int i = 25; i < 30; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 1920+200;
        float posY = 300 *(i-25);
        int delay = rand() % 3;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);
    }
    for (int i = 30; i < 35; i++){
        int ran = rand() % 7 +1;
        Sprite* coin = Sprite::create(StringUtils::format("moc3-vang-%d.png",ran));
        this->addChild(coin);
        coin->setAnchorPoint(Vec2(0.5,0.5));
        coin->setPosition(Vec2(960,540));
        coin->setTag(144);
        Vector<SpriteFrame*> animFrames;
        string str = "";
        for (int i = 1; i <= 7; i++) {
            str = StringUtils::format("moc3-vang-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 121, 122));
            animFrames.pushBack(spriteFrame);
        }
        auto anim = Animation::createWithSpriteFrames(animFrames, 0.08f);
        anim->retain();
        
        coin->runAction(RepeatForever::create(Animate::create(anim)));
        float posX = 1920+200;
        float posY = 350 *(i-30);
        int delay = rand() % 3+2;
        coin->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(1.5, Vec2(posX,posY)),NULL));
        coin->setTag(i);
        coin->setLocalZOrder(1);
    }
    labelResult->setString(money);
    rootNode->setVisible(true);
    rootNode->setLocalZOrder(5);
    //clipper->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.2, Vec2(10,10)),MoveBy::create(0.2, Vec2(-10,-10)),MoveBy::create(0.2, Vec2(10,10)),MoveBy::create(0.1, Vec2(-10,-10)),NULL)));
    imgbg->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 1.2),ScaleTo::create(0.3, 1),NULL)));
    CallFunc* call = CallFunc::create(CC_CALLBACK_0(LayerHuThuong::CallFuncRemove, this));
    this->runAction(Sequence::create(DelayTime::create(10), call, NULL));
    
}

void LayerHuThuong::CallFuncRemove(){
    this->stopAllActions();
    rootNode->setVisible(false);
    for (int i = 0;i<35;i++){
        if (this->getChildByTag(i))
            this->removeChildByTag(i);
    }
}
bool LayerHuThuong::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerHuThuong::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    this->CallFuncRemove();
}
void LayerHuThuong::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(LayerHuThuong::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerHuThuong::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerHuThuong::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
