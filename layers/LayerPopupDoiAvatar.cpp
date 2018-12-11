//
//  LayerPopupDoiAvatar.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/27/16.
//
//

#include "LayerPopupDoiAvatar.h"
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
LayerPopupDoiAvatar::LayerPopupDoiAvatar()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupDoiAvatar::~LayerPopupDoiAvatar()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupDoiAvatar::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupDoiAvatar.csb");
    
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
    Panel_2->setSwallowTouches(false);
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
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
    
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    pnlBg->setSwallowTouches(false);
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(940));

    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiAvatar::onBtnClose, this));
    }
    
    Button* btnChooseAvatar = dynamic_cast<Button*>(pnlBg->getChildByName("btnChooseAvatar"));
    if(btnChooseAvatar){
        btnChooseAvatar->addTouchEventListener(CC_CALLBACK_2(LayerPopupDoiAvatar::onChangeAvatarClick, this));
        btnChooseAvatar->setTitleText(dataManager.GetSysString(786));
    }
    //
        Layout* pnlListAvatar = (Layout*)pnlBg->getChildByName("pnlListAvatar");
    pnlListAvatar->setSwallowTouches(false);
        if (pnlListAvatar == NULL){
           // log("pnlListAvatar == NULL");
        }
    
        Size size = pnlListAvatar->getContentSize();
       // log("size = %f : %f", size.width, size.height);
    
        this->tableListAvatar = TableView::create(this, pnlListAvatar->getContentSize());
        this->tableListAvatar->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
        this->tableListAvatar->setAnchorPoint(pnlListAvatar->getAnchorPoint());
        this->tableListAvatar->setPosition(pnlListAvatar->getPosition());
        this->tableListAvatar->setDelegate(this);
        this->tableListAvatar->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
        pnlBg->addChild(this->tableListAvatar);


    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
    return true;
}
void LayerPopupDoiAvatar::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerPopupDoiAvatar::hideLayer(){
    this->setVisible(false);
}
void LayerPopupDoiAvatar::showLayer(){
    this->setVisible(true);
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("eeglsorq",params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}
void LayerPopupDoiAvatar::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}
void LayerPopupDoiAvatar::changeAvatarCallBack(Ref *pSend){
        ChangeAvatarMoc* change = (ChangeAvatarMoc*)pSend;
        if (change != NULL){
           // log("avatar choose: %s", change->getUrl().c_str());
        }
        for (int i = 0; i < this->arrAvatarChange.size(); ++i) {
            this->arrAvatarChange.at(i)->visibleIconCheck(false);
        }
}
Size LayerPopupDoiAvatar::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, (475 / 2));
}

void LayerPopupDoiAvatar::onChangeAvatarClick(Ref* pSender, ui::Widget::TouchEventType type){
    if (type == ui::Widget::TouchEventType::ENDED){
        
        string url = "";
        for (int i = 0; i < this->arrAvatarChange.size(); ++i) {
            ChangeAvatarMoc* obj = this->arrAvatarChange.at(i);
            if (obj->getIsChoose()) {
                url = obj->getUrl();
                break;
            }
        }
        
        if (url == "") {
           // log("Bạn chưa chọn avatar !");
            return;
        }
        
       // log(" ===================================================================================== ");
       // log("Send Request update aal %s", url.c_str());
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString("aal", url);
        boost::shared_ptr<IRequest> request(new ExtensionRequest("ruali", params));
        GameServer::getSingleton().Send(request);
        this->removeFromParentAndCleanup(true);
    }
}

