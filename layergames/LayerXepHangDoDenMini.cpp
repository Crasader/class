//
//  LayerXepHangDoDenMini.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/13/16.
//
//

#include "LayerXepHangDoDenMini.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"

LayerXepHangDoDenMini::LayerXepHangDoDenMini()
{
    GameServer::getSingleton().addListeners(this);
}


LayerXepHangDoDenMini::~LayerXepHangDoDenMini()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerXepHangDoDenMini::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerXepHangDoDenMini.csb");
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
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(850));
    
    auto Image_20 = static_cast<ImageView*>(pnlBg->getChildByName("Image_20"));
    Image_20->loadTexture(StringUtils::format("design/LayerDoDenMini/%shead-bxh.png",SceneManager::getSingleton().getFolderLang().c_str()));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerXepHangDoDenMini::onBtnClose, this));
    }
    
    auto pnltbl = static_cast<ImageView*>(pnlBg->getChildByName("pnlTable"));
    
    //
    
    this->tblListRank = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height-10));
    this->tblListRank->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tblListRank->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblListRank->setPosition(Vec2(pnltbl->getPosition().x+sizeAdd.width+7,pnltbl->getPosition().y+sizeAdd.height));
    this->tblListRank->setDelegate(this);
    this->tblListRank->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    this->addChild(this->tblListRank);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_XD_RANK_REQ, params));
    GameServer::getSingleton().Send(request);
    
    
    SceneManager::getSingleton().showLoading();
    
    this->setScale(1);
    return true;
}

void LayerXepHangDoDenMini::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerXepHangDoDenMini::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(1239, 70);
}

TableViewCell* LayerXepHangDoDenMini::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    item it = lstRank.at(idx);
    if (!cell) {
        
        cell = new TableViewCell();
        cell->autorelease();
        
        LayerItemRankDoDenMini* his = LayerItemRankDoDenMini::create();
        
        his->setDatas(it.stt,it.name,it.money);
        his->setTag(123);
        cell->addChild(his);
        
    }
    else
    {
        LayerItemRankDoDenMini* his = (LayerItemRankDoDenMini*)cell->getChildByTag(123);
        if (his){
            his->setDatas(it.stt,it.name,it.money);
        }
    }
    return cell;
    
}

ssize_t LayerXepHangDoDenMini::numberOfCellsInTableView(TableView *table)
{
    
    return lstRank.size();
}
void LayerXepHangDoDenMini::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerXepHangDoDenMini::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(EXT_EVENT_XD_RANK_RESP, cmd->c_str()) == 0){
        boost::shared_ptr<string> strhis = param->GetUtfString("ghtp");
        //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

        if (strhis != NULL){
            auto lstHis = mUtils::splitString(*strhis, '@');
            this->lstRank.clear();
            for(int i = 0; i< lstHis.size();i++){
                auto hisItems = mUtils::splitString(lstHis[i], '|');
                this->lstRank.clear();
                for (int i = 0; i < hisItems.size();i++){
                    auto lstItem = mUtils::splitString(hisItems[i], ':');
                    if (lstItem.size() < 3)
                        return;
                    item it ;
                    it.stt = i+1;
                    it.name = lstItem[0];
                    it.money = lstItem[2];
                    this->lstRank.push_back(it);
                }
            }
            this->tblListRank->reloadData();
            SceneManager::getSingleton().hideLoading();
        }
        
    }
}
string LayerXepHangDoDenMini::getTime(string str){
    time_t tt = atoi(str.c_str());
    struct tm * ptm = localtime(&tt);
    char buf[30];
    strftime (buf, 30, "%d/%m/%Y %H:%M:%S",  ptm);
    return string(buf);
}
string LayerXepHangDoDenMini::getResult(string str){
    vector<string> lst = mUtils::splitString(str, '_');
    if(lst.size() < 3)
        return "";
    string kq = lst[0]+"-"+lst[1]+"-"+lst[2]+ " " +StringUtils::format("%d",atoi(lst[0].c_str())+atoi(lst[1].c_str())+atoi(lst[2].c_str()));
    return kq;
}

