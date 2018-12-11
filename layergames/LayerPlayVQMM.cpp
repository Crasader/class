//
//  LayerPlayVQMM.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 8/31/16.
//
//

#include "LayerPlayVQMM.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"
#include "LayerLichSuVQMM.h"
#include "_Chat_.h"
#include "TaiXiuMessageDefs.h"
#include "../layers/LayerPopupThongBao.h"
#include "../layers/LayerGuideInGame.h"
#include "LayerXepHangMiniPoker.h"
#include "LayerLichSuMiniPoker.h"
#include "LayerXepHangVQMM.h"
#include "../layers/LayerHuThuong.h"
#include "../scenes/ScenePickRoom.h"
#include "../scenes/SceneMain.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerPlayVQMM::LayerPlayVQMM()
{
    
}


LayerPlayVQMM::~LayerPlayVQMM()
{
    
}

bool LayerPlayVQMM::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPlayVQMM.csb");
    
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
    //rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    //auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    //pnlB->setSwallowTouches(false);

    //pnlB->setContentSize(Size(visibleSize.width, visibleSize.height));
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
    //if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }
//    else{
//        sizeAdd = Size(0, 0);
//    }
    Size sizeAdd2 = Size(0,0);
    //rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    
    this->mQueueMsg = new ThreadQueueMsg();
    
    this->pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    this->pnlBg->setSwallowTouches(false);
    //this->pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));

    this->btnPlay = static_cast<Button*>(this->pnlBg->getChildByName("btnQuay"));
    if (this->btnPlay) {
        this->btnPlay->addTouchEventListener(CC_CALLBACK_2(LayerPlayVQMM::onBtnPlay, this));
        this->btnPlay->loadTextures(StringUtils::format("%s%smoc3-btn-quay-enable.png",PATH_VQ.c_str(),SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("%s%smoc3-btn-quay-disable.png",PATH_VQ.c_str(),SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("%s%smoc3-btn-quay-disable.png",PATH_VQ.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    }
    Button* btnClose = static_cast<Button*>(this->pnlBg->getChildByName("btnClose"));
    if (btnClose) {
        btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPlayVQMM::onBtnClose, this));
        btnClose->setPressedActionEnabled(true);
    }
    
    Button* btnXepHang = static_cast<Button*>(this->pnlBg->getChildByName("btnXepHang"));
    if (btnXepHang) {
        btnXepHang->addTouchEventListener(CC_CALLBACK_2(LayerPlayVQMM::onBtnXepHang, this));
        btnXepHang->setPressedActionEnabled(true);
    }
    
    Button* btnInfo = static_cast<Button*>(this->pnlBg->getChildByName("btnLichSu"));
    if (btnInfo) {
        btnInfo->addTouchEventListener(CC_CALLBACK_2(LayerPlayVQMM::onBtnLichSu, this));
        btnInfo->setPressedActionEnabled(true);
    }
    
    Button* btnHuongDan = static_cast<Button*>(this->pnlBg->getChildByName("btnHuongDan"));
    if (btnHuongDan) {
        btnHuongDan->addTouchEventListener(CC_CALLBACK_2(LayerPlayVQMM::onBtnHuongDan, this));
        btnHuongDan->setPressedActionEnabled(true);
    }
    
    
    this->imgVong1 = static_cast<ImageView*>(this->pnlBg->getChildByName("imgVong1"));
    this->imgVong1->loadTexture(StringUtils::format("%s%smoc3-vong3.png",PATH_VQ.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));
    this->imgVong2= static_cast<ImageView*>(this->pnlBg->getChildByName("imgVong2"));
    this->imgVong2->loadTexture(StringUtils::format("%s%smoc3-vong2.png",PATH_VQ.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));

    this->imgVong3= static_cast<ImageView*>(this->pnlBg->getChildByName("imgVong3"));
    this->imgVong3->loadTexture(StringUtils::format("%s%smoc3-vong1.png",PATH_VQ.c_str(),SceneManager::getSingleton().getFolderLang().c_str()));

    this->btnVong1 = static_cast<Button*>(this->pnlBg->getChildByName("btnVong1"));
    if (this->btnVong1) {
        this->btnVong1->setTag(1);
        this->btnVong1->addClickEventListener(CC_CALLBACK_1(LayerPlayVQMM::onBtnMuaLuotQuay, this));
        this->btnVong1->setPressedActionEnabled(true);
        this->btnVong1->setTitleText(dataManager.GetSysString(898));
    }
    

    this->btnVong2 = static_cast<Button*>(this->pnlBg->getChildByName("btnVong2"));
    if (this->btnVong2) {
        this->btnVong2->setTag(2);
        this->btnVong2->addClickEventListener(CC_CALLBACK_1(LayerPlayVQMM::onBtnMuaLuotQuay, this));
        this->btnVong2->setPressedActionEnabled(true);
        this->btnVong2->setTitleText(dataManager.GetSysString(899));

    }
    

    this->btnVong3 = static_cast<Button*>(this->pnlBg->getChildByName("btnVong3"));
    if (this->btnVong3) {
        this->btnVong3->setTag(3);
        this->btnVong3->addClickEventListener(CC_CALLBACK_1(LayerPlayVQMM::onBtnMuaLuotQuay, this));
        this->btnVong3->setPressedActionEnabled(true);
        this->btnVong3->setTitleText(dataManager.GetSysString(900));

    }
    this->btnVong3->setBright(false);

    

    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    if (mySelf)
    {
        boost::shared_ptr<UserVariable> rotatebonus = mySelf->GetVariable("rotatebonus");
        if (rotatebonus)
        {
            int rotatebonus_value = *rotatebonus->GetIntValue();
            if (rotatebonus_value>0){
                this->btnVong3->setTitleText("Free");
                this->isMuaLuotQuay = true;
                this->btnVong3->setEnabled(false);
                this->btnVong3->setBright(false);
                this->btnVong3->setVisible(true);
                this->btnPlay->setEnabled(true);
            }else{
                this->btnVong3->setVisible(false);

            }
        }
    }
    
    //Animation
    auto imgVong1_0_0 = static_cast<ImageView*>(this->imgVong2->getChildByName("imgVong1_0_0"));
    imgVong1_0_0->setVisible(false);
    Sprite* spriteLightOff = Sprite::create("VQMMMini/moc3-vong2-denled-1.png");
    spriteLightOff->setAnchorPoint(imgVong1_0_0->getAnchorPoint());
    spriteLightOff->setPosition(imgVong1_0_0->getPosition());
    this->imgVong2->addChild(spriteLightOff);
    
    Sprite* spriteLightOn = Sprite::create("VQMMMini/moc3-vong2-denled-2.png");
    spriteLightOn->setAnchorPoint(imgVong1_0_0->getAnchorPoint());
    spriteLightOn->setPosition(imgVong1_0_0->getPosition());
    this->imgVong2->addChild(spriteLightOn);
    

    
    
    auto imgVong1_0_0_0 = static_cast<ImageView*>(this->pnlBg->getChildByName("imgVong1_0_0_0"));
    
    auto imgVong1_0_0_0_0 = static_cast<ImageView*>(imgVong1_0_0_0->getChildByName("imgVong1_0_0_0_0"));
    imgVong1_0_0_0_0->setVisible(false);
    
    Sprite* vong3 = Sprite::create("VQMMMini/moc3-vong3-light-1.png");
    imgVong1_0_0_0->addChild(vong3);
    vong3->setAnchorPoint(imgVong1_0_0_0_0->getAnchorPoint());
    vong3->setPosition(imgVong1_0_0_0_0->getPosition());

    Vector<SpriteFrame*> animFrames;
    string str = "";
    for (int i = 1; i <= 4; i++) {
        str = StringUtils::format("VQMMMini/moc3-vong3-light-%d.png",i);
        auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 636, 636));
        animFrames.pushBack(spriteFrame);
    }
    auto anim = Animation::createWithSpriteFrames(animFrames, 0.8);
    anim->retain();
    
    vong3->runAction(RepeatForever::create(Animate::create(anim)));
    
    ActionInterval *actionrepeat = Sequence::create(FadeIn::create(0.8), FadeOut::create(1),RotateBy::create(1,30), nullptr);
    spriteLightOn->runAction(RepeatForever::create(actionrepeat));
    spriteLightOff->runAction(RepeatForever::create(RotateBy::create(1,30)));
    
    this->imgPrice = static_cast<Text*>(this->pnlBg->getChildByName("imgPrice"));
    this->imgPrice->setString("---");
    auto imgPrice_0 = static_cast<Text*>(this->pnlBg->getChildByName("imgPrice_0"));
    imgPrice_0->setString(dataManager.GetSysString(901));
    this->createListBet();
    this->mQueueMsg->setFunCallBack(this, callfunc_selector(LayerPlayVQMM::OnExtensionResponseProcess));
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPlayVQMM::updateEvent),this,1/60, false);
    
    GameServer::getSingleton().addListeners(this);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt("tm",0);
    boost::shared_ptr<IRequest> req(new ExtensionRequest("vqj", params));
    GameServer::getSingleton().Send(req);
    
    
    this->btnPlay->setEnabled(true);
    this->btnPlay->setBright(true);
    //cheat
    txtKetqua = static_cast<TextField*>(pnlBg->getChildByName("txtKetqua"));
