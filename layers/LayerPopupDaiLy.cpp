//
//  LayerPopupDaiLy.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerPopupDaiLy.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Requests/ExtensionRequest.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "LayerPopupBanMoc.h"
#include "../scenes/ScenePickRoom.h"
LayerPopupDaiLy::LayerPopupDaiLy()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupDaiLy::~LayerPopupDaiLy()
{
    GameServer::getSingleton().removeListeners(this);

}

bool LayerPopupDaiLy::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupDaiLy.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
 //   if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupDaiLy::onBtnClose, this));
    }

    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    
    this->btnMB = dynamic_cast<Button*>(pnlBg->getChildByName("btnMB"));
    if(this->btnMB ){
        this->btnMB ->addClickEventListener(CC_CALLBACK_1(LayerPopupDaiLy::onBtnMB, this));
    }
    this->btnMT = dynamic_cast<Button*>(pnlBg->getChildByName("btnMT"));
    if(this->btnMT ){
        this->btnMT ->addClickEventListener(CC_CALLBACK_1(LayerPopupDaiLy::onBtnMT, this));
    }
    this->btnMN = dynamic_cast<Button*>(pnlBg->getChildByName("btnMN"));
    if(this->btnMN ){
        this->btnMN ->addClickEventListener(CC_CALLBACK_1(LayerPopupDaiLy::onBtnMN, this));
    }
    
    auto pnltbl = static_cast<Layout*>(pnlBg->getChildByName("pnlTable"));
    
   
    //
    pnltbl->setVisible(false);
    pnltbl->setSwallowTouches(false);
    this->tblDaiLy = TableView::create(this, Size(pnltbl->getContentSize().width, pnltbl->getContentSize().height ));
    this->tblDaiLy->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tblDaiLy->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblDaiLy->setPosition(Vec2(pnltbl->getPosition().x + sizeAdd.width, pnltbl->getPosition().y));
    this->tblDaiLy->setDelegate(this);
    this->tblDaiLy->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    pnlBg->addChild(this->tblDaiLy);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_FAKE_DAILY_REQ,params));
    GameServer::getSingleton().Send(request);
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->hideWebView();
    }

    //this->txtChuChay->setScale(5);
    return true;
}
void LayerPopupDaiLy::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerPopupDaiLy::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(1470, 82);
}

TableViewCell* LayerPopupDaiLy::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        auto cellitem = LayerItemDaiLy::create();
        cellitem->setDatas(StringUtils::format("%d",idx+1), this->lstDaiLy[idx].ten, this->lstDaiLy[idx].khuvuc, this->lstDaiLy[idx].ma);
        cellitem->setTag(190);
        cell->addChild(cellitem);
    }
    else
    {
        LayerItemDaiLy* cellitem = (LayerItemDaiLy*)cell->getChildByTag(190);
        if (cellitem){
            cellitem->setDatas(StringUtils::format("%d",idx), this->lstDaiLy[idx].ten, this->lstDaiLy[idx].khuvuc, this->lstDaiLy[idx].ma);
            
        }
    }
    return cell;
    
}
ssize_t LayerPopupDaiLy::numberOfCellsInTableView(TableView *table)
{
    
    return this->lstDaiLy.size();//lstRank.size();
}

