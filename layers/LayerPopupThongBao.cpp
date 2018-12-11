#include "LayerPopupThongBao.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Requests/ExtensionRequest.h"
#include "../SceneManager.h"
#include "../scenes/SceneLogin.h"
#include "layergames/PhomMessDef.h"
using namespace cocos2d::ui;

LayerPopupThongBao::LayerPopupThongBao()
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupThongBao::~LayerPopupThongBao()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupThongBao::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(false);
	this->setTag(POPUP_THONGBAO);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupThongbao.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));

	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(pnlBg->getContentSize());

	btn_huy = dynamic_cast<Button*>(rootNode->getChildByName("btnCancel"));
	if (btn_huy != NULL)
	{
		btn_huy->addClickEventListener(CC_CALLBACK_1(LayerPopupThongBao::onButtonHuy, this));
        btn_huy->setTitleText(dataManager.GetSysString(755));

	}
	btn_dongy = dynamic_cast<Button*>(rootNode->getChildByName("btnAccept"));
	if (btn_dongy != NULL)
	{
		btn_dongy->addClickEventListener(CC_CALLBACK_1(LayerPopupThongBao::onButtonDongy, this));
        btn_dongy->setTitleText(dataManager.GetSysString(754));
    }

	lblMessage = static_cast<Text*>(rootNode->getChildByName("lblMessage"));
	lblTitle = static_cast<Text*>(rootNode->getChildByName("lblTitle"));

	mCallBack = NULL;
	mCallBackListener = NULL;
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
	return true;
}

Button* LayerPopupThongBao::getBtnHuy(){
	return btn_huy;
}

Button* LayerPopupThongBao::getBtnOk(){
	return btn_dongy;
}

Text* LayerPopupThongBao::getLblTitle(){
	return lblTitle;
}

bool LayerPopupThongBao::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPopupThongBao::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupThongBao::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerPopupThongBao::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerPopupThongBao::setMessage(string msg)
{
	this->lblTitle->setString(msg);
}

void LayerPopupThongBao::onButtonHuy(Ref* pSender)
{
    if (this->isExit){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
        return;
#endif
        
        Director::getInstance()->end();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
        return;
    }
	//
	if (SceneLogin::isInstanceExisted())
		SceneLogin::getInstance()->setShouldAuto(true);
	//
	this->removeFromParentAndCleanup(true);
}
void LayerPopupThongBao::setEndGameWhenCancle(){
    this->isExit = true;
}
void LayerPopupThongBao::setPopupClose(){
    this->isThoat = true;
    btn_huy->setTitleText(dataManager.GetSysString(567));
}

void LayerPopupThongBao::onButtonDongy(Ref* pSender)
{
	//sample
	/*
			paramCallBack->setTag(0);
			(mCallBack->*mCallBackListener)(paramCallBack);
			*/
	if (mCallBack && mCallBackListener){
		//mCallBack
		Node *param = Node::create();
		param->setTag(tagcallback);
		(mCallBack->*mCallBackListener)(param);
        if (!this->isThoat && this->btn_dongy != NULL)
            this->removeFromParent();

	}
	else{
        if (this->isThoat){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
            MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
            return;
#endif
            
            Director::getInstance()->end();
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
        }
		this->removeFromParentAndCleanup(true);
	}

}

void LayerPopupThongBao::setCallBackFromButton(Ref* target, SEL_CallFuncO listener, int tag)
{
	mCallBack = target;
	mCallBackListener = listener;
	tagcallback = tag;
}

void LayerPopupThongBao::setContentMess(string msg)
{
	this->lblMessage->setString(msg);
}
void LayerPopupThongBao::showAButton(){
	this->btn_huy->setVisible(false);
	this->btn_dongy->setPositionX(this->getContentSize().width / 2);
}

void LayerPopupThongBao::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));

	if (ptrNotifiedCmd->compare("notiRMFF") == 0)
	{
		//this->scheduleOnce(schedule_selector(LayerPopupThongBao::sendRequestCountMess), 2);
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupThongBao::sendRequestCountMess), this, 2, 1, 2, false);
	}
}

void LayerPopupThongBao::sendRequestCountMess(float dt)
{
	//delay 2s to get new mail
	log("sendRequestCountMess");
	boost::shared_ptr<IRequest> request(new ExtensionRequest("cmi", NULL));//count msg inbox
	GameServer::getSingleton().Send(request);
}

LayerPopupWebView::LayerPopupWebView()
{
	GameServer::getSingleton().addListeners(this);

}

LayerPopupWebView::~LayerPopupWebView()
{
	GameServer::getSingleton().removeListeners(this);

}