//    //    txtQT = static_cast<TextField*>(pnlBg->getChildByName("txtQT"));
//    //
//    //    Button* btnNhanQT = static_cast<Button*>(pnlBg->getChildByName("btnNhanQT"));
//    //    if (btnNhanQT) {
//    //        btnNhanQT->addTouchEventListener(CC_CALLBACK_2(LayerPlayGameMiniPoker::onBtnNhanQT, this));
//    //        //this->btn10k->setPressedActionEnabled(true);
//    //    }
//    //
    Button* btnKetQua = static_cast<Button*>(pnlBg->getChildByName("btnDoiTien"));
    if (btnKetQua) {
        btnKetQua->addTouchEventListener(CC_CALLBACK_2(LayerPlayVQMM::onBtnKetQua, this));
        //this->btn10k->setPressedActionEnabled(true);
    }
    
//    Scene *scene = Director::getInstance()->getRunningScene();
//    LayerHuThuong* hu = (LayerHuThuong*)scene->getChildByTag(19802);
//    if (hu){
//        hu->setMoney(mUtils::convertMoneyEx(1000000));
//    }
//    
       // this->setScale(0.8);
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->hideWebView();
    }
    return true;
}
void LayerPlayVQMM::onBtnKetQua(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        
        string strCheat = this->txtKetqua->getString();
        if (strCheat == "")
            return;
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();

        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString("glstc", strCheat);
        params->PutUtfString("uid", mySelf->Name());
        boost::shared_ptr<IRequest> req(new ExtensionRequest("vqctreq", params));
        GameServer::getSingleton().Send(req);
    }
}
void LayerPlayVQMM::createListBet()
{
    /*
#define MAT_LUOT_VQMM 5
#define NO_HU_VQMM 1
#define HAU_TO_EXP 2
#define HAU_TO_GOLD 0
#define HAU_TO_SILVER 1
 */
  
    this->listBet3.push_back(1);
    this->listBet3.push_back(500001);
    this->listBet3.push_back(30000);
    this->listBet3.push_back(300001);
    this->listBet3.push_back(5000);
    this->listBet3.push_back(200001);
    this->listBet3.push_back(200000);
    this->listBet3.push_back(1502);

    
    this->listBet2.push_back(1);
    this->listBet2.push_back(2002);
    this->listBet2.push_back(50000);
    this->listBet2.push_back(5000);
    this->listBet2.push_back(300001);
    this->listBet2.push_back(200000);
    this->listBet2.push_back(100001);
    this->listBet2.push_back(10000);
    this->listBet2.push_back(200001);
    this->listBet2.push_back(502);
    this->listBet2.push_back(100000);
    this->listBet2.push_back(5);

    this->listBet1.push_back(1);
    this->listBet1.push_back(50001);
    this->listBet1.push_back(5000);
    this->listBet1.push_back(150001);
    this->listBet1.push_back(10000);
    this->listBet1.push_back(100001);
    this->listBet1.push_back(50000);
    this->listBet1.push_back(5);
}


