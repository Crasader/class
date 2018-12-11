#include "LayerRechargeTop.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LayerRecharge.h"
#include "../scenes/SceneMain.h"
#include "../scenes/ScenePickRoom.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "../SceneManager.h"
USING_NS_CC;
using namespace network;
LayerRechargeTop::LayerRechargeTop()
{
    this->btnNotification = NULL;
    this->btnBonus = NULL;
    GameServer::getSingleton().addListeners(this);
}

LayerRechargeTop::~LayerRechargeTop()
{
    this->lstButtons.clear();
    this->lstLinks.clear();
    GameServer::getSingleton().removeListeners(this);
}

bool LayerRechargeTop::init()
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
        //btnNotification->addClickEventListener(CC_CALLBACK_1(LayerRechargeTop::onButtonNotification, this));
        this->btnNotification->setVisible(false);
    }
    btnBonus = dynamic_cast<Button*>(rootNode->getChildByName("btnTinKhuyenMai"));
    if (btnBonus != NULL){
        //btnBonus->addClickEventListener(CC_CALLBACK_1(LayerRechargeTop::onButtonBonus, this));
        this->btnBonus->setVisible(false);
    }
    //get layerTopMenuOnlyTop
    // 	auto layerTopMenuOnlyTop = dynamic_cast<Node*>(rootNode->getChildByName("layerTopMenuOnlyTop"));
    // 	if (layerTopMenuOnlyTop != NULL){
    auto layerTopMenuOnlyTopChild = dynamic_cast<Node*>(rootNode->getChildByName("layerTopMenuOnlyTop"));
    if (layerTopMenuOnlyTopChild != NULL){
        auto btnClose = dynamic_cast<Button*>(layerTopMenuOnlyTopChild->getChildByName("btnBack"));
        if (btnClose != NULL){
            btnClose->addClickEventListener(CC_CALLBACK_1(LayerRechargeTop::onButtonClose, this));
            btnClose->setPressedActionEnabled(true);
        }
        
    }
    loadTab();
    isGet = false;
    this->setContentSize(rootNode->getContentSize());
  

    return true;
}

void LayerRechargeTop::onExit(){
    Layer::onExit();
    
}

void LayerRechargeTop::loadTab(){
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerRechargeTop::getBtnNoti),this,0.1, false);
}

