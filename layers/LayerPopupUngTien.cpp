//
//  LayerPopupUngTien.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 12/8/16.
//
//

#include "LayerPopupUngTien.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupThongBao.h"
#include "../layergames/_Chat_.h"

LayerPopupUngTien::LayerPopupUngTien()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupUngTien::~LayerPopupUngTien()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerPopupUngTien::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupUngTien.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
    }else{
        sizeAdd = Size(0,0);
    }
    Size sizeAdd2;
    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
        sizeAdd2 = ChanUtils::getSizePos();
        
    }else{
        sizeAdd2 = Size(0,0);
    }
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupUngTien::onBtnClose, this));
    }
    
    this->pnlUngTien = static_cast<Layout*>(pnlBg->getChildByName("pnlUngTien"));
    this->pnlUngTien->setSwallowTouches(false);
    this->pnlLichSu = static_cast<Layout*>(pnlBg->getChildByName("pnlLichSu"));
    
    
    this->txtCapVip = static_cast<Text*>(pnlUngTien->getChildByName("txtCapVIP"));
    this->txtCapVip->setString("---");
    this->txtPhe = static_cast<Text*>(pnlUngTien->getChildByName("txtPhe"));
    this->txtPhe->setString("---");
    this->txtSoTienNo = static_cast<Text*>(pnlUngTien->getChildByName("txtTongNo"));
    this->txtSoTienNo->setString("---");

    
    this->txtMyMoney = static_cast<Text*>(pnlBg->getChildByName("txtMyMoney"));
    this->txtMyMoney->setString("---");
    this->txtSoLanUng = static_cast<Text*>(pnlUngTien->getChildByName("txtSoLanUng"));
    this->txtSoLanUng->setString("---");
    
    this->btnTabUngTien = static_cast<Button*>(pnlBg->getChildByName("btnTabUngTien"));
    if(this->btnTabUngTien){
        this->btnTabUngTien->addClickEventListener(CC_CALLBACK_1(LayerPopupUngTien::onBtnTabUngTien, this));
    }
    this->btnTabLichSu = static_cast<Button*>(pnlBg->getChildByName("btnTabLichSu"));
    if(this->btnTabLichSu){
        this->btnTabLichSu->addClickEventListener(CC_CALLBACK_1(LayerPopupUngTien::onBtnTabLichSu, this));
    }
    this->btnUngTien = static_cast<Button*>(pnlUngTien->getChildByName("btnUngTien"));
    if(this->btnUngTien){
        this->btnUngTien->addClickEventListener(CC_CALLBACK_1(LayerPopupUngTien::onBtnUngTien, this));
        this->btnUngTien->setEnabled(true);
        this->btnUngTien->setPressedActionEnabled(true);
    }
    this->btnHuongDan = static_cast<Button*>(pnlUngTien->getChildByName("btnHuongDan"));
    if(this->btnHuongDan){
        this->btnHuongDan->addClickEventListener(CC_CALLBACK_1(LayerPopupUngTien::onBtnHuongDan, this));
    }
    
    
    auto pnltbl = static_cast<Layout*>(pnlUngTien->getChildByName("pnlListVip"));
    
    //
    pnltbl->setVisible(false);
    this->tblListUngTien = TableView::create(this, Size(pnltbl->getContentSize().width,pnltbl->getContentSize().height));
    this->tblListUngTien->setAnchorPoint(pnltbl->getAnchorPoint());
    this->tblListUngTien->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    this->tblListUngTien->setPosition(Vec2(pnltbl->getPosition().x+sizeAdd.width,pnltbl->getPosition().y+10));
    this->tblListUngTien->setDelegate(this);
    this->tblListUngTien->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    pnlUngTien->addChild(this->tblListUngTien);
    
    
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView = cocos2d::experimental::ui::WebView::create();
    webView->setPosition(Vec2(this->pnlLichSu->getPosition().x,this->pnlLichSu->getPosition().y + sizeAdd.height));
    webView->setAnchorPoint(Vec2(0,0));
    webView->setContentSize(this->pnlLichSu->getContentSize());
    webView->setScalesPageToFit(true);
    webView->setLocalZOrder(99);
    this->addChild(webView);
    webView->setVisible(false);
    
    webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerPopupUngTien::onWebViewShouldStartLoading, this));
    webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerPopupUngTien::onWebViewDidFinishLoading, this));
    webView->setOnDidFailLoading(CC_CALLBACK_2(LayerPopupUngTien::onWebViewDidFailLoading, this));
