//
//  LayerBangXepHang.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerBangXepHang.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../layergames/_Chat_.h"
#include "../data_manager.h"
#include "Requests/ExtensionRequest.h"
#include "LayerPopupThongBao.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
LayerBangXepHang::LayerBangXepHang()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerBangXepHang::~LayerBangXepHang()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerBangXepHang::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerBangXepHang.csb");
    
    
    rootNode->setPosition(Vec2(0,0));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
   
    
    auto pnlTable = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    pnlTable->setVisible(false);

    

    this->scrollXepHang = ui::ScrollView::create();
    this->scrollXepHang->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollXepHang->setClippingEnabled(true);
    this->scrollXepHang->setContentSize(pnlTable->getContentSize());
    this->scrollXepHang->setAnchorPoint(pnlTable->getAnchorPoint());
    this->scrollXepHang->setPosition(pnlTable->getPosition());
    this->scrollXepHang->setScrollBarEnabled(true);
    this->scrollXepHang->setBounceEnabled(true);
    this->scrollXepHang->setTag(129323);
    this->scrollXepHang->setInnerContainerSize(Size(430, pnlTable->getContentSize().height));
    this->addChild(this->scrollXepHang);
    
    //this->lstop = "mrtuan1;124124|mrtuan2;124124124|mrtuan1;124124|mrtuan2;124124124|mrtuan1;124124|mrtuan2;124124124|mrtuan1;124124|mrtuan2;124124124|";//

    this->loadTyGia();
    return true;
}

void LayerBangXepHang::loadTyGia(){
    auto lst = mUtils::splitString(this->lstop, '|');
    this->lstXH.clear();
    this->scrollXepHang->setInnerContainerSize(Size(430,(lst.size())*100));
    this->scrollXepHang->removeAllChildren();
    this->scrollXepHang->scrollToTop(1, false);
    for (int i =0;i<lst.size();i++){
        
        auto item =  mUtils::splitString(lst[i], ';');
        if (item.size()<2) continue;
        ItemBangXepHang* it = ItemBangXepHang::create();
        it->setAnchorPoint(Vec2(0,1));
        it->setDatas(i,item[0],mUtils::convertMoneyEx(atof(item[1].c_str())));
        this->scrollXepHang->addChild(it);
        it->setPosition(Vec2(0,(lst.size())*100 - (i+1) * 90));
        if (SceneManager::getSingleton().isNagaNew){
            it->setPosition(Vec2(0,(lst.size())*100 - (i+1) * 100));
        }
    }
}
void LayerBangXepHang::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene) return;
   // log("OnExtensionResponse %s", cmd->c_str());
    if (strcmp("eetopresp", cmd->c_str()) == 0){//OK
        boost::shared_ptr<string> top = param->GetUtfString("top");
        
        if (top)
        {
            this->lstop = *top;
            this->loadTyGia();
        }
    }
}

void LayerBangXepHang::loadTop(int gid, int typeroom){
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt("gid", gid);
    params->PutInt("roomType", typeroom);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("eetoprq", params));
    GameServer::getSingleton().Send(request);
}


void LayerBangXepHang::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerBangXepHang::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerBangXepHang::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool ItemBangXepHang::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemXepHang.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    this->name = static_cast<Text*>(root->getChildByName("name"));
    this->money = static_cast<Text*>(root->getChildByName("money"));
    this->imgTop = static_cast<ImageView*>(root->getChildByName("imgRank"));
    this->txtRank = static_cast<Text*>(root->getChildByName("txtRank"));

    return true;
}

void ItemBangXepHang::setDatas(int idx,string name,string money){
    if (idx<3){
        this->txtRank->setVisible(false);
        this->imgTop->setVisible(true);
        this->imgTop->loadTexture(StringUtils::format("rank%d.png",idx+1));
    }else{
        this->txtRank->setVisible(true);
        this->imgTop->setVisible(false);
        this->txtRank->setString(StringUtils::format("%d",idx+1));
    }
    std::string _strNameSmall = "";
    (name.length() > 14) ? _strNameSmall = name.substr(0, 14) + "..." : _strNameSmall =name;
    this->name->setString(_strNameSmall);
    this->money->setString(money);
}




