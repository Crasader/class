#include "LayerPopupProfile.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../scenes/SceneMain.h"
#include "Requests/ExtensionRequest.h"
#include "../data_manager.h"
#include "LayerPopupBaoMatCaNhan.h"
#include "LayerPopupCuaHang.h"
#include "../mUtils.h"
#include "../Common.h"
#include "../layergames/_Chat_.h"
#include "Util/PasswordUtil.h"
#include "../layergames/ChanUtils.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupOTP.h"
#include "LayerNapHao.h"
#include "LayerPopupHomThu.h"
#include "LayerPopupQuaTang.h"
#include <string>
#include "LayerPopupDoiMatKhau.h"
#include "LayerPopupDoiAvatar.h"
#include "../scenes/ScenePickRoom.h"
#include "LayerPopupCapNhatSTK.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "PluginFacebook/PluginFacebook.h"
#include "mFaceBook.h"
#endif
LayerPopupProfile* LayerPopupProfile::instance = NULL;
std::string g_avatarLink = "";

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif

using namespace std;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"

extern "C" {
    //org/cocos2dx/cpp/AppActivity
    void Java_org_cocos2dx_cpp_Upload_setLinkAvatar(JNIEnv*  env, jobject thiz, jstring linkAvatar) {
        g_avatarLink = JniHelper::jstring2string(linkAvatar);
       // log("nhan duoc link avatar: %s", g_avatarLink.c_str());
        if (LayerPopupProfile::instance != NULL)
        {
            LayerPopupProfile::instance->SetLinkAvatarToServer(g_avatarLink);
        }
    }
}

#endif
#include "layergames/ChanUtils.h"

LayerPopupProfile::LayerPopupProfile()
{
    this->arrAvatarChange.clear();
}

LayerPopupProfile::~LayerPopupProfile()
{
    GameServer::getSingleton().removeListeners(this);
    if (instance != NULL){
    }
    this->arrAvatarChange.clear();
    this->listVip.clear();
    
}

void LayerPopupProfile::SetLinkAvatarToServer(std::string linkAvatar) {
   // log("Send Request update aal %s", linkAvatar.c_str());
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("aal", linkAvatar);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("ruali", params));
    GameServer::getSingleton().Send(request);
}

