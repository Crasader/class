//
//  LayerCaoThapItemLichSu.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 11/14/16.
//
//

#include "LayerCaoThapPopupLichSu.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"
#include "Requests/ExtensionRequest.h"

LayerCaoThapPopupLichSu::LayerCaoThapPopupLichSu()
{
    GameServer::getSingleton().addListeners(this);
}


LayerCaoThapPopupLichSu::~LayerCaoThapPopupLichSu()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerCaoThapPopupLichSu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupLichSuCaoThap.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
    pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
  //  if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    auto Image_17 = static_cast<ImageView*>(pnlBg->getChildByName("Image_17"));
    Image_17->loadTexture(StringUtils::format("design/LayerGameCaoThap/%shead-bxh.png",SceneManager::getSingleton().getFolderLang().c_str()));
    auto Text_1 = static_cast<Text*>(pnlBg->getChildByName("Text_1"));
    Text_1->setString(dataManager.GetSysString(856));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerCaoThapPopupLichSu::onBtnClose, this));
    }
    
    auto pnltbl = static_cast<ImageView*>(pnlBg->getChildByName("imgbgTable"));
    
    //
    
    this->tblListHis = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height-10));
    this->tblListHis->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tblListHis->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblListHis->setPosition(Vec2(pnltbl->getPosition().x+sizeAdd.width,pnltbl->getPosition().y+10));
    this->tblListHis->setDelegate(this);
    this->tblListHis->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    pnlBg->addChild(this->tblListHis);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_CAOTHAP_GET_HIS_PLAYER,params));
    GameServer::getSingleton().Send(request);
    
    
    SceneManager::getSingleton().showLoading();
    
    
    return true;
}

void LayerCaoThapPopupLichSu::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerCaoThapPopupLichSu::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(1239,70);
}

TableViewCell* LayerCaoThapPopupLichSu::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    item it = lstHis.at(idx);
    if (!cell) {
        
        cell = new TableViewCell();
        cell->autorelease();
        
        LayerCaoThapItemLichSu* his = LayerCaoThapItemLichSu::create();
        his->setDatas(it.phien,it.time,it.bet,it.buoc,it.cuadat,it.result,it.win);
        his->setTag(123);
        cell->addChild(his);
        
    }
    else
    {
        LayerCaoThapItemLichSu* his = (LayerCaoThapItemLichSu*)cell->getChildByTag(123);
        if (his){
            his->setDatas(it.phien,it.time,it.bet,it.buoc,it.cuadat,it.result,it.win);
        }
    }
    return cell;
    
}

