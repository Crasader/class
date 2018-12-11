//
//  LayerXepHangMiniPoker.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 9/1/16.
//
//

#include "LayerXepHangMiniPoker.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"

LayerXepHangMiniPoker::LayerXepHangMiniPoker()
{
    GameServer::getSingleton().addListeners(this);
}


LayerXepHangMiniPoker::~LayerXepHangMiniPoker()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerXepHangMiniPoker::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerXephangMiniPoker.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));

    pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
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
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    auto Image_20 = static_cast<ImageView*>(pnlBg->getChildByName("Image_20"));
    Image_20->loadTexture(StringUtils::format("design/LayerMiniPoker/%shead-bxh.png",SceneManager::getSingleton().getFolderLang().c_str()));
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(850));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerXepHangMiniPoker::onBtnClose, this));
    }
    
    auto pnltbl = static_cast<ImageView*>(pnlBg->getChildByName("pnlTable"));
    
    //
    
    this->tblListRank = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height-10));
    this->tblListRank->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tblListRank->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblListRank->setPosition(Vec2(pnltbl->getPosition().x+sizeAdd.width,pnltbl->getPosition().y+10));
    this->tblListRank->setDelegate(this);
    this->tblListRank->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    pnlBg->addChild(this->tblListRank);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PK_RANK_REQ,params));
    GameServer::getSingleton().Send(request);
    
    
    SceneManager::getSingleton().showLoading();
    this->setScale(1);
    
    return true;
}

void LayerXepHangMiniPoker::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerXepHangMiniPoker::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
        return Size(1433, 118);

    }else{
        return Size(1142, 81);
    }
}

TableViewCell* LayerXepHangMiniPoker::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    item it = lstRank.at(idx);
    if (!cell) {
        
        cell = new TableViewCell();
        cell->autorelease();
        
        LayerItemRankMiniPoker* his = LayerItemRankMiniPoker::create();
        
        his->setDatas(idx+1,it.name,it.time,it.bet,it.win,it.ketqua);
        his->setTag(123);
        cell->addChild(his);
        
    }
    else
    {
        LayerItemRankMiniPoker* his = (LayerItemRankMiniPoker*)cell->getChildByTag(123);
        if (his){
            his->setDatas(idx+1,it.name,it.time,it.bet,it.win,it.ketqua);
        }
    }
    return cell;
    
}

ssize_t LayerXepHangMiniPoker::numberOfCellsInTableView(TableView *table)
{
    
    return lstRank.size();
}
void LayerXepHangMiniPoker::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerXepHangMiniPoker::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(EXT_EVENT_PK_RANK_RESP, cmd->c_str()) == 0){
        //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

        boost::shared_ptr<string> strhis = param->GetUtfString("ghtp");
        if (strhis != NULL){
            auto lstRanks = mUtils::splitString(*strhis, '#');
            this->lstRank.clear();
            for(int i = 0; i< lstRanks.size();i++){
                auto lstItem = mUtils::splitString(lstRanks[i], '|');
                if (lstItem.size() < 6)
                    return;
                item it;
                it.name = lstItem[0];
                it.time = this->getTime(lstItem[1]);
                it.bet = mUtils::convertMoneyEx(atof(lstItem[2].c_str()));
                it.win = mUtils::convertMoneyEx(atof(lstItem[4].c_str()));
                it.ketqua = this->getResult(atoi(lstItem[5].c_str()));
                this->lstRank.push_back(it);
            }
            this->tblListRank->reloadData();
            SceneManager::getSingleton().hideLoading();
        }
        
    }
}
string LayerXepHangMiniPoker::getTime(string str){
    time_t tt = atoi(str.c_str());
    struct tm * ptm = localtime(&tt);
    char buf[30];
    strftime (buf, 30, "%d/%m/%Y %H:%M:%S",  ptm);
    return string(buf);
}
string LayerXepHangMiniPoker::getResult(int rc){
    switch (rc){
        case 1:
            return dataManager.GetSysString(632);
        case 2:
            return dataManager.GetSysString(677);
        case 3:
            return dataManager.GetSysString(676);
        case 4:
            return dataManager.GetSysString(557);
        case 5:
            return dataManager.GetSysString(675);
        case 6:
            return dataManager.GetSysString(648);
        case 7:
            return dataManager.GetSysString(647);
        case 8:
            return dataManager.GetSysString(611);
        case 9:
            return dataManager.GetSysString(589);
        case 10:
            return dataManager.GetSysString(590);
        case 12:
            return "Lucky 777";
    }
}
bool LayerXepHangMiniPoker::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerXepHangMiniPoker::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerXepHangMiniPoker::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerXepHangMiniPoker::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerItemRankMiniPoker::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemXepHangMiniPoker.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
//        this->imgbg = static_cast<ImageView*>(root->getChildByName("imgBg"));
//        this->txtRank = static_cast<Text*>(imgbg->getChildByName("txtRank"));
//        this->txtName = static_cast<Text*>(imgbg->getChildByName("txtName"));
//        this->txtMoney = static_cast<Text*>(imgbg->getChildByName("txtMoney"));
        this->imgBg = static_cast<ImageView*>(root->getChildByName("imgBg"));
        this->txtName = static_cast<Text*>(this->imgBg->getChildByName("txtName"));
        this->txtBet = static_cast<Text*>(this->imgBg->getChildByName("txtBet"));
        this->txtWin = static_cast<Text*>(this->imgBg->getChildByName("txtWin"));
        this->txtResult = static_cast<Text*>(this->imgBg->getChildByName("txtResult"));
        this->txtTime = static_cast<Text*>(this->imgBg->getChildByName("txtTime"));
        this->imgLine = static_cast<ImageView*>(this->imgBg->getChildByName("imgLine"));

        
    }
    else{
        this->imgBg = static_cast<ImageView*>(root->getChildByName("imgBg"));
        this->txtName = static_cast<Text*>(this->imgBg->getChildByName("txtName"));
        this->txtBet = static_cast<Text*>(this->imgBg->getChildByName("txtBet"));
        this->txtWin = static_cast<Text*>(this->imgBg->getChildByName("txtWin"));
        this->txtResult = static_cast<Text*>(this->imgBg->getChildByName("txtResult"));
        this->txtTime = static_cast<Text*>(this->imgBg->getChildByName("txtTime"));
    }
    
    return true;
}
void LayerItemRankMiniPoker::setDatas(int stt, string name,string time,string bet, string win,string ketqua){

        this->txtName->setString(ChanUtils::GetNamePlayer(name));
        this->txtTime->setString(time);
        this->txtBet->setString(bet);
        this->txtWin->setString(win);
        this->txtResult->setString(ketqua);

}
