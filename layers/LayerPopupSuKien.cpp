//
//  LayerPopupSuKien.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/25/17.
//
//

#include "LayerPopupSuKien.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupThongBao.h"
#include "Requests/LogoutRequest.h"
#include "../data_manager.h"
#include "../layergames/_Chat_.h"
LayerPopupSuKien::LayerPopupSuKien()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupSuKien::~LayerPopupSuKien()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupSuKien::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupSuKien.csb");
    ui::Helper::doLayout(rootNode);

    this->addChild(rootNode);
    
    auto pnltbl = static_cast<Layout*>(rootNode->getChildByName("pnlTable"));
    this->Image_1 = static_cast<ImageView*>(rootNode->getChildByName("Image_1"));
    this->Image_1->setVisible(false);
    Size sizeAdd;
//    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);

    //
    pnltbl->setVisible(false);
    this->tableSpecial = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height));
    this->tableSpecial->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tableSpecial->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tableSpecial->setPosition(Vec2(pnltbl->getPosition().x+sizeAdd.width,pnltbl->getPosition().y));
    this->tableSpecial->setDelegate(this);
    this->tableSpecial->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    this->addChild(this->tableSpecial);
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_EVENT_MISSION_REQ, params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();

    return true;
}
void LayerPopupSuKien::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerPopupSuKien::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(512,100);
}

TableViewCell* LayerPopupSuKien::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        auto item = LayerItemSuKien::create();
        //item->setAnchorPoint(Vec2(0.5, 0.5));
        //item->setPosition(Vec2(cell->getContentSize().width / 2 + 20, cell->getContentSize().height / 2 + 20));
        item->setDatas(this->sukien[idx].id, this->sukien[idx].name, this->sukien[idx].type, this->sukien[idx].url);
        item->setTag(15);
        cell->addChild(item);
        cell->setIdx(idx);
    }
    else
    {
        LayerItemSuKien* item = (LayerItemSuKien*)cell->getChildByTag(15);
        if (item){
            item->setDatas(this->sukien[idx].id, this->sukien[idx].name, this->sukien[idx].type, this->sukien[idx].url);
        }
    }
    return cell;
}
void LayerPopupSuKien::loadData(string data){
    //1|Đăng nhập giờ vàng|0|18/06/2017 00:00:00|18/06/2017 23:00:00|gamebai.net|0;2|Chơi game nhận thưởng|0|18/06/2017 00:00:00|19/06/2017 20:00:00|gamebai.net|0;3|Khuyến mãi nạp thẻ lần đầu|1|18/06/2017 00:00:00|18/06/2017 00:00:00|gamebai.net|0;4|Khuyến mãi nạp thẻ giờ vàng|1|18/06/2017 00:00:00|18/06/2017 00:00:00|gamebai.net|0;
    
    auto lstauto = mUtils::splitString(data,';');
    this->sukien.clear();
    for (int i = 0; i< lstauto.size(); i++) {
        auto lstitem = mUtils::splitString(lstauto[i],'|');
        if (lstitem.size()<7) {
            continue;
        }
        if (atoi(lstitem[6].c_str()) == 1 ) {
            SuKien it;
            it.id = lstitem[0];
            it.name = lstitem[1];
            it.type = lstitem[2];
            it.url = lstitem[5];
            this->sukien.push_back(it);
        }
        
        
    }
    this->tableSpecial->reloadData();

}
ssize_t LayerPopupSuKien::numberOfCellsInTableView(TableView *table)
{
    
    return this->sukien.size();//lstRank.size();
}
void LayerPopupSuKien::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    if (strcmp(EXT_EVENT_EVENT_MISSION_RESP, cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        if (param->GetInt("rc") == NULL || param->GetInt(AWARD_SILVER) == NULL || param->GetInt(AWARD_GOLD)== NULL || param->GetInt(ACC_EXP)== NULL) return;
        int rc = *param->GetInt("rc");
        if (rc == 0){
            int silver = *param->GetInt(AWARD_SILVER);
            int gold = *param->GetInt(AWARD_GOLD);
            int exp = *param->GetInt(ACC_EXP);

            string content = "Bạn nhận được ";
            
            if (silver > 0)
                content += StringUtils::format("%s %s ",mUtils::convertMoneyEx(silver).c_str(),SceneManager::getSingleton().getCoin(1).c_str());
            if (gold > 0)
                content += StringUtils::format("%s %s ",mUtils::convertMoneyEx(gold).c_str(),SceneManager::getSingleton().getCoin(0).c_str());
            if (exp > 0)
                content += StringUtils::format("%s EXP ",mUtils::convertMoneyEx(exp).c_str());
            
            content += "!";
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
           
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(content);
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,10000000,9073);

            
        }else{
            if (param->GetUtfString("rd") == NULL) return;
            string rd = *param->GetUtfString("rd");
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(rd);
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,10000000,9074);
        }
    }
    else if(strcmp(EXT_EVENT_GET_EVENT_MISSION_RESP, cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        this->Image_1->setVisible(true);

        if (!param->GetUtfString(EVENT_MISSION_RESULT_LIST)) return;
        string strGioVang = *param->GetUtfString(EVENT_MISSION_RESULT_LIST);
        
       // log("giờ vang: %s",strGioVang.c_str());
        this->loadData(strGioVang);
    }
  
}

