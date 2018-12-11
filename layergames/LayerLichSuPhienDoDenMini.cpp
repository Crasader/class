//
//  LayerLichSuPhienDoDenMini.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/13/16.
//
//

#include "LayerLichSuPhienDoDenMini.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"

LayerLichSuPhienDoDenMini::LayerLichSuPhienDoDenMini()
{
    GameServer::getSingleton().addListeners(this);
}


LayerLichSuPhienDoDenMini::~LayerLichSuPhienDoDenMini()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerLichSuPhienDoDenMini::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerLichSuPhienDoDenMini.csb");
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
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    
    
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(848));
    auto lbltdh1 = static_cast<Text*> (pnlBg->getChildByName("txtx"));
    lbltdh1->setString(dataManager.GetSysString(849));
    auto lbltdh2 = static_cast<Text*> (pnlBg->getChildByName("txtTongXiu_0"));
    lbltdh2->setString(dataManager.GetSysString(849));
    auto Sprite_1 = static_cast<Sprite*>(pnlBg->getChildByName("Sprite_1"));
    Sprite_1->setTexture(StringUtils::format("design/LayerTaiXiuMini/%shead-tai-chitietphien.png",SceneManager::getSingleton().getFolderLang().c_str()));
    auto Sprite_1_0 = static_cast<Sprite*>(pnlBg->getChildByName("Sprite_1_0"));
    Sprite_1_0->setTexture(StringUtils::format("design/LayerTaiXiuMini/%shead-xiu-chitietphien.png",SceneManager::getSingleton().getFolderLang().c_str()));
    
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerLichSuPhienDoDenMini::onBtnClose, this));
    }
    
    this->btnPre = static_cast<Button*>(pnlBg->getChildByName("btnPre"));
    if(this->btnPre){
        this->btnPre->addClickEventListener(CC_CALLBACK_1(::LayerLichSuPhienDoDenMini::onBtnPrev, this));
        this->btnPre->setPressedActionEnabled(true);
    }
    
    this->btnNext = static_cast<Button*>(pnlBg->getChildByName("btnNext"));
    if(this->btnNext){
        this->btnNext->addClickEventListener(CC_CALLBACK_1(LayerLichSuPhienDoDenMini::onBtnNext, this));        this->btnNext->setPressedActionEnabled(true);
    }
    
    this->txtPhien = static_cast<Text*>(pnlBg->getChildByName("txtPhien"));
    this->txtPhien->setString("");
    
    this->txtNgay = static_cast<Text*>(pnlBg->getChildByName("txtNgay")) ;
    this->txtNgay->setString("");
    
    //this->imgResult = static_cast<ImageView*>(pnlBg->getChildByName("imgResult")) ;
    
    this->txtTotal= static_cast<Text*>(pnlBg->getChildByName("txtTotal")) ;
    this->txtTotal->setString("");
    
    
    this->imgDice1= static_cast<ImageView*>(pnlBg->getChildByName("imgDice1")) ;
    
    this->imgDice2= static_cast<ImageView*>(pnlBg->getChildByName("imgDice2")) ;
    
    this->imgDice3= static_cast<ImageView*>(pnlBg->getChildByName("imgDice3")) ;
    
