#include "LayerDoiThuong.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "mUtils.h"
#include "LayerPopupThongBao.h"
#include "SceneManager.h"
#include "Requests/ExtensionRequest.h"
#include "../layergames/_Chat_.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupXacThucOTP.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
LayerDoiThuong::LayerDoiThuong()
{
	GameServer::getSingleton().addListeners(this);
}

LayerDoiThuong::~LayerDoiThuong()
{
	SceneManager::getSingleton().getImageDownloadProcess()->stopDownload();
	GameServer::getSingleton().removeListeners(this);
}

bool LayerDoiThuong::init()
{
	if (!Layer::init())
	{
		return false;
	}
	mTax = 1;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();



	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {

		// add background
		auto background = Sprite::create("background-in-game.jpg");
		background->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
		background->setAnchorPoint(Point(0, 0));
		background->setPosition(Point(0, 0));
		this->addChild(background);

		auto sprBg = Sprite::create("background-in-game.jpg");
		sprBg->setPosition(visibleSize / 2);
		sprBg->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
		this->addChild(sprBg);

	}
	else{
		auto sprBg = Sprite::create("background-sanh.jpg");
		sprBg->setPosition(visibleSize / 2);
		sprBg->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
		this->addChild(sprBg);

	}
	auto root = CSLoader::getInstance()->createNode("LayerDoiThuong.csb");
	this->addChild(root);

	//DoiThuongTop
	auto nodeDoiThuongTop = dynamic_cast<Node*>(root->getChildByName("nodeDoiThuongTop"));
	nodeDoiThuongTop->setAnchorPoint(Point(0.5, 0.5));
	nodeDoiThuongTop->setPosition(Point(visibleSize.width / 2, visibleSize.height - nodeDoiThuongTop->getContentSize().height / 2));
	auto layerTopMenuOnlyTop = static_cast<Layer*>(nodeDoiThuongTop->getChildByName("layerOnlyBack"));
	if (layerTopMenuOnlyTop != NULL) {
		auto btnClose = static_cast<Button*>(layerTopMenuOnlyTop->getChildByName("btnBack"));
		if (btnClose != NULL) {
			btnClose->addTouchEventListener(CC_CALLBACK_2(LayerDoiThuong::onBtnClose, this));
			btnClose->setPressedActionEnabled(true);
		}
	}

	lblMoney = static_cast<Text*>(nodeDoiThuongTop->getChildByName("lblMoney"));

	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself != NULL) {
		if (myself->GetVariable("aN") != NULL && myself->GetVariable("amf") != NULL){
			auto amf = myself->GetVariable("amf")->GetDoubleValue();
			if (amf != NULL)
				lblMoney->setString(mUtils::convertMoneyEx(*amf).c_str());

		}

	}

	btnTheCao = static_cast<Button*>(nodeDoiThuongTop->getChildByName("btnTheCao"));
	if (btnTheCao){
		btnTheCao->setEnabled(false);
		btnTheCao->addTouchEventListener(CC_CALLBACK_2(LayerDoiThuong::onBtnTheCao, this));
	}

	btnLichSu = static_cast<Button*>(nodeDoiThuongTop->getChildByName("btnLichSu"));
	if (btnLichSu){
		btnLichSu->setEnabled(true);
		btnLichSu->addTouchEventListener(CC_CALLBACK_2(LayerDoiThuong::onBtnLichSu, this));
	}
	btnVatPham = static_cast<Button*>(nodeDoiThuongTop->getChildByName("btnVatPham"));
	if (btnVatPham){
		btnVatPham->setEnabled(true);
		btnVatPham->addTouchEventListener(CC_CALLBACK_2(LayerDoiThuong::onBtnVatPham, this));
	}

	//nodeDoiThuong
	// 	auto nodeDoiThuong = dynamic_cast<Node*>(root->getChildByName("nodeDoiThuong"));
	// 	//nodeDoiThuong->setAnchorPoint(Point(0.5, 1));
	// 	nodeDoiThuong->setPositionY(visibleSize.height - nodeDoiThuongTop->getContentSize().height);

	bg = static_cast<Layout*>(root->getChildByName("Panel_1"));
    pnlWeb = static_cast<Layout*>(root->getChildByName("pnlWeb"));
    pnlWeb->setSwallowTouches(false);

	this->nodeChuY = dynamic_cast<Node*>(root->getChildByName("nodeChuY"));
	nodeChuY->setAnchorPoint(Point(0.5, 1));
	nodeChuY->setPositionY(visibleSize.height - nodeDoiThuongTop->getContentSize().height);
	this->lblChuy = static_cast<Text*>(this->nodeChuY->getChildByName("lblTextChuY"));

	Text* txtMinCoin = static_cast<Text*>(this->nodeChuY->getChildByName("txtMinCoin"));
	txtMinCoin->setVisible(false);
	if (txtMinCoin)
		txtMinCoin->setString(StringUtils::format("- Số tiền sau khi đổi phải còn lại ít nhất %s %s.", mUtils::format_money((long)SceneManager::getSingleton().getMinMoney(1))->c_str(), SceneManager::getSingleton().getCoin(0).c_str()));

	Text* txtMinLevel = static_cast<Text*>(this->nodeChuY->getChildByName("txtMinLevel"));
	if (txtMinLevel)
		txtMinLevel->setString(StringUtils::format("- Level %d trở lên mới được đổi thưởng.", SceneManager::getSingleton().getMinMoney(4)));
	Button* btnQuyDinhDoiThuong = static_cast<Button*>(this->nodeChuY->getChildByName("btnQuyDinhDoiThuong"));
	if (btnQuyDinhDoiThuong){
		btnQuyDinhDoiThuong->setEnabled(true);
		btnQuyDinhDoiThuong->addTouchEventListener(CC_CALLBACK_2(LayerDoiThuong::onBtnQuyDinhDoiThuong, this));
	}

	Button* btnXacThucSDT = static_cast<Button*>(this->nodeChuY->getChildByName("btnChonSoDienThoai"));
	if (btnXacThucSDT){
		btnXacThucSDT->setEnabled(true);
		btnXacThucSDT->setVisible(false);
		btnXacThucSDT->addTouchEventListener(CC_CALLBACK_2(LayerDoiThuong::onBtnXacThucSoDienThoai, this));
		btnXacThucSDT->setVisible(!dataManager.getAppConfig().isHideXacThucSDT);
	}


	if (myself != NULL) {
		if (myself->GetVariable("lvl") != NULL)
		{
			if (myself->GetVariable("lvl")->GetIntValue() != NULL)
			if (*myself->GetVariable("lvl")->GetIntValue() >= SceneManager::getSingleton().getMinMoney(4))
				txtMinLevel->setVisible(false);
		}
	}

	/*float w_bg = bg->getContentSize().width * (visibleSize.width / DESIGN_RESOLUTION_WIDTH);*/
	float h_bg = bg->getContentSize().height * (visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	bg->setContentSize(Size(bg->getContentSize().width, h_bg));
	//bg->setPositionY(visibleSize.height - (nodeDoiThuongTop->getContentSize().height + bg->getContentSize().height));

	this->tblPhanThuong = TableView::create(this, this->bg->getContentSize());
	this->tblPhanThuong->setAnchorPoint(this->bg->getAnchorPoint());
	this->tblPhanThuong->setContentSize(this->bg->getContentSize());
	this->tblPhanThuong->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	this->tblPhanThuong->setPosition(this->bg->getPosition());
	this->tblPhanThuong->setDelegate(this);
	this->tblPhanThuong->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(this->tblPhanThuong);

	this->loadTheCao();
	this->onBtnTheCao(NULL, ui::Widget::TouchEventType::ENDED);

	loadAllDatas();
	this->lblChuy->setString("- Hệ thống sẽ duyệt và xử lý đổi thẻ, vật phẩm liên tục. Chi tiết Click Button Quy định đổi thưởng.");
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		this->lblChuy->setString("- Vip 1 trở lên mới được đổi thưởng.\n- Chi tiết xem tại nút \"Quy định đổi thưởng\".");
	}
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
		/*this->btnTheCao->setTitleColor(Color3B::BLACK);
		this->btnVatPham->setTitleColor(Color3B::WHITE);
		this->btnLichSu->setTitleColor(Color3B::WHITE);*/
	}
	currTab = 1;
	return true;
}

