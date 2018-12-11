//
//  LayerGuide.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerGuide.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "LayerPopupDaiLy.h"
#include "../layergames/_Chat_.h"
#include "../scenes/ScenePickRoom.h"
#include "../scenes/SceneMain.h"
#include "ItemMenuGame.h"
LayerGuide::LayerGuide()
{

}


LayerGuide::~LayerGuide()
{

}

bool LayerGuide::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto rootNode = CSLoader::getInstance()->createNode("LayerGuideGame.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("pnlBG"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    Panel_2->setAnchorPoint(Point(0.5, 0.5));
    Panel_2->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
  //  if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);
    //rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->pnlOption = static_cast<Layout*>(rootNode->getChildByName("pnlOption"));

    Button* btnNo = dynamic_cast<Button*>(this->pnlOption->getChildByName("btnNo"));
    if(btnNo){
        btnNo->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnNo, this));
        btnNo->setTitleText(dataManager.GetSysString(1001));
    }

    Button* btnYes = dynamic_cast<Button*>(this->pnlOption->getChildByName("btnYes"));
    if(btnYes){
        btnYes->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnYes, this));
        btnYes->setTitleText(dataManager.GetSysString(1002));
    }
    
    this->addChild(rootNode);
    
    
    auto txtLabel = dynamic_cast<Text*>(this->pnlOption->getChildByName("txtLabel"));
    txtLabel->setString("Naga Club");
    if (SceneManager::getSingleton().isNagaNew){
        txtLabel->setString("Reahu Khmer");
    }
    if (SceneManager::getSingleton().isNagaWin){
        txtLabel->setString("Win Club");
    }
    if (SceneManager::getSingleton().isNagaCard){
        txtLabel->setString("Naga Card");
    }
    if (SceneManager::getSingleton().isMegaWin){
        txtLabel->setString("Mega Win");
    }
    
    auto txtContent = dynamic_cast<Text*>(this->pnlOption->getChildByName("txtContent"));
    txtContent->setString(dataManager.GetSysString(1000));

    this->pnlGuide1 = static_cast<Layout*>(rootNode->getChildByName("pnlGuide1"));
    this->pnlGuide2 = static_cast<Layout*>(rootNode->getChildByName("pnlGuide2"));
    this->pnlGuide3 = static_cast<Layout*>(rootNode->getChildByName("pnlGuide3"));
    this->pnlGuide4 = static_cast<Layout*>(rootNode->getChildByName("pnlGuide4"));
    this->pnlGuide5 = static_cast<Layout*>(rootNode->getChildByName("pnlGuide5"));
    this->pnlGuide6 = static_cast<Layout*>(rootNode->getChildByName("pnlGuide6"));
    this->pnlGuide7 = static_cast<Layout*>(rootNode->getChildByName("pnlGuide7"));
    this->pnlGuide8 = static_cast<Layout*>(rootNode->getChildByName("pnlGuide8"));
    this->pnlEnd = static_cast<Layout*>(rootNode->getChildByName("pnlEnd"));

    this->pnlOption->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    this->pnlGuide1->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    this->pnlGuide6->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    this->pnlGuide7->setPosition(Point(visibleSize.width/2,visibleSize.height/2+sizeAdd.height));
    this->pnlGuide8->setPosition(Point(visibleSize.width/2,visibleSize.height/2+sizeAdd.height));
    this->pnlEnd->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    
    auto txtLabelEnd = dynamic_cast<Text*>(this->pnlEnd->getChildByName("txtLabel"));
    txtLabelEnd->setString("Naga Club");
    if (SceneManager::getSingleton().isNagaNew){
        txtLabelEnd->setString("Reahu Khmer");
    }
    if (SceneManager::getSingleton().isNagaWin){
        txtLabelEnd->setString("Win Club");
    }
    if (SceneManager::getSingleton().isNagaCard){
        txtLabelEnd->setString("Naga Card");
    }
    if (SceneManager::getSingleton().isMegaWin){
        txtLabelEnd->setString("Mega Win");
    }

    this->pnlOption->setVisible(true);
    this->pnlOption->setScale(0);
    this->pnlOption->runAction(Sequence::create(DelayTime::create(0.2),ScaleTo::create(0.2, 1),NULL));
    this->pnlGuide1->setVisible(false);
    this->pnlGuide2->setVisible(false);
    this->pnlGuide3->setVisible(false);
    this->pnlGuide4->setVisible(false);
    this->pnlGuide5->setVisible(false);
    this->pnlGuide6->setVisible(false);
    this->pnlGuide7->setVisible(false);
    this->pnlGuide8->setVisible(false);
    this->pnlEnd->setVisible(false);

    //this->txtChuChay->setScale(5);
    return true;
}
void LayerGuide::onBtnCancelGuide(Ref* pSender){
    this->showEnd();
}
void LayerGuide::showEnd(){
    this->pnlGuide1->setVisible(false);
    this->pnlGuide2->setVisible(false);
    this->pnlGuide3->setVisible(false);
    this->pnlGuide4->setVisible(false);
    this->pnlGuide5->setVisible(false);
    this->pnlGuide6->setVisible(false);
    this->pnlGuide7->setVisible(false);
    this->pnlGuide8->setVisible(false);
    this->pnlEnd->setVisible(true);
    this->pnlEnd->setScale(0);
    this->pnlEnd->runAction(Sequence::create(DelayTime::create(0.2),ScaleTo::create(0.2, 1),NULL));
    auto txtContent = dynamic_cast<Text*>(this->pnlEnd->getChildByName("txtContent"));
    txtContent->setString(dataManager.GetSysString(1022));
    
    
    Button* btnPlay = dynamic_cast<Button*>(this->pnlEnd->getChildByName("btnPlay"));
    if(btnPlay){
        btnPlay->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnEnd, this));
        btnPlay->setTitleText(dataManager.GetSysString(1023));
    }

}
void LayerGuide::onBtnEnd(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerGuide::showGuide8(){
    this->pnlGuide8->setVisible(true);
    
    this->imgTextGuide8 = static_cast<ImageView*>(this->pnlGuide8->getChildByName("imgTextGuide8"));
    this->btnNextGuide8 = static_cast<Button*>(this->imgTextGuide8->getChildByName("btnNextGuide8"));
    if(this->btnNextGuide8){
        this->btnNextGuide8->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnNextGuide8, this));
    }
    auto txtGuide81 = static_cast<Text*>(this->imgTextGuide8->getChildByName("txtGuide81"));
    txtGuide81->setString(dataManager.GetSysString(1021));
    
    
    this->btnCancelGuide8 = static_cast<Button*>(this->imgTextGuide8->getChildByName("btnCancelGuide8"));
    if(this->btnCancelGuide8){
        this->btnCancelGuide8->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnCancelGuide, this));
    }
    this->imgLightGuide8 = static_cast<ImageView*>(this->pnlGuide8->getChildByName("imgLightGuide8"));
    this->imgLightGuide8->setSwallowTouches(true);
    
    this->imgLightBGGuide8 = static_cast<ImageView*>(this->pnlGuide8->getChildByName("imgLightBGGuide8"));
    this->imgTextGuide8->setVisible(false);
    this->imgLightGuide8->setVisible(false);
    this->imgLightBGGuide8->setScale(0);
    this->imgLightBGGuide8->runAction(Sequence::create(DelayTime::create(0.2),ScaleTo::create(0.2, 1),CallFunc::create([this](){this->imgLightGuide8->setVisible(true);}),CallFunc::create([this](){this->imgLightBGGuide8->setVisible(false);}), NULL));
    this->imgTextGuide8->runAction(Sequence::create(DelayTime::create(0.65),FadeIn::create(0.1),CallFunc::create([this](){this->imgTextGuide8->setVisible(true);}), NULL));
}
void LayerGuide::onBtnNextGuide8(Ref* pSender){
    this->btnCancelGuide8->setVisible(false);
    this->btnNextGuide8->setVisible(false);
    this->imgLightBGGuide8->runAction(Sequence::create( CallFunc::create([this](){this->imgLightBGGuide8->setVisible(true);}),CallFunc::create([this](){this->imgLightGuide8->setVisible(false);}),ScaleTo::create(0.3, 0), NULL));
    this->imgTextGuide8->runAction(Sequence::create(DelayTime::create(0.45),FadeOut::create(0.1),CallFunc::create([this](){this->pnlGuide8->setVisible(false);}),CallFunc::create([this](){this->showEnd();}), NULL));
}
void LayerGuide::showGuide7(){
    this->pnlGuide7->setVisible(true);
    
    this->imgTextGuide7 = static_cast<ImageView*>(this->pnlGuide7->getChildByName("imgTextGuide7"));
    this->btnNextGuide7 = static_cast<Button*>(this->imgTextGuide7->getChildByName("btnNextGuide7"));
    if(this->btnNextGuide7){
        this->btnNextGuide7->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnNextGuide7, this));
    }
    auto txtGuide71 = static_cast<Text*>(this->imgTextGuide7->getChildByName("txtGuide71"));
    txtGuide71->setString(dataManager.GetSysString(1017));
    
    auto txtGuide72 = static_cast<Text*>(this->imgTextGuide7->getChildByName("txtGuide72"));
    txtGuide72->setString(dataManager.GetSysString(1018));
    
    auto txtGuide73 = static_cast<Text*>(this->imgTextGuide7->getChildByName("txtGuide73"));
    txtGuide73->setString(dataManager.GetSysString(1019));
    
    auto txtGuide74 = static_cast<Text*>(this->imgTextGuide7->getChildByName("txtGuide74"));
    txtGuide74->setString(StringUtils::format("%s (%s)",dataManager.GetSysString(1020).c_str(),dataManager.getAppConfig().fanpage.c_str()));
    
    this->btnCancelGuide7 = static_cast<Button*>(this->imgTextGuide7->getChildByName("btnCancelGuide7"));
    if(this->btnCancelGuide7){
        this->btnCancelGuide7->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnCancelGuide, this));
    }
    this->imgLightGuide7 = static_cast<ImageView*>(this->pnlGuide7->getChildByName("imgLightGuide7"));
    this->imgLightGuide7->setSwallowTouches(true);
    
    this->imgLightBGGuide7 = static_cast<ImageView*>(this->pnlGuide7->getChildByName("imgLightBGGuide7"));
    this->imgTextGuide7->setVisible(false);
    this->imgLightGuide7->setVisible(false);
    this->imgLightBGGuide7->setScale(0);
    this->imgLightBGGuide7->runAction(Sequence::create(DelayTime::create(0.2),ScaleTo::create(0.2, 1),CallFunc::create([this](){this->imgLightGuide7->setVisible(true);}),CallFunc::create([this](){this->imgLightBGGuide7->setVisible(false);}), NULL));
    this->imgTextGuide7->runAction(Sequence::create(DelayTime::create(0.65),FadeIn::create(0.1),CallFunc::create([this](){this->imgTextGuide7->setVisible(true);}), NULL));
}
void LayerGuide::onBtnNextGuide7(Ref* pSender){
    this->btnCancelGuide7->setVisible(false);
    this->btnNextGuide7->setVisible(false);
    this->imgLightBGGuide7->runAction(Sequence::create(CallFunc::create([this](){this->imgLightBGGuide7->setVisible(true);}),CallFunc::create([this](){this->imgLightGuide7->setVisible(false);}),ScaleTo::create(0.3, 0), NULL));
    this->imgTextGuide7->runAction(Sequence::create(DelayTime::create(0.45),FadeOut::create(0.1),CallFunc::create([this](){this->pnlGuide7->setVisible(false);}),CallFunc::create([this](){this->showGuide8();}), NULL));
}
void LayerGuide::showGuide6(){
    this->pnlGuide6->setVisible(true);
    
    this->imgTextGuide6 = static_cast<ImageView*>(this->pnlGuide6->getChildByName("imgTextGuide6"));
    this->btnNextGuide6 = static_cast<Button*>(this->imgTextGuide6->getChildByName("btnNextGuide6"));
    if(this->btnNextGuide6){
        this->btnNextGuide6->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnNextGuide6, this));
    }
    //auto txtGuide61 = static_cast<Text*>(this->imgTextGuide6->getChildByName("txtGuide61"));
    //txtGuide51->setString(dataManager.GetSysString(596));
    
    auto txtGuide62 = static_cast<Text*>(this->imgTextGuide6->getChildByName("txtGuide62"));
    txtGuide62->setString(dataManager.GetSysString(1037));
    
    auto txtGuide61 = static_cast<Text*>(this->imgTextGuide6->getChildByName("txtGuide61"));
    txtGuide61->setString(dataManager.GetSysString(1036));
    
    this->btnCancelGuide6 = static_cast<Button*>(this->imgTextGuide6->getChildByName("btnCancelGuide6"));
    if(this->btnCancelGuide6){
        this->btnCancelGuide6->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnCancelGuide, this));
    }
    this->imgLightGuide6 = static_cast<ImageView*>(this->pnlGuide6->getChildByName("imgLightGuide6"));
    this->imgLightGuide6->setSwallowTouches(true);
    
    this->pnlTX = (Layout*)this->imgLightGuide6->getChildByName("pnlTX");
    this->pnlPK =(Layout*)this->imgLightGuide6->getChildByName("pnlPK");
    this->pnlCT =(Layout*)this->imgLightGuide6->getChildByName("pnlCT");
    
    this->txtHuTX = (Text*)this->pnlTX->getChildByName("txtHuTX");
    this->txtHuCT= (Text*)this->pnlCT->getChildByName("txtHuCT");
    this->txtHuPK= (Text*)this->pnlPK->getChildByName("txtHuPK");
    this->txtHuTX->setString("---");
    this->txtHuCT->setString("---");
    this->txtHuPK->setString("---");