TableViewCell* LayerPopupDoiAvatar::tableCellAtIndex(TableView *table, ssize_t idx)
{
            TableViewCell* cell = table->dequeueCell();
            if (!cell) {
                cell = new TableViewCell();
                cell->autorelease();
                cell->setAnchorPoint(Vec2(0, 0));
                for (int i = 0; i < 4; ++i) {
                    try{
                        int index = i + idx * 4;
                        float startX = (table->getContentSize().width-320*4)/2;
                        string url = SceneManager::getSingleton().arrLink.at(index);
                        ChangeAvatarMoc* sp = ChangeAvatarMoc::create(url);
                       // log("url init: %s", url.c_str());
                        sp->setPosition(Vec2(i * 320+startX, 0));
                        sp->setTag(index);
                        sp->onCallBack(this, callfuncO_selector(LayerPopupDoiAvatar::changeAvatarCallBack));
                        cell->addChild(sp);
                        this->arrAvatarChange.push_back(sp);
                    }
                    catch (...){
    
                    }
                }
            }
            else {
                for (int i = 0; i < 4; ++i) {
                    try{
                        int index = i + idx * 4;
                        string url = SceneManager::getSingleton().arrLink.at(index);
                       // log("url reload: %s", url.c_str());
                        ChangeAvatarMoc* sp = (ChangeAvatarMoc*)cell->getChildByTag(index);
                        if (sp != NULL){
                            sp->setIcon(url);
                            sp->onCallBack(this, callfuncO_selector(LayerPopupDoiAvatar::changeAvatarCallBack));
                        }
                    }
                    catch (...){
    
                    }
                }
            }
            return cell;
}
ssize_t LayerPopupDoiAvatar::numberOfCellsInTableView(TableView *table)
{
    
    return 2;//this->lstOffer.size();//lstRank.size();
}
void LayerPopupDoiAvatar::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
//    if (strcmp("eeglsors", cmd->c_str()) == 0){
//        SceneManager::getSingleton().hideLoading();
//        //4|Tặng 500 Vàng & khuyến mãi 5% Giá trị thẻ nạp duy nhất cho Thẻ nạp đầu tiên nếu nạp thẻ thành công bất kỳ mệnh giá|2|1|0;11|Tặng 1.500 Vàng & khuyến mãi 5% Giá trị thẻ nạp duy nhất cho Thẻ nạp đầu tiên nếu nạp thẻ thành công bất kỳ mệnh giá|2|1|0;1|Tặng 100 Vàng + 100 EXP|1|2|0;
//        if (param->GetUtfString("lso") == NULL) return;
//        string lso = *param->GetUtfString("lso");
//        auto lstoffer = mUtils::splitString(lso, ';');
//        this->lstOffer.clear();
//        for (int i = 0 ; i< lstoffer.size();i++){
//            auto itemoffer = mUtils::splitString(lstoffer[i], '|');
//            if (itemoffer.size() < 5) return;
//            Offer it;
//            it.id = atoi(itemoffer[0].c_str());
//            it.name = itemoffer[1];
//            it.thoigian = atoi(itemoffer[4].c_str());
//            it.trangthai = atoi(itemoffer[3].c_str());
//            it.type = atoi(itemoffer[2].c_str());
//            this->lstOffer.push_back(it);
//        }
////        if (this->lstOffer.size()>0){
////            this->txtThongBao->setString("Chúc mừng, Bạn nhận được quà tặng bất ngờ từ Mộc Quán!");
////        }else{
////            this->txtThongBao->setString("Hiện tại bạn không nhận được phần thưởng nào từ Mộc quán!");
////
////        }
//        this->tableSpecial->reloadData();
//    }else if (strcmp("eerors", cmd->c_str()) == 0){
//        SceneManager::getSingleton().hideLoading();
//        if (param->GetUtfString("rc") == NULL || param->GetUtfString("rv") == NULL) return;
//        int rc = *param->GetInt("rc");
//        if (rc == 0){
//            string rv = *param->GetUtfString("rv");
//            auto _currScene = Director::getInstance()->getRunningScene();
//            if (!_currScene) return;
//            auto lstrv = mUtils::splitString(rv, '|');
//            if (lstrv.size()<3) return;
//            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
//            layerPopup->setMessage(dataManager.GetSysString(141));
//            string content = "Bạn nhận được ";
//            if (atoi(lstrv[0].c_str())> 0){
//                content+=StringUtils::format("%s %s ",lstrv[0].c_str(),SceneManager::getSingleton().getCoin(0).c_str());
//            }
//            if (atoi(lstrv[1].c_str())> 0){
//                content+=StringUtils::format("%s %s ",lstrv[1].c_str(),SceneManager::getSingleton().getCoin(1).c_str());
//            }
//            if (atoi(lstrv[2].c_str())> 0){
//                content+=StringUtils::format("%s EXP ",lstrv[2].c_str());
//            }
//            content += "!";
//            layerPopup->setContentMess(content);
//                layerPopup->setTag(24);
//                layerPopup->showAButton();
//                _currScene->addChild(layerPopup,1000000);
//                
//                boost::shared_ptr<ISFSObject> params(new SFSObject());
//                boost::shared_ptr<IRequest> request(new ExtensionRequest("eeglsorq",params));
//                GameServer::getSingleton().Send(request);
//
//                SceneManager::getSingleton().showLoading();
//            }else{
//            if (param->GetUtfString("rd") == NULL) return;
//            string rd = *param->GetUtfString("rd");
//            auto _currScene = Director::getInstance()->getRunningScene();
//            if (!_currScene) return;
//            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
//            layerPopup->setMessage(dataManager.GetSysString(141));
//            layerPopup->setContentMess(rd);
//            layerPopup->setTag(24);
//            layerPopup->showAButton();
//            _currScene->addChild(layerPopup,1000000);
//        }
//    }
}