void LayerDoiThuong::onBtnClose(Ref* pSender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED)
		this->removeFromParentAndCleanup(true);
}

void LayerDoiThuong::createWebView() {
	//this.removeChildByTag(24);
	string myName = SceneManager::getSingleton().getMyName();
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL || myself->Name() == NULL)
		return;
	string url = "";
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		url = LS_DOI_THE_P2;
	}
	else{
		url = LS_DOI_THE;
	}
	url += myName + "&checksum=" + mUtils::getMD5KeyHis(myName);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	auto webView = cocos2d::experimental::ui::WebView::create();
	webView->setAnchorPoint(this->pnlWeb->getAnchorPoint());
	webView->setPosition(this->pnlWeb->getPosition());
	webView->setContentSize(Size(WIDTH_DESIGN,this->pnlWeb->getContentSize().height));

	webView->loadURL(url);
	webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerDoiThuong::onWebViewShouldStartLoading, this));
	webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerDoiThuong::onWebViewDidFinishLoading, this));
	webView->setOnDidFailLoading(CC_CALLBACK_2(LayerDoiThuong::onWebViewDidFailLoading, this));
	//webView->setContentSize(Size(lsvHistories->getContentSize().width * ratio, lsvHistories->getContentSize().height * ratio));
	webView->setScalesPageToFit(true);
	this->addChild(webView);
	webView->setVisible(false);
	webView->setTag(24);
	this->tblPhanThuong->setVisible(false);
	this->nodeChuY->setVisible(false);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(url);
#endif
}