void LayerPlayVQMM::onBtnMuaLuotQuay(Ref* pSender){
    
    auto node = (Button*)pSender;
    int tag = node->getTag();
    
    
    
    if (this->isDangQuay){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(118));
        return;
    }
    
    if (!this->isJoinGame){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(119));
        return;
    }
    
    
    if (SceneManager::getSingleton().getMoneyByType(0) < this->gia){
        //if (this->currTypeMoney == 0)
            SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
        //SceneManager::getSingleton().showToast(dataManager.GetSysString(120));
        return;

    }
    
    this->getMinMoney();
            if (SceneManager::getSingleton().getCurrRoomType() == 0){
                if (this->gia*tag > (SceneManager::getSingleton().getMoneyByType(0) - this->MinBetValue)){
                   // if (this->currTypeMoney == 0)
                        SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
                    return;
                }
            }

    
    
    if (tag == 1){
        
    }
    
    
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString(EXT_FIELD_GAME_BET_VALUE,"");
    params->PutUtfString(EXT_FIELD_GAME_BET_TYPE,StringUtils::format("%d",tag));
    boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ_VQMM, params));
    GameServer::getSingleton().Send(req);
    
    SceneManager::getSingleton().showLoading();
    
}
void LayerPlayVQMM::startGame(){
    if (this->rs == "")
        return;
    
    if (mUtils::isSoundOn()){
        effect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("bm-vqmm.mp3");
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
    }
    
    auto lst = mUtils::splitString(this->rs, ';');
    if (lst.size()<6) return;
    this->result = lst[1]+ ";"+lst[2]+ ";"+lst[3]+ ";"+lst[4];
    if (atoi(lst[1].c_str()) == 0 && atoi(lst[2].c_str()) == 0 &&atoi(lst[3].c_str()) == 0 &&atoi(lst[4].c_str()) == 0){
        this->typePrize =MAT_LUOT_VQMM ;
    }
    for (int k = 6; k < lst.size() ; k++){
        int resultBet= 0;
        auto lstphanthuong = mUtils::splitString(lst[k], ',');
        /*#define MAT_LUOT_VQMM 5
         #define NO_HU_VQMM 1
         #define HAU_TO_EXP 2
         #define HAU_TO_GOLD 0
         #define HAU_TO_SILVER 1*/
        if (atoi(lstphanthuong[1].c_str()) > 0){
            resultBet = 1;
            this->isNoHu = true;
        }
        if (atoi(lstphanthuong[2].c_str()) > 0){
            string str = lstphanthuong[2] + "0";
            resultBet = atoi(str.c_str());
        }
        if (atoi(lstphanthuong[3].c_str()) > 0){
            string str = lstphanthuong[3] + "1";
            resultBet = atoi(str.c_str());
        }
        if (atoi(lstphanthuong[4].c_str()) > 0){
            string str = lstphanthuong[4] + "2";
            resultBet = atoi(str.c_str());
            
            
        }
        if (atoi(lstphanthuong[1].c_str()) == 0 && atoi(lstphanthuong[2].c_str()) == 0 &&atoi(lstphanthuong[3].c_str()) == 0 &&atoi(lstphanthuong[4].c_str()) == 0){
            resultBet = 5;
        }
        int rounds = rand() % 20 + 10;
        int slices = 0;
        int start_rand = 3;
        int rand_space = 10;
        int dis_value = 1;
        float duration = 10;
        float delayTime = 0.3;
        
        int rand_angle_bonus = rand() % rand_space + start_rand;
        float degress = 0;
       // log("DEGRESS %.3f", degress);
        float angle = 0;//goc quay
        ActionInterval *rotate;
        //EaseOut *easeout = EaseOut::create(rotate, 6);
        EaseCircleActionOut *easeout ;
        // setStateArrButtonBuy(false);
        Sequence* seqAction ;
        switch (atoi(lstphanthuong[0].c_str())){
            case 1:
                slices = this->listBet1.size();
                for (int i = 0; i < listBet1.size(); i++)
                {
                    if (this->listBet1[i] == resultBet)
                    {
                        prize = i;
                        break;
                    }
                }
                degress = (slices - dis_value - prize)*(360 / slices);
                angle = 360 * rounds + degress + rand_angle_bonus;
                rotate = RotateTo::create(duration, angle);
                //EaseOut *easeout = EaseOut::create(rotate, 6);
                easeout = EaseCircleActionOut::create(rotate->clone());
                // setStateArrButtonBuy(false);
                seqAction = Sequence::create(easeout,
                                             DelayTime::create(delayTime),
                                             CallFunc::create(CC_CALLBACK_0(LayerPlayVQMM::actionRotateDone, this)), nullptr);
                this->imgVong1->runAction(seqAction);
                break;
            case 2:
                slices = this->listBet2.size();
                for (int i = 0; i < listBet2.size(); i++)
                {
                    if (this->listBet2[i] == resultBet)
                    {
                        prize = i;
                        break;
                    }
                }
                degress = (slices - dis_value - prize)*(360 / slices);
                angle = 360 * rounds + degress + rand_angle_bonus;
                rotate = RotateTo::create(duration, angle);
                //EaseOut *easeout = EaseOut::create(rotate, 6);
                easeout = EaseCircleActionOut::create(rotate->clone());
                // setStateArrButtonBuy(false);
                seqAction = Sequence::create(easeout,
                                             DelayTime::create(delayTime),
                                             CallFunc::create(CC_CALLBACK_0(LayerPlayVQMM::actionRotateDone, this)), nullptr);
                this->imgVong2->runAction(seqAction);
                break;
            case 3:
                slices = this->listBet3.size();
                for (int i = 0; i < listBet3.size(); i++)
                {
                    if (this->listBet3[i] == resultBet)
                    {
                        prize = i;
                        break;
                    }
                }
                degress = (slices - dis_value - prize)*(360 / slices);
                angle = 360 * rounds + degress + rand_angle_bonus;
                rotate = RotateTo::create(duration, angle);
                //EaseOut *easeout = EaseOut::create(rotate, 6);
                easeout = EaseCircleActionOut::create(rotate->clone());
                // setStateArrButtonBuy(false);
                seqAction = Sequence::create(easeout,
                                             DelayTime::create(delayTime),
                                             CallFunc::create(CC_CALLBACK_0(LayerPlayVQMM::actionRotateDone, this)), nullptr);
                this->imgVong3->runAction(seqAction);
                break;
        }

    }
   
}
void LayerPlayVQMM::actionRotateDone(){
    if (mUtils::isSoundOn()){
        CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(effect);
    }
    if (this->typePrize ==MAT_LUOT_VQMM ){
        this->resetGame();
        return;
    }
    
    
    
    if (this->isNoHu){
        auto lst = mUtils::splitString(this->result, ';');
        if (lst.size() < 4) return;
        long anhu = atol(lst[1].c_str());
        Scene *scene = Director::getInstance()->getRunningScene();
        LayerHuThuong* hu = (LayerHuThuong*)scene->getChildByTag(19802);
        if (hu){
            hu->setMoney(mUtils::convertMoneyEx(anhu));
        }
        this->resetGame();
    }
    else{
        
        Sprite* bgkq = Sprite::create("VQMMMini/img-ketqua.png");
        bgkq->setPosition(Vec2(this->pnlBg->getContentSize().width/2,300));

        auto lst = mUtils::splitString(this->result, ';');
        if (lst.size() < 4) return;
        string ketqua = dataManager.GetSysString(558)+"\n";
        if (atoi(lst[1].c_str())>0)
            ketqua += "+ " +mUtils::convertMoneyEx(atoi(lst[1].c_str())) + " " + SceneManager::getSingleton().getCoin(0) + " ";
        if (atoi(lst[2].c_str())>0)
            ketqua +="+ " + mUtils::convertMoneyEx(atoi(lst[2].c_str())) + " " + SceneManager::getSingleton().getCoin(1) + " ";
        if (atoi(lst[3].c_str())>0)
            ketqua += "+ " +mUtils::convertMoneyEx(atoi(lst[3].c_str())) + " EXP ";
        Label* labelResult = Label::createWithSystemFont(ketqua, "", 45);
        labelResult->setColor(Color3B::YELLOW);
        labelResult->setAlignment(cocos2d::TextHAlignment::CENTER);
        labelResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        labelResult->setPosition(Vec2(bgkq->getContentSize().width/2,bgkq->getContentSize().height/2));
        bgkq->addChild(labelResult);
        this->pnlBg->addChild(bgkq);
        //labelResult->setTag(TAG_RESULT_MONEY);
        bgkq->runAction(Sequence::create(DelayTime::create(2),FadeOut::create(0.2), RemoveSelf::create(true), NULL));
        this->resetGame();
    }
}
void LayerPlayVQMM::resetGame(){
    this->btnPlay->setEnabled(true);
    this->btnPlay->setBright(true);
    this->isMuaLuotQuay = false;
    this->btnVong3->setEnabled(true);
    this->btnVong3->setBright(true);
    this->btnVong2->setEnabled(true);
    this->btnVong2->setBright(true);
    this->btnVong1->setEnabled(true);
    this->btnVong1->setBright(true);
    this->isMuaLuotQuay = false;
    this->currentVongQuay = 1;
    this->rs = "";
    this->prize = -1;
    this->typePrize = -1;
    this->value = 0;
    this->isDangQuay = false;
    this->result = "";
    this->isNoHu = false;
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    if (mySelf)
    {
        boost::shared_ptr<UserVariable> rotatebonus = mySelf->GetVariable("rotatebonus");
        if (rotatebonus)
        {
            int rotatebonus_value = *rotatebonus->GetIntValue();
            if (rotatebonus_value<=0){
                this->btnVong3->setTitleText("Free");
                this->isMuaLuotQuay = true;
                this->btnVong3->setEnabled(false);
                this->btnVong3->setBright(false);
                this->btnVong3->setVisible(false);
                this->btnPlay->setEnabled(true);
            }else{
                this->btnVong3->setVisible(true);

            }
        }
    }
}
void LayerPlayVQMM::event_EXT_EVENT_GAME_RESULT() {

    boost::shared_ptr<string> grs = this->param->GetUtfString(EXT_FIELD_GAME_VQ_RESULT);
    boost::shared_ptr<string> eexp = this->param->GetUtfString("eexp");

    boost::shared_ptr<long> errc = this->param->GetInt("errc");

//    public static final String EXT_FIELD_ERROR_STRING = "eexp";
//    public static final String EXT_FIELD_ERROR_CODE = "errc";
//    boost::shared_ptr<string> lst7lucky = this->param->GetUtfString("lst7");
//    boost::shared_ptr<long> has7 = this->param->GetInt("has7");
    if (errc == NULL || eexp == NULL) return;
    if (*errc == 0){
        if (grs!= NULL){
            this->rs = *grs;
            this->startGame();
        }
    }
    else{
        SceneManager::getSingleton().showToast(*eexp);
        this->resetGame();
    }
//    if (rc != NULL)
//        this->currCodeRS = atof((*rc).c_str());
//    this->stopRound(this->strResult);
//    if (lst7lucky != NULL){
//        this->strlst7 = *lst7lucky;
//    }
//    if (has7 != NULL){
//        if (*has7 == 1) {
//            this->ishas7 = true;
//        }
//    }
}
void LayerPlayVQMM::event_EXT_EVENT_GAME_STATE_NTF(){
    boost::shared_ptr<long> vqbr = this->param->GetInt(EXT_FIELD_GAME_VQ_BOUGHT_ROUND);
    boost::shared_ptr<long> vqprice = this->param->GetInt(EXT_FIELD_GAME_VQ_PRICE);
    this->isJoinGame = true;
    SceneManager::getSingleton().showToast(dataManager.GetSysString(121));
//
//    if (vqbr){
//        this->currentVongQuay = *vqbr;
//       // this->currentVongQuay = atoi(vongmua.c_str());
//        switch(this->currentVongQuay){
//            case 1:
//                this->isMuaLuotQuay = true;
//                this->btnVong1->setEnabled(false);
//                this->btnVong1->setBright(false);
//                this->btnPlay->setEnabled(true);
//                break;
//            case 2:
//                this->isMuaLuotQuay = true;
//                this->btnVong2->setEnabled(false);
//                this->btnVong2->setBright(false);
//                this->btnPlay->setEnabled(true);
//                break;
//            case 3:
//
//                this->isMuaLuotQuay = true;
//                this->btnVong3->setEnabled(false);
//                this->btnVong3->setBright(false);
//                this->btnPlay->setEnabled(true);
//                break;
//
//        }
//    }
    if (vqprice){
        this->gia = *vqprice;
        this->imgPrice->setString(mUtils::convertMoneyEx(gia*3));
    }

}
void LayerPlayVQMM::updateEvent(float dt){
    if (this->mQueueMsg->isQueueEmpty())
        return;
    this->mQueueMsg->run();
}
void LayerPlayVQMM::onBtnClose(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        // this->getParent().imgBGTimer->setVisible(true);
//        LayerGameTaiXiuMini* parent = (LayerGameTaiXiuMini*)this->getParent();
//        if (parent)
//            parent->imgBGTimer->setVisible(true);
        this->removeFromParentAndCleanup(true);
    }
}
void LayerPlayVQMM::getMinMoney(){
    if (SceneManager::getSingleton().getGameID() == kGameLieng || SceneManager::getSingleton().getGameID() == kGameXiTo || SceneManager::getSingleton().getGameID() == kGamePoker)
    {
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        if (mySelf)
        {
            boost::shared_ptr<UserVariable> gameChip_ptr = mySelf->GetVariable(EXT_FIELD_GAME_CHIP);
            if (gameChip_ptr)
            {
                this->MinBetValue = *gameChip_ptr->GetDoubleValue();
            }
        }
    }
    else{
        vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
        int gameID = SceneManager::getSingleton().getGameID();
        int bet_ratio = 1;
        for (int i = 0; i < arrBet.size(); ++i)
        {
            if (arrBet.at(i).gameid == gameID)
            {
                bet_ratio = arrBet.at(i).ratioBet;
                break;
            }
        }
        string money = "0";
        boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
        if (room != NULL && room->GetVariable("params") != NULL) {
            std::string paramString = *room->GetVariable("params")->GetStringValue();
            std::vector<std::string> arrInfo = mUtils::splitString(paramString, '@');
            if (arrInfo.size() > 0) {
                money = arrInfo.at(0);
                if (SceneManager::getSingleton().getGameID() == kGameTaiXiu || SceneManager::getSingleton().getGameID() == kGameXocDia)
                    money = "0";
            }
        }
        this->MinBetValue = bet_ratio*(atoi(money.c_str()));
    }
}
void LayerPlayVQMM::onBtnPlay(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {

        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        int rotatebonus_value = 0;
        if (mySelf)
        {
            boost::shared_ptr<UserVariable> rotatebonus = mySelf->GetVariable("rotatebonus");
            if (rotatebonus)
            {
                rotatebonus_value = *rotatebonus->GetIntValue();
            }
        }
        if (rotatebonus_value <=0){
        if (SceneManager::getSingleton().getMoneyByType(0) < this->gia*3){
            //if (this->currTypeMoney == 0)
            SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
            //SceneManager::getSingleton().showToast(dataManager.GetSysString(120));
            return;
            
        }
        
        this->getMinMoney();
        if (SceneManager::getSingleton().getCurrRoomType() == 0){
            if (this->gia*3 > (SceneManager::getSingleton().getMoneyByType(0) - this->MinBetValue)){
                // if (this->currTypeMoney == 0)
                SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
                return;
            }
        }
        }

//        if (this->Animating){
//            auto toast = Chat::create(dataManager.GetSysString(421),-1);
//            this->addChild(toast);
//            return;
//        }
//        if (this->autoPlay){
//            auto toast = Chat::create(dataManager.GetSysString(420),-1);
//            this->addChild(toast);
//            return;
//        }
//
//
//        //this->showResult();
//        if (this->currBetMoney > this->currMyMoney  ) {
//            auto toast = Chat::create(dataManager.GetSysString(409), -1);
//            pnlBg->addChild(toast);
//            return;
//        }
//        if (SceneManager::getSingleton().getCurrRoomType() == this->currTypeMoney){
//            if (this->currBetMoney > (this->currMyMoney - this->MinBetValue)){
//                auto toast = Chat::create(dataManager.GetSysString(409), -1);
//                this->addChild(toast);
//                return;
//            }
//        }
//        this->currMyMoney -=this->currBetMoney;
        //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->currMyMoney));
        
        
        SceneManager::getSingleton().setLastGameID(132);

        boost::shared_ptr<ISFSObject> params(new SFSObject());
//        params->PutUtfString(EXT_FIELD_GAME_BET_VALUE,StringUtils::format("%d",this->currBetMoney));
//        params->PutUtfString(EXT_FIELD_GAME_BET_TYPE,StringUtils::format("%d",""));
        boost::shared_ptr<IRequest> req(new ExtensionRequest(EXT_EVENT_VQ_ROTATE_REQ, params));
        GameServer::getSingleton().Send(req);
        this->btnPlay->setEnabled(false);
        this->btnPlay->setBright(false);
        this->isDangQuay= true;
        
        ///Cheat
        
//        string strCheat = "10_1;14_1;13_1;12_1;11_1";
//        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
//        
//        boost::shared_ptr<ISFSObject> paramscheat(new SFSObject());
//        paramscheat->PutUtfString("glstc", strCheat);
//        paramscheat->PutUtfString("uid", mySelf->Name());
//        boost::shared_ptr<IRequest> reqcheat(new ExtensionRequest("pkctreq", paramscheat));
//        GameServer::getSingleton().Send(reqcheat);
    }
}
void LayerPlayVQMM::onBtnSuKien(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        LayerPopupWebView* _layer = LayerPopupWebView::create();
        string url = LINK_EVENT_MINIPOKER;
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
            url = SKTX_P2;
        _layer->setWebView(url);
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(_layer, 2000);
    }
}
void LayerPlayVQMM::onBtnHuongDan(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        LayerGuideInGame* layer = LayerGuideInGame::create();
        string url = GUIDE_LINK;
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
            url = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+GUIDE_P2_LINK;
        layer->setLink(StringUtils::format("%s%d", url.c_str(), 1401));
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(layer, 2000);
    }
}
void LayerPlayVQMM::onBtnLichSu(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        // this->getParent().imgBGTimer->setVisible(true);
        LayerLichSuVQMM* lichsu = LayerLichSuVQMM::create();
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(lichsu);
        lichsu->setLocalZOrder(2000);
    }
}
void LayerPlayVQMM::onBtnXepHang(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        // this->getParent().imgBGTimer->setVisible(true);
        LayerXepHangVQMM* xephang = LayerXepHangVQMM::create();
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return;
        scene->addChild(xephang);
        xephang->setLocalZOrder(2000);
    }
}
void LayerPlayVQMM::showResult() {
//    if (this->currCodeRS>0) {
//        auto imgRS = ImageView::create(this->getStrImageRS(this->currCodeRS));
//        imgRS->setPosition(this->imgResult->getPosition());
//        imgRS->setAnchorPoint(this->imgResult->getAnchorPoint());
//        imgRS->setTag(this->TAG_RESULT_IMG);
//        this->pnlBg->addChild(imgRS,10000000);
//        imgRS->runAction(Sequence::create(ScaleTo::create(0.5,1.3),ScaleTo::create(0.5,1),DelayTime::create(2),FadeOut::create(0.2),RemoveSelf::create(true), NULL));
//    }
    
    
    
//        //string font_name = "font_win-font.fnt";
//    string money = StringUtils::format("+%s",mUtils::convertMoneyEx(this->winMoney).c_str());
//    Label* labelResult = Label::createWithTTF(money, "__Roboto-Bold.ttf", 30);
//    labelResult->setColor(Color3B::YELLOW);
//    labelResult->setAnchorPoint(Vec2(0.5,0.5));
//    
//    labelResult->setPosition(Vec2(412,160));
//    this->pnlBg->addChild(labelResult);
//    labelResult->setTag(this->TAG_RESULT_MONEY);
//    labelResult->runAction(Sequence::create(MoveBy::create(1,Vec2(0,50)),DelayTime::create(2),FadeOut::create(0.2),RemoveSelf::create(true), NULL));
//    
//    this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create(CC_CALLBACK_0(LayerPlayVQMM::resetGame, this)),NULL));
//    //this->txtMyMoney->setString(mUtils::convertMoneyEx(this->currMyMoney));
//    if (this->isNoHu)
//        this->txtTongHu->setString(mUtils::convertMoneyEx(this->currHuThuong));
//    
//    this->isNoHu = false;
    
    
}
bool LayerPlayVQMM::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    auto rect = this->pnlBg->getBoundingBox();
    auto locationInNode = pTouch->getLocation();
    if (rect.containsPoint(locationInNode)){
        return true;
    }
    //var s = target.getContentSize();
    return false;
}
//Listen Server Event
void LayerPlayVQMM::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (0 == cmd->compare("vqe_bntf")) {
//        this->param = pObjs;
//        boost::shared_ptr<long> errc = this->param->GetInt("errc");
//       // log("errc minipoker :%d",*errc);
//        this->stopRound("2_1;5_4;10_2;14_1;5_3");
//        auto toast = Chat::create("Xảy ra lỗi vui lòng thử lại!",-1);
//        this->addChild(toast);
//        return;
    }
    if (0 != cmd->compare(EXT_EVENT_VQ_ROTATE_RESP)
        && 0 != cmd->compare(EXT_EVENT_GAME_END_VQMM)
        && 0 != cmd->compare(EXT_EVENT_GAME_BET_NTF_VQMINI)
        && 0 != cmd->compare(EXT_EVENT_GAME_STATE_NTF_VQMINI)
        && 0 != cmd->compare(EXT_EVENT_GAME_BET_ERROR_NOTIFY_VQMINI)) {
        return;
    }
    pObjs->PutUtfString("_cmd_", *cmd);
    this->mQueueMsg->addQueue(pObjs);
}

