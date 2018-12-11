//
//  LayerPopupBaoMatCaNhan.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/19/17.
//
//

#include "LayerPopupBaoMatCaNhan.h"
//
//  LayerPopupBaoMatCaNhan.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/24/17.
//
//

#include "LayerPopupBaoMatCaNhan.h"
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
#include "../scenes/ScenePickRoom.h"
LayerPopupBaoMatCaNhan::LayerPopupBaoMatCaNhan()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupBaoMatCaNhan::~LayerPopupBaoMatCaNhan()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerPopupBaoMatCaNhan::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupBaoMatCaNhan.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
        rootNode->setAnchorPoint(Point(0.5, 0.5));
        rootNode->setPosition(Point(visibleSize / 2));
        Size sizeAdd;
   //     if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
            sizeAdd = ChanUtils::getSizePos();
    
//        }else{
//            sizeAdd = Size(0,0);
//        }
        Size sizeAdd2 = Size(0,0);
        rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(781));
    auto lblTip = static_cast<Text*> (pnlBg->getChildByName("Text_5_1_0"));
    lblTip->setString(dataManager.GetSysString(787));
    auto lblTK = static_cast<Text*> (pnlBg->getChildByName("Text_5_1"));
    lblTK->setString(dataManager.GetSysString(788));
    lblTK->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);

    
    
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupBaoMatCaNhan::onBtnClose, this));
    }
    
    auto txtName = static_cast<Text*>(pnlBg->getChildByName("txtName"));
    txtName->setString(SceneManager::getSingleton().getMyName());
    
    this->txtCMTND = static_cast<TextField*>(pnlBg->getChildByName("txtCMTND"));
    this->txtCMTND->setPlaceHolder(dataManager.GetSysString(782));
    this->txtEmail = static_cast<TextField*>(pnlBg->getChildByName("txtEmail"));
    this->txtEmail->setPlaceHolder(dataManager.GetSysString(783));

    this->txtSDT = static_cast<TextField*>(pnlBg->getChildByName("txtSDT"));
    this->txtSDT->setPlaceHolder(dataManager.GetSysString(784));

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    this->txtCMTND->setInputMode(ui::EditBox::InputMode::NUMERIC);
    this->txtSDT->setInputMode(ui::EditBox::InputMode::NUMERIC);
    this->txtCMTND->setMaxLength(13);
    this->txtSDT->setMaxLength(13);