void LayerDoiThuong::onBtnLichSu(Ref* pSender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		if (this->getParent() == NULL) return;
		// this->btnLichSu->setBright(false);
		SceneManager::getSingleton().showLoading();
		this->btnLichSu->setEnabled(false);
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
			/*this->btnLichSu->setTitleColor(Color3B::BLACK);
			this->btnVatPham->setTitleColor(Color3B::WHITE);
			this->btnTheCao->setTitleColor(Color3B::WHITE);*/
		}
		// this->btnTheCao->setBright(true);
		this->btnTheCao->setEnabled(true);
		this->btnVatPham->setEnabled(true);
		this->tblPhanThuong->setVisible(false);
		nodeChuY->setVisible(false);
		this->createWebView();
		currTab = 3;
	}
}
void LayerDoiThuong::onBtnXacThucSoDienThoai(Ref* pSender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){

		this->btnTheCao->setBright(true);
		this->btnTheCao->setEnabled(true);

		// this->btnLichSu->setBright(true);
		this->btnLichSu->setEnabled(true);
		this->btnVatPham->setEnabled(true);

		SceneManager::getSingleton().showLoading();
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		std::string MyAI = *GameServer::getSingleton().getSmartFox()->MySelf()->Name();
		params->PutUtfString("aI", MyAI);
		boost::shared_ptr<IRequest> request(new ExtensionRequest("ire", params));
		GameServer::getSingleton().Send(request);
		log("My AccountId %s", MyAI.c_str());

	}
}
void LayerDoiThuong::onBtnQuyDinhDoiThuong(Ref* pSender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		auto _currScene = Director::getInstance()->getRunningScene();
		if (!_currScene) return;
		string strlink = "";
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
			strlink = QUYDINH_DOITHUONG_P2;
		}
		else{
			strlink = QUYDINH_DOITHUONG;
		}
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		strlink += "i";
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		strlink += "an";
#endif
		LayerPopupWebView* _layer = LayerPopupWebView::create();
		_layer->setWebView(strlink);
		_currScene->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_WEBVIEW,9023);

	}
}
void LayerDoiThuong::onBtnTheCao(Ref* pSender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		if (this->getParent() == NULL) return;
		this->btnTheCao->setBright(false);
		this->btnTheCao->setEnabled(false);

		// this->btnLichSu->setBright(true);
		this->btnLichSu->setEnabled(true);
		this->btnVatPham->setEnabled(true);

		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
			/*this->btnTheCao->setTitleColor(Color3B::BLACK);
			this->btnVatPham->setTitleColor(Color3B::WHITE);
			this->btnLichSu->setTitleColor(Color3B::WHITE);*/

		}

		this->removeChildByTag(24);
		this->tblPhanThuong->setVisible(true);
		this->nodeChuY->setVisible(true);
		currTab = 1;
		this->tblPhanThuong->reloadData();
		this->lblChuy->setString("- Hệ thống sẽ duyệt và xử lý đổi thẻ, vật phẩm liên tục. Chi tiết Click Button Quy định đổi thưởng.");
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
			this->lblChuy->setString("- Vip 1 trở lên mới được đổi thưởng.\n- Chi tiết xem tại nút \"Quy định đổi thưởng\".");
		}

	}
}
void LayerDoiThuong::onBtnVatPham(Ref* pSender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		if (this->getParent() == NULL) return;
		this->btnVatPham->setBright(false);
		this->btnVatPham->setEnabled(false);

		// this->btnLichSu->setBright(true);
		this->btnLichSu->setEnabled(true);
		this->btnTheCao->setEnabled(true);

		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
			/*this->btnVatPham->setTitleColor(Color3B::BLACK);
			this->btnTheCao->setTitleColor(Color3B::WHITE);
			this->btnLichSu->setTitleColor(Color3B::WHITE);*/
		}
		this->removeChildByTag(24);
		this->tblPhanThuong->setVisible(true);
		this->nodeChuY->setVisible(true);
		currTab = 2;
		this->tblPhanThuong->reloadData();
		this->lblChuy->setString("- Vui lòng check Hòm thư để xem thông tin trao vật phẩm.");
	}
}

void LayerDoiThuong::loadTheCao() {
	this->listTheCaos.clear();
	for (int i = 1; i <= 3; i++) {

		TheCao theCao1;
		theCao1.type = i;
		theCao1.cost = 60000;
		theCao1.menhgia = 50000;

		this->listTheCaos.push_back(theCao1);

		TheCao theCao2;
		theCao2.type = i;
		theCao2.cost = 120000;
		theCao2.menhgia = 100000;

		this->listTheCaos.push_back(theCao2);

		TheCao theCao3;
		theCao3.type = i;
		theCao3.cost = 240000;
		theCao3.menhgia = 200000;

		this->listTheCaos.push_back(theCao3);

		TheCao theCao4;
		theCao4.type = i;
		theCao4.cost = 600000;
		theCao4.menhgia = 500000;

		this->listTheCaos.push_back(theCao4);

	}

	//this->tblPhanThuong->reloadData();
}
bool LayerDoiThuong::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
	return true;
}
void LayerDoiThuong::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerDoiThuong::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void LayerDoiThuong::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(listener);
	Layer::onExit();
}

