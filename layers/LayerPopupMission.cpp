//
//  LayerPopupMission.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/8/16.
//
//

#include "LayerPopupMission.h"
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


LayerPopupMission::LayerPopupMission()
{
	GameServer::getSingleton().addListeners(this);
}

LayerPopupMission::~LayerPopupMission()
{
	lstNhiemvu1Lan.clear();
	lstDiemDanh.clear();
	lstNhiemvuNgay.clear();
	//SceneManager::getSingleton().getInviteableUsers().clear();
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupMission::CountDownTimerShare),this);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupMission::CountDownTimerInvite),this);

	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	{
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
		CC_SAFE_DELETE(face);
#endif
	}

	GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupMission::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupMission.csb");
	auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_5"));
	pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	Size sizeAdd = Size(0,0);
	rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd.height));

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);

	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
	if (btnClose){
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnClose, this));
	}

	btnTabNhiemVu = dynamic_cast<Button*>(pnlBg->getChildByName("btnNhiemvu"));
	if (btnTabNhiemVu){
		btnTabNhiemVu->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnTabNhiemVu, this));
	}
	btnTabNhiemVuNgay = dynamic_cast<Button*>(pnlBg->getChildByName("btnNhiemVuNgay"));
	if (btnTabNhiemVuNgay){
		btnTabNhiemVuNgay->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnTabNhiemVuNgay, this));
	}
	btnTagDiemDanh = static_cast<Button*>(pnlBg->getChildByName("btnDiemdanh"));
	if (btnTagDiemDanh){
		btnTagDiemDanh->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnTabDiemDanh, this));
	}
    btnNhiemVuTanThu = static_cast<Button*>(pnlBg->getChildByName("btnNhiemVuTanThu"));
    if (btnNhiemVuTanThu){
        btnNhiemVuTanThu->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnNhiemVuTanThu, this));
    }

    this->btnNhiemVuTanThu->setVisible(false);
    this->btnTabNhiemVu->setVisible(false);
    this->btnTabNhiemVuNgay->setVisible(false);
    this->btnTagDiemDanh->setVisible(false);

    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_MISSION_REQ, params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
    
	layoutDiemDanh = static_cast<Layout*>(pnlBg->getChildByName("pnlDiemdanh"));
	/*layoutDiemDanh->setVisible(false);*/

	Layout* pnlListDiemDanh = static_cast<Layout*>(layoutDiemDanh->getChildByName("pnlLstDiemdanh"));
	pnlListDiemDanh->setVisible(false);

	this->lblDes = static_cast<Text*>(layoutDiemDanh->getChildByName("lblDes"));




	UserDefault::getInstance()->setBoolForKey("isShouldShowWelcome", false);


	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{

		// Update Diem danh 06/12/2016

		this->pnlDiemDanhNew = static_cast<Layout*>(pnlBg->getChildByName("pnlDiemDanhNew"));
		this->txtDiemTuanDiemDanhNew = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtDiemTuan"));
		this->txtDiemNgayDiemDanhNew = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtDiemNgay"));
		if (this->txtDiemTuanDiemDanhNew)
			this->txtDiemTuanDiemDanhNew->setString("---");
		this->txtDiemNgayDiemDanhNew = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtDiemNgay"));
		if (this->txtDiemNgayDiemDanhNew)
			this->txtDiemNgayDiemDanhNew->setString("---");

		Button* btnDoiDiemDiemDanh = static_cast<Button*>(this->pnlDiemDanhNew->getChildByName("btnDoiDiem"));
		if (btnDoiDiemDiemDanh){
			btnDoiDiemDiemDanh->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnDoiDiem, this));
			btnDoiDiemDiemDanh->setPressedActionEnabled(true);
		}

		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		int acctype = 0;
		if (mySelf != NULL)
		{
			boost::shared_ptr<UserVariable> acctype_ptr = mySelf->GetVariable("accVipType");
			if (acctype_ptr != NULL)
				acctype = *acctype_ptr->GetIntValue();
		}

		this->txtCapVipDiemDanh = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtCapVip"));
		if (this->txtCapVipDiemDanh){
			string des = "";
			if (acctype == 0){
				des = "VIP 0";
			}
			else if (1 <= acctype && acctype <= 3){
				des = "VIP 1-3";
			}
			else if  (4 <= acctype && acctype  <= 7){
				des = "VIP 4-7";
			}
			else if  (8 <= acctype && acctype  <= 10){
				des = "VIP 8-10";
			}

			this->txtCapVipDiemDanh->setString(des);
		}

		this->txtDesCapVip = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtDes"));
		if (this->txtDesCapVip){
			this->txtDesCapVip->setString(StringUtils::format("(Bạn hiện tại là vip %d)", acctype));
		}
		this->txtDiemDiemDanh1 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtDiem1"));
		if (this->txtDiemDiemDanh1){
			this->txtDiemDiemDanh1->setString("---");
			this->lstTxtDiemDiemDanh.push_back(this->txtDiemDiemDanh1);
		}
		this->txtDiemDiemDanh2 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtDiem2"));
		if (this->txtDiemDiemDanh2){
			this->txtDiemDiemDanh2->setString("---");
			this->lstTxtDiemDiemDanh.push_back(this->txtDiemDiemDanh2);
		}
		this->txtDiemDiemDanh3 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtDiem3"));
		if (this->txtDiemDiemDanh3){
			this->txtDiemDiemDanh3->setString("---");
			this->lstTxtDiemDiemDanh.push_back(this->txtDiemDiemDanh3);
		}
		this->txtDiemDiemDanh4 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtDiem4"));
		if (this->txtDiemDiemDanh4){
			this->txtDiemDiemDanh4->setString("---");
			this->lstTxtDiemDiemDanh.push_back(this->txtDiemDiemDanh4);
		}
		this->txtDiemDiemDanh5 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtDiem5"));
		if (this->txtDiemDiemDanh5){
			this->txtDiemDiemDanh5->setString("---");
			this->lstTxtDiemDiemDanh.push_back(this->txtDiemDiemDanh5);
		}
		this->txtDiemDiemDanh6 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtDiem6"));
		if (this->txtDiemDiemDanh6){
			this->txtDiemDiemDanh6->setString("---");
			this->lstTxtDiemDiemDanh.push_back(this->txtDiemDiemDanh6);
		}
		this->txtDiemDiemDanh7 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtDiem7"));
		if (this->txtDiemDiemDanh7){
			this->txtDiemDiemDanh7->setString("---");
			this->lstTxtDiemDiemDanh.push_back(this->txtDiemDiemDanh7);
		}
		this->txtExpDiemDanh1 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtEXP1"));
		if (this->txtExpDiemDanh1){
			this->txtExpDiemDanh1->setString("---");
			this->lstTxtExpDiemDanh.push_back(this->txtExpDiemDanh1);
		}
		this->txtExpDiemDanh2 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtEXP2"));
		if (this->txtExpDiemDanh2){
			this->txtExpDiemDanh2->setString("---");
			this->lstTxtExpDiemDanh.push_back(this->txtExpDiemDanh2);
		}
		this->txtExpDiemDanh3 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtEXP3"));
		if (this->txtExpDiemDanh3){
			this->txtExpDiemDanh3->setString("---");
			this->lstTxtExpDiemDanh.push_back(this->txtExpDiemDanh3);
		}
		this->txtExpDiemDanh4 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtEXP4"));
		if (this->txtExpDiemDanh4){
			this->txtExpDiemDanh4->setString("---");
			this->lstTxtExpDiemDanh.push_back(this->txtExpDiemDanh4);
		}
		this->txtExpDiemDanh5 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtEXP5"));
		if (this->txtExpDiemDanh5){
			this->txtExpDiemDanh5->setString("---");
			this->lstTxtExpDiemDanh.push_back(this->txtExpDiemDanh5);
		}
		this->txtExpDiemDanh6 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtEXP6"));
		if (this->txtExpDiemDanh6){
			this->txtExpDiemDanh6->setString("---");
			this->lstTxtExpDiemDanh.push_back(this->txtExpDiemDanh6);
		}
		this->txtExpDiemDanh7 = static_cast<Text*>(this->pnlDiemDanhNew->getChildByName("txtEXP7"));
		if (this->txtExpDiemDanh7){
			this->txtExpDiemDanh7->setString("---");
			this->lstTxtExpDiemDanh.push_back(this->txtExpDiemDanh7);
		}
		this->imgBgDaDiemDanh1 = static_cast<ImageView*>(this->pnlDiemDanhNew->getChildByName("imgDiemDanh1"));
		if (this->imgBgDaDiemDanh1){
			this->lstBgDaDiemDanh.push_back(this->imgBgDaDiemDanh1);
		}
		this->imgBgDaDiemDanh2 = static_cast<ImageView*>(this->pnlDiemDanhNew->getChildByName("imgDiemDanh2"));
		if (this->imgBgDaDiemDanh2){
			this->lstBgDaDiemDanh.push_back(this->imgBgDaDiemDanh2);
		}
		this->imgBgDaDiemDanh3 = static_cast<ImageView*>(this->pnlDiemDanhNew->getChildByName("imgDiemDanh3"));
		if (this->imgBgDaDiemDanh3){
			this->lstBgDaDiemDanh.push_back(this->imgBgDaDiemDanh3);
		}
		this->imgBgDaDiemDanh4 = static_cast<ImageView*>(this->pnlDiemDanhNew->getChildByName("imgDiemDanh4"));
		if (this->imgBgDaDiemDanh4){
			this->lstBgDaDiemDanh.push_back(this->imgBgDaDiemDanh4);
		}
		this->imgBgDaDiemDanh5 = static_cast<ImageView*>(this->pnlDiemDanhNew->getChildByName("imgDiemDanh5"));
		if (this->imgBgDaDiemDanh5){
			this->lstBgDaDiemDanh.push_back(this->imgBgDaDiemDanh5);
		}
		this->imgBgDaDiemDanh6 = static_cast<ImageView*>(this->pnlDiemDanhNew->getChildByName("imgDiemDanh6"));
		if (this->imgBgDaDiemDanh6){
			this->lstBgDaDiemDanh.push_back(this->imgBgDaDiemDanh6);
		}
		this->imgBgDaDiemDanh7 = static_cast<ImageView*>(this->pnlDiemDanhNew->getChildByName("imgDiemDanh7"));
		if (this->imgBgDaDiemDanh7){
			this->lstBgDaDiemDanh.push_back(this->imgBgDaDiemDanh7);
		}

		this->tblListDiemDanh = TableView::create(this, pnlListDiemDanh->getContentSize());
		this->tblListDiemDanh->setAnchorPoint(pnlListDiemDanh->getAnchorPoint());
		this->tblListDiemDanh->setContentSize(pnlListDiemDanh->getContentSize());
		this->tblListDiemDanh->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
		this->tblListDiemDanh->setPosition(pnlListDiemDanh->getPosition());
		this->tblListDiemDanh->setDelegate(this);
		this->tblListDiemDanh->setVisible(false);
		this->tblListDiemDanh->setTag(tag_tbl_diemdanh);
		this->tblListDiemDanh->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
			tblListDiemDanh->setVisible(false);
		}
		//layoutDiemDanh->addChild(this->tblListDiemDanh);
	}
	else{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		face = new mFaceBook();
		face->setFunCallBack(this, callfuncND_selector(LayerPopupMission::showFriend));
#endif


		this->layoutDiemDanhNew = static_cast<Layout*>(pnlBg->getChildByName("pnlDiemdanhNew"));
		Layout * pnlListDiemDanhNew = static_cast<Layout*>(this->layoutDiemDanhNew->getChildByName("pnlLstDiemdanh"));

		this->txt1Ngay7 = static_cast<Text*>(this->layoutDiemDanhNew->getChildByName("txtGoldNgay7"));
		this->txt2Ngay7 = static_cast<Text*>(this->layoutDiemDanhNew->getChildByName("txtTheNapNgay7"));
		this->imgCheckNgay7 = static_cast<ImageView*>(this->layoutDiemDanhNew->getChildByName("imgCheckNgay7"));
		this->txt1Ngay7->setString("---");
		this->txt2Ngay7->setString("---");
		this->imgCheckNgay7->setVisible(false);

		Button* btnHuongDanDiemDanh = static_cast<Button*>(this->layoutDiemDanhNew->getChildByName("btnGuideDiemDanh"));
		if (btnHuongDanDiemDanh){
			btnHuongDanDiemDanh->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnHuongDanDiemDanh, this));
			btnHuongDanDiemDanh->setPressedActionEnabled(true);
		}

		this->tblListDiemDanh = TableView::create(this, pnlListDiemDanhNew->getContentSize());
		this->tblListDiemDanh->setAnchorPoint(pnlListDiemDanhNew->getAnchorPoint());
		this->tblListDiemDanh->setContentSize(pnlListDiemDanhNew->getContentSize());
		this->tblListDiemDanh->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
		this->tblListDiemDanh->setPosition(pnlListDiemDanhNew->getPosition());
		this->tblListDiemDanh->setDelegate(this);
		this->tblListDiemDanh->setVisible(false);
		this->tblListDiemDanh->setTag(tag_tbl_diemdanh);
		this->tblListDiemDanh->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);

		this->layoutDiemDanhNew->addChild(this->tblListDiemDanh);
	}

	this->tblListNhiemVu1Lan = TableView::create(this, Size(pnlListDiemDanh->getContentSize().width, pnlListDiemDanh->getContentSize().height));
	this->tblListNhiemVu1Lan->setAnchorPoint(pnlListDiemDanh->getAnchorPoint());
	this->tblListNhiemVu1Lan->setContentSize(pnlListDiemDanh->getContentSize());
	this->tblListNhiemVu1Lan->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	this->tblListNhiemVu1Lan->setPosition(pnlListDiemDanh->getPosition());
	this->tblListNhiemVu1Lan->setDelegate(this);
	this->tblListNhiemVu1Lan->setVisible(true);
	this->tblListNhiemVu1Lan->setTag(tag_tbl_nhiemvu1lan);
	this->tblListNhiemVu1Lan->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	layoutDiemDanh->addChild(this->tblListNhiemVu1Lan);
    
    
    
    this->tblListNhiemVuTanThu = TableView::create(this, Size(pnlListDiemDanh->getContentSize().width, pnlListDiemDanh->getContentSize().height));
    this->tblListNhiemVuTanThu->setAnchorPoint(pnlListDiemDanh->getAnchorPoint());
    this->tblListNhiemVuTanThu->setContentSize(pnlListDiemDanh->getContentSize());
    this->tblListNhiemVuTanThu->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblListNhiemVuTanThu->setPosition(pnlListDiemDanh->getPosition());
    this->tblListNhiemVuTanThu->setDelegate(this);
    this->tblListNhiemVuTanThu->setVisible(true);
    this->tblListNhiemVuTanThu->setTag(tag_tbl_nhiemvutanthu);
    this->tblListNhiemVuTanThu->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    layoutDiemDanh->addChild(this->tblListNhiemVuTanThu);


	//Update Nhiem vu ngay 06/12/2016
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{
		this->pnlNhiemVuNgay = static_cast<Layout*>(pnlBg->getChildByName("pnlNhiemVuNgay"));
        this->pnlNhiemVuNgay->setVisible(false);
		this->txtTienDoNhiemVuCoDinh = static_cast<Text*>(this->pnlNhiemVuNgay->getChildByName("txtTienDoDone"));
		this->txtStatusNhiemVuCoDinh = static_cast<Text*>(this->pnlNhiemVuNgay->getChildByName("txtStatusKey"));
		this->txtDiemTuanNhiemVuNgay = static_cast<Text*>(this->pnlNhiemVuNgay->getChildByName("txtDiemTuan"));
		this->txtDiemNgayNhiemVuNgay = static_cast<Text*>(this->pnlNhiemVuNgay->getChildByName("txtDiemNgay"));
		if (this->txtDiemTuanNhiemVuNgay)
			this->txtDiemTuanNhiemVuNgay->setString("---");
		this->txtDiemNgayNhiemVuNgay = static_cast<Text*>(this->pnlNhiemVuNgay->getChildByName("txtDiemNgay"));
		if (this->txtDiemNgayNhiemVuNgay)
			this->txtDiemNgayNhiemVuNgay->setString("---");
		if (this->txtTienDoNhiemVuCoDinh)
			this->txtTienDoNhiemVuCoDinh->setString("---");
		if (this->txtStatusNhiemVuCoDinh)
			this->txtStatusNhiemVuCoDinh->setString("---");

		Button* btnDoiDiemNhiemVuNgay = static_cast<Button*>(this->pnlNhiemVuNgay->getChildByName("btnDoiDiem"));
		if (btnDoiDiemNhiemVuNgay){
			btnDoiDiemNhiemVuNgay->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnDoiDiem, this));
			btnDoiDiemNhiemVuNgay->setPressedActionEnabled(true);
		}
		this->btnNhanKeyNhiemVuNgay = static_cast<Button*>(this->pnlNhiemVuNgay->getChildByName("btnNhanKey"));
		if (this->btnNhanKeyNhiemVuNgay){
			this->btnNhanKeyNhiemVuNgay->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnNhanKey, this));
			this->btnNhanKeyNhiemVuNgay->setPressedActionEnabled(true);
			this->btnNhanKeyNhiemVuNgay->setVisible(false);
		}

		Button* btnTheLe = static_cast<Button*>(this->pnlNhiemVuNgay->getChildByName("btnTheLe"));
		if (btnTheLe){
			btnTheLe->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnTheLe, this));
			btnTheLe->setPressedActionEnabled(true);
		}

		Layout* pnlTableNhiemVuNgay = static_cast<Layout*>(this->pnlNhiemVuNgay->getChildByName("Panel_4"));
		pnlTableNhiemVuNgay->setVisible(false);

		this->tblListNhiemVuNgay = TableView::create(this, Size(pnlTableNhiemVuNgay->getContentSize().width, pnlTableNhiemVuNgay->getContentSize().height));
		this->tblListNhiemVuNgay->setAnchorPoint(pnlTableNhiemVuNgay->getAnchorPoint());
		this->tblListNhiemVuNgay->setContentSize(pnlTableNhiemVuNgay->getContentSize());
		this->tblListNhiemVuNgay->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
		this->tblListNhiemVuNgay->setPosition(pnlTableNhiemVuNgay->getPosition());
		this->tblListNhiemVuNgay->setDelegate(this);
		this->tblListNhiemVuNgay->setVisible(true);
		this->tblListNhiemVuNgay->setTag(tag_tbl_nhiemvungay);
		this->tblListNhiemVuNgay->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		this->pnlNhiemVuNgay->addChild(this->tblListNhiemVuNgay);
	}
	else{
		this->tblListNhiemVuNgay = TableView::create(this, Size(pnlListDiemDanh->getContentSize().width, pnlListDiemDanh->getContentSize().height));
		this->tblListNhiemVuNgay->setAnchorPoint(pnlListDiemDanh->getAnchorPoint());
		this->tblListNhiemVuNgay->setContentSize(pnlListDiemDanh->getContentSize());
		this->tblListNhiemVuNgay->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
		this->tblListNhiemVuNgay->setPosition(pnlListDiemDanh->getPosition());
		this->tblListNhiemVuNgay->setDelegate(this);
		this->tblListNhiemVuNgay->setVisible(true);
		this->tblListNhiemVuNgay->setTag(tag_tbl_nhiemvungay);
		this->tblListNhiemVuNgay->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		layoutDiemDanh->addChild(this->tblListNhiemVuNgay);
	}
	//Facebook Bai Moc
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	{
		this->pnlCongDong = static_cast<Layout*>(pnlBg->getChildByName("pnlCongDong"));
		this->btnTabCongDong = static_cast<Button*>(pnlBg->getChildByName("btnCongDong"));
		if (this->btnTabCongDong){
			this->btnTabCongDong->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnTabCongDong, this));
			this->btnTabCongDong->setPressedActionEnabled(true);
		}
		auto bonusls = mUtils::splitString(dataManager.getAppConfig().isfb, '_');
		this->txtGoldMoi = static_cast<Text*>(this->pnlCongDong->getChildByName("txtGoldMoi"));
		this->txtNhanShare = static_cast<Text*>(this->pnlCongDong->getChildByName("txtNhanShare"));
		if (bonusls.size() >= 2){
			this->txtGoldMoi->setString(mUtils::convertMoneyEx(atof(bonusls[0].c_str())) + " Vàng");
			this->txtNhanShare->setString(mUtils::convertMoneyEx(atof(bonusls[1].c_str())) + " Vàng");

		}
		this->btnNhanMoi = static_cast<Button*>(this->pnlCongDong->getChildByName("btnNhanMoi"));
		if (this->btnNhanMoi){
			this->btnNhanMoi->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnNhanMoiFB, this));
			this->btnNhanMoi->setPressedActionEnabled(true);
		}
		this->btnNhanShare = static_cast<Button*>(this->pnlCongDong->getChildByName("btnNhanShare"));
		if (this->btnNhanShare){
			this->btnNhanShare->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnNhanShareFB, this));
			this->btnNhanShare->setPressedActionEnabled(true);
		}
		Button* btnChonTatCa = static_cast<Button*>(this->pnlCongDong->getChildByName("btnChonTatCa"));
		if (btnChonTatCa){
			btnChonTatCa->addClickEventListener(CC_CALLBACK_1(LayerPopupMission::onBtnChonTatCa, this));
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
	}

	onBtnTabNhiemVuNgay(NULL);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	IOS::logScreenToGA("Đang mở: Nhiệm Vụ");
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	string screen = "Nhiệm Vụ";
	NativeHelper::trackScreen(screen.c_str());
