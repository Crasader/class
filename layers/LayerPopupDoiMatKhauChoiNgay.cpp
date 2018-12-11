//
//  LayerPopupDoiMatKhauChoiNgay.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/19/17.
//
//

#include "LayerPopupDoiMatKhauChoiNgay.h"
//
//  LayerPopupDoiMatKhauChoiNgay.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/24/17.
//
//

#include "LayerPopupDoiMatKhauChoiNgay.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupThongBao.h"
#include "../data_manager.h"
#include "../layergames/_Chat_.h"
#include "LayerPopupOTP.h"
#include "Util/PasswordUtil.h"
LayerPopupDoiMatKhauChoiNgay::LayerPopupDoiMatKhauChoiNgay()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupDoiMatKhauChoiNgay::~LayerPopupDoiMatKhauChoiNgay()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerPopupDoiMatKhauChoiNgay::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupDoiMatKhauChoiNgay.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
        rootNode->setAnchorPoint(Point(0.5, 0.5));
        rootNode->setPosition(Point(visibleSize / 2));
        Size sizeAdd;
    //    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
            sizeAdd = ChanUtils::getSizePos();
//
//        }else{
//            sizeAdd = Size(0,0);
//        }
        Size sizeAdd2 = Size(0,0);
        rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiMatKhauChoiNgay::onBtnClose, this));
    }
    auto lblTK = static_cast<Text*> (pnlBg->getChildByName("Text_5_1_1"));
    //lblTK->setVisible(false);
    lblTK->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    lblTK->setAnchorPoint(Vec2(0,0.5));
    //lblTK->enable
    //lblTK->cu
    lblTK->setString(dataManager.GetSysString(771));
    
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(959));
    this->txtmatkhauMoi = static_cast<TextField*>(pnlBg->getChildByName("txtNewPass"));
    this->txtmatkhauMoi->setPlaceHolder(dataManager.GetSysString(844));

    this->txtnhaplaiMatKhauMoi = static_cast<TextField*>(pnlBg->getChildByName("txtNewRePass"));
    this->txtnhaplaiMatKhauMoi->setPlaceHolder(dataManager.GetSysString(845));

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    this->txtmatkhauMoi->setMaxLength(20);

    this->txtnhaplaiMatKhauMoi->setMaxLength(20);

#endif
    Button* btnCapNhat = dynamic_cast<Button*>(pnlBg->getChildByName("btnCapNhat"));
    if(btnCapNhat){
        btnCapNhat->addClickEventListener(CC_CALLBACK_1(LayerPopupDoiMatKhauChoiNgay::onBtnCapNhat, this));
        btnCapNhat->setTitleText(dataManager.GetSysString(786));
    }
    
    this->lblError = static_cast<Text*>(pnlBg->getChildByName("txtError"));
    Text* txtName = dynamic_cast<Text*>(pnlBg->getChildByName("txtName"));

    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself != NULL){
        
        if (myself->GetVariable("aN")){
            string _strName = *myself->GetVariable("aN")->GetStringValue();
            txtName->setString(_strName);
        }
    }
    
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
       return true;
}
void LayerPopupDoiMatKhauChoiNgay::onBtnOTP(Ref* pSender){
    LayerPopupOTP* otp = LayerPopupOTP::create();
    this->addChild(otp);
}
void LayerPopupDoiMatKhauChoiNgay::onBtnCapNhat(Ref* pSender){
       // log("onButtonUpdate");
    
        std::string strNewPass = string(txtmatkhauMoi->getString());
        for (int i = 0; i < strNewPass.size(); ++i)
        {
            if (strNewPass.at(i) == ' '){
                lblError->setVisible(true);
                lblError->setString(dataManager.GetSysString(468));
                return;
            }
        }
    
    
    
    
        //Validate
        int len2 = (txtmatkhauMoi->getString().length());
        int len3 = (txtnhaplaiMatKhauMoi->getString().length());
    
        if (txtmatkhauMoi->getString().compare(txtnhaplaiMatKhauMoi->getString()) != 0){
            lblError->setVisible(true);
            lblError->setString(dataManager.GetSysString(467));
            return;
        }
        else{
            lblError->setVisible(false);
        }
        if (len2 < 6 || len2 >24 || len3 < 6 || len3 >24){
            lblError->setVisible(true);
            lblError->setString(dataManager.GetSysString(466));
            return;
        }
        else{
            lblError->setVisible(false);
        }
        if (strcmp(_name.c_str(), txtmatkhauMoi->getString().c_str()) == 0){
            lblError->setString(dataManager.GetSysString(698));
            lblError->setVisible(true);
            return;
        }
        else{
            lblError->setVisible(false);
        }
        if (mUtils::isNumber(txtmatkhauMoi->getString().c_str())){
            lblError->setVisible(true);
            lblError->setString(dataManager.GetSysString(471));
            return;
        }
        else{
            lblError->setVisible(false);
        }
        if (!mUtils::checkAlpha(txtmatkhauMoi->getString().c_str())){
            lblError->setVisible(true);
            lblError->setString(dataManager.GetSysString(471));
            return;
        }
        else{
            lblError->setVisible(false);
        }
    
    
        //Send request
        boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
        if (myself == NULL)
            return;
        //Send request
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString("aI", myself->Name());
    
        params->PutUtfString("aP", "");
    
    
        //md5
        boost::shared_ptr<string> _npass = PasswordUtil::MD5Password(txtmatkhauMoi->getString());
        _npass = PasswordUtil::MD5Password(*_npass);
        params->PutUtfString("aNP", _npass);
    
        boost::shared_ptr<IRequest> request(new ExtensionRequest("rcp", params)); //Change password
        GameServer::getSingleton().Send(request);
}

void LayerPopupDoiMatKhauChoiNgay::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerPopupDoiMatKhauChoiNgay::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
     if (strcmp("cpc", cmd->c_str()) == 0){
       // log("change pass rc: %ld", *param->GetInt("rc"));
        
        if (*param->GetInt("rc") == 0){//OK
           // log("lbfree OK");
            SceneManager::getSingleton().showToast(dataManager.GetSysString(148));
            this->txtmatkhauMoi->setString("");
            this->txtnhaplaiMatKhauMoi->setString("");
            
        }
        else{//Not OK
           // log("lbfree Not OK - error: %s", param->GetUtfString("rd")->c_str());
            SceneManager::getSingleton().showToast(*param->GetUtfString("rd"));
        }
    }
   
}
void LayerPopupDoiMatKhauChoiNgay::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    
   }

bool LayerPopupDoiMatKhauChoiNgay::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupDoiMatKhauChoiNgay::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupDoiMatKhauChoiNgay::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupDoiMatKhauChoiNgay::onExit()
{
    dataManager.getAppConfig().isShowMatKhauDone = true;
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