//    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGuide::tangHuTX),this,1, false);
//    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGuide::tangHuPK),this,1, false);
//    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGuide::tangHuCT),this,1, false);
    
    
    
    auto lblTX = static_cast<Text*> (this->pnlTX->getChildByName("Text_1"));
    lblTX->setString(dataManager.GetSysString(857));
    auto lblCT = static_cast<Text*> (this->pnlCT->getChildByName("Text_1_0_0"));
    lblCT->setString(dataManager.GetSysString(858));
    auto lblPK = static_cast<Text*> (this->pnlPK->getChildByName("Text_1_0_0_0"));
    lblPK->setString(dataManager.GetSysString(859));
    
    auto lstmini = mUtils::splitString(dataManager.getAppConfig().gamemini, '_');
    for (int i = 0; i < lstmini.size(); i++){
        if (strcmp(lstmini[i].c_str(), "128") == 0){
            this->pnlTX->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "130") == 0){
            this->pnlPK->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "129") == 0){
            this->pnlCT->setVisible(false);
        }
    }
    
    this->imgLightBGGuide6 = static_cast<ImageView*>(this->pnlGuide6->getChildByName("imgLightBGGuide6"));
    ItemMenuGame* menu = ItemMenuGame::create();
    menu->playAnimationTCC();
    menu->hideButtonPlayTCC();
    menu->setPosition(Vec2(40,40));
    if (SceneManager::getSingleton().isNagaNew){
        menu->setPosition(Vec2(70,60));

    }
    this->imgLightGuide6->addChild(menu);
    this->imgTextGuide6->setVisible(false);
    this->imgLightGuide6->setVisible(false);
    this->imgLightBGGuide6->setScale(0);
    this->imgLightBGGuide6->runAction(Sequence::create(DelayTime::create(0.2),ScaleTo::create(0.2, 1),CallFunc::create([this](){this->imgLightGuide6->setVisible(true);}),CallFunc::create([this](){this->imgLightBGGuide6->setVisible(false);}), NULL));
    this->imgTextGuide6->runAction(Sequence::create(DelayTime::create(0.65),FadeIn::create(0.1),CallFunc::create([this](){this->imgTextGuide6->setVisible(true);}), NULL));


}
void LayerGuide::tangHuTX(float dt){
    //    static int count = 0;
    //    static int count2 = 0;
    //    static int step = 37;
    //    this->fakeHuTX+=step;
    //    count+=37;
    //    if (count>32443){
    //        count2+=34;
    //        step= 0;
    //        if (count2>6244){
    //            step = 37;
    //            count2 = 0 ;
    //            count = 0;
    //
    //        }
    //
    //    }
    this->txtHuTX->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().huTaiXiu));
    
    //    if (this->fakeHuTX > this->huTaiXiu){
    //        this->fakeHuTX = 623471;
    //        this->txtHuTX->setString(mUtils::convertMoneyEx(this->fakeHuTX));
    //    }
    //    SceneManager::getSingleton().huTaiXiu = this->fakeHuTX;
}
void LayerGuide::tangHuPK(float dt){
    //    static int count = 0;
    //    static int count2 = 0;
    //    static int step = 37;
    //    this->fakePoker+=step;
    //    count+=37;
    //    if (count>12443){
    //        count2+=34;
    //        step= 0;
    //        if (count2>6244){
    //            step = 37;
    //            count2 = 0 ;
    //            count = 0;
    //
    //        }
    //
    //    }
    this->txtHuPK->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().huPoker));
    //    if (this->fakePoker > this->huPoker){
    //        this->fakePoker = 629812;
    //        this->txtHuPK->setString(mUtils::convertMoneyEx(this->fakePoker));
    //    }
    //     = this->fakePoker;
}
void LayerGuide::tangHuCT(float dt){
    this->txtHuCT->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().huCaoThap));
}