void LayerDoiThuong::onDoiThuongClick(Ref* pSender){
	auto node = (Node*)pSender;
	int tag = node->getTag();

	if (currTab == 1){
		string strNhaMang = "";
		if (tag >= listTheCaos.size())
			return;
		switch (this->listTheCaos[tag].type){
		case 1:
			strNhaMang = "Vina";
			break;
		case 2:
			strNhaMang = "Mobi";
			break;
		case 3:
			strNhaMang = "Viettel";
			break;
		}

		boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
		if (myself != NULL){
			if (myself->GetVariable("amf") != NULL)
			{
				if (myself->GetVariable("lvl") != NULL){

					auto level = myself->GetVariable("lvl")->GetIntValue();
					if (level != NULL){
						if (*level < SceneManager::getSingleton().getMinMoney(4)){
							if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
								LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
								layerPopup->showAButton();
								layerPopup->setMessage(dataManager.GetSysString(141));
								layerPopup->setContentMess(StringUtils::format("Level %d trở lên mới được đổi thưởng!", SceneManager::getSingleton().getMinMoney(4)));
								layerPopup->setTag(24);
								this->addChild(layerPopup);
								return;
							}
						}
					}
				}
				if (myself->GetVariable("amf") != NULL){
					auto myMoney = myself->GetVariable("amf")->GetDoubleValue();
					if (myMoney != NULL){
						if (*myMoney < this->listTheCaos[tag].cost){
							LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
							layerPopup->showAButton();
							layerPopup->setMessage(dataManager.GetSysString(141));
							layerPopup->setContentMess("Bạn không đủ tiền để mua thẻ cào này!");
							layerPopup->setTag(24);
							this->addChild(layerPopup);
							return;
						}
						else if ((*myMoney - this->listTheCaos[tag].cost) < SceneManager::getSingleton().getMinMoney(1)){
							if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){

								LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
								layerPopup->showAButton();
								layerPopup->setMessage(dataManager.GetSysString(141));
								layerPopup->setContentMess(StringUtils::format("Số tiền của bạn sau khi đổi thẻ này phải lớn hơn %s %s!", mUtils::format_money((long)SceneManager::getSingleton().getMinMoney(1))->c_str(), SceneManager::getSingleton().getCoin(0).c_str()));
								layerPopup->setTag(24);
								this->addChild(layerPopup);
								return;
							}
						}

					}
				}
			}
		}


		LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
		layerPopup->setCallBackFromButton(this, callfuncO_selector(LayerDoiThuong::popUpCallBack), tag);
		layerPopup->setMessage(dataManager.GetSysString(141));
		layerPopup->setContentMess(StringUtils::format("Bạn muốn đổi %s %s lấy thẻ %s %s không?", mUtils::format_money(this->listTheCaos[tag].cost)->c_str(), SceneManager::getSingleton().getCoin(0).c_str(), strNhaMang.c_str(), mUtils::format_money(this->listTheCaos[tag].menhgia)->c_str()));
		layerPopup->setTag(24);
		this->addChild(layerPopup);
	}
	else{
		boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
		if (myself != NULL){
			if (myself->GetVariable("amf") != NULL)
			{
				if (myself->GetVariable("lvl") != NULL){
					auto level = myself->GetVariable("lvl")->GetIntValue();
					if (level != NULL){
						if (*level < SceneManager::getSingleton().getMinMoney(4)){
							if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){

								LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
								layerPopup->showAButton();
								layerPopup->setMessage(dataManager.GetSysString(141));
								layerPopup->setContentMess(StringUtils::format("Level %d trở lên mới được đổi thưởng!", SceneManager::getSingleton().getMinMoney(4)));
								layerPopup->setTag(24);
								this->addChild(layerPopup);
								return;
							}
						}
					}
				}
				if (myself->GetVariable("amf") != NULL){
					auto myMoney = myself->GetVariable("amf")->GetDoubleValue();
					if (myMoney != NULL){
						if (*myMoney < this->listVatPhams[tag].cost){
							LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
							layerPopup->showAButton();
							layerPopup->setMessage(dataManager.GetSysString(141));
							layerPopup->setContentMess("Bạn không đủ tiền để mua vật phẩm này!");
							layerPopup->setTag(24);
							this->addChild(layerPopup);
							return;
						}
						else if ((*myMoney - this->listVatPhams[tag].cost) < SceneManager::getSingleton().getMinMoney(1)){
							if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){

								LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
								layerPopup->showAButton();
								layerPopup->setMessage(dataManager.GetSysString(141));
								layerPopup->setContentMess(StringUtils::format("Số tiền của bạn sau khi mua vật phẩm này phải lớn hơn %s %s!", mUtils::format_money((long)SceneManager::getSingleton().getMinMoney(1))->c_str(), SceneManager::getSingleton().getCoin(0).c_str()));
								layerPopup->setTag(24);
								this->addChild(layerPopup);
								return;
							}
						}
					}
				}
			}
		}

		LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
		layerPopup->setCallBackFromButton(this, callfuncO_selector(LayerDoiThuong::popUpCallBack), tag);
		layerPopup->setMessage(dataManager.GetSysString(141));
		layerPopup->setContentMess(StringUtils::format("Bạn muốn đổi %s %s lấy %s không?", mUtils::format_money(this->listVatPhams[tag].cost)->c_str(), SceneManager::getSingleton().getCoin(0).c_str(), listVatPhams[tag].name.c_str()));
		layerPopup->setTag(24);
		this->addChild(layerPopup);
	}
}
void LayerDoiThuong::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	this->removeFromParentAndCleanup(true);
}
void LayerDoiThuong::popUpCallBack(Ref* pSender){
	if (currTab == 1){
		if (dataManager.getAppConfig().isactiveOTP){
			auto node = (Node*)pSender;
			tagVatPham = node->getTag();
			LayerPopupXacThucOTP* popup = LayerPopupXacThucOTP::create();
			popup->setCallBackFromButton(this, callfuncND_selector(LayerDoiThuong::onXacThucOTP));
			popup->setTag(29);
			this->addChild(popup);

		}
		else{
			int osid = 1;
			string devID = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			osid = 1;
            if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
                devID = dataManager.getAppConfig().tientomocp2 + NativeHelper::getDeviceId();
            else
                devID = NativeHelper::getDeviceId();
			/*isConnectting = NativeHelper::hasConnection();*/
			//devID = NativeHelper::getDeviceID();

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			osid = 2;
            if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
                devID = dataManager.getAppConfig().tientomocp2 + IOS::getDeviceID();
            else
                devID = IOS::getDeviceID();
#endif
			auto node = (Node*)pSender;
			tagVatPham = node->getTag();
			boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutUtfString("aN", myself->Name());
			params->PutUtfString("issuer", StringUtils::format("%d", listTheCaos[tagVatPham].type).c_str());
			params->PutInt("vv", this->listTheCaos[tagVatPham].menhgia);
			params->PutInt("vnc", 1);
			params->PutUtfString("voucherDesc", "");
			params->PutInt("osid", osid);
			params->PutUtfString("devid", devID);
			boost::shared_ptr<IRequest> request(new ExtensionRequest("rgb", params));
			GameServer::getSingleton().Send(request);
			if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				NativeHelper::trackDoiThuong(StringUtils::format("%d", this->listTheCaos[tagVatPham].menhgia).c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				IOS::trackDoiThuong(StringUtils::format("%d",this->listTheCaos[tagVatPham].menhgia));

#endif
			}
		}
		this->removeChildByTag(24, true);
	}
	else{

		if (dataManager.getAppConfig().isactiveOTP){
			LayerPopupXacThucOTP* popup = LayerPopupXacThucOTP::create();
			popup->setCallBackFromButton(this, callfuncND_selector(LayerDoiThuong::onXacThucOTP));
			popup->setTag(29);
			auto node = (Node*)pSender;
			tagVatPham = node->getTag();
			this->addChild(popup);

		}
		else{
			auto node = (Node*)pSender;
			tagVatPham = node->getTag();
			boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutUtfString("aN", myself->Name());
			params->PutInt("it", this->listVatPhams[tagVatPham].id);
			boost::shared_ptr<IRequest> request(new ExtensionRequest("regl", params));
			GameServer::getSingleton().Send(request);

			if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				NativeHelper::trackDoiThuong(StringUtils::format("%d", this->listTheCaos[tagVatPham].menhgia).c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				IOS::trackDoiThuong(StringUtils::format("%d",this->listTheCaos[tagVatPham].menhgia));

#endif

			}


		}
		this->removeChildByTag(24, true);

	}
}
void LayerDoiThuong::onXacThucOTP(Node* sender, void* data)
{
	string *value = (string*)data;
	string otp = *value;
	if (currTab == 1){
		int osid = 1;
		string devID = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		osid = 1;
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
            devID = dataManager.getAppConfig().tientomocp2 + NativeHelper::getDeviceId();
        else
            devID = NativeHelper::getDeviceId();
		/*isConnectting = NativeHelper::hasConnection();*/
		//devID = NativeHelper::getDeviceID();

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		osid = 2;
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
            devID = dataManager.getAppConfig().tientomocp2 + IOS::getDeviceID();
        else
            devID = IOS::getDeviceID();
#endif
		boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutUtfString("aN", myself->Name());
		params->PutUtfString("otpcode", otp);
		params->PutUtfString("issuer", StringUtils::format("%d", listTheCaos[tagVatPham].type).c_str());
		params->PutInt("vv", this->listTheCaos[tagVatPham].menhgia);
		params->PutInt("vnc", 1);
		params->PutUtfString("voucherDesc", "");
		params->PutInt("osid", osid);
		params->PutUtfString("devid", devID);
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rgb", params));
		GameServer::getSingleton().Send(request);
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			NativeHelper::trackDoiThuong(StringUtils::format("%d", this->listTheCaos[tagVatPham].menhgia).c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			IOS::trackDoiThuong(StringUtils::format("%d",this->listTheCaos[tagVatPham].menhgia));

#endif
		}
	}
	else{
		boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutUtfString("aN", myself->Name());
		params->PutUtfString("otpcode", otp);
		params->PutInt("it", this->listVatPhams[tagVatPham].id);
		boost::shared_ptr<IRequest> request(new ExtensionRequest("regl", params));
		GameServer::getSingleton().Send(request);

		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			NativeHelper::trackDoiThuong(StringUtils::format("%d", this->listTheCaos[tagVatPham].menhgia).c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			IOS::trackDoiThuong(StringUtils::format("%d",this->listTheCaos[tagVatPham].menhgia));

#endif

		}
	}

	this->removeChildByTag(29);
}