bool LayerPopupProfile::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 120)))
    {
        return false;
    }
    
    instance = this;
    GameServer::getSingleton().addListeners(this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Size winSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //root
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupCaNhan.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    //pnlBg->setAnchorPoint(Point(0.5, 0.5));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    
    /*	Button* btnLichSu;
     Button* btnBaoMat;
     Button* btnCuaHang;
     Button* btnQuaTang;
     Button* btnHopThu;*/
    
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("txtNhan"));
    lblTitle->setString(dataManager.GetSysString(766));
    
    auto lblTK = static_cast<Text*> (pnlBg->getChildByName("Text_5_1_1"));
    //lblTK->setVisible(false);
    lblTK->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
   // lblTK->setAnchorPoint(Vec2(0,0.5));
    //lblTK->enable
    //lblTK->cu
    lblTK->setString(dataManager.GetSysString(771));

    
    auto lblNV = static_cast<Text*> (pnlBg->getChildByName("Text_5_1_1_0"));
    lblNV->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    //lblNV->setAnchorPoint(Vec2(0,0.5));
    lblNV->setString(dataManager.GetSysString(772));

    auto nhanVip = static_cast<Text*> (pnlBg->getChildByName("Text_5"));
    nhanVip->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    //nhanVip->setAnchorPoint(Vec2(0,0.5));
    nhanVip->setString(dataManager.GetSysString(773));

    auto nhanLevel = static_cast<Text*> (pnlBg->getChildByName("Text_5_0"));
    nhanLevel->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    //nhanLevel->setAnchorPoint(Vec2(0,0.5));
    nhanLevel->setString(dataManager.GetSysString(774));

    auto nhanEmail = static_cast<Text*> (pnlBg->getChildByName("Text_5_1"));
    nhanEmail->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    //nhanEmail->setAnchorPoint(Vec2(0,0.5));
    nhanEmail->setString(dataManager.GetSysString(775));

    auto nhanDate = static_cast<Text*> (pnlBg->getChildByName("Text_5_1_0"));
    //nhanDate->setAnchorPoint(Vec2(0,0.5));
    nhanDate->setContentSize(Size(176,29));
    nhanDate->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    nhanDate->setString(dataManager.GetSysString(776));

    auto nhanWin = static_cast<Text*> (pnlBg->getChildByName("Text_5_1_0_0"));
    nhanWin->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    //nhanWin->setAnchorPoint(Vec2(0,0.5));
    nhanWin->setString(dataManager.GetSysString(777));

    auto nhanbank = static_cast<Text*> (pnlBg->getChildByName("Text_5_1_0_0_0"));
    nhanbank->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    //nhanbank->setAnchorPoint(Vec2(0,0.5));
    nhanbank->setString(dataManager.GetSysString(778));
    nhanbank->setVisible(false);
    this->btnLichSu = dynamic_cast<Button*>(pnlBg->getChildByName("btnLichSu"));
    if (this->btnLichSu != NULL){
        this->btnLichSu->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonLichSu, this));
        //this->btnLichSu->setEnabled(false);
    }
    
    
    this->btnBaoMat = dynamic_cast<Button*>(pnlBg->getChildByName("btnBaoMat"));
    if (this->btnBaoMat != NULL){
        this->btnBaoMat->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonBaoMat, this));
        this->btnBaoMat->setTitleText(dataManager.GetSysString(767));
    }
    this->btnCuaHang = dynamic_cast<Button*>(pnlBg->getChildByName("btnCuaHang"));
    if (this->btnCuaHang != NULL){
        this->btnCuaHang->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonCuaHang, this));
        this->btnCuaHang->setTitleText(dataManager.GetSysString(768));

    }
    this->btnQuaTang = dynamic_cast<Button*>(pnlBg->getChildByName("btnQuaTang"));
    if (this->btnQuaTang != NULL){
        this->btnQuaTang->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonQuaTang, this));
        this->btnQuaTang->setTitleText(dataManager.GetSysString(769));
    }
    this->btnHopThu = dynamic_cast<Button*>(pnlBg->getChildByName("btnHopThu"));
    if (this->btnHopThu != NULL){
        this->btnHopThu->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonHopThu, this));
        this->btnHopThu->setTitleText(dataManager.GetSysString(770));

    }
    
    auto btn_close = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if (btn_close != NULL){
        btn_close->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonClose, this));
        btn_close->setPressedActionEnabled(true);
    }
    
    auto btnDoiMatKhau = dynamic_cast<Button*>(pnlBg->getChildByName("btnDoiMatKhau"));
    if (btnDoiMatKhau != NULL){
        btnDoiMatKhau->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonDoiMatKhau, this));
        btnDoiMatKhau->setPressedActionEnabled(true);
        btnDoiMatKhau->setTitleText(dataManager.GetSysString(779));
    }
    auto btnDoiSoTK = dynamic_cast<Button*>(pnlBg->getChildByName("btnDoiSoTK"));
    if (btnDoiSoTK != NULL){
        btnDoiSoTK->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonDoiSTK, this));
        btnDoiSoTK->setPressedActionEnabled(true);
        btnDoiSoTK->setTitleText(dataManager.GetSysString(780));

       // if (dataManager.getAppConfig().isHideBilling){
            btnDoiSoTK->setVisible(false);
       // }
    }
    
    
    this->txtName = dynamic_cast<Text*>(pnlBg->getChildByName("txtName"));
    this->txtTenHienThi = dynamic_cast<Text*>(pnlBg->getChildByName("txtTenHienThi"));

    
    this->txtGold = dynamic_cast<Text*>(pnlBg->getChildByName("txtGold"));
    this->txtSilver = dynamic_cast<Text*>(pnlBg->getChildByName("txtSilver"));
    this->txtVip = dynamic_cast<Text*>(pnlBg->getChildByName("txtVip"));
    this->txtLevel = dynamic_cast<Text*>(pnlBg->getChildByName("txtLevel"));
    this->txtEmail = dynamic_cast<Text*>(pnlBg->getChildByName("txtEmail"));
    this->txtNgayThamGia = dynamic_cast<Text*>(pnlBg->getChildByName("txtNgayThamGia"));
    this->txtSoTranThang = dynamic_cast<Text*>(pnlBg->getChildByName("txtSoTranThang"));
    this->txtSTKNganHang = dynamic_cast<Text*>(pnlBg->getChildByName("txtSTKNganHang"));
    this->txtNgayThamGia = dynamic_cast<Text*>(pnlBg->getChildByName("txtNgayThamGia"));
    this->txtName->setString("---");
    this->txtTenHienThi->setString("---");
    this->txtGold->setString("---");
    this->txtSilver->setString("---");
    this->txtVip->setString("---");
    this->txtLevel->setString("---");
    this->txtEmail->setString("---");
    this->txtNgayThamGia->setString("---");
    this->txtSoTranThang->setString("---");
    this->txtSTKNganHang->setString("---");
    this->txtSTKNganHang->setVisible(false);
    if (dataManager.getAppConfig().isHideBilling){
        auto Text_5_1_0_0_0 = dynamic_cast<Text*>(pnlBg->getChildByName("Text_5_1_0_0_0"));
        Text_5_1_0_0_0->setVisible(false);
        this->txtSTKNganHang->setVisible(false);
        
    }

    //    btnCanhan->setEnabled(false);
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //        btnCanhan->setTitleColor(ccc3(255, 255, 255));
    //    }
    //    btnThanhtich->setEnabled(true);
    //    btnDoimatkhau->setEnabled(true);
    //    //ca nhan
    //    nodeCaNhan = dynamic_cast<Node*>(rootNode->getChildByName("layerCanhan"));
    //    labelCanhanEmail = static_cast<Text*>(nodeCaNhan->getChildByName("lblMail"));
    //    labelCanhanEmail->setString("---");
    //    labelCanhanPhone = static_cast<Text*>(nodeCaNhan->getChildByName("lblPhone"));
    //    labelCanhanPhone->setString("---");
    //    labelCanhanName = static_cast<Text*>(nodeCaNhan->getChildByName("lblName"));
    //    labelCanhanName->setString("---");
    //    labelCanhanLevel = static_cast<Text*>(nodeCaNhan->getChildByName("lblLevel"));
    //    labelCanhanLevel->setString("---");
    //    lblGold = static_cast<Text*>(nodeCaNhan->getChildByName("lblGold"));
    //    lblGold->setString("---");
    //    lblSilver = static_cast<Text*>(nodeCaNhan->getChildByName("lblSilver"));
    //    lblSilver->setString("---");
    //
    //    labelCaNhanVip = static_cast<Text*>(nodeCaNhan->getChildByName("lblVipName"));
    //    labelCaNhanVip->setString("---");
    //    labelDescLevel = static_cast<Text*>(nodeCaNhan->getChildByName("txtDesLevel"));
    //    labelDescLevel->setString("---");
    //    labelDescVip = static_cast<Text*>(nodeCaNhan->getChildByName("txtDesVip"));
    //    labelDescVip->setString("---");
    //    txtSoDuKetSat = static_cast<Text*>(nodeCaNhan->getChildByName("txtMoneyKet"));
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
    //        txtSoDuKetSat->setString("---");
    //        boost::shared_ptr<ISFSObject> paramsket(new SFSObject());
    //        boost::shared_ptr<IRequest> requestket(new ExtensionRequest("safeboxacmd", paramsket));
    //        GameServer::getSingleton().Send(requestket);
    //    }
    //
    //    auto btnGotoQuyenLoi = dynamic_cast<Button*>(nodeCaNhan->getChildByName("btnInfoVip"));
    //    if (btnGotoQuyenLoi != NULL){
    //        btnGotoQuyenLoi->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonQuyenLoi, this));
    //        btnGotoQuyenLoi->setPressedActionEnabled(true);
    //    }
    //    auto btnDoiTien = dynamic_cast<Button*>(nodeCaNhan->getChildByName("btnDoiTien"));
    //    if (btnDoiTien != NULL){
    //        btnDoiTien->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonDoiTien, this));
    //        btnDoiTien->setPressedActionEnabled(true);
    //        btnDoiTien->setVisible(false);
    //    }
    //
    //    //Change Avatar
    //    this->nodeChangeAvatar = (Node*)rootNode->getChildByName("layerChangeAvatar");
    //
    //    this->GetAllUrl();
    //    this->initListAvatar();
    //
    //    this->btnChangeAvatarOK = (Button*)this->nodeChangeAvatar->getChildByName("btnChooseAvatar");
    //    if (this->btnChangeAvatarOK != NULL) {
    //        this->btnChangeAvatarOK->addTouchEventListener(CC_CALLBACK_2(LayerPopupProfile::onChangeAvatarClick, this));
    //        this->btnChangeAvatarOK->setPressedActionEnabled(true);
    //    }
    //
    //    this->btnChangeAvatarCancel = (Button*)this->nodeChangeAvatar->getChildByName("btnCancel");
    //    if (this->btnChangeAvatarCancel != NULL) {
    //        this->btnChangeAvatarCancel->addTouchEventListener(CC_CALLBACK_2(LayerPopupProfile::onChangeAvatarCancel, this));
    //        btnChangeAvatarCancel->setPressedActionEnabled(true);
    //    }
    //    // 	spriteCanhanAvatar = dynamic_cast<Sprite*>(nodeCaNhan->getChildByName("spriteAvatar"));
    //    // 	spriteCanhanLevel = dynamic_cast<Sprite*>(nodeCaNhan->getChildByName("spritelevelProgress"));
    //    ldbLevel = dynamic_cast<LoadingBar*>(nodeCaNhan->getChildByName("ldbLevel"));
    //
    //    ldbVip = dynamic_cast<LoadingBar*>(nodeCaNhan->getChildByName("ldbVip"));
    //
    //    this->avtVip = static_cast<ImageView*>(nodeCaNhan->getChildByName("imgAvatarVip"));
    //    this->avtVip->setLocalZOrder(7);
    //    this->avtVip->setVisible(false);
    //
    //    //	auto btn_canhan_naptien = dynamic_cast<Button*>(nodeCaNhan->getChildByName("btnNaptien"));
    //    //	btn_canhan_naptien->setTitleColor(Color3B(101, 18, 19));
    //    //	if (btn_canhan_naptien != NULL)
    //    //	{
    //    //		btn_canhan_naptien->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonNaptien, this));
    //    //		btn_canhan_naptien->setPressedActionEnabled(true);
    //    //	}
        this->btnDoiAvatar = dynamic_cast<Button*>(pnlBg->getChildByName("btnDoiAvatar"));
        if (this->btnDoiAvatar != NULL)
        {
            this->btnDoiAvatar->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonDoiAvatar, this));
            this->btnDoiAvatar->setPressedActionEnabled(true);
            //this->btnDoiAvatar->setVisible(false);
        }
    //    auto btn_canhan_updateinfo = dynamic_cast<Button*>(nodeCaNhan->getChildByName("btnUpdateInfo"));
    //    if (btn_canhan_updateinfo != NULL)
    //    {
    //        btn_canhan_updateinfo->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonUpdateInfo, this));
    //        btn_canhan_updateinfo->setPressedActionEnabled(true);
    //    }
        auto btn_logout = dynamic_cast<Button*>(pnlBg->getChildByName("btnDangXuat"));
        if (btn_logout != NULL)
        {
            btn_logout->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonLogout, this));
            btn_logout->setPressedActionEnabled(true);
            btn_logout->setTitleText(dataManager.GetSysString(753));
        }
        btn_lkfb = dynamic_cast<Button*>(pnlBg->getChildByName("btnLienKetFB"));
        if (btn_lkfb != NULL)
        {
            btn_lkfb->setVisible(!dataManager.getAppConfig().isHideBilling);
            auto txtBonus = dynamic_cast<Text*>(btn_lkfb->getChildByName("txtFBBonus"));
            if (txtBonus) {
                txtBonus->setString(StringUtils::format("+%d", SceneManager::getSingleton().getFacebookBonus()));
                if (SceneManager::getSingleton().isNagaWin){
                    txtBonus->setVisible(false);
                }
            }
    
            bool isLinkFB = UserDefault::getInstance()->getBoolForKey("isLinkFacebook");
            btn_lkfb->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonLKFacebook, this));
            btn_lkfb->setPressedActionEnabled(true);
            UserDefault *def = UserDefault::getInstance();
            if (std::strcmp(dataManager.getAppConfig().loginType.c_str(), "facebook") == 0){
                btn_lkfb->setVisible(false);
            }
            else{
                if (isLinkFB)
                    btn_lkfb->setVisible(false);
                else
                    btn_lkfb->setVisible(true);
            }
            if (dataManager.getAppConfig().isHideBilling){
                btn_lkfb->setVisible(false);
            }
        }
    //    //thong tin ca nhan
    //    nodeUpdateProfile = dynamic_cast<Node*>(rootNode->getChildByName("layerProfileUpdate"));
    //    btnBackInfo = dynamic_cast<Button*>(nodeUpdateProfile->getChildByName("btnChildBack"));
    //    if (btnBackInfo != NULL)
    //    {
    //        btnBackInfo->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonBackInfo, this));
    //        btnBackInfo->setPressedActionEnabled(true);
    //    }
    //
        ImageView* imgAva = dynamic_cast<ImageView*>(pnlBg->getChildByName("imgAvatar"));
        if (imgAva != NULL){
            imgAva->setTouchEnabled(true);
            imgAva->addTouchEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonDoiAvatar, this));
            this->circleAvatar = CircleAvatar::create();
            this->circleAvatar->hideIconDefault(false);
            circleAvatar->displayTypeVip(true);
            float scX = imgAva->getScaleX();
            float scY = imgAva->getScaleY();
    
            Size _sizeImg = imgAva->getContentSize();
            Size _sizeAva = this->circleAvatar->GetSizeAvatar();
    
            scX = _sizeImg.width / _sizeAva.width * scX;
            scY = _sizeImg.height / _sizeAva.height * scY;
    
            this->circleAvatar->setAnchorPoint(Vec2::ZERO);
            this->circleAvatar->setScale(scX, scY);
    
            this->circleAvatar->setPosition(imgAva->getPosition());
            pnlBg->addChild(this->circleAvatar);
            this->btnDoiAvatar->_setLocalZOrder(imgAva->getLocalZOrder() + 1);
        }
    //    txtinfoEmail = dynamic_cast<TextField*>(nodeUpdateProfile->getChildByName("txtEmail"));
    //    txtinfoPhoneOLD = dynamic_cast<TextField*>(nodeUpdateProfile->getChildByName("txtPhoneOld"));
    //    lblErrorEmail = static_cast<Text*>(nodeUpdateProfile->getChildByName("lblErrorEmail"));
    //    lblErrorEmail->setVisible(false);
    //    lblErrorPhone = dynamic_cast<Text*>(nodeUpdateProfile->getChildByName("lblErrorPhone"));
    //    lblErrorPhone->setVisible(false);
    //
    //#if(CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
    //    txtinfoEmail->setEnableEditBox(false);
    //#endif
    //    txtinfoPhone = dynamic_cast<TextField*>(nodeUpdateProfile->getChildByName("txtPhoneUpdate"));
    //#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    txtinfoPhone->setInputMode(EditBox::InputMode::PHONE_NUMBER);
    //#endif
    //    auto btn_capnhatthongtin = dynamic_cast<Button*>(nodeUpdateProfile->getChildByName("btnUpdateProfile"));
    //    if (btn_capnhatthongtin != NULL)
    //    {
    //        btn_capnhatthongtin->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonUpdateProfile, this));
    //        btn_capnhatthongtin->setPressedActionEnabled(true);
    //    }
    //
    //    auto lsvThanhtich = dynamic_cast<ListView*>(rootNode->getChildByName("lsvThanhTich"));
    //    Size sizeAdd = ChanUtils::getSizePos();
    //
    //    lsvThanhtich->setAnchorPoint(Point(0, 0));
    //    //thanh tich
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //        nodeThanhTich = dynamic_cast<Node*>(rootNode->getChildByName("layerGoldHistory"));
    //
    //        auto btnDetail = dynamic_cast<Button*>(nodeThanhTich->getChildByName("btnDetail"));
    //        if (btnDetail){
    //            btnDetail->setPressedActionEnabled(true);
    //            btnDetail->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonDetailGoldHistory, this));
    //        }
    //    }
    //    else{
    //        nodeThanhTich = dynamic_cast<Node*>(rootNode->getChildByName("nodeThanhTich"));
    //        auto ratio = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    //
    //        this->btnSilver= dynamic_cast<Button*>(nodeThanhTich->getChildByName("btnSilver"));
    //        this->btnGold= dynamic_cast<Button*>(nodeThanhTich->getChildByName("btnGold"));
    //
    //        if (this->btnSilver){
    //            this->btnSilver->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonSilver, this));
    //        }
    //        if (this->btnGold){
    //            this->btnGold->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonGold, this));
    //        }
    //        auto lsvtt = dynamic_cast<ListView*>(nodeThanhTich->getChildByName("lsvtt"));
    //        tblHistories = TableView::create(this, lsvtt->getContentSize());
    //        tblHistories->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    //        tblHistories->setPosition(lsvtt->getPosition());
    //        tblHistories->setDelegate(this);
    //        tblHistories->setTag(this->tag_Table_History);
    //        tblHistories->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //        nodeThanhTich->addChild(tblHistories);
    //        //this->addChild(nodeThanhTich);
    //    }
    //    nodeThanhTich->setVisible(false);
    //
    //    //doi mat khau
    //    nodeDoiMatKhau = dynamic_cast<Node*>(rootNode->getChildByName("layerChangPass"));
    //    txtOldPass = dynamic_cast<TextField*>(nodeDoiMatKhau->getChildByName("txtOldPass"));
    //    txtmatkhauMoi = dynamic_cast<TextField*>(nodeDoiMatKhau->getChildByName("txtNewPass"));
    //    txtnhaplaiMatKhauMoi = dynamic_cast<TextField*>(nodeDoiMatKhau->getChildByName("txtRePass"));
    //    this->bgOldPass = dynamic_cast<ImageView*>(nodeDoiMatKhau->getChildByName("Image_4"));
    //    this->txtOldpassLabel = dynamic_cast<Text*>(nodeDoiMatKhau->getChildByName("Text_10"));
    //    lblError = dynamic_cast<Text*>(nodeDoiMatKhau->getChildByName("lblError"));
    //    lblError->setVisible(false);
    //
    //    auto btn_update_password = dynamic_cast<Button*>(nodeDoiMatKhau->getChildByName("btnChangPassResult"));
    //    if (btn_update_password != NULL)
    //    {
    //        btn_update_password->addClickEventListener(CC_CALLBACK_1(LayerPopupProfile::onButtonUpdatePassword, this));
    //        btn_update_password->setPressedActionEnabled(true);
    //    }
    SceneManager::getSingleton().showLoading();

        loadAllDatas();
    //    //Send request
        boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
        if (myself == NULL)
            return false;
        //
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString("aI", myself->Name());
        boost::shared_ptr<IRequest> request(new ExtensionRequest("rgaci", params));
        GameServer::getSingleton().Send(request);
    
