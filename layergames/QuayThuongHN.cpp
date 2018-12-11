#include "QuayThuongHN.h"
#include "data_manager.h"
#include "mUtils.h"
#include "PhomMessDef.h"
#include "SceneManager.h"
#include "ChanUtils.h"
#include "Avatar52La.h"
#include "../layers/LayerPopupInfo.h"
#include "layers/LayerPopupThongBao.h"
#include "QuayThuong.h"
#include "layers/LayerPopupRecharge.h"
#include "scenes/SceneMain.h"
#define TIME_SHOW_RESULT 5

QuayThuongHN::QuayThuongHN()
{
	log("QuayThuongHN");
	luotquay = 0;
	pSprite = NULL;
	GameServer::getSingleton().addListeners(this);
}
QuayThuongHN::~QuayThuongHN()
{
	//MINHDV - changes - request get num spin to set callback response for layer main bottom
	this->requestNumspin();
	//show popup event
	auto currScene = Director::getInstance()->getRunningScene();
	if (currScene != NULL)
	{
		SceneMain* sceneMain = dynamic_cast<SceneMain*> (currScene->getChildByTag(2468));
		if (sceneMain && dataManager.getAppConfig().isLoginFisrtDay)
		{
			sceneMain->showEvent();
		}
	}

	GameServer::getSingleton().removeListeners(this);
}

bool QuayThuongHN::init()
{
	if (!Layer::init())
		return false;
	visibleSize = Director::getInstance()->getVisibleSize();
	//createSmartFoxEvent();
	createListBet();
	createBackground();
	createButtons();

	return true;
}

void QuayThuongHN::createBackground()
{
	int dis_value = 100;
	int add_value = 50;
	string bg_img = "sam-3cay-bg.jpg";
	Sprite* bg = Sprite::create(bg_img);
	float scaleY = visibleSize.width / HEIGHT_DESIGN;
	bg->setScaleY(scaleY);
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	Sprite* shadow = Sprite::create("vongquay-shadow.png");
	shadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	shadow->setPosition(Vec2(visibleSize.width / 2 + add_value, visibleSize.height / 2));
	this->addChild(shadow);

	pSprite = Sprite::create("vongquay.png");
	pSprite->setPosition(Vec2(visibleSize.width / 2 + add_value, visibleSize.height / 2));
	pSprite->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(pSprite);

	Sprite *arrowCircle = Sprite::create("muiten_hn.png");
	arrowCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	arrowCircle->setPosition(Vec2(visibleSize.width / 2 + add_value, visibleSize.height / 2 + pSprite->getContentSize().height / 2.7 + 60));
	this->addChild(arrowCircle);
	//create label result
	labelResult = Label::createWithTTF("", kBoldFont, 60);
	labelResult->setHorizontalAlignment(TextHAlignment::LEFT);
	labelResult->setPosition(visibleSize.width / 2 + pSprite->getContentSize().width / 1.4 + add_value, visibleSize.height / 2);
	this->addChild(labelResult);

	Label *label1 = Label::createWithTTF("Phần thưởng:", kNormalFont, 50);
	label1->setHorizontalAlignment(TextHAlignment::LEFT);
	label1->setPosition(labelResult->getPositionX(), labelResult->getPositionY() + dis_value);
	this->addChild(label1);

	Label *label2 = Label::createWithTTF("Lưu ý:\nVip 1 trở lên được\n quay thưởng mỗi ngày 1 lần.\nChi tiết xem tại nút hướng dẫn", kNormalFont, 40);
	label2->setHorizontalAlignment(TextHAlignment::CENTER);
	label2->setPosition(visibleSize.width / 2 + pSprite->getContentSize().width / 1.4 + add_value + 20,
		labelResult->getPositionY() - dis_value - 40);
	label2->setColor(Color3B::ORANGE);
	this->addChild(label2);

	layerBaoDanh = LayerBaoDanh::create();
	layerBaoDanh->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	layerBaoDanh->setPosition(Vec2(visibleSize.width / 2 - pSprite->getContentSize().width / 1.4 - 30, visibleSize.height / 2));
	this->addChild(layerBaoDanh);
}