LayerBangXepHangTo::LayerBangXepHangTo()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerBangXepHangTo::~LayerBangXepHangTo()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerBangXepHangTo::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupXepHang.csb");
    
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
    //   if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
    sizeAdd = ChanUtils::getSizePos();
    
    //    }
    //    else{
    //        sizeAdd = Size(0, 0);
    //    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    
    
    auto pnlTable = dynamic_cast<Layout*>(pnlBg->getChildByName("pnlContent"));
    pnlTable->setVisible(false);
    
    ImageView* imgheader = dynamic_cast<ImageView*>(pnlBg->getChildByName("imgheader"));
    imgheader->loadTexture(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"bxhrank.png");
    
    this->txtNhan = dynamic_cast<Text*>(pnlBg->getChildByName("txtNhan"));
    this->txtNhan->setString(dataManager.GetSysString(850));
    auto btn_close = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if (btn_close != NULL){
        btn_close->addClickEventListener(CC_CALLBACK_1(LayerBangXepHangTo::onBtnClose, this));
        btn_close->setPressedActionEnabled(true);
    }
    
    this->scrollXepHang = ui::ScrollView::create();
    this->scrollXepHang->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollXepHang->setClippingEnabled(true);
    this->scrollXepHang->setContentSize(pnlTable->getContentSize());
    this->scrollXepHang->setAnchorPoint(pnlTable->getAnchorPoint());
    this->scrollXepHang->setPosition(pnlTable->getPosition());
    this->scrollXepHang->setScrollBarEnabled(true);
    this->scrollXepHang->setBounceEnabled(false);
    this->scrollXepHang->setTag(129323);
    this->scrollXepHang->setInnerContainerSize(Size(430, pnlTable->getContentSize().height));
    this->addChild(this->scrollXepHang);
    
    //this->lstop = "mrtuan1;124124|mrtuan2;124124124|mrtuan1;124124|mrtuan2;124124124|mrtuan1;124124|mrtuan2;124124124|mrtuan1;124124|mrtuan2;124124124|";//
    
    this->loadTyGia();
    this->setScale(1);
    return true;
}
void LayerBangXepHangTo::loadTopNoHuBauTom(){
    this->txtNhan->setString(dataManager.GetSysString(856));
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_GET_TOP_OPEN_JACKPOT_REQ, params,lastRoom));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
}
void LayerBangXepHangTo::loadTopByType(){
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(GAME_EXT_EVENT_GET_TOP_REQ, params,lastRoom));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
}
void LayerBangXepHangTo::onBtnClose(Ref* pSender){
    this->removeFromParent();
}
void LayerBangXepHangTo::loadTyGia(){
    auto lst = mUtils::splitString(this->lstop, '|');
    this->lstXH.clear();
    int height = 0;
    if (lst.size()<6){
        height = 691;
    }
    else{
        height = (lst.size())*100;
    }
    this->scrollXepHang->setInnerContainerSize(Size(430,height));
    this->scrollXepHang->removeAllChildren();
    for (int i =0;i<lst.size();i++){
        auto item =  mUtils::splitString(lst[i], ';');
        if (item.size()<2) continue;
        ItemBangXepHangTo* it = ItemBangXepHangTo::create();
        it->setAnchorPoint(Vec2(0,1));
        it->setDatas(i,item[0],mUtils::convertMoneyEx(atof(item[1].c_str())));
        this->scrollXepHang->addChild(it);
        it->setPosition(Vec2(0,height - (i+1) * 90));
    }
}
void LayerBangXepHangTo::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene) return;
   // log("OnExtensionResponse %s", cmd->c_str());
    if (strcmp("eetopresp", cmd->c_str()) == 0){//OK
        boost::shared_ptr<string> top = param->GetUtfString("top");
        SceneManager::getSingleton().hideLoading();
        //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

        if (top)
        {
            this->lstop = *top;
            this->loadTyGia();
        }
    }
    if (strcmp(GAME_EXT_EVENT_GET_TOP_RESP, cmd->c_str()) == 0){
        boost::shared_ptr<string> top = param->GetUtfString(EXT_FIELD_RESULT_TOP_10);
        SceneManager::getSingleton().hideLoading();
        //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
        if (top)
        {
            this->showTop(*top);
        }
    }
    if (strcmp(EXT_EVENT_TCC_GET_TOP_OPEN_JACKPOT_RES, cmd->c_str()) == 0){
        boost::shared_ptr<string> top = param->GetUtfString(EXT_FIELD_GAME_JACKPOT_TOP_OPEN_MONEY);
        SceneManager::getSingleton().hideLoading();
        //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
        if (top)
        {
            this->showTop(*top);
        }
    }
}
void LayerBangXepHangTo::showTop(string str){
    auto lst = mUtils::splitString(str, ';');
    this->lstXH.clear();
    int height = 0;
    if (lst.size()<6){
        height = 691;
    }
    else{
        height = (lst.size())*100;
    }
    this->scrollXepHang->setInnerContainerSize(Size(430,height));
    this->scrollXepHang->removeAllChildren();
    for (int i =0;i<lst.size();i++){
        auto item =  mUtils::splitString(lst[i], ',');
        if (item.size()<2) continue;
        ItemBangXepHangTo* it = ItemBangXepHangTo::create();
        it->setAnchorPoint(Vec2(0,1));
        it->setDatas(i,item[0],mUtils::convertMoneyEx(atof(item[1].c_str())));
        this->scrollXepHang->addChild(it);
        it->setPosition(Vec2(0,height - (i+1) * 90));
    }
}
void LayerBangXepHangTo::loadTop(int gid, int typeroom){
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt("gid", gid);
    params->PutInt("roomType", typeroom);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("eetoprq", params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
}


void LayerBangXepHangTo::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerBangXepHang::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerBangXepHangTo::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool ItemBangXepHangTo::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemXepHangTo.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    this->bg = static_cast<ImageView*>(root->getChildByName("bg"));
    this->name = static_cast<Text*>(this->bg->getChildByName("name"));
    this->money = static_cast<Text*>(this->bg->getChildByName("money"));
    this->imgTop = static_cast<ImageView*>(this->bg->getChildByName("imgRank"));
    this->txtRank = static_cast<Text*>(this->bg->getChildByName("txtRank"));
    
    return true;
}

