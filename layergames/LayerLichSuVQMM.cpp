//
//  LayerLichSuVQMM.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/13/16.
//
//

#include "LayerLichSuVQMM.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"
#include "Requests/ExtensionRequest.h"

LayerLichSuVQMM::LayerLichSuVQMM()
{
    GameServer::getSingleton().addListeners(this);
}


LayerLichSuVQMM::~LayerLichSuVQMM()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerLichSuVQMM::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerLichSuVQMM.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
  
    
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
    auto Image_20 = static_cast<ImageView*>(pnlBg->getChildByName("Image_20"));
    Image_20->loadTexture(StringUtils::format("design/VQMMMini/%shead-ls.png",SceneManager::getSingleton().getFolderLang().c_str()));
    auto Text_1 = static_cast<Text*>(pnlBg->getChildByName("Text_1"));
    Text_1->setString(dataManager.GetSysString(856));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerLichSuVQMM::onBtnClose, this));
    }
    this->btnMoc = dynamic_cast<Button*>(pnlBg->getChildByName("btnMoc"));
    
    if(this->btnMoc){
        this->btnMoc->addClickEventListener(CC_CALLBACK_1(LayerLichSuVQMM::onBtnMoc, this));
        this->btnMoc->setTitleText(SceneManager::getSingleton().getCoin(0));
    }
    this->btnHao = dynamic_cast<Button*>(pnlBg->getChildByName("btnHao"));
    
    if(this->btnHao){
        this->btnHao->addClickEventListener(CC_CALLBACK_1(LayerLichSuVQMM::onBtnHao, this));
        this->btnHao->setTitleText(SceneManager::getSingleton().getCoin(1));

    }
    
    auto pnltbl = static_cast<ImageView*>(pnlBg->getChildByName("pnlTable"));
    
    //
    
    this->tblListLichSu = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height));
    this->tblListLichSu->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tblListLichSu->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblListLichSu->setPosition(Vec2(pnltbl->getPosition().x+10,pnltbl->getPosition().y+sizeAdd.height));
    this->tblListLichSu->setDelegate(this);
    this->tblListLichSu->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    this->addChild(this->tblListLichSu);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_VQ_USER_HIS_REQ, params));
    GameServer::getSingleton().Send(request);
    
    
    SceneManager::getSingleton().showLoading();
    this->setScale(1);
    
    return true;
}

void LayerLichSuVQMM::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerLichSuVQMM::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(1239, 80);
}

TableViewCell* LayerLichSuVQMM::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    if (this->currentTab == 1){
        item it = lstLichSuMoc.at(idx);
        if (!cell) {
            
            cell = new TableViewCell();
            cell->autorelease();
            
            LayerItemLichSuVQMM* his = LayerItemLichSuVQMM::create();
            
            his->setDatas(it.stt,it.name,it.money);
            his->setTag(123);
            cell->addChild(his);
            
        }
        else
        {
            LayerItemLichSuVQMM* his = (LayerItemLichSuVQMM*)cell->getChildByTag(123);
            if (his){
                his->setDatas(it.stt,it.name,it.money);
            }
        }
        
    }else{
        item it = lstLichSuHao.at(idx);
        if (!cell) {
            
            cell = new TableViewCell();
            cell->autorelease();
            
            LayerItemLichSuVQMM* his = LayerItemLichSuVQMM::create();
            
            his->setDatas(it.stt,it.name,it.money);
            his->setTag(123);
            cell->addChild(his);
            
        }
        else
        {
            LayerItemLichSuVQMM* his = (LayerItemLichSuVQMM*)cell->getChildByTag(123);
            if (his){
                his->setDatas(it.stt,it.name,it.money);
            }
        }
        
    }
    return cell;
    
}

ssize_t LayerLichSuVQMM::numberOfCellsInTableView(TableView *table)
{
    if (this->currentTab == 1)
        return lstLichSuMoc.size();
    else
        return lstLichSuHao.size();
    
}
void LayerLichSuVQMM::onBtnMoc(Ref* pSender){
    this->currentTab = 1;
    this->tblListLichSu->reloadData();
    this->btnMoc->setEnabled(false);
    this->btnMoc->setBright(false);
    
    this->btnHao->setEnabled(true);
    this->btnHao->setBright(true);
}
void LayerLichSuVQMM::onBtnHao(Ref* pSender){
    this->currentTab = 2;
    this->tblListLichSu->reloadData();
    
    this->btnMoc->setEnabled(true);
    this->btnMoc->setBright(true);
    
    this->btnHao->setEnabled(false);
    this->btnHao->setBright(false);
}
void LayerLichSuVQMM::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerLichSuVQMM::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(EXT_EVENT_VQ_USER_HIS_RESP, cmd->c_str()) == 0){
        //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
        SceneManager::getSingleton().hideLoading();
        boost::shared_ptr<string> strhis = param->GetUtfString("ghs");
        if (strhis != NULL){
            auto lstHis = mUtils::splitString(*strhis, '|');
            this->lstLichSuMoc.clear();
            this->lstLichSuHao.clear();
            
            for(int i = 0; i< lstHis.size();i++){
                auto hisItems = mUtils::splitString(lstHis[i], ';');
                    if (hisItems.size() < 7)
                        continue;
                    int sovong = hisItems.size()-6;
                    item it ;
                    if (atoi(hisItems[2].c_str())>0){
                        it.stt = this->lstLichSuMoc.size()+1;
                        it.name = StringUtils::format("%d",sovong);
                        it.money = hisItems[2];
                        this->lstLichSuMoc.push_back(it);
                        
                    }
                    if (atoi(hisItems[3].c_str())>0){
                        it.stt = this->lstLichSuHao.size()+1;
                        it.name = StringUtils::format("%d",sovong);
                        it.money = hisItems[3];
                        this->lstLichSuHao.push_back(it);
                        
                    }
            }
            this->onBtnMoc(NULL);
            SceneManager::getSingleton().hideLoading();
        }
        
    }
}
string LayerLichSuVQMM::getTime(string str){
    time_t tt = atoi(str.c_str());
    struct tm * ptm = localtime(&tt);
    char buf[30];
    strftime (buf, 30, "%d/%m/%Y %H:%M:%S",  ptm);
    return string(buf);
}
string LayerLichSuVQMM::getResult(string str){
    vector<string> lst = mUtils::splitString(str, '_');
    if(lst.size() < 3)
        return "";
    string kq = lst[0]+"-"+lst[1]+"-"+lst[2]+ " " +StringUtils::format("%d",atoi(lst[0].c_str())+atoi(lst[1].c_str())+atoi(lst[2].c_str()));
    return kq;
}

bool LayerLichSuVQMM::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerLichSuVQMM::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerLichSuVQMM::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerLichSuVQMM::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerItemLichSuVQMM::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemLichSuVQMM.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    
    
    this->imgbg = static_cast<ImageView*>(root->getChildByName("imgBg"));
    this->txtName = static_cast<Text*>(this->imgbg->getChildByName("txtName"));
    this->txtMoney = static_cast<Text*>(this->imgbg->getChildByName("txtMoney"));
    this->txtRank = static_cast<Text*>(this->imgbg->getChildByName("txtRank"));
    
    
    return true;
}
void LayerItemLichSuVQMM::setDatas(int stt,string name,string money){
    this->txtRank->setString(StringUtils::format("%d",stt));
    this->txtName->setString(ChanUtils::GetNamePlayer(name));
    this->txtMoney->setString(mUtils::convertMoneyEx(atof(money.c_str())));
    
}