void LayerDoiThuong::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerDoiThuong::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
		if (currTab == 1){
			auto _child_ = LayerItemDoiThuong::create();
			return Size(table->getContentSize().width, _child_->getContentSize().height);
		}
		else{
			return Size(table->getContentSize().width, 300);
		}
	}
	if (currTab == 1){
		auto _child_ = LayerItemDoiThuong::create();
		return Size(table->getContentSize().width, _child_->getContentSize().height + 30);
	}
	else{
		return Size(table->getContentSize().width, 330);
	}
}

TableViewCell* LayerDoiThuong::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto cell = table->dequeueCell();
	auto _cTag = 123;
	if (currTab == 1){
		if (!cell) {
			cell = TableViewCell::create();

		}
		else {
			cell->removeAllChildren();
		}

		if (idx < 0)
			return cell;

		auto space = 30;
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
			space = 25;
		}
		auto cols = 4;
		auto _child_ = LayerItemDoiThuong::create();
		auto sizeTable = this->bg->getContentSize();
		auto width = _child_->getContentSize().width;
		auto startX = (sizeTable.width - (_child_->getContentSize().width)*cols - space * (cols - 1)) / 2;

		//comment
		for (int i = 0; i < cols; i++) {
			int index = idx *cols + i;
			if (index < this->listTheCaos.size())
			{
				auto _child = LayerItemDoiThuong::create();
				//  _child.onPlayRoomCallBack(this.playRoomCallBack, this);
				_child->onDoiThuongCallBack(this, callfuncO_selector(LayerDoiThuong::onDoiThuongClick));
				_child->setPosition(startX + (i * width) + space * i, 0);
				//  _child.setDatas(this.listTheCaos[idx].type,this.listTheCaos[idx].menhgia,this.listTheCaos[idx].cost);
				cell->addChild(_child, 0, _cTag + i);
				_child->setDatas(this->listTheCaos[index].type, this->listTheCaos[index].menhgia, this->listTheCaos[index].cost);
				_child->setVisible(true);
				_child->setTag(index);
			}
		}


		//        int cols = 4;
		//        int i = 0;
		//        for (i = 0; i < cols; i++) {
		//
		//                LayerItemDoiThuong* _child = (LayerItemDoiThuong*)cell->getChildByTag(_cTag + i);
		//                if (NULL != _child) {
		//                    _child->onDoiThuongCallBack(this, callfuncO_selector(LayerDoiThuong::onDoiThuongClick));
		//                    _child->setDatas(this->listTheCaos[index].type, this->listTheCaos[index].menhgia, this->listTheCaos[index].cost);
		//                    _child->setVisible(true);
		//                    _child->setTag(index);
		//                }
		//            }
		//            else{
		//                auto _child = cell->getChildByTag(_cTag + i);
		//                if (NULL != _child) {
		//                    _child->setVisible(false);
		//                }
		//            }
		//
		//        }
		//        for (int j = i; j < cols; j++) {
		//            auto _child = cell->getChildByTag(_cTag + j);
		//            if (NULL != _child) {
		//                _child->setVisible(false);
		//            }
		//        }

	}
	else{
		if (!cell) {
			cell = TableViewCell::create();
		}
		else {
			cell->removeAllChildren();
		}
		if (idx < 0)
			return cell;
		auto cols = 4;
		auto _child_ = LayerItemVatPham::create();
		auto sizeTable = this->bg->getContentSize();
		auto width = _child_->getContentSize().width;
		auto startX = (sizeTable.width - (_child_->getContentSize().width)*cols - 30 * (cols - 1)) / 2;

		//comment
		for (int i = 0; i < cols; i++) {
			int index = idx *cols + i;
			if (index < this->listVatPhams.size())
			{
				auto _child = LayerItemVatPham::create();
				_child->onDoiThuongCallBack(this, callfuncO_selector(LayerDoiThuong::onDoiThuongClick));
				//  _child.onPlayRoomCallBack(this.playRoomCallBack, this);
				_child->setPosition(startX + (i * width) + 30 * i, 0);
				//  _child.setDatas(this.listTheCaos[idx].type,this.listTheCaos[idx].menhgia,this.listTheCaos[idx].cost);
				cell->addChild(_child, 0, _cTag + i);
				_child->setDatas(this->listVatPhams[index].img, this->listVatPhams[index].name, this->listVatPhams[index].cost);
				_child->setVisible(true);
				_child->setTag(index);
			}
		}
	}
	return cell;

}

