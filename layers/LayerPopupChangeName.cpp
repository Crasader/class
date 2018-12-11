#include "LayerPopupChangeName.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Requests/ExtensionRequest.h"
#include "../layergames/_Chat_.h"
#include "../mUtils.h"
#include "../SceneManager.h"
#include "Requests/LogoutRequest.h"
#include "../data_manager.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupThongBao.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#include "ui/UIEditBox/UIEditBox.h"
#include "scenes/SceneMain.h"

using namespace cocos2d::ui;

LayerPopupChangeName::LayerPopupChangeName(){

}

LayerPopupChangeName::~LayerPopupChangeName(){
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupChangeName::init(){
	if (!Layer::init()){
		return false;
	}
	//MINHDV change
	setFlagPopup(FLAG_CHANGE_NAME);//set for default

	GameServer::getSingleton().addListeners(this);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto _layer = CSLoader::getInstance()->createNode("LayerPopupChangeName.csb");
	_layer->setAnchorPoint(Point(0.5, 0.5));
	_layer->setPosition(Point(visibleSize / 2));
	auto pnlLayer = static_cast<Layout*>(_layer->getChildByName("Panel_2"));
	//pnlLayer->setAnchorPoint(Point(0.5, 0.5));
	pnlLayer->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	lblError = (Text*)(_layer->getChildByName("lblError"));
	lblError->setVisible(false);
	txtNameDisplay = static_cast<TextField*>(_layer->getChildByName("txtNameDisplay"));
	btnOK = static_cast<Button*>(_layer->getChildByName("btnOK"));
	if (txtNameDisplay)
	{
		txtNameDisplay->setMaxLengthEnabled(true);
		txtNameDisplay->setMaxLength(20);
	}
	if (btnOK != NULL){
		btnOK->setPressedActionEnabled(true);
		btnOK->addClickEventListener(CC_CALLBACK_1(LayerPopupChangeName::onButtonOK, this));
	}
	btnClose = static_cast<Button*>(_layer->getChildByName("btnClose"));
	if (btnClose)
	{
		btnClose->setPressedActionEnabled(true);
		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPopupChangeName::onButtonClose, this));
	}
	lblTitle = (Text*)_layer->getChildByName("Text_1");

	lblHeader = (Text*)_layer->getChildByName("Text_1_0");
	this->addChild(_layer);
	return true;
}

void LayerPopupChangeName::onButtonOK(Ref* pSender){
	if (_flag == FLAG_CHANGE_NAME)
	{
		log("onbtnOK-changeName");
		std::string _str = txtNameDisplay->getString();
		if (_str.length() <= 0){
			lblError->setVisible(true);
		}
		else if (_str.length() < 6){
			lblError->setString("* Tên hiển thị không được nhỏ hơn 6 ký tự");
			lblError->setVisible(true);
		}
		else if (mUtils::findSpecialCharNotSpace(string(txtNameDisplay->getString())) == true){
			lblError->setString(dataManager.GetSysString(40));
			lblError->setVisible(true);
		}
		else{
			/*this->removeFromParentAndCleanup(true);*/

			int  osid = 2; //ios
			std::string devID = "";
			/*bool isConnectting = false;*/
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
			getPartnerInfo();
			string LT = UserDefault::getInstance()->getStringForKey("LT", "facebook");

			boost::shared_ptr<ISFSObject> params(new SFSObject());
			string fbID = UserDefault::getInstance()->getStringForKey("facebook_id");
			string gID = UserDefault::getInstance()->getStringForKey("google_id");
			if (LT.compare("facebook") == 0)
				params->PutUtfString("fgid", fbID);// FG_ID = "fgid";
			else
				params->PutUtfString("fgid", gID);// FG_ID = "fgid";

			int utype = LT.compare("facebook") == 0 ? 1 : 2;
			params->PutInt("utype", utype);//UPDATE_TYPE = "utype"; 1 là facebook - 2 google
			params->PutUtfString("aI", txtNameDisplay->getString());//

			params->PutInt("osid", osid);// Operatorsystems
			params->PutUtfString("devid", devID);// device id
			params->PutUtfString("accParId", partnerID);// LOGIN_PARTNER_ID = "accParId";
			params->PutInt("srcref", atoi(srcID.c_str()));//SRC_REF = "srcref";
			SceneManager::getSingleton().showLoading();
			boost::shared_ptr<IRequest> request(new ExtensionRequest("ruaarf", params));//REQUEST_UPDATE_ACC_AFTER_RF = "ruaarf";
			GameServer::getSingleton().Send(request);
		}
	}
	else if (_flag == FLAG_CHANGE_PHONE)
	{
//		log("onbtnOK-change PHONE");
//		std::string _str = txtNameDisplay->getString();
//		if (_str.length() <= 0){
//			lblError->setString("* Số điện thoại không được để trống");
//			lblError->setVisible(true);
//		}
//		else if (_str.length() < 10 || _str.length() > 11){
//			lblError->setString("* Số điện thoại không hợp lệ");
//			lblError->setVisible(true);
//		}
//		else{
//           
//			GameServer::getSingleton().Send(request);
//			SceneMain* parent = dynamic_cast<SceneMain*> (this->getParent());
//			if (parent)
//			{
//				parent->showToast(dataManager.GetSysString(36));
//				parent->removeChild(this, true);
//			}
//
//		}
	}
}