#endif
    Button* btnCapNhat = dynamic_cast<Button*>(pnlBg->getChildByName("btnCapNhat"));
    if(btnCapNhat){
        btnCapNhat->addClickEventListener(CC_CALLBACK_1(LayerPopupBaoMatCaNhan::onBtnCapNhat, this));
        btnCapNhat->setTitleText(dataManager.GetSysString(786));

    }
    
    this->txtErrorCMT = static_cast<Text*>(pnlBg->getChildByName("txtErrorCMT"));
    this->txtErrorSDT = static_cast<Text*>(pnlBg->getChildByName("txtErrorSDT"));
    this->txtErrorEmail = static_cast<Text*>(pnlBg->getChildByName("txtErrorEmail"));
    
    auto btnKichHoatOTP = dynamic_cast<Button*>(pnlBg->getChildByName("btnKichHoatOTP"));
    if(btnKichHoatOTP){
        btnKichHoatOTP->addClickEventListener(CC_CALLBACK_1(LayerPopupBaoMatCaNhan::onBtnOTP, this));
        btnKichHoatOTP->setTitleText(dataManager.GetSysString(785));
        btnKichHoatOTP->setVisible(false);
    }
    
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
void LayerPopupBaoMatCaNhan::onBtnOTP(Ref* pSender){
    LayerPopupOTP* otp = LayerPopupOTP::create();
    this->addChild(otp);
}
void LayerPopupBaoMatCaNhan::onBtnCapNhat(Ref* pSender){
       // log("Cap nhat");
        this->txtErrorCMT->setVisible(false);
        this->txtErrorSDT->setVisible(false);
       this->txtErrorEmail->setVisible(false);
 
        string strS;
        if (this->txtEmail->getString().length() > 0){
            if (!mUtils::isValidEmailAddress(this->txtEmail->getString().c_str())){

                this->txtErrorEmail->setVisible(true);
                this->txtErrorEmail->setString(dataManager.GetSysString(595));
                return;
            }
            else{
                string a = this->txtEmail->getString();
                auto lst = mUtils::splitString(a,'@');
                if (lst.size() == 2){
                    int n = std::count(lst[1].begin(), lst[1].end(), '.');
                    if (n >= 2){
                        this->txtErrorEmail->setVisible(true);
                        this->txtErrorEmail->setString(dataManager.GetSysString(595));
                        return;
                    }
                    else{
                        this->txtErrorEmail->setVisible(false);

                    }
                    if (lst[0].length()>=6){
                        this->txtErrorEmail->setVisible(false);
                    }else{
                        this->txtErrorEmail->setVisible(true);
                        this->txtErrorEmail->setString(dataManager.GetSysString(595));
                        return;
    
                    }
                }else{
                    this->txtErrorEmail->setVisible(true);
                    this->txtErrorEmail->setString(dataManager.GetSysString(595));
                    return;
    
    
                }
    
            }
            strS = (this->txtEmail->getString());
            if (mUtils::trimString(strS).length() != 0 && mUtils::trimString(strS).length() < 6){
                this->txtErrorEmail->setVisible(true);
                this->txtErrorEmail->setString(dataManager.GetSysString(43));
                return;
            }
            else{
                this->txtErrorEmail->setVisible(false);
            }



            if(strS.find_first_not_of(' ') != std::string::npos)
            {
                this->txtErrorEmail->setVisible(false);

            }else{
                this->txtErrorEmail->setVisible(true);
                this->txtErrorEmail->setString(dataManager.GetSysString(595));
                return;
            }
            vector<string> lstStr2 = mUtils::splitString(strS,' ');
			if (lstStr2.size() >= 2){
                this->txtErrorEmail->setVisible(true);
                this->txtErrorEmail->setString(dataManager.GetSysString(595));
                return;
            }
            else{
                this->txtErrorEmail->setVisible(false);
            }

            
        }
        //Validate
    
      //  if (strcmp(this->txtEmail->getString().c_str(), "") == 0){
                if (strcmp(this->txtSDT->getString().c_str(), "") == 0){
                    this->txtErrorSDT->setVisible(true);
                    this->txtErrorSDT->setString(dataManager.GetSysString(521));
                    return;
                }
                else{
                    this->txtErrorSDT->setVisible(false);
                }
    
                if (mUtils::findContainChar(this->txtSDT->getString().c_str())){
                    this->txtErrorSDT->setVisible(true);
                    this->txtErrorSDT->setString(dataManager.GetSysString(560));
                    return;
                }
                else{
                    this->txtErrorSDT->setVisible(false);
                }
    
//                if (this->txtSDT->getString().length() < 10){
//                    this->txtErrorSDT->setVisible(true);
//                    this->txtErrorSDT->setString(dataManager.GetSysString(650));
//                    return;
//                }
//                else{
//                    this->txtErrorSDT->setVisible(false);
//                }
    
    if (this->txtEmail->getString().length() == 0){
        this->txtErrorEmail->setVisible(true);
        this->txtErrorEmail->setString(dataManager.GetSysString(522));
        return;
    }
    else{
        this->txtErrorCMT->setVisible(false);
    }
    
    if (this->txtCMTND->getString().length() == 0){
        this->txtErrorCMT->setVisible(true);
        this->txtErrorCMT->setString(dataManager.GetSysString(523));
        return;
    }
    else{
        this->txtErrorCMT->setVisible(false);
    }
    
    if (this->txtCMTND->getString().length() < 9){
        this->txtErrorCMT->setVisible(true);
        this->txtErrorCMT->setString(dataManager.GetSysString(651));
        return;
    }
    else{
        this->txtErrorCMT->setVisible(false);
    }
    if (this->txtCMTND->getString().length() >12){
        this->txtErrorCMT->setVisible(true);
        this->txtErrorCMT->setString(dataManager.GetSysString(651));
        return;
    }
    else{
        this->txtErrorCMT->setVisible(false);
    }
    if (mUtils::findContainChar(this->txtCMTND->getString().c_str())){
        this->txtErrorCMT->setVisible(true);
        this->txtErrorCMT->setString(dataManager.GetSysString(560));
        return;
    }
    else{
        this->txtErrorCMT->setVisible(false);
    }
    
        //Send request
        boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
        if (myself == NULL || myself->GetVariable("aN") == NULL)
            return;
        //
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString("aI", myself->Name());
        params->PutUtfString("aN", *myself->GetVariable("aN")->GetStringValue());
        //if (this->txtSDT->getString().length() > 0){
            params->PutUtfString("aMo", this->txtSDT->getString());
        //}
        //else {
        //    params->PutUtfString("aMo", phoneNumber);
        //}
        //if (email.length() == 0 && txtinfoEmail->getString().length() > 0){
        params->PutUtfString("aM", this->txtEmail->getString());
        params->PutUtfString("cmt", this->txtCMTND->getString());
        //}
        //else if (email.length() > 0){
        //    params->PutUtfString("aM", email);
    
       // }
        params->PutInt("aSt", 1);
        params->PutInt("aT", 1);
        params->PutInt("aS", true);
        boost::shared_ptr<IRequest> request(new ExtensionRequest("rua", params));
        GameServer::getSingleton().Send(request);
    
        SceneManager::getSingleton().showLoading();
    //    //this->showToast("Cập nhật thông tin thành công!");
    //    this->onButtonBackInfo(NULL);
    //    //Send request
}