ssize_t LayerDoiThuong::numberOfCellsInTableView(TableView *table)
{
	if (currTab == 1){
		if (this->listTheCaos.size() == 0)
			return 0;
		if (this->listTheCaos.size() < 4)
			return 1;
		auto rows = this->listTheCaos.size() / 4;
		if (this->listTheCaos.size() % 4 != 0)
			rows += 1;
		return rows;
	}
	else{
		if (this->listVatPhams.size() == 0)
			return 0;
		if (this->listVatPhams.size() < 4)
			return 1;
		auto rows = this->listVatPhams.size() / 4;
		if (this->listVatPhams.size() % 4 != 0)
			rows += 1;
		return rows;
	}
}
void LayerDoiThuong::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));

	if (strcmp("tgbresp", cmd->c_str()) == 0){
		int rc = *param->GetInt("rc");
		auto rd = *param->GetUtfString("rd");
		if (rc == 0) {
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setMessage(dataManager.GetSysString(141));
			layerPopup->setContentMess("Bạn đã đăng ký đổi thẻ thành công. Chúng tôi sẽ xử lý và gửi thẻ về hòm thư của bạn trong thời gian sớm nhất!");
			layerPopup->setTag(24);
			layerPopup->showAButton();
			this->addChild(layerPopup);
		}
		else {
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setMessage(dataManager.GetSysString(141));
			layerPopup->setContentMess(rd + "");
			layerPopup->setTag(24);
			layerPopup->showAButton();
			this->addChild(layerPopup);

		}
	}
	else if (strcmp("respcf", cmd->c_str()) == 0){ //RESP_GET_CONFIG = "respcf";
		mTax = *param->GetDouble("cfv");//CONFIG_VALUE = "cfv";
		//recalculator
		for (int i = 0; i < listTheCaos.size(); i++){
			listTheCaos.at(i).cost = listTheCaos.at(i).menhgia*mTax;
		}
		tblPhanThuong->reloadData();
		//
		SceneManager::getSingleton().hideLoading();
	}
	else if (strcmp("rgl", cmd->c_str()) == 0){//RESP_GIFT_LIST = "rgl";
		if (param->GetUtfString("glis") == NULL) return;
		auto param_str = *param->GetUtfString("glis");
		if (param_str == "") return;
		auto items = mUtils::splitString(param_str, '|');
		for (int i = 0; i < items.size(); i++){
			VatPham vatpham;
			if (items.at(i) == "") return;
			auto infos = mUtils::splitString(items.at(i), '@');
			for (int j = 0; j < infos.size(); j++){
				vatpham.id = atoi(infos.at(0).c_str());
				vatpham.name = infos.at(1);
				vatpham.desc = infos.at(2);
				vatpham.img = infos.at(3);
				vatpham.cost = atoi(infos.at(4).c_str());
			}
			this->listVatPhams.push_back(vatpham);
		}
		SceneManager::getSingleton().hideLoading();
	}
	else  if (strcmp("rgr", cmd->c_str()) == 0){
		int rc = *param->GetInt("rc");
		auto rd = *param->GetUtfString("rd");
		if (rc == 0) {
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setMessage(dataManager.GetSysString(141));
			layerPopup->setContentMess("Bạn đã đăng ký đổi vật phẩm thành công. Chúng tôi sẽ xử lý và gửi thông tin nhận thưởng về hòm thư của bạn trong thời gian sớm nhất!");
			layerPopup->setTag(24);
			layerPopup->showAButton();
			this->addChild(layerPopup);
		}
		else {
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setMessage(dataManager.GetSysString(141));
			layerPopup->setContentMess(rd + "");
			layerPopup->setTag(24);
			layerPopup->showAButton();
			this->addChild(layerPopup);

		}
	}
	else if (strcmp("notiRMFF", cmd->c_str()) == 0){
		if (currTab == 3)
			onBtnTheCao(NULL, ui::Widget::TouchEventType::ENDED);

	}
	else if (strcmp("ire", cmd->c_str()) == 0){

		boost::shared_ptr<string> token = param->GetUtfString("ire");
		if (token == NULL)
			return;
		log("Tocken Upload = %s", token->c_str());
		SceneManager::getSingleton().hideLoading();

		string myName = SceneManager::getSingleton().getMyName();
		string url = "";
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		}
		else{
			url = dataManager.getAppConfig().urllinksdt;
		}
		url += myName + "&checksum=" + mUtils::getMD5KeyHis(myName) + "&tk=" + *token;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		auto webView = cocos2d::experimental::ui::WebView::create();
		webView->setAnchorPoint(this->bg->getAnchorPoint());
		webView->setPosition(this->bg->getPosition());
		webView->setContentSize(Size(WIDTH_DESIGN,this->bg->getContentSize().height));

		webView->loadURL(url);
		webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerDoiThuong::onWebViewShouldStartLoading, this));
		webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerDoiThuong::onWebViewDidFinishLoading, this));
		webView->setOnDidFailLoading(CC_CALLBACK_2(LayerDoiThuong::onWebViewDidFailLoading, this));
		//webView->setContentSize(Size(lsvHistories->getContentSize().width * ratio, lsvHistories->getContentSize().height * ratio));
		webView->setScalesPageToFit(true);
		this->addChild(webView);
		webView->setVisible(false);
		webView->setTag(24);
		this->tblPhanThuong->setVisible(false);
		this->nodeChuY->setVisible(false);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		Application::getInstance()->openURL(url);