#endif

	return true;
}
void LayerPopupMission::onBtnNhanMoiFB(Ref* pSender){

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
void LayerPopupMission::onBtnNhanShareFB(Ref* pSender){


#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//face->onRequestPublishPermission();
#endif

}
void LayerPopupMission::onBtnChonTatCa(Ref* pSender){
	SceneManager::getSingleton().clearInviteUserIdx();
	for (int i = 0; i < SceneManager::getSingleton().getInviteableUsers().size(); i++){

		SceneManager::getSingleton().pushInviteUserIdxs(SceneManager::getSingleton().getInviteableUsers()[i].uid);
		SceneManager::getSingleton().setInviteInviteableUsers(i, true);

	}
	this->tblFriendFB->reloadData();
}
void LayerPopupMission::onBtnHuongDanDiemDanh(Ref* pSender){
	string strlink = "";
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
		strlink = "https://baimoc.com/ingame/huong-dan-diem-danh.html";
	}
	LayerPopupWebView* _layer = LayerPopupWebView::create();
	_layer->setWebView(strlink);
	this->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_WEBVIEW);

}
void LayerPopupMission::onBtnTheLe(Ref* pSender){
	//    auto _currScene = Director::getInstance()->getRunningScene();
	//    if (!_currScene) return;
	string strlink = "";
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
		//strlink = QUYDINH_DOITHUONG_P2;
	}
	else{
		strlink = THELE_NHIEMVU;
	}
	//#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//    strlink += "i";
	//#endif
	//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//    strlink += "an";
	//#endif
	LayerPopupWebView* _layer = LayerPopupWebView::create();
	_layer->setWebView(strlink);
	this->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_WEBVIEW);
}

void LayerPopupMission::onBtnNhanKey(Ref* pSender){
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest("eegkbrq", params));
	GameServer::getSingleton().Send(request);
	SceneManager::getSingleton().showLoading();
}
void LayerPopupMission::onBtnDoiDiem(Ref* pSender){
	LayerPopupDoiDiem* popup = LayerPopupDoiDiem::create();
	this->addChild(popup);
	//    LayerPopupHetTien* popup = LayerPopupHetTien::create();
	//    this->addChild(popup);
}
void LayerPopupMission::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerPopupMission::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
		switch (table->getTag()) {
		case tag_tbl_diemdanh:
			return Size(table->getContentSize().width, 150);
		case tag_tbl_nhiemvu1lan:
			return Size(table->getContentSize().width, 150);
        case tag_tbl_nhiemvutanthu:
                return Size(table->getContentSize().width, 150);
		case tag_tbl_nhiemvungay:
			return Size(table->getContentSize().width, 82);
		}
		return Size(0, 0);
	}
	else{
		switch (table->getTag()) {
		case tag_tbl_diemdanh:
			return Size(table->getContentSize().width, 205);
		case tag_tbl_nhiemvu1lan:
			return Size(table->getContentSize().width, 150);
		case tag_tbl_nhiemvungay:
			return Size(table->getContentSize().width, 150);
		case tag_tbl_friendfb:
			return Size(table->getContentSize().width, 250);
		}
		return Size(0, 0);
	}
}

TableViewCell* LayerPopupMission::tableCellAtIndex(TableView *table, ssize_t idx)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
		switch (table->getTag()) {
		case tag_tbl_diemdanh:
			return createCell4LstDiemDanh(table, idx);
		case tag_tbl_nhiemvu1lan:
			return createCell4LstNhiemVu1Lan(table, idx);
		case tag_tbl_nhiemvungay:
			return createCell4LstNhiemVuNgay(table, idx);
        case tag_tbl_nhiemvutanthu:
            return createCell4LstNhiemVuTanThu(table, idx);
		}
    		return NULL;
	}
	else{
		switch (table->getTag()) {
		case tag_tbl_diemdanh:
			return createCell4LstDiemDanh(table, idx);
		case tag_tbl_nhiemvu1lan:
			return createCell4LstNhiemVu1Lan(table, idx);
		case tag_tbl_nhiemvungay:
			return createCell4LstNhiemVuNgay(table, idx);
		case tag_tbl_friendfb:
			return createCell4LstFriendFacebook(table, idx);
		}
		return NULL;
	}
}