bool LayerPopupWebView::init()
{
	if (!Layer::init())
		return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupWebView.csb");
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	this->addChild(rootNode);
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
	pnlBg->setScaleY(visibleSize.height / pnlBg->getContentSize().height);
	this->setContentSize(pnlBg->getContentSize());

	auto pnlWebView = (Layout*)rootNode->getChildByName("pnlw");
	Button* btnClose = (Button*)rootNode->getChildByName("btnClose");
	if (btnClose)
	{
		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPopupWebView::onButtonClose, this));
	}

	if (pnlWebView)
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
		auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
		auto posX = pnlWebView->getPositionX();
		auto posY = pnlWebView->getPositionY() + distance;
		webView = cocos2d::experimental::ui::WebView::create();
		webView->setAnchorPoint(Vec2(0.5, 0.5));
		webView->setPosition(Vec2(pnlWebView->getContentSize().width/2, pnlWebView->getContentSize().height/2));
		webView->setContentSize(Size(pnlWebView->getContentSize().width-125, pnlWebView->getContentSize().height-125));
		webView->setScalesPageToFit(true);
		pnlWebView->addChild(webView);
		webView->setVisible(false);

		webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerPopupWebView::onWebViewShouldStartLoading, this));
		webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerPopupWebView::onWebViewDidFinishLoading, this));
		webView->setOnDidFailLoading(CC_CALLBACK_2(LayerPopupWebView::onWebViewDidFailLoading, this));
#endif
	}
	return true;
}

void LayerPopupWebView::setWebView(string urlWeb)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	SceneManager::getSingleton().showLoading();
	webView->loadURL(urlWeb);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(urlWeb);
#endif
}

void LayerPopupWebView::setWebFile(string pathFile)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	webView->loadFile(pathFile);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(pathFile);
#endif
}

void LayerPopupWebView::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type)
{
	//
	if (SceneLogin::isInstanceExisted())
		SceneLogin::getInstance()->setShouldAuto(true);
	//
	if (type == ui::Widget::TouchEventType::ENDED)
		this->removeFromParentAndCleanup(true);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerPopupWebView::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().showLoading();
	sender->setVisible(false);
	return true;
}

void LayerPopupWebView::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
	webView->setVisible(true);
}

void LayerPopupWebView::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
}


#endif
void LayerPopupWebView::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	this->removeFromParentAndCleanup(true);
}



LayerPopupDanhSachCai::LayerPopupDanhSachCai()
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupDanhSachCai::~LayerPopupDanhSachCai()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupDanhSachCai::init()
{
	if (!Layer::init())
	{
		return false;
	}

	return true;
}

bool LayerPopupDanhSachCai::init(std::string _lstCaiReg)
{
	if (!Layer::init())
	{
		return false;
	}

	auto _visible = Director::getInstance()->getVisibleSize();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupDanhSachCai.csb");
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(_visible / 2));
	this->addChild(rootNode);

	auto pnlListCaiReg = static_cast<Layout*>(rootNode->getChildByName("pnlListCaiReg"));
	auto btnClose = static_cast<Button*>(rootNode->getChildByName("btnClose"));
	btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPopupDanhSachCai::onButtonClose, this));

	this->btnOk = static_cast<Button*>(rootNode->getChildByName("btnReg"));
	this->btnCancel = static_cast<Button*>(rootNode->getChildByName("btnCancelReg"));

	tblLstBoss = TableView::create(this, Size(pnlListCaiReg->getContentSize().width, pnlListCaiReg->getContentSize().height));
	tblLstBoss->setDirection(TableView::Direction::VERTICAL);
	tblLstBoss->setAnchorPoint(Point(0, 0));
	tblLstBoss->setPosition(Point(pnlListCaiReg->getPositionX(), 80));
	tblLstBoss->setDelegate(this);
	tblLstBoss->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tblLstBoss);

	if (_lstCaiReg.compare(" ") != 0){
		this->lstBoss = mUtils::splitString(_lstCaiReg, ',');
		log("lst= %s, size: %d", _lstCaiReg.c_str(), this->lstBoss.size());
		tblLstBoss->reloadData();
	}

	//request list
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom != NULL){
		boost::shared_ptr<SFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> req2(new ExtensionRequest("glbrq", params, lastRoom));
		GameServer::getSingleton().Send(req2);
	}

	return true;
}

void LayerPopupDanhSachCai::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED){
		this->removeFromParent();
	}
}

Button* LayerPopupDanhSachCai::getButtonOk()
{
	return this->btnOk;
}

Button* LayerPopupDanhSachCai::getButtonCancel()
{
	return this->btnCancel;
}