//    boost::shared_ptr<ISFSObject> params2(new SFSObject());
//    boost::shared_ptr<IRequest> request2(new ExtensionRequest(EXT_EVENT_GET_ACC_BANK_INFOR_REQ, params2));
//    GameServer::getSingleton().Send(request2);

    
    //    //
    //    //Get histories - Send request
    boost::shared_ptr<ISFSObject> params3(new SFSObject());

    boost::shared_ptr<IRequest> request3(new ExtensionRequest("rghba", params3));
        GameServer::getSingleton().Send(request3);
    //
    //    boost::shared_ptr<IRequest> request2(new ExtensionRequest("rglf", params));
    //    GameServer::getSingleton().Send(request2);
    //
    //
    //    string strVip = dataManager.getAppConfig().sListVip;
    //    auto lstStrVip = mUtils::splitString(strVip, '_');
    //    for (int i = 0; i < lstStrVip.size(); i++){
    //        double vip = atof(lstStrVip[i].c_str());
    //        this->listVip.push_back(vip);
    //    }
    //    for (int i = 100; i <= 117; i++){
    //        if (i == 113 || i == 114 || i == 115 || i == 104 || i == 105 || i == 101 || i == 106 || i == 108 ) continue;
    //        if (strstr(dataManager.getAppConfig().sListHiddenGame.c_str(), StringUtils::format("%d", i).c_str()))
    //            continue;
    //        lstGameID.push_back(i);
    //    }
    //
    //    isEmailEdited = true;
    //
    //#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    webView = cocos2d::experimental::ui::WebView::create();
    //    webView->setPosition(Vec2(lsvThanhtich->getPosition().x,lsvThanhtich->getPosition().y + sizeAdd.height));
    //    webView->setAnchorPoint(Vec2(0,1));
    //    webView->setContentSize(lsvThanhtich->getContentSize());
    //    //webView->loadURL("http://gamebai.net/thong-bao.php?embed=1");
    //    webView->setScalesPageToFit(true);
    //    webView->setLocalZOrder(99);
    //    this->addChild(webView);
    //    webView->setVisible(false);
    //
    //    webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerPopupProfile::onWebViewShouldStartLoading, this));
    //    webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerPopupProfile::onWebViewDidFinishLoading, this));
    //    webView->setOnDidFailLoading(CC_CALLBACK_2(LayerPopupProfile::onWebViewDidFailLoading, this));
    //#endif
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->hideWebView();
    }
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

    return true;
}

void LayerPopupProfile::onButtonDoiSTK(Ref* pSender){
    LayerPopupCapNhatSTK* popup = LayerPopupCapNhatSTK::create();
    this->addChild(popup);
}

bool LayerPopupProfile::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupProfile::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupProfile::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerPopupProfile::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    SceneManager::getSingleton().numPopup+=1;
}

void LayerPopupProfile::onExit()
{
    SceneManager::getSingleton().numPopup-=1;

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    Layer::onExit();
    if (!SceneManager::getSingleton().getIsShowTX() && !SceneManager::getSingleton().getIsShowXD()&& !SceneManager::getSingleton().getIsShowPK()&& !SceneManager::getSingleton().getIsShowVQ()&& !SceneManager::getSingleton().getIsShowCT()){

    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->showWebView();
    }
    }
    
}
void LayerPopupProfile::onButtonDoiTien(Ref* pSender){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
    webView->setVisible(false);
#endif
    LayerNapHao* popup = LayerNapHao::create();
    this->addChild(popup);
}
void LayerPopupProfile::onButtonGold(Ref* pSender)
{
    
    ////    this->btnGold->setEnabled(false);
    ////    this->btnGold->setBright(false);
    ////    this->btnSilver->setEnabled(true);
    ////    this->btnSilver->setBright(true);
    //
    //    auto type = mUtils::splitString(this->thanhtich, '/');
    //    if (type.size()>=2)
    //    {
    //        vector<string> lstHis;
    //        lstHis = mUtils::splitString(type[0], '|');
    //        //
    //        lstHistorys.clear();
    //        for (int i = 0; i < lstGameID.size(); i++){
    //            bool check = false;
    //            StructHistory historys;
    //            historys.ID = lstGameID.at(i);
    //            for (int j = 0; j < lstHis.size(); j++){
    //                vector<string> his = mUtils::splitString(lstHis.at(j), ';');
    //                if (atoi(his.at(0).c_str()) == lstGameID.at(i)){
    //                    historys.numOfLevel = atoi(his.at(3).c_str());
    //                    historys.numOfWin = atoi(his.at(1).c_str());
    //                    historys.numOfLose = atoi(his.at(2).c_str());
    //                    check = true;
    //                    break;
    //                }
    //            }
    //            if (!check){
    //                historys.numOfLevel = 0;
    //                historys.numOfWin = 0;
    //                historys.numOfLose = 0;
    //            }
    //            lstHistorys.push_back(historys);
    //        }
    //    }else{
    //        vector<string> lstHis;
    //        //
    //        lstHistorys.clear();
    //        for (int i = 0; i < lstGameID.size(); i++){
    //            bool check = false;
    //            StructHistory historys;
    //            historys.ID = lstGameID.at(i);
    //            for (int j = 0; j < lstHis.size(); j++){
    //                vector<string> his = mUtils::splitString(lstHis.at(j), ';');
    //                if (atoi(his.at(0).c_str()) == lstGameID.at(i)){
    //                    historys.numOfLevel = atoi(his.at(3).c_str());
    //                    historys.numOfWin = atoi(his.at(1).c_str());
    //                    historys.numOfLose = atoi(his.at(2).c_str());
    //                    check = true;
    //                    break;
    //                }
    //            }
    //            if (!check){
    //                historys.numOfLevel = 0;
    //                historys.numOfWin = 0;
    //                historys.numOfLose = 0;
    //            }
    //            lstHistorys.push_back(historys);
    //        }
    //    }
    //
    //
    //    //OK, reload datas now
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
    //        tblHistories->reloadData();
    //    }
    //
    
}
void LayerPopupProfile::onButtonSilver(Ref* pSender)
{
    
    //    this->btnGold->setEnabled(true);
    //    this->btnGold->setBright(true);
    //    this->btnSilver->setEnabled(false);
    //    this->btnSilver->setBright(false);
    
    //    auto type = mUtils::splitString(this->thanhtich, '/');
    //    if (type.size()>=2)
    //    {
    //        vector<string> lstHis;
    //        lstHis = mUtils::splitString(type[1], '|');
    //        //
    //        lstHistorys.clear();
    //        for (int i = 0; i < lstGameID.size(); i++){
    //            bool check = false;
    //            StructHistory historys;
    //            historys.ID = lstGameID.at(i);
    //            for (int j = 0; j < lstHis.size(); j++){
    //                vector<string> his = mUtils::splitString(lstHis.at(j), ';');
    //                if (atoi(his.at(0).c_str()) == lstGameID.at(i)){
    //                    historys.numOfLevel = atoi(his.at(3).c_str());
    //                    historys.numOfWin = atoi(his.at(1).c_str());
    //                    historys.numOfLose = atoi(his.at(2).c_str());
    //                    check = true;
    //                    break;
    //                }
    //            }
    //            if (!check){
    //                historys.numOfLevel = 0;
    //                historys.numOfWin = 0;
    //                historys.numOfLose = 0;
    //            }
    //            lstHistorys.push_back(historys);
    //        }
    //    }else{
    //        vector<string> lstHis;
    //        //
    //        lstHistorys.clear();
    //        for (int i = 0; i < lstGameID.size(); i++){
    //            bool check = false;
    //            StructHistory historys;
    //            historys.ID = lstGameID.at(i);
    //            for (int j = 0; j < lstHis.size(); j++){
    //                vector<string> his = mUtils::splitString(lstHis.at(j), ';');
    //                if (atoi(his.at(0).c_str()) == lstGameID.at(i)){
    //                    historys.numOfLevel = atoi(his.at(3).c_str());
    //                    historys.numOfWin = atoi(his.at(1).c_str());
    //                    historys.numOfLose = atoi(his.at(2).c_str());
    //                    check = true;
    //                    break;
    //                }
    //            }
    //            if (!check){
    //                historys.numOfLevel = 0;
    //                historys.numOfWin = 0;
    //                historys.numOfLose = 0;
    //            }
    //            lstHistorys.push_back(historys);
    //        }
    //    }
    //    //OK, reload datas now
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
    //        tblHistories->reloadData();
    //    }
}
void LayerPopupProfile::onButtonLichSu(Ref* pSender)
{
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //        /*btnCanhan->setTitleColor(ccc3(255, 255, 255));
    //         btnThanhtich->setTitleColor(ccc3(0, 0, 0));
    //         btnDoimatkhau->setTitleColor(ccc3(0, 0, 0));
    //         btnQuyenLoi->setTitleColor(ccc3(0, 0, 0));*/
    //    }
    //
    //    btnCanhan->setEnabled(false);
    //    btnCanhan->setBright(false);
    //    btnThanhtich->setEnabled(true);
    //    btnThanhtich->setBright(true);
    //    btnDoimatkhau->setEnabled(true);
    //    btnDoimatkhau->setBright(true);
    //    btnQuyenLoi->setEnabled(true);
    //
    //#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
    //    webView->setVisible(false);
    //#endif
    //    nodeCaNhan->setVisible(true);
    //    nodeThanhTich->setVisible(false);
    //    nodeDoiMatKhau->setVisible(false);
    //    nodeUpdateProfile->setVisible(false);
    //    this->nodeChangeAvatar->setVisible(false);
}
void LayerPopupProfile::onButtonBaoMat(Ref* pSender)
{
    //this->removeFromParent();

    auto _currScene = Director::getInstance()->getRunningScene();
    LayerPopupBaoMatCaNhan* popup = LayerPopupBaoMatCaNhan::create();
    _currScene->addChild(popup,2000,9081);
}