void LayerPopupChangeName::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	if (ptrEvetnParams == NULL) return;
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	if (ptrEventParamValueCmd == NULL) return;
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
	if (cmd == NULL) return;
	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	if (ptrEventParamValueParams == NULL)
		return;
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (param == NULL) return;
	log("OnExtensionResponse %s", cmd->c_str());
	if (strcmp("resuaarf", cmd->c_str()) == 0){//OK
		log("param: %d", *param->GetInt("rc"));
       // log("param: %s", (*param->GetUtfString("rd")).c_str());

		if (param->GetInt("rc") == NULL || param->GetInt("rd") == NULL) return;
		SceneManager::getSingleton().hideLoading();
		if (*param->GetInt("rc") == 0){


			log("Change Name OK");
			Chat *toast = Chat::create(dataManager.GetSysString(439), -1);
			this->addChild(toast);
			//

			LayerPopupThongBao* layerpoup = LayerPopupThongBao::create();
			layerpoup->setMessage(dataManager.GetSysString(141));
			layerpoup->setContentMess(dataManager.GetSysString(440));
			layerpoup->getBtnHuy()->setVisible(false);
			layerpoup->getBtnOk()->setPositionX(layerpoup->getLblTitle()->getPositionX());
			layerpoup->setCallBackFromButton(this, callfuncO_selector(LayerPopupChangeName::onPopupCallBack), 1);
			this->addChild(layerpoup);
		}
		else{
			auto rd = *param->GetUtfString("rd");
//			string mess;
//			switch (rc){
//			case 139:
//				mess = "Tên đã tồn tại!";
//				break;
//			}
			Chat *toast = Chat::create(rd, -1);
			this->addChild(toast);
		}
	}
}
void LayerPopupChangeName::onPopupCallBack(Ref* pSender){
	SceneManager::getSingleton().showLoading();
	boost::shared_ptr<IRequest> request(new LogoutRequest());
	GameServer::getSingleton().Send(request);
	//
	SceneManager::getSingleton().rcForLogin = true;
	SceneManager::getSingleton().gotoLogin();
	UserDefault::getInstance()->setBoolForKey("isShouldShowWelcome", true);
}
void LayerPopupChangeName::getPartnerInfo()
{
	string sPartInfo = "";
	partnerID = "1";
	srcID = "0";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sPartInfo = NativeHelper::getPartnerID();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	sPartInfo = IOS::tryGetPartnerID();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	sPartInfo = "1-1";
#endif
	CCLOG("************************** %s", sPartInfo.c_str());
	//split partnerID and srcID
	if (sPartInfo == "")
		sPartInfo = "1-0";
	vector<string> lstPartInfo = mUtils::splitString(sPartInfo, '-');
	if (lstPartInfo.size() < 2){
		return;
	}
	partnerID = lstPartInfo.at(0);
	srcID = lstPartInfo.at(1);
}

