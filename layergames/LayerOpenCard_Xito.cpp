#include "LayerOpenCard_Xito.h"
#include "Requests/ExtensionRequest.h"
#include "mUtils.h"
#include "XiToClientMessageDefs.h"
#include "AllData.h"

LayerOpenCard_Xito::LayerOpenCard_Xito() : listDeal(""), p_Timer(15) {}

LayerOpenCard_Xito::LayerOpenCard_Xito(const string& lst, const float& timer) : listDeal(lst), p_Timer(timer){

}

LayerOpenCard_Xito* LayerOpenCard_Xito::create(const string& lst, const float& timer){
    LayerOpenCard_Xito* l = new LayerOpenCard_Xito(lst, timer);
    if(l && l->init()){
        l->autorelease ();
        return l;
    }else{
        delete l;
        l = NULL;
        return NULL;
    }
}

bool LayerOpenCard_Xito::init(){
    if(!Layer::init())
        return false;

    Sprite* bg = Sprite::create("xito-chontay.png");
    bg->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
    this->addChild(bg);

    //
    vector<string> arrCards = mUtils::splitString(this->listDeal, ',');
    //Card 1
    int iTag = atoi(arrCards.at(0).c_str());
    string Url = XiToHelper::FindTypeCard(iTag);

    Button* btnCard1 = Button::create();
    btnCard1->addTouchEventListener(CC_CALLBACK_2(LayerOpenCard_Xito::onChooseCard, this));
    btnCard1->setTouchEnabled(true);
    btnCard1->loadTextures(Url, Url, "", Widget::TextureResType::PLIST);
    btnCard1->setPosition(Vec2(WIDTH_DESIGN / 2 - btnCard1->getContentSize().width, (HEIGHT_DESIGN >> 1)));
    btnCard1->setTag(iTag);
    btnCard1->setScale(1.7);
    btnCard1->setPressedActionEnabled(true);
    this->addChild(btnCard1);

    //Card 2
    iTag = atoi(arrCards.at(1).c_str());
    Url = XiToHelper::FindTypeCard(iTag);

    Button* btnCard2 = Button::create();
    btnCard2->addTouchEventListener(CC_CALLBACK_2(LayerOpenCard_Xito::onChooseCard, this));
    btnCard2->setTouchEnabled(true);
    btnCard2->loadTextures(Url, Url, "", Widget::TextureResType::PLIST);
    btnCard2->setPosition(Vec2(WIDTH_DESIGN / 2 + btnCard2->getContentSize().width, (HEIGHT_DESIGN >> 1)));
    btnCard2->setTag(iTag);
    btnCard2->setScale(1.7);
    btnCard2->setPressedActionEnabled(true);
    this->addChild(btnCard2);
    arrCards.clear();

    Sprite* timebg = Sprite::create("xito-chontay-loadbg.png");
    timebg->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN / 2 - 150));
    this->addChild(timebg);

    ProgressTo* to1 = ProgressTo::create(this->p_Timer, 100);
    ProgressTimer* timer = ProgressTimer::create(Sprite::create("xito-chontay-loading.png"));
    timer->setType(ProgressTimer::Type::BAR);
    timer->setMidpoint(Vec2(0, 0));
    timer->setBarChangeRate(Vec2(1, 0));
    this->addChild(timer);
    timer->setPosition (Vec2(WIDTH_DESIGN >> 1, (HEIGHT_DESIGN >> 1) - 150));
    timer->runAction(to1);

    return true;
}

void LayerOpenCard_Xito::onChooseCard(Ref *sender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        Node* node = (Node*)sender;
        if(node) {
            int idx = node->getTag();
            this->SendOpenCards(idx);
        }
    }
}

LayerOpenCard_Xito::~LayerOpenCard_Xito() {

}

void LayerOpenCard_Xito::SendOpenCards(const int& cardIdx) {
    boost::shared_ptr<ISFSObject> param (new SFSObject());
    param->PutInt(EXT_FIELD_CARD_ID, cardIdx);
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    boost::shared_ptr<IRequest> request (new ExtensionRequest(EXT_EVENT_SELECT_OPEN_CARD_REQ, param, lastRoom));
    GameServer::getSingleton().Send(request);
    this->removeFromParentAndCleanup(true);
}

bool LayerOpenCard_Xito::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    return true;
}

void LayerOpenCard_Xito::onEnter() {
    Layer::onEnter();
    this->runAction(mUtils::getActionOpenPopup());
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerOpenCard_Xito::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void LayerOpenCard_Xito::onExit() {
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    Layer::onExit();
}