#endif

	}
}

void LayerDoiThuong::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);

	if (ptrNotifiedUser->IsItMe() == true){
		boost::shared_ptr<double> amf;
		boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
		if (myself == NULL)
			return;
		amf = myself->GetVariable("amf")->GetDoubleValue();
		lblMoney->setString(mUtils::convertMoneyEx(*amf).c_str());
	}
}

void LayerDoiThuong::loadAllDatas()
{
	SceneManager::getSingleton().showLoading();
	//
	boost::shared_ptr<ISFSObject> params(new SFSObject());

	params->PutInt("cfk", 15);//CONFIG_KEY = "cfk"; 15: lấy tỉ giá qui đổi ngược

	boost::shared_ptr<IRequest> request(new ExtensionRequest("reqgcf", params)); //REQUEST_GET_CONFIG = "reqgcf";
	GameServer::getSingleton().Send(request);

	boost::shared_ptr<ISFSObject> params1(new SFSObject());

	boost::shared_ptr<IRequest> request1(new ExtensionRequest("ggl", params1)); //GET_GIFT_LIST = "reqgcf";
	GameServer::getSingleton().Send(request1);
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerDoiThuong::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().showLoading();
	sender->setVisible(false);
	return true;
}

void LayerDoiThuong::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
	sender->setVisible(true);
}