bool LayerXepHangDoDenMini::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerXepHangDoDenMini::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerXepHangDoDenMini::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerXepHangDoDenMini::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerItemRankDoDenMini::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemXepHangDoDenMini.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
        this->imgbg = static_cast<ImageView*>(root->getChildByName("imgBg"));
        this->txtRank = static_cast<Text*>(imgbg->getChildByName("txtRank"));
        this->txtName = static_cast<Text*>(imgbg->getChildByName("txtName"));
        this->txtMoney = static_cast<Text*>(imgbg->getChildByName("txtMoney"));
        this->imgRank = static_cast<ImageView*>(imgbg->getChildByName("imgRank"));
        this->imgLine = static_cast<ImageView*>(imgbg->getChildByName("imgLine"));

    }
    else{
        this->imgbg = static_cast<ImageView*>(root->getChildByName("imgBg"));
        this->imgRank = static_cast<ImageView*>(this->imgbg->getChildByName("imgRank"));
        this->imgAvatar = static_cast<ImageView*>(this->imgbg->getChildByName("imgAvatar"));
        this->txtName = static_cast<Text*>(this->imgbg->getChildByName("txtName"));
        this->txtMoney = static_cast<Text*>(this->imgbg->getChildByName("txtMoney"));
        this->txtRank = static_cast<Text*>(this->imgbg->getChildByName("txtRank"));
    }
   
    return true;
}
void LayerItemRankDoDenMini::setDatas(int stt,string name,string money){
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
        this->imgRank->setVisible(false);
        this->imgbg->loadTexture("bg-xephang.png");
        this->imgLine->setVisible(true);
        if (stt == 1){
            this->imgRank->setVisible(true);
            this->imgbg->loadTexture("bg-xephang-top1.png");
            this->imgRank->loadTexture("1-award.png");
            this->imgLine->setVisible(false);

        }else if (stt == 2){
            this->imgRank->setVisible(true);
            this->imgbg->loadTexture("bg-xephang-top2.png");
            this->imgRank->loadTexture("2-award.png");
            this->imgLine->setVisible(false);

        }else if (stt == 3){
            this->imgRank->setVisible(true);
            this->imgbg->loadTexture("bg-xephang-top3.png");
            this->imgRank->loadTexture("3-award.png");
            this->imgLine->setVisible(false);

        }

        this->txtRank->setString(StringUtils::format("%d",stt));
        this->txtName->setString(name);
        this->txtMoney->setString(formatMoneySymbol(money));
        
    }
    else{
        //this->imgbg->loadTexture("4-5-vv-bg.png");
        //this->imgRank->loadTexture("4-5-vv-award.png");
        //this->imgRank->setContentSize(Size(59,59));
        this->txtRank->setVisible(true);
        if (stt == 1){
//            this->imgbg->loadTexture("1-bg.png");
//            this->txtRank->setVisible(false);
//            this->imgRank->loadTexture("1-award.png");
//            this->imgRank->setContentSize(Size(85,75));
        }
        if (stt == 2){
//            this->imgbg->loadTexture("2-3-bg.png");
//            this->txtRank->setVisible(false);
//            this->imgRank->loadTexture("2-award.png");
//            this->imgRank->setContentSize(Size(61,75));

        }
        if (stt == 3){
//            this->imgbg->loadTexture("2-3-bg.png");
//            this->txtRank->setVisible(false);
//            this->imgRank->loadTexture("3-award.png");
//            this->imgRank->setContentSize(Size(61,75));

        }
        this->imgbg->loadTexture("xh-1.png");
        if (stt % 2 == 0){
             this->imgbg->loadTexture("xh-2.png");
        }
        this->txtRank->setString(StringUtils::format("%d",stt));
        this->txtName->setString(ChanUtils::GetNamePlayer(name));
        this->txtMoney->setString(mUtils::convertMoneyEx(atof(money.c_str())));
    }
   
}