void ItemBangXepHangTo::setDatas(int idx,string name,string money){
    if (idx<3){
        this->txtRank->setVisible(false);
        this->imgTop->setVisible(true);
        this->imgTop->loadTexture(StringUtils::format("rank%d.png",idx+1));
    }else{
        this->txtRank->setVisible(true);
        this->imgTop->setVisible(false);
        this->txtRank->setString(StringUtils::format("%d",idx+1));
    }
    if (idx %2 == 0){
        this->bg->loadTexture("bxhbg1.png");
    }else{
        this->bg->loadTexture("bxhbg2.png");
    }
    std::string _strNameSmall = "";
    (name.length() > 16) ? _strNameSmall = name.substr(0, 16) + "..." : _strNameSmall =name;
    this->name->setString(_strNameSmall);
    this->money->setString(money);
    if (SceneManager::getSingleton().isNagaCard){
        this->bg->loadTexture("table-tbody.png");
        
    }
}
LayerPopupXepHangSlot::LayerPopupXepHangSlot()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupXepHangSlot::~LayerPopupXepHangSlot()
{
    GameServer::getSingleton().removeListeners(this);
    
}
bool LayerPopupXepHangSlot::init()
{
    if (!Layer::init())
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupXepHangSlot.csb");
    
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;

    sizeAdd = ChanUtils::getSizePos();
    
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    
    
    auto pnlTable = dynamic_cast<Layout*>(pnlBg->getChildByName("pnlContent"));
    pnlTable->setVisible(false);
    
    ImageView* imgheader = dynamic_cast<ImageView*>(pnlBg->getChildByName("imgheader"));
    imgheader->loadTexture(StringUtils::format("design/ResSlot/%shead-bxh.png",SceneManager::getSingleton().getFolderLang().c_str()));
    Text* txtNhan = dynamic_cast<Text*>(pnlBg->getChildByName("txtNhan"));
    txtNhan->setString(dataManager.GetSysString(850));
    auto btn_close = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if (btn_close != NULL){
        btn_close->addClickEventListener(CC_CALLBACK_1(LayerPopupXepHangSlot::onBtnClose, this));
        btn_close->setPressedActionEnabled(true);
    }
    
   
    this->scrollXepHang = ui::ScrollView::create();
    this->scrollXepHang->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollXepHang->setClippingEnabled(true);
    this->scrollXepHang->setContentSize(pnlTable->getContentSize());
    this->scrollXepHang->setAnchorPoint(pnlTable->getAnchorPoint());
    this->scrollXepHang->setPosition(pnlTable->getPosition());
    this->scrollXepHang->setScrollBarEnabled(true);
    this->scrollXepHang->setBounceEnabled(false);
    this->scrollXepHang->setTag(129323);
    this->scrollXepHang->setInnerContainerSize(Size(430, pnlTable->getContentSize().height));
    pnlBg->addChild(this->scrollXepHang);
    
    this->loadTop();
    this->setScale(1);
    return true;
}
void LayerPopupXepHangSlot::onBtnClose(Ref* pSender){
    this->removeFromParent();
}
void LayerPopupXepHangSlot::loadTyGia(){
    auto lst = mUtils::splitString(this->lstop, '|');
    this->lstXH.clear();
    int height = 0;
    if (lst.size()<6){
        height = 691;
    }
    else{
        height = (lst.size())*100;
    }
    this->scrollXepHang->setInnerContainerSize(Size(430,height));
    this->scrollXepHang->removeAllChildren();
    for (int i =0;i<lst.size();i++){
        auto item =  mUtils::splitString(lst[i], ',');
        if (item.size()<2) continue;
        //(string time,string name,string money,string phien)
        ItemXepHangSlot* it = ItemXepHangSlot::create();
        it->setAnchorPoint(Vec2(0,1));
        it->setDatas(i,item[1],item[0],mUtils::convertMoneyEx(atof(item[3].c_str())),"#"+item[2]);
        this->scrollXepHang->addChild(it);
        it->setPosition(Vec2(0,height - (i+1) * 90));
    }
}
void LayerPopupXepHangSlot::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene) return;
   // log("OnExtensionResponse %s", cmd->c_str());
    if (strcmp(EXT_EVENT_GET_TOP_RES, cmd->c_str()) == 0){//OK
        boost::shared_ptr<string> top = param->GetUtfString(EXT_FIELD_SLOT_TOP_LIST);
        SceneManager::getSingleton().hideLoading();
        //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
        if (top)
        {
            this->lstop = *top;
            this->loadTyGia();
        }
    }
}