#endif
    
    this->onBtnTabUngTien(NULL);
    
    
    //
    
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    int acctype = 0;
    double money = 0;
    if (mySelf != NULL)
    {
        boost::shared_ptr<UserVariable> acctype_ptr = mySelf->GetVariable("accVipType");
        if (acctype_ptr != NULL)
            this->currVip = *acctype_ptr->GetIntValue();
        boost::shared_ptr<UserVariable> amf = mySelf->GetVariable("amf");
        if (amf != NULL)
            money = *amf->GetDoubleValue();
        this->myMoney = money;
    }
    
    auto txtDieuKien = static_cast<Text*>(pnlUngTien->getChildByName("txtSoLanUng_0"));
    
    
    int minVip = 10 - dataManager.getAppConfig().minungtien.size()+1;
    
    if (this->currVip >= minVip){
        if (txtDieuKien)
            txtDieuKien->setString("< "+ mUtils::convertMoneyEx(dataManager.getAppConfig().minungtien[this->currVip-minVip]*1000)+ " "+SceneManager::getSingleton().getCoin(0));
    }
    else{
        if (txtDieuKien)
            txtDieuKien->setString(StringUtils::format("Vip %d trở lên!",minVip));
    }
    
    auto btnTraNo = static_cast<Text*>(pnlUngTien->getChildByName("btnTraNo"));
    if (btnTraNo){
        btnTraNo->addClickEventListener(CC_CALLBACK_1(LayerPopupUngTien::onBtnTraNo, this));
    }
    
    
    this->txtMyMoney->setString(mUtils::convertMoneyEx(money));
    this->txtCapVip->setString(StringUtils::format("VIP %d",this->currVip));
    
    this->txtMoney = static_cast<TextField*>(pnlUngTien->getChildByName("txtMoney"));
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    this->txtMoney->setInputMode(ui::EditBox::InputMode::NUMERIC);
    this->txtMoney->setMaxLength(9);
#endif
    //
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("eegigarq",params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
    
    
    return true;
}
void LayerPopupUngTien::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}
void LayerPopupUngTien::onBtnTraNo(Ref* pSender){
    
    if (this->currSotienno == 0) {
        auto toast = Chat::create("Bạn không có khoản nợ nào", -1);
        this->addChild(toast);
        return;
    }
    
    if (atoll(this->txtMoney->getString().c_str()) > this->currSotienno){
        auto toast = Chat::create("Bạn không nên nhập quá số tiền nợ !", -1);
        this->addChild(toast);
        return;
    }
    
    
    string sotientrano = this->txtMoney->getString();
    
    if (strcmp(sotientrano.c_str(), "") == 0){
        auto toast = Chat::create("Bạn chưa nhập số tiền trả nợ!", -1);
        this->addChild(toast);
        return;
    }
    
    if (mUtils::findContainChar(this->txtMoney->getString().c_str())) {
        auto toast = Chat::create("Bạn chỉ được nhập số nguyên dương!", -1);
        this->addChild(toast);
        return;
    }
    
    if (atoi(sotientrano.c_str())<1){
        auto toast = Chat::create("Mức tiền tối thiểu là 1 "+SceneManager::getSingleton().getCoin(0)+"!",-1);
        this->addChild(toast);
        return;
    }
    
    if (this->myMoney < atoi(sotientrano.c_str())) {
        auto toast = Chat::create("Bạn không đủ tiền!", -1);
        this->addChild(toast);
        return;
    }
    
    
    int tientra = atoi(sotientrano.c_str());
    
    if (tientra < 0){
        auto toast = Chat::create("Vui lòng nhập lại!", -1);
        this->addChild(toast);
        return;
    }
    
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt("pv", tientra);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("eepmrq",params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
    
}

Size LayerPopupUngTien::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(1380,166);
}

