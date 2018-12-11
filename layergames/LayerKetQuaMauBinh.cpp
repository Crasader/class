//
//  LayerKetQuaMauBinh.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 4/25/17.
//
//

#include "LayerKetQuaMauBinh.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"

LayerKetQuaMauBinh::LayerKetQuaMauBinh()
{
}


LayerKetQuaMauBinh::~LayerKetQuaMauBinh()
{
    
}

bool LayerKetQuaMauBinh::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerShowKetQuaMauBinh.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
    }else{
        sizeAdd = Size(0,0);
    }
    Size sizeAdd2;
    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
        sizeAdd2 = ChanUtils::getSizePos();
        
    }else{
        sizeAdd2 = Size(0,0);
    }
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
//    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
//    if(btnClose){
//        btnClose->addClickEventListener(CC_CALLBACK_1(LayerKetQuaMauBinh::onBtnClose, this));
//    }
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(922));
    auto pnltbl = static_cast<ImageView*>(pnlBg->getChildByName("pnlTable"));
    
    //
    
    this->tblKetQua = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height-10));
    this->tblKetQua->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tblKetQua->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblKetQua->setPosition(Vec2(pnltbl->getPosition().x+sizeAdd.width,pnltbl->getPosition().y+10));
    this->tblKetQua->setDelegate(this);
    this->tblKetQua->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    pnlBg->addChild(this->tblKetQua);
    

    this->btnStart = static_cast<Button*>(pnlBg->getChildByName("btnStart"));
    if (this->btnStart){
        this->btnStart->addClickEventListener(CC_CALLBACK_1(LayerKetQuaMauBinh::onBtnBatDau,this));
        this->btnStart->setVisible(false);
        this->btnStart->setTitleText(dataManager.GetSysString(29));
    }
    
    
    
    this->layerTimer = LayerTimer::create();
    this->layerTimer->setCount(15);
    this->layerTimer->setLocalZOrder(130);
    this->layerTimer->setScale(0.6);
    this->layerTimer->stopTimer();
    this->layerTimer->setPosition(0,450);
    //this->layerTimer.getAnchorPoint(Image_2.getAnchorPoint());
    //this->layerTimer.setPosition(Image_2.getPosition());
    pnlBg->addChild(this->layerTimer);
    // this->layerTimer.setVisible(true);

    
    return true;
}

void LayerKetQuaMauBinh::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerKetQuaMauBinh::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(1055, 100);
}

TableViewCell* LayerKetQuaMauBinh::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    auto it = this->lstkq[idx];
    if (!cell) {
        
        cell = new TableViewCell();
        cell->autorelease();
        
        LayerItemKetQuaMauBinh* kq = LayerItemKetQuaMauBinh::create();
        
        kq->setDatas(it.name,it.chi,it.tien);
        kq->setTag(123);
        cell->addChild(kq);
        
    }
    else
    {
        LayerItemKetQuaMauBinh* kq = (LayerItemKetQuaMauBinh*)cell->getChildByTag(123);
        if (kq){
            kq->setDatas(it.name,it.chi,it.tien);
        }
    }
    return cell;
    
}

ssize_t LayerKetQuaMauBinh::numberOfCellsInTableView(TableView *table)
{
    
    return this->lstkq.size();
}
void LayerKetQuaMauBinh::onBtnBatDau(Ref* pSender){
    boost::shared_ptr<ISFSObject> parameter(new SFSObject());
    boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
    GameServer::getSingleton().Send(request);
    this->removeFromParentAndCleanup(true);
}
bool LayerKetQuaMauBinh::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerKetQuaMauBinh::startReady(int _time){
                                                  this->btnStart->setVisible(true);
                                                  this->layerTimer->setCount(_time);
                                                  this->layerTimer->startTimer();
                                              }
void LayerKetQuaMauBinh::setKetQua(string str){
    auto lstkq = mUtils::splitString(str,';');
    for (int i = 0; i < lstkq.size(); i++){
        auto lstrow = mUtils::splitString(lstkq[i],'|');
        ketqua kq;
        kq.name = lstrow[0];
        kq.chi = lstrow[2];
        kq.tien = lstrow[1];
        this->lstkq.push_back(kq);
    }
    this->tblKetQua->reloadData();
}

void LayerKetQuaMauBinh::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerKetQuaMauBinh::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerKetQuaMauBinh::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerItemKetQuaMauBinh::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemKetQuaMB.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    Layout* bg = static_cast<Layout*>(root->getChildByName("Panel_1_0"));
    this->txtName = static_cast<Text*>(bg->getChildByName("txtName"));
    this->txtChi = static_cast<Text*>(bg->getChildByName("txtChi"));
    this->txtTien = static_cast<Text*>(bg->getChildByName("txtTien"));
    
    return true;
}
void LayerItemKetQuaMauBinh::setDatas(string name,string chi,string tien){
    string schi = "";
    if (atoi(chi.c_str())>=0){
        if (SceneManager::getSingleton().currLang == 1){
            if (atoi(chi.c_str())<=1)
                schi = "+" +chi+" "+dataManager.GetSysString(921);
            else
                schi = "+" +chi+" "+dataManager.GetSysString(921)+"s";
        }
        else{
            schi = "+" +chi+" "+dataManager.GetSysString(921);
        }
        
    }else{
        if (SceneManager::getSingleton().currLang == 1){
            if (atoi(chi.c_str())>=-1)
                schi = chi+" "+dataManager.GetSysString(921);
            else
                schi = chi+" "+dataManager.GetSysString(921)+"s";
        }else{
            schi = chi+" "+dataManager.GetSysString(921);
        }
    }
    string stien = "";
    if (atoi(tien.c_str())>=0){
        stien = "+" +mUtils::convertMoneyEx(atoll(tien.c_str()));
    }else{
        long long money = 0 - atoll(tien.c_str());
        stien = "-" + mUtils::convertMoneyEx(money);
    }
	this->txtName->setString(formatUserName(name, 13));
    this->txtChi->setString(schi);
    this->txtTien->setString(stien);

    
}