void LayerPopupDaiLy::onBtnMB(Ref* pSender){
    this->btnMB->setEnabled(false);
    this->btnMB->setBright(false);
    
    this->btnMT->setEnabled(true);
    this->btnMT->setBright(true);
    
    this->btnMN->setEnabled(true);
    this->btnMN->setBright(true);
    this->lstDaiLy.clear();
    auto lst = mUtils::splitString(this->strdaily, ';');
    for (int i = 0; i< lst.size();i++){
        auto items = mUtils::splitString(lst[i], '-');
        if (atoi(items[0].c_str()) == 1){
            item it;
            if (items.size()<4)
                continue;
            it.ten = items[1];
            it.ma = items[2];
            it.khuvuc = items[3];
            this->lstDaiLy.push_back(it);
        }
    }
    this->tblDaiLy->reloadData();
    
    
    
}
void LayerPopupDaiLy::onBtnMT(Ref* pSender){
    
    this->btnMB->setEnabled(true);
    this->btnMB->setBright(true);
    
    this->btnMT->setEnabled(false);
    this->btnMT->setBright(false);
    
    this->btnMN->setEnabled(true);
    this->btnMN->setBright(true);
    
    this->lstDaiLy.clear();
    auto lst = mUtils::splitString(this->strdaily, ';');
    for (int i = 0; i< lst.size();i++){
        auto items = mUtils::splitString(lst[i], '-');
        if (atoi(items[0].c_str()) == 2){
            item it;
            if (items.size()<4)
                continue;
            it.ten = items[1];
            it.ma = items[2];
            it.khuvuc = items[3];
            this->lstDaiLy.push_back(it);
        }
    }
    this->tblDaiLy->reloadData();
}
void LayerPopupDaiLy::onBtnMN(Ref* pSender){
    this->btnMB->setEnabled(true);
    this->btnMB->setBright(true);
    
    this->btnMT->setEnabled(true);
    this->btnMT->setBright(true);
    
    this->btnMN->setEnabled(false);
    this->btnMN->setBright(false);
    this->lstDaiLy.clear();
    auto lst = mUtils::splitString(this->strdaily, ';');
    for (int i = 0; i< lst.size();i++){
        auto items = mUtils::splitString(lst[i], '-');
        if (atoi(items[0].c_str()) == 3){
            item it;
            if (items.size()<4)
                continue;
            it.ten = items[1];
            it.ma = items[2];
            it.khuvuc = items[3];
            this->lstDaiLy.push_back(it);
        }
    }
    this->tblDaiLy->reloadData();

}
void LayerPopupDaiLy::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
   // log("OnExtensionResponse %s", cmd->c_str());
    if (strcmp(EXT_EVENT_GET_FAKE_DAILY_RESP, cmd->c_str()) == 0){//OK
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetUtfString("rd") == NULL) return;
        
        this->strdaily = *param->GetUtfString("rd");
        this->onBtnMB(NULL);
    }
}
void LayerPopupDaiLy::hideLayer(){
    this->setVisible(false);
}
void LayerPopupDaiLy::showLayer(){
    this->setVisible(true);

}
bool LayerPopupDaiLy::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerPopupDaiLy::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerPopupDaiLy::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupDaiLy::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupDaiLy::onExit()
{
    Layer::onExit();
    if (!SceneManager::getSingleton().getIsShowTX() && !SceneManager::getSingleton().getIsShowXD()&& !SceneManager::getSingleton().getIsShowPK()&& !SceneManager::getSingleton().getIsShowVQ()&& !SceneManager::getSingleton().getIsShowCT()){

    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return ;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->showWebView();
    }
    }
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerItemDaiLy::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemDaiLy.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    auto imgbg = dynamic_cast<ImageView*>(root->getChildByName("imgbg"));
    btnBanMoc = dynamic_cast<Button*>(imgbg->getChildByName("btnBanMoc"));
    this->txtSTT = dynamic_cast<Text*>(imgbg->getChildByName("txtSTT"));
    this->txtTenDaily= dynamic_cast<Text*>(imgbg->getChildByName("txtTenDaily"));
    this->txtMaDaiLy= dynamic_cast<Text*>(imgbg->getChildByName("txtMaDaiLy"));
    this->txtKhuVuc= dynamic_cast<Text*>(imgbg->getChildByName("txtKhuVuc"));
    if (this->btnBanMoc)
        this->btnBanMoc->addClickEventListener(CC_CALLBACK_1(LayerItemDaiLy::onBtnBanMoc, this));
    
    return true;
}

void LayerItemDaiLy::setDatas(string stt, string name, string khuvuc, string ma)
{
    this->txtSTT->setString(stt);
    this->txtTenDaily->setString(name);
    this->txtMaDaiLy->setString(ma);
    this->txtKhuVuc->setString(khuvuc);
}

void LayerItemDaiLy::onBtnBanMoc(Ref* pSender){
    
    auto currscene = Director::getInstance()->getRunningScene();
    if (currscene == NULL)
        return;
    LayerPopupBanMoc* popup = LayerPopupBanMoc::create();
    popup->setDaiLy(this->txtTenDaily->getString());
    currscene->addChild(popup,2000);
}