TableViewCell* LayerPopupUngTien::tableCellAtIndex(TableView *table, ssize_t idx)
{
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
    
    
    auto cols = 5;
    auto sizeTable = Size(1380,330);
    auto width = 226;
    int space = 60;
    auto startX = (sizeTable.width - (width)*cols - space * (cols - 2)) / 2 ;
    
    //comment
    for (int i = 0; i < cols; i++) {
        int index = idx *cols + i;
        if (index < this->lstu.size())
        {
            auto _child = LayerItemUngTien::create();
            _child->setAnchorPoint(Vec2(0,0));
            _child->setPosition(startX + (i * width) + space * i, 5);
            _child->setDatas(this->lstu[index].vip, this->lstu[index].money, this->lstu[index].isActive);
            cell->addChild(_child, 0, _cTag + i);
            _child->setVisible(true);
            _child->setTag(index);
        }
    }
    return cell;
    
}
int LayerPopupUngTien::getCell(){
    int i = this->lstu.size();
    if (i == 0)
        return 0;
    if (i < 5)
        return 1;
    int rows = i / 5;
    if (i % 5 != 0)
        rows += 1;
    return rows;
}
ssize_t LayerPopupUngTien::numberOfCellsInTableView(TableView *table)
{
    
    return getCell();//lstRank.size();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerPopupUngTien::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().showLoading();
    webView->setVisible(false);
    return true;
    
}

void LayerPopupUngTien::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
    if(this->currTab == 2)
        webView->setVisible(true);
}

void LayerPopupUngTien::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
}
#endif