void QuayThuongHN::createButtons()
{
	int add_value = 50;
	btnArrow = Button::create("roulette-ap.png", "disable-roulette-ap.png");
	btnArrow->setPosition(Vec2(visibleSize.width / 2 + add_value, visibleSize.height / 2));
	btnArrow->setAnchorPoint(Vec2(0.5, 0.5));
	btnArrow->addTouchEventListener(CC_CALLBACK_2(QuayThuongHN::onButtonArrow, this));
	this->addChild(btnArrow);

	int widButton2 = 123;
	int heiButton = 123;
	int dis = 30;
	int w_coin = 82;
	float ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;

	Size sizeAdd = ChanUtils::getSizePos();
	Size winSize = Director::getInstance()->getVisibleSize();

	Button *btnBack = Button::create("moc3-btn-thoat.png", "", "");
	btnBack->setScale9Enabled(false);
	btnBack->setAnchorPoint(Vec2(0, 0));
	btnBack->setPosition(Vec2(dis, visibleSize.height - heiButton));
	btnBack->addTouchEventListener(CC_CALLBACK_2(QuayThuongHN::onButtonBack, this));
	this->addChild(btnBack);

	/*Button* btnNapTien = Button::create("button_coin.png", "", "");
	btnNapTien->setScale9Enabled(false);
	btnNapTien->setAnchorPoint(Vec2(0, 0));
	btnNapTien->setPosition(Vec2(dis, dis));
	btnNapTien->addTouchEventListener(CC_CALLBACK_2(QuayThuongHN::onButtonCoin, this));
	this->addChild(btnNapTien);*/
}

void QuayThuongHN::onButtonArrow(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		// 		// 		KetQuaQuayThuongHN* layerketqua = KetQuaQuayThuongHN::create();
		// 		// 		layerketqua->setMoney(listBet.at(8));
		// 		// 		this->addChild(layerketqua);
		// 		// 		return;
		// 		if (luotquay == 0)
		// 		{
		// 			this->showToast("Bạn đã hết lượt quay \n Vui lòng mua thêm");
		// 			return;
		// 		}
		// 		btnArrow->setEnabled(false);
		// 		boost::shared_ptr<ISFSObject> params(new SFSObject());
		// 		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		// 		if (lastRoom == NULL)
		// 			return;
		// 		boost::shared_ptr<IRequest> request(new ExtensionRequest("grreq", params, lastRoom));
		// 
		// 		GameServer::getSingleton().Send(request);
		// 		int result = rand() % 9;
		// 		startRotating(result);
		// 		this->startRotating(10);
		// 		return;
		if (luotquay <= 0)
		{
			//hien thong bao cho vip 0 "Vip 1 trở lên được quay thưởng hàng ngày"
			boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
			int acctype = -1;
			if (mySelf != NULL)
			{
				boost::shared_ptr<UserVariable> acctype_ptr = mySelf->GetVariable("accVipType");
				if (acctype_ptr != NULL)
					acctype = *acctype_ptr->GetIntValue();
			}
			if (acctype == 0 && isLoginFirstDay)
			{
				//isLoginFirst = false;
				showToast("Vip 1 trở lên được quay thưởng hàng ngày");
				return;
			}

			showToast("Bạn đã hết lượt quay hôm nay");
			return;
		}
		btnArrow->setEnabled(false);
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_DO_SPIN, params));
		GameServer::getSingleton().Send(request);
	}
}

