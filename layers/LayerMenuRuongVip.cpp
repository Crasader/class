//
//  LayerMenuRuongVip.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/10/16.
//
//

#include "LayerMenuRuongVip.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "LayerPopupRuongVip.h"
#include "LayerMainTop.h"
#include "../layergames/PhomMessDef.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerMenuRuongVip::LayerMenuRuongVip()
{
    GameServer::getSingleton().addListeners(this);
}


LayerMenuRuongVip::~LayerMenuRuongVip()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerMenuRuongVip::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerMenuRuongVip.csb");
//    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
//    pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
//    rootNode->setAnchorPoint(Point(0.5, 0.5));
//    rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    this->setContentSize(rootNode->getContentSize());
    ImageView* imgCircle = static_cast<ImageView*>(rootNode->getChildByName("Image_1"));
    auto ActionRotate = RepeatForever::create(RotateBy::create(0.2,10));
    imgCircle->runAction(ActionRotate);
    this->addChild(rootNode);
    
    this->txtValue = static_cast<Text*>(rootNode->getChildByName("lblValue"));
    this->txtNum = static_cast<Text*>(rootNode->getChildByName("lblNumKey"));
    auto btnRuongVip = static_cast<Button*>(rootNode->getChildByName("btnRuong"));
    if (btnRuongVip != NULL){
        btnRuongVip->setPressedActionEnabled(true);
        btnRuongVip->addClickEventListener(CC_CALLBACK_1(LayerMenuRuongVip::gotoRuongVip, this));
    }
    
    setValueChest("0", "0");
    
    getInfoChest(0);
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerMenuRuongVip::getInfoChest),this,60, false);
    
    //setValueChest("21238712387","300");
    
    return true;
}
void LayerMenuRuongVip::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp("rsldc", cmd->c_str()) == 0){
       // log("ra");
        if (param->GetUtfString("detailcoffer") == NULL) return;
        string dc = *param->GetUtfString("detailcoffer");
        auto lstdc = mUtils::splitString(dc, '|');
        if (lstdc.size()<=0) return;
        setValueChest(lstdc[0], lstdc[1]);
    }

}
bool LayerMenuRuongVip::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerMenuRuongVip::gotoRuongVip(Ref* pSender){
    LayerMainTop* top = (LayerMainTop*)(this->getParent());
    if (top) {
        top->gotoRuongVip();
    }
}
void LayerMenuRuongVip::setValueChest(string value,string num){
    this->txtValue->setString(formatMoneySymbolChest(atol(value.c_str())));
    this->txtNum->setString(mUtils::convertMoneyEx(atoll(num.c_str())));
}
void LayerMenuRuongVip::getInfoChest(float dt){
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("rdc", params));
    GameServer::getSingleton().Send(request);
}
void LayerMenuRuongVip::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerSendMessage::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerMenuRuongVip::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
