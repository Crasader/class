//
//  LayerPopupTaoBan.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/13/16.
//
//

#include "LayerPopupTaoBan.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"
#include "Requests/ExtensionRequest.h"
#include "../data_manager.h"
#include "../scenes/SceneMain.h"
LayerPopupTaoBan::LayerPopupTaoBan()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupTaoBan::~LayerPopupTaoBan()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerPopupTaoBan::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupTaoBan.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
   
    pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
//    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
//
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(868));
    auto lblTip = static_cast<Text*> (pnlBg->getChildByName("txtLabel"));
    lblTip->setString(dataManager.GetSysString(869));
    auto lblLabel = static_cast<Text*> (pnlBg->getChildByName("txtLabel_0"));
    lblLabel->setString(dataManager.GetSysString(870));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupTaoBan::onBtnClose, this));
    }

    this->btnMucCuoc = dynamic_cast<Button*>(pnlBg->getChildByName("btnMucCuoc"));
    if(this->btnMucCuoc){
        this->btnMucCuoc->addClickEventListener(CC_CALLBACK_1(LayerPopupTaoBan::onBtnMucCuoc, this));
    }
    
    Button* btnTaoBan = dynamic_cast<Button*>(pnlBg->getChildByName("btnTaoBan"));
    if(btnTaoBan){
        btnTaoBan->addClickEventListener(CC_CALLBACK_1(LayerPopupTaoBan::onBtnTaoBan, this));
        btnTaoBan->setTitleText(dataManager.GetSysString(862));

    }
    
    this->tfMatKhau = dynamic_cast<TextField*>(pnlBg->getChildByName("tfMatKhau"));
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    this->tfMatKhau->setInputMode(ui::EditBox::InputMode::NUMERIC);
    this->tfMatKhau->setMaxLength(4);