void LayerPopupProfile::onButtonCuaHang(Ref* pSender){
    //this->removeFromParent();

    auto _currScene = Director::getInstance()->getRunningScene();
    
    LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
    _currScene->addChild(popup,2000,9082);
    //SceneManager::getSingleton().showLoading();
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //        /*btnQuyenLoi->setTitleColor(ccc3(255, 255, 255));
    //         btnThanhtich->setTitleColor(ccc3(0, 0, 0));
    //         btnDoimatkhau->setTitleColor(ccc3(0, 0, 0));
    //         btnCanhan->setTitleColor(ccc3(0, 0, 0));*/
    //    }
    //
    //    btnCanhan->setEnabled(true);
    //    btnCanhan->setBright(true);
    //    btnThanhtich->setEnabled(true);
    //    btnThanhtich->setBright(true);
    //    btnDoimatkhau->setEnabled(true);
    //    btnDoimatkhau->setBright(true);
    //    btnQuyenLoi->setEnabled(false);
    //
    //    nodeCaNhan->setVisible(false);
    //    nodeThanhTich->setVisible(false);
    //    nodeDoiMatKhau->setVisible(false);
    //    nodeUpdateProfile->setVisible(false);
    //    this->nodeChangeAvatar->setVisible(false);
    //    /*getUser*/
    //    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    //    if (myself == NULL || myself->Name() == NULL)
    //        return;
    //    boost::shared_ptr<UserVariable> accVipType = myself->GetVariable("accVipType");
    //    if (accVipType == NULL)
    //        return;
    //    /*init linkURL*/
    //    int __vip = *accVipType->GetIntValue();
    //    string userName = *myself->Name();
    //
    //    string checksum = mUtils::getMD5KeyHis(*myself->Name());
    //
    //#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    if (!isLoadUrl){
    //        string url ="";
    //        if(SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //            url = StringUtils::format("?username=%s&checksum=%s&v=%d&l=%d",userName.c_str(),checksum.c_str(),vip,level);
    //            webView->loadURL(LOIICH_P2_VIP + url);
    //        }else{
    //            url = StringUtils::format("u=%s&c=%s&l=%d&v=%d",userName.c_str(),checksum.c_str(),level,vip);
    //            webView->loadURL(LOIICH_VIP + url);
    //        }
    //    }
    //    else
    //        webView->setVisible(true);
    //#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    //    if (!isLoadUrl){
    //        string url = "";
    //        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //            url = StringUtils::format("?username=%s&checksum=%s&v=%d&l=%d", userName.c_str(), checksum.c_str(), vip, level);
    //            Application::getInstance()->openURL(LOIICH_P2_VIP + url);
    //        }
    //        else{
    //            url = StringUtils::format("u=%s&c=%s&l=%d&v=%d", userName.c_str(), checksum.c_str(), level, vip);
    //            Application::getInstance()->openURL(LOIICH_VIP + url);
    //        }
    //    }
    //
    //#endif
}

void LayerPopupProfile::onButtonClose(Ref* pSender)
{
    this->removeFromParentAndCleanup(true);
}
void LayerPopupProfile::onButtonHopThu(Ref* pSender)

{
//    if (dataManager.getAppConfig().isHideBilling){
//        Chat* toast = Chat::create("Hiện tại không có thư nào nào!", -1);
//        SceneMain* _scene = (SceneMain*)this->getParent();
//        _scene->addChild(toast,100000);
//        return;
//    }
    auto _currScene = Director::getInstance()->getRunningScene();
   // this->removeFromParent();

    LayerPopupHomThu* popup = LayerPopupHomThu::create();
    _currScene->addChild(popup,2000,9083);

    //this->removeFromParentAndCleanup(true);
    
}
void LayerPopupProfile::onButtonQuaTang(Ref* pSender)
{
//    if (dataManager.getAppConfig().isHideBilling){
//        Chat* toast = Chat::create("Hiện tại không có quà tặng nào!", -1);
//        SceneMain* _scene = (SceneMain*)this->getParent();
//        _scene->addChild(toast,100000);
//        return;
//    }
    auto _currScene = Director::getInstance()->getRunningScene();
   // this->removeFromParent();

    LayerPopupQuaTang* popup = LayerPopupQuaTang::create();
    popup->onBtnQuaTang(NULL);
    _currScene->addChild(popup,2000,9084);

    
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //        /*btnThanhtich->setTitleColor(ccc3(255, 255, 255));
    //         btnDoimatkhau->setTitleColor(ccc3(0, 0, 0));
    //         btnQuyenLoi->setTitleColor(ccc3(0, 0, 0));
    //         btnCanhan->setTitleColor(ccc3(0, 0, 0));*/
    //    }
    //
    //    this->onButtonGold(NULL);
    //
    //    btnThanhtich->setEnabled(false);
    //    btnThanhtich->setBright(false);
    //    btnCanhan->setEnabled(true);
    //    btnCanhan->setBright(true);
    //    btnDoimatkhau->setEnabled(true);
    //    btnDoimatkhau->setBright(true);
    //    btnQuyenLoi->setEnabled(true);
    //
    //#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
    //    webView->setVisible(false);
    //#endif
    //    nodeCaNhan->setVisible(false);
    //    nodeThanhTich->setVisible(true);
    //    nodeDoiMatKhau->setVisible(false);
    //    nodeUpdateProfile->setVisible(false);
    //    this->nodeChangeAvatar->setVisible(false);
    //    this->onButtonGold(NULL);
    
}

void LayerPopupProfile::onButtonDoiMatKhau(Ref* pSender)
{
    
    LayerPopupDoiMatKhau* popup = LayerPopupDoiMatKhau::create();
    this->addChild(popup);
    
    //    //reset
    //    txtOldPass->setString("");
    //    txtmatkhauMoi->setString("");
    //    txtnhaplaiMatKhauMoi->setString("");
    //    lblError->setString("");
    //
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //        /*btnDoimatkhau->setTitleColor(ccc3(255, 255, 255));
    //         btnThanhtich->setTitleColor(ccc3(0, 0, 0));
    //         btnQuyenLoi->setTitleColor(ccc3(0, 0, 0));
    //         btnCanhan->setTitleColor(ccc3(0, 0, 0));*/
    //    }
    ////    btnCanhan->setEnabled(true);
    ////    btnThanhtich->setEnabled(true);
    ////    btnDoimatkhau->setEnabled(false);
    ////    btnQuyenLoi->setEnabled(true);
    //
    //#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
    //    webView->setVisible(false);
    //#endif
    //    nodeCaNhan->setVisible(false);
    //    nodeThanhTich->setVisible(false);
    //    nodeDoiMatKhau->setVisible(true);
    //    nodeUpdateProfile->setVisible(false);
    //    this->nodeChangeAvatar->setVisible(false);
}

void LayerPopupProfile::onButtonDoiAvatar(Ref* pSender)
{
    LayerPopupDoiAvatar* popup = LayerPopupDoiAvatar::create();
    this->addChild(popup);
    //   // log("Doi avatar");
    //
    //    this->nodeUpdateProfile->setVisible(false);
    //    this->nodeThanhTich->setVisible(false);
    //    this->nodeDoiMatKhau->setVisible(false);
    //    this->nodeCaNhan->setVisible(false);
    //
    //    this->nodeChangeAvatar->setVisible(true);
    //
    //    this->tableListAvatar->reloadData();
    //
    //    //	SceneManager::getSingleton().showLoading();
    //    //	boost::shared_ptr<ISFSObject> params(new SFSObject());
    //    //	std::string MyAI = *GameServer::getSingleton().getSmartFox()->MySelf()->Name();
    //    //	params->PutUtfString("aI", MyAI);
    //    //	boost::shared_ptr<IRequest> request(new ExtensionRequest("ire", params));
    //    //	GameServer::getSingleton().Send(request);
    //    //	log("My AccountId %s", MyAI.c_str());
    
}