void LayerPopupUngTien::onBtnTabUngTien(Ref* pSender){
    this->btnTabUngTien->setBright(false);
    this->btnTabUngTien->setEnabled(false);
    
    this->btnTabLichSu->setBright(true);
    this->btnTabLichSu->setEnabled(true);
    
    this->pnlUngTien->setVisible(true);
    this->pnlLichSu->setVisible(false);
    this->currTab = 1;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
    webView->setVisible(false);
#endif
    
}
void LayerPopupUngTien::onBtnTabLichSu(Ref* pSender){
    this->btnTabUngTien->setBright(true);
    this->btnTabUngTien->setEnabled(true);
    
    this->btnTabLichSu->setBright(false);
    this->btnTabLichSu->setEnabled(false);
    this->currTab = 2;
    this->pnlUngTien->setVisible(false);
    this->pnlLichSu->setVisible(true);
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
    if (myself == NULL || myself->Name() == NULL)
        return;
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
    string urlLinkHistories = StringUtils::format("%s?u=",LICHSU_UNGTIEN);
    urlLinkHistories += *myself->Name();
    urlLinkHistories += "&checksum=";
    urlLinkHistories += mUtils::getMD5KeyHis(*myself->Name());
    webView->loadURL(urlLinkHistories);
#endif
}
void LayerPopupUngTien::onBtnHuongDan(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    string strlink = "";
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
        //strlink = QUYDINH_DOITHUONG_P2;
    }
    else{
        strlink = HUONGDAN_UNGTIEN;
    }
    //#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    strlink += "i";
    //#endif
    //#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    strlink += "an";
    //#endif
    LayerPopupWebView* _layer = LayerPopupWebView::create();
    _layer->setWebView(strlink);
    _currScene->addChild(_layer, 100000,9064);
}
void LayerPopupUngTien::onBtnUngTien(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
    layerPopup->setCallBackFromButton(this, callfuncO_selector(LayerPopupUngTien::popUpCallBack),5);
    layerPopup->setMessage(dataManager.GetSysString(141));
    layerPopup->setContentMess(StringUtils::format("Bạn có chắc chắn muốn ứng %s",mUtils::convertMoneyEx(this->numAdvace).c_str()));
    layerPopup->setTag(24);
    _currScene->addChild(layerPopup,1000000,9065);
}
void LayerPopupUngTien::popUpCallBack(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (_currScene)
        _currScene->removeChildByTag(24);
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("eegarq",params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}
void LayerPopupUngTien::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerPopupUngTien::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp("eegigarsp", cmd->c_str()) == 0){
        
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetInt("noga") == NULL) return;
        int numAdvace = *param->GetInt("noga");
        this->txtSoLanUng->setString(StringUtils::format("%d",numAdvace));
        
        if (param->GetInt("voga") == NULL) return;
        this->numAdvace = *param->GetInt("voga");
        
        if (param->GetInt("voo") == NULL) return;
        int numOwe = *param->GetInt("voo");
        this->currSotienno = numOwe;
        this->txtSoTienNo->setString(mUtils::convertMoneyEx(numOwe));
        
        if (param->GetInt("fee") == NULL) return;
        int numFee= *param->GetInt("fee");
        this->txtPhe->setString(StringUtils::format("%d",numFee));
        
        if (param->GetInt("isga") == NULL) return;
        int isga = *param->GetInt("isga");
        if (isga == 1){
            this->btnUngTien->setEnabled(true);
            this->btnUngTien->setBright(true);
        }else{
            this->btnUngTien->setEnabled(false);
            this->btnUngTien->setBright(false);
        }
        
        if (param->GetUtfString("lstu") == NULL) return;
        string str = *param->GetUtfString("lstu");
        
        vector<string> lststr = mUtils::splitString(str, ',');
        this->lstu.clear();
        for (int i = 0; i< lststr.size();i++){
            vector<string> lstitem = mUtils::splitString(lststr[i], ':');
            if (lstitem.size()<2)
                return;
            itemu it;
            it.vip = atoi(lstitem[0].c_str());
            
            if(this->currVip == atoi(lstitem[0].c_str()))
                it.isActive = 1;
            else
                it.isActive = 0;
            it.money = atoi(lstitem[1].c_str());
            
            this->lstu.push_back(it);
        }
        
        for (int i = 0; i < this->lstu.size(); i++){
            if (this->currVip == this->lstu[i].vip){
                itemu it = this->lstu[i];
                vector<itemu>::iterator itr = this->lstu.begin() + i;
                this->lstu.erase(itr);
                this->lstu.insert(this->lstu.begin(), it);
                break;
            }
        }
        this->tblListUngTien->reloadData();
    }
    else if (strcmp("eegarsp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetInt("rc") == NULL) return;
        int rc = *param->GetInt("rc");
        
        if (rc == 0){
            if (param->GetInt("rv") == NULL) return;
            int rv = *param->GetInt("rv");
            
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(StringUtils::format("Bạn đã ứng thành công %d",rv));
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,1000000,9066);
            
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
            _currScene->addChild(layerPopup,1000000,9067);
        }
        
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        boost::shared_ptr<IRequest> request(new ExtensionRequest("eegigarq",params));
        GameServer::getSingleton().Send(request);
        
        SceneManager::getSingleton().showLoading();
        
    }
    else if (strcmp("eepmrsp", cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        
        if (param->GetInt("rc") == NULL) return;
        int rc = *param->GetInt("rc");
        
        if (rc == 0){
            
            if (param->GetInt("ov") == NULL) return;
            int ov = *param->GetInt("ov");
            
            if (param->GetInt("rv") == NULL) return;
            int rv = *param->GetInt("rv");
            
            
            
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(StringUtils::format("Bạn đã trả nợ với số tiền %s %s, số tiền nợ còn lại là %s %s",mUtils::convertMoneyEx(rv).c_str(),SceneManager::getSingleton().getCoin(0).c_str(),mUtils::convertMoneyEx(ov).c_str(),SceneManager::getSingleton().getCoin(0).c_str()));
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,1000000,9068);
            
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            boost::shared_ptr<IRequest> request(new ExtensionRequest("eegigarq",params));
            GameServer::getSingleton().Send(request);
            
            SceneManager::getSingleton().showLoading();
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
            _currScene->addChild(layerPopup,1000000,9069);
        }
        
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        boost::shared_ptr<IRequest> request(new ExtensionRequest("eegigarq",params));
        GameServer::getSingleton().Send(request);
        
        SceneManager::getSingleton().showLoading();
        
    }
    
}
void LayerPopupUngTien::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    
    if (ptrNotifiedUser->IsItMe() == true){
        boost::shared_ptr<double> amf;
        boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
        if (myself == NULL)
            return;
        if (myself->GetVariable("amf") == NULL)
            return;
        amf = myself->GetVariable("amf")->GetDoubleValue();
        if (amf == NULL) return;
        txtMyMoney->setString(mUtils::convertMoneyEx(*amf).c_str());
        this->myMoney = *amf;
    }
}
bool LayerPopupUngTien::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupUngTien::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupUngTien::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupUngTien::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool LayerItemUngTien::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemUngTien.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    
    this->txtCapVip = static_cast<Text*>(root->getChildByName("txtVIP"));
    this->txtMoney = static_cast<Text*>(root->getChildByName("txtMoney"));
    this->imgBg = static_cast<ImageView*>(root->getChildByName("imgActive"));
    
    return true;
}
void LayerItemUngTien::setDatas(int vip, int money , int isActive){
    this->txtCapVip->setString(StringUtils::format("VIP %d",vip));
    this->txtMoney->setString(mUtils::convertMoneyEx(money));
    
    if (isActive == 1){
        this->imgBg->loadTexture("bg-slot-ungtien.png");
    }
}

