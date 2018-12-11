//
//  LayerPopupCapNhatSTK.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerPopupCapNhatSTK.h"
#include "cocostudio/CocoStudio.h"
#include "Requests/ExtensionRequest.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "LayerPopupDaiLy.h"
#include "LayerPopupOTP.h"
#include "LayerPopupThongBao.h"
#include "../layergames/_Chat_.h"
LayerPopupCapNhatSTK::LayerPopupCapNhatSTK()
{
    GameServer::getSingleton().addListeners(this);

}


LayerPopupCapNhatSTK::~LayerPopupCapNhatSTK()
{
    GameServer::getSingleton().removeListeners(this);

}

bool LayerPopupCapNhatSTK::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupCapNhatSTK.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
 //   if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupCapNhatSTK::onBtnClose, this));
    }

    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    this->nodeSTK = static_cast<Node*>(pnlBg->getChildByName("nodeSTK"));
     this->btnChonNganHang = dynamic_cast<Button*>(this->nodeSTK->getChildByName("btnChonSTK"));
    if( this->btnChonNganHang){
         this->btnChonNganHang->addClickEventListener(CC_CALLBACK_1(LayerPopupCapNhatSTK::onBtnChonNganHang, this));
    }
    Button* btnDoi = dynamic_cast<Button*>(this->nodeSTK->getChildByName("btnDoi"));
    if(btnDoi){
        btnDoi->addClickEventListener(CC_CALLBACK_1(LayerPopupCapNhatSTK::onBtnDoi, this));
    }
    
    this->txtTK= static_cast<TextField*>(this->nodeSTK->getChildByName("txtTK"));

    this->txtChuTK =static_cast<TextField*>(this->nodeSTK->getChildByName("txtChuTK"));

    this->txtChiNhanh= static_cast<TextField*>(this->nodeSTK->getChildByName("txtChiNhanh"));
    this->txtErrorTK= static_cast<Text*>(this->nodeSTK->getChildByName("txtErrorTK"));
    this->txtErrorChiNhanh= static_cast<Text*>(this->nodeSTK->getChildByName("txtErrorChiNhanh"));
    this->txtErrorTenNganHang= static_cast<Text*>(this->nodeSTK->getChildByName("txtErrorTenNganHang"));
    this->txtErrorChuTK= static_cast<Text*>(this->nodeSTK->getChildByName("txtErrorChuTK"));
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //this->txtTK->setInputMode(ui::EditBox::InputMode::NUMERIC);
    this->txtChuTK->setMaxLength(30);
    this->txtTK->setMaxLength(25);
    this->txtChiNhanh->setMaxLength(30);

#endif
    this->txtErrorTK->setVisible(false);
    this->txtErrorChiNhanh->setVisible(false);
    this->txtErrorTenNganHang->setVisible(false);
    this->txtErrorChuTK->setVisible(false);
    
    this->nodeOTP = static_cast<Node*>(pnlBg->getChildByName("nodeOTP"));
    this->txtOTP =  static_cast<TextField*>(this->nodeOTP->getChildByName("txtOTP"));
    
    Button* btnBack = dynamic_cast<Button*>(this->nodeOTP->getChildByName("btnBack"));
    if(btnBack){
        btnBack->addClickEventListener(CC_CALLBACK_1(LayerPopupCapNhatSTK::onBtnBack, this));
    }
    Button* btnXacNhan = dynamic_cast<Button*>(this->nodeOTP->getChildByName("btnXacNhan"));
    if(btnXacNhan){
        btnXacNhan->addClickEventListener(CC_CALLBACK_1(LayerPopupCapNhatSTK::onBtnXacNhan, this));
    }
    
    this->nodeOTP->setVisible(false);
    this->nodeSTK->setVisible(true);

    this->pnlNganHang = dynamic_cast<Layout*>(this->nodeSTK->getChildByName("pnlNganHang"));
    this->pnlNganHang->setVisible(false);
    this->pnlNganHang->setSwallowTouches(false);
    this->scrollView = ui::ScrollView::create();
    this->scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollView->setClippingEnabled(true);
    this->scrollView->setContentSize(this->pnlNganHang->getContentSize());
    this->scrollView->setAnchorPoint(this->pnlNganHang->getAnchorPoint());
    this->scrollView->setPosition(this->pnlNganHang->getPosition());
    this->scrollView->setScrollBarEnabled(true);
    this->scrollView->setBounceEnabled(true);
    this->scrollView->setTag(12932);
    this->nodeSTK->addChild(this->scrollView);
       //this->txtChuChay->setScale(5);
    
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_SUPPORTED_BANK_REQ, params));
    GameServer::getSingleton().Send(request);

    SceneManager::getSingleton().showLoading();

    return true;
}
void LayerPopupCapNhatSTK::onBtnBack(Ref* pSender){
    this->nodeOTP->setVisible(false);
    this->nodeSTK->setVisible(true);
}
void LayerPopupCapNhatSTK::onBtnXacNhan(Ref* pSender){
    
    string otp = this->txtOTP->getString();
    
    if (otp == ""){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(145));
        return;
    }
    
    string number = this->txtTK->getString();
    string branch = this->txtChiNhanh->getString();
    string name = this->txtChuTK->getString();
    
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt(BANK_ID, this->lstbank[this->currentClick].id);
    params->PutUtfString(BANK_NUMBER, number);
    params->PutUtfString(BANK_NAME, name);
    params->PutUtfString(BANK_BRANCH, branch);
    params->PutUtfString("otpcode", otp);

    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_UPDATE_BANK_ACC_REQ,params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}