void LayerPopupProfile::onButtonNaptien(Ref* pSender)
{
   // log("Nap tien");
    SceneMain* _scene = (SceneMain*)this->getParent();
    
    this->removeFromParentAndCleanup(true);
    
    if (_scene)
        _scene->gotoChargeMoney();
}

void LayerPopupProfile::onButtonUpdateInfo(Ref* pSender)
{
    //   // log("update info");
    //    nodeUpdateProfile->setVisible(true);
    //    nodeCaNhan->setVisible(false);
    //    nodeThanhTich->setVisible(false);
    //    nodeDoiMatKhau->setVisible(false);
    //    this->nodeChangeAvatar->setVisible(false);
    //    if (!isEmailEdited){
    //        //this->showToast("Bạn không được thay đổi Email!");
    //        lblErrorEmail->setString("Bạn không được thay đổi Email!");
    //        lblErrorEmail->setVisible(true);
    //#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
    //        txtinfoEmail->setEnableEditBox(false);
    //#endif
    //
    //    }
    //    else{
    //        lblErrorEmail->setVisible(false);
    //#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
    //        txtinfoEmail->setEnableEditBox(true);
    //#endif
    //    }
    //}
    //
    //void LayerPopupProfile::onButtonDetailGoldHistory(Ref* pSender){
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //        nodeThanhTich->setVisible(false);
    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    //        webView->loadURL(GOLD_HISTORY);
    //#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    //        Application::getInstance()->openURL(GOLD_HISTORY);
    //#endif
    //    }
}

void LayerPopupProfile::onButtonBackInfo(Ref* pSender)
{
    //    nodeCaNhan->setVisible(true);
    //    nodeUpdateProfile->setVisible(false);
}

void LayerPopupProfile::onButtonUpdateProfile(Ref* pSender)
{
    //   // log("Cap nhat");
    //    lblErrorEmail->setVisible(false);
    //    lblErrorPhone->setVisible(false);
    //
    //    if (!isEmailEdited)
    //        lblErrorEmail->setVisible(true);
    //    string strS;
    //    if (isEmailEdited && txtinfoEmail->getString().length() > 0){
    //        if (!mUtils::isValidEmailAddress(txtinfoEmail->getString().c_str())){
    //           // log("LayerUpdateInfo::onButtonUpdate email: %s is not valid!", txtinfoEmail->getString().c_str());
    //            lblErrorEmail->setVisible(true);
    //            lblErrorEmail->setString("Định dạng email không đúng!");
    //            return;
    //        }
    //        else{
    //            string a = txtinfoEmail->getString();
    //            auto lst = mUtils::splitString(a,'@');
    //            if (lst.size() >= 2){
    //                if (lst[0].length()>=6){
    //                    lblErrorEmail->setVisible(false);
    //                }else{
    //                    lblErrorEmail->setVisible(true);
    //                    lblErrorEmail->setString("Định dạng email không đúng!");
    //                    return;
    //
    //                }
    //            }else{
    //                lblErrorEmail->setVisible(true);
    //                lblErrorEmail->setString("Định dạng email không đúng!");
    //                return;
    //
    //
    //            }
    //
    //        }
    //        strS = (txtinfoEmail->getString());
    //        if (mUtils::trimString(strS).length() != 0 && mUtils::trimString(strS).length() < 6){
    //            lblErrorEmail->setVisible(true);
    //            lblErrorEmail->setString(dataManager.GetSysString(43));
    //            return;
    //        }
    //        else{
    //            lblErrorEmail->setVisible(false);
    //        }
    //    }
    //    //Validate
    //
    //    if (!isEmailEdited || strcmp(txtinfoEmail->getString().c_str(), "") == 0){
    //        if (phoneNumber.length() > 0){
    //            if (strcmp(txtinfoPhoneOLD->getString().c_str(), "") == 0){
    //                lblErrorPhone->setVisible(true);
    //                lblErrorPhone->setString("Bạn chưa nhập số điện thoại cũ!");
    //                return;
    //            }
    //            else{
    //                lblErrorPhone->setVisible(false);
    //            }
    //
    //            if (mUtils::findContainChar(txtinfoPhoneOLD->getString().c_str())){
    //                lblErrorPhone->setVisible(true);
    //                lblErrorPhone->setString("Chỉ được nhập số!");
    //                return;
    //            }
    //            else{
    //                lblErrorPhone->setVisible(false);
    //            }
    //            if (strcmp(txtinfoPhone->getString().c_str(), "") == 0){
    //                lblErrorPhone->setVisible(true);
    //                lblErrorPhone->setString("Bạn chưa nhập số điện thoại mới!");
    //                return;
    //            }
    //            else{
    //                lblErrorPhone->setVisible(false);
    //            }
    //
    //            if (mUtils::findContainChar(txtinfoPhone->getString().c_str())){
    //                lblErrorPhone->setVisible(true);
    //                lblErrorPhone->setString("Chỉ được nhập số!");
    //                return;
    //            }
    //            else{
    //                lblErrorPhone->setVisible(false);
    //            }
    //
    //            string txtInputPhone = txtinfoPhoneOLD->getString().c_str();
    //            if (strcmp(txtInputPhone.c_str(), phoneNumber.c_str()) != 0){
    //                lblErrorPhone->setVisible(true);
    //                lblErrorPhone->setString("Số điện thoại cũ nhập vào chưa đúng!");
    //                return;
    //            }
    //            else{
    //                lblErrorPhone->setVisible(false);
    //            }
    //            if (mUtils::findContainChar(txtinfoPhone->getString().c_str())){
    //                lblErrorPhone->setVisible(true);
    //                lblErrorPhone->setString("Chỉ được nhập số!");
    //                return;
    //            }
    //            else{
    //                lblErrorPhone->setVisible(false);
    //            }
    //            if (txtinfoPhone->getString().length() < 10){
    //                lblErrorPhone->setVisible(true);
    //                lblErrorPhone->setString("Số điện thoại nhập vào không đúng định dạng!");
    //                return;
    //            }
    //            else{
    //                lblErrorPhone->setVisible(false);
    //            }
    //        }
    //        else {
    //
    //            if (strcmp(txtinfoPhone->getString().c_str(), "") == 0){
    //                lblErrorPhone->setVisible(true);
    //                lblErrorPhone->setString("Bạn chưa nhập số điện thoại mới!");
    //                return;
    //            }
    //            else{
    //                lblErrorPhone->setVisible(false);
    //            }
    //
    //            if (txtinfoPhone->getString().length() < 10){
    //                lblErrorPhone->setVisible(true);
    //                lblErrorPhone->setString("Số điện thoại nhập vào không đúng định dạng!");
    //                return;
    //            }
    //            else{
    //                lblErrorPhone->setVisible(false);
    //            }
    //        }
    //    }
    //
    //    //Send request
    //    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    //    if (myself == NULL || myself->GetVariable("aN") == NULL)
    //        return;
    //    //
    //    boost::shared_ptr<ISFSObject> params(new SFSObject());
    //    params->PutUtfString("aI", myself->Name());
    //    params->PutUtfString("aN", *myself->GetVariable("aN")->GetStringValue());
    //    if (txtinfoPhone->getString().length() > 0){
    //        params->PutUtfString("aMo", txtinfoPhone->getString());
    //    }
    //    else {
    //        params->PutUtfString("aMo", phoneNumber);
    //    }
    //    if (email.length() == 0 && txtinfoEmail->getString().length() > 0){
    //        params->PutUtfString("aM", txtinfoEmail->getString());
    //    }
    //    else if (email.length() > 0){
    //        params->PutUtfString("aM", email);
    //
    //    }
    //    params->PutInt("aSt", 1);
    //    params->PutInt("aT", 1);
    //    params->PutInt("aS", true);
    //    boost::shared_ptr<IRequest> request(new ExtensionRequest("rua", params));
    //    GameServer::getSingleton().Send(request);
    //
    //    SceneManager::getSingleton().showLoading();
    //    //this->showToast("Cập nhật thông tin thành công!");
    //    this->onButtonBackInfo(NULL);
    //    //Send request
}
std::string LayerPopupProfile::tail(std::string const& source, size_t const length) {
    if (length >= source.size()) { return source; }
    return source.substr(source.size() - length);
} // tail
void LayerPopupProfile::onButtonUpdatePassword(Ref* pSender)
{
    //   // log("onButtonUpdate");
    //
    //    std::string strNewPass = string(txtmatkhauMoi->getString());
    //    for (int i = 0; i < strNewPass.size(); ++i)
    //    {
    //        if (strNewPass.at(i) == ' '){
    //            lblError->setVisible(true);
    //            lblError->setString("Mật khẩu không được chứa ký tự đặc biệt");
    //            return;
    //        }
    //    }
    //    string _oldPass_ = txtOldPass->getString();
    //    boost::shared_ptr<string> _pass = PasswordUtil::MD5Password(_oldPass_);
    //    _pass = PasswordUtil::MD5Password(*_pass);
    //
    //    if (strcmp(_oldPass.c_str(), "") == 0){
    //
    //    }
    //    else{
    //
    //        if (_pass->compare(_oldPass) != 0){
    //            //this->showToast("Mật khẩu cũ không đúng!");
    //            lblError->setVisible(true);
    //            lblError->setString("Mật khẩu cũ không đúng!");
    //            return;
    //        }
    //        else{
    //            lblError->setVisible(false);
    //        }
    //
    //    }
    //
    //    //Validate
    //    int len1 = (txtOldPass->getString().length());
    //    int len2 = (txtmatkhauMoi->getString().length());
    //    int len3 = (txtnhaplaiMatKhauMoi->getString().length());
    //
    //    if (txtmatkhauMoi->getString().compare(txtnhaplaiMatKhauMoi->getString()) != 0){
    //        lblError->setVisible(true);
    //        lblError->setString(dataManager.GetSysString(42));
    //        return;
    //    }
    //    else{
    //        lblError->setVisible(false);
    //    }
    //    if (len2 < 6 || len2 >24 || len3 < 6 || len3 >24){
    //        lblError->setVisible(true);
    //        lblError->setString(dataManager.GetSysString(41));
    //        return;
    //    }
    //    else{
    //        lblError->setVisible(false);
    //    }
    //    if (strcmp(_name.c_str(), txtmatkhauMoi->getString().c_str()) == 0){
    //        lblError->setString("Tài khoản và mật khẩu không được giống nhau!");
    //        lblErrorPhone->setVisible(true);
    //        return;
    //    }
    //    else{
    //        lblError->setVisible(false);
    //    }
    //    if (mUtils::isNumber(txtmatkhauMoi->getString().c_str())){
    //        lblError->setVisible(true);
    //        lblError->setString("Mật khẩu phải bao gồm cả số và chữ!");
    //        return;
    //    }
    //    else{
    //        lblError->setVisible(false);
    //    }
    //    if (!mUtils::checkAlpha(txtmatkhauMoi->getString().c_str())){
    //        lblError->setVisible(true);
    //        lblError->setString("Mật khẩu phải bao gồm cả số và chữ!");
    //        return;
    //    }
    //    else{
    //        lblError->setVisible(false);
    //    }
    //    if (strcmp(_oldPass_.c_str(), txtmatkhauMoi->getString().c_str()) == 0){
    //        lblError->setVisible(true);
    //        lblError->setString("Mật khẩu cũ và mật khẩu mới không được giống nhau!");
    //
    //        return;
    //    }
    //    else{
    //        lblError->setVisible(false);
    //    }
    //
    //    //Send request
    //    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    //    if (myself == NULL)
    //        return;
    //    //Send request
    //    boost::shared_ptr<ISFSObject> params(new SFSObject());
    //    params->PutUtfString("aI", myself->Name());
    //
    //    params->PutUtfString("aP", _pass);
    //
    //
    //    //md5
    //    boost::shared_ptr<string> _npass = PasswordUtil::MD5Password(txtmatkhauMoi->getString());
    //    _npass = PasswordUtil::MD5Password(*_npass);
    //    params->PutUtfString("aNP", _npass);
    //
    //    boost::shared_ptr<IRequest> request(new ExtensionRequest("rcp", params)); //Change password
    //    GameServer::getSingleton().Send(request);
}