void QuayThuongHN::actionRotateDone()
{
	this->requestListAttend();
	labelResult->setString(listBet.at(prize).text);

	auto actionScale = ScaleBy::create(0.6, 1.2, 1.2);
	auto resultaction = Sequence::create(actionScale, actionScale->reverse(), NULL);
	labelResult->runAction(Repeat::create(resultaction, 3));

	btnArrow->setEnabled(true);
	KetQuaQuayThuong* layerketqua = KetQuaQuayThuong::create();
	layerketqua->setResult(listBet.at(prize).text);
	this->addChild(layerketqua);
	//log("PHAN THUONG CUA BAN LA %d", listBet.at(prize));
}

void QuayThuongHN::showToast(string mess)
{
	int add_value = 50;
	Chat *toast = Chat::create(mess, -1);
	toast->setPositionX(toast->getPositionX() + add_value);
	this->addChild(toast, 1000);
}

void QuayThuongHN::layerPopupThongBaoCallBack(Ref *pSender)
{
	int	tag = ((Node*)pSender)->getTag();
	//tien lay theo tag button
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutInt("am", tag);
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest("btreq", params, lastRoom));

	GameServer::getSingleton().Send(request);
	this->removeChildByTag(24);
}

void QuayThuongHN::createLayerNotification(string title, string content_mess, int tag)
{
	LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
	layerPopup->setCallBackFromButton(this, callfuncO_selector(QuayThuongHN::layerPopupThongBaoCallBack), tag);
	layerPopup->setMessage(title);
	layerPopup->setContentMess(content_mess);
	layerPopup->setTag(24);
	this->addChild(layerPopup);
}

void QuayThuongHN::startRotating(int resultBet)
{
	btnArrow->setEnabled(false);
	int rounds = rand() % 20 + 10;
	int slices = listBet.size();
	for (int i = 0; i < listBet.size(); i++)
	{
		if (listBet.at(i).id == resultBet)
		{
			prize = i;
			break;
		}
	}
	int start_rand = 3;
	int rand_space = 30;
	int dis_value = 1;
	if (gameVersion == GameVersion::p2)
	{
		rand_space = 10;
		dis_value = 0;
		start_rand = -5;
	}
	int rand_angle_bonus = rand() % rand_space + start_rand;
	float degress = (slices - dis_value - prize)*(360 / slices);
	log("DEGRESS %.3f", degress);
	float angle = 360 * rounds + degress + rand_angle_bonus; //goc quay
	ActionInterval *rotate = RotateTo::create(10, angle);
	//EaseOut *easeout = EaseOut::create(rotate, 6);
	EaseCircleActionOut *easeout = EaseCircleActionOut::create(rotate->clone());
	pSprite->runAction(Sequence::create(easeout,
		DelayTime::create(0.3),
		CallFunc::create(CC_CALLBACK_0(QuayThuongHN::actionRotateDone, this)), nullptr));
}

