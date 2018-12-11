//
//  LayerCongDong.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/8/16.
//
//

#include "LayerCongDong.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../mUtils.h"
#include <time.h>
#include "../data_manager.h"
#include "LayerPopupThongBao.h"
#include "../layergames/PhomMessDef.h"
#include "../layergames/ChanUtils.h"
#include "../scenes/SceneMain.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
#include "layergames/QuayThuongHN.h"
#include "LayerPopupDoiDiem.h"
#include "LayerPopupUngTien.h"
#include "LayerPopupHetTien.h"
using namespace cocos2d::ui;
class SpriteEx : public Sprite
{
public:
	static SpriteEx* createWithUrl(const std::string& url) {
		SpriteEx* sprite = new SpriteEx();
		sprite->autorelease();
		sprite->init();
		sprite->updateWithUrl(url);
		return sprite;
	}
	static SpriteEx* create() {
		SpriteEx* sprite = new SpriteEx();
		sprite->autorelease();
		sprite->init();
		sprite->retain();
		return sprite;
	}

	virtual bool init() {
		_image_size.x = 0;
		_image_size.y = 0;
		return Sprite::init();
	}

	void setSize(Vec2 s) {
		_image_size = s;
	}

	void updateWithUrl(const std::string& url) {
		network::HttpRequest* request = new network::HttpRequest();
		request->setUrl(url.data());
		request->setRequestType(network::HttpRequest::Type::GET);
		request->setResponseCallback([=](network::HttpClient* client, network::HttpResponse* response) {
			if (!response->isSucceed()) {
				CCLOG("ERROR, remote sprite load image failed");
				return;
			}

			std::vector<char> *buffer = response->getResponseData();
			Image img;
			img.initWithImageData(reinterpret_cast<unsigned char*>(&(buffer->front())), buffer->size());

			if (0)
			{
				// save image file to device.
				std::string path = FileUtils::getInstance()->getWritablePath() + "p.png";
				CCLOG("save image path = %s", path.data());
				bool ret = img.saveToFile(path);
				CCLOG("save file %s", ret ? "success" : "failure");

				this->initWithFile(path);
			}
			else {

				// create sprite with texture
				Texture2D *texture = new Texture2D();
				texture->autorelease();
				texture->initWithImage(&img);
				this->initWithTexture(texture);
				if (0 != _image_size.x) {
					auto size = getContentSize();
					setScaleX(_image_size.x / size.width);
					setScaleY(_image_size.y / size.height);
				}
			}
		});
		network::HttpClient::getInstance()->send(request);
		request->release();
	}

private:
	cocos2d::Vec2 _image_size;
};


LayerCongDong::LayerCongDong()
{
	GameServer::getSingleton().addListeners(this);
}

LayerCongDong::~LayerCongDong()
{
	
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCongDong::CountDownTimerShare),this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCongDong::CountDownTimerInvite),this);

	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	{
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
		CC_SAFE_DELETE(face);
#endif
	}

	GameServer::getSingleton().removeListeners(this);
}

bool LayerCongDong::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;

	auto rootNode = CSLoader::getInstance()->createNode("LayerCongDong.csb");
	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		face = new mFaceBook();
		face->setFunCallBack(this, callfuncND_selector(LayerCongDong::showFriend));
