//
//  LayerOffer.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/27/16.
//
//

#include "LayerOffer.h"
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
LayerOffer::LayerOffer()
{
    GameServer::getSingleton().addListeners(this);
}


LayerOffer::~LayerOffer()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerOffer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerOffer.csb");
    
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    
    auto pnltbl = static_cast<Layout*>(rootNode->getChildByName("pnlTable"));
    
    //
    pnltbl->setVisible(false);
    this->tableSpecial = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height));
    this->tableSpecial->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tableSpecial->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tableSpecial->setPosition(Vec2(pnltbl->getPosition().x,pnltbl->getPosition().y));
    this->tableSpecial->setDelegate(this);
    this->tableSpecial->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    this->addChild(this->tableSpecial);


    this->txtNoGift = static_cast<Text*>(rootNode->getChildByName("txtNoGift"));
    this->txtNoGift->setVisible(false);
    this->txtNoGift->setString(dataManager.GetSysString(961));
    return true;
}
void LayerOffer::hideLayer(){
    this->setVisible(false);
}
void LayerOffer::showLayer(){
    this->setVisible(true);
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("eeglsorq",params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}
void LayerOffer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerOffer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(1435,150);
}

TableViewCell* LayerOffer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        auto item = LayerItemOffer::create();
        //item->setAnchorPoint(Vec2(0.5, 0.5));
        //item->setPosition(Vec2(cell->getContentSize().width / 2 + 20, cell->getContentSize().height / 2 + 20));
        item->setDatas(this->lstOffer[idx].id, this->lstOffer[idx].trangthai, this->lstOffer[idx].thoigian, this->lstOffer[idx].name, this->lstOffer[idx].type);
        item->setTag(15);
        cell->addChild(item);
        cell->setIdx(idx);
    }
    else
    {
        LayerItemOffer* item = (LayerItemOffer*)cell->getChildByTag(15);
        if (item){
            item->setDatas(this->lstOffer[idx].id, this->lstOffer[idx].trangthai, this->lstOffer[idx].thoigian, this->lstOffer[idx].name, this->lstOffer[idx].type);
        }
    }
    return cell;
}
ssize_t LayerOffer::numberOfCellsInTableView(TableView *table)
{
    
    return this->lstOffer.size();//lstRank.size();
}
void LayerOffer::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp("eeglsors", cmd->c_str()) == 0){
        if (dataManager.getAppConfig().isHideBilling || dataManager.getAppConfig().is24h)
            return;

        SceneManager::getSingleton().hideLoading();
        //4|Tặng 500 Vàng & khuyến mãi 5% Giá trị thẻ nạp duy nhất cho Thẻ nạp đầu tiên nếu nạp thẻ thành công bất kỳ mệnh giá|2|1|0;11|Tặng 1.500 Vàng & khuyến mãi 5% Giá trị thẻ nạp duy nhất cho Thẻ nạp đầu tiên nếu nạp thẻ thành công bất kỳ mệnh giá|2|1|0;1|Tặng 100 Vàng + 100 EXP|1|2|0;
        string lso = *param->GetUtfString("lso");
        if (dataManager.getAppConfig().isHideBilling)
            lso = "";
        auto lst = mUtils::splitString(lso, '/');
        if (lst.size()<2 ) {
            this->txtNoGift->setVisible(true);

            return;
        }
        auto lstoffer = mUtils::splitString(lst[0], ';');
        this->lstOffer.clear();
        if (lstoffer.size()<=0){
            this->txtNoGift->setVisible(true);

        }
        for (int i = 0 ; i< lstoffer.size();i++){
            auto itemoffer = mUtils::splitString(lstoffer[i], '|');
            if (itemoffer.size() < 5) return;
            Offer it;
            it.id = atoi(itemoffer[0].c_str());
            it.name = itemoffer[1];
            it.thoigian = atoi(itemoffer[4].c_str());
            it.trangthai = atoi(itemoffer[3].c_str());
            it.type = atoi(itemoffer[2].c_str());
            this->lstOffer.push_back(it);
        }
//        if (this->lstOffer.size()>0){
//            this->txtThongBao->setString("Chúc mừng, Bạn nhận được quà tặng bất ngờ từ Mộc Quán!");
//        }else{
//            this->txtThongBao->setString("Hiện tại bạn không nhận được phần thưởng nào từ Mộc quán!");
//
//        }
        this->tableSpecial->reloadData();
    }else if (strcmp("eerors", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        if (param->GetInt(AWARD_SILVER) == NULL || param->GetInt("rc") == NULL || param->GetInt(AWARD_GOLD) == NULL || param->GetUtfString("rd") == NULL || param->GetInt(AWARD_EXP) == NULL) return;
        int rc = *param->GetInt("rc");
        if (rc == 0){
            
            
            int silver = *param->GetInt(AWARD_SILVER);
            int gold = *param->GetInt(AWARD_GOLD);
            int exp = *param->GetInt(AWARD_EXP);
            
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            string content = dataManager.GetSysString(530)+ " ";
            if (silver> 0){
                content+=StringUtils::format("%s %s ",mUtils::convertMoneyEx(silver).c_str(),SceneManager::getSingleton().getCoin(1).c_str());
            }
            if (gold> 0){
                content+=StringUtils::format("%s %s ",mUtils::convertMoneyEx(gold).c_str(),SceneManager::getSingleton().getCoin(0).c_str());
            }
            if (exp> 0){
                content+=StringUtils::format("%d EXP ",exp);
            }
            content += "!";

            layerPopup->setContentMess(content);
                layerPopup->setTag(24);
                layerPopup->showAButton();
                _currScene->addChild(layerPopup,1000000,9092);
                
                boost::shared_ptr<ISFSObject> params(new SFSObject());
                boost::shared_ptr<IRequest> request(new ExtensionRequest("eeglsorq",params));
                GameServer::getSingleton().Send(request);

                SceneManager::getSingleton().showLoading();
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
            _currScene->addChild(layerPopup,1000000,9093);
        }
    }
}