void LayerPopupChangeName::setContentPopUp(string title, string mess, string strhint)
{
	lblTitle->setString(title);
	lblHeader->setString(mess);
	txtNameDisplay->setPlaceHolder(strhint);
	if (_flag == FLAG_CHANGE_PHONE)
	{
		btnClose->setVisible(true);
        if(CC_TARGET_PLATFORM != CC_PLATFORM_LINUX){
            txtNameDisplay->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
        }
	}
}

void LayerPopupChangeName::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}


LayerPopupChangeMobile::LayerPopupChangeMobile(){

}

LayerPopupChangeMobile::~LayerPopupChangeMobile(){
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupChangeMobile::init(){
	if (!Layer::init()){
		return false;
	}
	//MINHDV change
	setFlagPopup(FLAG_CHANGE_NAME);//set for default

	GameServer::getSingleton().addListeners(this);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto _layer = CSLoader::getInstance()->createNode("LayerPopupChangeMobile.csb");
	_layer->setAnchorPoint(Point(0.5, 0.5));
	_layer->setPosition(Point(visibleSize / 2));
	auto pnlLayer = static_cast<Layout*>(_layer->getChildByName("Panel_2"));
	//pnlLayer->setAnchorPoint(Point(0.5, 0.5));
	pnlLayer->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	lblError = (Text*)(_layer->getChildByName("lblError"));
	lblError->setVisible(false);
	txtNameDisplay = static_cast<TextField*>(_layer->getChildByName("txtNameDisplay"));
	if (txtNameDisplay)
	{
		if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
		{
			txtNameDisplay->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
		}
	}
	btnOK = static_cast<Button*>(_layer->getChildByName("btnOK"));
	if (btnOK != NULL){
		btnOK->setPressedActionEnabled(true);
		btnOK->addClickEventListener(CC_CALLBACK_1(LayerPopupChangeMobile::onButtonOK, this));
	}
	btnClose = static_cast<Button*>(_layer->getChildByName("btnClose"));
	if (btnClose)
	{
		btnClose->setPressedActionEnabled(true);
		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPopupChangeMobile::onButtonClose, this));
	}
	lblTitle = (Text*)_layer->getChildByName("Text_1");

	lblHeader = (Text*)_layer->getChildByName("Text_1_0");
	this->addChild(_layer);
	return true;
}

void LayerPopupChangeMobile::onButtonOK(Ref* pSender){
	log("onbtnOK-change PHONE");
	std::string _str = txtNameDisplay->getString();
	if (_str.length() <= 0){
		lblError->setString("* Số điện thoại không được để trống");
		lblError->setVisible(true);
	}
	else if (_str.length() < 10 || _str.length() > 11){
		lblError->setString("* Số điện thoại không hợp lệ");
		lblError->setVisible(true);
	}
    else if (mUtils::findContainChar(_str.c_str())){
        lblError->setVisible(true);
        lblError->setString("Chỉ được nhập số!");
        return;
    }
	else{
        boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
        if (myself == NULL || myself->GetVariable("aN") == NULL){
            lblError->setString("* Có lỗi xảy ra! Vui lòng thử lại!");
            return;
        }
        
        string myName = SceneManager::getSingleton().getMyName();
        string myAN = *myself->GetVariable("aN")->GetStringValue();
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString("aI", myName);
        params->PutUtfString("aMo", txtNameDisplay->getString());
        params->PutUtfString("aN", *myself->GetVariable("aN")->GetStringValue());
        boost::shared_ptr<IRequest> request(new ExtensionRequest("rua", params));
		GameServer::getSingleton().Send(request);
		SceneMain* parent = dynamic_cast<SceneMain*> (this->getParent());
		if (parent)
		{
			parent->showToast(dataManager.GetSysString(134));
			parent->removeChild(this, true);
		}

	}
}

void LayerPopupChangeMobile::setContentPopUp(string title, string mess, string strhint)
{
	lblTitle->setString(title);
	lblHeader->setString(mess);
	txtNameDisplay->setPlaceHolder(strhint);
	if (_flag == FLAG_CHANGE_PHONE)
	{
		btnClose->setVisible(true);
        if(CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
        {
            txtNameDisplay->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
        }
    }
}

void LayerPopupChangeMobile::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