#endif
    
    if (dataManager.getAppConfig().isNhapPass){
        this->tfMatKhau->setPlaceHolder(dataManager.GetSysString(633));
    }


    this->pnlMenhGia = dynamic_cast<Layout*>(pnlBg->getChildByName("pnlMenhGia"));
    this->pnlMenhGia->setVisible(false);
    this->pnlMenhGia->setSwallowTouches(false);
    Size container_size = Size(this->pnlMenhGia->getContentSize().width, this->pnlMenhGia->getContentSize().height*3);
    this->container = Layer::create();
    container->setContentSize(container_size);
    this->scrollView = ui::ScrollView::create();
    this->scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollView->setClippingEnabled(true);
    this->scrollView->setTouchEnabled(true);
    this->scrollView->setContentSize(this->pnlMenhGia->getContentSize());
    this->scrollView->setAnchorPoint(this->pnlMenhGia->getAnchorPoint());
    this->scrollView->setPosition(this->pnlMenhGia->getPosition());
    this->scrollView->setScrollBarEnabled(true);
    this->scrollView->setBounceEnabled(true);
    this->scrollView->setTag(12932);
    //this->scrollView->addChild(this->container);
    pnlBg->addChild(this->scrollView);
    
    this->loadMucCuoc();
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    
    params->PutInt("cfk", 17);//CONFIG_KEY = "cfk"; 1: thuế chuyển tiền giữa các accounts
    
    boost::shared_ptr<IRequest> request(new ExtensionRequest("reqgcf", params)); //REQUEST_GET_CONFIG = "reqgcf";
    GameServer::getSingleton().Send(request);

    this->config = 17;
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
    return true;
}
void LayerPopupTaoBan::loadMucCuoc(){
    string listBet = SceneManager::getSingleton().getListBet();
    vector<string> listConfig = mUtils::splitString(listBet, '|');

    
    for (int i = 0; i < listConfig.size() ; i++){
        vector<string> lstt1 = mUtils::splitString(listConfig[i], ':');
		vector<string> lstt2 = mUtils::splitString(lstt1[1], ',');
		for (int j = 0; j < lstt2.size(); j++){
            if (atoi(lstt2[j].c_str())>=this->minCuoc)
                listbet.push_back(lstt2[j]);
        }
    }
    
    this->scrollView->setInnerContainerSize(Size(692, 80*this->listbet.size()));

  
    for (int i = 0 ; i < this->listbet.size(); i++){
            auto button = ui::Button::create();
            button->loadTextures("design/guifiles/moc3-btnnhamang.png", "design/guifiles/moc3-btnnhamang.png", "design/guifiles/moc3-btnnhamang.png");
            button->setAnchorPoint(Point(0, 1));
            button->setContentSize(Size(692, 80));
            button->setTitleText(mUtils::convertMoneyEx(atoi(this->listbet[i].c_str())));
            button->setTitleFontSize(40);
            //button->setTitleFontName("__Roboto-Bold.ttf");
            button->addClickEventListener(CC_CALLBACK_1(LayerPopupTaoBan::onMucCuoc, this));
            //button->setTitleColor(ccc3(255, 255, 255));
            button->setTag(i);
            /*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
            button->setPosition(Vec2(0,80*this->listbet.size() - i * 80));
            this->scrollView->addChild(button);
            this->scrollView->setVisible(false);
        }
    
    int tag = 0;
    this->betValue = atoi(this->listbet[tag].c_str());
    this->isShowSelectMenhGia = false;
    this->scrollView->setVisible(false);
    
    this->btnMucCuoc->setTitleText(mUtils::convertMoneyEx(this->betValue));
}
void LayerPopupTaoBan::onMucCuoc(Ref* pSender){
    auto sender = (Button*)pSender;
    int tag = sender->getTag();
    this->betValue = atoi(this->listbet[tag].c_str());
    this->isShowSelectMenhGia = false;
    this->scrollView->setVisible(false);
    
    this->btnMucCuoc->setTitleText(mUtils::convertMoneyEx(this->betValue));
}
void LayerPopupTaoBan::onBtnMucCuoc(Ref* pSender){
    if (this->isShowSelectMenhGia) {
        this->isShowSelectMenhGia = false;
        this->scrollView->setVisible(false);
        
        
    }else{
        this->isShowSelectMenhGia = true;
        this->scrollView->setVisible(true);
        
    }
}
double LayerPopupTaoBan::getMinBetByGame(int gameID, double amf)
{
    double betUser = 0;
    vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
    int bet_ratio = 1;
    for (int i = 0; i < arrBet.size(); ++i)
    {
        if (arrBet.at(i).gameid == gameID)
        {
            bet_ratio = arrBet.at(i).ratioBet;
            break;
        }
    }
    betUser = amf / bet_ratio;
    return betUser;
}
void LayerPopupTaoBan::onBtnTaoBan(Ref* pSender){
    double amfUser = 0;
    int typeRoom = SceneManager::getSingleton().getCurrRoomType();
    amfUser = SceneManager::getSingleton().getMoneyByType(typeRoom);
    int fee = this->betValue;
    if (this->betValue >= this->maxCuoc){
        fee = this->maxCuoc;
    }
    if (amfUser <= 0 || this->betValue > amfUser - fee)
    {
        SceneManager::getSingleton().showPopupMoneyCharge();
        return;
    }
    int _gameID = SceneManager::getSingleton().getGameID();
    double betUser = this->getMinBetByGame(_gameID, amfUser-fee);
    if (betUser < this->betValue)
    {
        SceneManager::getSingleton().showPopupMoneyCharge();
        return;
    }
    
    if (dataManager.getAppConfig().isNhapPass){
        if (this->tfMatKhau->getString() == ""){
            SceneManager::getSingleton().showToast(dataManager.GetSysString(125));
            return;
        }
    }
    
    
    if (this->tfMatKhau->getString().length()<4){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(126));
        return;
    }
    
    if (this->tfMatKhau->getString() != "" && mUtils::findContainChar(this->tfMatKhau->getString())){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(127));
        return;
    }


	int m_gameID = SceneManager::getSingleton().getGameID();
	string _configStr = StringUtils::format("%d@%d@%d",
		this->betValue, 0, 1);
	if (m_gameID == kGameChan)
	{
		string xmlConfig = "<tableconfig><playing_rule><checkAnBaoWhenU>0</checkAnBaoWhenU><checkULao>0</checkULao>";
		xmlConfig.append("<choiNuoiGa>1</choiNuoiGa><choiUXuong>1</choiUXuong><choiDiemToiThieu>0</choiDiemToiThieu>");
		xmlConfig.append("<koChoiCacCuocSac></koChoiCacCuocSac></playing_rule><timeout><playerReadyTimeout>60</playerReadyTimeout>");
		xmlConfig.append("<discardTimeout>30</discardTimeout><turnTimeout>90</turnTimeout><expectingUTimeout>10</expectingUTimeout>");
		xmlConfig.append("<xuongCuocSacTimeout>60</xuongCuocSacTimeout><stepTimeout>30</stepTimeout><waitForChiuTimeout>5</waitForChiuTimeout>");
		xmlConfig.append("<waitUDeTimeout>10</waitUDeTimeout></timeout></tableconfig>");

		_configStr = StringUtils::format("%d@0@1@%s", this->betValue, xmlConfig.c_str());
	}
	else if (m_gameID == kGamePhom)
	{
		int discard_timeout = 30;
		int endGameTimeout = 8;//hien tai default cua phom dang la 8s
		string xmlConfig = StringUtils::format("<tableconfig><playing_rule><U_0_Factor>5</U_0_Factor><U_9_Factor>5</U_9_Factor><U_KHAN_Factor>6</U_KHAN_Factor><U_10_Factor>10</U_10_Factor><rank2Factor>1</rank2Factor><rank3Factor>2</rank3Factor><rank4Factor>3</rank4Factor><momFactor>4</momFactor><fakeTaking_Factor>5</fakeTaking_Factor><discardBeforeHandoffPhom>false</discardBeforeHandoffPhom><take3MustEndPlaying>true</take3MustEndPlaying><endPlayingAfterU10>false</endPlayingAfterU10><choiNuoiGa>true</choiNuoiGa></playing_rule><timeout><playerReadyTimeout>%d</playerReadyTimeout><discardTimeout>%d</discardTimeout><turnTimeout>%d</turnTimeout><endGameTimeout>%d</endGameTimeout></timeout></tableconfig>", discard_timeout, discard_timeout, discard_timeout, endGameTimeout);
		_configStr = StringUtils::format("%d@0@1@%s", this->betValue, xmlConfig.c_str());
	}
    
    
    boost::shared_ptr<ISFSObject> roomConfig(new SFSObject());
    roomConfig->PutUtfString("gid", StringUtils::format("%d", SceneManager::getSingleton().getGameID()));
    roomConfig->PutUtfString("params", _configStr);
    roomConfig->PutUtfString("rpass", this->tfMatKhau->getString());
    roomConfig->PutInt("roomType", typeRoom);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("rcroom", roomConfig));
    GameServer::getSingleton().Send(request);
    SceneMain::getSingleton().prepareToPickGame();


}
void LayerPopupTaoBan::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerPopupTaoBan::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp("respcf", cmd->c_str()) == 0){ //RESP_GET_CONFIG = "respcf";
       
        if (param->GetDouble("cfv") == NULL) return;
        
        if (this->config == 17){
            this->minCuoc = *param->GetDouble("cfv");
            SceneManager::getSingleton().showLoading();
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            
            params->PutInt("cfk", 18);//CONFIG_KEY = "cfk"; 1: thuế chuyển tiền giữa các accounts
            
            boost::shared_ptr<IRequest> request(new ExtensionRequest("reqgcf", params)); //REQUEST_GET_CONFIG = "reqgcf";
            GameServer::getSingleton().Send(request);
            
            this->config = 18;
        }
        if (this->config == 18){
            SceneManager::getSingleton().hideLoading();
            this->maxCuoc = *param->GetDouble("cfv");
        }
    }
    //CREATE_ROOM_RESP = "createroomresp"
    else if (strcmp("createroomresp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        if (param->GetInt("rc") == NULL || param->GetUtfString("rd") == NULL) return;
        int rc = *param->GetInt("rc");
        string rd = *param->GetUtfString("rd");
        if (rc != 0){
            SceneManager::getSingleton().showToast(rd);
        }
    }
}

bool LayerPopupTaoBan::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupTaoBan::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupTaoBan::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupTaoBan::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
