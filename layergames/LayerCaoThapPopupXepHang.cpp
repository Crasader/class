//
//  LayerCaoThapItemXepHang.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 11/14/16.
//
//

#include "LayerCaoThapPopupXepHang.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"
#include "Requests/ExtensionRequest.h"

LayerCaoThapPopupXepHang::LayerCaoThapPopupXepHang()
{
    GameServer::getSingleton().addListeners(this);
}


LayerCaoThapPopupXepHang::~LayerCaoThapPopupXepHang()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerCaoThapPopupXepHang::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupXepHangCaoThap.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
    pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
//    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
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
    auto Text_1 = static_cast<Text*>(pnlBg->getChildByName("Text_1"));
    Text_1->setString(dataManager.GetSysString(850));
    auto Image_17 = static_cast<ImageView*>(pnlBg->getChildByName("Image_17"));
    Image_17->loadTexture(StringUtils::format("design/LayerGameCaoThap/%shead-bxh.png",SceneManager::getSingleton().getFolderLang().c_str()));
    
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupXepHang::onBtnClose, this));
    }
    
    auto pnltbl = static_cast<ImageView*>(pnlBg->getChildByName("imgbgTable"));
    
    //
    
    this->tblListRank = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height-10));
    this->tblListRank->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tblListRank->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblListRank->setPosition(Vec2(pnltbl->getPosition().x+sizeAdd.width,pnltbl->getPosition().y+10));
    this->tblListRank->setDelegate(this);
    this->tblListRank->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    pnlBg->addChild(this->tblListRank);
    
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_CAOTHAP_GET_TOP_WIN,params));
        GameServer::getSingleton().Send(request);
    
    
        SceneManager::getSingleton().showLoading();
    
    
    return true;
}

void LayerCaoThapPopupXepHang::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerCaoThapPopupXepHang::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(1239,70);
}

TableViewCell* LayerCaoThapPopupXepHang::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    item it = lstRank.at(idx);
    if (!cell) {
        
        cell = new TableViewCell();
        cell->autorelease();
        
        LayerCaoThapItemXepHang* his = LayerCaoThapItemXepHang::create();
        
        his->setDatas(it.stt,it.name,it.thang);
        his->setTag(123);
        cell->addChild(his);
        
    }
    else
    {
        LayerCaoThapItemXepHang* his = (LayerCaoThapItemXepHang*)cell->getChildByTag(123);
        if (his){
            his->setDatas(it.stt,it.name,it.thang);
        }
    }
    return cell;
    
}

ssize_t LayerCaoThapPopupXepHang::numberOfCellsInTableView(TableView *table)
{
    
    return lstRank.size();
}
void LayerCaoThapPopupXepHang::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerCaoThapPopupXepHang::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(EXT_EVENT_CAOTHAP_GET_TOP_WIN_RESP, cmd->c_str()) == 0){
        boost::shared_ptr<string> strhis = param->GetUtfString("ghtp");
        //abcd589262|1482894579|10000|14_1;4_3;5_2|11000|0@0@0@0;1@10000@10000@10000;0@10000@11000@53000|11000;12100;38500#abcd589262|1482894569|10000|8_4;7_1|0|0@0@0@0;0@10000@17000@17000|0;0;0#
        if (strhis != NULL){
         
            auto lstRanks = mUtils::splitString(*strhis, '#');
            this->lstRank.clear();
            for(int i = 0; i< lstRanks.size();i++){
                auto lstItem = mUtils::splitString(lstRanks[i], '|');
                if (lstItem.size() < 6)
                    return;
                item it;
                it.stt = i+1;
                it.name = lstItem[0];
                it.thang = lstItem[4];
              this->lstRank.push_back(it);
            }
            this->tblListRank->reloadData();
            SceneManager::getSingleton().hideLoading();
        }
        
    }
}
string LayerCaoThapPopupXepHang::getTime(string str){
    time_t tt = atoi(str.c_str());
    struct tm * ptm = localtime(&tt);
    char buf[30];
    strftime (buf, 30, "%d/%m/%Y %H:%M:%S",  ptm);
    return string(buf);
}
bool LayerCaoThapPopupXepHang::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerCaoThapPopupXepHang::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerCaoThapPopupXepHang::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerCaoThapPopupXepHang::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerCaoThapItemXepHang::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemXepHangCaoThap.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    
    this->bg = static_cast<Layout*>(root->getChildByName("Panel_4"));
    this->bg->setSwallowTouches(false);
    this->txtRank = static_cast<Text*>(this->bg->getChildByName("txtRank"));
    this->txtName = static_cast<Text*>(this->bg->getChildByName("txtName"));
    this->txtGetMoney = static_cast<Text*>(this->bg->getChildByName("txtGetMoney"));
    
    return true;
}
void LayerCaoThapItemXepHang::setDatas(int stt,string name, string thang){
        this->txtRank->setString(StringUtils::format("%d",stt));
        this->txtName->setString(ChanUtils::GetNamePlayer(name));
    this->txtGetMoney->setString(mUtils::convertMoneyEx(atof(thang.c_str())));
}
