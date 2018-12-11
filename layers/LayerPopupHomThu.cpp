#include "LayerPopupHomThu.h"
#include "Requests/ExtensionRequest.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../scenes/SceneMain.h"
#include "../scenes/ScenePickRoom.h"
#include "../SceneManager.h"
#include "../layergames/PhomMessDef.h"
#include "../layergames/ChanUtils.h"
USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

LayerPopupHomThu::LayerPopupHomThu()
{
    GameServer::getSingleton().addListeners(this);
}

LayerPopupHomThu::~LayerPopupHomThu()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupHomThu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    this->setKeyboardEnabled(true);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupHomThu.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
 //   if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
 //   }
//    else{
//        sizeAdd = Size(0, 0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));
    auto pnl = dynamic_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    auto title_setting = static_cast<Text*> (pnl->getChildByName("Text_1"));
    title_setting->setString(dataManager.GetSysString(756));
    //អ្នកទើបទទួលបានអ្នកលេង tester07 ជូន 10000 Zen
   // title_setting->setString("អ្នកទើបទទួលបានអ្នកលេង");
    
    
    this->txtNoMail = static_cast<Text*> (pnl->getChildByName("txtNoMail"));
    this->txtNoMail->setString(dataManager.GetSysString(960));
    this->txtNoMail->setVisible(false);
    Button* btnClose = dynamic_cast<Button*>(pnl->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupHomThu::onBtnClose, this));
    }
    auto pnlTable=dynamic_cast<Layout*>(pnl->getChildByName("pnlTable"));
    ui::Helper::doLayout(rootNode);

    this->addChild(rootNode);
    tblInbox = TableView::create(this, Size(pnlTable->getContentSize().width, pnlTable->getContentSize().height));
    tblInbox->setDirection(TableView::Direction::VERTICAL);
    tblInbox->setAnchorPoint(Point(0, 0));
    tblInbox->setPosition(Vec2(pnlTable->getPosition().x,pnlTable->getPosition().y+sizeAdd.height));
    
    tblInbox->setDelegate(this);
    tblInbox->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    this->addChild(tblInbox);
    
    loadAllDatas();
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->hideWebView();
    }
    this->setScale(1);
    return true;
}