void LayerDoiThuong::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
}

#endif

bool LayerItemDoiThuong::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto root = CSLoader::getInstance()->createNode("LayerItemDoiThuong.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);

	this->setContentSize(root->getContentSize());

	this->sprNhaMang = static_cast<Sprite*>(root->getChildByName("sprNhaMang"));

	this->lblMoney = static_cast<Text*>(root->getChildByName("lblMoney"));
	this->lblMoneyReceive = static_cast<Text*>(root->getChildByName("lblMoneyReceive"));
	this->btnDoiThuong = static_cast<Button*>(root->getChildByName("btnDoiThuong"));
	if (this->btnDoiThuong)
	{
		btnDoiThuong->setSwallowTouches(false);
		this->btnDoiThuong->addTouchEventListener(CC_CALLBACK_2(LayerItemDoiThuong::onBtnDoiThuong, this));
	}

	return true;
}
void LayerItemDoiThuong::setDatas(int loaiMang, int txtMoneyReceive, int txtMoney){
	this->lblMoneyReceive->setString(mUtils::format_money(txtMoney)->c_str());
	this->lblMoney->setString(mUtils::format_money(txtMoneyReceive)->c_str());
	string srcIMG = "";
	switch (loaiMang){
	case 1:
		srcIMG = "dt-vina.png";
		break;
	case 2:
		srcIMG = "dt-mobi.png";
		break;
	case 3:
		srcIMG = "dt-viettel.png";
		break;
	}

	this->sprNhaMang->setTexture(srcIMG);
}
void LayerItemDoiThuong::onDoiThuongCallBack(Ref* target, SEL_CallFuncO listener){
	mCallBack = target;
	mCallBackListener = listener;
}
void LayerItemDoiThuong::onBtnDoiThuong(Ref* pSender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		if (mCallBack && mCallBackListener){
			//mCallBack
			Node* node = Node::create();
			node->setTag(this->getTag());
			(mCallBack->*mCallBackListener)(node);
		}
	}
}

bool LayerItemVatPham::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto root = CSLoader::getInstance()->createNode("LayerItemVatPham.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);

	this->setContentSize(root->getContentSize());

	this->imgVatPham = dynamic_cast<ImageView*>(root->getChildByName("imgVatPham"));

	this->lblCost = static_cast<Text*>(root->getChildByName("lblValueVatPham"));
	this->lblName = static_cast<Text*>(root->getChildByName("lblNameVatPham"));
	this->btnDoiThuong = static_cast<Button*>(root->getChildByName("btnItemVatPham"));
	if (this->btnDoiThuong)
	{
		btnDoiThuong->setSwallowTouches(false);
		this->btnDoiThuong->addTouchEventListener(CC_CALLBACK_2(LayerItemVatPham::onBtnDoiThuong, this));
	}
	return true;
}
void LayerItemVatPham::setDatas(string urlVatpham, string txtName, int txtCost){
	this->imgVatPham->setVisible(false);
	this->lblName->setString(txtName);
	this->lblCost->setString((mUtils::format_money(txtCost)->c_str()));
	Node* clipper = Node::create();
	Sprite* gameTitle = Sprite::create("icon_default.png");
	clipper->setPosition(this->imgVatPham->getPosition());
	clipper->setContentSize(Size(gameTitle->getContentSize().width, gameTitle->getContentSize().height));
	clipper->retain();
	this->addChild(clipper);
	SceneManager::getSingleton().getImageDownloadProcess()->downLoadImage(clipper, urlVatpham);
}
void LayerItemVatPham::onDoiThuongCallBack(Ref* target, SEL_CallFuncO listener){
	mCallBack = target;
	mCallBackListener = listener;
}
void LayerItemVatPham::onBtnDoiThuong(Ref* pSender, ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		if (mCallBack && mCallBackListener){
			//mCallBack
			Node* node = Node::create();
			node->setTag(this->getTag());
			(mCallBack->*mCallBackListener)(node);
		}
	}
}