void LayerGuide::onBtnNextGuide6(Ref* pSender){
    this->btnCancelGuide6->setVisible(false);
    this->btnNextGuide6->setVisible(false);
    this->imgLightBGGuide6->runAction(Sequence::create(CallFunc::create([this](){this->imgLightBGGuide6->setVisible(true);}),CallFunc::create([this](){this->imgLightGuide6->setVisible(false);}),ScaleTo::create(0.3, 0), NULL));
    this->imgTextGuide6->runAction(Sequence::create(DelayTime::create(0.45),FadeOut::create(0.1),CallFunc::create([this](){this->pnlGuide6->setVisible(false);}),CallFunc::create([this](){this->showGuide7();}), NULL));
}
void LayerGuide::showGuide5(){
    this->pnlGuide5->setVisible(true);
    
    this->imgTextGuide5 = static_cast<ImageView*>(this->pnlGuide5->getChildByName("imgTextGuide5"));
    this->btnNextGuide5 = static_cast<Button*>(this->imgTextGuide5->getChildByName("btnNextGuide5"));
    if(this->btnNextGuide5){
        this->btnNextGuide5->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnNextGuide5, this));
    }
    auto txtGuide51 = static_cast<Text*>(this->imgTextGuide5->getChildByName("txtGuide51"));
    txtGuide51->setString(dataManager.GetSysString(596));
    
    auto txtGuide52 = static_cast<Text*>(this->imgTextGuide5->getChildByName("txtGuide52"));
    txtGuide52->setString(dataManager.GetSysString(1015));
    
    
    auto txtGuide53 = static_cast<Text*>(this->imgTextGuide5->getChildByName("txtGuide53"));
    txtGuide53->setString(dataManager.GetSysString(1014));
    
    
    this->btnCancelGuide5 = static_cast<Button*>(this->imgTextGuide5->getChildByName("btnCancelGuide5"));
    if(this->btnCancelGuide5){
        this->btnCancelGuide5->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnCancelGuide, this));
    }
    this->imgLightGuide5 = static_cast<ImageView*>(this->pnlGuide5->getChildByName("imgLightGuide5"));
    this->imgLightGuide5->setSwallowTouches(true);
    
    auto sprCoin = dynamic_cast<Sprite*>(this->imgLightGuide5->getChildByName("sprCoin"));

    auto sprLight = dynamic_cast<Sprite*>(this->imgLightGuide5->getChildByName("sprLight"));
