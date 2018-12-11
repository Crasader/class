//
//  LayerLichSuTaiXiuMini.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/13/16.
//
//

#include "LayerLichSuTaiXiuMini.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"

LayerLichSuTaiXiuMini::LayerLichSuTaiXiuMini()
{
    GameServer::getSingleton().addListeners(this);
}


LayerLichSuTaiXiuMini::~LayerLichSuTaiXiuMini()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerLichSuTaiXiuMini::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerLichSuTaiXiuMini.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
    pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
   // if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
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
    lblTitle->setString(dataManager.GetSysString(856));
    auto Image_20 = static_cast<ImageView*>(pnlBg->getChildByName("Image_20"));

    Image_20->loadTexture(StringUtils::format("design/LayerTaiXiuMini/%shead-ls.png",SceneManager::getSingleton().getFolderLang().c_str()));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerLichSuTaiXiuMini::onBtnClose, this));
    }
    
    auto pnltbl = static_cast<Layout*>(pnlBg->getChildByName("pnlTable"));

    
    //
    
    this->tblListHis = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height-10));
    this->tblListHis->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tblListHis->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblListHis->setPosition(Vec2(pnltbl->getPosition().x+8,pnltbl->getPosition().y+sizeAdd.height));
    this->tblListHis->setDelegate(this);
    this->tblListHis->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    this->addChild(this->tblListHis);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TX_USER_HIS_REQ, params));
    GameServer::getSingleton().Send(request);

    SceneManager::getSingleton().showLoading();
    
    this->setScale(1);
    
    return true;
}

void LayerLichSuTaiXiuMini::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerLichSuTaiXiuMini::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(1239, 75);
}

TableViewCell* LayerLichSuTaiXiuMini::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    item it = lstHistory.at(idx);
    if (!cell) {
        
        cell = new TableViewCell();
        cell->autorelease();
        
        LayerItemLichSuTXMini* his = LayerItemLichSuTXMini::create();

        his->setDatas(it.stt,it.phien,it.time,it.cuadat,it.ketqua,it.dat,it.tralai,it.nhan);
        his->setTag(123);
        cell->addChild(his);
        
    }
    else
    {
        LayerItemLichSuTXMini* his = (LayerItemLichSuTXMini*)cell->getChildByTag(123);
        if (his){
            his->setDatas(it.stt,it.phien,it.time,it.cuadat,it.ketqua,it.dat,it.tralai,it.nhan);
        }
    }
    return cell;

}

ssize_t LayerLichSuTaiXiuMini::numberOfCellsInTableView(TableView *table)
{
    
    return lstHistory.size();
}
void LayerLichSuTaiXiuMini::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerLichSuTaiXiuMini::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(EXT_EVENT_TX_USER_HIS_RESP, cmd->c_str()) == 0){
        boost::shared_ptr<string> strhis = param->GetUtfString("ghs");
        if (strhis != NULL){
            auto lstHisMatch = mUtils::splitString(*strhis, '#');
            this->lstHistory.clear();
            for(int i = 0; i< lstHisMatch.size();i++){
                 auto lstHis = mUtils::splitString(lstHisMatch[i], '@');
                for (int j = 0;j<lstHis.size();j++){
                auto hisItems = mUtils::splitString(lstHis[j], '|');
                item it;
                if(hisItems.size()<4)
                    continue;
                it.stt = j+1;
                it.phien = hisItems[0];
                it.time = this->getTime(hisItems[1]);
                auto hisBet = mUtils::splitString(hisItems[3], ';');
                if(hisBet.size()<5)
                    continue;
                if (hisBet[2] == "TAI")
                    it.cuadat = dataManager.GetSysString(671);
                else
                    it.cuadat = dataManager.GetSysString(709);
                it.dat = hisBet[3];
                it.tralai = hisBet[4];
                it.nhan = hisBet[5];
                it.ketqua = this->getResult(hisItems[2]);
                
                this->lstHistory.push_back(it);
                }
            }
            this->tblListHis->reloadData();
            SceneManager::getSingleton().hideLoading();
            //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

        }

    }
}
string LayerLichSuTaiXiuMini::getTime(string str){
    time_t tt = atoi(str.c_str());
    struct tm * ptm = localtime(&tt);
    char buf[30];
    strftime (buf, 30, "%d/%m/%Y %H:%M:%S",  ptm);
    return string(buf);
}
string LayerLichSuTaiXiuMini::getResult(string str){
    vector<string> lst = mUtils::splitString(str, '_');
    if(lst.size() < 3)
        return "";
    string kq = lst[0]+"-"+lst[1]+"-"+lst[2]+ " " +StringUtils::format("%d",atoi(lst[0].c_str())+atoi(lst[1].c_str())+atoi(lst[2].c_str()));
    return kq;
}

bool LayerLichSuTaiXiuMini::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerLichSuTaiXiuMini::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerLichSuTaiXiuMini::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerLichSuTaiXiuMini::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerItemLichSuTXMini::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemLichSuTaiXiuMini.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    
    imgBg = static_cast<ImageView*>(root->getChildByName("Image_3"));
    
    this->txtPhien = static_cast<Text*>(imgBg->getChildByName("txtPhien"));
    this->txtThoiGian = static_cast<Text*>(imgBg->getChildByName("txtThoiGian"));
    this->txtCuaDat = static_cast<Text*>(imgBg->getChildByName("txtCuaDat"));
    this->txtKetQua = static_cast<Text*>(imgBg->getChildByName("txtKetQua"));
    this->txtTienCuoc = static_cast<Text*>(imgBg->getChildByName("txtTienCuoc"));
    this->txtTienTraLai = static_cast<Text*>(imgBg->getChildByName("txtTienTraLai"));
    this->txtTienNhan = static_cast<Text*>(imgBg->getChildByName("txtTienNhan"));
    
    return true;
}
void LayerItemLichSuTXMini::setDatas(int stt,string phien,string time,string cuadat,string ketqua,string dat, string tralai,string nhan){
    this->txtPhien->setString(phien);
    this->txtThoiGian->setString(time);
    this->txtCuaDat->setString(cuadat);
    this->txtKetQua->setString(ketqua);
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
        this->txtTienCuoc->setString(formatMoneySymbol(dat));
        this->txtTienTraLai->setString(formatMoneySymbol(tralai));
        this->txtTienNhan->setString(formatMoneySymbol(nhan));

    }
    else{
//        imgBg->loadTexture("");
//        if (stt % 2 == 0){
//            imgBg->loadTexture("ls-bg-1.png");
//        }
        this->txtTienCuoc->setString(mUtils::convertMoneyEx(atof(dat.c_str())));
        this->txtTienTraLai->setString(mUtils::convertMoneyEx(atof(tralai.c_str())));
        this->txtTienNhan->setString(mUtils::convertMoneyEx(atof(nhan.c_str())));

    }
}

