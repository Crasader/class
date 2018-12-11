//
//  LayerLichSuMiniPoker.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 9/1/16.
//
//

#include "LayerLichSuMiniPoker.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"

LayerLichSuMiniPoker::LayerLichSuMiniPoker()
{
    GameServer::getSingleton().addListeners(this);
}


LayerLichSuMiniPoker::~LayerLichSuMiniPoker()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerLichSuMiniPoker::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerLichSuMiniPoker.csb");
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
    auto Image_20 = static_cast<ImageView*>(pnlBg->getChildByName("Image_20"));
    Image_20->loadTexture(StringUtils::format("design/LayerMiniPoker/%shead-ls.png",SceneManager::getSingleton().getFolderLang().c_str()));
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(856));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerLichSuMiniPoker::onBtnClose, this));
    }
    
    auto pnltbl = static_cast<ImageView*>(pnlBg->getChildByName("pnlTable"));
    
    //
    
    this->tblListHis = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height-10));
    this->tblListHis->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tblListHis->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblListHis->setPosition(Vec2(pnltbl->getPosition().x+sizeAdd.width,pnltbl->getPosition().y+10));
    this->tblListHis->setDelegate(this);
    this->tblListHis->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    pnlBg->addChild(this->tblListHis);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PK_USER_HIS_REQ,params));
    GameServer::getSingleton().Send(request);
    
    
    SceneManager::getSingleton().showLoading();
    this->setScale(1);
    
    return true;
}

void LayerLichSuMiniPoker::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerLichSuMiniPoker::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(1239, 70);
}

TableViewCell* LayerLichSuMiniPoker::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    item it = lstHis.at(idx);
    if (!cell) {
        
        cell = new TableViewCell();
        cell->autorelease();
        
        LayerItemLichSuMiniPoker* his = LayerItemLichSuMiniPoker::create();
        
        his->setDatas(it.phien,it.time,it.bet,it.result,it.win);
        his->setTag(123);
        cell->addChild(his);
        
    }
    else
    {
        LayerItemLichSuMiniPoker* his = (LayerItemLichSuMiniPoker*)cell->getChildByTag(123);
        if (his){
            his->setDatas(it.phien,it.time,it.bet,it.result,it.win);
        }
    }
    return cell;
    
}