//    this->txtMoney= static_cast<Text*>(pnlBg->getChildByName("txtMoney")) ;
//    this->txtMoney->setString("");
//    
//    
//    this->txtName= static_cast<Text*>(pnlBg->getChildByName("txtName")) ;
//    this->txtName->setString("");
    
    
    this->txtTongTai = static_cast<Text*>(pnlBg->getChildByName("txtTongTai"));
    this->txtTongTai->setString("");
    this->txtTongXiu = static_cast<Text*>(pnlBg->getChildByName("txtTongXiu")) ;
    this->txtTongXiu->setString("");
    
    this->txtTongHoanTai = static_cast<Text*>(pnlBg->getChildByName("txtHoanTai"));
    this->txtTongHoanTai->setString("");
    this->txtHoanXiu = static_cast<Text*>(pnlBg->getChildByName("txtHoanXiu"));
    this->txtHoanXiu->setString("");
    
    //auto imgTable = static_cast<ImageView*>(pnlBg->getChildByName("imgTable"));
    
    auto pnltblTai = static_cast<Layout*>(pnlBg->getChildByName("pnlTableTai"));
    this->imgCoin = static_cast<ImageView*>(pnlBg->getChildByName("imgCoin"));

    //
    auto pnlTableXiu = static_cast<Layout*>(pnlBg->getChildByName("pnlTableXiu"));
    this->tblListHisXiu = TableView::create(this, pnlTableXiu->getContentSize());
    this->tblListHisXiu->setAnchorPoint(pnlTableXiu->getAnchorPoint());
    this->tblListHisXiu->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblListHisXiu->setPosition(Vec2(pnlTableXiu->getPosition().x+sizeAdd.width,pnlTableXiu->getPosition().y+sizeAdd.height));
    this->tblListHisXiu->setDelegate(this);
    this->tblListHisXiu->setTag(2);
    this->tblListHisXiu->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    pnlBg->addChild(this->tblListHisXiu);
    

    
    this->tblListHisTai = TableView::create(this, pnltblTai->getContentSize());
    this->tblListHisTai->setAnchorPoint(pnltblTai->getAnchorPoint());
    this->tblListHisTai->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblListHisTai->setPosition(Vec2(pnltblTai->getPosition().x+sizeAdd.width,pnltblTai->getPosition().y+sizeAdd.height));
    this->tblListHisTai->setDelegate(this);
    this->tblListHisTai->setTag(1);
    this->tblListHisTai->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    pnlBg->addChild(this->tblListHisTai);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_XD_GAME_HIS_REQ, params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();

    this->setScale(1);
    return true;
}
void LayerLichSuPhienDoDenMini::reloadCoin(int type){
//    if (type == 0){
//        this->imgCoin->loadTexture("profile-coin.png");
//    }
//    else{
//        this->imgCoin->loadTexture("button_xu.png");
//    }
}
void LayerLichSuPhienDoDenMini::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerLichSuPhienDoDenMini::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(590, 70);
}

TableViewCell* LayerLichSuPhienDoDenMini::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (table->getTag() == 1){
        item it = this->lstHistoryMatchTai.at(idx);
        if (!cell) {
            
            cell = new TableViewCell();
            cell->autorelease();
            
            LayerItemLichSuPhienDoDenMiniDo* his = LayerItemLichSuPhienDoDenMiniDo::create();
            
            his->setDatas(it.stt,it.time,it.name,it.tai,it.xiu,it.hoan);
            his->setTag(123);
            cell->addChild(his);
            
        }
        else
        {
            LayerItemLichSuPhienDoDenMiniDo* his = (LayerItemLichSuPhienDoDenMiniDo*)cell->getChildByTag(123);
            if (his){
                his->setDatas(it.stt,it.time,it.name,it.tai,it.xiu,it.hoan);
            }
        }
        return cell;
    }else{
        item it = this->lstHistoryMatchXiu.at(idx);
        if (!cell) {
            
            cell = new TableViewCell();
            cell->autorelease();
            
            LayerItemLichSuPhienDoDenMiniDen* his = LayerItemLichSuPhienDoDenMiniDen::create();
            
            his->setDatas(it.stt,it.time,it.name,it.tai,it.xiu,it.hoan);
            his->setTag(123);
            cell->addChild(his);
            
        }
        else
        {
            LayerItemLichSuPhienDoDenMiniDen* his = (LayerItemLichSuPhienDoDenMiniDen*)cell->getChildByTag(123);
            if (his){
                his->setDatas(it.stt,it.time,it.name,it.tai,it.xiu,it.hoan);
            }
        }

    }
    return cell;

}