#endif
		this->pnlCongDong = static_cast<Layout*>(rootNode->getChildByName("pnlCongDong"));
		auto bonusls = mUtils::splitString(dataManager.getAppConfig().isfb, '_');
		this->txtGoldMoi = static_cast<Text*>(this->pnlCongDong->getChildByName("txtGoldMoi"));
		this->txtNhanShare = static_cast<Text*>(this->pnlCongDong->getChildByName("txtNhanShare"));
		if (bonusls.size() >= 2){
			this->txtGoldMoi->setString(mUtils::convertMoneyEx(atof(bonusls[0].c_str())) + " Vàng");
			this->txtNhanShare->setString(mUtils::convertMoneyEx(atof(bonusls[1].c_str())) + " Vàng");

		}
		this->btnNhanMoi = static_cast<Button*>(this->pnlCongDong->getChildByName("btnNhanMoi"));
		if (this->btnNhanMoi){
			this->btnNhanMoi->addClickEventListener(CC_CALLBACK_1(LayerCongDong::onBtnNhanMoiFB, this));
			this->btnNhanMoi->setPressedActionEnabled(true);
		}
		this->btnNhanShare = static_cast<Button*>(this->pnlCongDong->getChildByName("btnNhanShare"));
		if (this->btnNhanShare){
			this->btnNhanShare->addClickEventListener(CC_CALLBACK_1(LayerCongDong::onBtnNhanShareFB, this));
			this->btnNhanShare->setPressedActionEnabled(true);
		}
		Button* btnChonTatCa = static_cast<Button*>(this->pnlCongDong->getChildByName("btnChonTatCa"));
		if (btnChonTatCa){
			btnChonTatCa->addClickEventListener(CC_CALLBACK_1(LayerCongDong::onBtnChonTatCa, this));
			btnChonTatCa->setPressedActionEnabled(true);
		}
		pnlFriend = static_cast<Layout*>(this->pnlCongDong->getChildByName("pnlTable"));
		this->tblFriendFB = TableView::create(this, pnlFriend->getContentSize());
		this->tblFriendFB->setAnchorPoint(pnlFriend->getAnchorPoint());
		this->tblFriendFB->setContentSize(pnlFriend->getContentSize());
		this->tblFriendFB->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
		this->tblFriendFB->setPosition(pnlFriend->getPosition());
		this->tblFriendFB->setDelegate(this);
		this->tblFriendFB->setVisible(false);
		this->tblFriendFB->setTag(tag_tbl_friendfb);
		this->tblFriendFB->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);

		this->pnlCongDong->addChild(this->tblFriendFB);

    
    
	return true;
}
void LayerCongDong::hideLayer(){
    this->setVisible(false);
}
void LayerCongDong::showLayer(){
    this->setVisible(true);
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("eegifbrq", params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading(40);
    this->btnNhanMoi->setEnabled(false);
    this->btnNhanShare->setEnabled(false);
    
    SceneManager::getSingleton().clearInviteableUsers();
    SceneManager::getSingleton().clearInviteUserIdx();
#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    face->LoginForFriend();
#endif
    
}
void LayerCongDong::onBtnNhanMoiFB(Ref* pSender){

	if (SceneManager::getSingleton().getInviteUserIdxs().size() < 25){
		Chat *toast = Chat::create("Hãy chọn đủ 25 bạn", -1);
		this->addChild(toast);
		return;
	}


#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (SceneManager::getSingleton().getInviteUserIdxs().size() > 0) {
		vector<string> list = SceneManager::getSingleton().getInviteUserIdxs();
		std::vector<std::string> invite_ids;
		for (auto i : list) {
			invite_ids.push_back(i);
		}
		sdkbox::PluginFacebook::inviteFriendsWithInviteIds(invite_ids, "Mời chơi", "Chơi hay");
	}
#endif

}
void LayerCongDong::onBtnNhanShareFB(Ref* pSender){


#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//face->onRequestPublishPermission();
#endif

}
void LayerCongDong::onBtnChonTatCa(Ref* pSender){
	SceneManager::getSingleton().clearInviteUserIdx();
	for (int i = 0; i < SceneManager::getSingleton().getInviteableUsers().size(); i++){

		SceneManager::getSingleton().pushInviteUserIdxs(SceneManager::getSingleton().getInviteableUsers()[i].uid);
		SceneManager::getSingleton().setInviteInviteableUsers(i, true);

	}
	this->tblFriendFB->reloadData();
}
void LayerCongDong::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerCongDong::tableCellSizeForIndex(TableView *table, ssize_t idx)
{

    return Size(table->getContentSize().width, 250);
	
}

TableViewCell* LayerCongDong::tableCellAtIndex(TableView *table, ssize_t idx)
{
	return createCell4LstFriendFacebook(table, idx);
}