TableViewCell* LayerPopupMission::createCell4LstFriendFacebook(TableView *table, ssize_t idx){
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
	auto _child_ = LayerItemFriendFacebook::create();
	auto sizeTable = this->pnlFriend->getContentSize();
	auto width = _child_->getContentSize().width;
	auto startX = (sizeTable.width - (_child_->getContentSize().width)*cols - 30 * (cols - 1)) / 2;

	//comment
	for (int i = 0; i < cols; i++) {
		int index = idx *cols + i;
		if (index < SceneManager::getSingleton().getInviteableUsers().size())
		{
			auto _child = LayerItemFriendFacebook::create();
			//_child->onInviteFriendCallBack(this, callfuncO_selector(LayerPopupMission::onBtnNhanMoiFB));
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

TableViewCell* LayerPopupMission::createCell4LstDiemDanh(TableView *table, ssize_t idx){
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

	auto cols = 3;
	auto _child_ = LayerItemDiemDanhP2::create();
	auto sizeTable = Size(1120, 490);
	auto width = _child_->getContentSize().width;
	int space = 60;
	auto startX = (sizeTable.width - (width)*cols - space * (cols - 1)) / 2;

	//comment
	for (int i = 0; i < cols; i++) {
		int index = idx *cols + i;
		if (index < this->lstDiemDanhP2.size())
		{
			auto _child = LayerItemDiemDanhP2::create();
			_child->setPosition(startX + (i * width) + space * i, -35);

			cell->addChild(_child, 0, _cTag + i);
			_child->setDatas(this->lstDiemDanhP2[index].stt, this->lstDiemDanhP2[index].phanthuong, this->lstDiemDanhP2[index].isCheck);
			_child->setVisible(true);
			_child->setTag(index);
		}
	}
	return cell;
}
TableViewCell* LayerPopupMission::createCell4LstNhiemVuTanThu(TableView *table, ssize_t idx){
    auto cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        auto nhiemvuCell = LayerItemTanThu::create();
        nhiemvuCell->setAnchorPoint(Vec2(0.5, 0.5));
        nhiemvuCell->setPosition(Vec2(cell->getContentSize().width / 2 + 20, cell->getContentSize().height / 2 + 20));
        nhiemvuCell->setDatas(lstTanThu[idx].id, lstTanThu[idx].name, lstTanThu[idx].status, lstTanThu[idx].bonus, lstTanThu[idx].receive,lstTanThu[idx].type);
        nhiemvuCell->setTag(tag_cell_nhiemvu1lan);
        cell->addChild(nhiemvuCell);
        cell->setIdx(idx);
    }
    else
    {
        LayerItemTanThu* nhiemvuCell = (LayerItemTanThu*)cell->getChildByTag(tag_cell_nhiemvu1lan);
        if (nhiemvuCell){
            nhiemvuCell->setDatas(lstTanThu[idx].id, lstTanThu[idx].name, lstTanThu[idx].status, lstTanThu[idx].bonus, lstTanThu[idx].receive,lstTanThu[idx].type);
            
        }
    }
    return cell;

}
TableViewCell* LayerPopupMission::createCell4LstNhiemVu1Lan(TableView *table, ssize_t idx){
	auto cell = table->dequeueCell();
	if (!cell)
	{
		cell = new TableViewCell();
		auto nhiemvuCell = LayerItemThanhTuu::create();
		nhiemvuCell->setAnchorPoint(Vec2(0.5, 0.5));
		nhiemvuCell->setPosition(Vec2(cell->getContentSize().width / 2 + 20, cell->getContentSize().height / 2 + 20));
		nhiemvuCell->setDatas(this->lstThanhTuu[idx].id, this->lstThanhTuu[idx].name, this->lstThanhTuu[idx].star1, this->lstThanhTuu[idx].star2,this->lstThanhTuu[idx].star3,this->lstThanhTuu[idx].star4,this->lstThanhTuu[idx].star5 , this->lstThanhTuu[idx].bonus,this->lstThanhTuu[idx].currStar,this->lstThanhTuu[idx].getStar);
		nhiemvuCell->setTag(tag_cell_nhiemvu1lan);
		cell->addChild(nhiemvuCell);
		cell->setIdx(idx);
	}
	else
	{
		LayerItemThanhTuu* nhiemvuCell = (LayerItemThanhTuu*)cell->getChildByTag(tag_cell_nhiemvu1lan);
		if (nhiemvuCell){
			nhiemvuCell->setDatas(this->lstThanhTuu[idx].id, this->lstThanhTuu[idx].name, this->lstThanhTuu[idx].star1, this->lstThanhTuu[idx].star2,this->lstThanhTuu[idx].star3,this->lstThanhTuu[idx].star4,this->lstThanhTuu[idx].star5 , this->lstThanhTuu[idx].bonus,this->lstThanhTuu[idx].currStar,this->lstThanhTuu[idx].getStar);

		}
	}
	return cell;

}
TableViewCell* LayerPopupMission::createCell4LstNhiemVuNgay(TableView *table, ssize_t idx){
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){

		auto cell = table->dequeueCell();
		if (!cell)
		{
			cell = new TableViewCell();
			auto nhiemvuCell = LayerItemNhiemVuNgayNew::create();
			nhiemvuCell->setAnchorPoint(Vec2(0.5, 0.5));
			nhiemvuCell->setDatas(lstNhiemvuNgayNew[idx].id, lstNhiemvuNgayNew[idx].name, lstNhiemvuNgayNew[idx].socap, lstNhiemvuNgayNew[idx].khoinghiep, lstNhiemvuNgayNew[idx].trieuphu, lstNhiemvuNgayNew[idx].daigia, lstNhiemvuNgayNew[idx].capdohientai, lstNhiemvuNgayNew[idx].tongcapdo, lstNhiemvuNgayNew[idx].tiendohientai, lstNhiemvuNgayNew[idx].tongtiendo, lstNhiemvuNgayNew[idx].diem);
			//
			//
			//		nhiemvuCell->setPosition(Vec2(cell->getContentSize().width / 2 + 20, cell->getContentSize().height / 2 + 20));
			nhiemvuCell->setTag(tag_cell_nhiemvu1lan);
			cell->addChild(nhiemvuCell);
			cell->setIdx(idx);
		}
		else
		{
			cell->setIdx(idx);
			LayerItemNhiemVuNgayNew* nhiemvuCell = (LayerItemNhiemVuNgayNew*)cell->getChildByTag(tag_cell_nhiemvu1lan);
			if (nhiemvuCell){
				nhiemvuCell->setDatas(lstNhiemvuNgayNew[idx].id, lstNhiemvuNgayNew[idx].name, lstNhiemvuNgayNew[idx].socap, lstNhiemvuNgayNew[idx].khoinghiep, lstNhiemvuNgayNew[idx].trieuphu, lstNhiemvuNgayNew[idx].daigia, lstNhiemvuNgayNew[idx].capdohientai, lstNhiemvuNgayNew[idx].tongcapdo, lstNhiemvuNgayNew[idx].tiendohientai, lstNhiemvuNgayNew[idx].tongtiendo, lstNhiemvuNgayNew[idx].diem);
			}
		}
		return cell;

	}
	else{
		auto cell = table->dequeueCell();
		if (!cell)
		{
			cell = new TableViewCell();
			auto nhiemvuCell = LayerItemNhiemVuMotLan::create();
			nhiemvuCell->setAnchorPoint(Vec2(0.5, 0.5));
			nhiemvuCell->setDatas("0", lstNhiemvuNgay[idx].name, lstNhiemvuNgay[idx].status, lstNhiemvuNgay[idx].type, lstNhiemvuNgay[idx].bonus, false);


			nhiemvuCell->setPosition(Vec2(cell->getContentSize().width / 2 + 20, cell->getContentSize().height / 2 + 20));
			nhiemvuCell->setTag(tag_cell_nhiemvu1lan);
			cell->addChild(nhiemvuCell);
			cell->setIdx(idx);
		}
		else
		{
			cell->setIdx(idx);
			LayerItemNhiemVuMotLan* nhiemvuCell = (LayerItemNhiemVuMotLan*)cell->getChildByTag(tag_cell_nhiemvu1lan);
			if (nhiemvuCell){
				nhiemvuCell->setDatas("0", lstNhiemvuNgay[idx].name, lstNhiemvuNgay[idx].status, lstNhiemvuNgay[idx].type, lstNhiemvuNgay[idx].bonus, false);
			}
		}
		return cell;
	}

}
ssize_t LayerPopupMission::numberOfCellsInTableView(TableView *table)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
		switch (table->getTag()) {
		case tag_tbl_diemdanh:
			return getCellDiemDanh();
		case tag_tbl_nhiemvu1lan:
			return this->lstThanhTuu.size();
		case tag_tbl_nhiemvungay:
			return lstNhiemvuNgayNew.size();
                case tag_tbl_nhiemvutanthu:
                return lstTanThu.size();
		}
		return 0;
	}
	else{
		int rowItem = 0;
		int sizeList = SceneManager::getSingleton().getInviteableUsers().size();
		if (sizeList % 4 == 0){
			rowItem = sizeList / 4;
		}
		else{
			rowItem = sizeList / 4 + 1;
		}
		switch (table->getTag()) {
		case tag_tbl_diemdanh:
			return getCellDiemDanh();
		case tag_tbl_nhiemvu1lan:
			return lstNhiemvu1Lan.size();
		case tag_tbl_nhiemvungay:
			return lstNhiemvuNgay.size();
		case tag_tbl_friendfb:
			return rowItem;
		}
		return 0;
	}

}
int LayerPopupMission::getCellDiemDanh(){
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
		if (this->lstDiemDanh.size() == 0)
			return 0;
		if (this->lstDiemDanh.size() < 4)
			return 1;
		int rows = this->lstDiemDanh.size() / 4;
		if (this->lstDiemDanh.size() % 4 != 0)
			rows += 1;
		return rows;
	}
	else{
		if (this->lstDiemDanhP2.size() == 0)
			return 0;
		if (this->lstDiemDanhP2.size() < 4)
			return 1;
		int rows = this->lstDiemDanhP2.size() / 4;
		if (this->lstDiemDanhP2.size() % 4 != 0)
			rows += 1;
		return rows;
	}
}
void LayerPopupMission::onBtnClose(Ref* pSender){
	SceneMain* parent = (SceneMain*)this->getParent();
	if (parent) {
		parent->showEvent();
	}
	this->removeFromParentAndCleanup(true);
}
string LayerPopupMission::getCurrentTime(){
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);

	int day = aTime->tm_mday;
	int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
	int year = aTime->tm_year + 1900;

	return StringUtils::format("%d/%d", month, year).c_str();
}
void LayerPopupMission::showFriend(Node* sender, void* data){
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
void LayerPopupMission::onBtnTabCongDong(Ref* pSender){

	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		curr_tab = 3;
		SceneManager::getSingleton().clearInviteableUsers();
		SceneManager::getSingleton().clearInviteUserIdx();
#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		face->LoginForFriend();
#endif
		//only for dev win32
		//this->fakeList();
		this->lblDes->setVisible(false);
		btnTabNhiemVu->setEnabled(true);
		btnTagDiemDanh->setEnabled(true);
		btnTabNhiemVuNgay->setEnabled(true);
		this->btnTabCongDong->setEnabled(false);
		this->pnlCongDong->setVisible(true);

		this->layoutDiemDanhNew->setVisible(false);
		this->layoutDiemDanh->setVisible(false);

		this->tblListDiemDanh->setVisible(false);
		this->tblListNhiemVuNgay->setVisible(false);
		this->tblListNhiemVu1Lan->setVisible(false);
		this->tblFriendFB->setVisible(true);
		this->tblFriendFB->reloadData();

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> request(new ExtensionRequest("eegifbrq", params));
		GameServer::getSingleton().Send(request);
		SceneManager::getSingleton().showLoading(40);
		this->btnNhanMoi->setEnabled(false);
		this->btnNhanShare->setEnabled(false);
	}
}
void LayerPopupMission::onBtnNhiemVuTanThu(cocos2d::Ref *pSender){
    this->lblDes->setVisible(false);
    this->pnlDiemDanhNew->setVisible(false);
    this->pnlNhiemVuNgay->setVisible(false);
    
    layoutDiemDanh->setVisible(true);
    btnTabNhiemVu->setEnabled(true);
    btnNhiemVuTanThu->setEnabled(false);
    btnTagDiemDanh->setEnabled(true);
    btnTabNhiemVuNgay->setEnabled(true);
    
    //this->tblListDiemDanh->setVisible(false);
    this->tblListNhiemVuNgay->setVisible(false);
    this->tblListNhiemVu1Lan->setVisible(false);
    this->tblListNhiemVuTanThu->setVisible(true);
    this->tblListNhiemVuTanThu->reloadData();
    
  
}

void LayerPopupMission::onBtnTabNhiemVu(cocos2d::Ref *pSender){

	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
		this->lblDes->setVisible(false);
		this->pnlDiemDanhNew->setVisible(false);
		this->pnlNhiemVuNgay->setVisible(false);
        btnNhiemVuTanThu->setEnabled(true);


		layoutDiemDanh->setVisible(true);
		btnTabNhiemVu->setEnabled(false);
		btnTagDiemDanh->setEnabled(true);
		btnTabNhiemVuNgay->setEnabled(true);

		//this->tblListDiemDanh->setVisible(false);
		this->tblListNhiemVuNgay->setVisible(false);
		this->tblListNhiemVu1Lan->setVisible(true);
		this->tblListNhiemVu1Lan->reloadData();
        this->tblListNhiemVuTanThu->setVisible(false);


		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_ARCHIEMENT_REQ, params));
		GameServer::getSingleton().Send(request);
		SceneManager::getSingleton().showLoading();
	}
	else{
		curr_tab = 2;
		this->lblDes->setVisible(false);

		btnTabNhiemVu->setEnabled(false);
		btnTagDiemDanh->setEnabled(true);
		btnTabNhiemVuNgay->setEnabled(true);

		this->btnTabCongDong->setEnabled(true);
		this->pnlCongDong->setVisible(false);

		this->layoutDiemDanhNew->setVisible(false);
		this->layoutDiemDanh->setVisible(true);

		this->tblListDiemDanh->setVisible(false);
		this->tblListNhiemVuNgay->setVisible(false);
		this->tblFriendFB->setVisible(false);
		this->tblListNhiemVu1Lan->setVisible(true);
		this->tblListNhiemVu1Lan->reloadData();

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rldot", params));
		GameServer::getSingleton().Send(request);
		SceneManager::getSingleton().showLoading();
	}
}
void LayerPopupMission::onBtnTabNhiemVuNgay(cocos2d::Ref *pSender){

	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
		this->pnlDiemDanhNew->setVisible(false);
		this->lblDes->setVisible(false);
		btnTabNhiemVu->setEnabled(true);
		btnTabNhiemVuNgay->setEnabled(false);
		btnTagDiemDanh->setEnabled(true);
        btnNhiemVuTanThu->setEnabled(true);


		this->pnlNhiemVuNgay->setVisible(true);

		layoutDiemDanh->setVisible(false);

		this->tblListNhiemVu1Lan->setVisible(false);
		this->tblListNhiemVuNgay->setVisible(true);
		//this->tblListDiemDanh->setVisible(false);
        this->tblListNhiemVuTanThu->setVisible(false);

		this->tblListNhiemVuNgay->reloadData();

		boost::shared_ptr<ISFSObject> params1(new SFSObject());
		boost::shared_ptr<IRequest> request1(new ExtensionRequest("eegprq", params1));
		GameServer::getSingleton().Send(request1);

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rldt", params));
		GameServer::getSingleton().Send(request);
		SceneManager::getSingleton().showLoading();
	}
	else{
		curr_tab = 1;
		this->lblDes->setVisible(false);
		btnTabNhiemVu->setEnabled(true);
		btnTabNhiemVuNgay->setEnabled(false);
		btnTagDiemDanh->setEnabled(true);
		this->layoutDiemDanhNew->setVisible(false);
		this->layoutDiemDanh->setVisible(true);

		this->btnTabCongDong->setEnabled(true);
		this->pnlCongDong->setVisible(false);

		this->tblListNhiemVu1Lan->setVisible(false);
		this->tblListNhiemVuNgay->setVisible(true);
		this->tblListDiemDanh->setVisible(false);
		this->tblFriendFB->setVisible(false);
		this->tblListNhiemVuNgay->reloadData();

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rldt", params));
		GameServer::getSingleton().Send(request);
		SceneManager::getSingleton().showLoading();
	}
}
void LayerPopupMission::onBtnTabDiemDanh(cocos2d::Ref *pSender){
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{
		this->lblDes->setVisible(false);

		layoutDiemDanh->setVisible(false);
		btnTabNhiemVu->setEnabled(true);
		btnTabNhiemVuNgay->setEnabled(true);
		btnTagDiemDanh->setEnabled(false);
        btnNhiemVuTanThu->setEnabled(true);


		this->pnlNhiemVuNgay->setVisible(false);
		this->pnlDiemDanhNew->setVisible(true);


		this->tblListNhiemVuNgay->setVisible(false);
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
			this->tblListDiemDanh->setVisible(true);
		}
		else{
			//this->tblListDiemDanh->setVisible(true);
		}
		this->tblListNhiemVu1Lan->setVisible(false);
        this->tblListNhiemVuTanThu->setVisible(false);


		boost::shared_ptr<ISFSObject> params1(new SFSObject());
		boost::shared_ptr<IRequest> request1(new ExtensionRequest("eegprq", params1));
		GameServer::getSingleton().Send(request1);

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		string dateTime = getCurrentTime();
		params->PutUtfString("month", dateTime);
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rla", params));
		GameServer::getSingleton().Send(request);
		SceneManager::getSingleton().showLoading();
	}
	else{
		curr_tab = 4;
		this->lblDes->setVisible(true);

		this->layoutDiemDanh->setVisible(false);
		btnTabNhiemVu->setEnabled(true);
		btnTabNhiemVuNgay->setEnabled(true);
		btnTagDiemDanh->setEnabled(false);

		this->btnTabCongDong->setEnabled(true);
		this->pnlCongDong->setVisible(false);

		this->tblListNhiemVuNgay->setVisible(false);
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
			this->tblListDiemDanh->setVisible(true);
		}
		else{
			this->tblListDiemDanh->setVisible(true);
		}
		this->tblListNhiemVu1Lan->setVisible(false);
		this->tblFriendFB->setVisible(false);
		this->layoutDiemDanhNew->setVisible(true);

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		string dateTime = getCurrentTime();
		params->PutUtfString("month", dateTime);
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rla", params));
		GameServer::getSingleton().Send(request);
		SceneManager::getSingleton().showLoading();
	}
}
void LayerPopupMission::doDiemDanh(){
    this->isDiemDanh = true;
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
	//	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	//	{
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutInt("osid", osid);
	params->PutUtfString("devid", devID);
	params->PutInt("srcref", atoi(srcID.c_str()));
	boost::shared_ptr<IRequest> request(new ExtensionRequest("ra", params));
	GameServer::getSingleton().Send(request);
	dataManager.getAppConfig().isLoginFisrtDay = false;
	onBtnTabDiemDanh(NULL);
    SceneManager::getSingleton().showLoading();

	//	}
	//	else if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	//	{
	//		if (dataManager.getAppConfig().isHideBilling)//for puslish to store
	//			return;
	//		///diem danh lan dau tien trong ngay cho vao quay thuong hang ngay luon +  request ban tin diem danh
	//		//dataManager.getAppConfig().isLoginFisrtDay = false;
	//		//onBtnTabNhiemVuNgay(NULL);
	//		auto currScene = this->getParent();
	//		if (currScene != NULL)
	//		{
	//			QuayThuongHN* quaythuongHN = QuayThuongHN::create();
	//			//cheat zoder only for scene main
	//			currScene->addChild(quaythuongHN, 10000000);
	//			//currScene->addChild(quaythuongHN, ZORDER_LIST::ZORDER_QUAYTHUONG_HN);
	//			quaythuongHN->sendRequestAttend(osid, devID, srcID);
	//			//cheat for show only popup event - gamep2
	//			this->removeFromParent();
	//		}
	//	}

}