ssize_t LayerLichSuMiniPoker::numberOfCellsInTableView(TableView *table)
{
    
    return lstHis.size();
}
void LayerLichSuMiniPoker::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerLichSuMiniPoker::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(EXT_EVENT_PK_USER_HIS_RESP, cmd->c_str()) == 0){
        //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

        boost::shared_ptr<string> strhis = param->GetUtfString("ghs");
        if (strhis != NULL){
            //mrtuan2|1472722253|100|4_2;14_1;14_4;10_2;7_4|238|9#mrtuan2|1472722338|100|7_4;8_4;14_2;3_1;6_2|0|-1#mrtuan2|1472722353|100|14_3;5_3;9_3;11_1;7_3|0|-1#mrtuan2|1472722368|100|7_2;10_1;8_4;5_1;7_4|0|-1#mrtuan2|1472722382|100|2_3;7_2;4_4;12_3;3_3|0|-1#mrtuan2|1472722396|100|8_3;12_3;13_4;4_3;5_4|0|-1#mrtuan2|1472722410|100|14_1;5_3;9_4;6_3;2_4|0|-1#mrtuan2|1472722424|100|5_1;12_4;14_4;8_1;2_4|0|-1#mrtuan2|1472722438|100|5_3;5_1;11_2;9_2;7_4|0|-1#mrtuan2|1472722452|100|9_2;12_4;4_1;6_4;2_1|0|-1#mrtuan2|1472722466|100|2_2;14_1;2_1;13_4;4_2|0|-1#mrtuan2|1472722472|100|10_3;5_2;13_4;6_3;3_2|0|-1#mrtuan2|1472722479|100|7_3;8_1;9_1;12_3;14_3|0|-1#mrtuan2|1472722488|100|6_3;13_4;12_3;14_2;6_4|0|-1#mrtuan2|1472722504|100|11_2;14_1;2_4;9_2;11_3|238|9#mrtuan2|1472722518|100|2_3;14_3;5_3;2_1;7_2|0|-1#mrtuan2|1472722532|100|4_2;2_3;8_3;14_1;7_4|0|-1#mrtuan2|1472722546|100|4_4;6_4;7_2;14_4;5_1|0|-1#mrtuan2|1472722560|100|6_1;5_3;4_2;6_2;12_1|0|-1#mrtuan2|1472722573|100|3_4;8_4;14_3;5_1;6_2|0|-1#mrtuan2|1472722586|100|7_2;8_3"
            auto lstHiss = mUtils::splitString(*strhis, '#');
            this->lstHis.clear();
            for(int i = 0; i< lstHiss.size();i++){
                auto lstItem = mUtils::splitString(lstHiss[i], '|');
                if (lstItem.size() < 5)
                    return;
                item it;
                it.phien = StringUtils::format("#%s",lstItem[1].c_str());
                it.time = this->getTime(lstItem[1]);
                it.bet = mUtils::convertMoneyEx(atof(lstItem[2].c_str()));
                it.result = lstItem[3];
                it.win = mUtils::convertMoneyEx(atof(lstItem[4].c_str()));
                this->lstHis.push_back(it);
            }
            this->tblListHis->reloadData();
            SceneManager::getSingleton().hideLoading();
        }
        
    }
}
string LayerLichSuMiniPoker::getTime(string str){
    time_t tt = atoi(str.c_str());
    struct tm * ptm = localtime(&tt);
    char buf[30];
    strftime (buf, 30, "%d/%m/%Y %H:%M:%S",  ptm);
    return string(buf);
}
bool LayerLichSuMiniPoker::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerLichSuMiniPoker::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerLichSuMiniPoker::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerLichSuMiniPoker::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerItemLichSuMiniPoker::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemLichSuMiniPoker.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
        this->imgBg = static_cast<ImageView*>(root->getChildByName("imgBg"));
        this->txtPhien = static_cast<Text*>(this->imgBg->getChildByName("txtPhien"));
        this->txtBet = static_cast<Text*>(this->imgBg->getChildByName("txtBet"));
        this->txtGet = static_cast<Text*>(this->imgBg->getChildByName("txtGet"));
        this->txtTime = static_cast<Text*>(this->imgBg->getChildByName("txtTime"));
        this->img1 = static_cast<ImageView*>(this->imgBg->getChildByName("img1"));
        this->img2 = static_cast<ImageView*>(this->imgBg->getChildByName("img2"));
        this->img3 = static_cast<ImageView*>(this->imgBg->getChildByName("img3"));
        this->img4 = static_cast<ImageView*>(this->imgBg->getChildByName("img4"));
        this->img5 = static_cast<ImageView*>(this->imgBg->getChildByName("img5"));
        
    }
    else{
        this->imgBg = static_cast<ImageView*>(root->getChildByName("imgBg"));
        this->txtPhien = static_cast<Text*>(this->imgBg->getChildByName("txtPhien"));
        this->txtBet = static_cast<Text*>(this->imgBg->getChildByName("txtBet"));
        this->txtGet = static_cast<Text*>(this->imgBg->getChildByName("txtGet"));
        this->txtTime = static_cast<Text*>(this->imgBg->getChildByName("txtTime"));
        this->img1 = static_cast<ImageView*>(this->imgBg->getChildByName("img1"));
        this->img2 = static_cast<ImageView*>(this->imgBg->getChildByName("img2"));
        this->img3 = static_cast<ImageView*>(this->imgBg->getChildByName("img3"));
        this->img4 = static_cast<ImageView*>(this->imgBg->getChildByName("img4"));
        this->img5 = static_cast<ImageView*>(this->imgBg->getChildByName("img5"));
    }
    
    return true;
}
void LayerItemLichSuMiniPoker::setDatas(string phien,string time,string bet, string result,string win){
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
      
        this->txtPhien->setString(phien);
        this->txtTime->setString(time);
        this->txtBet->setString(bet);
        this->txtGet->setString(win);
        this->showResult(result);
    }
    else{
        this->txtPhien->setString(phien);
        this->txtTime->setString(time);
        this->txtBet->setString(bet);
        this->txtGet->setString(win);
        this->showResult(result);
        
    }
}
void LayerItemLichSuMiniPoker::showResult(string str){
    auto lstStrCard = mUtils::splitString(str,';');
    if (lstStrCard.size()<5)
        return;
    this->img1->loadTexture(StringUtils::format("%s.png",lstStrCard[0].c_str()));
    this->img2->loadTexture(StringUtils::format("%s.png",lstStrCard[1].c_str()));
    this->img3->loadTexture(StringUtils::format("%s.png",lstStrCard[2].c_str()));
    this->img4->loadTexture(StringUtils::format("%s.png",lstStrCard[3].c_str()));
    this->img5->loadTexture(StringUtils::format("%s.png",lstStrCard[4].c_str()));
    
}