LayerPopupDanhSachCai* LayerPopupDanhSachCai::create()
{
	LayerPopupDanhSachCai* pRet = new LayerPopupDanhSachCai();
	if (pRet && pRet->init()){
		pRet->autorelease();
		return pRet;
	}
	else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

LayerPopupDanhSachCai* LayerPopupDanhSachCai::create(std::string _lstCaiReg)
{
	LayerPopupDanhSachCai *pRet = new LayerPopupDanhSachCai();
	if (pRet && pRet->init(_lstCaiReg)){
		pRet->autorelease();
		return pRet;
	}
	else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void LayerPopupDanhSachCai::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	/*boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));

	if (0 == ptrNotifiedCmd->compare("glbrs")){
	log("glbrs");
	this->event_EXT_EVENT_GET_LIST_BOSS_RES(param);
	}*/
}

void LayerPopupDanhSachCai::event_EXT_EVENT_GET_LIST_BOSS_RES(boost::shared_ptr<ISFSObject> param)
{
	//std::string lstDkCai = "";
	//boost::shared_ptr<string> _dsBoss = param->GetUtfString("bl");
	//if (_dsBoss != NULL){
	//	lstDkCai = *_dsBoss;
	//}
	////lstDkCai = "tester01,tester02, tester03,tester04,tester05";
	//if (lstDkCai.compare(" ") != 0){
	//	this->lstBoss = mUtils::splitString(lstDkCai, ',');
	//	log("lst= %s, size: %d", lstDkCai.c_str(), this->lstBoss.size());
	//	tblLstBoss->reloadData();
	//}
}

void LayerPopupDanhSachCai::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

cocos2d::Size LayerPopupDanhSachCai::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	Size _itemSize = Size(1383, 80);
	return _itemSize;
}

cocos2d::extension::TableViewCell* LayerPopupDanhSachCai::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	TableViewCell *cell = this->tblLstBoss->dequeueCell();
	string res = this->lstBoss[idx];
	int _cTag = 123;
	int cols = 3;
	if (!cell) {
		cell = new TableViewCell();
		cell->setAnchorPoint(Vec2(0, 0));
		cell->autorelease();
		cell->setTag(_cTag);

		LayerItemBoss *itemHis = LayerItemBoss::create();
		itemHis->setAnchorPoint(Vec2::ZERO);
		itemHis->setPosition(Vec2::ZERO);
		itemHis->setData(res, idx);
		itemHis->setTag(_cTag + idx);
		cell->addChild(itemHis);
	}
	else
	{
		LayerItemBoss* itemHis = (LayerItemBoss*)cell->getChildByTag(_cTag + idx);
		if (itemHis)
		{
			itemHis->setData(res, idx);
		}
	}
	return cell;
}

ssize_t LayerPopupDanhSachCai::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return this->lstBoss.size();
}

LayerPopupActiveCai::LayerPopupActiveCai()
{

}

LayerPopupActiveCai::~LayerPopupActiveCai()
{

}

bool LayerPopupActiveCai::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto _visible = Director::getInstance()->getVisibleSize();

	auto rootNode = CSLoader::getInstance()->createNode("LayerXocDiaPopupActiveCai.csb");
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(_visible / 2));
	this->addChild(rootNode);

	this->btnOk = static_cast<Button*>(rootNode->getChildByName("btnOk"));
	this->btnExit = static_cast<Button*>(rootNode->getChildByName("btnClose"));
	this->btnExit->addTouchEventListener(CC_CALLBACK_2(LayerPopupActiveCai::processButtonExitTouched, this));
	this->btnOk->addTouchEventListener(CC_CALLBACK_2(LayerPopupActiveCai::processButtonCupCaiTouched, this));
	return true;
}

Button* LayerPopupActiveCai::getButtonOk()
{
	return this->btnOk;
}

Button* LayerPopupActiveCai::getButtonExit()
{
	return this->btnExit;
}

void LayerPopupActiveCai::processButtonExitTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		this->removeFromParent();
	}
}

void LayerPopupActiveCai::processButtonCupCaiTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<SFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_REGISTER_BOSS_REG, params, lastRoom));
		GameServer::getSingleton().Send(req2);
		this->removeFromParent();
	}
}

LayerItemBoss::LayerItemBoss()
{

}

LayerItemBoss::~LayerItemBoss()
{

}

bool LayerItemBoss::init()
{
	if (!Layer::init())
		return false;
	auto root = CSLoader::getInstance()->createNode("LayerItemPopupDanhSachCai.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());
	this->setAnchorPoint(Vec2::ZERO);

	this->lblName = (Text*)root->getChildByName("lblAccName");
	this->lblStatus = (Text*)root->getChildByName("lblStatus");
	this->lblPos = (Text*)root->getChildByName("lblPos");

	return true;
}

void LayerItemBoss::setData(string _username, int idx)
{
	string _statusStr = dataManager.GetSysString(282);
	Color3B colorStatus = Color3B::WHITE;
	if (idx == 0)
	{
		_statusStr = dataManager.GetSysString(283);
		colorStatus = Color3B::GREEN;
	}
	this->lblStatus->setString(_statusStr);
	this->lblStatus->setColor(colorStatus);
	//this->lblName->setString(ChanUtils::GetNamePlayer(_username));
	this->lblPos->setString(StringUtils::format("%d", idx + 1));
}