void LayerPopupMission::doGetBonusFB() {
	onBtnTabNhiemVu(NULL);
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest("rfb", params));
	GameServer::getSingleton().Send(request);
	SceneManager::getSingleton().showLoading();
}

void LayerPopupMission::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(EXT_EVENT_NEW_MISSION_RESP,cmd->c_str()) == 0){
        if (param->GetInt(AWARD_SILVER) == NULL || param->GetInt("rc") == NULL || param->GetInt(AWARD_GOLD) == NULL || param->GetUtfString("rd") == NULL) return;
        SceneManager::getSingleton().hideLoading();
        int rc = *param->GetInt("rc");
        if (rc == 0){
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_MISSION_REQ, params));
            GameServer::getSingleton().Send(request);
            int silver = *param->GetInt(AWARD_SILVER);
            int gold = *param->GetInt(AWARD_GOLD);
            
            string content = "Bạn nhận được ";
            
            if (silver > 0)
                content += StringUtils::format("%s %s ",mUtils::convertMoneyEx(silver).c_str(),SceneManager::getSingleton().getCoin(1).c_str());
            if (gold > 0)
                content += StringUtils::format("%s %s ",mUtils::convertMoneyEx(gold).c_str(),SceneManager::getSingleton().getCoin(0).c_str());
        
            content += "!";
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(content);
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,10000000,9053);
            
        }
        else{
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(*param->GetUtfString("rd"));
            this->addChild(layerPopup);
            
        }
        
    }

    
     if (strcmp(EXT_EVENT_ARCHIEMENT_RESP,cmd->c_str()) == 0){
         if (param->GetInt(AWARD_SILVER) == NULL || param->GetInt("rc") == NULL || param->GetInt(AWARD_GOLD) == NULL || param->GetUtfString("rd") == NULL) return;
         SceneManager::getSingleton().hideLoading();
         int rc = *param->GetInt("rc");
         if (rc == 0){
             
             boost::shared_ptr<ISFSObject> params(new SFSObject());
             boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_ARCHIEMENT_REQ, params));
             GameServer::getSingleton().Send(request);
             
            int silver = *param->GetInt(AWARD_SILVER);
             int gold = *param->GetInt(AWARD_GOLD);
             
             string content = "Bạn nhận được ";
             
             if (silver > 0)
                 content += StringUtils::format("%s %s ",mUtils::convertMoneyEx(silver).c_str(),SceneManager::getSingleton().getCoin(1).c_str());
             if (gold > 0)
                 content += StringUtils::format("%s %s ",mUtils::convertMoneyEx(gold).c_str(),SceneManager::getSingleton().getCoin(0).c_str());
             content += "!";
             auto _currScene = Director::getInstance()->getRunningScene();
             if (!_currScene) return;
             
             LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
             layerPopup->setMessage(dataManager.GetSysString(141));
             layerPopup->setContentMess(content);
             layerPopup->setTag(24);
             layerPopup->showAButton();
             _currScene->addChild(layerPopup,10000000,54);
         }
         else{
             LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
             layerPopup->showAButton();
             layerPopup->setMessage(dataManager.GetSysString(141));
             layerPopup->setContentMess(*param->GetUtfString("rd"));
             this->addChild(layerPopup);
             
         }

     }
    if (strcmp(EXT_EVENT_GET_MISSION_RESP,cmd->c_str()) == 0){
        if (param->GetUtfString(MISSION_RESULT_LIST) == NULL) return;
        SceneManager::getSingleton().hideLoading();
        //"176|1|Liên kết tài khoản Facebook|50|5000|0|0|0;177|2|Kích hoạt OTP|50|5000|0|0|0;178|3|Cập nhật Email|0|20000|0|0|0;179|4|Hoàn thành mốc Chăm chỉ ngày|150|20000|0|0|0;180|5|Nạp tiền lần đầu tiên|100|100000|0|0|0;";//
        string strMisson = *param->GetUtfString(MISSION_RESULT_LIST);
        if (strMisson == ""){
            this->btnNhiemVuTanThu->setVisible(false);
            this->btnTabNhiemVu->setVisible(true);
            this->btnTabNhiemVuNgay->setVisible(true);
            this->btnTagDiemDanh->setPosition(this->btnTabNhiemVu->getPosition());
            this->btnTabNhiemVu->setPosition(this->btnTabNhiemVuNgay->getPosition());
            this->btnTabNhiemVuNgay->setPosition(this->btnNhiemVuTanThu->getPosition());
            this->btnTagDiemDanh->setVisible(true);
            if (!this->isDiemDanh)
                onBtnTabNhiemVuNgay(NULL);
        }else{
            this->btnNhiemVuTanThu->setVisible(true);
            this->btnTabNhiemVu->setVisible(true);
            this->btnTabNhiemVuNgay->setVisible(true);
            this->btnTagDiemDanh->setVisible(true);
            if (!this->isDiemDanh)
                onBtnNhiemVuTanThu(NULL);
        }
       // log("nhiệm vụ: %s", strMisson.c_str());
        
        if (strcmp("",strMisson.c_str()) == 0){
            this->btnNhiemVuTanThu->setVisible(false);
        }else{
            this->btnNhiemVuTanThu->setVisible(true);
        }
        this->lstTanThu.clear();
        
        auto lstMission = mUtils::splitString(strMisson, ';');
        for (int i = 0 ;i < lstMission.size();i++){
            auto itemMission = mUtils::splitString(lstMission[i], '|');
            if (itemMission.size()<9)
                continue;
            TanThu it;
            it.id = itemMission[1];
            it.name = itemMission[2];
            it.bonus = itemMission[3]+ "-"+itemMission[4]+"-"+itemMission[5];
            it.status = itemMission[6];
            it.receive = itemMission[7];
            it.type = itemMission[8];
            this->lstTanThu.push_back(it);
        }
        this->tblListNhiemVuTanThu->reloadData();

    }
    
    if (strcmp(EXT_EVENT_GET_ARCHIEMENT_RESP,cmd->c_str()) == 0){
        if (param->GetUtfString(ARCHIEMENT_RESULT) == NULL) return;
        SceneManager::getSingleton().hideLoading();
        string strArch = *param->GetUtfString(ARCHIEMENT_RESULT);
       // log("thành tựu: %s", strArch.c_str());
        //661|1|Thắng ... ván game bất kỳ|20-0:20-0|50-0:50-0|100-0:100-0|200-0:200-0|500-0:500-0|0-10000;
        
        auto lsttt = mUtils::splitString(strArch, ';');
        this->lstThanhTuu.clear();
        for (int i  = 0; i < lsttt.size() ; i++){
            auto lstit = mUtils::splitString(lsttt[i], '|');
            if (lstit.size() < 10)
                continue;
            
            ThanhTuu it;
            it.id = lstit[1];
            it.name = lstit[2];
            it.star1 = lstit[3];
            it.star2 = lstit[4];
            it.star3 = lstit[5];
            it.star4 = lstit[6];
            it.star5 = lstit[7];
            it.bonus = lstit[8];
            it.currStar = lstit[9];
            it.getStar = lstit[10];
            this->lstThanhTuu.push_back(it);
        }
        this->tblListNhiemVu1Lan->reloadData();
    }

	if (strcmp("rsa", cmd->c_str()) == 0){
		//only for game p1
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
		{
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			string dateTime = getCurrentTime();
			params->PutUtfString("month", dateTime);
			boost::shared_ptr<IRequest> request(new ExtensionRequest("rla", params));
			GameServer::getSingleton().Send(request);
			SceneManager::getSingleton().showLoading();
			log("ra");

			if (param->GetUtfString("atb") == NULL || param->GetInt("rc") == NULL) return;
			SceneManager::getSingleton().hideLoading();
			int rc = *param->GetInt("rc");
			log("RA :%d", rc);
			if (rc == 0){

				boost::shared_ptr<ISFSObject> params1(new SFSObject());
				boost::shared_ptr<IRequest> request1(new ExtensionRequest("eegprq", params1));
				GameServer::getSingleton().Send(request1);

				string bonus = *param->GetUtfString("atb");
				log("Bonus RA :%s", bonus.c_str());
				auto lstBonus = mUtils::splitString(bonus, '|');
				if (lstBonus.size() < 6) {
					return;
				}
				int tien = atoi(lstBonus[5].c_str());
				int exp = atoi(lstBonus[1].c_str());
				int moruong = atoi(lstBonus[2].c_str());
				int quay = atoi(lstBonus[3].c_str());
				int diemhoatdong = atoi(lstBonus[4].c_str());
				if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
					LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
					layerPopup->showAButton();
					string title = "";
					if (dataManager.getAppConfig().isLoginFisrt)
						title = "Chào mừng bạn";
					else
						title = "Chào mừng quay lại";

					dataManager.getAppConfig().isLoginFisrt = false;

					if (tien > 0){
						layerPopup->setMessage(title);
                        layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s %s quà điểm danh!", mUtils::convertMoneyEx(tien).c_str(), SceneManager::getSingleton().getCoin(1).c_str()));
						this->addChild(layerPopup);
						return;
					}
					else  if (exp > 0){
						if (diemhoatdong > 0){
							layerPopup->setMessage(title);
							layerPopup->setContentMess(StringUtils::format("Bạn nhận được %d điểm hoạt động và %d EXP!", diemhoatdong, exp));
							this->addChild(layerPopup);
							return;
						}

					}
					else  if (moruong > 0){
						layerPopup->setMessage(title);
						layerPopup->setContentMess(StringUtils::format("Bạn nhận được %d lần mở rương quà điểm danh!", moruong));
						this->addChild(layerPopup);
						return;

					}
					else  if (quay > 0){
						layerPopup->setMessage(title);
						layerPopup->setContentMess(StringUtils::format("Bạn nhận được %d lượt quay vòng quay may mắn quà điểm danh!", quay));
						this->addChild(layerPopup);
						return;

					}
				}
			}
		}
		else{
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			string dateTime = getCurrentTime();
			params->PutUtfString("month", dateTime);
			boost::shared_ptr<IRequest> request(new ExtensionRequest("rla", params));
			GameServer::getSingleton().Send(request);
			SceneManager::getSingleton().showLoading();
			log("ra");
			auto a = param->GetUtfString("atb");
			auto b = param->GetUtfString("rc");
			if (param->GetUtfString("atb") == NULL || param->GetInt("rc") == NULL) return;
			SceneManager::getSingleton().hideLoading();
			int rc = *param->GetInt("rc");
			log("RA :%d", rc);
			if (rc == 0){


				string bonus = *param->GetUtfString("atb");
				log("Bonus RA :%s", bonus.c_str());
				auto lstBonus = mUtils::splitString(bonus, '|');
				if (lstBonus.size() < 4) {
					return;
				}
				int tien = atoi(lstBonus[0].c_str());
				int exp = atoi(lstBonus[1].c_str());
				int moruong = atoi(lstBonus[2].c_str());
				int quay = atoi(lstBonus[3].c_str());
				//int diemhoatdong = atoi(lstBonus[4].c_str());
				LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
				layerPopup->showAButton();
				string title = "";
				if (dataManager.getAppConfig().isLoginFisrt)
					title = "Chào mừng bạn";
				else
					title = "Chào mừng quay lại";

				dataManager.getAppConfig().isLoginFisrt = false;

				if (tien > 0){
					layerPopup->setMessage(title);
					layerPopup->setContentMess(StringUtils::format("Bạn nhận được %d %s quà điểm danh!", tien, SceneManager::getSingleton().getCoin(1).c_str()));
					this->addChild(layerPopup);
					return;
				}
				else  if (exp > 0){
					if (tien > 0){
						layerPopup->setMessage(title);
						layerPopup->setContentMess(StringUtils::format("Bạn nhận được %d %s và %d EXP!", tien, SceneManager::getSingleton().getCoin(1).c_str(), exp));
						this->addChild(layerPopup);
						return;
					}

				}
				else  if (moruong > 0){
					layerPopup->setMessage(title);
					layerPopup->setContentMess(StringUtils::format("Bạn nhận được %d lần mở rương quà điểm danh!", moruong));
					this->addChild(layerPopup);
					return;

				}
				else  if (quay > 0){
					layerPopup->setMessage(title);
					layerPopup->setContentMess(StringUtils::format("Bạn nhận được %d lượt quay vòng quay may mắn quà điểm danh!", quay));
					this->addChild(layerPopup);
					return;

				}
			}
		}
	}
	else  if (strcmp("rsfb", cmd->c_str()) == 0){

		if (param->GetUtfString("coinBonus") == NULL || param->GetInt("rc") == NULL) return;
		SceneManager::getSingleton().hideLoading();
		int rc = *param->GetInt("rc");
		if (rc == 0){
			UserDefault::getInstance()->setBoolForKey("isReceivedFacebook", true);
			dataManager.getAppConfig().isreceivedRegisterFace = true;
			string bonus = *param->GetUtfString("coinBonus");
			auto lstbonus = mUtils::splitString(bonus, '|');
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->showAButton();
			layerPopup->setMessage("Chúc mừng");

			boost::shared_ptr<ISFSObject> params(new SFSObject());
			boost::shared_ptr<IRequest> request(new ExtensionRequest("rldot", params));
			GameServer::getSingleton().Send(request);
			SceneManager::getSingleton().showLoading();
			if (atoi(lstbonus[0].c_str()) > 0){
				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s %s quà liên kết facebook!", lstbonus[0].c_str(), SceneManager::getSingleton().getCoinUnit().c_str()));
				this->addChild(layerPopup);
			}
			else if (atoi(lstbonus[1].c_str()) > 0){
				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s Exp quà liên kết facebook!", lstbonus[1].c_str()));
				this->addChild(layerPopup);
			}
			else if (atoi(lstbonus[2].c_str()) > 0){
				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s lần mở rương vip quà liên kết facebook!", lstbonus[2].c_str()));
				this->addChild(layerPopup);
			}
			else if (atoi(lstbonus[3].c_str()) > 0){
				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s lượt quay vòng quay may mắn quà liên kết facebook!", lstbonus[3].c_str()));
				this->addChild(layerPopup);
			}
		}
		else{
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->showAButton();
			layerPopup->setMessage(dataManager.GetSysString(141));
			layerPopup->setContentMess("Nhận thưởng thất bại! Vui lòng thử lại!");
			this->addChild(layerPopup);

		}
	}

	else if (strcmp("rsla", cmd->c_str()) == 0){
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
		{
			if (param->GetUtfString("listattend") == NULL || param->GetInt("numattend") == NULL)
				return;
			SceneManager::getSingleton().hideLoading();
			string strlistattent = *param->GetUtfString("listattend");
			int numAttend = *param->GetInt("numattend");
			log("List Diem danh %s", strlistattent.c_str());
			lstDiemDanh.clear();
			auto listDay = mUtils::splitString(strlistattent, ';');
			for (int i = 0; i < listDay.size(); i++){
				if (i >= 7)
					return;
				string lstBonus = listDay[i];
				auto listBonus = mUtils::splitString(lstBonus, '|');
				if (listBonus.size() < 4)
					continue;
                this->lstTxtDiemDiemDanh[i]->setString(mUtils::convertMoneyEx(atoi(listBonus[3].c_str())));
				this->lstTxtExpDiemDanh[i]->setString(listBonus[2] + " EXP");

			}
			for (int i = 0; i < numAttend; i++){
				this->lstBgDaDiemDanh[i]->setVisible(true);
				this->lstTxtDiemDiemDanh[i]->setColor(Color3B(255,255,255));
				this->lstTxtExpDiemDanh[i]->setColor(Color3B(4, 0, 1));
			}

			//tblListDiemDanh->reloadData();
		}
		else{
			SceneManager::getSingleton().hideLoading();
			if (param->GetUtfString("listattend") == NULL || param->GetInt("numattend") == NULL)
				return;
			SceneManager::getSingleton().hideLoading();
			string strlistattent = *param->GetUtfString("listattend");
			int numAttend = *param->GetInt("numattend");
			log("List Diem danh %s", strlistattent.c_str());
			this->lstDiemDanhP2.clear();

			auto lstDiemDanh = mUtils::splitString(strlistattent, ';');
			if (lstDiemDanh.size() < 7) return;
			for (int i = 0; i < lstDiemDanh.size() - 1; i++){
				DiemDanhP2 it;
				it.stt = StringUtils::format("%d", i + 1);
				it.phanthuong = lstDiemDanh[i];
				it.isCheck = false;
				this->lstDiemDanhP2.push_back(it);
			}
			auto lstItem7 = mUtils::splitString(lstDiemDanh[6], '|');
			if (lstItem7.size() < 3) return;
			this->txt1Ngay7->setString(mUtils::convertMoneyEx(atol(lstItem7[0].c_str())));
			this->txt2Ngay7->setString(StringUtils::format("+ %s", lstItem7[2].c_str()) + "%");
			for (int j = 0; j < numAttend; j++){
				if (j != 6){
					this->lstDiemDanhP2[j].isCheck = true;
				}
				else {
					this->imgCheckNgay7->setVisible(true);
				}

			}
			this->tblListDiemDanh->reloadData();
		}
	}
	else if (strcmp("rsldt", cmd->c_str()) == 0){
		if (param->GetUtfString("listtask") == NULL) return;
		if (curr_tab != 3)
			SceneManager::getSingleton().hideLoading();
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
			string strlistTask = *param->GetUtfString("listtask");
			auto lst = mUtils::splitString(strlistTask, '-');
			//1|Hoàn thành .... ván game bất kỳ|4|8|12|16|0|4|0|16|0-0-0
			//id|name|socap|khoinghiep|trieuphu|daigia|capdohientai|tongcapdo|tiendo|tongtiendo|diem
			if (lst.size() < 4)
				return;
			string strTask = lst[0];
			this->lstNhiemvuNgayNew.clear();
			auto lstTask = mUtils::splitString(strTask, ';');
			int currDone = 0;
			for (int i = 0; i < lstTask.size(); i++){
				auto itemTask = mUtils::splitString(lstTask[i], '|');
				if (itemTask.size() < 11)
					return;
				MissionNew it;
				it.id = itemTask[0];
				it.name = itemTask[1];
				it.socap = itemTask[2];
				it.khoinghiep = itemTask[3];
				it.trieuphu = itemTask[4];
				it.daigia = itemTask[5];
				it.capdohientai = itemTask[6];
				it.tongcapdo = itemTask[7];
				it.tiendohientai = itemTask[8];
				it.tongtiendo = itemTask[9];
				it.diem = itemTask[10];

				if (atoi(itemTask[6].c_str()) >= 1)
					currDone++;

				this->lstNhiemvuNgayNew.push_back(it);

			}
			if (this->txtTienDoNhiemVuCoDinh)
				this->txtTienDoNhiemVuCoDinh->setString(StringUtils::format("%d/%lu", currDone, lstTask.size()));

			this->tblListNhiemVuNgay->reloadData();
			bool isDone = (atoi(lst[1].c_str()) == 1) ? true : false;
			bool isGet = (atoi(lst[2].c_str()) == 1) ? true : false;
			if (isDone){
				if (isGet){
					this->txtStatusNhiemVuCoDinh->setString("Đã nhận");
					this->btnNhanKeyNhiemVuNgay->setVisible(false);
				}
				else{
					this->txtStatusNhiemVuCoDinh->setString("Chưa nhận");
					this->btnNhanKeyNhiemVuNgay->setVisible(true);
				}
			}
			else{
				this->btnNhanKeyNhiemVuNgay->setVisible(false);
                this->txtStatusNhiemVuCoDinh->setString(mUtils::convertMoneyEx(atof(lst[3].c_str())));
			}

		}
		else{
			string strlistTask = *param->GetUtfString("listtask");
			auto listTask = mUtils::splitString(strlistTask, ';');
			lstNhiemvuNgay.clear();
			for (int i = 0; i < listTask.size(); i++){
				string lstItemTask = listTask[i];
				auto listItemTask = mUtils::splitString(lstItemTask, '|');
				Mission mission;
				if (listItemTask.size() < 6)
					return;
				//  for (int i = 0; i < listItemTask.size(); i++){
				mission.id = listItemTask[0];
				mission.type = listItemTask[1];
				mission.name = listItemTask[2];
				mission.status = listItemTask[4];
				mission.bonus = listItemTask[5];
				// }
				lstNhiemvuNgay.push_back(mission);

				//            if (28<= atoi(listItemTask[0].c_str())&& atoi(listItemTask[0].c_str())<=29){
				//                auto lstStatus = mUtils::splitString(listItemTask[4], '/');
				//                if (lstStatus.size()>1){
				//                    float done = atof(lstStatus[0].c_str());
				//                    float to = atof(lstStatus[1].c_str());
				//                    if (done < to)
				//                    {
				//                        tblListNhiemVuNgay->reloadData();
				//                        return;
				//                    }
				//                }
				//            }

			}
			tblListNhiemVuNgay->reloadData();

			log("%s", strlistTask.c_str());
		}
	}

	else if (strcmp("rsldot", cmd->c_str()) == 0){

		if (param->GetUtfString("listtask") == NULL) return;
		if (curr_tab != 3)
			SceneManager::getSingleton().hideLoading();
		string strlistTask = *param->GetUtfString("listtask");
		log("%s", strlistTask.c_str());
		auto listTask = mUtils::splitString(strlistTask, ';');

		lstNhiemvu1Lan.clear();
		lstNotShow.clear();
		for (int i = 0; i < listTask.size(); i++){
			bool isNotShow = false;
			string lstItemTask = listTask[i];
			auto listItemTask = mUtils::splitString(lstItemTask, '|');
			Mission mission;
			if (std::strcmp(listItemTask[0].c_str(), "5") == 0)
				continue;
			mission.id = listItemTask[0];
			mission.type = listItemTask[1];
			mission.name = listItemTask[2];
			mission.status = listItemTask[4];
			mission.bonus = listItemTask[5];
			if (std::strcmp(listItemTask[0].c_str(), "7") == 0 && std::strcmp(mission.status.c_str(), "0") == 0)
			{
				if (std::strcmp(dataManager.getAppConfig().loginType.c_str(), "facebook") == 0) {
					bool isLink = UserDefault::getInstance()->getBoolForKey("isReceivedFacebook");
					if (!isLink){//|| std::strcmp(status.c_str(),"0")==0) {
						mission.status = "0";
					}
					else{
						mission.status = "1";
					}
				}
				else{
					bool isLink = UserDefault::getInstance()->getBoolForKey("isLinkFacebook");
					if (!isLink){// || std::strcmp(status.c_str(),"0")==0) {
						mission.status = "0";
					}
					else{
						mission.status = "1";
					}
				}
			}
			if (30 <= atoi(listItemTask[0].c_str()) && atoi(listItemTask[0].c_str()) <= 39){

				auto lstStatus = mUtils::splitString(listItemTask[4], '/');
				if (lstStatus.size() > 1){
					float done = atof(lstStatus[0].c_str());
					float to = atof(lstStatus[1].c_str());
					if (done < to)
					{
						lstNotShow.push_back(atoi(listItemTask[0].c_str()) + 10);
					}
				}
			}
			if (atoi(listItemTask[0].c_str()) == 50){

				auto lstStatus = mUtils::splitString(listItemTask[4], '/');
				if (lstStatus.size()>1){
					float done = atof(lstStatus[0].c_str());
					float to = atof(lstStatus[1].c_str());
					if (done < to)
					{
						lstNotShow.push_back(atoi(listItemTask[0].c_str()) + 1);
					}
				}
			}

			for (int k = 0; k < lstNotShow.size(); k++){
				if (atoi(listItemTask[0].c_str()) == lstNotShow[k])
					isNotShow = true;
			}
			if (isNotShow)
				continue;
			lstNhiemvu1Lan.push_back(mission);
			//            if (17<= atoi(listItemTask[0].c_str())&& atoi(listItemTask[0].c_str())<=27){
			//                auto lstStatus = mUtils::splitString(listItemTask[4], '/');
			//                if (lstStatus.size()>1){
			//                    float done = atof(lstStatus[0].c_str());
			//                    float to = atof(lstStatus[1].c_str());
			//                    if (done < to)
			//                    {
			//                        tblListNhiemVu1Lan->reloadData();
			//                        return;
			//                    }
			//            }
			//           }
		}
		tblListNhiemVu1Lan->reloadData();
	}
	else if (strcmp("eegprsp", cmd->c_str()) == 0){

		if (param->GetInt("pbw") == NULL) return;
		int pbw = *param->GetInt("pbw");

		this->txtDiemTuanDiemDanhNew->setString(mUtils::convertMoneyEx(pbw));
		this->txtDiemTuanNhiemVuNgay->setString(mUtils::convertMoneyEx(pbw));

		if (param->GetInt("pbd") == NULL) return;
		int pbd = *param->GetInt("pbd");

		this->txtDiemNgayDiemDanhNew->setString(mUtils::convertMoneyEx(pbd));
		this->txtDiemNgayNhiemVuNgay->setString(mUtils::convertMoneyEx(pbd));
	}
	else if (strcmp("eegkbrsp", cmd->c_str()) == 0){

		SceneManager::getSingleton().hideLoading();

		if (param->GetInt("rc") == NULL) return;
		int rc = *param->GetInt("rc");
        
		if (rc == 0){

			boost::shared_ptr<ISFSObject> params(new SFSObject());
			boost::shared_ptr<IRequest> request(new ExtensionRequest("rldt", params));
			GameServer::getSingleton().Send(request);
			SceneManager::getSingleton().showLoading();

			auto _currScene = Director::getInstance()->getRunningScene();
			if (!_currScene) return;
			LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
			layerPopup->setMessage(dataManager.GetSysString(141));
			layerPopup->setContentMess(StringUtils::format("Nhận thưởng thành công!"));
			layerPopup->setTag(24);
			layerPopup->showAButton();
			_currScene->addChild(layerPopup, 1000000,9055);

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
			_currScene->addChild(layerPopup, 1000000,9056);
		}

		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rldt", params));
		GameServer::getSingleton().Send(request);

		SceneManager::getSingleton().showLoading();
	}
	else if (strcmp("eegfbrs", cmd->c_str()) == 0){

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
			layerPopup->setContentMess(StringUtils::format("Bạn đã nhận được %d Vàng!", rv));
			layerPopup->setTag(24);
			layerPopup->showAButton();
			_currScene->addChild(layerPopup, 1000000,9057);

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
			_currScene->addChild(layerPopup, 1000000,9058);
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
				this->btnNhanMoi->setTitleColor(Color3B::BLACK);
				Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupMission::CountDownTimerInvite),this);

			}
			else{
				this->btnNhanMoi->setEnabled(false);
				this->showTimerInvite();
				this->btnNhanMoi->setTitleColor(Color3B(144, 238, 144));
				Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupMission::CountDownTimerInvite),this,1, false);
			}
			auto item2 = mUtils::splitString(lstinfo[1], '|');
			if (item2.size() < 2) return;
			this->timerShare = atoi(item2[1].c_str());
			if (this->timerShare <= 0){
				this->btnNhanShare->setEnabled(true);
				this->btnNhanShare->setTitleText("Chia sẻ");
				this->btnNhanShare->setTitleColor(Color3B::BLACK);
				Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupMission::CountDownTimerShare),this);

			}
			else{
				this->btnNhanShare->setEnabled(false);
				this->btnNhanShare->setTitleColor(Color3B(144, 238, 144));
				this->showTimerShare();
				Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupMission::CountDownTimerShare),this,1, false);
			}

		}
	}
}
void LayerPopupMission::CountDownTimerShare(float dt){

	if (this->timerShare == 0){
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupMission::CountDownTimerShare),this);
		this->btnNhanShare->setTitleText("Chia sẻ");
		this->btnNhanShare->setEnabled(true);
	}
	this->timerShare -= 1;
	this->showTimerShare();
}
void LayerPopupMission::showTimerShare(){
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
void LayerPopupMission::CountDownTimerInvite(float dt){
	if (this->timerInvite == 0){
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupMission::CountDownTimerInvite),this);
		this->btnNhanMoi->setEnabled(true);
		this->btnNhanMoi->setTitleText("Mời");
	}
	this->timerInvite -= 1;
	this->showTimerInvite();
}
void LayerPopupMission::showTimerInvite(){
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
void LayerPopupMission::getPartnerInfo(){

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

bool LayerPopupMission::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPopupMission::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupMission::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
	//
	//    GameServer::getSingleton().addListeners(this);
}

void LayerPopupMission::onExit()
{
	Layer::onExit();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	//    GameServer::getSingleton().removeListeners(this);
	//    Layer::onExit();
}

void LayerPopupMission::fakeList()
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
bool LayerItemDiemDanh::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto root = CSLoader::getInstance()->createNode("ItemDiemDanh.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);

	this->setContentSize(root->getContentSize());

	layoutBonus = static_cast<Layout*>(root->getChildByName("pnlContent"));
	layoutBonus->setSwallowTouches(false);
	this->sprTypeBonus = static_cast<Sprite*>(layoutBonus->getChildByName("imgTypeBonus"));
	this->isCom = static_cast<Sprite*>(layoutBonus->getChildByName("Sprite_2"));

	this->lblDay = static_cast<Text*>(layoutBonus->getChildByName("lblDay"));
	this->lblBonus = static_cast<Text*>(layoutBonus->getChildByName("labelBonus"));
	return true;
}
void LayerItemDiemDanh::setDatas(int day, int status, string tien, string exp, string moruong, string quay){

	if (status == 0)
		this->isCom->setVisible(false);
	else
		this->isCom->setVisible(true);


	lblDay->setString(StringUtils::format("NGÀY %d", day + 1));


	string srcImgType = "";
	if (atof(tien.c_str()) > 0) {
		srcImgType = "coin-bonus.png";
		lblBonus->setString(mUtils::convertMoneyEx(atoll(tien.c_str())));
		sprTypeBonus->setTexture(srcImgType);
		return;
	}
	else if (atof(exp.c_str()) > 0) {
		srcImgType = "exp-bonus.png";
		lblBonus->setString(exp);
		sprTypeBonus->setTexture(srcImgType);
		return;

	}
	if (atof(moruong.c_str()) > 0) {
		srcImgType = "chest-key-bonus.png";
		lblBonus->setString(moruong);
		sprTypeBonus->setTexture(srcImgType);
		return;
	}
	if (atof(quay.c_str()) > 0) {
		srcImgType = "roulette-bonus.png";
		lblBonus->setString(quay);
		sprTypeBonus->setTexture(srcImgType);
		sprTypeBonus->setPositionY(sprTypeBonus->getPositionY() + 10);
		return;

	}
}
void LayerItemDiemDanh::resetItem(){
	layoutBonus->setVisible(false);
}
bool LayerItemDiemDanhP2::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto root = CSLoader::getInstance()->createNode("ItemDiemDanhP2.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);

	this->setContentSize(root->getContentSize());

	this->pnlType1 = static_cast<Layout*>(root->getChildByName("pnlType1"));
	this->txt1Type1 = static_cast<Text*>(pnlType1->getChildByName("txt1"));
	this->pnlType2 = static_cast<Layout*>(root->getChildByName("pnlType2"));
	this->txt1Type2 = static_cast<Text*>(pnlType2->getChildByName("txt1"));
	this->pnlType3 = static_cast<Layout*>(root->getChildByName("pnlType3"));
	this->txt1Type3 = static_cast<Text*>(pnlType3->getChildByName("txt1"));
	this->txt2Type3 = static_cast<Text*>(pnlType3->getChildByName("txt2"));
	this->pnlType4 = static_cast<Layout*>(root->getChildByName("pnlType4"));
	this->txt1Type4 = static_cast<Text*>(pnlType4->getChildByName("txt1"));
	this->txt2Type4 = static_cast<Text*>(pnlType4->getChildByName("txt2"));
	this->txtSTT = static_cast<Text*>(root->getChildByName("txtSTT"));
	this->imgcheck = static_cast<ImageView*>(root->getChildByName("imgcheck"));

	return true;
}
void LayerItemDiemDanhP2::setDatas(string stt, string phanthuong, bool isCheck){
	this->txtSTT->setString(StringUtils::format("Ngày %s", stt.c_str()));
	if (isCheck){
		this->imgcheck->setVisible(true);
	}
	else{
		this->imgcheck->setVisible(false);
	}
	this->pnlType1->setVisible(false);
	this->pnlType2->setVisible(false);
	this->pnlType3->setVisible(false);
	this->pnlType4->setVisible(false);

	auto lstBonus = mUtils::splitString(phanthuong, '|');
	if (lstBonus.size()<3)
		return;
	if (atoi(lstBonus[0].c_str())>0){
		if (atoi(lstBonus[1].c_str()) == 0 && atoi(lstBonus[2].c_str()) == 0){
			this->pnlType1->setVisible(true);
			this->txt1Type1->setString(mUtils::convertMoneyEx(atol(lstBonus[0].c_str())));
		}
		if (atoi(lstBonus[1].c_str()) > 0 && atoi(lstBonus[2].c_str()) == 0){
			this->pnlType4->setVisible(true);
			this->txt1Type4->setString(mUtils::convertMoneyEx(atol(lstBonus[0].c_str())));
			this->txt2Type4->setString(mUtils::convertMoneyEx(atol(lstBonus[1].c_str())));
		}
		if (atoi(lstBonus[1].c_str()) == 0 && atoi(lstBonus[2].c_str()) > 0){
			this->pnlType3->setVisible(true);
			this->txt1Type3->setString(mUtils::convertMoneyEx(atol(lstBonus[0].c_str())));
			this->txt2Type3->setString(StringUtils::format("+ %s", lstBonus[2].c_str()) + "% Thẻ nạp cuối");
		}
	}
	else{
		if (atoi(lstBonus[1].c_str()) > 0 && atoi(lstBonus[2].c_str()) == 0){
			this->pnlType2->setVisible(true);
			this->txt1Type4->setString(mUtils::convertMoneyEx(atol(lstBonus[1].c_str())));
		}
	}
}
void LayerItemDiemDanhP2::resetItem(){

}
bool LayerItemNhiemVuMotLan::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto root = CSLoader::getInstance()->createNode("ItemMission.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);


	this->imgStatus1Bonus = static_cast<ImageView*>(root->getChildByName("Image_2"));
	this->txtName = static_cast<Text*>(root->getChildByName("lblName"));
	this->txtStatus = static_cast<Text*>(root->getChildByName("lblStatus"));
	this->txtBonus = static_cast<Text*>(imgStatus1Bonus->getChildByName("lblBonus"));
	this->imgCheck = static_cast<ImageView*>(imgStatus1Bonus->getChildByName("imgCheck"));
	this->bgStatus = static_cast<Sprite*>(root->getChildByName("bgStatus"));
	this->ldStatus = static_cast<LoadingBar*>(bgStatus->getChildByName("lbStatus"));
	this->coin = static_cast<ImageView*>(imgStatus1Bonus->getChildByName("Image_4"));
	this->btnRegard = dynamic_cast<Button*>(root->getChildByName("btnRegard"));

	this->imgStatus2Bonus = static_cast<ImageView*>(root->getChildByName("img2Bonus"));
	this->txt2BonusCoin = static_cast<Text*>(imgStatus2Bonus->getChildByName("lblBonus"));
	this->txt2BonusKey = static_cast<Text*>(imgStatus2Bonus->getChildByName("lblKey"));
	this->imgCheck2Bonus = static_cast<ImageView*>(imgStatus2Bonus->getChildByName("imgCheck"));


	this->imgStatusBonusKey = static_cast<ImageView*>(root->getChildByName("Image_3"));;
	this->txt1BonusKey = static_cast<Text*>(imgStatusBonusKey->getChildByName("lblBonus"));
	this->imgCheck1BonusKey = static_cast<ImageView*>(imgStatusBonusKey->getChildByName("imgCheck"));

	if (btnRegard){
		btnRegard->addClickEventListener(CC_CALLBACK_1(LayerItemNhiemVuMotLan::onBtnGetFBBonus, this));
		auto txtBonus = dynamic_cast<Text*>(btnRegard->getChildByName("lblMoney"));
		if (txtBonus) {
			txtBonus->setString(StringUtils::format("+%d", SceneManager::getSingleton().getFacebookBonus()));
		}
		//        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		//            //btnRegard->setContentSize()
		//            auto pos = txtBonus->getPosition();
		//            txtBonus->setVisible(Vec2(pos.x-10,pos.y-15));
		//        }
	}

	this->setContentSize(root->getContentSize());



	return true;
}
void LayerItemNhiemVuMotLan::setDatasTanThu(string id, string name, string status, string type, string bonus, bool isShowcoin){
    
    int intType = atoi(type.c_str());
    txtName->setString(name);
    btnRegard->setVisible(false);
    this->imgStatusBonusKey->setVisible(false);
    
    auto lstBonus = mUtils::splitString(bonus, '-');
    
    string bonusPri = bonus + " ĐIỂM";
    if (lstBonus.size() > 1 && atoi(lstBonus[0].c_str()) > 0 && atoi(lstBonus[2].c_str()) > 0){
        this->imgStatus1Bonus->setVisible(false);
        this->imgStatus2Bonus->setVisible(true);
        this->txt2BonusCoin->setString(formatMoneySymbol(atol(lstBonus[0].c_str())));
        this->txt2BonusKey->setString("+" + formatMoneySymbol(atol(lstBonus[2].c_str())));
        if (intType == 0){
            bgStatus->setVisible(true);
            txtStatus->setVisible(true);
            auto lstStatus = mUtils::splitString(status, '/');
            if (lstStatus.size() > 1){
                float done = atof(lstStatus[0].c_str());
                float to = atof(lstStatus[1].c_str());
                if (done > to){
                    done = to;
                    status = lstStatus[1] + "/" + lstStatus[1];
                }
                float percent = done / to;
                
                
                ldStatus->setPercent(percent * 100);
                txtStatus->setString(status);
                if (done == to)
                    this->imgCheck2Bonus->setVisible(true);
                else
                    this->imgCheck2Bonus->setVisible(false);
                
                return;
            }
        }
        
    }
    else{
        this->imgStatus1Bonus->setVisible(true);
        this->imgStatus2Bonus->setVisible(false);
    }
    if (lstBonus.size() > 1 && atoi(lstBonus[0].c_str()) > 0 && atoi(lstBonus[1].c_str()) > 0 && atoi(lstBonus[2].c_str()) == 0 && atoi(lstBonus[3].c_str()) == 0 && atoi(lstBonus[4].c_str()) == 0) {
        bonusPri = formatMoneySymbol(atol(lstBonus[0].c_str()));
    }
    
//    if (lstBonus.size() > 1 && atoi(lstBonus[0].c_str()) == 0 && atoi(lstBonus[1].c_str()) == 0 && atoi(lstBonus[2].c_str()) == 0 && atoi(lstBonus[3].c_str()) == 0 && atoi(lstBonus[4].c_str()) > 0) {
//        bonusPri = lstBonus[4] + " ĐIỂM";
//        
//    }
//    
//    if (lstBonus.size() > 1 && atoi(lstBonus[0].c_str()) == 0 && atoi(lstBonus[4].c_str()) == 0 && atoi(lstBonus[2].c_str()) == 0 && atoi(lstBonus[3].c_str()) == 0 && atoi(lstBonus[1].c_str()) > 0) {
//        bonusPri = lstBonus[1] + " EXP";
//        
//    }
    
    if (lstBonus.size() > 1 && atoi(lstBonus[0].c_str()) > 0){
        coin->setVisible(true);
        txtBonus->setString(bonusPri);
    }
    else if (lstBonus.size() > 1 && atoi(lstBonus[2].c_str()) > 0){
        this->imgStatusBonusKey->setVisible(true);
        this->imgStatus1Bonus->setVisible(false);
        this->imgStatus2Bonus->setVisible(false);
        
        txt1BonusKey->setString(lstBonus[2]);
        if (intType == 0){
            //bgStatus->setVisible(true);
            //txtStatus->setVisible(true);
            auto lstStatus = mUtils::splitString(status, '/');
            if (lstStatus.size() > 1){
                float done = atof(lstStatus[0].c_str());
                float to = atof(lstStatus[1].c_str());
                if (done > to){
                    done = to;
                    status = lstStatus[1] + "/" + lstStatus[1];
                }
                float percent = done / to;
                
                
                ldStatus->setPercent(percent * 100);
                txtStatus->setString(status);
                if (done == to)
                    this->imgCheck1BonusKey->setVisible(true);
                else
                    this->imgCheck1BonusKey->setVisible(false);
                
                return;
            }
        }
        
    }
    else{
        coin->setVisible(false);
        txtBonus->setString(bonusPri);
        //txtBonus->setPositionX(txtBonus->getPositionX()+10);
    }
    
    
    if (intType == 0){
        bgStatus->setVisible(true);
        txtStatus->setVisible(true);
        auto lstStatus = mUtils::splitString(status, '/');
        if (lstStatus.size() > 1){
            float done = atof(lstStatus[0].c_str());
            float to = atof(lstStatus[1].c_str());
            if (done > to){
                done = to;
                status = lstStatus[1] + "/" + lstStatus[1];
            }
            
            float percent = done / to;
            
            ldStatus->setPercent(percent * 100);
            txtStatus->setString(status);
            if (done == to)
                imgCheck->setVisible(true);
            else
                imgCheck->setVisible(false);
            
            return;
        }
    }
    else if (intType == 1){
        this->btnRegard->setVisible(false);
        bgStatus->setVisible(false);
        txtStatus->setVisible(false);
        int isDone = atoi(status.c_str());
        
        if (std::strcmp(id.c_str(), "7") == 0)
        {
            if (std::strcmp(dataManager.getAppConfig().loginType.c_str(), "facebook") == 0) {
                if (std::strcmp(status.c_str(), "0") == 0){//|| std::strcmp(status.c_str(),"0")==0) {
                    this->imgStatus1Bonus->setVisible(false);
                    btnRegard->setVisible(true);
                }
                else{
                    imgCheck->setVisible(true);
                    this->imgStatus1Bonus->setVisible(true);
                    btnRegard->setVisible(false);
                }
            }
            else{
                if (std::strcmp(status.c_str(), "0") == 0){// || std::strcmp(status.c_str(),"0")==0) {
                    this->imgStatus1Bonus->setVisible(true);
                    imgCheck->setVisible(false);
                    btnRegard->setVisible(false);
                }
                else{
                    this->imgStatus1Bonus->setVisible(true);
                    imgCheck->setVisible(true);
                    btnRegard->setVisible(false);
                }
            }
            return;
        }
        else{
            this->btnRegard->setVisible(false);
            if (isDone == 1)
                imgCheck->setVisible(true);
            else
                imgCheck->setVisible(false);
            return;
        }
        
        
    }
    else if (intType == 15){
        
        bgStatus->setVisible(false);
        txtStatus->setVisible(false);
        return;
        
    }

}
void LayerItemNhiemVuMotLan::setDatas(string id, string name, string status, string type, string bonus, bool isShowcoin){

	int intType = atoi(type.c_str());
	txtName->setString(name);
	btnRegard->setVisible(false);
	this->imgStatusBonusKey->setVisible(false);

	auto lstBonus = mUtils::splitString(bonus, '-');

	string bonusPri = bonus + " ĐIỂM";
	if (lstBonus.size() > 1 && atoi(lstBonus[0].c_str()) > 0 && atoi(lstBonus[2].c_str()) > 0){
		this->imgStatus1Bonus->setVisible(false);
		this->imgStatus2Bonus->setVisible(true);
		this->txt2BonusCoin->setString(formatMoneySymbol(atol(lstBonus[0].c_str())));
		this->txt2BonusKey->setString("+" + formatMoneySymbol(atol(lstBonus[2].c_str())));
		if (intType == 0){
			bgStatus->setVisible(true);
			txtStatus->setVisible(true);
			auto lstStatus = mUtils::splitString(status, '/');
			if (lstStatus.size() > 1){
				float done = atof(lstStatus[0].c_str());
				float to = atof(lstStatus[1].c_str());
				if (done > to){
					done = to;
					status = lstStatus[1] + "/" + lstStatus[1];
				}
				float percent = done / to;


				ldStatus->setPercent(percent * 100);
				txtStatus->setString(status);
				if (done == to)
					this->imgCheck2Bonus->setVisible(true);
				else
					this->imgCheck2Bonus->setVisible(false);

				return;
			}
		}

	}
	else{
		this->imgStatus1Bonus->setVisible(true);
		this->imgStatus2Bonus->setVisible(false);
	}
	if (lstBonus.size() > 1 && atoi(lstBonus[0].c_str()) > 0 && atoi(lstBonus[1].c_str()) == 0 && atoi(lstBonus[2].c_str()) == 0 && atoi(lstBonus[3].c_str()) == 0 && atoi(lstBonus[4].c_str()) == 0) {
		bonusPri = formatMoneySymbol(atol(lstBonus[0].c_str()));
	}

	if (lstBonus.size() > 1 && atoi(lstBonus[0].c_str()) == 0 && atoi(lstBonus[1].c_str()) == 0 && atoi(lstBonus[2].c_str()) == 0 && atoi(lstBonus[3].c_str()) == 0 && atoi(lstBonus[4].c_str()) > 0) {
		bonusPri = lstBonus[4] + " ĐIỂM";

	}

	if (lstBonus.size() > 1 && atoi(lstBonus[0].c_str()) == 0 && atoi(lstBonus[4].c_str()) == 0 && atoi(lstBonus[2].c_str()) == 0 && atoi(lstBonus[3].c_str()) == 0 && atoi(lstBonus[1].c_str()) > 0) {
		bonusPri = lstBonus[1] + " EXP";

	}

	if (lstBonus.size() > 1 && atoi(lstBonus[0].c_str()) > 0){
		coin->setVisible(true);
		txtBonus->setString(bonusPri);
	}
	else if (lstBonus.size() > 1 && atoi(lstBonus[2].c_str()) > 0){
		this->imgStatusBonusKey->setVisible(true);
		this->imgStatus1Bonus->setVisible(false);
		this->imgStatus2Bonus->setVisible(false);

		txt1BonusKey->setString(lstBonus[2]);
		if (intType == 0){
			//bgStatus->setVisible(true);
			//txtStatus->setVisible(true);
			auto lstStatus = mUtils::splitString(status, '/');
			if (lstStatus.size() > 1){
				float done = atof(lstStatus[0].c_str());
				float to = atof(lstStatus[1].c_str());
				if (done > to){
					done = to;
					status = lstStatus[1] + "/" + lstStatus[1];
				}
				float percent = done / to;


				ldStatus->setPercent(percent * 100);
				txtStatus->setString(status);
				if (done == to)
					this->imgCheck1BonusKey->setVisible(true);
				else
					this->imgCheck1BonusKey->setVisible(false);

				return;
			}
		}

	}
	else{
		coin->setVisible(false);
		txtBonus->setString(bonusPri);
		//txtBonus->setPositionX(txtBonus->getPositionX()+10);
	}


	if (intType == 0){
		bgStatus->setVisible(true);
		txtStatus->setVisible(true);
		auto lstStatus = mUtils::splitString(status, '/');
		if (lstStatus.size() > 1){
			float done = atof(lstStatus[0].c_str());
			float to = atof(lstStatus[1].c_str());
			if (done > to){
				done = to;
				status = lstStatus[1] + "/" + lstStatus[1];
			}

			float percent = done / to;

			ldStatus->setPercent(percent * 100);
			txtStatus->setString(status);
			if (done == to)
				imgCheck->setVisible(true);
			else
				imgCheck->setVisible(false);

			return;
		}
	}
	else if (intType == 1){
		this->btnRegard->setVisible(false);
		bgStatus->setVisible(false);
		txtStatus->setVisible(false);
		int isDone = atoi(status.c_str());

		if (std::strcmp(id.c_str(), "7") == 0)
		{
			if (std::strcmp(dataManager.getAppConfig().loginType.c_str(), "facebook") == 0) {
				if (std::strcmp(status.c_str(), "0") == 0){//|| std::strcmp(status.c_str(),"0")==0) {
					this->imgStatus1Bonus->setVisible(false);
					btnRegard->setVisible(true);
				}
				else{
					imgCheck->setVisible(true);
					this->imgStatus1Bonus->setVisible(true);
					btnRegard->setVisible(false);
				}
			}
			else{
				if (std::strcmp(status.c_str(), "0") == 0){// || std::strcmp(status.c_str(),"0")==0) {
					this->imgStatus1Bonus->setVisible(true);
					imgCheck->setVisible(false);
					btnRegard->setVisible(false);
				}
				else{
					this->imgStatus1Bonus->setVisible(true);
					imgCheck->setVisible(true);
					btnRegard->setVisible(false);
				}
			}
			return;
		}
		else{
			this->btnRegard->setVisible(false);
			if (isDone == 1)
				imgCheck->setVisible(true);
			else
				imgCheck->setVisible(false);
			return;
		}


	}
	else if (intType == 15){

		bgStatus->setVisible(false);
		txtStatus->setVisible(false);
		return;

	}

}
void LayerItemNhiemVuMotLan::resetItem(){

}
void LayerItemNhiemVuMotLan::onBtnGetFBBonus(Ref* pSender){
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest("rfb", params));
	GameServer::getSingleton().Send(request);
	SceneManager::getSingleton().showLoading();
	dataManager.getAppConfig().isMissionFacebookClick = true;
}