ssize_t LayerLichSuPhienDoDenMini::numberOfCellsInTableView(TableView *table)
{
    if (table->getTag() == 1)
        return this->lstHistoryMatchTai.size();
    else
        return this->lstHistoryMatchXiu.size();
}
void LayerLichSuPhienDoDenMini::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerLichSuPhienDoDenMini::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(EXT_EVENT_XD_GAME_HIS_DETAIL_RESP, cmd->c_str()) == 0){
        //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

        this->txtTongTai->setString("");
        this->txtTongXiu->setString("");
        this->txtTongHoanTai->setString("");
        this->txtHoanXiu->setString("");
        boost::shared_ptr<string> strhis = param->GetUtfString("ghs");
        if (strhis == NULL)
            return;
        auto lstHis = mUtils::splitString(*strhis,'|');
                if (strcmp(lstHis[3].c_str(),"null")==0)
        {
            SceneManager::getSingleton().hideLoading();
            return;

        }
        this->lstHistoryMatchTai.clear();
        this->showDice(lstHis[3]);
        int totalResult = this->getTotalPoint(lstHis[3]);
//        if (totalResult >=11){
//            this->imgResult->loadTexture("txt-tai.png");
//        }
//        else{
//            this->imgResult->loadTexture("xiu-txt.png");
//        }
        
        
        this->txtTotal->setString(StringUtils::format("%d",totalResult));

        if (lstHis.size() < 10){
            SceneManager::getSingleton().hideLoading();
            return;
        }
//        if (strcmp(lstHis[4].c_str(), "null") != 0){
//            this->txtName->setString(lstHis[4]);
//            this->txtMoney->setString(mUtils::convertMoneyEx(atof(lstHis[5].c_str())));
//        }
//        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
//            this->txtTongTai->setString(formatMoneySymbol(lstHis[7]));
//            this->txtTongXiu->setString(formatMoneySymbol(lstHis[8]));
//            this->txtTongHoanTai->setString(formatMoneySymbol(lstHis[9]));
//        }
//        else{

//        }
        
       
        
        
        auto lstBet = mUtils::splitString(lstHis[6],'@');
        int hoantai = 0;
        int hoanxiu = 0;
        int tongtai = 0;
        int tongxiu = 0;
        for (int i = 0; i< lstBet.size();i++){
            auto lstItem = mUtils::splitString(lstBet[i],';');
            item it ;
            if (lstItem.size() <6)
                continue;
            //killer88;1468318647;TAI;10000;0;1000
              it.stt = i+1;
            it.time = this->getTime(lstItem[1]);
            it.name = lstItem[0];
            if (lstItem[2] == "DO"){
                it.tai = lstItem[3];
                tongtai+=atoi(lstItem[3].c_str());
                
            }
            if (lstItem[2] == "DEN"){
                it.xiu = lstItem[3];
                tongxiu+=atoi(lstItem[3].c_str());

            }
            it.hoan = lstItem[5];
            if (atoi(it.tai.c_str())>0){
                this->lstHistoryMatchTai.push_back(it);
                hoantai+=atoi(it.hoan.c_str());
            }
            if (atoi(it.xiu.c_str())>0){
                this->lstHistoryMatchXiu.push_back(it);
                hoanxiu+=atoi(it.hoan.c_str());

            }

        }
        this->txtTongTai->setString(mUtils::convertMoneyEx(tongtai));
        this->txtTongXiu->setString(mUtils::convertMoneyEx(tongxiu));
        this->txtTongHoanTai->setString(mUtils::convertMoneyEx(hoantai));
        this->tblListHisTai->reloadData();
        this->tblListHisXiu->reloadData();
        this->txtHoanXiu->setString(mUtils::convertMoneyEx(hoanxiu));
        

        SceneManager::getSingleton().hideLoading();
    }
    else if(strcmp(EXT_EVENT_XD_GAME_HIS_RESP, cmd->c_str()) == 0){
        boost::shared_ptr<string> strhis = param->GetUtfString("ghs");
        if (strhis != NULL){

			
            
            auto lstrMatch = mUtils::splitString(*strhis, '|');
            this->lstMatch.clear();
            if (lstrMatch.size()-1 <=16){
                for (int i = 0; i < lstrMatch.size()-1; i++){
                    
                    auto lstItemMatch = mUtils::splitString(lstrMatch[i], '_');
                    match it;
                    if (lstItemMatch.size() < 2)
                        continue;
                    it.mid = lstItemMatch[0];
                    it.time = this->getDay(lstItemMatch[1]);
                    this->lstMatch.push_back(it);
                    
                }
            }else{
                for (int i = lstrMatch.size()-17; i < lstrMatch.size()-1; i++){
                    
                    auto lstItemMatch = mUtils::splitString(lstrMatch[i], '_');
                    match it;
                    if (lstItemMatch.size() < 2)
                        continue;
                    it.mid = lstItemMatch[0];
                    it.time = this->getDay(lstItemMatch[1]);
                    this->lstMatch.push_back(it);
                    
                }
            }
            
            SceneManager::getSingleton().hideLoading();
            this->loadHis();


        }
    }
}
string LayerLichSuPhienDoDenMini::getTime(string str){
    time_t tt = atoi(str.c_str());
    struct tm * ptm = localtime(&tt);
    char buf[30];
    strftime (buf, 30, "%H:%M:%S",  ptm);
    return string(buf);
}
string LayerLichSuPhienDoDenMini::GetUrlXiNgau(int number) {
    string num = "";
    switch (number){
        case 0:
            num = "den-show.png";
            break;
        case 1:
            num = "do-show.png";
            break;
    }
    return num;
}
int LayerLichSuPhienDoDenMini::getTotalPoint(string str){
    auto arrDice = mUtils::splitString(str,'_');
    if (arrDice.size()<3)
        return 0;
    return atoi(arrDice[0].c_str())+atoi(arrDice[1].c_str())+atoi(arrDice[2].c_str());
}
void LayerLichSuPhienDoDenMini::showDice(string str){
    auto arrDice = mUtils::splitString(str,'_');
    if (arrDice.size()<3)
        return;
    string url = this->GetUrlXiNgau(atoi(arrDice.at(0).c_str()));
    this->imgDice1->loadTexture(url);
    this->imgDice1->setVisible(true);
    
    url = this->GetUrlXiNgau(atoi(arrDice.at(1).c_str()));
    this->imgDice2->loadTexture(url);
    this->imgDice2->setVisible(true);
    
    url = this->GetUrlXiNgau(atoi(arrDice.at(2).c_str()));
    this->imgDice3->loadTexture(url);
    this->imgDice3->setVisible(true);
}
void LayerLichSuPhienDoDenMini::loadHis(){
    for (int i = 0; i< this->lstMatch.size();i++ ){
        if (strcmp(this->phienid.c_str(),this->lstMatch[i].mid.c_str())==0){
            this->currHisID = i;
            if (this->currHisID == this->lstMatch.size()-1)
                this->btnNext->setVisible(false);
            if (this->currHisID == 0)
                this->btnPre->setVisible(false);
            this->txtNgay->setString(this->lstMatch[i].time);
            this->txtPhien->setString(dataManager.GetSysString(645)+": #"+this->lstMatch[i].mid);
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutInt("mid", atoi(this->phienid.c_str()));
            boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_XD_GAME_HIS_DETAIL_REQ, params));
            GameServer::getSingleton().Send(request);
        }
        
    }
    SceneManager::getSingleton().showLoading();
}
string LayerLichSuPhienDoDenMini::getDay(string str){
    time_t tt = atoi(str.c_str());
    struct tm * ptm = localtime(&tt);
    char buf[30];
    strftime (buf, 30, "%d/%m/%Y",  ptm);
    return string(buf);
}
void LayerLichSuPhienDoDenMini::setCurrentItem(int tag){
    if(this->lstHistory.size()<=0)
        return;
    this->currItem = tag ;
    if (this->currItem>0 && this->currItem + 1 > this->lstHistory.size() && this->lstHistory.size()>=16)
        return;
    string currHis;
    if (this->lstHistory.size() <= 16)
        currHis = this->lstHistory[this->currItem-(16-this->lstHistory.size())];
    else
        currHis = this->lstHistory[this->currItem];
    
    auto arrHis = mUtils::splitString(currHis, '|');
    if (arrHis.size()<2)
        return;
    this->phienid = arrHis[0];
    auto arrDice = mUtils::splitString(arrHis[1], '-');
    if(arrDice.size()<3)
        return;
    this->showDice(arrDice[0]+"_"+arrDice[1]+"_"+arrDice[2]);
    
    int totalResult = atoi(arrDice[0].c_str()) +atoi(arrDice[1].c_str())+atoi(arrDice[2].c_str());
    //    if (totalResult >=11){
    //        this->imgResult->loadTexture("txt-tai.png");
    //    }
    //    else{
    //        this->imgResult->loadTexture("xiu-txt.png");
    //    }
    this->txtTotal->setString(StringUtils::format("%d",totalResult));
}
void LayerLichSuPhienDoDenMini::setLstHistory(vector<string> list){
    this->lstHistory.clear();
    for(int i = 0; i<list.size();i++){
        
        this->lstHistory.push_back(list[i]);
        
    }
}
bool LayerLichSuPhienDoDenMini::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerLichSuPhienDoDenMini::onBtnPrev(Ref* pSender){
    this->lstHistoryMatchTai.clear();
    this->lstHistoryMatchXiu.clear();

    this->tblListHisTai->reloadData();
    this->tblListHisXiu->reloadData();
    this->btnNext->setVisible(true);
    this->btnPre->setVisible(true);
    
    this->txtTongTai->setString("");
    this->txtTongXiu->setString("");
    this->txtTongHoanTai->setString("");
//    this->txtName->setString("");
//    this->txtMoney->setString("");
    
    if (this->currHisID == 0 || this->currHisID - 1 > this->lstMatch.size()){
        this->setCurrentItem(this->currItem);
        this->phienid = this->lstMatch[this->currHisID].mid;
        this->loadHis();
        this->btnPre->setVisible(false);
        return;
        
    }
    this->currHisID-=1;
    this->currItem-=1;
    this->setCurrentItem(this->currItem);
    this->phienid = this->lstMatch[this->currHisID].mid;
    this->loadHis();
    if (this->currHisID == 0)
        this->btnPre->setVisible(false);
}