void LayerRechargeTop::getBtnNoti(float dt){
    //SceneManager::getSingleton().showLoading();
    if (isGet)
        return;
    int size = SceneManager::getSingleton().lstTabRecharge.size();
    if (size == 0)
        return;
    auto orgX = (DESIGN_RESOLUTION_WIDTH - ((size*290)+(size-1)*50))/2;
    for (rapidjson::SizeType i = 0; i < size; ++i){
        auto button = ui::Button::create();
        button->loadTextures("design/guifiles/popup-header-tabs-no.png", "design/guifiles/popup-header-tabs.png", "design/guifiles/popup-header-tabs.png");
        button->setAnchorPoint(Point(0, 0.5));
        button->setTitleText(SceneManager::getSingleton().lstTabRecharge[i].name);
        button->setTitleFontSize(50);
        //button->setTitleFontName("__Roboto-Bold.ttf");
        button->setTitleColor(ccc3(255, 227, 160));
        button->setTag(i);
        button->setVisible(false);
        
        /*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
        button->setPosition(Point(orgX + (i*340), btnNotification->getPositionY() - 10));
        button->addClickEventListener(CC_CALLBACK_1(LayerRechargeTop::onButtonClicked, this));
        this->addChild(button);
        this->lstButtons.push_back(button);
        this->lstLinks.push_back(SceneManager::getSingleton().lstTabRecharge[i].link);
    }
    SceneManager::getSingleton().hideLoading();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("pmchkrq", params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
    if (!isGet)
        isGet = true;
}

void LayerRechargeTop::onButtonClicked(Ref* pSender){
    for (int i = 0; i < this->lstButtons.size(); ++i){
        this->lstButtons.at(i)->setEnabled(true);
        /*this->lstButtons.at(i)->setTitleColor(ccc3(255, 227, 160));*/
    }
    auto button = (ui::Button*)pSender;
    this->lstButtons.at(button->getTag())->setEnabled(false);
   
    auto parent = (LayerRecharge*)this->getParent();
     parent->onBtnHideInApp();
    /*this->lstButtons.at(button->getTag())->setTitleColor(ccc3(223, 0, 41));*/
    if(button->getTag()==0){
        if (parent != NULL){
            parent->onBtnShowInApp();
            parent->hideWebView();
        }
    }
    else{
        if (parent != NULL){
            boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
            if (myself == NULL || myself->Name() == NULL)
                return;
            /*init linkURL*/
            string link = this->lstLinks[button->getTag()];
            link += +"?u="+*myself->Name();
            link += "&c=";
            link += mUtils::getMD5KeyHis(*myself->Name());
            parent->reloadData(link);
            parent->showWebView();
        }
    }
    
}
void LayerRechargeTop::loadFirst(){
}
void LayerRechargeTop::showLocalPayment(bool isShowInApp, bool isShowLocal){
    if (lstButtons.size()==1){
        onButtonClicked(this->lstButtons.at(0));
    }
    if (dataManager.getAppConfig().isHideBilling)
    {
        this->lstButtons.at(0)->setVisible(true);

        onButtonClicked(this->lstButtons.at(0));
        if (this->lstButtons.size()>1){
            for (int i =1;i<this->lstButtons.size();i++){
                this->lstButtons.at(i)->setVisible(false);
            }
        }
           }
    else if (!dataManager.getAppConfig().isHideBilling){
         auto parent = (LayerRecharge*)this->getParent();
         this->lstButtons.at(0)->setVisible(isShowInApp);
        if (this->lstButtons.size()>1){
            for (int i =1;i<this->lstButtons.size();i++){
                this->lstButtons.at(i)->setVisible(isShowLocal);
            }
        }
        if(isShowLocal)
        {
            onButtonClicked(this->lstButtons.at(1));
            if (parent != NULL){
                parent->showWebView();
            }
        }
        else{
            onButtonClicked(this->lstButtons.at(0));
            if (parent != NULL){
                parent->onBtnShowInApp();
                parent->hideWebView();
            }

        }
        
    }
    
}
void LayerRechargeTop::onButtonClose(Ref* pSender)
{
    ScenePickRoom* _parent1 = (ScenePickRoom*)this->getParent()->getParent();
    LayerRecharge* recharge = (LayerRecharge*)this->getParent();
    if (_parent != NULL){
    if (recharge->currScene == 2){
        //_parent1->gotoPickGame();
        _parent1->layerScenePickRoomBottom->setVisible(true);
        this->getParent()->removeFromParentAndCleanup(true);
        return;
    }
    }

    if (this->getParent() == NULL)
        return;
    SceneMain* _parent = (SceneMain*)this->getParent()->getParent();
    if (_parent != NULL){
        if(recharge->currScene == 1)
        _parent->gotoPickGame();
    }
    
}
void LayerRechargeTop::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    
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
    else if (strcmp("pmchkrs", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        string cf = *param->GetUtfString("pmstt");
        auto cfs = mUtils::splitString(cf, '|');
        if (cfs.size() > 1){
            bool isShowInapp = false;
            bool isShowLocalPayment = false;
            if (strcmp("0", cfs[0].c_str()) == 0)
                isShowLocalPayment = false;
            else
                isShowLocalPayment = true;
            if (strcmp("0", cfs[1].c_str()) == 0)
                isShowInapp = false;
            else
                isShowInapp = true;
            this->showLocalPayment(isShowInapp, isShowLocalPayment);
        }
    }

//    else if (strcmp("rsiapp", cmd->c_str()) == 0){//OK
//        int rc = *param->GetInt("rc");
//        if (rc == 0){
//            string valuePur = "";
//            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
//            layerPopup->showAButton();
//            layerPopup->setMessage("Chúc mừng");
//            layerPopup->setContentMess("Nạp tiền thành công!");
//            this->addChild(layerPopup);
//            SceneManager::getSingleton().hideLoading();
//        }
//        else{
//            string valuePur = "";
//            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
//            layerPopup->showAButton();
//            layerPopup->setMessage("Chúc mừng");
//            layerPopup->setContentMess("Nạp tiền thất bại. Vui lòng thử lại !");
//            this->addChild(layerPopup);
//            SceneManager::getSingleton().hideLoading();
//        }
//    }
}