TableViewCell* LayerCongDong::createCell4LstFriendFacebook(TableView *table, ssize_t idx){
	auto cell = table->dequeueCell();
	auto _cTag = 123;
	if (!cell) {
		cell = TableViewCell::create();
	}
	else {
		cell->removeAllChildren();
	}
	if (idx < 0)
		return cell;
	auto cols = 4;
	auto _child_ = LayerItemFriendFacebookCongDong::create();
	auto sizeTable = this->pnlFriend->getContentSize();
	auto width = _child_->getContentSize().width;
	auto startX = (sizeTable.width - (_child_->getContentSize().width)*cols - 30 * (cols - 1)) / 2;

	//comment
	for (int i = 0; i < cols; i++) {
		int index = idx *cols + i;
		if (index < SceneManager::getSingleton().getInviteableUsers().size())
		{
			auto _child = LayerItemFriendFacebookCongDong::create();
			//_child->onInviteFriendCallBack(this, callfuncO_selector(LayerCongDong::onBtnNhanMoiFB));
			//  _child.onPlayRoomCallBack(this.playRoomCallBack, this);
			_child->setPosition(startX + (i * width) + 30 * i, 0);
			//  _child.setDatas(this.listTheCaos[idx].type,this.listTheCaos[idx].menhgia,this.listTheCaos[idx].cost);
			cell->addChild(_child, 0, _cTag + i);
			//_child->setDatas(SceneManager::getSingleton().getInviteableUsers()[index].uid, SceneManager::getSingleton().getInviteableUsers()[index].url_picture, SceneManager::getSingleton().getInviteableUsers()[index].name, SceneManager::getSingleton().getInviteableUsers()[index].isInvite);
			_child->setDatas(SceneManager::getSingleton().getInviteableUsers()[index].uid, SceneManager::getSingleton().getInviteableUsers()[index].url_picture, SceneManager::getSingleton().getInviteableUsers()[index].name, SceneManager::getSingleton().getInviteableUsers()[index].isInvite);
			_child->setVisible(true);
			_child->setTag(index);
		}
	}
	return cell;
}

ssize_t LayerCongDong::numberOfCellsInTableView(TableView *table)
{

		int rowItem = 0;
		int sizeList = SceneManager::getSingleton().getInviteableUsers().size();
		if (sizeList % 4 == 0){
			rowItem = sizeList / 4;
		}
		else{
			rowItem = sizeList / 4 + 1;
		}
    return rowItem;

}
string LayerCongDong::getCurrentTime(){
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);

	int day = aTime->tm_mday;
	int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
	int year = aTime->tm_year + 1900;

	return StringUtils::format("%d/%d", month, year).c_str();
}
void LayerCongDong::showFriend(Node* sender, void* data){
	if (data){
		string *_type = (string*)data;
		string type = *_type;
		if (type == "1"){
			string lstuid;
			for (int i = 0; i < SceneManager::getSingleton().getInviteUserIdxs().size() - 24; i++){
				lstuid = lstuid + SceneManager::getSingleton().getInviteUserIdxs()[i] + ";";
			}
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutInt("type", 1);
			params->PutUtfString("fbid", lstuid);
			boost::shared_ptr<IRequest> request(new ExtensionRequest("eegfbrq", params));
			GameServer::getSingleton().Send(request);
			SceneManager::getSingleton().showLoading();

		}
		else{
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutInt("type", 2);
			params->PutUtfString("fbid", "share");
			boost::shared_ptr<IRequest> request(new ExtensionRequest("eegfbrq", params));
			GameServer::getSingleton().Send(request);
			SceneManager::getSingleton().showLoading();

		}
	}
	else{
		while (SceneManager::getSingleton().getInviteableUsers().size() > 25){
			int k = SceneManager::getSingleton().getInviteableUsers().size() - 1;
			int a = rand() % k + 0;
			SceneManager::getSingleton().earseInviteableUsers(a);
		}
		this->tblFriendFB->reloadData();
		SceneManager::getSingleton().hideLoading();
	}
}