ssize_t LayerCaoThapPopupLichSu::numberOfCellsInTableView(TableView *table)
{
    
    return lstHis.size();
}
void LayerCaoThapPopupLichSu::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerCaoThapPopupLichSu::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(EXT_EVENT_CAOTHAP_GET_HIS_PLAYER_RESP, cmd->c_str()) == 0){
        boost::shared_ptr<string> strhis = param->GetUtfString("ghs");
        //mrtuan1|1482466120|100|3_3;14_2;9_3;4_3|0|0@110@1070;1@110@110;0@231@165|0;0;0
        //hujkiikkkk|1505272500|1000|13_3;10_3;5_3;9_1|0|0@0@0@0;1@1000@10700@1100;1@1100@11770@1210;1@1210@3146@1573;1@1573@4089@2044;1@2044@2248@7154;1@0@0@0|0;0;0
        if (strhis != NULL){
            auto lstHiss = mUtils::splitString(*strhis, '#');
            //if ()
            this->lstHis.clear();
            for(int i = 0; i< lstHiss.size();i++){
                auto lstItem = mUtils::splitString(lstHiss[i], '|');
                if (lstItem.size() < 7)
                    return;
                auto lstNumbet = mUtils::splitString(lstItem[3], ';');
                auto lstMoneyBet = mUtils::splitString(lstItem[5], ';');
                if (lstNumbet.size() != lstMoneyBet.size())
                    continue;
                for (int j = lstNumbet.size()-1; j >=0;j--){
                    item it;
                    auto lstCuaDat = mUtils::splitString(lstMoneyBet[j], '@');
                    it.phien = StringUtils::format("#%s",lstItem[1].c_str());
                    it.time = this->getTime(lstItem[1]);
                    if (j == 0 ){
                        it.bet = mUtils::convertMoneyEx(atof(lstItem[2].c_str()));
                        it.buoc = StringUtils::format("%d",j+1);
                        it.cuadat = "";
                        it.result = lstNumbet[j];
                        it.win = "-"+mUtils::convertMoneyEx(atof(lstItem[2].c_str()));
                    }
                    else if(j == 1){
                        if (atoi(lstItem[4].c_str()) == 0){
                            if (atoi(lstCuaDat[0].c_str()) == 0){
                                it.bet = mUtils::convertMoneyEx(atof(lstCuaDat[1].c_str()));
                                it.buoc = StringUtils::format("%d",j+1);
                                it.cuadat = dataManager.GetSysString(684);
                                it.result = lstNumbet[j];
                                it.win = "0";
                            }
                            else{
                                
                                it.bet = mUtils::convertMoneyEx(atof(lstCuaDat[1].c_str()));
                                it.buoc = StringUtils::format("%d",j+1);
                                it.cuadat = dataManager.GetSysString(568);
                                it.result = lstNumbet[j];
                                it.win = "0";
                            }
                            
                        }else{
                            if (atoi(lstCuaDat[0].c_str()) == 0){
                                it.bet = mUtils::convertMoneyEx(atof(lstCuaDat[1].c_str()));
                                it.buoc = StringUtils::format("%d",j+1);
                                it.cuadat = dataManager.GetSysString(684);
                                it.result = lstNumbet[j];
                                it.win = mUtils::convertMoneyEx(atof(lstItem[4].c_str()));
                            }
                            else{
                                
                                it.bet = mUtils::convertMoneyEx(atof(lstCuaDat[1].c_str()));
                                it.buoc = StringUtils::format("%d",j+1);
                                it.cuadat = dataManager.GetSysString(568);
                                it.result = lstNumbet[j];
                                it.win = mUtils::convertMoneyEx(atof(lstItem[4].c_str()));
                            }
                            
                        }
                    }
                    else{
                        if (j == lstNumbet.size()-1){
                            if (atoi(lstItem[4].c_str()) == 0){
                                if (atoi(lstCuaDat[0].c_str()) == 0){
                                    it.bet = mUtils::convertMoneyEx(atof(lstCuaDat[1].c_str()));
                                    it.buoc = StringUtils::format("%d",j+1);
                                    it.cuadat = dataManager.GetSysString(684);
                                    it.result = lstNumbet[j];
                                    it.win = "0";
                                }
                                else{
                                    
                                    it.bet = mUtils::convertMoneyEx(atof(lstCuaDat[1].c_str()));
                                    it.buoc = StringUtils::format("%d",j+1);
                                    it.cuadat = dataManager.GetSysString(568);
                                    it.result = lstNumbet[j];
                                    it.win = "0";
                                }

                            }else{
                                if (atoi(lstCuaDat[0].c_str()) == 0){
                                    it.bet = mUtils::convertMoneyEx(atof(lstCuaDat[1].c_str()));
                                    it.buoc = StringUtils::format("%d",j+1);
                                    it.cuadat = dataManager.GetSysString(684);
                                    it.result = lstNumbet[j];
                                    it.win = mUtils::convertMoneyEx(atof(lstItem[4].c_str()));
                                }
                                else{
                                    
                                    it.bet = mUtils::convertMoneyEx(atof(lstCuaDat[1].c_str()));
                                    it.buoc = StringUtils::format("%d",j+1);
                                    it.cuadat = dataManager.GetSysString(568);
                                    it.result = lstNumbet[j];
                                    it.win = mUtils::convertMoneyEx(atof(lstItem[4].c_str()));
                                }

                            }
                            
                        }else{
                            if (atoi(lstCuaDat[0].c_str()) == 0){
                                it.bet = mUtils::convertMoneyEx(atof(lstCuaDat[1].c_str()));
                                it.buoc = StringUtils::format("%d",j+1);
                                it.cuadat = dataManager.GetSysString(684);
                                it.result = lstNumbet[j];
                                it.win = mUtils::convertMoneyEx(atof(lstCuaDat[2].c_str()));
                            }
                            else{
                                
                                it.bet = mUtils::convertMoneyEx(atof(lstCuaDat[1].c_str()));
                                it.buoc = StringUtils::format("%d",j+1);
                                it.cuadat = dataManager.GetSysString(568);
                                it.result = lstNumbet[j];
                                it.win = mUtils::convertMoneyEx(atof(lstCuaDat[3].c_str()));
                            }
                        }
                        
                    }
                    
                    this->lstHis.push_back(it);

                }
            }
            this->tblListHis->reloadData();
            SceneManager::getSingleton().hideLoading();
        }
        
    }
}
string LayerCaoThapPopupLichSu::getTime(string str){
    time_t tt = atoi(str.c_str());
    struct tm * ptm = localtime(&tt);
    char buf[30];
    strftime (buf, 30, "%d/%m/%Y %H:%M:%S",  ptm);
    return string(buf);
}
bool LayerCaoThapPopupLichSu::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerCaoThapPopupLichSu::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerCaoThapPopupLichSu::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerCaoThapPopupLichSu::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerCaoThapItemLichSu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemLichSuCaoThap.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    
 
        this->bg = static_cast<Layout*>(root->getChildByName("Panel_4"));
        this->bg->setSwallowTouches(false);
        this->txtPhien = static_cast<Text*>(this->bg->getChildByName("txtPhien"));
        this->txtBet = static_cast<Text*>(this->bg->getChildByName("txtBetMoney"));
        this->txtGet = static_cast<Text*>(this->bg->getChildByName("txtGetMoney"));
        this->txtTime = static_cast<Text*>(this->bg->getChildByName("txtTime"));
        this->imgResult = static_cast<ImageView*>(this->bg->getChildByName("imgQuanBai"));
        this->txtCuaDat = static_cast<Text*>(this->bg->getChildByName("txtCuaDat"));
        this->txtBuoc = static_cast<Text*>(this->bg->getChildByName("txtBuoc"));
        this->txtQuanBai = static_cast<Text*>(this->bg->getChildByName("txtQuanBai"));
        this->txtQuanBai->setFontSize(this->txtQuanBai->getFontSize()-7);

  
    return true;
}
void LayerCaoThapItemLichSu::setDatas(string phien,string time,string bet, string buoc,string cuadat,string result,string win){
        this->txtPhien->setString(phien);
        this->txtBet->setString(bet);
        this->txtGet->setString(win);
        this->txtTime->setString(time);
        auto lst = mUtils::splitString(result, '_');
        if (lst.size()<2)
            return;
        string pType = lst[0];
        switch (atoi(lst[0].c_str())) {
            case 11:
                pType = "J";
                break;
                
            case 12:
                pType = "Q";
                break;
            case 13:
                pType = "K";
                break;
            case 14:
                pType = "A";
                break;
        }
        string url = "";
        switch (atoi(lst[1].c_str())) {
            case 1:
                url = "moc3-chatco.png";
                break;
                
            case 2:
                url = "moc3-chatro.png";
                break;
            case 3:
                url = "moc3-chattep.png";
                break;
            case 4:
                url = "moc3-chatbich.png";
                break;
        }
        this->imgResult->loadTexture(url);
        this->txtCuaDat->setString(cuadat);
        this->txtQuanBai->setString(pType);
        this->txtBuoc->setString(buoc);
    
}