void LayerLichSuPhienDoDenMini::onBtnNext(Ref* pSender){
    this->lstHistoryMatchTai.clear();
    this->tblListHisTai->reloadData();
    this->lstHistoryMatchXiu.clear();
    this->tblListHisXiu->reloadData();
    this->btnNext->setVisible(true);
    this->btnPre->setVisible(true);
    
    this->txtTongTai->setString("");
    this->txtTongXiu->setString("");
    this->txtTongHoanTai->setString("");
//    this->txtName->setString("");
//    this->txtMoney->setString("");
    
    this->currHisID+=1;
    this->currItem+=1;
    if (this->currHisID - 1 > this->lstMatch.size())
        return;
    this->setCurrentItem(this->currItem);
    this->phienid = this->lstMatch[this->currHisID].mid;
    this->loadHis();
    if (this->currHisID == this->lstMatch.size()-1)
        this->btnNext->setVisible(false);
}
void LayerLichSuPhienDoDenMini::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerLichSuPhienDoDenMini::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
   //     GameServer::getSingleton().addListeners(this);
}

void LayerLichSuPhienDoDenMini::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
   //     GameServer::getSingleton().removeListeners(this);
//    Layer::onExit();
}
bool LayerItemLichSuPhienDoDenMiniDo::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemLichSuPhienDo.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    this->imgBG = static_cast<ImageView*>(root->getChildByName("imgBG"));
    this->txtTime =  static_cast<Text*>(this->imgBG->getChildByName("txtTime"));
    this->txtTen =  static_cast<Text*>(this->imgBG->getChildByName("txtTen"));
    this->txtTai =  static_cast<Text*>(this->imgBG->getChildByName("txtDat"));
    this->txtHoan =  static_cast<Text*>(this->imgBG->getChildByName("txtHoan"));

    
    return true;
}
void LayerItemLichSuPhienDoDenMiniDo::setDatas(int stt,string time,string name,string tai,string xiu,string hoan){
    this->txtTime->setString(time);
    std::string _strNameSmall = "";
    (name.length() > 10) ? _strNameSmall = name.substr(0, 10) + "..." : _strNameSmall =name;
    this->txtTen->setString(_strNameSmall);
    this->txtTai->setString(mUtils::convertMoneyEx(atof(tai.c_str())));
    this->txtHoan->setString(mUtils::convertMoneyEx(atof(hoan.c_str())));
 
}
bool LayerItemLichSuPhienDoDenMiniDen::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemLichSuPhienDen.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    this->imgBG = static_cast<ImageView*>(root->getChildByName("imgBG"));
    this->txtTime =  static_cast<Text*>(this->imgBG->getChildByName("txtTime"));
    this->txtTen =  static_cast<Text*>(this->imgBG->getChildByName("txtTen"));
    this->txtXiu =  static_cast<Text*>(this->imgBG->getChildByName("txtDat"));

    this->txtHoan =  static_cast<Text*>(this->imgBG->getChildByName("txtHoan"));
    
    
    return true;
}
void LayerItemLichSuPhienDoDenMiniDen::setDatas(int stt,string time,string name,string tai,string xiu,string hoan){

        this->txtTime->setString(time);
        std::string _strNameSmall = "";
    	(name.length() > 10) ? _strNameSmall = name.substr(0, 10) + "..." : _strNameSmall =name;
        this->txtTen->setString(_strNameSmall);
        this->txtXiu->setString(mUtils::convertMoneyEx(atof(xiu.c_str())));
        this->txtHoan->setString(mUtils::convertMoneyEx(atof(hoan.c_str())));
    
}