void LayerCongDong::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
   if (strcmp("eegfbrs", cmd->c_str()) == 0){

		SceneManager::getSingleton().hideLoading();

		if (param->GetInt("rc") == NULL) return;
		int rc = *param->GetInt("rc");

		if (rc == 0){
			auto _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;

			if (param->GetInt("rv") == NULL) return;
			int rv = *param->GetInt("rv");

			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(StringUtils::format("Bạn đã nhận được %d %s!", rv,SceneManager::getSingleton().getCoin(0).c_str()));
			layerPopup->setTag(24);
			layerPopup->showAButton();
			_currScene->addChild(layerPopup, 1000000,9096);

		}
		else{
			if (param->GetUtfString("rd") == NULL) return;
			string rd = *param->GetUtfString("rd");
			auto _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setMessage(dataManager.GetSysString(141));
			layerPopup->setContentMess(rd);
			layerPopup->setTag(24);
			layerPopup->showAButton();
			_currScene->addChild(layerPopup, 1000000,9097);
		}

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> request(new ExtensionRequest("eegifbrq", params));
		GameServer::getSingleton().Send(request);

		SceneManager::getSingleton().showLoading();
	}
	else if (strcmp("eegifbrs", cmd->c_str()) == 0){
		//SceneManager::getSingleton().hideLoading();
		if (param->GetUtfString("info") != NULL){
			string info = *param->GetUtfString("info");
			auto lstinfo = mUtils::splitString(info, ';');
			if (lstinfo.size() < 2) return;
			auto item1 = mUtils::splitString(lstinfo[0], '|');
			if (item1.size() < 2) return;
			this->timerInvite = atoi(item1[1].c_str());
			if (this->timerInvite <= 0){
				this->btnNhanMoi->setEnabled(true);
				this->btnNhanMoi->setTitleText("Mời");
				Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCongDong::CountDownTimerInvite),this);

			}
			else{
				this->btnNhanMoi->setEnabled(false);
				this->showTimerInvite();
				Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerCongDong::CountDownTimerInvite),this,1, false);
			}
			auto item2 = mUtils::splitString(lstinfo[1], '|');
			if (item2.size() < 2) return;
			this->timerShare = atoi(item2[1].c_str());
			if (this->timerShare <= 0){
				this->btnNhanShare->setEnabled(true);
				this->btnNhanShare->setTitleText("Chia sẻ");
				Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCongDong::CountDownTimerShare),this);

			}
			else{
				this->btnNhanShare->setEnabled(false);
				this->showTimerShare();
				Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerCongDong::CountDownTimerShare),this,1, false);
			}

		}
	}
}
void LayerCongDong::CountDownTimerShare(float dt){

	if (this->timerShare == 0){
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCongDong::CountDownTimerShare),this);
		this->btnNhanShare->setTitleText("Chia sẻ");
		this->btnNhanShare->setEnabled(true);
	}
	this->timerShare -= 1;
	this->showTimerShare();
}
void LayerCongDong::showTimerShare(){
	int hour = this->timerShare / 3600;
	int second = this->timerShare % 3600;
	int minute = second / 60;
	second %= 60;
	if (hour >= 10){
		if (second >= 10){
			if (minute >= 10)
				this->btnNhanShare->setTitleText(StringUtils::format("%d:%d:%d", hour, minute, second));
			else
				this->btnNhanShare->setTitleText(StringUtils::format("%d:0%d:%d", hour, minute, second));
		}
		else{
			if (minute >= 10)
				this->btnNhanShare->setTitleText(StringUtils::format("%d:%d:0%d", hour, minute, second));
			else
				this->btnNhanShare->setTitleText(StringUtils::format("%d:0%d:0%d", hour, minute, second));
		}
	}
	else{
		if (second >= 10){
			if (minute >= 10)
				this->btnNhanShare->setTitleText(StringUtils::format("%0d:%d:%d", hour, minute, second));
			else
				this->btnNhanShare->setTitleText(StringUtils::format("%0d:0%d:%d", hour, minute, second));
		}
		else{
			if (minute >= 10)
				this->btnNhanShare->setTitleText(StringUtils::format("%0d:%d:0%d", hour, minute, second));
			else
				this->btnNhanShare->setTitleText(StringUtils::format("%0d:0%d:0%d", hour, minute, second));
		}
	}
}
void LayerCongDong::CountDownTimerInvite(float dt){
	if (this->timerInvite == 0){
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerCongDong::CountDownTimerInvite),this);
		this->btnNhanMoi->setEnabled(true);
		this->btnNhanMoi->setTitleText("Mời");
	}
	this->timerInvite -= 1;
	this->showTimerInvite();
}
void LayerCongDong::showTimerInvite(){
	int hour = this->timerInvite / 3600;
	int second = this->timerInvite % 3600;
	int minute = second / 60;
	second %= 60;
	if (hour >= 10){
		if (second >= 10){
			if (minute >= 10)
				this->btnNhanMoi->setTitleText(StringUtils::format("%d:%d:%d", hour, minute, second));
			else
				this->btnNhanMoi->setTitleText(StringUtils::format("%d:0%d:%d", hour, minute, second));
		}
		else{
			if (minute >= 10)
				this->btnNhanMoi->setTitleText(StringUtils::format("%d:%d:0%d", hour, minute, second));
			else
				this->btnNhanMoi->setTitleText(StringUtils::format("%d:0%d:0%d", hour, minute, second));
		}
	}
	else{
		if (second >= 10){
			if (minute >= 10)
				this->btnNhanMoi->setTitleText(StringUtils::format("%0d:%d:%d", hour, minute, second));
			else
				this->btnNhanMoi->setTitleText(StringUtils::format("%0d:0%d:%d", hour, minute, second));
		}
		else{
			if (minute >= 10)
				this->btnNhanMoi->setTitleText(StringUtils::format("%0d:%d:0%d", hour, minute, second));
			else
				this->btnNhanMoi->setTitleText(StringUtils::format("%0d:0%d:0%d", hour, minute, second));
		}
	}
}
void LayerCongDong::getPartnerInfo(){

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

bool LayerCongDong::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerCongDong::onEnter()
{
	Layer::onEnter();
//	auto dispatcher = Director::getInstance()->getEventDispatcher();
//	auto listener = EventListenerTouchOneByOne::create();
//	listener->setSwallowTouches(true);
//	listener->onTouchBegan = CC_CALLBACK_2(LayerCongDong::onTouchBegan, this);
//	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//	_touchListener = listener;
	//
	//    GameServer::getSingleton().addListeners(this);
}

void LayerCongDong::onExit()
{
	Layer::onExit();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	//    GameServer::getSingleton().removeListeners(this);
	//    Layer::onExit();
}

void LayerCongDong::fakeList()
{
	//SceneManager::getSingleton().getInviteableUsers().clear();
	//	string jsonData = dataManager.GetSysString(267);
	//	rapidjson::Document document;
	//	document.Parse<0>(jsonData.c_str());
	//	if (document.HasParseError()) {
	//		return;
	//	}
	//	if (document.IsObject()) {
	//		//invited facebook get list Friend
	//		rapidjson::Value& data = document["data"];
	//		FriendInfo friendInfo;
	//		for (rapidjson::SizeType i = 0; i < data.Size(); ++i){
	//			CCLOG("id: %s - name: %s", data[i]["id"].GetString(), data[i]["name"].GetString());
	//			friendInfo.uid = data[i]["id"].GetString();
	//			friendInfo.name = data[i]["name"].GetString();
	//			rapidjson::Value& picture = data[i]["picture"];
	//			CCLOG("url: %s", picture["data"]["url"].GetString());
	//			friendInfo.url_picture = picture["data"]["url"].GetString();
	//			SceneManager::getSingleton().pushInviteableUsers(friendInfo);
	//		}
	//	}
}

//Item

bool LayerItemFriendFacebookCongDong::init(){
	if (!Layer::init())
	{
		return false;
	}

	auto root = CSLoader::getInstance()->createNode("ItemFriendFace.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());

	this->urlAvatar = dynamic_cast<ImageView*>(root->getChildByName("imgAvatarFace"));
	this->imgSelected = dynamic_cast<ImageView*>(root->getChildByName("imgSelected"));
	this->nameFriend = static_cast<Text*>(root->getChildByName("lblName"));
	this->btnSelectedInvite = static_cast<Button*>(root->getChildByName("btnInviteSelected"));
	if (this->btnSelectedInvite)
	{
		btnSelectedInvite->setSwallowTouches(false);
		this->btnSelectedInvite->addClickEventListener(CC_CALLBACK_1(LayerItemFriendFacebookCongDong::onBtnInvitedFriend, this));
	}

	return true;
}

void LayerItemFriendFacebookCongDong::setDatas(string _uid, string urlAvatar, string name, bool isInvite){
	this->uid = _uid;
	this->imgSelected->setVisible(false);
	if (isInvite)
		this->imgSelected->setVisible(true);
	this->imgSelected->setLocalZOrder(15);
	this->urlAvatar->setVisible(true);
	std::string smallContent = utf8_substr((name), 0, 13) + "...";
	this->nameFriend->setString(smallContent);
	Node* clipper = Node::create();
	_isInvite = isInvite;
	Sprite* gameTitle = Sprite::create("icon_default.png");
	clipper->setPosition(this->urlAvatar->getPosition());
	clipper->setContentSize(Size(gameTitle->getContentSize().width, gameTitle->getContentSize().height));
	clipper->retain();
	this->addChild(clipper);
	//log("name facebook small : %s",smallContent.c_str());
	//log("name facebook : %s",name.c_str());
	string url = mUtils::replaceAll(urlAvatar, "https:", "http:");
	//SceneManager::getSingleton().getImageDownloadProcess()->downLoadImage(clipper, url);
	_iconSprite = SpriteEx::createWithUrl(urlAvatar);
	_iconSprite->setPosition(Vec2(gameTitle->getContentSize().width / 2 + 5, gameTitle->getContentSize().height / 2 + 5));
	_iconSprite->setSize(Size(gameTitle->getContentSize().width + 10, gameTitle->getContentSize().height + 10));
	_iconSprite->retain();
	this->urlAvatar->addChild(_iconSprite, 10);
}

void  LayerItemFriendFacebookCongDong::onInviteFriendCallBack(Ref* target, SEL_CallFuncO listener){
	mCallBack = target;
	mCallBackListener = listener;
}
void  LayerItemFriendFacebookCongDong::onBtnInvitedFriend(Ref* pSender){
	//if (mCallBack && mCallBackListener){
	//	//mCallBack
	//	Node* node = Node::create();
	//	node->setTag(this->getTag());
	//	(mCallBack->*mCallBackListener)(node);
	//}
	count_clicked++;
	if (!_isInvite){
		this->imgSelected->setVisible(true);
		for (int i = 0; i < SceneManager::getSingleton().getInviteableUsers().size(); i++){
			if (SceneManager::getSingleton().getInviteableUsers()[i].uid == uid){
				SceneManager::getSingleton().setInviteInviteableUsers(i, true);

			}
		}
		SceneManager::getSingleton().pushInviteUserIdxs(uid);
		_isInvite = true;
	}
	else{
		this->imgSelected->setVisible(false);
		for (int i = 0; i < SceneManager::getSingleton().getInviteableUsers().size(); i++){
			if (SceneManager::getSingleton().getInviteableUsers()[i].uid == uid){
				SceneManager::getSingleton().setInviteInviteableUsers(i, false);
				SceneManager::getSingleton().earseInviteUserIdxs(i);

			}
		}
		for (int i = 0; i < SceneManager::getSingleton().getInviteUserIdxs().size(); i++){
			if (SceneManager::getSingleton().getInviteUserIdxs()[i] == uid){
				SceneManager::getSingleton().earseInviteUserIdxs(i);
			}

		}
		_isInvite = false;
	}
}