void LayerPopupHomThu::loadAllDatas(){
    
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    //Send request
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("aI", myself->Name());
    params->PutUtfString("startT", "24/02/2013 08:30:59");
    params->PutUtfString("endT", mUtils::getCurrentDateTime("%d/%m/%Y 23:59:59").c_str());
    params->PutInt("startR", 0);
    params->PutInt("endR", 100);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("glmi", params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}

void LayerPopupHomThu::onExit(){
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    Layer::onExit();
    SceneManager::getSingleton().numPopup-=1;

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

void LayerPopupHomThu::onEnter(){
    Layer::onEnter();
    SceneManager::getSingleton().numPopup+=1;

    currentTab = 1;
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupHomThu::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void LayerPopupHomThu::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    if (mySelf == NULL)
        return;
    
    if (strcmp("dmi", cmd->c_str()) == 0){
        // 		Chat *toast = Chat::create("Xóa thư thành công!", -1);
        // 		this->addChild(toast);
        auto layerThongbao = LayerPopupThongBao::create();
        layerThongbao->setMessage(dataManager.GetSysString(141));
        layerThongbao->showAButton();
        layerThongbao->setContentMess(dataManager.GetSysString(710));
        this->addChild(layerThongbao);
        //
        loadAllDatas();
        boost::shared_ptr<IRequest> request(new ExtensionRequest("cmi", NULL));//count msg inbox
        GameServer::getSingleton().Send(request);
    }
    
    if (strcmp("glmi", cmd->c_str()) == 0){
        //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

        if (dataManager.getAppConfig().isHideBilling){
            Chat* toast = Chat::create(dataManager.GetSysString(458), -1);
            SceneMain* _scene = (SceneMain*)this->getParent();
            _scene->addChild(toast,2000);
            SceneManager::getSingleton().hideLoading();

            return;
        }

        this->lstInboxs.clear();
        this->lstSends.clear();
        boost::shared_ptr<string> glmi = param->GetUtfString("glmi");
        if (glmi == NULL || glmi->compare("") == 0){
            lstInbox.clear();
            tblInbox->reloadData();
            SceneManager::getSingleton().hideLoading();
            this->txtNoMail->setVisible(true);
            return;
        }
        vector<string> lstRegex;
        lstRegex.push_back("$#");
        vector<std::string> lstMsgs = mUtils::splitStringByListRegex(*glmi, lstRegex);
        if (lstMsgs.size()<=0){
            this->txtNoMail->setVisible(true);
        }
        lstRegex.clear();
        lstRegex.push_back("$%");
        info infos;
        for (int i = 0; i < lstMsgs.size(); ++i){
            auto lstInfos = mUtils::splitStringByListRegex(lstMsgs[i], lstRegex);
            if (lstInfos[1] == mySelf->Name()->c_str()){
                
                if (lstInfos.size() < 5)
                    return;
                //for (int j = 0; j < lstInfos.size(); ++j){
                infos.id = atoi(lstInfos.at(0).c_str());
                infos.fromUser = lstInfos.at(1);
                infos.toUser = lstInfos.at(2);
                infos.content = lstInfos.at(3);
                infos.datetime = lstInfos.at(4);
                
                infos.isRead = true;
                //}
                lstSends.push_back(infos);
            }
            else{
                infos.id = atoi(lstInfos.at(0).c_str());
                infos.fromUser = lstInfos.at(1);
                infos.toUser = lstInfos.at(2);
                infos.content = lstInfos.at(3);
                infos.datetime = lstInfos.at(4);
                infos.isRead = (lstInfos.at(5).compare("1") == 0);
                if (lstInfos.size()>=7) {
                    if (lstInfos.at(6) != "")
                        infos.subject = lstInfos.at(6);
                    else
                        infos.subject = lstInfos.at(3);
                }
                else{
                    infos.subject = lstInfos.at(3);
                }
                lstInboxs.push_back(infos);
            }
        }
        this->tblInbox->reloadData();
        
        SceneManager::getSingleton().hideLoading();

    }
    else{
        
    }
    
}

void LayerPopupHomThu::onBtnChildBack(Ref* pSender){
    
}
void LayerPopupHomThu::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerHomThuCell::onButtonDelete(Ref* pSender){
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself != NULL) {
        SceneManager::getSingleton().showLoading();
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString("aI", myself->Name());
        params->PutInt("miid", getIDMail());
        boost::shared_ptr<IRequest> request(new ExtensionRequest("dmi", params));
        GameServer::getSingleton().Send(request);
    }
}

void LayerPopupHomThu::tableCellTouched(cocos2d::extension::TableView *table, cocos2d::extension::TableViewCell *cell){
    if (this->currentTab == 1){
        int idx = cell->getIdx();
        // mark as read
        boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
        if (myself == NULL)
            return;
        if (!lstInboxs.at(idx).isRead){
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutUtfString("aI", myself->Name()->c_str());
            params->PutInt("miid", lstInboxs.at(idx).id);
            boost::shared_ptr<IRequest> request(new ExtensionRequest("rmi", params));
            GameServer::getSingleton().Send(request);
            lstInboxs.at(idx).isRead = true;
            boost::shared_ptr<IRequest> request2(new ExtensionRequest("cmi", NULL));//count msg inbox
            GameServer::getSingleton().Send(request2);
            tblInbox->reloadData();
        }
        
        auto _layer = LayerPopupMailDetails::create();
        _layer->setDatas(lstInboxs.at(idx).id, lstInboxs.at(idx).fromUser, StringUtils::format("%s: %s",dataManager.GetSysString(635).c_str(), lstInboxs.at(idx).datetime.c_str()), lstInboxs.at(idx).content.c_str());
        auto parent = this->getParent();
        if (parent)
        {
            parent->addChild(_layer, 1000000);
        }
    }
    // 	else{
    // 		int idx = cell->getIdx();
    // 		auto _layer = LayerPopupMailDetails::create();
    // 		this->addChild(_layer);
    // 		_layer->setDatas(lstSends.at(idx).id, lstSends.at(idx).fromUser, StringUtils::format("Ngày gửi: %s ", lstSends.at(idx).datetime.c_str()), lstSends.at(idx).content.c_str());
    // 	}
    
}

Size LayerPopupHomThu::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx){
    //return Size(LayerPopupHomThu->getContentSize().width - 20, 40);
    return Size(1472, 90);
}