void LayerPopupBaoMatCaNhan::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerPopupBaoMatCaNhan::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp("uac", cmd->c_str()) == 0){
        if (param->GetInt("rc") != NULL){
            int rc = *param->GetInt("rc");
            string rd = *param->GetUtfString("rd");
            if (rc == 0){
                this->txtCMTND->setString("");
                this->txtEmail->setString("");
                this->txtSDT->setString("");
                auto layerThongbao = LayerPopupThongBao::create();
                layerThongbao->setMessage(dataManager.GetSysString(141));
                layerThongbao->setContentMess(dataManager.GetSysString(561));
                layerThongbao->getBtnHuy()->setVisible(false);
                layerThongbao->getBtnOk()->setPositionX(Director::getInstance()->getVisibleSize().width / 2);
                this->addChild(layerThongbao);
                boost::shared_ptr<ISFSObject> params1(new SFSObject());
                params1->PutUtfString("aI", SceneManager::getSingleton().getMyName());
                boost::shared_ptr<IRequest> request1(new ExtensionRequest("rgaci", params1));
                GameServer::getSingleton().Send(request1);
                SceneManager::getSingleton().hideLoading();
            }
            else{
                auto layerThongbao = LayerPopupThongBao::create();
                layerThongbao->setMessage(dataManager.GetSysString(141));
                layerThongbao->setContentMess(rd);
                layerThongbao->getBtnHuy()->setVisible(false);
                layerThongbao->getBtnOk()->setPositionX(Director::getInstance()->getVisibleSize().width / 2);
                this->addChild(layerThongbao);
                SceneManager::getSingleton().hideLoading();
            }
        }
    }
  
}
void LayerPopupBaoMatCaNhan::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    
   }

bool LayerPopupBaoMatCaNhan::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupBaoMatCaNhan::onEnter()
{
    
    Layer::onEnter();
    SceneManager::getSingleton().numPopup+=1;

    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupBaoMatCaNhan::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupBaoMatCaNhan::onExit()
{
    Layer::onExit();
    SceneManager::getSingleton().numPopup-=1;

    if (!SceneManager::getSingleton().getIsShowTX() && !SceneManager::getSingleton().getIsShowXD()&& !SceneManager::getSingleton().getIsShowPK()&& !SceneManager::getSingleton().getIsShowVQ()&& !SceneManager::getSingleton().getIsShowCT()){

    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return ;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->showWebView();
    }
    }
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