void LayerPopupProfile::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size LayerPopupProfile::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    //    if (table->getTag() == this->tag_Table_History){
    //        return Size(table->getContentSize().width, 158);
    //    }
    //    else if (table->getTag() == this->tag_Table_Avatar){
    //        return Size(table->getContentSize().width, (475 / 2));
    //    }
    return Size(0, 0);
}

TableViewCell* LayerPopupProfile::tableCellAtIndex(TableView *table, ssize_t idx)
{
    //    if (table->getTag() == this->tag_Table_History){
    //        TableViewCell *cell = table->dequeueCell();
    //        StructHistory his = lstHistorys.at(idx);
    //        if (!cell) {
    //
    //            cell = new TableViewCell();
    //            cell->autorelease();
    //
    //            LayerThanhTichCell* thanhtich = LayerThanhTichCell::create(his.ID, his.numOfWin, his.numOfLevel, his.numOfLose, 0);
    //
    //            thanhtich->setTag(123);
    //            cell->addChild(thanhtich);
    //
    //        }
    //        else
    //        {
    //            LayerThanhTichCell* thanhtich = (LayerThanhTichCell*)cell->getChildByTag(123);
    //            if (thanhtich){
    //                thanhtich->setDatas(his.ID, his.numOfWin, his.numOfLevel, his.numOfLose, 0);
    //            }
    //        }
    //        return cell;
    //    }
    //
    //    else if (table->getTag() == this->tag_Table_Avatar){
    //        TableViewCell* cell = table->dequeueCell();
    //        if (!cell) {
    //            cell = new TableViewCell();
    //            cell->autorelease();
    //            cell->setAnchorPoint(Vec2(0, 0));
    //            for (int i = 0; i < 4; ++i) {
    //                try{
    //                    int index = i + idx * 4;
    //                    string url = SceneManager::getSingleton().arrLink.at(index);
    //                    ChangeAvatar* sp = ChangeAvatar::create(url);
    //                   // log("url init: %s", url.c_str());
    //                    sp->setPosition(Vec2(i * 320, 0));
    //                    sp->setTag(index);
    //                    sp->onCallBack(this, callfuncO_selector(LayerPopupProfile::changeAvatarCallBack));
    //                    cell->addChild(sp);
    //                    this->arrAvatarChange.push_back(sp);
    //                }
    //                catch (...){
    //
    //                }
    //            }
    //        }
    //        else {
    //            for (int i = 0; i < 4; ++i) {
    //                try{
    //                    int index = i + idx * 4;
    //                    string url = SceneManager::getSingleton().arrLink.at(index);
    //                   // log("url reload: %s", url.c_str());
    //                    ChangeAvatar* sp = (ChangeAvatar*)cell->getChildByTag(index);
    //                    if (sp != NULL){
    //                        sp->setIcon(url);
    //                        sp->onCallBack(this, callfuncO_selector(LayerPopupProfile::changeAvatarCallBack));
    //                    }
    //                }
    //                catch (...){
    //
    //                }
    //            }
    //        }
    //        return cell;
    //    }
    //
    return NULL;
}
void LayerPopupProfile::onButtonLKFacebook(Ref* pSender){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    mFaceBook* fb = new mFaceBook();
    fb->setFunCallBack(this, callfuncND_selector(LayerPopupProfile::doLienKetFB));
    fb->onGetUID();
#endif
}
ssize_t LayerPopupProfile::numberOfCellsInTableView(TableView *table)
{
    //    if (table->getTag() == this->tag_Table_History){
    //        return lstHistorys.size();
    //    }
    //    else if (table->getTag() == this->tag_Table_Avatar){
    //        return 2;
    //    }
    return 0;
}