// Hàm này tạo 1 tableview Row để add vào table view
TableViewCell* LayerPopupHomThu::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx){
    
    if (this->currentTab == 1){
        TableViewCell *cell = table->dequeueCell();
        if (!cell){
            cell = new TableViewCell();
            cell->autorelease();
            //auto rankCell1 = LayerHomThuCell::create();
            LayerHomThuCell* rankCell = LayerHomThuCell::create();
            rankCell->setPosition(0, 0);
            rankCell->setTag(123);
            rankCell->setDatas(lstInboxs.at(idx), currentTab);
            cell->addChild(rankCell);
        }
        else{
            auto rankCell = (LayerHomThuCell*)cell->getChildByTag(123);
            if (rankCell){
                rankCell->setDatas(lstInboxs.at(idx), currentTab);
            }
        }
        cell->setIdx(idx);
        return cell;
    }
    // 	else{
    // 		TableViewCell *cell = table->dequeueCell();
    // 		if (!cell){
    // 			cell = new TableViewCell();
    // 			cell->autorelease();
    // 			//auto rankCell1 =LayerHomThuCell::create();
    // 			LayerHomThuCell* rankCell = LayerHomThuCell::create();
    // 			rankCell->setPosition(0,0);
    // 			rankCell->setTag(123);
    //             rankCell->setDatas(lstSends.at(idx),currentTab);
    // 			cell->addChild(rankCell);
    // 		}
    // 		else{
    // 			auto rankCell =(LayerHomThuCell*) cell->getChildByTag(123);
    // 			if (rankCell){
    //                 rankCell->setDatas(lstSends.at(idx),currentTab);
    // 			}
    // 		}
    // 		cell->setIdx(idx);
    // 		return cell;
    /*	}*/
    
}

// Hàm gán giá trị số hàng của table view
ssize_t LayerPopupHomThu::numberOfCellsInTableView(cocos2d::extension::TableView *table){
    if (this->currentTab == 1){
        return this->lstInboxs.size();
    }
    // 	else
    // 	{
    // 		return this->lstSends.size();
    // 	}
    
}

bool LayerPopupHomThu::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    return true;
}

LayerHomThuCell::LayerHomThuCell(){
    this->lblName = NULL;
    this->lblDate = NULL;
    this->lblMailTitle = NULL;
    this->circleAvatar = NULL;
    this->MyAI = "";
    this->MyAal = "";
    //GameServer::getSingleton().addListeners();
}

LayerHomThuCell::~LayerHomThuCell(){
    
}

bool LayerHomThuCell::init(){
    if (!Layer::init())	{
        return false;
    }
    try
    {
        boost::shared_ptr<User> user = GameServer::getSingleton().getSmartFox()->MySelf();
        if (user != NULL){
            this->MyAI = *user->Name();
            boost::shared_ptr<UserVariable> var = user->GetVariable("aal");
            if (var != NULL){
                if (var != NULL)
                    this->MyAal = *var->GetStringValue();
            }
        }
    }
    catch (...)
    {
        
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto rootNode = CSLoader::getInstance()->createNode("LayerItemMailInbox.csb");
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    
    this->lblName = static_cast<Text*>(rootNode->getChildByName("lblName"));
    this->lblDate = static_cast<Text*>(rootNode->getChildByName("lblDate"));
    this->lblMailTitle = static_cast<Text*>(rootNode->getChildByName("lblMailTitle"));
    this->imgUnread = static_cast<ImageView*>(rootNode->getChildByName("imgUnread"));
    btnDelete = dynamic_cast<Button*>(rootNode->getChildByName("btnDel"));
    if (btnDelete != NULL){
        btnDelete->addClickEventListener(CC_CALLBACK_1(LayerHomThuCell::onButtonDelete, this));
        btnDelete->setPressedActionEnabled(true);
    }
    btnDelete->setVisible(false);
    return true;
}

void LayerHomThuCell::setDatas(const LayerPopupHomThu::info& data, int currTab){
    //     if(currTab == 1)
    this->lblName->setString(dataManager.GetSysString(757));
    //     else
    //         this->lblName->setString(data.toUser);
    this->lblDate->setString(data.datetime);
    int size_s  = data.subject.size();
    std::string smallContent = data.subject;//, 80);
//         if (data.content.length() > 80){
//             smallContent = data.subject.substr(0, 30) + "...";
//         }
//         else{
//             smallContent = data.subject;
//         }
    this->lblMailTitle->setString(smallContent);
    //this->lblMailTitle->setString();
    //    if(data.fromUser == this->MyAI){
    //        if(this->circleAvatar != NULL){
    //            this->circleAvatar->setAvatarImage(this->MyAal);
    //        }
    //    }
    if (this->imgUnread)
        this->imgUnread->setVisible(!data.isRead);
}

void LayerPopupHomThu::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
        this->onBtnClose(this);
    }
}

void LayerHomThuCell::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    if (mySelf == NULL)
        return;
    
    if (strcmp("dmi", cmd->c_str()) == 0){
        // 		Chat *toast = Chat::create("Xóa thư thành công!", -1);
        // 		this->addChild(toast);
        auto layerThongbao = LayerPopupThongBao::create();
        layerThongbao->showAButton();
        layerThongbao->setContentMess(dataManager.GetSysString(710));
        this->addChild(layerThongbao);
        //
        auto _parent = static_cast<LayerPopupHomThu*>(this->getParent());
        if (_parent)
            _parent->loadAllDatas();
        //
        
    }
}