void LayerPopupXepHangSlot::loadTop(){
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_TOP_REQ, params,lastRoom));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
}


void LayerPopupXepHangSlot::onEnter()
{
    Layer::onEnter();
}

void LayerPopupXepHangSlot::onExit()
{
    Layer::onExit();
}
bool ItemXepHangSlot::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemXepHangSlot.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    this->bg = static_cast<ImageView*>(root->getChildByName("bg"));
    this->name = static_cast<Text*>(this->bg->getChildByName("name"));
    this->money = static_cast<Text*>(this->bg->getChildByName("money"));
    this->phien = static_cast<Text*>(this->bg->getChildByName("phien"));
    this->time = static_cast<Text*>(this->bg->getChildByName("time"));
    this->time->setFontSize(24);
    this->name->setFontSize(24);

    return true;
}

void ItemXepHangSlot::setDatas(int idx,string time,string name,string money,string phien){
    std::string _strNameSmall = "";
    (name.length() > 16) ? _strNameSmall = name.substr(0, 16) + "..." : _strNameSmall =name;
    this->name->setString(_strNameSmall);
    this->money->setString(money);
    this->phien->setString(phien);
    this->time->setString(time);
    if (idx %2 == 0){
        this->bg->loadTexture("bxhbg1.png");
    }else{
        this->bg->loadTexture("bxhbg2.png");
    }
    if (SceneManager::getSingleton().isNagaCard){
        this->bg->loadTexture("table-tbody.png");
    }
}

