#include "LayerNotificationTop.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LayerNotification.h"
#include "../scenes/SceneMain.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "../SceneManager.h"
#include "../layergames/PhomMessDef.h"
USING_NS_CC;
using namespace network;
LayerNotificationTop::LayerNotificationTop()
{
	this->btnNotification = NULL;
	this->btnBonus = NULL;
	GameServer::getSingleton().addListeners(this);
}

LayerNotificationTop::~LayerNotificationTop()
{
	this->lstButtons.clear();
	this->lstLinks.clear();
	GameServer::getSingleton().removeListeners(this);
}

bool LayerNotificationTop::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerNotificationTop.csb");

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);
	//add event listenner
	btnNotification = dynamic_cast<Button*>(rootNode->getChildByName("btnThongBao"));
	if (btnNotification != NULL){
		//btnNotification->addClickEventListener(CC_CALLBACK_1(LayerNotificationTop::onButtonNotification, this));
		this->btnNotification->setVisible(false);
	}
	btnBonus = dynamic_cast<Button*>(rootNode->getChildByName("btnTinKhuyenMai"));
	if (btnBonus != NULL){
		//btnBonus->addClickEventListener(CC_CALLBACK_1(LayerNotificationTop::onButtonBonus, this));
		this->btnBonus->setVisible(false);
	}
	//get layerTopMenuOnlyTop
	// 	auto layerTopMenuOnlyTop = dynamic_cast<Node*>(rootNode->getChildByName("layerTopMenuOnlyTop"));
	// 	if (layerTopMenuOnlyTop != NULL){
	auto layerTopMenuOnlyTopChild = dynamic_cast<Node*>(rootNode->getChildByName("layerTopMenuOnlyTop"));
	if (layerTopMenuOnlyTopChild != NULL){
		auto btnClose = dynamic_cast<Button*>(layerTopMenuOnlyTopChild->getChildByName("btnBack"));
		if (btnClose != NULL){
			btnClose->addClickEventListener(CC_CALLBACK_1(LayerNotificationTop::onButtonClose, this));
			btnClose->setPressedActionEnabled(true);
		}

	}
	loadNotification();
	isGet = false;
	this->setContentSize(rootNode->getContentSize());

	return true;
}
void LayerNotificationTop::update(){
}

void LayerNotificationTop::onExit(){
	Layer::onExit();

}

void LayerNotificationTop::loadNotification(){
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerNotificationTop::getBtnNoti),this,0.1, false);
}

void LayerNotificationTop::getBtnNoti(float dt){
	//SceneManager::getSingleton().showLoading();
	if (isGet)
		return;
	int size = SceneManager::getSingleton().lstbtnNoti.size();
	if (size == 0)
		return;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto orgX = (DESIGN_RESOLUTION_WIDTH - ((size * 304) + (size - 2) * 20)) / 2;

	for (rapidjson::SizeType i = 0; i < size; ++i){
		auto button = ui::Button::create();
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
			button->loadTextures("design/guifiles/popup-header-tabs-no.png", "design/guifiles/tab-btn-active.png", "design/guifiles/tab-btn-active.png");
			button->setAnchorPoint(Point(0, 0.5));
			//button->setContentSize(Size(380, 200));
			button->setTitleText(SceneManager::getSingleton().lstbtnNoti[i].name);
			button->setTitleFontSize(40);
			//button->setTitleFontName("UVNHUONGQUE_N.TTF");
            button->setContentSize(Size(button->getContentSize().width,160));

			//button->setTitleColor(ccc3(255, 255, 255));
			button->setTag(i);
			/*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
			button->setPosition(Point(orgX + (i * 340), btnNotification->getPositionY()+5));
		}
		else{
			button->loadTextures("design/guifiles/popup-header-no.png", "design/guifiles/popup-header-tabs.png", "design/guifiles/popup-header-tabs.png");
			button->setAnchorPoint(Point(0.5, 0.5));
			//button->setContentSize(Size(380, 200));
			button->setTitleText(SceneManager::getSingleton().lstbtnNoti[i].name);
			button->setTitleFontSize(40);
			//button->setTitleFontName("UVNHUONGQUE_N.TTF");
			button->setTitleColor(ccc3(255, 255, 255));
			button->setTag(i);
			/*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
			button->setPosition(Point(orgX + (i * 304), btnNotification->getPositionY() - 20));
		}
		button->addClickEventListener(CC_CALLBACK_1(LayerNotificationTop::onButtonClicked, this));
		this->addChild(button);
		this->lstButtons.push_back(button);
		this->lstLinks.push_back(SceneManager::getSingleton().lstbtnNoti[i].link);
	}
	loadFirst();
	if (!isGet)
		isGet = true;
}

void LayerNotificationTop::onButtonClicked(Ref* pSender){
	SceneManager::getSingleton().showLoading();
	for (int i = 0; i < this->lstButtons.size(); ++i){
		this->lstButtons.at(i)->setEnabled(true);
		this->lstButtons.at(i)->setBright(true);
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
			//this->lstButtons.at(i)->setTitleColor(ccc3(255, 255, 255));
		}

	}
	auto button = (ui::Button*)pSender;
	this->lstButtons.at(button->getTag())->setEnabled(false);
	this->lstButtons.at(button->getTag())->setBright(false);
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		//this->lstButtons.at(button->getTag())->setTitleColor(ccc3(26, 26, 26));
	}
	auto parent = (LayerNotification*)this->getParent();

	if (parent != NULL){
		parent->reloadData(this->lstLinks.at(button->getTag()));
	}
}
void LayerNotificationTop::loadFirst(){
	onButtonClicked(this->lstButtons.at(0));
}

void LayerNotificationTop::onButtonClose(Ref* pSender)
{
	if (this->getParent() == NULL)
		return;
	SceneMain* _parent = (SceneMain*)this->getParent()->getParent();
	if (_parent != NULL){
		_parent->gotoPickGame();
	}
}
void LayerNotificationTop::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

	CCLOG("REceiver call function");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	CCLOG("onExtentions");
	if (strcmp("notiRMFF", cmd->c_str()) == 0){
		onButtonClose(NULL);

	}
}