void LayerPopupProfile::loadAllDatas()
{
    //labelCanhanLevel->setString("sfdf");
    boost::shared_ptr<double> amf,amfs;
    boost::shared_ptr<string> aCt;
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL || myself->GetVariable("amf") == NULL
        || myself->GetVariable("lvl") == NULL || myself->GetVariable("amfs") == NULL|| myself->GetVariable("aCt") == NULL || myself->GetVariable("aal") == NULL || myself->GetVariable("adn") == NULL){
        //		log("myself %s= NULL | myself->GetVariable(am) %s= NULL | myself->GetVariable(amf)%s=NULL",
        //			myself == NULL ? "=" : "!", myself->GetVariable("am") == NULL ? "=" : "!", myself->GetVariable("amf") == NULL ? "=" : "!");
        return;
    }
    this->txtTenHienThi->setString(*myself->GetVariable("adn")->GetStringValue());

    amf = myself->GetVariable("amf")->GetDoubleValue();
    amfs = myself->GetVariable("amfs")->GetDoubleValue();
    int vip = *myself->GetVariable("accVipType")->GetIntValue();
    aCt = myself->GetVariable("aCt")->GetStringValue();

    if (amf)
        this->txtGold->setString(mUtils::convertMoneyEx(*amf).c_str());
    if (amfs)
        this->txtSilver->setString(mUtils::convertMoneyEx(*amfs).c_str());
    if (vip)
        this->txtVip->setString(StringUtils::format("%d", vip));
    if (aCt)
        this->txtNgayThamGia->setString(*aCt);

    boost::shared_ptr<std::string> s = myself->GetVariable("aal")->GetStringValue();
    if (s != NULL && this->circleAvatar != NULL){
        this->circleAvatar->setAvatarImage(*s);
    }
    //
}
void LayerPopupProfile::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    this->removeFromParentAndCleanup(true);
}
void LayerPopupProfile::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    //
    boost::shared_ptr<double> am, amf,amfs;
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL || myself->GetVariable("amf") == NULL
        || myself->GetVariable("lvl") == NULL
        || myself->GetVariable("exp") == NULL || myself->GetVariable("amfs") == NULL){
        //		log("myself %s= NULL | myself->GetVariable(am) %s= NULL | myself->GetVariable(amf)%s=NULL",
        //			myself == NULL ? "=" : "!", myself->GetVariable("am") == NULL ? "=" : "!", myself->GetVariable("amf") == NULL ? "=" : "!");
        return;
    }
    amf = myself->GetVariable("amf")->GetDoubleValue();
    amfs = myself->GetVariable("amfs")->GetDoubleValue();
    //
   // log("LayerUpdateInfo::OnExtensionResponse: cmd = %s", cmd->c_str());
    
    if (0 == strcmp("uavatar", cmd->c_str())){
        boost::shared_ptr<long> rc = param->GetInt("rc");
        boost::shared_ptr<string> rd = param->GetUtfString("rd");
        if (rc != NULL && rd != NULL){
           // log("update avatar: rc = %ld", *rc);
           // log("update avatar: rd = %s", rd->c_str());
        }
        //        this->nodeCaNhan->setVisible(true);
        //        this->nodeThanhTich->setVisible(false);
        //        this->nodeDoiMatKhau->setVisible(false);
        //        this->nodeUpdateProfile->setVisible(false);
        //        this->nodeChangeAvatar->setVisible(false);
    }
    
    if (strcmp("gaic", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();


        //        if (param->GetUtfString("aP") != NULL && param->GetUtfString("aM") != NULL){
        //Insert datas to textfield
        //            _oldPass = *param->GetUtfString("aP");
        //            if (strcmp("", _oldPass.c_str()) == 0){
        //                this->txtOldPass->setVisible(false);
        //                this->txtOldPass->setEnableEditBox(false);
        //                this->bgOldPass->setVisible(false);
        //                this->txtOldpassLabel->setVisible(false);
        //            }
        //txtinfoPhone->setString(*param->GetUtfString("aMo"));
        //            string a = *param->GetUtfString("aM");
        //            auto strs = mUtils::splitString(a, '@');
        //            string emailHide = "";
        //            if (strcmp(a.c_str(), "") == 0)
        //                emailHide = "";
        //            else
        //                emailHide = "******" + tail(strs[0], 4) + "@" + strs[1];
        //            txtinfoEmail->setString(emailHide);
        //        }
        //        if (txtinfoEmail->getString().length() != 0){
        //            isEmailEdited = false;
        //        }
        //        else{
        //            isEmailEdited = true;
        //#if(CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
        //#endif
        //        }
        if (param->GetUtfString("aM") != NULL && param->GetUtfString("aMo") != NULL){
            string a = *param->GetUtfString("aM");
            //email = a;
            auto strs = mUtils::splitString(a, '@');
            string emailHide = "";
            if (strcmp(a.c_str(), "") == 0)
                emailHide = "---";
            else
                emailHide = "******" + tail(strs[0], 4) + "@" + strs[1];
            this->txtEmail->setString(emailHide);
            //            phoneNumber = *param->GetUtfString("aMo");
            //            string phoneHide = "";
            //            if (strcmp(phoneNumber.c_str(), "") == 0)
            //                phoneHide = "";
            //            else
            //                phoneHide = "******" + tail(phoneNumber, 4);
            //            labelCanhanPhone->setString(phoneHide);
            
            UserDefault *def = UserDefault::getInstance();
            def->setStringForKey("aM", *param->GetUtfString("aM"));
            def->setStringForKey("aMo", *param->GetUtfString("aMo"));
        }
        this->txtLevel->setString(StringUtils::format("%d", *myself->GetVariable("lvl")->GetIntValue()));
        if (param->GetUtfString("aN") != NULL){
            _name = *param->GetUtfString("aN");
            if (_name.compare("") == 0)
                _name = *myself->Name();
            this->txtName->setString(_name);
        }
        this->txtGold->setString(mUtils::convertMoneyEx(*amf).c_str());
        this->txtSilver->setString(mUtils::convertMoneyEx(*amfs).c_str());
        //        if (myself->GetVariable("lvl")->GetIntValue() != NULL && myself->GetVariable("exp") != NULL){
        //            if (myself->GetVariable("aM") != NULL)
        //                labelCanhanEmail->setString(*myself->GetVariable("aM")->GetStringValue());
        //            ldbLevel->setPercent(dataManager.getLevelProgress(*myself->GetVariable("lvl")->GetIntValue(), *myself->GetVariable("exp")->GetIntValue()));
        //            int levelNext = *myself->GetVariable("lvl")->GetIntValue() + 1;
        //            level = *myself->GetVariable("lvl")->GetIntValue();
        //
        //            if (*myself->GetVariable("lvl")->GetIntValue() < 100)
        //                this->labelDescLevel->setString(StringUtils::format("Cần thêm %d EXP để lên Level %d", dataManager.getExpNeed(*myself->GetVariable("lvl")->GetIntValue(), *myself->GetVariable("exp")->GetIntValue()), levelNext));
        //            else
        //                this->labelDescLevel->setVisible(false);
        //        }
        
        
        try{
            boost::shared_ptr<std::string> s = myself->GetVariable("aal")->GetStringValue();
            if (s != NULL && this->circleAvatar != NULL){
                this->circleAvatar->setAvatarImage(*s);
            }
        }
        catch (...){
           // log("exception");
        }
        if (myself->GetVariable("accVipType") != NULL){
            int vip = *myself->GetVariable("accVipType")->GetIntValue();
            //            if (vip == 0)
            //                this->labelCaNhanVip->setString("Bạn chưa là Vip");
            //            else
            this->txtVip->setString(StringUtils::format("%d", vip));
            //            avtVip->loadTexture(mUtils::getSrcVip(vip));
            //            this->circleAvatar->setTypeVip(vip);
            //            if (myself->GetVariable("am") != NULL){
            //                double money = *myself->GetVariable("am")->GetDoubleValue();
            //                if (vip < listVip.size()){
            //                    double moneyVipNeed = listVip[vip] * 1000000;
            //                    auto moneyVipNext = moneyVipNeed - money;
            //                    this->labelDescVip->setString(StringUtils::format("Nạp thêm %s để lên VIP %d", mUtils::convertMoneyEx(moneyVipNext).c_str(), vip + 1));
            //                    ldbVip->setPercent((money / moneyVipNeed) * 100);
            //                }
            //                else{
            //                    this->labelDescVip->setVisible(false);
            //                    ldbVip->setPercent(100);
            //                }
            //
            //            }
            //       }
        }
    }
    else if (strcmp("rsglf", cmd->c_str()) == 0){
       // log("lien ket fb chua: %d", *param->GetInt("lst"));
       // log("account Type: %d", *param->GetInt("acctype"));
        //this->showToast(param->GetUtfString("rd")->c_str());
    }
    else if (strcmp("rsilf", cmd->c_str()) == 0){
        auto scene = Director::getInstance()->getRunningScene();
        if (!scene) return;
        //// log("LK fb:%s",*param->GetUtfString("rd")->c_str());
        if (param->GetInt("rc") == NULL) return;
        int rc = *param->GetInt("rc");
        if (rc == 0){
           // string bonus = *param->GetUtfString("coinBonus");
          //  auto lstbonus = mUtils::splitString(bonus, '|');
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(564));
            
            
            //			if (atoi(lstbonus[0].c_str()) > 0){
            layerPopup->setContentMess(dataManager.GetSysString(565));
            scene->addChild(layerPopup,2000);
            //			}
            //			else if (atoi(lstbonus[1].c_str()) > 0){
            //				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s Exp quà liên kết facebook!", lstbonus[1].c_str()));
            //				this->addChild(layerPopup);
            //			}
            //			else if (atoi(lstbonus[2].c_str()) > 0){
            //				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s lần mở rương vip quà liên kết facebook!", lstbonus[2].c_str()));
            //				this->addChild(layerPopup);
            //			}
            //			else if (atoi(lstbonus[3].c_str()) > 0){
            //				layerPopup->setContentMess(StringUtils::format("Bạn nhận được %s lượt quay vòng quay may mắn quà liên kết facebook!", lstbonus[3].c_str()));
            //				this->addChild(layerPopup);
            //			}
            btn_lkfb->setVisible(false);
            UserDefault *def = UserDefault::getInstance();
            def->setBoolForKey("isLinkFacebook", true);
            dataManager.getAppConfig().isreceivedLinkFace = true;
        }
        else{

            string desc = "";
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            
            scene->addChild(layerPopup,2000);
            
            switch (rc) {
                case 1:
                    desc = dataManager.GetSysString(686);
                    layerPopup->setContentMess(desc);
                    break;
                case 2:
                    desc = dataManager.GetSysString(687);
                    layerPopup->setContentMess(desc);
                    break;
                case 3:
                    desc = dataManager.GetSysString(688);
                    layerPopup->setContentMess(desc);
                    break;
                case 4:
                    desc = dataManager.GetSysString(689);
                    layerPopup->setContentMess(desc);
                    break;
                    
            }
        }
        
    }
    
    else if (strcmp("cpc", cmd->c_str()) == 0){
       // log("change pass rc: %ld", *param->GetInt("rc"));
        
        if (*param->GetInt("rc") == 0){//OK
           // log("lbfree OK");
            //            this->showToast(dataManager.GetSysString(25));
            //            this->txtOldPass->setVisible(true);
            //            this->txtOldPass->setEnableEditBox(true);
            //            this->bgOldPass->setVisible(true);
            //            this->txtOldpassLabel->setVisible(true);
            
        }
        else{//Not OK
           // log("lbfree Not OK - error: %s", param->GetUtfString("rd")->c_str());
            //this->showToast(dataManager.GetSysString(26));
        }
    }
    else if (strcmp("rghba", cmd->c_str()) == 0){ //History response
        if (param->GetUtfString("rghba") == NULL)
            return;
       // log("History response: %s", param->GetUtfString("rghba")->c_str());
        auto type = mUtils::splitString(*param->GetUtfString("rghba"),'/');
        int tongtranthang = 0;
            if (type.size()>=2)
            {
                vector<string> lstHis;
                lstHis = mUtils::splitString(type[0], '|');
                for (int j = 0; j < lstHis.size(); j++){
                    vector<string> his = mUtils::splitString(lstHis.at(j), ';');
                    tongtranthang+=atoi(his.at(1).c_str());
                }
            }else if (type.size()==1){
                vector<string> lstHis;
                lstHis = mUtils::splitString(type[0], '|');
                for (int j = 0; j < lstHis.size(); j++){
                    vector<string> his = mUtils::splitString(lstHis.at(j), ';');
                    tongtranthang+=atoi(his.at(1).c_str());
                }
            }
        if (type.size()>=2)
        {
            vector<string> lstHis;
            lstHis = mUtils::splitString(type[1], '|');
            for (int j = 0; j < lstHis.size(); j++){
                vector<string> his = mUtils::splitString(lstHis.at(j), ';');
                tongtranthang+=atoi(his.at(1).c_str());
            }
        }else if (type.size()==1){
            vector<string> lstHis;
            lstHis = mUtils::splitString(type[1], '|');
            for (int j = 0; j < lstHis.size(); j++){
                vector<string> his = mUtils::splitString(lstHis.at(j), ';');
                tongtranthang+=atoi(his.at(1).c_str());
            }
        }
        this->txtSoTranThang->setString(StringUtils::format("%d",tongtranthang));
    }
    if (strcmp("ire", cmd->c_str()) == 0)
    {
        boost::shared_ptr<string> token = param->GetUtfString("ire");
        if (token == NULL)
            return;
       // log("Tocken Upload = %s", token->c_str());
        SceneManager::getSingleton().hideLoading();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        IOS::tryOpenPhoto(token->c_str());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        NativeHelper::callChooseImage(token->c_str());
#endif
    }
    else if (strcmp("safeboxaresp", cmd->c_str()) == 0){
        if (param->GetDouble("safeboxa") == NULL) return;
        double currMoneyInKet = *param->GetDouble("safeboxa");
        // this->txtSoDuKetSat->setString(mUtils::convertMoneyEx(currMoneyInKet));
    }
    else  if (strcmp(EXT_EVENT_GET_ACC_BANK_INFOR_RESP, cmd->c_str()) == 0){//OK
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetUtfString("rd") == NULL) return;
        string bankstr = *param->GetUtfString("rd");
        //1|2;TCB;Ngân hàng TMCP Kỹ Thương Việt Nam;Nguyen Tuan Anh;124124124124124124;Ha Noi|1;2;5;10;20;50
        auto lstbank = mUtils::splitString(bankstr,'|');
        if (lstbank.size() < 5) return;
        if (atoi(lstbank[0].c_str()) == 0){
            this->txtSTKNganHang->setString("---");
        }else{
            auto infobank = mUtils::splitString(lstbank[1],';');
            if (infobank.size()<6)
                return;
            this->txtSTKNganHang->setString(infobank[4]);

        }
        if (dataManager.getAppConfig().isHideBilling){
            this->txtSTKNganHang->setVisible(false);
        }
    }
    SceneManager::getSingleton().hideLoading();
}
void LayerPopupProfile::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    if (ptrEventParams == NULL)
        return;
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    if (ptrNotifiedUser->IsItMe() == true)
        loadAllDatas();
}
void LayerPopupProfile::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    auto locationInNode = this->convertToNodeSpace(pTouch->getLocation());
    auto s = this->getContentSize();
    auto rect = Rect(0, 0, s.width, s.height);
    if (!rect.containsPoint(locationInNode))
    {
        this->removeFromParentAndCleanup(true);
    }
}