//No Hu Bau Tom
LayerPopupNoHuBauTom::LayerPopupNoHuBauTom()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupNoHuBauTom::~LayerPopupNoHuBauTom()
{
    GameServer::getSingleton().removeListeners(this);
    
}
bool LayerPopupNoHuBauTom::init()
{
    if (!Layer::init())
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupHuThuongBauTom.csb");
    
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
    
    sizeAdd = ChanUtils::getSizePos();
    
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    
    
    auto pnlTable = dynamic_cast<Layout*>(pnlBg->getChildByName("pnlContent"));
    pnlTable->setVisible(false);
    
    this->imgheader = dynamic_cast<ImageView*>(pnlBg->getChildByName("imgHeader"));
//
    Text* txtNhan = dynamic_cast<Text*>(pnlBg->getChildByName("txtNhan"));
    txtNhan->setString(dataManager.GetSysString(850));
    auto btn_close = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if (btn_close != NULL){
        btn_close->addClickEventListener(CC_CALLBACK_1(LayerPopupNoHuBauTom::onBtnClose, this));
        btn_close->setPressedActionEnabled(true);
    }
    
    auto btnLichSu = dynamic_cast<Button*>(pnlBg->getChildByName("btnLichSu"));
    if (btnLichSu != NULL){
        btnLichSu->addClickEventListener(CC_CALLBACK_1(LayerPopupNoHuBauTom::onBtnLichSuNoHu, this));
        btnLichSu->setPressedActionEnabled(true);
        btnLichSu->setTitleText(dataManager.GetSysString(856));
    }
    
    this->btnThang = dynamic_cast<Button*>(pnlBg->getChildByName("btnThang"));
    if (this->btnThang != NULL){
        this->btnThang->addClickEventListener(CC_CALLBACK_1(LayerPopupNoHuBauTom::onBtnThang, this));
        this->btnThang->setPressedActionEnabled(true);
        this->btnThang->setTitleText(StringUtils::format("%s %s",dataManager.GetSysString(660).c_str(),SceneManager::getSingleton().getCoin(SceneManager::getSingleton().getCurrRoomType()).c_str()));
    }
    this->btnThua = dynamic_cast<Button*>(pnlBg->getChildByName("btnThua"));
    if (this->btnThua != NULL){
        this->btnThua->addClickEventListener(CC_CALLBACK_1(LayerPopupNoHuBauTom::onBtnThua, this));
        this->btnThua->setPressedActionEnabled(true);
                this->btnThua->setTitleText(StringUtils::format("%s %s",dataManager.GetSysString(1034).c_str(),SceneManager::getSingleton().getCoin(SceneManager::getSingleton().getCurrRoomType()).c_str()));
    }
    
    this->txtLabelTimer = dynamic_cast<Text*>(pnlBg->getChildByName("txtLabelTimer"));
    this->txtLabelTimer->setString(dataManager.GetSysString(1033));
    this->txtTimer = dynamic_cast<Text*>(pnlBg->getChildByName("txtTimer"));
    this->txtTimer->setString("---");
    
    this->txtNoHu = dynamic_cast<Text*>(pnlBg->getChildByName("txtNoHu"));
    this->txtNoHu->setVisible(false);
    this->txtNoHu->setString(dataManager.GetSysString(1035));

    
    this->scrollXepHang = ui::ScrollView::create();
    this->scrollXepHang->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollXepHang->setClippingEnabled(true);
    this->scrollXepHang->setContentSize(pnlTable->getContentSize());
    this->scrollXepHang->setAnchorPoint(pnlTable->getAnchorPoint());
    this->scrollXepHang->setPosition(pnlTable->getPosition());
    this->scrollXepHang->setScrollBarEnabled(true);
    this->scrollXepHang->setBounceEnabled(false);
    this->scrollXepHang->setTag(129323);
    this->scrollXepHang->setInnerContainerSize(Size(430, pnlTable->getContentSize().height));
    pnlBg->addChild(this->scrollXepHang);
    

    return true;
}
void LayerPopupNoHuBauTom::onBtnThang(Ref* pSender){
    this->btnThua->setEnabled(true);
    this->btnThua->setBright(true);

    this->btnThang->setEnabled(false);
    this->btnThang->setBright(false);
    
    this->imgheader->loadTexture(StringUtils::format("design/ResBautom/%sheaderwin.png",SceneManager::getSingleton().getFolderLang().c_str()));
    
    int height = 0;
    if (this->lstXHWin.size()<6){
        height = 611;
    }
    else{
        height = (this->lstXHWin.size())*100;
    }
    this->scrollXepHang->setInnerContainerSize(Size(430,height));
    this->scrollXepHang->removeAllChildren();
    for (int i =0;i<this->lstXHWin.size();i++){
        ItemNoHuBauTom* it = ItemNoHuBauTom::create();
        it->setAnchorPoint(Vec2(0,1));
        it->setDatas(i+1,this->lstXHWin[i].name,this->lstXHWin[i].moneyspend,this->lstXHWin[i].moneyget);
        this->scrollXepHang->addChild(it);
        it->setPosition(Vec2(0,height - (i+1) * 90));
    }
    
}
void LayerPopupNoHuBauTom::onBtnThua(Ref* pSender){
    this->btnThua->setEnabled(false);
    this->btnThua->setBright(false);
    
    this->btnThang->setEnabled(true);
    this->btnThang->setBright(true);
    this->imgheader->loadTexture(StringUtils::format("design/ResBautom/%sheaderlose.png",SceneManager::getSingleton().getFolderLang().c_str()));

    int height = 0;
    if (this->lstXHLose.size()<6){
        height = 611;
    }
    else{
        height = (this->lstXHLose.size())*100;
    }
    this->scrollXepHang->setInnerContainerSize(Size(430,height));
    this->scrollXepHang->removeAllChildren();
    for (int i =0;i<this->lstXHLose.size();i++){
        ItemNoHuBauTom* it = ItemNoHuBauTom::create();
        it->setAnchorPoint(Vec2(0,1));
        it->setDatas(i+1,this->lstXHLose[i].name,this->lstXHLose[i].moneyspend,this->lstXHLose[i].moneyget);
        this->scrollXepHang->addChild(it);
        it->setPosition(Vec2(0,height - (i+1) * 90));
    }
}
void LayerPopupNoHuBauTom::onBtnLichSuNoHu(Ref* pSender){
    LayerBangXepHangTo* xh = LayerBangXepHangTo::create();
    xh->loadTopNoHuBauTom();
    this->addChild(xh,700000);
}
void LayerPopupNoHuBauTom::onBtnClose(Ref* pSender){
    this->removeFromParent();
}
void LayerPopupNoHuBauTom::loadTyGia(){

}
void LayerPopupNoHuBauTom::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene) return;
   // log("OnExtensionResponse %s", cmd->c_str());
    if (strcmp(EXT_EVENT_TCC_GET_TOP_POINT_JACKPOT_RES, cmd->c_str()) == 0){//OK
        boost::shared_ptr<string> top = param->GetUtfString(EXT_FIELD_GAME_JACKPOT_TOP_POINT);
        SceneManager::getSingleton().hideLoading();
        //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
        if (top)
        {
            this->lstop = *top;
            //naga1751511,0,22957;mrtuan4,0,19131;thuytest1133,0,17217;|
            this->lstXHWin.clear();
            this->lstXHLose.clear();
            auto lst = mUtils::splitString(this->lstop, '|');
            if (lst.size()>0 && lst.size()<2){
                auto listWin = mUtils::splitString(lst[0], ';');
                for (int i = 0; i < listWin.size();i++){
                    auto it = mUtils::splitString(listWin[i], ',');
                    if (it.size()>2){
                        itemxh item;
                        item.name = it[0];
                        item.moneyspend = it[1];
                        item.moneyget = it[2];
                        this->lstXHWin.push_back(item);
                    }
                }
            }
            if (lst.size()>1){
                auto listWin = mUtils::splitString(lst[0], ';');
                for (int i = 0; i < listWin.size();i++){
                    auto it = mUtils::splitString(listWin[i], ',');
                    if (it.size()>2){
                        itemxh item;
                        item.name = it[0];
                        item.moneyspend = it[1];
                        item.moneyget = it[2];
                        this->lstXHWin.push_back(item);
                    }
                }
                
                auto listLose = mUtils::splitString(lst[1], ';');
                for (int i = 0; i < listLose.size();i++){
                    auto it = mUtils::splitString(listLose[i], ',');
                    if (it.size()>2){
                        itemxh item;
                        item.name = it[0];
                        item.moneyspend = it[1];
                        item.moneyget = it[2];
                        this->lstXHLose.push_back(item);
                    }
                }
            }
            this->onBtnThang(this->btnThang);
        }
    }
}
void LayerPopupNoHuBauTom::updateTimeMoQuy(float dt){
    
    int _time = this->timeNoHu--;
    if (this->timeNoHu<=0){
        this->txtTimer->setString("00:00:00:00");
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupNoHuBauTom::updateTimeMoQuy),this);
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
    this->txtTimer->setString(StringUtils::format("%dd:%s:%s:%s",_day,strH.c_str(),strM.c_str(),strS.c_str()));
}
void LayerPopupNoHuBauTom::setTimeNoHu(int time){
    this->timeNoHu = time;
    if (this->timeNoHu>0){
        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupNoHuBauTom::updateTimeMoQuy),this,1, false);
    }else{
        this->txtTimer->setString("00:00:00:00");
    }
    this->loadTop();

}
void LayerPopupNoHuBauTom::showNoHuThuong(string top){
    auto emitter = ParticleSystemQuad::create("particles/coin_rain.plist");
    Size visibleSize = Director::getInstance()->getVisibleSize();

    emitter->setPosition(Vec2(visibleSize.width/2,visibleSize.height));
    emitter->setAutoRemoveOnFinish(true);
    //emitter->setScale(3);
    emitter->setTag(278122);
    emitter->setAutoRemoveOnFinish(true);
    emitter->setScale(2.5);
    emitter->setLife(3);
    emitter->setTotalParticles(400);
    // emitter->set
    if (SceneManager::getSingleton().getCurrRoomType() == 0){
        TextureCache* tc = Director::getInstance()->getTextureCache();
        Texture2D* coin = tc->addImage("ResBautom/dongmoc-min.png");
        emitter->setTexture(coin);
    }
    else{
        TextureCache* tc = Director::getInstance()->getTextureCache();
        Texture2D* coin = tc->addImage("ResBautom/donghao-min.png");
        emitter->setTexture(coin);
    }
    this->addChild(emitter, 1000000);
    
    this->setScale(1);
    this->txtLabelTimer->setVisible(false);
    this->txtTimer->setVisible(false);
    this->txtNoHu->setVisible(true);
    this->txtNoHu->setString((dataManager.GetSysString(1035)));
    this->lstop = top;
    //naga1751511,0,22957;mrtuan4,0,19131;thuytest1133,0,17217;|
    this->lstXHWin.clear();
    this->lstXHLose.clear();
    auto lst = mUtils::splitString(this->lstop, '|');
    if (lst.size()>0 && lst.size()<2){
        auto listWin = mUtils::splitString(lst[0], ';');
        for (int i = 0; i < listWin.size();i++){
            auto it = mUtils::splitString(listWin[i], ',');
            if (it.size()>2){
                itemxh item;
                item.name = it[0];
                item.moneyspend = it[1];
                item.moneyget = it[2];
                this->lstXHWin.push_back(item);
            }
        }
    }
    if (lst.size()>1){
        auto listWin = mUtils::splitString(lst[0], ';');
        for (int i = 0; i < listWin.size();i++){
            auto it = mUtils::splitString(listWin[i], ',');
            if (it.size()>2){
                itemxh item;
                item.name = it[0];
                item.moneyspend = it[1];
                item.moneyget = it[2];
                this->lstXHWin.push_back(item);
            }
        }
        
        auto listLose = mUtils::splitString(lst[1], ';');
        for (int i = 0; i < listLose.size();i++){
            auto it = mUtils::splitString(listLose[i], ',');
            if (it.size()>2){
                itemxh item;
                item.name = it[0];
                item.moneyspend = it[1];
                item.moneyget = it[2];
                this->lstXHLose.push_back(item);
            }
        }
    }
    if (this->lstXHWin.size()>0){
        this->onBtnThang(this->btnThang);
    }
    else if (this->lstXHLose.size() > 0){
        this->onBtnThua(this->btnThua);
    }
}
void LayerPopupNoHuBauTom::loadTop(){
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_GET_TOP_POINT_JACKPOT_REQ, params,lastRoom));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
}