void LayerPopupCapNhatSTK::onBtnChonNganHang(Ref* pSender){
    if (this->isShowNganHang){
        this->isShowNganHang= false;
        this->scrollView->setVisible(false);
    }
    else{
        this->isShowNganHang= true;
        this->scrollView->setVisible(true);
    }
}
bool LayerPopupCapNhatSTK::utf8_check_is_valid(const string& string)
{
    int c,i,ix,n,j;
    for (i=0, ix=string.length(); i < ix; i++)
    {
        c = (unsigned char) string[i];
        //if (c==0x09 || c==0x0a || c==0x0d || (0x20 <= c && c <= 0x7e) ) n = 0; // is_printable_ascii
        if (0x00 <= c && c <= 0x7f) n=0; // 0bbbbbbb
        else if ((c & 0xE0) == 0xC0) n=1; // 110bbbbb
        else if ( c==0xed && i<(ix-1) && ((unsigned char)string[i+1] & 0xa0)==0xa0) return false; //U+d800 to U+dfff
        else if ((c & 0xF0) == 0xE0) n=2; // 1110bbbb
        else if ((c & 0xF8) == 0xF0) n=3; // 11110bbb
        //else if (($c & 0xFC) == 0xF8) n=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
        //else if (($c & 0xFE) == 0xFC) n=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
        else return false;
        for (j=0; j<n && i<ix; j++) { // n bytes matching 10bbbbbb follow ?
            if ((++i == ix) || (( (unsigned char)string[i] & 0xC0) != 0x80))
                return false;
        }
    }
    return true;
}
void LayerPopupCapNhatSTK::onBtnDoi(Ref* pSender){
    string number = this->txtTK->getString();
    string branch = this->txtChiNhanh->getString();
    string name = this->txtChuTK->getString();
    
    if (name == "")
    {
        this->txtErrorChuTK->setVisible(true);
        this->txtErrorChuTK->setString(dataManager.GetSysString(690));
        return;
        
    }else{
        this->txtErrorChuTK->setVisible(false);
    }
    
    if (name.length() < 6)
    {
        this->txtErrorChuTK->setVisible(true);
        this->txtErrorChuTK->setString(dataManager.GetSysString(691));
        return;
        
    }else{
        this->txtErrorChuTK->setVisible(false);
    }
    if (validateName(name)){
        
        this->txtErrorChuTK->setVisible(true);
        this->txtErrorChuTK->setString(dataManager.GetSysString(692));
        return;
        
    }else{
        this->txtErrorChuTK->setVisible(false);
    }
    
    if (this->isWhitespace(name)){
        this->txtErrorChuTK->setVisible(true);
        this->txtErrorChuTK->setString(dataManager.GetSysString(701));
        return;
        
    }else{
        this->txtErrorChuTK->setVisible(false);
    }
    

    if (name.length() > 30)
    {
        this->txtErrorChuTK->setVisible(true);
        this->txtErrorChuTK->setString(dataManager.GetSysString(693));
        return;
        
    }else{
        this->txtErrorChuTK->setVisible(false);
    }
    if (mUtils::isNumber(name.c_str())){
        this->txtErrorChuTK->setVisible(true);
        this->txtErrorChuTK->setString(dataManager.GetSysString(694));
        return;
        
    }else{
        this->txtErrorChuTK->setVisible(false);
    }
    
    while(!name.empty() && std::isspace(*name.begin()))
        name.erase(name.begin());
    
    while(!name.empty() && std::isspace(*name.rbegin()))
        name.erase(name.length()-1);

    
    if (this->currentClick == -1)
    {
        this->txtErrorTenNganHang->setVisible(true);
        this->txtErrorTenNganHang->setString(dataManager.GetSysString(529));
        return;
        
    }else{
        this->txtErrorTenNganHang->setVisible(false);
    }
    
    if (mUtils::findSpecialCharSpace(number))
    {
        this->txtErrorTK->setVisible(true);
        this->txtErrorTK->setString(dataManager.GetSysString(655));
        return;

    }else{
        this->txtErrorTK->setVisible(false);
    }
    if (number == "")
    {
        this->txtErrorTK->setVisible(true);
        this->txtErrorTK->setString(dataManager.GetSysString(656));
        return;
        
    }else{
        this->txtErrorTK->setVisible(false);
    }
    
    if (number.length()<9)
    {
        this->txtErrorTK->setVisible(true);
        this->txtErrorTK->setString(dataManager.GetSysString(657));
        return;
        
    }else{
        this->txtErrorTK->setVisible(false);
    }
    
    if (number.length()>30)
    {
        this->txtErrorTK->setVisible(true);
        this->txtErrorTK->setString(dataManager.GetSysString(658));
        return;
        
    }else{
        this->txtErrorTK->setVisible(false);
    }

    if (branch == "")
    {
        this->txtErrorChiNhanh->setVisible(true);
        this->txtErrorChiNhanh->setString(dataManager.GetSysString(695));
        return;
        
    }else{
        this->txtErrorChiNhanh->setVisible(false);
    }
    
    if (branch.length()<6)
    {
        this->txtErrorChiNhanh->setVisible(true);
        this->txtErrorChiNhanh->setString(dataManager.GetSysString(696));
        return;
        
    }else{
        this->txtErrorChiNhanh->setVisible(false);
    }

    if (branch.length()>30)
    {
        this->txtErrorChiNhanh->setVisible(true);
        this->txtErrorChiNhanh->setString(dataManager.GetSysString(697));
        return;
        
    }else{
        this->txtErrorChiNhanh->setVisible(false);
    }
    if (this->isWhitespace(branch)){
        this->txtErrorChiNhanh->setVisible(true);
        this->txtErrorChiNhanh->setString(dataManager.GetSysString(702));
        return;
        
    }else{
        this->txtErrorChiNhanh->setVisible(false);
    }
    
    if (dataManager.getAppConfig().isactiveOTP){
        SceneManager::getSingleton().showLoading();
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        boost::shared_ptr<IRequest> request(new ExtensionRequest("gotpcmd", params));
        GameServer::getSingleton().Send(request);

        
    }
    else{
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutInt(BANK_ID, this->lstbank[this->currentClick].id);
        params->PutUtfString(BANK_NUMBER, number);
        params->PutUtfString(BANK_NAME, name);
        params->PutUtfString(BANK_BRANCH, branch);
        boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_UPDATE_BANK_ACC_REQ,params));
        GameServer::getSingleton().Send(request);
        
        SceneManager::getSingleton().showLoading();
    }
   
}
bool LayerPopupCapNhatSTK::validateName(string src){
    if (src.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != std::string::npos)
    {
        return true;
    }
    return false;
}
bool LayerPopupCapNhatSTK::isWhitespace(std::string s){
    for(int index = 0; index < s.length(); index++){
        if(!std::isspace(s[index]))
            return false;
    }
    return true;
}
void LayerPopupCapNhatSTK::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
   // log("OnExtensionResponse %s", cmd->c_str());
    if (strcmp(EXT_EVENT_GET_SUPPORTED_BANK_RESP, cmd->c_str()) == 0){//OK
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetUtfString("rd") == NULL) return;
        string bankstr = *param->GetUtfString("rd");
        
        auto lstbank = mUtils::splitString(bankstr,';');
        this->lstbank.clear();
        this->scrollView->setInnerContainerSize(Size(692, lstbank.size() * 80));

        for (int i = 0; i < lstbank.size(); i++){
            auto item = mUtils::splitString(lstbank[i],'|');
            bank it;
            it.id = atoi(item[0].c_str());
            it.name = item[1];
            it.fullname = item[2];
            
            auto button = ui::Button::create();
            button->loadTextures("design/guifiles/moc3-btnnhamang.png", "design/guifiles/moc3-btnnhamang.png", "design/guifiles/moc3-btnnhamang.png");
            button->setAnchorPoint(Point(0, 1));
            button->setContentSize(Size(692, 80));
            button->setTitleText(item[1]);
            button->setTitleFontSize(40);
            //button->setTitleFontName("__Roboto-Bold.ttf");
            button->addClickEventListener(CC_CALLBACK_1(LayerPopupCapNhatSTK::onNganHang, this));
            //button->setTitleColor(ccc3(255, 255, 255));
            button->setTag(i);
            /*button->setPosition(Vec2(orgX + (300 + 20)*i, btnNotification->getPositionY()));*/
            button->setPosition(Vec2(0,lstbank.size() * 80 - this->lstbank.size() * 80));
            this->scrollView->addChild(button);
            this->scrollView->setVisible(false);
            this->lstbank.push_back(it);
        }
        
       
       // log("bank : %s",bankstr.c_str());
    }else if (strcmp(EXT_EVENT_UPDATE_BANK_ACC_RESP, cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        if (param->GetUtfString("rd") == NULL || param->GetInt("rc") == NULL) return;
        int rc = *param->GetInt("rc");
        if (rc == 0){
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(dataManager.GetSysString(134));
            this->addChild(layerPopup);
            
            this->txtTK->setString("");
            this->txtChiNhanh->setString("");
            this->txtChuTK->setString("");
            
            boost::shared_ptr<ISFSObject> params2(new SFSObject());
            boost::shared_ptr<IRequest> request2(new ExtensionRequest(EXT_EVENT_GET_ACC_BANK_INFOR_REQ, params2));
            GameServer::getSingleton().Send(request2);
            SceneManager::getSingleton().showLoading();
            
            this->nodeOTP->setVisible(false);
            this->nodeSTK->setVisible(true);
            
            this->txtOTP->setString("");

        }
        
        else{
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(*param->GetUtfString("rd"));
            this->addChild(layerPopup);
            
        }
    }else if (strcmp("otpresp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetInt("otperrcode") == NULL) return;
        
        int rc = *param->GetInt("otperrcode");
        if (rc == 0){
            Chat* toast = Chat::create(dataManager.GetSysString(447),-1);
            this->addChild(toast);
            this->nodeSTK->setVisible(false);
            this->nodeOTP->setVisible(true);
        }
        else{
            if (param->GetUtfString("otperrdesc") == NULL) return;
            string rd = *param->GetUtfString("otperrdesc");
            Chat* toast = Chat::create(rd,-1);
            this->addChild(toast);
        }
    }
    

}
void LayerPopupCapNhatSTK::onNganHang(Ref* pSender){
    auto node = (Node*)pSender;
    this->currentClick = node->getTag();
    this->btnChonNganHang->setTitleText(this->lstbank[this->currentClick].name);
    this->isShowNganHang= false;
    this->scrollView->setVisible(false);
}
void LayerPopupCapNhatSTK::hideLayer(){
    this->setVisible(false);
}
void LayerPopupCapNhatSTK::showLayer(){
    this->setVisible(true);

}
bool LayerPopupCapNhatSTK::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerPopupCapNhatSTK::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerPopupCapNhatSTK::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupCapNhatSTK::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupCapNhatSTK::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