void LayerPlayVQMM::OnExtensionResponseProcess(){
    this->param = this->mQueueMsg->frontQueue();
    boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");

    if (0 == cmd->compare(EXT_EVENT_VQ_ROTATE_RESP)) {
        this->event_EXT_EVENT_GAME_RESULT();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_END_VQMM)){
        this->event_EXT_EVENT_GAME_END();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_BET_NTF_VQMINI)) {
        this->event_EXT_EVENT_GAME_BET_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_STATE_NTF_VQMINI)) {
        this->event_EXT_EVENT_GAME_STATE_NTF();
    }
    else if (0 == cmd->compare(EXT_EVENT_GAME_BET_ERROR_NOTIFY_VQMINI)) {
        this->event_EXT_EVENT_GAME_BET_ERROR_NOTIFY_VQMINI();
    }
    this->mQueueMsg->deQueue();
}
void LayerPlayVQMM::event_EXT_EVENT_GAME_BET_ERROR_NOTIFY_VQMINI(){
    boost::shared_ptr<long> errc = this->param->GetInt(EXT_FIELD_ERROR_CODE);
    if (errc != NULL) {
        SceneManager::getSingleton().hideLoading();
        int errcode = *errc;
        if (errcode == RESCODE_GAME_ALREADY_BUY_LOTTERY) {
            // this->Toast.ShowToast("Bạn không đủ tiền để chơi");
            SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(966));
        }
        if (errcode == RESCODE_GAME_IS_PLAYING){
            SceneManager::getSingleton().showToast(dataManager.GetSysString(123));

        }
        if (errcode == 32){
            SceneManager::getSingleton().showToast(dataManager.GetSysString(124));
            
        }
    }
        
}
void LayerPlayVQMM::event_EXT_EVENT_GAME_BET_NTF(){
    boost::shared_ptr<string> number = this->param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
    SceneManager::getSingleton().hideLoading();
    this->isMuaLuotQuay = true;
    this->btnPlay->setEnabled(true);
    if (number != NULL) {
        // this->Toast.ShowToast("Bạn không đủ tiền để chơi");
       // SceneManager::getSingleton().showToast(StringUtils::format("Bạn đã mua lượt quay vòng %s!",number->c_str()));
        this->currentVongQuay = atoi(number->c_str());
        switch(this->currentVongQuay){
            case  1:
                this->btnVong2->setEnabled(true);
                this->btnVong2->setBright(true);
                this->btnVong3->setEnabled(true);
                this->btnVong3->setBright(true);
                this->btnVong1->setEnabled(false);
                this->btnVong1->setBright(false);
                break;
            case  2:
                this->btnVong1->setEnabled(true);
                this->btnVong1->setBright(true);
                this->btnVong3->setEnabled(true);
                this->btnVong3->setBright(true);
                this->btnVong2->setEnabled(false);
                this->btnVong2->setBright(false);
                break;
            case  3:
                this->btnVong2->setEnabled(true);
                this->btnVong2->setBright(true);
                this->btnVong1->setEnabled(true);
                this->btnVong1->setBright(true);
                this->btnVong3->setEnabled(false);
                this->btnVong3->setBright(false);
                break;
        }
    }
    SceneManager::getSingleton().hideLoading();
}
void LayerPlayVQMM::event_EXT_EVENT_GAME_END(){
    
    boost::shared_ptr<long> number = this->param->GetInt(EXT_FIELD_GAME_AUTO_AMOUNT);
//    if (number != NULL){
//        this->numberAuto = *number;
//        if (this->numberAuto > 0){
//            this->autoPlay = true;
//        }
//        else{
//            this->autoPlay = false;
//            this->imgAutoPlay->setVisible(false);
//        }
//    }

}