//    if (sprLight){
//        Vector<SpriteFrame*> animFrames;
//
//        string str = "";
//        for (int i = 1; i <= 3; i++) {
//            str = StringUtils::format("moc3-bg-as%d.png",i);
//            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 323, 320));
//            animFrames.pushBack(spriteFrame);
//        }
//
//        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.15f);
//        Animate* animN = Animate::create(animation);
//        sprLight->runAction(RepeatForever::create(animN));
//    }
//    if (sprCoin){
//        Vector<SpriteFrame*> animFrames;
//
//        string str = "";
//        for (int i = 1; i <= 12; i++) {
//            str = StringUtils::format("moc3-%d.png",i);
//            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 323, 320));
//            animFrames.pushBack(spriteFrame);
//        }
//
//        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.15f);
//        auto animN = Animate::create(animation);
//        sprCoin->runAction(RepeatForever::create(animN));
//    }
//    auto imgDoiThuong = dynamic_cast<ImageView*>(this->imgLightGuide5->getChildByName("imgDoiThuong"));
//    imgDoiThuong->loadTexture(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"imgdoiwing.png");
    
    this->imgLightBGGuide5 = static_cast<ImageView*>(this->pnlGuide5->getChildByName("imgLightBGGuide5"));
    this->imgTextGuide5->setVisible(false);
    this->imgLightGuide5->setVisible(false);
    this->imgLightBGGuide5->setScale(0);
    this->imgLightBGGuide5->runAction(Sequence::create(DelayTime::create(0.2),ScaleTo::create(0.2, 1),CallFunc::create([this](){this->imgLightGuide5->setVisible(true);}),CallFunc::create([this](){this->imgLightBGGuide5->setVisible(false);}), NULL));
    this->imgTextGuide5->runAction(Sequence::create(DelayTime::create(0.65),FadeIn::create(0.1),CallFunc::create([this](){this->imgTextGuide5->setVisible(true);}), NULL));
}
void LayerGuide::onBtnNextGuide5(Ref* pSender){
    if (SceneManager::getSingleton().isNagaWin){
        this->btnCancelGuide5->setVisible(false);
        this->btnNextGuide5->setVisible(false);
        this->imgLightBGGuide5->runAction(Sequence::create(CallFunc::create([this](){this->imgLightBGGuide5->setVisible(true);}),CallFunc::create([this](){this->imgLightGuide5->setVisible(false);}),ScaleTo::create(0.3, 0), NULL));
        this->imgTextGuide5->runAction(Sequence::create(DelayTime::create(0.45),FadeOut::create(0.1),CallFunc::create([this](){this->pnlGuide5->setVisible(false);}),CallFunc::create([this](){this->showGuide7();}), NULL));
        return;
    }
    this->btnCancelGuide5->setVisible(false);
    this->btnNextGuide5->setVisible(false);
    this->imgLightBGGuide5->runAction(Sequence::create(CallFunc::create([this](){this->imgLightBGGuide5->setVisible(true);}),CallFunc::create([this](){this->imgLightGuide5->setVisible(false);}),ScaleTo::create(0.3, 0), NULL));
    this->imgTextGuide5->runAction(Sequence::create(DelayTime::create(0.45),FadeOut::create(0.1),CallFunc::create([this](){this->pnlGuide5->setVisible(false);}),CallFunc::create([this](){this->showGuide6();}), NULL));
}
void LayerGuide::showGuide4(){
    this->pnlGuide4->setVisible(true);
    
    this->imgTextGuide4 = static_cast<ImageView*>(this->pnlGuide4->getChildByName("imgTextGuide4"));
    this->btnNextGuide4 = static_cast<Button*>(this->imgTextGuide4->getChildByName("btnNextGuide4"));
    if(this->btnNextGuide4){
        this->btnNextGuide4->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnNextGuide4, this));
    }
    auto txtGuide41 = static_cast<Text*>(this->imgTextGuide4->getChildByName("txtGuide41"));
    txtGuide41->setString(dataManager.GetSysString(1012));
    
    auto txtGuide42 = static_cast<Text*>(this->imgTextGuide4->getChildByName("txtGuide42"));
    txtGuide42->setString(dataManager.GetSysString(1013));
    
    
    auto txtGuide43 = static_cast<Text*>(this->imgTextGuide4->getChildByName("txtGuide43"));
    txtGuide43->setString(dataManager.GetSysString(1014));
    
    
    this->btnCancelGuide4 = static_cast<Button*>(this->imgTextGuide4->getChildByName("btnCancelGuide4"));
    if(this->btnCancelGuide4){
        this->btnCancelGuide4->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnCancelGuide, this));
    }
    this->imgLightGuide4 = static_cast<ImageView*>(this->pnlGuide4->getChildByName("imgLightGuide4"));
    this->imgLightGuide4->setSwallowTouches(true);
    
    auto txtGold = dynamic_cast<Text*>(this->imgLightGuide4->getChildByName("txtGold"));
    auto txtSilver = dynamic_cast<Text*>(this->imgLightGuide4->getChildByName("txtSilver"));
    
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    if (myself == NULL ||
        myself->GetVariable("amf") == NULL ||
        myself->GetVariable("amfs") == NULL)
    {
       
        return;
    }
    
    //am = myself->GetVariable("am")->GetDoubleValue();
    auto amf = myself->GetVariable("amf")->GetDoubleValue();
    auto amfs = myself->GetVariable("amfs")->GetDoubleValue();

    txtSilver->setString(mUtils::convertMoneyEx(*amfs).c_str());
    txtGold->setString(mUtils::convertMoneyEx(*amf).c_str());
    
    this->imgLightBGGuide4 = static_cast<ImageView*>(this->pnlGuide4->getChildByName("imgLightBGGuide4"));
    this->imgTextGuide4->setVisible(false);
    this->imgLightGuide4->setVisible(false);
    this->imgLightBGGuide4->setScale(0);
    this->imgLightBGGuide4->runAction(Sequence::create(DelayTime::create(0.2),ScaleTo::create(0.2, 1),CallFunc::create([this](){this->imgLightGuide4->setVisible(true);}),CallFunc::create([this](){this->imgLightBGGuide4->setVisible(false);}), NULL));
    this->imgTextGuide4->runAction(Sequence::create(DelayTime::create(0.65),FadeIn::create(0.1),CallFunc::create([this](){this->imgTextGuide4->setVisible(true);}), NULL));
}
void LayerGuide::showGuide3(){
    this->pnlGuide3->setVisible(true);
    
    this->imgTextGuide3 = static_cast<ImageView*>(this->pnlGuide3->getChildByName("imgTextGuide3"));
    this->btnNextGuide3 = static_cast<Button*>(this->imgTextGuide3->getChildByName("btnNextGuide3"));
    if(this->btnNextGuide3){
        this->btnNextGuide3->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnNextGuide3, this));
    }
    auto txtGuide31 = static_cast<Text*>(this->imgTextGuide3->getChildByName("txtGuide31"));
    txtGuide31->setString(dataManager.GetSysString(1009));
    
    auto txtGuide32 = static_cast<Text*>(this->imgTextGuide3->getChildByName("txtGuide32"));
    txtGuide32->setString(dataManager.GetSysString(1010));
    
    
    auto txtGuide33 = static_cast<Text*>(this->imgTextGuide3->getChildByName("txtGuide33"));
    txtGuide33->setString(dataManager.GetSysString(1011));
    
    
    this->btnCancelGuide3 = static_cast<Button*>(this->imgTextGuide3->getChildByName("btnCancelGuide3"));
    if(this->btnCancelGuide3){
        this->btnCancelGuide3->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnCancelGuide, this));
    }
    this->imgLightGuide3 = static_cast<ImageView*>(this->pnlGuide3->getChildByName("imgLightGuide3"));
    this->imgLightGuide3->setSwallowTouches(true);
    
    auto Image_32 = static_cast<ImageView*>(this->imgLightGuide3->getChildByName("Image_32"));
    UserDefault *def = UserDefault::getInstance();
    int avatar = def->getIntegerForKey("avatar");
    string src= StringUtils::format("avatars/avatar/avatar_%d.png",avatar);
    Image_32->loadTexture(src);
    
    this->imgLightBGGuide3 = static_cast<ImageView*>(this->pnlGuide3->getChildByName("imgLightBGGuide3"));
    this->imgTextGuide3->setVisible(false);
    this->imgLightGuide3->setVisible(false);
    this->imgLightBGGuide3->setScale(0);
    this->imgLightBGGuide3->runAction(Sequence::create(DelayTime::create(0.2),ScaleTo::create(0.2, 1),CallFunc::create([this](){this->imgLightGuide3->setVisible(true);}),CallFunc::create([this](){this->imgLightBGGuide3->setVisible(false);}), NULL));
    this->imgTextGuide3->runAction(Sequence::create(DelayTime::create(0.65),FadeIn::create(0.1),CallFunc::create([this](){this->imgTextGuide3->setVisible(true);}), NULL));
}
void LayerGuide::onBtnNextGuide3(Ref* pSender){
    this->btnCancelGuide3->setVisible(false);
    this->btnNextGuide3->setVisible(false);
    this->imgLightBGGuide3->runAction(Sequence::create(CallFunc::create([this](){this->imgLightBGGuide3->setVisible(true);}),CallFunc::create([this](){this->imgLightGuide3->setVisible(false);}),ScaleTo::create(0.3, 0), NULL));
    this->imgTextGuide3->runAction(Sequence::create(DelayTime::create(0.45),FadeOut::create(0.1),CallFunc::create([this](){this->pnlGuide3->setVisible(false);}),CallFunc::create([this](){this->showGuide4();}), NULL));
}
void LayerGuide::onBtnNextGuide4(Ref* pSender){
    this->btnCancelGuide4->setVisible(false);
    this->btnNextGuide4->setVisible(false);
    this->imgLightBGGuide4->runAction(Sequence::create(CallFunc::create([this](){this->imgLightBGGuide4->setVisible(true);}),CallFunc::create([this](){this->imgLightGuide4->setVisible(false);}),ScaleTo::create(0.3, 0), NULL));
    this->imgTextGuide4->runAction(Sequence::create(DelayTime::create(0.45),FadeOut::create(0.1),CallFunc::create([this](){this->pnlGuide4->setVisible(false);}),CallFunc::create([this](){this->showGuide5();}), NULL));
}
void LayerGuide::showGuide2(){
    this->pnlGuide2->setVisible(true);
    this->imgTextGuide2 = static_cast<ImageView*>(this->pnlGuide2->getChildByName("imgTextGuide2"));
    this->btnNextGuide2 = static_cast<Button*>(this->imgTextGuide2->getChildByName("btnNextGuide2"));
    this->imgLightGuide2 = static_cast<ImageView*>(this->pnlGuide2->getChildByName("imgLightGuide2"));
    this->imgLightGuide2->setSwallowTouches(true);
    if(this->btnNextGuide2){
        this->btnNextGuide2->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnNextGuide2, this));
    }
    auto txtGuide21 = static_cast<Text*>(this->imgTextGuide2->getChildByName("txtGuide21"));
    txtGuide21->setString(dataManager.GetSysString(1006));
    
    auto txtGuide22 = static_cast<Text*>(this->imgTextGuide2->getChildByName("txtGuide22"));
    txtGuide22->setString(dataManager.GetSysString(1007));
    
    
    auto txtGuide23 = static_cast<Text*>(this->imgTextGuide2->getChildByName("txtGuide23"));
    txtGuide23->setString(dataManager.GetSysString(1008));
    
    auto btnPoker = dynamic_cast<Button*>(this->imgLightGuide2->getChildByName("btnPoker"));

    auto btnTaiXiu = dynamic_cast<Button*>(this->imgLightGuide2->getChildByName("btnTaiXiu"));
    
    auto btnSapNgua = dynamic_cast<Button*>(this->imgLightGuide2->getChildByName("btnSapNgua"));

    
    auto btnCaoThap = dynamic_cast<Button*>(this->imgLightGuide2->getChildByName("btnCaoThap"));
    
    auto btnVongQuay = dynamic_cast<Button*>(this->imgLightGuide2->getChildByName("btnVongQuay"));
    
    auto txtTX= dynamic_cast<Text*>(btnTaiXiu->getChildByName("txtTX"));
    auto txtPK= dynamic_cast<Text*>(btnPoker->getChildByName("txtPK"));
    auto txtDD= dynamic_cast<Text*>(btnSapNgua->getChildByName("txtDD"));
    auto txtVQ= dynamic_cast<Text*>(btnVongQuay->getChildByName("txtVQ"));
   // auto txtCT= dynamic_cast<Text*>(btnCaoThap->getChildByName("txtCT"));
    
    txtTX->setString(dataManager.GetSysString(969));
    txtPK->setString(dataManager.GetSysString(859));
    txtDD->setString(dataManager.GetSysString(970));
    txtVQ->setString(dataManager.GetSysString(971));
    //txtCT->setString(dataManager.GetSysString(858));
    
    
    
    auto pokerPos = btnPoker->getPosition();
    auto xocdiaPos = btnSapNgua->getPosition();
    btnPoker->setPosition(xocdiaPos);
    btnSapNgua->setPosition(pokerPos);
    
    
    auto lstmini = mUtils::splitString(dataManager.getAppConfig().gamemini, '_');
    for (int i = 0; i < lstmini.size(); i++){
        if (strcmp(lstmini[i].c_str(), "128") == 0){
            btnTaiXiu->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "130") == 0){
            btnPoker->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "129") == 0){
            if (btnCaoThap)
                btnCaoThap->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "132") == 0){
            btnVongQuay->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "134") == 0){
            btnSapNgua->setVisible(false);
        }
    }
    
    this->btnCancelGuide2 = static_cast<Button*>(this->imgTextGuide2->getChildByName("btnCancelGuide2"));
    if(this->btnCancelGuide2){
        this->btnCancelGuide2->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnCancelGuide, this));
    }
    
    this->imgLightBGGuide2 = static_cast<ImageView*>(this->pnlGuide2->getChildByName("imgLightBGGuide2"));
    this->imgTextGuide2->setVisible(false);
    this->imgLightGuide2->setVisible(false);
    this->imgLightBGGuide2->setScale(0);
    this->imgLightBGGuide2->runAction(Sequence::create(DelayTime::create(0.2),ScaleTo::create(0.2, 1),CallFunc::create([this](){this->imgLightGuide2->setVisible(true);}),CallFunc::create([this](){this->imgLightBGGuide2->setVisible(false);}), NULL));
    this->imgTextGuide2->runAction(Sequence::create(DelayTime::create(0.65),FadeIn::create(0.1),CallFunc::create([this](){this->imgTextGuide2->setVisible(true);}), NULL));
    
}
void LayerGuide::onBtnNextGuide2(Ref* pSender){
    this->btnCancelGuide2->setVisible(false);
    this->btnNextGuide2->setVisible(false);
    this->imgLightBGGuide2->runAction(Sequence::create(CallFunc::create([this](){this->imgLightBGGuide2->setVisible(true);}),CallFunc::create([this](){this->imgLightGuide2->setVisible(false);}),ScaleTo::create(0.3, 0), NULL));
    this->imgTextGuide2->runAction(Sequence::create(DelayTime::create(0.45),FadeOut::create(0.1),CallFunc::create([this](){this->pnlGuide2->setVisible(false);}),CallFunc::create([this](){this->showGuide3();}), NULL));
}
void LayerGuide::createListData()
{
    
    //string _listRoom = "100_true;113_true;111_true;112_true;102_true;109_true;116_false;103_false;114_false;115_false;117_false";
    this->lstIcons.clear();
    string _listRoom = dataManager.getAppConfig().list_game;
    //     if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
    //         _listRoom = "112_true;113_true;111_true;102_true;100_true;116_true;114_true;117_true;109_true;106_true;103_true;115_false";
    vector<string> lstconfig = mUtils::splitString(_listRoom, ';');
    for (int i = 0; i < lstconfig.size(); i++)
    {
        vector<string> configs = mUtils::splitString(lstconfig.at(i), '_');
        if (configs.size() > 1)
        {
            string _gameid = configs.at(0);
            string _str1 = configs.at(1);
            menuStruct it;
            it.gameID = atoi(_gameid.c_str());
            it.state = 0 == _str1.compare("true") ? true : false;
            this->lstIcons.push_back(it);
        }
    }
    
    for (int i = 0; i < lstconfig.size(); i++)
    {
        vector<string> configs = mUtils::splitString(lstconfig.at(i), '_');
        if (configs.size() > 1)
        {
            string _gameid = configs.at(0);
            string _str1 = configs.at(1);
            int _id = atoi(_gameid.c_str());
            bool _state = 0 == _str1.compare("true") ? true : false;
            string _imgDis = StringUtils::format("gameicons/%s%s_dis.png",SceneManager::getSingleton().getFolderLang().c_str(),_gameid.c_str());
            string _imgNormal = StringUtils::format("gameicons/%s%s.png",SceneManager::getSingleton().getFolderLang().c_str(),_gameid.c_str());
            this->lstIconsNew.push_back(iconStruct(_id, _state, _imgNormal, _imgDis));
        }
    }
}
void LayerGuide::showGuide1(){
    this->pnlGuide1->setVisible(true);

    this->imgTextGuide1 = static_cast<ImageView*>(this->pnlGuide1->getChildByName("imgTextGuide1"));
    this->btnNextGuide1 = static_cast<Button*>(this->imgTextGuide1->getChildByName("btnNextGuide1"));
    if(this->btnNextGuide1){
        this->btnNextGuide1->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnNextGuide1, this));
    }
    auto txtGuide11 = static_cast<Text*>(this->imgTextGuide1->getChildByName("txtGuide11"));
    txtGuide11->setString(dataManager.GetSysString(1003));
    
    auto txtGuide12 = static_cast<Text*>(this->imgTextGuide1->getChildByName("txtGuide12"));
    txtGuide12->setString(dataManager.GetSysString(1004));

    
    auto txtGuide13 = static_cast<Text*>(this->imgTextGuide1->getChildByName("txtGuide13"));
    txtGuide13->setString(dataManager.GetSysString(1005));
    
    
    this->btnCancelGuide1 = static_cast<Button*>(this->imgTextGuide1->getChildByName("btnCancelGuide1"));
    if(this->btnCancelGuide1){
        this->btnCancelGuide1->addClickEventListener(CC_CALLBACK_1(LayerGuide::onBtnCancelGuide, this));
    }
    this->imgLightGuide1 = static_cast<ImageView*>(this->pnlGuide1->getChildByName("imgLightGuide1"));
    this->imgLightGuide1->setSwallowTouches(true);
    this->pvMain = (PageView*)this->imgLightGuide1->getChildByName("pnlGame");
    this->pvMain->setSwallowTouches(true);
    this->createListData();
    this->createPageView();
    this->imgTextGuide1->setVisible(false);
    this->pvMain->setVisible(false);
    this->imgLightGuide1->setScale(0);
    this->imgLightGuide1->runAction(Sequence::create(ScaleTo::create(0.2, 1),CallFunc::create([this](){this->pvMain->setVisible(true);}), NULL));
    this->imgTextGuide1->runAction(Sequence::create(DelayTime::create(0.4),FadeIn::create(0.1),CallFunc::create([this](){this->imgTextGuide1->setVisible(true);}), NULL));

}
void LayerGuide::createPageView()
{
    
    if (SceneManager::getSingleton().isNagaWin){
        if (!pvMain)
            return;
        pvMain->setClippingEnabled(false);
        pvMain->setUsingCustomScrollThreshold(true);
        pvMain->setCustomScrollThreshold(pvMain->getContentSize().width / 4);
        pvMain->setClippingEnabled(true);
        //pvMain->removeAllItems();
        pvMain->removeAllPages();
        pvMain->setSwallowTouches(false);
        int jumpunit = 4;
        int pageCount = 2;
        if (lstIcons.size()<=8)
            pageCount=1;
        int distanceX = 65;
        int distanceY = 43;
        Size sizeIcon = Size(247, 281);
        Size sizeIcon2 = Size(247, 240);
        
        try{
            
            for (int i = 0; i < pageCount; i++)
            {
                Layout* layout = Layout::create();
                layout->setContentSize(pvMain->getContentSize());
                if (lstIcons.size() >= 6)
                {
                    if (i == 0)
                    {
                        for (int j = 0; j < jumpunit * 2; j++)
                        {
                            float posX;
                            float posY;
                            if (j < jumpunit)
                            {
                                posX = j * sizeIcon.width + distanceX * (j - 1) + 100;
                                posY = sizeIcon.height + distanceY ;
                            }
                            else{
                                posX = (j - jumpunit) * sizeIcon.width + (j - jumpunit - 1) * distanceX + 100;;
                                posY = 0;
                            }
                            ItemMenuGame* item = ItemMenuGame::create();
                            item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                            item->setPosition(Vec2(posX,posY));
                            layout->addChild(item);
                            item->setTag(this->lstIcons[j].gameID);
                            this->lstItemGame.push_back(item);
                        }
                    }
                    else{
                        for (int j = jumpunit * pageCount; j < this->lstIcons.size(); j++) {
                            int posX;
                            int posY;
                            int j_tg = j - jumpunit * 2;
                            if (j_tg < jumpunit) {
                                posX = j_tg * sizeIcon.width + distanceX * (j_tg - 1) + 100;
                                posY = sizeIcon.height + distanceX +0;
                            } else {
                                posX = (j_tg - jumpunit) * sizeIcon.width + (j_tg - jumpunit - 1) * distanceX + 100;
                                posY = 0;
                            }
                            
                            ItemMenuGame* item = ItemMenuGame::create();
                            
                            //item->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                            item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                            item->setPosition(Vec2(posX,posY));
                            layout->addChild(item);
                            this->lstItemGame.push_back(item);
                            
                           
                        }
                    }
                }else{
                    if (i == 0)
                    {
                        for (int j = 0; j < this->lstIcons.size(); j++)
                        {
                            float posX;
                            float posY;
                            if (j < jumpunit)
                            {
                                posX = j * sizeIcon.width + distanceX * (j - 1) + 100;
                                posY = sizeIcon.height + distanceY +0;
                            }
                            else{
                                posX = (j - jumpunit) * sizeIcon.width + (j - jumpunit - 1) * distanceX + 100;;
                                posY = 0;
                            }
                            ItemMenuGame* item = ItemMenuGame::create();
                            item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                            item->setPosition(Vec2(posX,posY));
                            layout->addChild(item);
                            this->lstItemGame.push_back(item);
                            
                          
                        }
                    }
                    else{
                        for (int j = jumpunit ; j < lstIcons.size(); j++)
                        {
                            float posX;
                            float posY;
                            if (j == jumpunit + 8)
                            {
                                posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX+100;
                                posY = 0;
                            }
                            else if (j > jumpunit + 8)
                            {
                                posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX+100;
                                posY = 0;
                            }
                            else {
                                posX = (j - jumpunit ) * sizeIcon.width + distanceX+100;
                                posY = 0;
                            }
                            ItemMenuGame* item = ItemMenuGame::create();
                            item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                            item->setPosition(Vec2(posX,posY));
                            layout->addChild(item);
                            this->lstItemGame.push_back(item);
                            
                           
                            
                            
                        }
                    }
                }
                pvMain->insertPage(layout, i);
            }
            pvMain->scrollToPage(0);
        }
        catch (...)
        {
            // log("Unknow exceptions");
        }
        return;
    }
    
    if (SceneManager::getSingleton().isNagaNew){
        
        //this->createScrollView(); return;
        if (!pvMain)
            return;
        pvMain->setClippingEnabled(false);
        pvMain->setUsingCustomScrollThreshold(true);
        pvMain->setCustomScrollThreshold(pvMain->getContentSize().width / 4);
        pvMain->setClippingEnabled(true);
        //pvMain->removeAllItems();
        pvMain->removeAllPages();
        pvMain->setSwallowTouches(false);
        int jumpunit = 4;
        int pageCount = 2;
        if (lstIcons.size()<=8)
            pageCount=1;
        int distanceX = 55;
        int distanceY = 20;
        Size sizeIcon = Size(265.00, 315);
        try{
            
            for (int i = 0; i < pageCount; i++)
            {
                Layout* layout = Layout::create();
                layout->setContentSize(pvMain->getContentSize());
                if (lstIcons.size() >= 6)
                {
                    if (i == 0)
                    {
                        for (int j = 0; j < jumpunit * 2; j++)
                        {
                            float posX;
                            float posY;
                            if (j < jumpunit)
                            {
                                posX = j * sizeIcon.width + distanceX * (j - 1) + 20;
                                posY = sizeIcon.height + distanceY-20 ;
                            }
                            else{
                                posX = (j - jumpunit) * sizeIcon.width + (j - jumpunit - 1) * distanceX + 20;;
                                posY = -20;
                            }
                            ItemMenuGame* item = ItemMenuGame::create();
                            item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                            item->setPosition(Vec2(posX,posY));
                            layout->addChild(item);
                            item->setTag(this->lstIcons[j].gameID);
                            this->lstItemGame.push_back(item);
                            
     
                            
                            
                        }
                    }
                    else{
                        for (int j = jumpunit * pageCount; j < this->lstIcons.size(); j++) {
                            int posX;
                            int posY;
                            int j_tg = j - jumpunit * 2;
                            if (j_tg < jumpunit) {
                                posX = j_tg * sizeIcon.width + distanceX * (j_tg - 1) + 20;
                                posY = sizeIcon.height + distanceX -20;
                            } else {
                                posX = (j_tg - jumpunit) * sizeIcon.width + (j_tg - jumpunit - 1) * distanceX + 20;
                                posY = -20;
                            }
                            
                            ItemMenuGame* item = ItemMenuGame::create();
                            
                            //item->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                            item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                            item->setPosition(Vec2(posX,posY));
                            layout->addChild(item);
                            this->lstItemGame.push_back(item);
                            
 
                            
                        }
                    }
                }else{
                    if (i == 0)
                    {
                        for (int j = 0; j < this->lstIcons.size(); j++)
                        {
                            float posX;
                            float posY;
                            if (j < jumpunit)
                            {
                                posX = j * sizeIcon.width + distanceX * (j - 1) + 20;
                                posY = sizeIcon.height + distanceY -20;
                            }
                            else{
                                posX = (j - jumpunit) * sizeIcon.width + (j - jumpunit - 1) * distanceX + 20;;
                                posY = -20;
                            }
                            ItemMenuGame* item = ItemMenuGame::create();
                            item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                            item->setPosition(Vec2(posX,posY));
                            layout->addChild(item);
                            this->lstItemGame.push_back(item);
                            
                         
                            
                        }
                    }
                    else{
                        for (int j = jumpunit ; j < lstIcons.size(); j++)
                        {
                            float posX;
                            float posY;
                            if (j == jumpunit + 8)
                            {
                                posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX+20;
                                posY = -20;
                            }
                            else if (j > jumpunit + 8)
                            {
                                posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX+20;
                                posY = -20;
                            }
                            else {
                                posX = (j - jumpunit ) * sizeIcon.width + distanceX+20;
                                posY = -20;
                            }
                            ItemMenuGame* item = ItemMenuGame::create();
                            item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                            item->setPosition(Vec2(posX,posY));
                            layout->addChild(item);
                            this->lstItemGame.push_back(item);
                            
                           
                        }
                    }
                }
                pvMain->insertPage(layout, i);
            }
            pvMain->scrollToPage(0);
        }
        catch (...)
        {
            // log("Unknow exceptions");
        }
        
        
        return;
    }
    
    if (!pvMain)
        return;
    pvMain->setClippingEnabled(false);
    pvMain->setUsingCustomScrollThreshold(true);
    pvMain->setCustomScrollThreshold(pvMain->getContentSize().width / 4);
    pvMain->setClippingEnabled(true);
    //pvMain->removeAllItems();
    pvMain->removeAllPages();
    pvMain->setSwallowTouches(false);
    int jumpunit = 4;
    //
    int pageCount = 1;
    //
    if (lstIcons.size()>4 && lstIcons.size()<=8)
        pageCount = 2;
    if (lstIcons.size()>8 && lstIcons.size()<=12)
        pageCount = 3;
    if (lstIcons.size()>12 && lstIcons.size()<=16)
        pageCount = 4;
    
    int distanceX = 45;
    int distanceY = 5;
    Size sizeIcon = Size(333, 314);
    try{
        
        for (int i = 0; i < pageCount; i++)
        {
            Layout* layout = Layout::create();
            layout->setSwallowTouches(false);
            layout->setContentSize(pvMain->getContentSize());
            if (lstIcons.size() >= 4)
            {
                if (i == 0)
                {
                    for (int j = 0; j < jumpunit; j++)
                    {
                        float posX;
                        float posY;
                        if (j < jumpunit)
                        {
                            posX = j * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        else{
                            posX = (j - jumpunit) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                    }
                }
                else{
                    for (int j = jumpunit*i; j < lstIcons.size(); j++)
                    {
                        float posX;
                        float posY;
                        if (j == jumpunit + 8)
                        {
                            posX = (j - (jumpunit  + 4)) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        else if (j > jumpunit + 8)
                        {
                            posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        else {
                            posX = (j - jumpunit) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                    }
                }
            }else{
                if (i == 0)
                {
                    for (int j = 0; j < lstIcons.size(); j++)
                    {
                        float posX;
                        float posY;
                        if (j < jumpunit)
                        {
                            posX = j * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        else{
                            posX = (j - jumpunit) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                    }
                }
                else{
                    for (int j = jumpunit ; j < lstIcons.size(); j++)
                    {
                        float posX;
                        float posY;
                        if (j == jumpunit + 8)
                        {
                            posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        else if (j > jumpunit + 8)
                        {
                            posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        else {
                            posX = (j - jumpunit ) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                        
                    }
                }
            }
            pvMain->insertPage(layout, i);
        }
        pvMain->scrollToPage(0);
    }
    catch (...)
    {
       // log("Unknow exceptions");
    }
}
void LayerGuide::onBtnNextGuide1(Ref* pSender){
    this->btnCancelGuide1->setVisible(false);
    this->btnNextGuide1->setVisible(false);
    this->imgLightGuide1->runAction(Sequence::create(CallFunc::create([this](){this->imgLightGuide1->setVisible(true);}),CallFunc::create([this](){this->pvMain->setVisible(false);}),ScaleTo::create(0.3, 0), NULL));
    this->imgTextGuide1->runAction(Sequence::create(DelayTime::create(0.45),FadeOut::create(0.2),CallFunc::create([this](){this->pnlGuide1->setVisible(false);}),CallFunc::create([this](){this->showGuide2();}), NULL));
}

void LayerGuide::onBtnYes(Ref* pSender){
   this->pnlOption->setVisible(false);
    this->showGuide1();
}
void LayerGuide::onBtnNo(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerGuide::onEnter()
{
    Layer::onEnter();
    SceneManager::getSingleton().numPopup+=1;


}

void LayerGuide::onExit()
{
    Layer::onExit();
    SceneMain* parent = (SceneMain*)this->getParent();
    if (parent){
        parent->showEvent();
    }
    SceneManager::getSingleton().numPopup-=1;
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGuide::tangHuTX),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGuide::tangHuPK),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGuide::tangHuCT),this);
}