bool LayerItemNhiemVuNgayNew::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto root = CSLoader::getInstance()->createNode("LayerItemNhiemVuNgayNew.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);

	this->setContentSize(root->getContentSize());

	this->pnlType1 = static_cast<Layout*>(root->getChildByName("pnlType1"));
	this->pnlType1->setSwallowTouches(false);

	this->pnlType2 = static_cast<Layout*>(root->getChildByName("pnlType2"));
	this->pnlType2->setSwallowTouches(false);

	this->txtMissionNameType1 = static_cast<Text*>(this->pnlType1->getChildByName("txtMissionName"));
	this->txtDanhHieu1Type1 = static_cast<Text*>(this->pnlType1->getChildByName("txtDanhHieu1"));
	this->txtDanhHieu2Type1 = static_cast<Text*>(this->pnlType1->getChildByName("txtDanhHieu2"));
	this->txtDanhHieu3Type1 = static_cast<Text*>(this->pnlType1->getChildByName("txtDanhHieu3"));
	this->txtDanhHieu4Type1 = static_cast<Text*>(this->pnlType1->getChildByName("txtDanhHieu4"));
	this->lstTxtDanhHieu.push_back(this->txtDanhHieu1Type1);
	this->lstTxtDanhHieu.push_back(this->txtDanhHieu2Type1);
	this->lstTxtDanhHieu.push_back(this->txtDanhHieu3Type1);
	this->lstTxtDanhHieu.push_back(this->txtDanhHieu4Type1);


	this->danhnhieu1Type1 = static_cast<ImageView*>(this->pnlType1->getChildByName("danhnhieu1"));
	this->danhnhieu2Type1 = static_cast<ImageView*>(this->pnlType1->getChildByName("danhnhieu2"));
	this->danhnhieu3Type1 = static_cast<ImageView*>(this->pnlType1->getChildByName("danhnhieu3"));
	this->danhnhieu4Type1 = static_cast<ImageView*>(this->pnlType1->getChildByName("danhnhieu4"));
	this->lstImgDanhHieu.push_back(this->danhnhieu1Type1);
	this->lstImgDanhHieu.push_back(this->danhnhieu2Type1);
	this->lstImgDanhHieu.push_back(this->danhnhieu3Type1);
	this->lstImgDanhHieu.push_back(this->danhnhieu4Type1);

	this->txtTienDoType1 = static_cast<Text*>(this->pnlType1->getChildByName("txtTienDo"));
	this->txtDiemType1 = static_cast<Text*>(this->pnlType1->getChildByName("txtDiem"));

	this->txtMissionNameType2 = static_cast<Text*>(this->pnlType2->getChildByName("txtMissionName"));
	this->txtDanhHieu1Type2 = static_cast<Text*>(this->pnlType2->getChildByName("txtDanhHieu1"));
	this->danhnhieu1Type2 = static_cast<ImageView*>(this->pnlType2->getChildByName("danhnhieu1"));

	this->txtTienDoType2 = static_cast<Text*>(this->pnlType2->getChildByName("txtTienDo"));
	this->txtDiemType2 = static_cast<Text*>(this->pnlType2->getChildByName("txtDiem"));
	return true;
}
void LayerItemNhiemVuNgayNew::setDatas(string id, string name, string socap, string khoinghiep, string trieuphu, string daigia, string levelhientai, string tonglevel, string tiendo, string tongtiendo, string currpoint){

	if (atoi(tonglevel.c_str()) == 1){

		this->pnlType1->setVisible(false);
		this->pnlType2->setVisible(true);

		this->txtMissionNameType2->setString(name);
		this->txtDanhHieu1Type2->setString(tonglevel);
		this->txtDanhHieu1Type2->setColor(Color3B(250, 222, 157));

		if (atoi(levelhientai.c_str()) == atoi(tonglevel.c_str())){
			this->danhnhieu1Type2->setVisible(true);
			this->txtDanhHieu1Type2->setColor(Color3B(105, 46, 1));
		}

		this->txtTienDoType2->setString(StringUtils::format("%s/%s", tiendo.c_str(), tongtiendo.c_str()));
		this->txtDiemType2->setString(currpoint);

	}
	else{
		this->pnlType1->setVisible(true);
		this->pnlType2->setVisible(false);

		this->txtMissionNameType1->setString(name);
		this->txtDanhHieu1Type1->setString(formatMoneySymbolChest(atol(socap.c_str())));
		this->txtDanhHieu2Type1->setString(formatMoneySymbolChest(atol(khoinghiep.c_str())));
		this->txtDanhHieu3Type1->setString(formatMoneySymbolChest(atol(trieuphu.c_str())));
		this->txtDanhHieu4Type1->setString(formatMoneySymbolChest(atol(daigia.c_str())));

		for (int i = 0; i < 4; i++){
			this->lstImgDanhHieu[i]->setVisible(false);
			this->lstTxtDanhHieu[i]->setColor(Color3B(250, 222, 157));
		}

		for (int i = 0; i < atoi(levelhientai.c_str()); i++){
			this->lstImgDanhHieu[i]->setVisible(true);
			this->lstTxtDanhHieu[i]->setColor(Color3B(105, 46, 1));
		}

		//        this->danhnhieu1Type1 = static_cast<ImageView*>(this->pnlType1->getChildByName("danhnhieu1"));
		//        this->danhnhieu2Type1 = static_cast<ImageView*>(this->pnlType1->getChildByName("danhnhieu2"));
		//        this->danhnhieu3Type1 = static_cast<ImageView*>(this->pnlType1->getChildByName("danhnhieu3"));
		//        this->danhnhieu4Type1 = static_cast<ImageView*>(this->pnlType1->getChildByName("danhnhieu4"));

		this->txtTienDoType1->setString(StringUtils::format("%s/%s", tiendo.c_str(), tongtiendo.c_str()));
		this->txtDiemType1->setString(currpoint);
	}

}
void LayerItemNhiemVuNgayNew::resetItem(){

	this->lstTxtDanhHieu.clear();

	this->lstTxtDanhHieu.push_back(this->txtDanhHieu1Type1);
	this->lstTxtDanhHieu.push_back(this->txtDanhHieu2Type1);
	this->lstTxtDanhHieu.push_back(this->txtDanhHieu3Type1);
	this->lstTxtDanhHieu.push_back(this->txtDanhHieu4Type1);


	this->lstImgDanhHieu.clear();
	this->lstImgDanhHieu.push_back(this->danhnhieu1Type1);
	this->lstImgDanhHieu.push_back(this->danhnhieu2Type1);
	this->lstImgDanhHieu.push_back(this->danhnhieu3Type1);
	this->lstImgDanhHieu.push_back(this->danhnhieu4Type1);
	for (int i = 0; i < 4; i++){
		this->lstImgDanhHieu[i]->setVisible(false);
		this->lstTxtDanhHieu[i]->setColor(Color3B(250, 222, 157));
	}
}
void LayerItemNhiemVuNgayNew::onBtnGetFBBonus(Ref* pSender){
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest("rfb", params));
	GameServer::getSingleton().Send(request);
	SceneManager::getSingleton().showLoading();
	dataManager.getAppConfig().isMissionFacebookClick = true;
}