//void LayerPlayVQMM::event_EXT_EVENT_GAME_AUTO_RESP(){
//    boost::shared_ptr<long> number = this->param->GetInt(EXT_FIELD_GAME_AUTO_AMOUNT);
//    boost::shared_ptr<long> errc = this->param->GetInt(EXT_FIELD_ERROR_CODE);
//
//    if (number != NULL){
//        this->numberAuto = *number;
//        if (this->numberAuto > 0){
//            this->autoPlay = true;
//            auto toast = Chat::create("Bạn đang để chế độ auto play!",-1);
//            this->addChild(toast);
//        }
//        else{
//            if (errc != NULL){
//                int ec = *errc;
//                if (ec == 1){
//                    auto toast = Chat::create("Bạn không đủ tiền để chơi tiếp!",-1);
//                    this->addChild(toast);
//                }
//            }
//            this->autoPlay = false;
//            this->imgAutoPlay->setVisible(false);
//        }
//    }
//    this->btnAutoPlay->setEnabled(true);
//}
void LayerPlayVQMM::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    if (ptrEvetnParams == NULL)
        return;
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEvetnParams)["user"];
    boost::shared_ptr<User> user = ((boost::static_pointer_cast<User>(ptrEventParamValueUser)));
    //
    if (user != NULL)
    {
        string myName = SceneManager::getSingleton().getMyName();
        if (0 == myName.compare(*user->Name()))
        {
            // url
            
            boost::shared_ptr<UserVariable> aalptr = user->GetVariable("aal");
                boost::shared_ptr<UserVariable> amfptr = user->GetVariable("amf");
                if (amfptr)
                {
                    boost::shared_ptr<double> amf = amfptr->GetDoubleValue();
                    if (amf)
                    {
                        this->currMyMoney = *amf;
//                        if (!this->Animating && !this->CurrentGameState)
//                            this->txtMyMoney->setString(_money);
                        
                        //this->amfUser = this->currMyMoney;
                    }
                }
            boost::shared_ptr<UserVariable> rotatebonus = user->GetVariable("rotatebonus");
            if (rotatebonus)
            {
                int rotatebonus_value = *rotatebonus->GetIntValue();
                if (rotatebonus_value<=0){
                    this->btnVong3->setTitleText("Free");
                    //this->isMuaLuotQuay = false;
                    this->btnVong3->setVisible(false);
                }else{
                    this->btnVong3->setTitleText("Free");

                    this->btnVong3->setVisible(true);

                }
            }

        }
    }
}
void LayerPlayVQMM::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    auto target = this->pnlBg;
    auto delta = pTouch->getDelta();
    //  if (target.x ==0 || target.x == WIDTH_DESIGN-143 || target.y ==0 || target.y == HEIGHT_DESIGN-149)
    //     return;
    auto x = target->getPositionX() + delta.x;
    auto y = target->getPositionY() + delta.y;
    
    this->pnlBg->setPosition(Vec2(x,y));
    //    if (abs(delta.x) > 10 || abs(delta.y) > 10)
    //        this->isTouchMove =true;
    //    //listener->setSwallowTouches(true);
}
void LayerPlayVQMM::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPlayVQMM::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerPlayVQMM::onTouchMoved, this);

    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton()->addListeners(this);
}
void LayerPlayVQMM::onExit()
{
    Layer::onExit();
    //dataManager.getAppConfig().isLoginFisrt = false;
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(effect);
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("aI", SceneManager::getSingleton().getMyName());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("rgaci", params));
    GameServer::getSingleton().Send(request);

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(listener);
    GameServer::getSingleton().removeListeners(this);
    SceneManager::getSingleton().setIsShowVQ(false);
    SceneManager::getSingleton().setIsShowVQBonus(true);
    if (!SceneManager::getSingleton().getIsShowTX() && !SceneManager::getSingleton().getIsShowXD()&& !SceneManager::getSingleton().getIsShowPK()&& !SceneManager::getSingleton().getIsShowVQ()&& !SceneManager::getSingleton().getIsShowCT()){
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return ;
        ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
        if (_scene)
        {
            _scene->showWebView();
        }
    }
    //SceneMain::getSingleton().gotoDiemDanh();
    boost::shared_ptr<ISFSObject> params2(new SFSObject());
    boost::shared_ptr<IRequest> req2(new ExtensionRequest("vql", params2));
    GameServer::getSingleton().Send(req2);

    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPlayVQMM::updateEvent),this);

}