bool LayerPopupDoiAvatar::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupDoiAvatar::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupDoiAvatar::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupDoiAvatar::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
   //     GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
ChangeAvatarMoc::ChangeAvatarMoc(const string& url){
    this->url = url;
    this->isChoose = false;
    this->iconCheck = NULL;
    this->circleAvatar = NULL;
    this->mCallBack = NULL;
    this->mCallBackListener = NULL;
}

ChangeAvatarMoc::~ChangeAvatarMoc(){
    //gImgDownloadProcess->stopDownload();
    //delete gImgDownloadProcess;
   // log("ChangeAvatarMoc::~ChangeAvatarMoc()");
}

ChangeAvatarMoc* ChangeAvatarMoc::create(const string& url){
    ChangeAvatarMoc* a = new ChangeAvatarMoc(url);
    if (a && a->init())
    {
        a->autorelease();
        return a;
    }
    else
    {
        delete a;
        a = NULL;
        return NULL;
    }
}

bool ChangeAvatarMoc::init(){
    if (!Layer::init())
        return false;
    
    this->setAnchorPoint(Vec2(0, 0));
    
    this->circleAvatar = CircleAvatar::create();
    this->circleAvatar->setScale(1.2);
    //this->circleAvatar->hideIconDefault(false);
    this->circleAvatar->setPosition(Vec2(140, 475 / 4));
    this->addChild(this->circleAvatar);
    ImageView* trans = ImageView::create("icon_transparent_default.png");
    trans->setAnchorPoint(this->circleAvatar->getAnchorPoint());
    trans->setPosition(Vec2(140, 475 / 4));
    trans->setTouchEnabled(true);
    trans->addTouchEventListener(CC_CALLBACK_2(ChangeAvatarMoc::onAvatarTouches, this));
    this->addChild(trans);
    
    Size _si = Size(159, 159);
    this->iconCheck = Sprite::create("moc3-btn-checked.png");
    this->iconCheck->setAnchorPoint(Vec2(0.5, 0.5));
    this->iconCheck->setPosition(Vec2(_si.width - 30, _si.height - 30));
    this->iconCheck->setScale(0.8);
    this->iconCheck->setVisible(false);
    trans->addChild(this->iconCheck, 1000);

    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself != NULL ) {
        auto all = myself->GetVariable("aal");
        if (all){
            if (all->GetStringValue()->compare(this->url) == 0){
                this->iconCheck->setVisible(true);
            }
        }
    }
//    this->gImgDownloadProcess = new ImageFromUrl();
//    gImgDownloadProcess->loadImage(trans,this->url);
    this->setIcon(this->url);
    //this->setIcon(this->url);
    
    return true;
}

void ChangeAvatarMoc::setIcon(string& url){
    this->url = url;
    this->circleAvatar->setAvatarImage(url);
}

string& ChangeAvatarMoc::getUrl() {
    return this->url;
}

bool ChangeAvatarMoc::getIsChoose(){
    return this->isChoose;
}

void ChangeAvatarMoc::visibleIconCheck(bool state) {
    this->iconCheck->setVisible(state);
    this->isChoose = state;
}

void ChangeAvatarMoc::onCallBack(Ref *target, SEL_CallFuncO listener){
    this->mCallBack = target;
    this->mCallBackListener = listener;
}

void ChangeAvatarMoc::onAvatarTouches(Ref* pSender, ui::Widget::TouchEventType type){
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if (this->mCallBack && this->mCallBackListener) {
            (this->mCallBack->*this->mCallBackListener)(this);
        }
        this->iconCheck->setVisible(!this->iconCheck->isVisible());
        this->isChoose = !this->isChoose;
    }
}