void QuayThuongHN::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(QuayThuongHN::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void QuayThuongHN::onExit()
{
	Layer::onExit();
	//_eventDispatcher->removeEventListenersForTarget(circleAvatar);
}

void QuayThuongHN::createListBet()
{
	listBet.push_back(SliceInfo(7, "100 EXP"));
	listBet.push_back(SliceInfo(4, "500 "+SceneManager::getSingleton().getCoin(0)));
	listBet.push_back(SliceInfo(1, "5% Thẻ nạp"));
	listBet.push_back(SliceInfo(8, "200 EXP"));
	listBet.push_back(SliceInfo(5, "1.000 Vàng"));
	listBet.push_back(SliceInfo(2, "7 % Thẻ nạp"));
	listBet.push_back(SliceInfo(9, "500 EXP"));
	listBet.push_back(SliceInfo(6, "2.000 Vàng"));
	listBet.push_back(SliceInfo(3, "10 % Thẻ nạp"));
	listBet.push_back(SliceInfo(10, "Thêm lượt"));
}

bool QuayThuongHN::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void QuayThuongHN::onButtonBack(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void QuayThuongHN::onButtonCoin(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		LayerPopupRecharge* _child = LayerPopupRecharge::create();
		this->addChild(_child);
	}
}

Scene* QuayThuongHN::createScene()
{
	auto scene = Scene::create();
	QuayThuongHN* quaythuongHN = QuayThuongHN::create();
	scene->addChild(quaythuongHN);
	return scene;
}

void QuayThuongHN::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (ptrNotifiedCmd == NULL)
		return;
	if (0 == ptrNotifiedCmd->compare(EXT_EVENT_RES_DO_SPIN))
	{
		boost::shared_ptr<long> ptr_reward = param->GetInt(EXT_FIELD_REWARD_ID);
		boost::shared_ptr<long> ptr_numspin = param->GetInt(EXT_FIELD_NUM_SPIN);
		if (ptr_reward)
		{
			int id = *ptr_reward;
			if (id != -1)
				startRotating(id);
		}
		if (ptr_numspin)
		{
			luotquay = *ptr_numspin;
		}
	}
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_LIST_ATTEND_RES))
	{
		//{"encrypts":"","numattend":1,"listattend":"0_0_0_0_0_0_1","rc":0}
		//layerBaoDanh->setListBaoDanh()
		boost::shared_ptr<long> numattend_ptr = param->GetInt(EXT_FILED_NUM_ATTEND);
		boost::shared_ptr<string> listattend = param->GetUtfString(EXT_FIELD_LIST_ATTEND);
		if (numattend_ptr != NULL)
		{
			int numattend = *numattend_ptr;
			layerBaoDanh->setListBaoDanh(numattend);
		}
		// 		if (listattend != NULL)
		// 		{
		// 			layerBaoDanh->setListBaoDanh(*listattend);
		// 		}
	}//Change for game p2
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_RES_NUM_SPIN))
	{
		SceneManager::getSingleton().hideLoading();
		boost::shared_ptr<long> ptr_numspin = param->GetInt(EXT_FIELD_NUM_SPIN);
		if (ptr_numspin)
		{
			int num_spin = *ptr_numspin;
			if (num_spin > 0)
			{
				luotquay = num_spin;
			}
		}
	}
	else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_ATTEND_RES))
	{
		this->requestListAttend();
		this->requestNumspin();
	}
}

void QuayThuongHN::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{

}

void QuayThuongHN::setLuotquay(int _luotquay)
{
	luotquay = _luotquay;
}

void QuayThuongHN::createSmartFoxEvent()
{
	// 	SceneManager::getSingleton().setTimeOutLoading(5);
	// 	SceneManager::getSingleton().showLoading();
	this->requestNumspin();
}

void QuayThuongHN::sendRequestAttend(int osid, string devid, string srcid)
{
	if (dataManager.getAppConfig().isLoginFisrtDay)
	{
		this->isLoginFirstDay = true;
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutInt("osid", osid);
		params->PutUtfString("devid", devid);
		params->PutInt("srcref", atoi(srcid.c_str()));
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_ATTEND_REQ, params));
		GameServer::getSingleton().Send(request);
		//dataManager.getAppConfig().isLoginFisrtDay = false;
	}
	else if (dataManager.getAppConfig().isLoginFisrtDay == false || osid == -1){
		this->requestListAttend();
		this->requestNumspin();
	}
}

void QuayThuongHN::requestListAttend()
{
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_LIST_ATTEND_REQ, params));
	GameServer::getSingleton().Send(request);
}

void QuayThuongHN::requestNumspin()
{
	//MINHDV - changes - request get num spin to set quay thuong hn - for game p2
	int lf = -1;
	if (dataManager.getAppConfig().isLoginFisrt) //dang nhap lan dau tien
	{
		isLoginFirstDay = false;
		dataManager.getAppConfig().isLoginFisrt = false;
		lf = 0;
	}
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutInt(EXT_FIELD_LOGIN_FIRST, lf);
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_STATUS_SPIN, params));
	GameServer::getSingleton().Send(request);
}