bool LayerOffer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerOffer::onEnter()
{
    Layer::onEnter();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = CC_CALLBACK_2(LayerOffer::onTouchBegan, this);
//    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerOffer::onExit()
{
    Layer::onExit();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerItemOffer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("ItemSpecialOffer.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    
    this->txtName = static_cast<Text*>(root->getChildByName("txtName"));
    this->txtTimer = static_cast<Text*>(root->getChildByName("txtTimer"));
    this->btnGet = static_cast<Button*>(root->getChildByName("btnGet"));
    
    if (this->btnGet){
        this->btnGet->addClickEventListener(CC_CALLBACK_1(LayerItemOffer::onBtnNhanThuong, this));
        this->btnGet->setPressedActionEnabled(true);
    }
    
    return true;
}
void LayerItemOffer::setDatas(int index, int trangthai, int sogiay,string name, int type){
    this->btnGet->setTag(index);
    this->currType = type;
    if (trangthai == 1){
        this->btnGet->setEnabled(false);
        this->btnGet->setTitleText(dataManager.GetSysString(600));
    }else if (trangthai == 0){
        if (type == 1){
            this->btnGet->setEnabled(true);
            this->btnGet->setTitleText(dataManager.GetSysString(638));
        }else{
            this->btnGet->setEnabled(true);
            this->btnGet->setTitleText(dataManager.GetSysString(566));
        }
       
    }else if (trangthai == 2){
        this->btnGet->setEnabled(false);
        this->btnGet->setTitleText(dataManager.GetSysString(613));
    }
    this->timer = sogiay;
    if (sogiay<=0){
        this->txtTimer->setString("00:00:00");
    }else{
        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerItemOffer::CountDownTimer),this,1, false);
    }
    this->txtName->setString(name);
}
void LayerItemOffer::onBtnNhanThuong(Ref* pSender){
    
    if (this->currType == 2)
        return;
    
    auto node = (Node*)(pSender);
    int tag = node->getTag();

    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt("id", tag);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("eerorq",params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}
void LayerItemOffer::CountDownTimer(float dt){
     if (this->timer == 0){
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerItemOffer::CountDownTimer),this);
        this->btnGet->setEnabled(false);
        this->btnGet->setTitleText(dataManager.GetSysString(613));
        this->txtTimer->setString("00:00:00");
    }
    this->timer -=1;
    this->txtTimer->setVisible(true);

    this->showTimer();
}
void LayerItemOffer::showTimer(){
    int hour= this->timer/3600;
    int second=this->timer % 3600;
    int minute=second/60;
    second %= 60;
    if ( hour >= 10){
        if (second >= 10){
            if (minute >= 10)
                this->txtTimer->setString(StringUtils::format("%d:%d:%d",hour,minute,second));
            else
                this->txtTimer->setString(StringUtils::format("%d:0%d:%d",hour,minute,second));
        }
        else{
            if (minute >= 10)
                this->txtTimer->setString(StringUtils::format("%d:%d:0%d",hour,minute,second));
            else
                this->txtTimer->setString(StringUtils::format("%d:0%d:0%d",hour,minute,second));
        }
    }else{
        if (second >= 10){
            if (minute >= 10)
                this->txtTimer->setString(StringUtils::format("%0d:%d:%d",hour,minute,second));
            else
                this->txtTimer->setString(StringUtils::format("%0d:0%d:%d",hour,minute,second));
        }
        else{
            if (minute >= 10)
                this->txtTimer->setString(StringUtils::format("%0d:%d:0%d",hour,minute,second));
            else
                this->txtTimer->setString(StringUtils::format("%0d:0%d:0%d",hour,minute,second));
        }
    }
}
