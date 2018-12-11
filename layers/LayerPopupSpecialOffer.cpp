//
//  LayerPopupSpecialOffer.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/27/16.
//
//

#include "LayerPopupSpecialOffer.h"
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
LayerPopupSpecialOffer::LayerPopupSpecialOffer()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupSpecialOffer::~LayerPopupSpecialOffer()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupSpecialOffer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupSpecialOffer.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_5"));
    pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
    }else{
        sizeAdd = Size(0,0);
    }
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
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = static_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupSpecialOffer::onBtnClose, this));
    }
    
    this->txtThongBao = static_cast<Text*>(pnlBg->getChildByName("txtThongBao"));
    this->txtThongBao->setString("");
    
    auto pnltbl = static_cast<Layout*>(pnlBg->getChildByName("pnlTable"));
    
    //
    pnltbl->setVisible(false);
    this->tableSpecial = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height));
    this->tableSpecial->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tableSpecial->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tableSpecial->setPosition(Vec2(pnltbl->getPosition().x+sizeAdd.width,pnltbl->getPosition().y-20));
    this->tableSpecial->setDelegate(this);
    this->tableSpecial->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    pnlBg->addChild(this->tableSpecial);

    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("eeglsorq",params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
    
    return true;
}
void LayerPopupSpecialOffer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerPopupSpecialOffer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(1435,150);
}

TableViewCell* LayerPopupSpecialOffer::tableCellAtIndex(TableView *table, ssize_t idx)
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
ssize_t LayerPopupSpecialOffer::numberOfCellsInTableView(TableView *table)
{
    
    return this->lstOffer.size();//lstRank.size();
}
void LayerPopupSpecialOffer::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerPopupSpecialOffer::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp("eeglsors", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        //4|Tặng 500 Vàng & khuyến mãi 5% Giá trị thẻ nạp duy nhất cho Thẻ nạp đầu tiên nếu nạp thẻ thành công bất kỳ mệnh giá|2|1|0;11|Tặng 1.500 Vàng & khuyến mãi 5% Giá trị thẻ nạp duy nhất cho Thẻ nạp đầu tiên nếu nạp thẻ thành công bất kỳ mệnh giá|2|1|0;1|Tặng 100 Vàng + 100 EXP|1|2|0;
        if (param->GetUtfString("lso") == NULL) return;
        string lso = *param->GetUtfString("lso");
        auto lstoffer = mUtils::splitString(lso, ';');
        this->lstOffer.clear();
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
        if (this->lstOffer.size()>0){
            this->txtThongBao->setString("Chúc mừng, Bạn nhận được quà tặng bất ngờ từ Mộc Quán!");
        }else{
            this->txtThongBao->setString("Hiện tại bạn không nhận được phần thưởng nào từ Mộc quán!");

        }
        this->tableSpecial->reloadData();
    }else if (strcmp("eerors", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        if (param->GetUtfString("rc") == NULL) return;
        int rc = *param->GetInt("rc");
        if (rc == 0){
            string rv = *param->GetUtfString("rv");
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            auto lstrv = mUtils::splitString(rv, '|');
            if (lstrv.size()<2) return;
            if (atoi(lstrv[0].c_str())> 0){
                if (atoi(lstrv[1].c_str()) <= 0){
                    LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
                    layerPopup->setMessage("Thông báo");
                    layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s Vàng!",lstrv[0].c_str()));
                    layerPopup->setTag(24);
                    layerPopup->showAButton();
                    _currScene->addChild(layerPopup,1000000);
                }else if (atoi(lstrv[1].c_str()) > 0){
                    LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
                    layerPopup->setMessage("Thông báo");
                    layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s Vàng và %s EXP!",lstrv[0].c_str(),lstrv[1].c_str()));
                    layerPopup->setTag(24);
                    layerPopup->showAButton();
                    _currScene->addChild(layerPopup,1000000);
                }
                
                boost::shared_ptr<ISFSObject> params(new SFSObject());
                boost::shared_ptr<IRequest> request(new ExtensionRequest("eeglsorq",params));
                GameServer::getSingleton().Send(request);

                SceneManager::getSingleton().showLoading();
            }
            }else{
            if (param->GetUtfString("rd") == NULL) return;
            string rd = *param->GetUtfString("rd");
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage("Thông báo");
            layerPopup->setContentMess(rd);
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,1000000);
        }
    }
}

bool LayerPopupSpecialOffer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupSpecialOffer::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupSpecialOffer::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupSpecialOffer::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
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
        this->btnGet->setTitleText("Đã nhận");
    }else if (trangthai == 0){
        if (type == 1){
            this->btnGet->setEnabled(true);
            this->btnGet->setTitleText("Nhận");
        }else{
            this->btnGet->setEnabled(true);
            this->btnGet->setTitleText("Chưa nhận");
        }
       
    }else if (trangthai == 2){
        this->btnGet->setEnabled(false);
        this->btnGet->setTitleText("Hết hạn");
    }
    this->timer = sogiay;
    if (sogiay<=0){
        this->txtTimer->setString("00:00:00");
    }else{
        this->schedule(schedule_selector(LayerItemOffer::CountDownTimer),1);
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
        this->unschedule(schedule_selector(LayerItemOffer::CountDownTimer));
        this->btnGet->setEnabled(false);
        this->btnGet->setTitleText("Hết hạn");
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