bool LayerBaoDanh::init()
{
	if (!Layer::init())
		return false;
	int dis_value = 30;

	Sprite* bg = Sprite::create("bg-baodanh.png");
	this->setContentSize(bg->getContentSize());
	this->addChild(bg);

	Label *labelHeader = Label::createWithTTF("Báo danh", kBoldFont, 50);
	labelHeader->setHorizontalAlignment(TextHAlignment::CENTER);
	labelHeader->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	labelHeader->setColor(Color3B(255, 225, 122));
	labelHeader->setPosition(Vec2(bg->getContentSize().width / 2,
		bg->getContentSize().height - dis_value));
	bg->addChild(labelHeader);

	Button* btnTut = Button::create("huongdan.png");
	btnTut->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	btnTut->setPosition(Vec2(bg->getContentSize().width - btnTut->getContentSize().width,
		bg->getContentSize().height - dis_value));
	btnTut->addTouchEventListener(CC_CALLBACK_2(LayerBaoDanh::onButtonTut, this));
	bg->addChild(btnTut);

	int arr_size = 7;
	int col_num = 2;
	int startY = 100;
	for (int i = 0; i < arr_size; i++)
	{
		Vec2 pos;
		int row_idx = i / col_num;
		int posY = bg->getContentSize().height - row_idx * 100 - startY;
		int posX = 20;
		if (i % 2 != 0)
			posX = bg->getContentSize().height / 2 + 10;
		pos = Vec2(posX, posY);
		log("POS x= %d / y = %d", posX, posY);
		ImageView* imageView = createBaoDanh(pos, i);
		bg->addChild(imageView);
		arrImgBaoDanh.push_back(imageView);
	}
	// 	string sample_list = "1_0_0_0_0_0_0";
	// 	this->setListBaoDanh(sample_list);

	return true;
}

ImageView* LayerBaoDanh::createBaoDanh(Vec2 pos, int tag)
{
	ImageView *imv = ImageView::create("chuabaodanh.png");
	imv->setAnchorPoint(Vec2(0, 1));
	imv->setPosition(pos);
	imv->setTag(tag);

	Text* txtNgay = Text::create(StringUtils::format("Ngày %d", (tag + 1)), kNormalFont, 30);
	txtNgay->setTextHorizontalAlignment(TextHAlignment::LEFT);
	txtNgay->setAnchorPoint(Vec2::ZERO);
	txtNgay->setPosition(Vec2(imv->getContentSize().width, 20));
	imv->addChild(txtNgay);

	return imv;
}

void LayerBaoDanh::setListBaoDanh(string _list)
{
	//string sample_list = "1_0_0_0_0_0_0";
	vector<string> arr_str = mUtils::splitString(_list, '_');
	if (arr_str.size() > 0 && arrImgBaoDanh.size() == arr_str.size())
	{
		for (int i = 0; i < arr_str.size(); i++)
		{
			string img_path = "chuabaodanh.png";
			if (0 == arr_str.at(i).compare("1"))
				img_path = "dabaodanh.png";
			arrImgBaoDanh.at(i)->loadTexture(img_path);
		}
	}
}

void LayerBaoDanh::setListBaoDanh(int _num)
{
	int _length = _num;
	if (_num > arrImgBaoDanh.size())
		_length = arrImgBaoDanh.size();
	for (int i = 0; i < _length; i++)
	{
		arrImgBaoDanh.at(i)->loadTexture("dabaodanh.png");
	}
}

void LayerBaoDanh::onButtonTut(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto _currScene = Director::getInstance()->getRunningScene();
		if (!_currScene) return;
		//string strlink = "";

		LayerPopupWebView* _layer = LayerPopupWebView::create();
		_layer->setWebView(GUIDE_QUAY_THUONG_HN_P2);
		_currScene->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_WEBVIEW);
	}
}