void LayerPopupNoHuBauTom::onEnter()
{
    Layer::onEnter();
}

void LayerPopupNoHuBauTom::onExit()
{
    Layer::onExit();
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupNoHuBauTom::updateTimeMoQuy),this);

}
bool ItemNoHuBauTom::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemHuThuongBauTom.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    auto pnlBg = static_cast<Layout*>(root->getChildByName("pnlBg"));
    pnlBg->setSwallowTouches(false);
    this->txtSTT = static_cast<Text*>(pnlBg->getChildByName("txtSTT"));
    this->txtName = static_cast<Text*>(pnlBg->getChildByName("txtName"));
    this->txtMoneySpend = static_cast<Text*>(pnlBg->getChildByName("txtMoneySpend"));
    this->txtMoneyGet = static_cast<Text*>(pnlBg->getChildByName("txtMoneyGet"));
   
    return true;
}

void ItemNoHuBauTom::setDatas(int idx,string name,string moneyspend,string moneyget){
    this->txtSTT->setString(StringUtils::format("#%d",idx));
    std::string _strNameSmall = "";
    (name.length() > 16) ? _strNameSmall = name.substr(0, 16) + "..." : _strNameSmall =name;
    this->txtName->setString(_strNameSmall);
    this->txtMoneySpend->setString(mUtils::convertMoneyEx(atof(moneyspend.c_str())));
    this->txtMoneyGet->setString(mUtils::convertMoneyEx(atof(moneyget.c_str())));
}