void LayerPopupProfile::onChangeAvatarClick(Ref* pSender, ui::Widget::TouchEventType type){
    if (type == ui::Widget::TouchEventType::ENDED){
        
        string url = "";
        for (int i = 0; i < this->arrAvatarChange.size(); ++i) {
            ChangeAvatar* obj = this->arrAvatarChange.at(i);
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
    }
}

void LayerPopupProfile::onChangeAvatarCancel(Ref* pSender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED){
        //        this->nodeCaNhan->setVisible(true);
        //        this->nodeThanhTich->setVisible(false);
        //        this->nodeDoiMatKhau->setVisible(false);
        //        this->nodeUpdateProfile->setVisible(false);
        //        this->nodeChangeAvatar->setVisible(false);
    }
}

void LayerPopupProfile::showToast(string mess)
{
    Chat *toast = Chat::create(mess, -1);
    this->addChild(toast);
}

void LayerPopupProfile::onButtonLogout(Ref* pSender){
    //    auto layerpoup = LayerPopupThongBao::create();
    //    layerpoup->setMessage("Đăng xuất");
    //    layerpoup->setContentMess("Bạn có muốn đăng xuất không?");
    //    layerpoup->setCallBackFromButton(this, callfuncO_selector(LayerPopupProfile::onPopupCallBack), 1);
    //    this->addChild(layerpoup, 100000000);
    boost::shared_ptr<IRequest> request(new LogoutRequest());
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().isShouldAuto = false;
    //
    SceneManager::getSingleton().isLogout = true;
    SceneManager::getSingleton().rcForLogin = false;
    SceneManager::getSingleton().gotoLogin();
    SceneManager::getSingleton().isLogin =false;
    //this->removeFromParent();
}
void LayerPopupProfile::doLienKetFB(Node* sender, void* data){
    string *value = (string*)data;
    string uid = *value;
    if (strcmp(uid.c_str(), "cancel") == 0){
        this->removeFromParentAndCleanup(true);
        return;
    }
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("fbID", uid);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("rilf", params));
    GameServer::getSingleton().Send(request);
    this->removeFromParentAndCleanup(true);
}
LayerThanhTichCell* LayerThanhTichCell::create(int idGame, int soTranThang, int soTranHoa, int soTranThua, int soTranBoCuoc)
{
    LayerThanhTichCell* thanhtich = new LayerThanhTichCell();
    
    if (thanhtich && thanhtich->init(idGame, soTranThang, soTranHoa, soTranThua, soTranBoCuoc))  {
        return thanhtich;
    }
    else{
        CC_SAFE_DELETE(thanhtich);
        return NULL;
    }
}

bool LayerThanhTichCell::init(int idGame, int soTranThang, int soTranHoa, int soTranThua, int soTranBoCuoc)
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerItemThanhTich.csb");
    ui::Helper::doLayout(rootNode);
    
    avatarGame = dynamic_cast<ImageView*>(rootNode->getChildByName("imgGame"));
    labelTenGame = static_cast<Text*>(rootNode->getChildByName("lblNameGame"));
    labelSoTranThang = static_cast<Text*>(rootNode->getChildByName("lblThang"));
    labelSoTranHoa = static_cast<Text*>(rootNode->getChildByName("lblHoa"));
    labelSoTranThua = static_cast<Text*>(rootNode->getChildByName("lblThua"));
    labelSoTranBoCuoc = static_cast<Text*>(rootNode->getChildByName("lblBoCuoc"));
    //labelTenGame = dynamic_cast<>
    
    this->addChild(rootNode);
    
    setDatas(idGame, soTranThang, soTranHoa, soTranThua, soTranBoCuoc);
    
    return true;
}

LayerThanhTichCell::LayerThanhTichCell(void)
{
    
}

LayerThanhTichCell::~LayerThanhTichCell(void)
{
    
}

void LayerThanhTichCell::setDatas(int idGame, int soTranThang, int soTranHoa, int soTranThua, int soTranBoCuoc)
{
    labelTenGame->setString(*mUtils::getGameNameByID(idGame));
    labelSoTranThang->setString(StringUtils::format("%d", soTranThang));
    labelSoTranHoa->setString(StringUtils::format("%d", soTranHoa));
    labelSoTranThua->setString(StringUtils::format("%d", soTranThua));
    labelSoTranBoCuoc->setString(StringUtils::format("%d", soTranBoCuoc));
    //change icon
    string sUrl = mUtils::getUrlImageGame(idGame);
    avatarGame->loadTexture(sUrl);
    avatarGame->setScale9Enabled(false);
    avatarGame->setContentSize(Size(135, 135));
}

void LayerPopupProfile::initListAvatar(){
    
//    Layout* pnlListAvatar = (Layout*)this->nodeChangeAvatar->getChildByName("pnlListAvatar");
//    
//    if (pnlListAvatar == NULL){
//       // log("pnlListAvatar == NULL");
//    }
//    
//    Size size = pnlListAvatar->getContentSize();
//   // log("size = %f : %f", size.width, size.height);
//    
//    this->tableListAvatar = TableView::create(this, pnlListAvatar->getContentSize());
//    this->tableListAvatar->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
//    this->tableListAvatar->setAnchorPoint(Vec2(0, 0));
//    this->tableListAvatar->setPosition(Vec2(0, 0));
//    this->tableListAvatar->setDelegate(this);
//    this->tableListAvatar->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
//    this->tableListAvatar->setTag(this->tag_Table_Avatar);
//    pnlListAvatar->addChild(this->tableListAvatar);
}



void LayerPopupProfile::changeAvatarCallBack(Ref *pSend){
//    ChangeAvatar* change = (ChangeAvatar*)pSend;
//    if (change != NULL){
//       // log("avatar choose: %s", change->getUrl().c_str());
//    }
//    for (int i = 0; i < this->arrAvatarChange.size(); ++i) {
//        this->arrAvatarChange.at(i)->visibleIconCheck(false);
//    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerPopupProfile::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().showLoading();
    webView->setVisible(false);
    return true;
    
}

void LayerPopupProfile::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
    isLoadUrl = true;
    //webView->setVisible(true);
}

void LayerPopupProfile::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
}
#endif

// Change Avatar Layer
ChangeAvatar::ChangeAvatar(const string& url){
    this->url = url;
    this->isChoose = false;
    this->iconCheck = NULL;
    this->circleAvatar = NULL;
    this->mCallBack = NULL;
    this->mCallBackListener = NULL;
}

ChangeAvatar::~ChangeAvatar(){
   // log("ChangeAvatar::~ChangeAvatar()");
}

ChangeAvatar* ChangeAvatar::create(const string& url){
    ChangeAvatar* a = new ChangeAvatar(url);
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

bool ChangeAvatar::init(){
    if (!Layer::init())
        return false;
    
    this->setAnchorPoint(Vec2(0, 0));
    
    this->circleAvatar = CircleAvatar::create();
    //this->circleAvatar->hideIconDefault(false);
    this->circleAvatar->setPosition(Vec2(140, 475 / 4));
    
    ImageView* trans = ImageView::create("icon_transparent_default.png");
    trans->setAnchorPoint(Vec2(0.5, 0.5));
    trans->setPosition(Vec2(140, 475 / 4));
    trans->setTouchEnabled(true);
    trans->addTouchEventListener(CC_CALLBACK_2(ChangeAvatar::onAvatarTouches, this));
    this->addChild(trans);
    
    Size _si = Size(148, 148);
    this->iconCheck = Sprite::create("change-avatar-select.png");
    this->iconCheck->setAnchorPoint(Vec2(0.5, 0.5));
    this->iconCheck->setPosition(Vec2(_si.width / 2 - 30, _si.height / 2 - 30));
    this->iconCheck->setScale(0.8);
    this->iconCheck->setVisible(false);
    

    
    this->setIcon(this->url);
    
    return true;
}

void ChangeAvatar::setIcon(string& url){
    this->url = url;
    this->circleAvatar->setAvatarImage(url);
}

string& ChangeAvatar::getUrl() {
    return this->url;
}

bool ChangeAvatar::getIsChoose(){
    return this->isChoose;
}

void ChangeAvatar::visibleIconCheck(bool state) {
    this->iconCheck->setVisible(state);
    this->isChoose = state;
}

void ChangeAvatar::onCallBack(Ref *target, SEL_CallFuncO listener){
    this->mCallBack = target;
    this->mCallBackListener = listener;
}

void ChangeAvatar::onAvatarTouches(Ref* pSender, ui::Widget::TouchEventType type){
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if (this->mCallBack && this->mCallBackListener) {
            (this->mCallBack->*this->mCallBackListener)(this);
        }
        this->iconCheck->setVisible(!this->iconCheck->isVisible());
        this->isChoose = !this->isChoose;
    }
}