bool LayerPopupSuKien::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    auto locationInNode = pTouch->getLocation();
    //var s = target.getContentSize();
    auto rectNode = this->Image_1->getBoundingBox();//Rect(this->menu->getPosition().x, this->menu->getPosition().y, 143,149);
    Size sizeAdd;
    sizeAdd = ChanUtils::getSizePos();
    
    auto rect = Rect(rectNode.origin.x + sizeAdd.width, rectNode.origin.y + sizeAdd.height, rectNode.size.width, rectNode.size.height+80);
    //listener->setSwallowTouches(false);
    
    if (rect.containsPoint(locationInNode)) {
       /// auto rect2 = Rect(rectNode.origin.x + sizeAdd.width, rectNode.origin.y+rectNode.size.height + sizeAdd.height, rectNode.size.width, 70);
        //listener->setSwallowTouches(false);
        //if (rect2.containsPoint(locationInNode))
            //this->removeFromParent();
            return true;
    }
    else{
        this->removeFromParent();
    }
   
    return false;
}

void LayerPopupSuKien::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupSuKien::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupSuKien::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerItemSuKien::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemSuKien.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    
    this->txtName = static_cast<Text*>(root->getChildByName("txtName"));
    this->btnGet = static_cast<Button*>(root->getChildByName("btnNhanThuong"));
    this->huongDan = static_cast<Button*>(root->getChildByName("btnHuongDan"));
    
    if (this->huongDan){
        this->huongDan->addClickEventListener(CC_CALLBACK_1(LayerItemSuKien::onBtnHuongDan, this));
        this->huongDan->setPressedActionEnabled(true);
    }
    
    if (this->btnGet){
        this->btnGet->addClickEventListener(CC_CALLBACK_1(LayerItemSuKien::onBtnNhanThuong, this));
        this->btnGet->setPressedActionEnabled(true);
    }
    
    return true;
}
void LayerItemSuKien::onBtnHuongDan(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    string strlink = this->urlHuongDan;
        LayerPopupWebView* _layer = LayerPopupWebView::create();
    _layer->setWebView(strlink);
    _currScene->addChild(_layer, 100000,9075);
}
void LayerItemSuKien::setDatas(string id, string name, string type,string url){
    this->btnGet->setVisible(false);
    this->txtName->setString(name);
    this->urlHuongDan = url;
    this->btnGet->setTag(atoi(id.c_str()));
    if (atoi(type.c_str())==0) {
        this->btnGet->setVisible(true);

    }
}
void LayerItemSuKien::onBtnNhanThuong(Ref* pSender){
    
    auto node = (Node*)(pSender);
    int tag = node->getTag();
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    //EVENT_MISSION_ID = "evmid";
    params->PutInt("evmid", tag);
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_EVENT_MISSION_REQ,params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}