bool LayerItemFriendFacebook::init(){
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
		this->btnSelectedInvite->addClickEventListener(CC_CALLBACK_1(LayerItemFriendFacebook::onBtnInvitedFriend, this));
	}

	return true;
}

void LayerItemFriendFacebook::setDatas(string _uid, string urlAvatar, string name, bool isInvite){
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

void  LayerItemFriendFacebook::onInviteFriendCallBack(Ref* target, SEL_CallFuncO listener){
	mCallBack = target;
	mCallBackListener = listener;
}
void  LayerItemFriendFacebook::onBtnInvitedFriend(Ref* pSender){
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
bool LayerItemTanThu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("ItemTanThu.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->txtName = static_cast<Text*>(root->getChildByName("lblName"));

    this->btnRegard1 = static_cast<Button*>(root->getChildByName("btnreward1"));
    this->btnRegard2 = static_cast<Button*>(root->getChildByName("btnreward2"));
    
    if (this->btnRegard1){
        this->btnRegard1->addClickEventListener(CC_CALLBACK_1(LayerItemTanThu::onBtnReward, this));
    }
    if (this->btnRegard2){
        this->btnRegard2->addClickEventListener(CC_CALLBACK_1(LayerItemTanThu::onBtnReward, this));
    }
    this->imgCheck1 = static_cast<ImageView*>(this->btnRegard1->getChildByName("imgCheck"));
    this->lblBonus = static_cast<Text*>(this->btnRegard1->getChildByName("lblBonus"));
    this->imgCheck2 = static_cast<ImageView*>(this->btnRegard2->getChildByName("imgCheck"));
    this->silver2 = static_cast<Text*>(this->btnRegard2->getChildByName("silver1"));
    this->gold2 = static_cast<Text*>(this->btnRegard2->getChildByName("gold1"));
    
    this->bg1 = static_cast<ImageView*>(root->getChildByName("imgbg"));
    this->bg2 = static_cast<ImageView*>(root->getChildByName("imgbg2"));
    this->imgCoin = static_cast<ImageView*>(this->btnRegard1->getChildByName("imgCoin"));

    
    this->setContentSize(root->getContentSize());
    

    return true;
}
void LayerItemTanThu::setDatas(string id, string name, string status, string bonus,string receive,string type){
    this->txtName->setString(name);
    
    auto lstBonus = mUtils::splitString(bonus, '-');
    this->btnRegard1->setTag(atoi(id.c_str()));
    this->btnRegard2->setTag(atoi(id.c_str()));

    this->btnRegard1->setVisible(false);
    this->btnRegard2->setVisible(false);
    this->btnRegard1->setEnabled(false);
    this->btnRegard1->setEnabled(false);
    
    this->imgCheck1->setVisible(false);
    this->imgCheck2->setVisible(false);
    this->bg1->setVisible(false);
    this->bg2->setVisible(false);
    if (lstBonus.size() <3)
        return;
    
    if (atoi(lstBonus[0].c_str()) > 0 && atoi(lstBonus[1].c_str()) > 0){
        this->btnRegard2->setVisible(true);
        this->silver2->setString(mUtils::convertMoneyEx(atof(lstBonus[1].c_str())));
        this->gold2->setString(mUtils::convertMoneyEx(atof(lstBonus[0].c_str())));
        this->bg1->setVisible(true);

        if (atoi(type.c_str()) == 1){
            if (atoi(status.c_str()) == 1 && atoi(receive.c_str()) == 0 )
                this->btnRegard2->setEnabled(true);
            else if (atoi(status.c_str()) == 1 && atoi(receive.c_str()) == 1 ){
                this->btnRegard2->setEnabled(false);
                this->imgCheck2->setVisible(true);
            }
        }else{
            if (atoi(status.c_str()) == 1 && atoi(receive.c_str()) == 1 ){
                this->btnRegard2->setEnabled(false);
                this->imgCheck2->setVisible(true);
            }
        }
    }

    
    else if (atoi(lstBonus[0].c_str()) > 0 && atoi(lstBonus[1].c_str()) <= 0){
        this->lblBonus->setString(mUtils::convertMoneyEx(atof(lstBonus[0].c_str())));
        this->btnRegard1->setVisible(true);
        this->bg2->setVisible(true);
        this->imgCoin->loadTexture("profile-coin.png");


        if (atoi(type.c_str()) == 1){
            if (atoi(status.c_str()) == 1 && atoi(receive.c_str()) == 0 )
                this->btnRegard1->setEnabled(true);
            else if (atoi(status.c_str()) == 1 && atoi(receive.c_str()) == 1 ){
                this->btnRegard1->setEnabled(false);
                this->imgCheck1->setVisible(true);
            }
        }else{
            if (atoi(status.c_str()) == 1 && atoi(receive.c_str()) == 1 ){
                this->btnRegard1->setEnabled(false);
                this->imgCheck1->setVisible(true);
            }

        }

    }
    else if (atoi(lstBonus[0].c_str()) <= 0 && atoi(lstBonus[1].c_str()) > 0){
        this->bg2->setVisible(true);

        this->lblBonus->setString(mUtils::convertMoneyEx(atof(lstBonus[1].c_str())));
        this->btnRegard1->setVisible(true);
     
        this->imgCoin->loadTexture("button_xu.png");

        
        if (atoi(type.c_str()) == 1){
            if (atoi(status.c_str()) == 1 && atoi(receive.c_str()) == 0 )
                this->btnRegard1->setEnabled(true);
            else if (atoi(status.c_str()) == 1 && atoi(receive.c_str()) == 1 ){
                this->btnRegard1->setEnabled(false);
                this->imgCheck1->setVisible(true);
            }
        }else{
            if (atoi(status.c_str()) == 1 && atoi(receive.c_str()) == 1 ){
                this->btnRegard1->setEnabled(false);
                this->imgCheck1->setVisible(true);
            }
        }

    }
}
void LayerItemTanThu::resetItem(){
    
}
void LayerItemTanThu::onBtnReward(Ref* pSender){
    auto node = (Node*)pSender;
    
    int tag = node->getTag();
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_NEW_MISSION_REQ, params));
     params->PutInt(EVENT_MISSION_ID,tag);
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
    dataManager.getAppConfig().isMissionFacebookClick = true;
}
bool LayerItemThanhTuu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("ItemThanhTuu.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->lbStatus = static_cast<LoadingBar*>(root->getChildByName("lbtt"));
 
    this->txtStats = static_cast<Text*>(root->getChildByName("lblStatus"));
    
    this->txtName = static_cast<Text*>(root->getChildByName("lblName"));
    
    this->btnRegard1 = static_cast<Button*>(root->getChildByName("btnreward1"));
    if (this->btnRegard1){
        this->btnRegard1->addClickEventListener(CC_CALLBACK_1(LayerItemThanhTuu::onBtnReward, this));
    }
    this->imgCoin = static_cast<ImageView*>(this->btnRegard1->getChildByName("imgCoin"));
    this->imgCheck = static_cast<ImageView*>(this->btnRegard1->getChildByName("imgCheck"));
    this->txtBonus = static_cast<Text*>(this->btnRegard1->getChildByName("lblBonus"));
    for (int i = 1; i <= 5 ; i++){
        auto star = static_cast<ImageView*>(root->getChildByName(StringUtils::format("star%d",i)));
        this->lstStar.push_back(star);
    }
    
    this->setContentSize(root->getContentSize());
    
    
    return true;
}
void LayerItemThanhTuu::setDatas(string id, string name, string star1,string star2,string star3,string star4,string star5 ,string bonus,string currStar, string getStar){
    this->txtName->setString(name);
    this->btnRegard1->setEnabled(false);
    for (int i = 0; i < lstStar.size();i++){
        this->lstStar[i]->loadTexture("tt_star_dis.png");
    }
    this->currgetStar = atoi(getStar.c_str());
    for (int i = 0; i <= this->currgetStar;i++){
        if (i < this->lstStar.size())
            this->lstStar[i]->loadTexture("tt_star_act.png");
    }

    this->imgCoin->setVisible(true);
    this->imgCheck->setVisible(false);
    this->txtBonus->setVisible(true);
    
    auto lstBonus = mUtils::splitString(bonus, '-');
    
    switch (atoi(getStar.c_str())){
        case 0:
            this->loadStar(star1);
            break;
        case 1:
            this->loadStar(star2);
            break;
        case 2:
            this->loadStar(star3);
            break;
        case 3:
            this->loadStar(star4);
            break;
        case 4:
            this->loadStar(star5);
            break;
        default:
            this->loadStar(star5);
            break;
    }
    
    
    if (lstBonus.size() < 2)
        return;
    
    if (atoi(lstBonus[0].c_str()) > 0){
        this->txtBonus->setString(mUtils::convertMoneyEx(atof(lstBonus[0].c_str())));
        this->imgCoin->loadTexture("profile-coin.png");

    }
    else if (atoi(lstBonus[1].c_str()) > 0){
        this->txtBonus->setString(mUtils::convertMoneyEx(atof(lstBonus[1].c_str())));
        this->imgCoin->loadTexture("button_xu.png");
        
    }
    this->btnRegard1->setTag(atoi(id.c_str()));
    
}
void LayerItemThanhTuu::loadStar(string star){
    auto lststatus = mUtils::splitString(star, '-');
    if (lststatus.size()<3)
        return;
    
    auto lstprogress = mUtils::splitString(lststatus[1], ':');
    if (lstprogress.size()<2)
        return;
    if (atoi(lstprogress[0].c_str()) == atoi(lstprogress[1].c_str())){
        if (this->currgetStar< 5)
            this->btnRegard1->setEnabled(true);
        else{
            this->imgCoin->setVisible(false);
            this->imgCheck->setVisible(true);
            this->txtBonus->setVisible(false);
        }

    }
    this->txtStats->setString(lstprogress[0]+ "/"+lstprogress[1]);
    float percent = atof(lstprogress[0].c_str()) / atof(lstprogress[1].c_str());
    
    this->lbStatus->setPercent(percent * 100);
    
}
void LayerItemThanhTuu::resetItem(){
    
}
void LayerItemThanhTuu::onBtnReward(Ref* pSender){

    auto node = (Node*)pSender;
    
    int tag = node->getTag();
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt(ARCHIEMENT_ID,tag);
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_ARCHIEMENT_REQ, params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
}

