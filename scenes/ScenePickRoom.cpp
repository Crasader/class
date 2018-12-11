#include "ScenePickRoom.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layers/LayerCreateRoom.h"
#include "../AllData.h"
#include "../data_manager.h"
#include "../mUtils.h"
#include "../layers/LayerPopupInviteGame.h"

#include "../layergames/_Chat_.h"

#include "Requests/JoinRoomRequest.h"
#include "Requests/SetUserVariablesRequest.h"
#include "Entities/Variables/UserVariable.h"
#include "Entities/Variables/SFSUserVariable.h"
#include "Entities/Invitation/SFSInvitation.h"
#include "Entities/Invitation/Invitation.h"
#include "Requests/SubscribeRoomGroupRequest.h"
#include "Requests/UnsubscribeRoomGroupRequest.h"
#include "Requests/ExtensionRequest.h"

#include <string>
#include "layers/LayerPopupRecharge.h"
#include "layers/LayerPopupProfile.h"
#include "layers/LayerPopupSearchRoom.h"
#include "layergames/PhomMessDef.h"
#include "layers/LayerChatInScenePickRoom.h"
#include "layers/LayerPopupRankGame.h"
#include "layergames/ChanUtils.h"
#include "Requests/LeaveRoomRequest.h"
#include "layers/LayerRecharge.h"
#include "../layergames/LayerGameTaiXiuMini.h"
#include "../Common.h"
#include "../layergames/LayerMenuMiniGame.h"
#include "../layergames/XiToPopupChonTien.h"
#include "../layers/LayerPopupChonTienLieng.h"
#include "../layers/LayerPopupHetTien.h"
#include "../layers/LayerHuThuong.h"
#include "../layers/LayerMainTop.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
#include "cotuong/ServerMessageDef.h"
#include "cotuong/gameUtils.h"
#include "../layergames/LayerPopupTaoBan.h"
#include "../layergames/LayerPopupTimBan.h"
#include "../layers/LayerPopupCuaHang.h"
#include "../layers/LayerPopupDoiMoc.h"
#include "SceneMain.h"
#include "../layers/LayerGuideInGame.h"
USING_NS_CC;
using namespace std;

#define TIME_AUTO_RELOAD 60//1 phut
#define  tag_layer_create_room 245
#define  tag_layer_find_room 246
#define tag_layer_avatar 247
#define tag_layer_nap_tien  52345
#define zorder_layer_nap_tien 5000
#define tag_layer_invite  249
#define tag_layer_toast 250
#define tag_layer_password 251
#define TIME_AUTO_INVITE 3
#define tag_ScenePickRoom 8866
LayerPlayRoom::LayerPlayRoom()
{
    
}
LayerPlayRoom::~LayerPlayRoom()
{
}

LayerPlayRoom* LayerPlayRoom::create(int gameID)
{
    LayerPlayRoom* banchoi = new LayerPlayRoom();
    
    if (banchoi && banchoi->init(gameID))  {
        banchoi->autorelease();
        return banchoi;
    }
    else{
        CC_SAFE_DELETE(banchoi);
        return NULL;
    }
}

static inline void printAllChildrenTypes(Node* node, int level)
{
    std::string spaces(level, ' ');
   // log("%s %02d : %s - name: %s", spaces.c_str(), level, typeid(*node).name(), node->getName().c_str());
    
    ++level;
    int index = 0;
    
    while (index < node->getChildren().size()){
        Node* obj = node->getChildren().at(index);
        printAllChildrenTypes(static_cast<Node*>(obj), level);
        index++;
    }
}

bool LayerPlayRoom::init(int gameID)
{
    if (!Layer::init()) {
        return false;
    }
    setGameID(gameID);
    setBetValue(0);
    typePr = SceneManager::getSingleton().getTypePickRoom();
    if (typePr == TableType::PlayTable)
    {
        //		switch (gameID)
        //		{
        //		case kGameSamSolo:
        //		case kGameDemLaSolo:
        //			rootNode = CSLoader::getInstance()->createNode("LayerItemTablePickRoom2Slot.csb");
        //			break;
        //		case kGamePhom:
        //		case kGameSam:
        //		case kGameTLMNDemLa:
        //		case kGameTienLenMienBac:
        //		case kGameTienLenMienNam:
        //            rootNode = CSLoader::getInstance()->createNode("LayerItemTablePickRoom.csb");
        //                break;
        //		case kGameChan:
        //			rootNode = CSLoader::getInstance()->createNode("LayerItemTablePickRoomChan.csb");
        //			break;
        //		case kGameLieng:
        //		case kGameBaCayChuong:
        //		case kGameBaCay:
        //			rootNode = CSLoader::getInstance()->createNode("LayerItemTablePickRoom7Slot.csb");
        //			break;
        //		case kGameXiTo:
        //			rootNode = CSLoader::getInstance()->createNode("LayerItemTablePickRoom5Slot.csb");
        //			break;
        //		default:
        //			rootNode = CSLoader::getInstance()->createNode("LayerItemTablePickRoom.csb");
        //			break;
        //		}
        rootNode = CSLoader::getInstance()->createNode("LayerItemTablePickRoom.csb");
        
        ui::Helper::doLayout(rootNode);
        this->addChild(rootNode);
        
        txtBetValue = (Text*)rootNode->getChildByName("txtBetValue");
        imgTable = (Button*)rootNode->getChildByName("imgTable");
        imgCoin = (ImageView*)rootNode->getChildByName("imgCoin");
        int roomType = SceneManager::getSingleton().getCurrRoomType();
        
        if (roomType == 0){
            imgCoin->loadTexture("moc3-icon-moc.png");
            imgTable->loadTextures("moc3-banvip.png", "moc3-banvip.png", "moc3-banvip.png");
        }
        else{
            imgCoin->loadTexture("moc3-icon-hao.png");
            imgTable->loadTextures("moc3-banthuong.png", "moc3-banthuong.png", "moc3-banthuong.png");
            
        }
        if (imgTable)
        {
            imgTable->setSwallowTouches(false);
            imgTable->addTouchEventListener(CC_CALLBACK_2(LayerPlayRoom::onButtonRoom, this));
        }
    }
    else if (typePr == TableType::BetTable)
    {
        btnCoin = Button::create();
        btnCoin->loadTextures("choose-item.png", "choose-item.png", "");
        btnCoin->setPosition(Vec2(0, 0));
        btnCoin->setSwallowTouches(false);
        btnCoin->setPressedActionEnabled(true);
        btnCoin->addTouchEventListener(CC_CALLBACK_2(LayerPlayRoom::onButtonRoom, this));
        
        coin = Sprite::create("choose-item-coin-1.png");
        coin->setAnchorPoint(Vec2(0.5, 0));
        coin->setPosition(Vec2(155, 115));
        btnCoin->addChild(coin);
        
        labelmoney = Label::createWithTTF("", kNormalFont, FONT_SIZE_NORMAL);
        labelmoney->setColor(Color3B(255, 227, 160));
        labelmoney->setAnchorPoint(Vec2(0.5, 0.5));
        labelmoney->setPosition(155, 75);
        btnCoin->addChild(labelmoney);
        
        
        this->addChild(btnCoin);
    }
    
    return true;
}

void LayerPlayRoom::setDataRoom(int betvl, int usercount)
{
    
    this->setOpacity(255);
    
    int roomType = SceneManager::getSingleton().getCurrRoomType();
    
    if (roomType == 0){
        imgCoin->loadTexture("moc3-icon-moc.png");
        imgTable->loadTextures("moc3-banvip.png", "moc3-banvip.png", "moc3-banvip.png");
        
    }
    else{
        imgCoin->loadTexture("moc3-icon-hao.png");
        imgTable->loadTextures("moc3-banthuong.png", "moc3-banthuong.png", "moc3-banthuong.png");
        
        
    }
    
    userCount = usercount;
    imgTable->setOpacity(255);
    setBetValue(betvl);
    if (txtBetValue)
        txtBetValue->setString(formatMoneySymbol(betvl));
    int maxUser = getUserCountByGame(nGameID);
    
    int maxCount = getUserCountByGame(nGameID);
    if (typePr == TableType::PlayTable)
    {
        if (userCount >= maxCount)
        {
            imgTable->setOpacity(130);
            //            auto _currScene = Director::getInstance()->getRunningScene();
            //            if (!_currScene) return;
            //            ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(tag_ScenePickRoom);
            //            if (_scene)
            //            {
            //                _scene->showToast(dataManager.GetSysString(259));
            //                return;
            //            }
        }
    }
    
    //reset lai trang thai
    //    string filefull = "slot-full.png";
    //    string fileempty = "slot-empty.png";
    //    if (SceneManager::getSingleton().getGameID() == kGameChan){
    //        filefull = "ChanNew/table2.png";
    //        fileempty = "ChanNew/table3.png";
    //    }
    //	for (int i = 0; i < maxUser; i++)
    //	{
    //		ImageView* imgSlot = (ImageView*)rootNode->getChildByName(StringUtils::format("imgSlot%d", i + 1));
    //		if (imgSlot)
    //		{
    //			imgSlot->loadTexture(fileempty);
    //		}
    //	}
    //	for (int i = 0; i < usercount; i++)
    //	{
    //		ImageView* imgSlot = (ImageView*)rootNode->getChildByName(StringUtils::format("imgSlot%d", i + 1));
    //		if (imgSlot)
    //		{
    //			imgSlot->loadTexture(filefull);
    //		}
    //	}
}

void LayerPlayRoom::onButtonRoom(Ref* pSender, ui::Widget::TouchEventType type)
{
    //return;
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        double amf = 0;
        
        int roomType = SceneManager::getSingleton().getCurrRoomType();
        if (roomType == 0)
            amf = GameUtils::getAmfGoldUser(GameUtils::getMyName());
        else if (roomType == 1)
            amf = GameUtils::getAmfSilverUser(GameUtils::getMyName());
        
        double betUser = this->getMinBetByGame(getGameID(), amf);
        if (betUser < nBetValue) //khong du tien
        {
            
            if (SceneManager::getSingleton().getCurrRoomType() == 1){
                auto _currScene = Director::getInstance()->getRunningScene();
                if (!_currScene) return;
                LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
                popup->onBtnNapHao(NULL);
                _currScene->addChild(popup,2000,9137);
            }
            else{
                auto _currScene = Director::getInstance()->getRunningScene();
                if (!_currScene) return;
                SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(965));
            }
            
            //			if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
            //			{
            //				auto _currScene = Director::getInstance()->getRunningScene();
            //				if (!_currScene) return;
            //				ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(tag_ScenePickRoom);
            //				LayerPopupHetTien* popup = LayerPopupHetTien::create();
            //				_scene->addChild(popup, ZORDER_POPUP_CHONTIEN);
            //
            //			}
            //			else{
            //				double ratioMoney = getRatioMinBetByGame(getGameID(), nBetValue - betUser);
            //				string strMoney = mUtils::format_money(ratioMoney)->c_str();
            //				auto _currScene = Director::getInstance()->getRunningScene();
            //				if (!_currScene) return;
            //				ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(tag_ScenePickRoom);
            //				_scene->onButtonCharge(NULL, ui::Widget::TouchEventType::ENDED);
            //				string mess = StringUtils::format("Tài khoản của bạn không đủ.\n Vui lòng nạp thêm %s %s để chơi tiếp !",
            //					strMoney.c_str(), SceneManager::getSingleton().getCoinUnit().c_str());
            //				_scene->showToast(mess);
            //			}
            return;
        }
        
        int maxCount = getUserCountByGame(nGameID);
        if (typePr == TableType::PlayTable)
        {
            if (userCount >= maxCount)
            {
                auto _currScene = Director::getInstance()->getRunningScene();
                if (!_currScene) return;
                ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(tag_ScenePickRoom);
                if (_scene)
                {
                    _scene->showToast(dataManager.GetSysString(400));
                    return;
                }
            }
            else{
                int _gameID = SceneManager::getSingleton().getGameID();
                if (_gameID == kGameXiTo || _gameID == kGamePoker){
                    vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
                    int bet_ratio = 1;
                    for (int i = 0; i < arrBet.size(); ++i)
                    {
                        if (arrBet.at(i).gameid == _gameID)
                        {
                            bet_ratio = arrBet.at(i).ratioBet;
                            break;
                        }
                    }
                    
                    XiToPopupChonTien* chontien = XiToPopupChonTien::create();
                    chontien->setMinMaxValue(nBetValue, bet_ratio*nBetValue);
                    auto _currScene = Director::getInstance()->getRunningScene();
                    if (!_currScene) return;
                    _currScene->addChild(chontien, ZORDER_POPUP_RANK,9138);
                    
                }
                else if (_gameID == kGameLieng){
                    vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
                    int bet_ratio = 1;
                    for (int i = 0; i < arrBet.size(); ++i)
                    {
                        if (arrBet.at(i).gameid == _gameID)
                        {
                            bet_ratio = arrBet.at(i).ratioBet;
                            break;
                        }
                    }
                    
                    LayerPopupChonTienLieng* chontien = LayerPopupChonTienLieng::create();
                    chontien->setMinMaxValue(nBetValue, bet_ratio*nBetValue);
                    auto _currScene = Director::getInstance()->getRunningScene();
                    if (!_currScene) return;
                    _currScene->addChild(chontien, ZORDER_POPUP_RANK,9139);
                }
                else{
                    //SceneManager::getSingleton().showLoading(30);
                    int roomType = SceneManager::getSingleton().getCurrRoomType();
                    boost::shared_ptr<ISFSObject> param(new SFSObject());
                    param->PutInt(EXT_FIELD_ROOM_TYPE, roomType);
                    param->PutInt(AMOUNT_BET, nBetValue);
                    param->PutUtfString(GAME_ID, StringUtils::format("%d", _gameID));
                    boost::shared_ptr<IRequest> request(new ExtensionRequest(REQUEST_AUTO_JOIN_ROOM, param));
                    GameServer::getSingleton().Send(request);
                    SceneMain::getSingleton().prepareToPickGame();

                }
            }
        }
        else{
            int _gameID = SceneManager::getSingleton().getGameID();
            if (_gameID == kGameXiTo || _gameID == kGamePoker){
                XiToPopupChonTien* chontien = XiToPopupChonTien::create();
                
                auto _currScene = Director::getInstance()->getRunningScene();
                if (!_currScene) return;
                _currScene->addChild(chontien, ZORDER_POPUP_CHONTIEN,9140);
                
            }
            else if (_gameID == kGameLieng){
                LayerPopupChonTienLieng* chontien = LayerPopupChonTienLieng::create();
                
                auto _currScene = Director::getInstance()->getRunningScene();
                if (!_currScene) return;
                _currScene->addChild(chontien, ZORDER_POPUP_CHONTIEN,9141);
            }
            else{
                int roomType = SceneManager::getSingleton().getCurrRoomType();

                boost::shared_ptr<ISFSObject> param(new SFSObject());
                param->PutInt(AMOUNT_BET, nBetValue);
                param->PutInt(EXT_FIELD_ROOM_TYPE, roomType);
                param->PutUtfString(GAME_ID, StringUtils::format("%d", _gameID));
                boost::shared_ptr<IRequest> request(new ExtensionRequest(REQUEST_AUTO_JOIN_ROOM, param));
                GameServer::getSingleton().Send(request);
                SceneMain::getSingleton().prepareToPickGame();

            }
        }
    }
}

int LayerPlayRoom::getUserCountByGame(int gameID)
{
    switch (gameID)
    {
        case kGamePhom:
        case kGameTienLenMienBac:
        case kGameTienLenMienNam:
        case kGameChan:
        case kGameSam:
        case kGameTLMNDemLa:
            return 4;
        case kGameSamSolo:
        case kGameDemLaSolo:
            return 2;
        case kGameLieng:
        case kGameBaCay:
        case kGameBaCayChuong:
            return 7;
        case kGameXiTo:
            return 5;
        default:
            break;
    }
}
double LayerPlayRoom::getRatioMinBetByGame(int gameID, double amf)
{
    double betUser = 0;
    vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
    int bet_ratio = 1;
    for (int i = 0; i < arrBet.size(); ++i)
    {
        if (arrBet.at(i).gameid == gameID)
        {
            bet_ratio = arrBet.at(i).ratioBet;
            break;
        }
    }
    betUser = amf * bet_ratio;
    return betUser;
}

double LayerPlayRoom::getMinBetByGame(int gameID, double amf)
{
    double betUser = 0;
    vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
    int bet_ratio = 1;
    for (int i = 0; i < arrBet.size(); ++i)
    {
        if (arrBet.at(i).gameid == gameID)
        {
            bet_ratio = arrBet.at(i).ratioBet;
            break;
        }
    }
    betUser = amf / bet_ratio;
    return betUser;
}

void LayerPlayRoom::setDataBet(int _money, int _type, int _state)
{
    bool _status = _state;
    btnCoin->setEnabled(_status);
    setBetValue(_money);
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
    {
        int _opacity = 255;
        if (!_status)
        {
            _opacity = 50;
            btnCoin->loadTextures("choose-item-disable.png", "choose-item-disable.png", "");
        }
        else
        {
            btnCoin->loadTextures("choose-item.png", "choose-item.png", "");
        }
        string imgpath;
        switch (_type)
        {
            case 0:
                imgpath += "choose-item-coin-1.png";
                break;
            case 1:
                imgpath += "choose-item-coin-2.png";
                break;
            case 2:
                imgpath += "choose-item-coin-3.png";
                break;
            case 3:
                imgpath += "choose-item-coin-4.png";
                break;
            case 4:
                imgpath += "choose-item-coin-5.png";
                break;
            default:
                imgpath += "choose-item-coin-1.png";
                break;
        }
        coin->setTexture(imgpath);
        labelmoney->setOpacity(_opacity);
        coin->setOpacity(_opacity);
    }
    else{
        btnCoin->loadTextures("table.png", "table.png", "");
        labelmoney->setPosition(btnCoin->getContentSize().width / 2 - 10, 60);
        labelmoney->setHorizontalAlignment(TextHAlignment::RIGHT);
        coin->setPosition(btnCoin->getContentSize().width - 50, 45);
        coin->setTexture("icon-vang.png");
    }
    moneybet = _money;
    labelmoney->setString(formatMoneySymbol(_money));
}

/************************************************************************/
/* Scene pick room                                                                     */
/************************************************************************/
ScenePickRoom::ScenePickRoom()
{
    mScrollViewTableList = NULL;
    m_lblGameName = NULL;
    btnDaiGia = NULL;
    btnTrieuPhu = NULL;
    btnBinhDan = NULL;
    mRoomID4Invite = 0;
    mCurrentRoomIndex = 0;
    mCurrentRoomIDJoin = 0;
    passRoom4Invite = "";
    listCuoc.clear();
    isRejoinRoom = false;
    isDenyAllInvite = false;
    
    lblMoney1 = NULL;
    lblMoney2 = NULL;
    lblMoney3 = NULL;
    lstFilterMoney.clear();
    currFilterMoneyIndex = -1;
}
ScenePickRoom::~ScenePickRoom()
{
    GameServer::getSingleton().removeListeners(this);
    
   // log("ScenePickRoom::~ScenePickRoom()");
    try{
        lstFilterMoney.clear();
        unSubscribeGame();
        SceneManager::getSingleton().setKickRoom(false);
        SceneManager::getSingleton().setKickNem(false);
        this->unschedule(schedule_selector(ScenePickRoom::timeoutChatMaxtime));
        
    }
    catch (std::exception ex) {
       // log("ScenePickRoom- throw exception: %s", ex.what());
    }
}

cocos2d::Scene* ScenePickRoom::createScene(int gameID)
{
    auto scene = Scene::create();
    
    auto layer = ScenePickRoom::create();
    if (layer == NULL)
        return NULL;
    layer->setGameID(gameID);
    // 	string _currGameID = StringUtils::format("%d", gameID);
    // 	if (_currGameID.length() == 3)
    // 		_currGameID = StringUtils::format("%d%d", gameID, 1);
    // 	layer->setCurrGroupList(_currGameID);
    // 	layer->subscribeGame(layer->getCurrGroupList());
    //layer->loadAllDatas();
    layer->setTag(tag_ScenePickRoom);
    scene->addChild(layer);
    
    return scene;
}

bool ScenePickRoom::init()
{
    if (!Layer::init())
        return false;
    SceneManager::getSingleton().setAutoCreateRoom(false);
    this->setKeyboardEnabled(false);
    setGameID(SceneManager::getSingleton().getGameID());
    //for bet table version
    typePR = SceneManager::getSingleton().getTypePickRoom();
    //get new notification
    //getNotificationFromUrl();
    UserDefault *def = UserDefault::getInstance();
    def->setStringForKey("lastgameid", "");
    currFilterBet = 0;
    int gameId = getGameID();
    ///set config by game version
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
    {
        isWebView = false;
    }
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::logScreenToGA("Phòng chờ: " + *mUtils::getGameNameByID(gameId));
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string screen = "Chờ: " + *mUtils::getGameNameByID(gameId);
    NativeHelper::trackScreen(screen.c_str());
#endif
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
//    Sprite* sprite = Sprite::create("bg_sanh_cho_game.jpg");
//    sprite->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
//    sprite->setAnchorPoint(Vec2(0, 0));
//    sprite->setPosition(Vec2(0, 0));
//    this->addChild(sprite);
    
    string filename = "LayerChonBanChoiNew.csb";
    /* if (SceneManager::getSingleton().getGameID() == kGameChan){
     filename = "LayerChonBanChoi_chan.csb";
     sprite->setTexture("ChanNew/bg.jpg");
     }*/
    
    auto rootNode = CSLoader::getInstance()->createNode(filename);
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
    //if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
    sizeAdd = ChanUtils::getSizePos();
    
    //}
    //    else{
    //        sizeAdd = Size(0, 0);
    //    }
    Size sizeAdd2;
    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
        sizeAdd2 = Size(0, 0);
        
    }
    else{
        sizeAdd2 = Size(0, 0);
    }
    rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd2.height));
    
    
    SceneManager::getSingleton().setCurrScene(1);
    
    //get Node from cocos studio
    //auto nodeMenuTop = dynamic_cast<Node*>(rootNode->getChildByName("nodeMenuTop"));
    auto pnlBg = (Button*)rootNode->getChildByName("pnlBg");
    btnMoneyGold = (Button*)pnlBg->getChildByName("btnBanVip");
    btnMoneySilver = (Button*)pnlBg->getChildByName("btnBanThuong");
//    this->btnMoneyGold->setTitleText(dataManager.GetSysString(860));
//    this->btnMoneySilver->setTitleText(dataManager.GetSysString(861));
    
    btnMoneyGold->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonTypeRoom, this));
    btnMoneySilver->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonTypeRoom, this));
    // 	auto layerTopMenuOnlyBack = static_cast<Node*>(nodeMenuTop->getChildByName("layerTopMenuOnlyBack"));
    // 	layerTopMenuOnlyBack->setContentSize(layerTopMenuOnlyBack->getContentSize());
    
    auto tg = BORDER_TOP_MENU * (visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    mScrollViewTableList = dynamic_cast<ui::Layout*>(pnlBg->getChildByName("scrollViewTableList"));
//    layerScenePickRoomBottom = LayerMainBottom::create();
//    layerScenePickRoomBottom->currScene = 2;
//    this->addChild(layerScenePickRoomBottom);
    
    
    auto btnTaoBan = (Button*)pnlBg->getChildByName("btnTaoBan");
    if (btnTaoBan){
        btnTaoBan->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonTaoBan, this));
        //		btnTaoBan->setEnabled(false);
        //		btnTaoBan->setBright(false);
        btnTaoBan->setTitleText(dataManager.GetSysString(862));
        
    }
    
    auto btnTimBan = (Button*)pnlBg->getChildByName("btnTimBan");
    if (btnTimBan){
        btnTimBan->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonTimBan, this));
        btnTimBan->setTitleText(dataManager.GetSysString(863));
        
    }
    
    
    
    if (SceneManager::getSingleton().getCapVip() >= dataManager.getAppConfig().mintaoban){
        btnTaoBan->setEnabled(true);
        btnTaoBan->setBright(true);
    }
    
    Layout* pnlRight = static_cast<Layout*>(pnlBg->getChildByName("pnlChat"));
    pnlRight->setVisible(false);
    auto btnChoiNgay = static_cast<Button*>(pnlBg->getChildByName("btnChoiNgay"));
    if (btnChoiNgay){
        btnChoiNgay->addTouchEventListener(CC_CALLBACK_2(ScenePickRoom::onButtonPlayNow, this));
        btnChoiNgay->setTitleText(dataManager.GetSysString(864));
    }
    
    
    //    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
    //        sizeAdd2 = ChanUtils::getSizePos();
    //
    //    }
    //    else{
    //        sizeAdd2 = Size(0, 0);
    //    }
    
    //if (mUtils::isSoundOn()){
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    //}
    
    if (mUtils::isSoundOn()){
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bm-theme_v3.mp3", true);
    }
    //	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    LayerHuThuong* huthuong = LayerHuThuong::create();
    huthuong->setTag(19802);
    this->addChild(huthuong, 100000);
    //	}
    //
    //	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //		LayerMenuMiniGame* minigame = LayerMenuMiniGame::create();
    //		minigame->setPosition(Vec2(sizeAdd.width, sizeAdd.height));
    //		this->addChild(minigame, ZORDER_LIST::ZORDER_TAIXIU_MINI);
    //
    //	}
    //	else{
    //		LayerMenuMiniGame* minigame = LayerMenuMiniGame::create();
    //		minigame->setPosition(Vec2(sizeAdd2.width, sizeAdd2.height));
    //		this->addChild(minigame, ZORDER_LIST::ZORDER_TAIXIU_MINI);
    //
    //
    //	}
    //Size sizeAdd = ChanUtils::getSizePos();
    
    if (!SceneManager::getSingleton().isNagaWin){
            LayerChatInScenePickRoom* chatLayer = LayerChatInScenePickRoom::create();
            chatLayer->setPosition(Vec2(pnlRight->getPositionX(), pnlRight->getPositionY() + sizeAdd.height));
            //chatLayer->setLocalZOrder(5);
            this->addChild(chatLayer);
    }

    //	chatLayer->createPopupChat();
    //
    //	Button* btnPlayNow = chatLayer->getButtonPlayNow();
    //	btnPlayNow->addTouchEventListener(CC_CALLBACK_2(ScenePickRoom::onButtonPlayNow, this));
    //	if (gameId == kGameXiTo || gameId == kGameLieng)
    //		btnPlayNow->setVisible(false);
    // 	btnMail = dynamic_cast<Button*>(nodeMenuTop->getChildByName("btnMail"));
    // 	if (btnMail != NULL){
    // 		btnMail->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::gotoMail, this));
    // 		btnMail->setPressedActionEnabled(true);
    // 		bagItem = BagItem::create();
    // 		bagItem->setPosition(Vec2(btnMail->getContentSize().width / 4, btnMail->getContentSize().height));
    // 		btnMail->addChild(bagItem);
    // 		bagItem->setVisible(false);
    // 	}
    //1416 - 1010
    //Create tableview
    int BOTTOM_SHADOW = 50;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
        BOTTOM_SHADOW = 10;
    
    // 	float _width = mScrollViewTableList->getContentSize().width + 50;
    // 	float _height = 800 + sizeAdd.height; //fix cung do size mScrollViewTableList sai so voi ban web
    // 	if (typePR == TableType::BetTable)
    // 		_height = 1010;
    tblPlayRooms = TableView::create(this, Size(mScrollViewTableList->getContentSize().width,
                                                mScrollViewTableList->getContentSize().height));
    tblPlayRooms->setAnchorPoint(Vec2::ZERO);
    tblPlayRooms->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    tblPlayRooms->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tblPlayRooms->setBounceable(true);
    tblPlayRooms->setDelegate(this);
   // mScrollViewTableList->addChild(tblPlayRooms);
    //layerScenePickRoomBottom->setLocalZOrder(1000);
    mScrollViewTableList->setSwallowTouches(false);
    
    this->scrollView = ui::ScrollView::create();
    this->scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollView->setClippingEnabled(true);
    this->scrollView->setContentSize(this->mScrollViewTableList->getContentSize());
    this->scrollView->setPosition(this->mScrollViewTableList->getPosition());
    this->scrollView->setAnchorPoint(Vec2::ZERO);
    this->scrollView->setScrollBarEnabled(true);
    this->scrollView->setBounceEnabled(true);
    this->scrollView->setTag(12932);
    this->scrollView->setInnerContainerSize(Size(692, this->mScrollViewTableList->getContentSize().height));
    this->addChild(this->scrollView);
    
    
    if (SceneManager::getSingleton().isNagaWin){
        auto pnlEvent = static_cast<Layout*>(pnlBg->getChildByName("pnlEvent"));
        pnlEvent->setVisible(false);
        this->sprStick = static_cast<Sprite*>(pnlEvent->getChildByName("sprStick"));
        auto txtEvent = static_cast<Text*>(pnlEvent->getChildByName("txtEvent"));
        auto rotation = RotateBy::create(0.5, 360, 360);
        rotation->setTag(123);
        this->sprStick->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),ScaleTo::create(0.15,1.02),CallFunc::create([this](){this->sprStick->setTexture("stiker-do.png");}),DelayTime::create(0.15),rotation,DelayTime::create(0.5),CallFunc::create([this](){this->sprStick->stopActionByTag(123);}),CallFunc::create([this](){this->sprStick->setRotation(0);}),ScaleTo::create(0.15,1),CallFunc::create([this](){this->sprStick->setTexture("stiker-xanh.png");}),DelayTime::create(0.5),NULL)));
        auto btnSticker = static_cast<Button*>(pnlEvent->getChildByName("btnSticker"));
        if (btnSticker){
            btnSticker->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onBtnSticker, this));
        }
        
        auto lst = mUtils::splitString(dataManager.getAppConfig().STICKER_GAME_STR,';');
        for (int i = 0;i < lst.size();i++){
            auto item = mUtils::splitString(lst[i],',');
            if (item.size() < 3) continue;
            if (SceneManager::getSingleton().getGameID() == atoi(item[0].c_str())){
                    pnlEvent->setVisible(true);
                    this->urlSticker = item[2];
                    txtEvent->setString(item[1]);
            }
            
        }
    }
    
//    LayerMainTop *mLayerMainTop = LayerMainTop::create();
//    Size _s = mLayerMainTop->getContentSize();
//
//    mLayerMainTop->setPosition(Point(0, visibleSize.height - mLayerMainTop->getContentSize().height));
//    mLayerMainTop->setLocalZOrder(1000);
//    ui::Helper::doLayout(mLayerMainTop);
//    mLayerMainTop->currentParent = 1;
//    mLayerMainTop->btnThoat->loadTextures("moc3-btn-thoat.png", "moc3-btn-thoat.png", "moc3-btn-thoat.png");
//    //mLayerMainTop->setTag(TAG_TOP);
//    this->addChild(mLayerMainTop);
    
    
    //	if (nodeMenuTop)
    //	{
    //		nodeMenuTop->setAnchorPoint(Point(0, 1));
    //		nodeMenuTop->setPosition(Point(0, visibleSize.height));
    //		Layer *layerTopMenuOnlyBack = static_cast<Layer*>(nodeMenuTop->getChildByName("layerTopMenuOnlyBack"));
    //		if (layerTopMenuOnlyBack)
    //		{
    //			Button* btnClose = static_cast<Button*>(layerTopMenuOnlyBack->getChildByName("btnBack"));
    //			if (btnClose)
    //			{
    //				if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
    //				{
    //					btnClose->loadTextures("quaylai.png", "quaylai.png", "");
    //				}
    //				btnClose->setPressedActionEnabled(true);
    //				btnClose->addTouchEventListener(CC_CALLBACK_2(ScenePickRoom::onButtonClose, this));
    //			}
    //		}
    //
    //		///MINHDV -changes - add ui picker
    //		Node* nodeFilter = nodeMenuTop->getChildByName("nodeFilter");
    //		if (nodeFilter)
    //		{
    //			nodeFilter->setLocalZOrder(10);
    //			//nodeFilter->setPositionY(sizeAdd.height / 4);
    //			this->Image_24 = (Sprite*)nodeFilter->getChildByName("Image_24");
    //			if (this->Image_24)
    //			{
    //				this->lblMoney1 = (Text*)this->Image_24->getChildByName("lblMoney1");
    //				this->lblMoney1->setTouchEnabled(true);
    //				this->lblMoney2 = (Text*)this->Image_24->getChildByName("lblMoney2");
    //				this->lblMoney2->setTouchEnabled(true);
    //				this->lblMoney3 = (Text*)this->Image_24->getChildByName("lblMoney3");
    //				this->lblMoney3->setTouchEnabled(true);
    //				this->lblMoney4 = (Text*)this->Image_24->getChildByName("lblMoney4");
    //				this->lblMoney4->setTouchEnabled(true);
    //				this->lblMoneyAll = (Text*)this->Image_24->getChildByName("lblMoneyAll");
    //				this->lblMoneyAll->setTouchEnabled(true);
    //				this->spriteArrow = (Sprite*)this->Image_24->getChildByName("spriteArrow");
    //				this->lblMoney1->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonFilterMoney, this));
    //				this->lblMoney2->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonFilterMoney, this));
    //				this->lblMoney3->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonFilterMoney, this));
    //				this->lblMoney4->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonFilterMoney, this));
    //				this->lblMoneyAll->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonFilterMoney, this));
    //			}
    //			//create labelThong bao
    //			pnlThongBao = dynamic_cast<Layout*>(nodeFilter->getChildByName("pnlThongBao"));
    //			if (pnlThongBao)
    //				pnlThongBao->addTouchEventListener(CC_CALLBACK_2(ScenePickRoom::onButtonThongBao, this));
    //			createLabelThongBao("   ");
    //		}
    //
    //		btnBinhDan = dynamic_cast<Button*>(nodeMenuTop->getChildByName("btnBinhDan"));
    //		if (btnBinhDan != NULL){
    //			btnBinhDan->setTag(100);
    //			btnBinhDan->setPressedActionEnabled(true);
    //			btnBinhDan->addTouchEventListener(CC_CALLBACK_2(ScenePickRoom::onButtonFilter, this));
    //			//btnMienPhi->setTitleText("Bình dân");
    //		}
    //
    //		btnTrieuPhu = dynamic_cast<Button*>(nodeMenuTop->getChildByName("btnTrieuPhu"));
    //		if (btnTrieuPhu != NULL){
    //			btnTrieuPhu->setTag(200);
    //			btnTrieuPhu->setPressedActionEnabled(true);
    //			btnTrieuPhu->addTouchEventListener(CC_CALLBACK_2(ScenePickRoom::onButtonFilter, this));
    //			//btnBinhDan->setTitleText("Bình dân");
    //		}
    //
    //		btnDaiGia = dynamic_cast<Button*>(nodeMenuTop->getChildByName("btnDaiGia"));
    //		if (btnDaiGia != NULL){
    //			btnDaiGia->setTag(300);
    //			btnDaiGia->setPressedActionEnabled(true);
    //			btnDaiGia->addTouchEventListener(CC_CALLBACK_2(ScenePickRoom::onButtonFilter, this));
    //			//btnDaiGia->setTitleText("Bình dân");
    //		}
    //		//for game ver p2
    //		btnAll = dynamic_cast<Button*>(nodeMenuTop->getChildByName("btnAll"));
    //		if (btnAll != NULL)
    //		{
    //			btnAll->setTag(400);
    //			btnAll->setPressedActionEnabled(true);
    //			btnAll->addTouchEventListener(CC_CALLBACK_2(ScenePickRoom::onButtonFilter, this));
    //		}
    //
    //
    auto m_lblGameName = dynamic_cast<Text*>(pnlBg->getChildByName("txtTenGame"));
    //
    if (m_lblGameName){
        m_lblGameName->setString(getGameNameByID(gameId));
        //			if (gameId == kGameLieng){
        //				m_lblGameName->setFontSize(50.0);
    }
    //			if (typePR == TableType::BetTable &&
    //				SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
    //			{
    //				m_lblGameName->setPositionX(visibleSize.width / 2);
    //				btnBinhDan->setVisible(false);
    //				btnTrieuPhu->setVisible(false);
    //				btnAll->setVisible(false);
    //				btnDaiGia->setVisible(false);
    //			}
    //			else if (typePR == TableType::BetTable && SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
    //			{
    //				btnBinhDan->setVisible(false);
    //				btnTrieuPhu->setVisible(false);
    //				btnDaiGia->setVisible(false);
    //				// 				if (nodeFilter)
    //				// 				{
    //				// 					Sprite *imgFilter = (Sprite*)nodeFilter->getChildByName("Image_24");
    //				// 					if (imgFilter)
    //				// 						imgFilter->setVisible(false);
    //				// 				}
    //			}
    //		}
    //		Button* btnRankGame = static_cast<Button*>(nodeMenuTop->getChildByName("btnRank"));
    //		if (btnRankGame)
    //		{
    //			btnRankGame->setPressedActionEnabled(true);
    //			btnRankGame->setLocalZOrder(10);
    //			btnRankGame->addTouchEventListener(CC_CALLBACK_2(ScenePickRoom::onButtonRank, this));
    //		}
    //
    //		// 		string listBet = SceneManager::getSingleton().getListBet();
    //		// 		if (0 != listBet.compare(""))
    //		// 		{
    //		// 			initFilterMoney(listBet);
    //		// 			if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
    //		// 			{
    //		// 				onButtonFilter(btnBinhDan, ui::Widget::TouchEventType::ENDED);
    //		// 			}
    //		// 			else{
    //		// 				onButtonFilter(btnAll, ui::Widget::TouchEventType::ENDED);
    //		// 			}
    //		// 		}
    //		// 		else{
    //		//send request get list bet
    //		//}
    //	}
    //	//for gamebai 2.0
    //    if (SceneManager::getSingleton().getCurrRoomType() == 1){
    
    //    }else{
    //        this->onButtonTypeRoom(btnMoneyGold);
    //
    //    }
    
    boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
    boost::shared_ptr<int> _a(new int(1));
    boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("pai", _a, VARIABLETYPE_INT));
    collectionUserVariable->push_back(variable);	//
    boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
    GameServer::getSingleton().Send(request);
    //rejoin if i can
    RejoinInfoStruct rejoinInfo = SceneManager::getSingleton().getRejoinInfo();
    mCurrentRoomIDJoin = atoi(rejoinInfo.gameID.c_str());
    //if can rejoin, send to server - HoangDD
    if (rejoinInfo.isNeedRejoin){
        SceneManager::getSingleton().showLoading();
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ScenePickRoom::onScheduleRejoinRoom), this, 0.5, 1, 0.5, false);
        
    }
    //
    isDenyAllInvite = UserDefault::getInstance()->getBoolForKey("denyAllInvite", true);
    if (typePR != TableType::BetTable)
    {
        Director::getInstance()->getScheduler()->schedule(schedule_selector(ScenePickRoom::onScheduleReloadPlayRoom), this,TIME_AUTO_RELOAD,false);
        int inviteTime = dataManager.getAppConfig().invitetime;
        Director::getInstance()->getScheduler()->schedule(schedule_selector(ScenePickRoom::autoInvitePlay), this,inviteTime,false);

    }
    //this->schedule(schedule_selector(ScenePickRoom::onScheduleThongBao));
    // 	this->schedule(schedule_selector(SceneManager::updateEvent));
    // 	this->schedule(schedule_selector(SceneManager::updateEvent4CheckConnection), 1);
    
    startTimeOutChat();
    createListData();
    //MINHDV add new
    // 	boost::shared_ptr<IRequest> requestMail(new ExtensionRequest("cmi", NULL));//count msg inbox
    // 	GameServer::getSingleton().Send(requestMail);
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    if (mySelf)
    {
        
        boost::shared_ptr<UserVariable> amf_ptr;
        if (SceneManager::getSingleton().getCurrRoomType() == 0){
            amf_ptr = mySelf->GetVariable("amf");
        }
        else{
            amf_ptr = mySelf->GetVariable("amfs");
            
        }
        if (amf_ptr != NULL)
        {
            if (amf_ptr->GetDoubleValue())
                userMoney = *(amf_ptr->GetDoubleValue());
        }
    }
    //set grouplist and subcribe group
    //this->setCurrGroupList(boost::to_string(gameId));
    this->setCurrGroupList(StringUtils::format("%d", gameId));
    subscribeGame(getCurrGroupList());
    
    
    
    auto pnlWebView = (Layout*)pnlBg->getChildByName("pnlWebView");
    //#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    this->bxh = LayerBangXepHang::create();
    this->addChild(this->bxh);
    this->bxh->setPosition(Vec2(pnlWebView->getPosition().x,pnlWebView->getPosition().y+sizeAdd.height));
    this->bxh->setAnchorPoint(pnlWebView->getAnchorPoint());
    this->bxh->loadTop(SceneManager::getSingleton().getGameID(),0);
    auto Image_3 = dynamic_cast<ImageView*>(pnlBg->getChildByName("Image_3"));
    
    this->txtXH = (Text*)Image_3->getChildByName("Text_1");
    this->txtXH->setString(dataManager.GetSysString(979));
    //        auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    //        auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    //        auto posX = pnlWebView->getPositionX();
    //        auto posY = pnlWebView->getPositionY()
    //    webView = cocos2d::experimental::ui::WebView::create();
    //    webView->setAnchorPoint(pnlWebView->getAnchorPoint());
    //    webView->setPosition(Vec2(pnlWebView->getPosition().x,pnlWebView->getPosition().y));
    //    webView->setContentSize(pnlWebView->getContentSize());
    //    webView->setScalesPageToFit(true);
    //    pnlBg->addChild(webView);
    //    webView->setVisible(true);
    //
    //    string strID = StringUtils::format("%d", getGameID());
    //    string type = StringUtils::format("%d", SceneManager::getSingleton().getCurrRoomType());
    //    string strlink = RANK_PICKROOM + strID + "&type="+type;
    //    webView->loadURL(strlink);
    //    auto Image_3 = dynamic_cast<ImageView*>(pnlBg->getChildByName("Image_3"));
    //
    auto Text_1 = dynamic_cast<Text*>(Image_3->getChildByName("Text_1"));
    Text_1->setString(dataManager.GetSysString(866));
    if (dataManager.getAppConfig().isHideBilling){
        this->bxh->setVisible(false);
        auto Text_1_0 = dynamic_cast<Text*>(Image_3->getChildByName("Text_1_0"));
        Text_1->setString(dataManager.GetSysString(867));
        
        Text_1_0->setVisible(true);
    }
    
    //#endif
    if (SceneMain::getSingleton().currentLayer == 0){
        this->roomType = 1;
        this->onButtonTypeRoom(btnMoneyGold);
    }else{
        if (SceneManager::getSingleton().getCurrRoomType() == 1){
            this->roomType = 0;
            this->onButtonTypeRoom(btnMoneySilver);
        }else{
            this->roomType = 1;
            this->onButtonTypeRoom(btnMoneyGold);
        }
    }
   
    
    return true;
}
void ScenePickRoom::onBtnSticker(Ref* pSender){
    LayerGuideInGame* layer = LayerGuideInGame::create();
    layer->setLink(this->urlSticker);
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene)
        return;
    scene->addChild(layer, 2000);
    layer->setTag(1504);
}
void ScenePickRoom::hideWebView(){
    //#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    webView->setVisible(false);
    //    if (dataManager.getAppConfig().isHideBilling){
    //        webView->setVisible(false);
    //    }
    //
    //#endif
}
void ScenePickRoom::showWebView(){
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    if (dataManager.getAppConfig().isHideBilling){
    ////        webView->setVisible(false);
    ////        auto Image_3 = dynamic_cast<ImageView*>(pnlBg->getChildByName("Image_3"));
    ////        auto Text_1 = dynamic_cast<Text*>(Image_3->getChildByName("Text_1"));
    ////        Text_1->setString("GỢI Ý");
    ////        auto Text_1_0 = dynamic_cast<Text*>(Image_3->getChildByName("Text_1_0"));
    ////        Text_1_0->setVisible(true);
    //        return;
    //    }
    //    //webView->setVisible(true);
    //    string strID = StringUtils::format("%d", getGameID());
    //    string type = StringUtils::format("%d", SceneManager::getSingleton().getCurrRoomType());
    //    string strlink = RANK_PICKROOM + strID + "&type=" + type;
    //    webView->loadURL(strlink);
    //    if (dataManager.getAppConfig().isHideBilling){
    //        webView->setVisible(false);
    //    }
    
#endif
}
void ScenePickRoom::onButtonTaoBan(Ref* pSender){
    if (SceneManager::getSingleton().getCurrRoomType() == 1){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(401));
        
        return;
    }
    if (SceneManager::getSingleton().getCapVip() < dataManager.getAppConfig().mintaoban){
        SceneManager::getSingleton().showToast(StringUtils::format("%s %d",dataManager.GetSysString(402).c_str(), dataManager.getAppConfig().mintaoban));
        
        return;
    }
    auto _currScene = Director::getInstance()->getRunningScene();

    LayerPopupTaoBan* popup = LayerPopupTaoBan::create();
    _currScene->addChild(popup, 2000,9155);
}
void ScenePickRoom::onButtonTimBan(Ref* pSender){
    if (SceneManager::getSingleton().getCurrRoomType() == 1){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(403));
        
        return;
    }
    auto _currScene = Director::getInstance()->getRunningScene();
    LayerPopupTimBan* popup = LayerPopupTimBan::create();
    _currScene->addChild(popup, 2000,9154);
}
void ScenePickRoom::timeoutChatMaxtime(float dt){
    SceneManager::getSingleton().timerchat++;
   // log("timer chat couting!");
    if (SceneManager::getSingleton().timerchat == dataManager.getAppConfig().chattimeout*SceneManager::getSingleton().sec) {
        SceneManager::getSingleton().timerchat = 0;
        SceneManager::getSingleton().chatcounttime = 0;
        SceneManager::getSingleton().arrCurrentChat.clear();
        this->unschedule(schedule_selector(ScenePickRoom::timeoutChatMaxtime));
    }
}
void ScenePickRoom::startTimeOutChat(){
    if (SceneManager::getSingleton().timerchat > 0){
        this->schedule(schedule_selector(ScenePickRoom::timeoutChatMaxtime), 1);
    }
}

//Mail Top
void ScenePickRoom::gotoMail(Ref *pSender){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    this->tagChild = TOP_MAIL;
    
    if (this->getChildByTag(TAG_CHILD) != NULL){
        this->removeChildByTag(TAG_CHILD, true);
    }
    
    LayerMailInbox* _layerMailInbox = LayerMailInbox::create();
    _layerMailInbox->setTag(TAG_CHILD);
    _layerMailInbox->setContentSize(Size(1920, 1080));
    _layerMailInbox->setLocalZOrder(0);
    _layerMailInbox->setAnchorPoint(Vec2(0, 0));
    _layerMailInbox->setTag(TAG_CHILD);
    this->addChild(_layerMailInbox);
}

void ScenePickRoom::gotoPickRoom(){
    this->removeChildByTag(TAG_CHILD, true);
}

void ScenePickRoom::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type){
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        unSubscribeGame();
        SceneManager::getSingleton().setGameID(-1);
        SceneManager::getSingleton().gotoMain();
    }
    
}
void ScenePickRoom::gotoChargeMoney()
{
    //    this->tagChild = TOP_RECHARGE;
    //    if (this->getChildByTag(TAG_CHILD) != NULL){
    //        this->removeChildByTag(TAG_CHILD, true);
    //    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    LayerRecharge* _layerRecharge = LayerRecharge::create();
    _layerRecharge->setContentSize(visibleSize);
    _layerRecharge->setAnchorPoint(Vec2(0, 0));
    _layerRecharge->currScene = 2;
    //_layer->setPosition(Point(mLayerMainTop->getPositionX(), visibleSize.height / 2 - mLayerMainContent->getContentSize().height / 2));
    _layerRecharge->setTag(TAG_CHILD);
    this->addChild(_layerRecharge);
    //    if(mLayerMainTop->isVisible())
    //        mLayerMainTop->setVisible(false);
    layerScenePickRoomBottom->setVisible(false);
    setDenyAllInvite();
}
void ScenePickRoom::onButtonFilter(Ref* pSender, ui::Widget::TouchEventType type)

{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getSingleton().showLoading();
        //		auto _btn = dynamic_cast<Button*>(pSender);
        //		if (_btn){
        //			if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
        //			{
        //				Color3B _colorPress = Color3B(26, 26, 26);
        //				Color3B _colorNormal = Color3B(255, 255, 255);
        //				switch (_btn->getTag())
        //				{
        //				case 100://binh dan
        //					currFilterBet = tag_binh_dan;
        //					btnBinhDan->setEnabled(false);
        //					btnTrieuPhu->setEnabled(true);
        //					btnDaiGia->setEnabled(true);
        //					btnBinhDan->setTitleColor(_colorPress);
        //					btnTrieuPhu->setTitleColor(_colorNormal);
        //					btnDaiGia->setTitleColor(_colorNormal);
        //					break;
        //				case 200://trieu phu
        //					currFilterBet = tag_trieu_phu;
        //					btnBinhDan->setEnabled(true);
        //					btnTrieuPhu->setEnabled(false);
        //					btnDaiGia->setEnabled(true);
        //					btnBinhDan->setTitleColor(_colorNormal);
        //					btnTrieuPhu->setTitleColor(_colorPress);
        //					btnDaiGia->setTitleColor(_colorNormal);
        //					break;
        //				case 300://Dai gia
        //					currFilterBet = tag_dai_gia;
        //					btnBinhDan->setEnabled(true);
        //					btnTrieuPhu->setEnabled(true);
        //					btnDaiGia->setEnabled(false);
        //					btnTrieuPhu->setTitleColor(_colorNormal);
        //					btnBinhDan->setTitleColor(_colorNormal);
        //					btnDaiGia->setTitleColor(_colorPress);
        //					break;
        //				}
        //				onButtonFilterPicker();
        //			}
        //			else{
        //				Color3B _colorPress = Color3B(255, 255, 255);
        //				Color3B _colorNormal = Color3B(255, 255, 255);
        //				switch (_btn->getTag())
        //				{
        //				case 100://binh dan
        //					currFilterBet = tag_binh_dan;
        //					btnBinhDan->setEnabled(false);
        //					btnTrieuPhu->setEnabled(true);
        //					btnDaiGia->setEnabled(true);
        //					btnBinhDan->setTitleColor(_colorPress);
        //					btnTrieuPhu->setTitleColor(_colorNormal);
        //					btnDaiGia->setTitleColor(_colorNormal);
        //					btnAll->setEnabled(true);
        //					btnAll->setTitleColor(_colorNormal);
        //					break;
        //				case 200://trieu phu
        //					currFilterBet = tag_trieu_phu;
        //					btnBinhDan->setEnabled(true);
        //					btnTrieuPhu->setEnabled(false);
        //					btnDaiGia->setEnabled(true);
        //					btnBinhDan->setTitleColor(_colorNormal);
        //					btnTrieuPhu->setTitleColor(_colorPress);
        //					btnDaiGia->setTitleColor(_colorNormal);
        //					btnAll->setEnabled(true);
        //					btnAll->setTitleColor(_colorNormal);
        //					break;
        //				case 300://Dai gia
        //					currFilterBet = tag_dai_gia;
        //					btnBinhDan->setEnabled(true);
        //					btnTrieuPhu->setEnabled(true);
        //					btnDaiGia->setEnabled(false);
        //					btnTrieuPhu->setTitleColor(_colorNormal);
        //					btnBinhDan->setTitleColor(_colorNormal);
        //					btnDaiGia->setTitleColor(_colorPress);
        //					btnAll->setEnabled(true);
        //					btnAll->setTitleColor(_colorNormal);
        //					break;
        //				case 400://all
        //					currFilterBet = tag_all;
        //					btnAll->setEnabled(false);
        //					btnBinhDan->setEnabled(true);
        //					btnTrieuPhu->setEnabled(true);
        //					btnDaiGia->setEnabled(true);
        //					btnTrieuPhu->setTitleColor(_colorNormal);
        //					btnBinhDan->setTitleColor(_colorNormal);
        //					btnDaiGia->setTitleColor(_colorNormal);
        //					btnAll->setTitleColor(_colorPress);
        //					break;
        //				}
        //tat ca thi khong can chon khoang tien
        //				SceneManager::getSingleton().showLoading();
        //				string betvl = "All";
        //
        //				if (currFilterBet == tag_all)
        //				{
        //					betvl = "All_All";
        //				}
        //				else{
        //					if (arrBet.size() == 0)
        //						return;
        //					int jumpArr = arrBet.size() / 3;//3 muc
        //					try{
        //						lstFilterMoney.clear();
        //						for (int i = currFilterBet * jumpArr; i < jumpArr * (currFilterBet + 1); i++)
        //						{
        //							lstFilterMoney.push_back(arrBet.at(i));
        //						}
        //					}
        //					catch (...)
        //					{
        //						log("EXCEPTION");
        //					}
        //					currFilterMoneyIndex = lstFilterMoney.size() - 1;
        //				}
        //				onButtonFilterRoom(betvl);
        //			}
        int gameID = getGameID();
        boost::shared_ptr<SFSObject> param(new SFSObject());
        // 		param->PutInt(EXT_FIELD_BET_FROM, NULL);
        // 		param->PutInt(EXT_FIELD_BET_TO, NULL);
        //param->PutUtfString(EXT_FIELD_GAME_ID, boost::to_string(gameID));
        param->PutUtfString(EXT_FIELD_GAME_ID, StringUtils::format("%d", gameID));
        param->PutInt("test", 5);
        param->PutInt(EXT_FIELD_ROOM_TYPE, this->roomType);
        boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_GET_LIST_ROOM, param));
        GameServer::getSingleton().Send(request);
    }
}

void ScenePickRoom::OnSmartFoxInvitation(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    // 	if (getIsDenyAllInvite())
    // 		return;
    return;
    if (this->getChildByTag(tag_layer_create_room) ||
        this->getChildByTag(tag_layer_find_room) ||
        this->getChildByTag(tag_layer_avatar) ||
        this->getChildByTag(tag_layer_nap_tien) ||
        this->getChildByTag(tag_layer_invite) ||
        this->getChildByTag(tag_layer_password))
        return;
    
   // log("Invite Player");
    passRoom4Invite = "";
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueInvitation = (*ptrEvetnParams)["invitation"];
    boost::shared_ptr<Invitation::Invitation> invitation = ((boost::static_pointer_cast<Invitation::Invitation>(ptrEventParamValueInvitation)));
    
    // 	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = invitation->Params();
    //
    boost::shared_ptr<User> MySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    //Get PAI
    string pass_ = "";
    if (MySelf == NULL) return;
    if (param->GetUtfString("pass") != NULL)
        passRoom4Invite = *param->GetUtfString("pass");
    
   // log("Invitee: %s Inviter: %s, gameID: %s, minbet: %s, pass: %s",
//        invitation->Invitee()->Name()->c_str(),
//        invitation->Inviter()->Name()->c_str(),
//        param->GetUtfString("gid")->c_str(),
//        param->GetUtfString("mb")->c_str(),
//        passRoom4Invite.c_str());
    
    if (MySelf->GetVariable("pai")->GetIntValue() != 0)
    {
        return;
        bool isInvite = UserDefault::getInstance()->getBoolForKey("denyAllInvite", true);
        
        if (!isInvite) return;
        if (!this->getChildByTag(tagAutoInvite)){
            int roomID = atoi(param->GetUtfString("roomid")->c_str());
            boost::shared_ptr<Room> ro = GameServer::getSingleton().getSmartFox()->GetRoomById(roomID);
            if (ro == NULL) return;
            string sRoomIdDisplay = mUtils::getRoomIDByName(ro->Name()->c_str());
            if (sRoomIdDisplay == "")
                sRoomIdDisplay = StringUtils::format("%d", roomID);
            
            int roomMax = ro->MaxUsers();
            int roomNum = ro->UserCount();
            string _status = StringUtils::format("%d/%d", roomNum, roomMax);
            auto _currScene = Director::getInstance()->getRunningScene();
            
            LayerPopupInviteGame* _layer = LayerPopupInviteGame::create();
            _layer->setCurrScreen(LayerPopupInviteGame::tagPickGame);
            _layer->setTag(tag_layer_invite);
            boost::shared_ptr<UserVariable> userPtr = invitation->Inviter()->GetVariable(ACC_DISPLAY_NAME);
            string userInvite = invitation->Inviter()->Name()->c_str();
            if (userPtr != NULL)
                userInvite = userPtr->GetStringValue()->c_str();
            _layer->setDatas(userInvite, 12345, sRoomIdDisplay
                             , getGameID()
                             , passRoom4Invite
                             , atoi(param->GetUtfString("mb")->c_str())
                             , _status, roomID);
            _layer->setLocalZOrder(tag_layer_invite);
            _currScene->addChild(_layer,3000,9142);
        }
    }
}

void ScenePickRoom::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    ///loi nay hay bi nen bi unschedule luon
    //this->unschedule(CC_SCHEDULE_SELECTOR(ScenePickRoom::onScheduleUpdateRoomList));
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
    boost::shared_ptr<Room> roomJoin = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
    // 	if (!roomJoin->IsPasswordProtected())
    // 		SceneManager::getSingleton().setAutoCreateRoom(true);
    /*if (roomJoin->GroupId()->compare(SERVICE_CHAT_ROOM) == 0)
     return;
    // log("Join Room In Chon Ban Choi ID = %d ------------------------------", getGameID());
     string gameID = StringUtils::format("%d", getGameID());
     if (gameID.length() > 3)
     gameID = gameID.substr(0, 3);*/
    /*moc3-change*/
    int roomID = roomJoin->Id();
    if (roomJoin->GroupId()->compare(SERVICE_CHAT_ROOM) == 0 || roomJoin->GroupId()->compare("200") == 0)
        return;
    int gameID = atoi(roomJoin->GroupId()->c_str());
    SceneManager::getSingleton().setGameID(gameID);
   // log("Join Room In Chon Ban Choi ID = %d ------------------------------", gameID);
    string _gameID = StringUtils::format("%d", gameID);
    if (_gameID.length() > 3)
        _gameID = _gameID.substr(0, 3);
    /*moc3-end change*/
    isRejoinRoom = false;
    //SceneManager::getSingleton().hideLoading();
    RejoinInfoStruct sRejoinInfoOld = SceneManager::getSingleton().getRejoinInfo();
    mCurrentRoomIDJoin = -1;
    RejoinInfoStruct rejoinInfo;
    SceneManager::getSingleton().setRejoinInfo(rejoinInfo);
    //Update, khong chap nhan loi moi nua
    boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
    boost::shared_ptr<int> _a(new int(0));
    boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("pai", _a, VARIABLETYPE_INT));
    collectionUserVariable->push_back(variable);	//
    boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
    GameServer::getSingleton().Send(request);
    
    if (sRejoinInfoOld.roomName.length() > 0){
        SceneManager::getSingleton().gotoGame(atoi(_gameID.c_str()));
        return;
    }
   // if (getGameID() != kGamePoker){
        SceneManager::getSingleton().gotoGame(atoi(_gameID.c_str()));
//    }
    
    
}

void ScenePickRoom::OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
   // log("Join Room Error");
    //SceneManager::getSingleton().hideLoading();
    RejoinInfoStruct rejoinInfo;
    SceneManager::getSingleton().setRejoinInfo(rejoinInfo);
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueErrorMessage = (*ptrEventParams)["errorMessage"];
    boost::shared_ptr<string> ptrErrorMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueErrorMessage);
    //
    boost::shared_ptr<void> ptrEventParamValueErrorCode = (*ptrEventParams)["errorCode"];
    boost::shared_ptr<short> ptrErrorCode = ((boost::static_pointer_cast<short>))(ptrEventParamValueErrorCode);
    //
   // log("OnSmartFoxRoomJoinError - errorCode: %d", *ptrErrorCode);
    boost::shared_ptr<string> message(new string("Join Room Failure: " + *ptrErrorMessage));
    //Error code = 20 is Room is fulls
    
    string mess = mUtils::getErrorSmartfoxExplain(*ptrErrorCode);
    SceneManager::getSingleton().hideLoading();
   // log("Join Room Failure: %s", mess.c_str());
    //Neu ma rejoin room, thi se ko thong bao gi ca
    if (!isRejoinRoom)
    {
        if (*ptrErrorCode != 19) //tam an err code 19
            this->showToast(mess.c_str());
    }
    
    isRejoinRoom = false;
    //
    switch (*ptrErrorCode){
        case 20:
            //Room is full
            if (mCurrentRoomIDJoin != -1){
                int a = -1;
                if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom()){
                    a = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->Id();
                }
                //if (a == -1) return;
                boost::shared_ptr<long int> id2Left(new long int(a));
                //
                boost::shared_ptr<IRequest> request(new JoinRoomRequest(mCurrentRoomIDJoin, "", id2Left, true));
                GameServer::getSingleton().Send(request);
                mCurrentRoomIDJoin = -1;
            }
            break;
        case 96:
            SceneManager::getSingleton().showLoading();
            //GameServer::getSingleton().getSmartFox()->Disconnect();
            break;
        default:
            if (*ptrErrorCode != 19)
                this->showToast(mess.c_str());
            break;
    }
    //SceneManager::getSingleton().gotoPickGame(SceneManager::getSingleton().getGameID());
}

void ScenePickRoom::OnSmartFoxRoomCreationError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
   // log("Create Room Error");
    //SceneManager::getSingleton().hideLoading();
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueErrorMessage = (*ptrEventParams)["errorMessage"];
    boost::shared_ptr<string> ptrErrorMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueErrorMessage);
    if (ptrErrorMessage != NULL) return;
    boost::shared_ptr<string> message(new string("Room Create Failure: " + *ptrErrorMessage));
    //
    this->showToast(message->c_str());
}

void ScenePickRoom::OnSmartFoxRoomAdd(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
}

void ScenePickRoom::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    //log("User Exit Room Ở chọn bàn chơi");
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEvetnParams)["user"];
    boost::shared_ptr<User> user = ((boost::static_pointer_cast<User>(ptrEventParamValueUser)));
    if (user->IsItMe()){
        boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
        boost::shared_ptr<int> _a(new int(1));
        boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("pai", _a, VARIABLETYPE_INT));
        collectionUserVariable->push_back(variable);	//
        boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
        GameServer::getSingleton().Send(request);
    }
}

void ScenePickRoom::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    if (mySelf == NULL)
        return;
    if (ptrNotifiedCmd == NULL)
        return;
   
    else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_RES_GET_LIST_BET))
    {
       // log("EXT_EVENT_RES_GET_LIST_BET");
        boost::shared_ptr<string> listPtr = param->GetUtfString(EXT_FIELD_LIST_BET);
        boost::shared_ptr<long> dfz_ptr = param->GetInt(EXT_FIELD_DEFAULT_ZONE);
        if (listPtr)
        {
            string listBet = listPtr->c_str();
            SceneManager::getSingleton().setListBet(listBet);
            initFilterMoney(listBet);
            onButtonFilter(btnTrieuPhu, ui::Widget::TouchEventType::ENDED);
            //			if (dfz_ptr != NULL && SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
            //			{
            //				int dfz = *dfz_ptr;
            //				switch (dfz)
            //				{
            //				case 0:
            //					onButtonFilter(btnBinhDan, ui::Widget::TouchEventType::ENDED);
            //					break;
            //				case 1:
            //					onButtonFilter(btnTrieuPhu, ui::Widget::TouchEventType::ENDED);
            //					break;
            //				case 2:
            //					onButtonFilter(btnDaiGia, ui::Widget::TouchEventType::ENDED);
            //					break;
            //				default:
            //					onButtonFilter(btnBinhDan, ui::Widget::TouchEventType::ENDED);
            //					break;
            //				}
            //			}
            //			else{
            //				//if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
            //				onButtonFilter(btnBinhDan, ui::Widget::TouchEventType::ENDED);
            //				// 				else if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
            //				// 					onButtonFilter(btnAll, ui::Widget::TouchEventType::ENDED);
            //			}
            // 			string _list = "100:3,1,2,3,2|200:3,1,2|100:3,1,2,3,2|200:3,1,2";
            //  			vector<string> arrStr = mUtils::splitString(_list, '|');
            //  			for (int i = 0; i < arrStr.size(); i++)
            //  			{
            //  				vector<string> arrInfo = mUtils::splitString(arrStr.at(i), ':');
            //  				if (arrInfo.size()> 1)
            //  				{
            //  					int betvl = atoi(arrInfo.at(0).c_str());
            //  					vector<string> arrUserCount = mUtils::splitString(arrInfo.at(1), ',');
            //  					for (int j = 0; j < arrUserCount.size(); j++)
            //  					{
            //  						listRoom.push_back(RoomInfo(betvl, atoi(arrUserCount.at(j).c_str())));
            //  					}
            //  				}
            //  			}
            //  			tblPlayRooms->reloadData();
        }
    }
    else if (0 == ptrNotifiedCmd->compare("rsilf")){
        //// log("LK fb:%s",*param->GetUtfString("rd")->c_str());
        if (param->GetUtfString("coinBonus") == NULL || param->GetInt("rc") == NULL) return;
        int rc = *param->GetInt("rc");
        if (rc == 0){
            string bonus = *param->GetUtfString("coinBonus");
            auto lstbonus = mUtils::splitString(bonus, '|');
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            
            
            //			if (atoi(lstbonus[0].c_str()) > 0){
            layerPopup->setContentMess(dataManager.GetSysString(619));
            this->addChild(layerPopup);
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
            UserDefault *def = UserDefault::getInstance();
            def->setBoolForKey("isLinkFacebook", true);
            dataManager.getAppConfig().isreceivedLinkFace = true;
        }
        else{
            string desc = "";
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            
            this->addChild(layerPopup);
            
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
    else if (0 == ptrNotifiedCmd->compare(EXT_EVENT_RES_GET_LIST_ROOM) && typePR == TableType::PlayTable)
    {
        boost::shared_ptr<string> listPtr = param->GetUtfString(EXT_FIELD_LIST_ROOM);
        if (listPtr)
        {
           // log("LIST ROOM = %s", listPtr->c_str());
            listRoom.clear();
            vector<string> arrStr = mUtils::splitString(*listPtr, '|');
            for (int i = 0; i < arrStr.size(); i++)
            {
                vector<string> arrInfo = mUtils::splitString(arrStr.at(i), ':');
                if (arrInfo.size()> 1)
                {
                    //                   bool isExist = false;
                    int betvl = atoi(arrInfo.at(0).c_str());
                    //                    for (int i = 0 ; i < listRoom.size() ;i ++){
                    //                        if (betvl == listRoom[i].betvl )
                    //                            isExist = true;
                    //                    }
                    //                    if (isExist)
                    //                        continue;
                    //                    vector<string> arrUserCount = mUtils::splitString(arrInfo.at(1), ',');
                    //                    for (int j = 0; j < arrUserCount.size(); j++)
                    //                    {
                    //                        if (0 != arrUserCount.at(j).compare(" "))
                    listRoom.push_back(RoomInfo(betvl, 0));
                    //                    }
                }
            }
            //load theo muc hien tai neu khong phai auto reload
            //            if (lstFilterMoney.size() >0)
            //            {
            //                string betvl = lstFilterMoney.at(currFilterMoneyIndex);
            //                if (0 == betvl.compare("All"))
            //                {
            SceneManager::getSingleton().hideLoading();
            listRoomMain.clear();
            listRoomMain = listRoom;
            //tblPlayRooms->reloadData();
            this->createScrollView();
            SceneManager::getSingleton().hideLoading();
            //                }
            //                else{
            //                    if (isAutoReload)
            //                    {
            //                        isAutoReload = false;
            //                        onButtonFilterRoom(betvl);
            //                    }
            //                }
            //            }
            //            else{
            //                SceneManager::getSingleton().hideLoading();
            //                listRoomMain.clear();
            //                listRoomMain = listRoom;
            //                tblPlayRooms->reloadData();
            //            }
            
        }
    }
    else {
       // log("CMD %s", ptrNotifiedCmd->c_str());
    }
    
    // 	if (strcmp("cmi", ptrNotifiedCmd->c_str()) == 0){
    // 		if (!param->GetInt("slmin")) return;
    // 		int count = *param->GetInt("slmin");
    // 		if (count == 0){
    // 			bagItem->setVisible(false);
    // 			return;
    // 		}
    // 		bagItem->setVisible(true);
    // 		bagItem->stopAllActions();
    // 		bagItem->setNumber(boost::to_string(count));
    // 		bagItem->setVisible(true);
    // 		bagItem->runAction(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 1), NULL));// cc.sequence(cc.scaleTo(0.5, 1.2), cc.scaleTo(0.5, 1)));
    // 	}
    
}
void ScenePickRoom::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    if (ptrEventParams == NULL)
        return;
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    if (ptrNotifiedUser == NULL) return;
    if (ptrNotifiedUser->IsItMe() == true)
    {
        //loadDataInfo();
        boost::shared_ptr<UserVariable> amfNow;
        if (SceneManager::getSingleton().getCurrRoomType() == 0){
            amfNow = ptrNotifiedUser->GetVariable("amf");
        }
        else{
            amfNow = ptrNotifiedUser->GetVariable("amfs");
            
        }
        if (amfNow != NULL)
        {
            boost::shared_ptr<double> amf = amfNow->GetDoubleValue();
            if (amf != NULL)
            {
                if (*amf != userMoney)
                {
                    userMoney = *amf;
                    this->createListData();
                }
            }
        }
    }
}

void ScenePickRoom::OnSmartFoxRoomDestroy(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
   // log("Room Destroy !!!!");
    //A Room was removed
    // 	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    // 	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
    // 	boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);
    //
    // 	log("Room %s destroyed!", ptrNotifiedRoom->Name()->c_str());
    // 	if( ptrNotifiedRoom->IsJoined() ){
    // 		log("Destroy mie roi con dau");
    // 	}
}

void ScenePickRoom::subscribeGame(string type)
{
    vector<string> lstGroups = mUtils::splitString(type, ';');
    for (int i = 0; i < lstGroups.size(); i++){
        if (lstGroups.at(i).compare("") == 0) continue;
        boost::shared_ptr<IRequest> request(new SubscribeRoomGroupRequest(lstGroups.at(i)));
        GameServer::getSingleton().Send(request);
    }
    //
    boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
    boost::shared_ptr<int> _a(new int(atoi(getCurrGroupList().substr(0, 3).c_str())));
    boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("cvg", _a, VARIABLETYPE_INT));
    collectionUserVariable->push_back(variable);	//
    boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
    GameServer::getSingleton().Send(request);
}

void ScenePickRoom::autoInvitePlay(float dt)
{
    if (this->getChildByTag(tag_layer_create_room) ||
        this->getChildByTag(tag_layer_find_room) ||
        this->getChildByTag(tag_layer_avatar) ||
        this->getChildByTag(tag_layer_nap_tien) ||
        this->getChildByTag(tag_layer_invite) ||
        this->getChildByTag(tag_layer_password))
        return;
    this->unschedule(schedule_selector(ScenePickRoom::autoInvitePlay));
    int gameID = SceneManager::getSingleton().getGameID();
    //	if (gameID == kGameLieng || gameID == kGameXiTo)
    //		return;
    if (!getIsDenyAllInvite())//do setting la nhan loi moi, nen bien = false tuc la khong chan loi moi
        return;
    
    if (this->getChildByTag(tagAutoInvite))
        return;
    boost::shared_ptr<User> MySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    if (MySelf == NULL)
        return;
    if (MySelf->GetVariable("pai")->GetIntValue() == 0)
        return;
   // log("autoInvitePlay");
    
    double amfUser = 0;
    if (this->roomType == 0)
        amfUser = GameUtils::getAmfGoldUser(GameUtils::getMyName());
    else if (this->roomType == 1)
        amfUser = GameUtils::getAmfSilverUser(GameUtils::getMyName());
    
    double betUser = getMinBetByGame(gameID, amfUser);
    //toi thieu la betUser >0 la ok van dc moi
    if (betUser >= 1)
    {
        auto _currScene = Director::getInstance()->getRunningScene();
        LayerPopupInviteGame* _layer = LayerPopupInviteGame::create();
        _layer->setTag(tagAutoInvite);
        _layer->showFakeInfo(gameID);
        //_layer->setLocalZOrder(tag_layer_invite);
        _layer->setLocalZOrder(5000);
        _currScene->addChild(_layer,3000,9144);
    }
    //
    // 	log("rooms size %d", lstRooms.size());
    // 	int i = 0;
    
    //  	for (i = 0; i < lstRooms.size(); i++)
    //  	{
    //  		if (lstRooms.at(i)->IsPasswordProtected()) continue;
    //  		boost::shared_ptr<Room> r = lstRooms.at(i);
    //  		boost::shared_ptr<RoomVariable> rv = r->GetVariable("params");
    //  		string s = *rv->GetStringValue();
    //  		vector<string> lstV = mUtils::splitString(s, '@');
    //  		long roomMoney = atol(lstV[0].c_str());
    //  		int room_numbers = atoi(lstV[2].c_str());
    //  		if (r->MaxUsers() == room_numbers)
    //  			continue;
    //  		double betUser = getMinBetByGame(gameID, *Amf);
    //  		//toi thieu la betUser >0 la ok van dc moi
    //  		if (betUser > 0)
    //  		{
    //  			lstRoomsID.push_back(lstRooms.at(i)->Id());
    //  			lstRoomsBet.push_back(roomMoney);
    //  		}
    //  		// 		if (*Amf > roomMoney){
    //  		// 			lstRoomsID.push_back(lstRooms.at(i)->Id());
    //  		// 			lstRoomsBet.push_back(roomMoney);
    //  		// 		}
    //  	}
    //
    // 	if (lstRoomsID.size() == 0)
    // 	{
    // 		return;
    // 	}
    // 	srand(time(NULL));
    // 	int vt = rand() % lstRoomsID.size();
    // 	log("moi vao phong id = %ld", lstRoomsID.at(vt));
    //
    // 	if (MySelf->GetVariable("pai")->GetIntValue() != 0)
    // 	{
    // 		int roomID = lstRoomsID.at(vt);
    // 		boost::shared_ptr<Room> ro = GameServer::getSingleton().getSmartFox()->GetRoomById(roomID);
    // 		if (ro == NULL) return;
    // 		string sRoomIdDisplay = mUtils::getRoomIDByName(ro->Name()->c_str());
    // 		if (sRoomIdDisplay == "")
    // 			sRoomIdDisplay = StringUtils::format("%d", roomID);
    // 		LayerPopupInviteGame* _layer = LayerPopupInviteGame::create();
    // 		_layer->setTag(tagAutoInvite);
    // 		_layer->setDatas(dataManager.GetSysString(248), 10000, sRoomIdDisplay, getGameID(), "", lstRoomsBet.at(vt), "2/4", roomID);
    // 		_layer->setLocalZOrder(tag_layer_invite);
    // 		this->addChild(_layer);
    // 	}
}

void ScenePickRoom::continueInvitePlay()
{
   // log("continueInvitePlay");
    Director::getInstance()->getScheduler()->schedule(schedule_selector(ScenePickRoom::autoInvitePlay), this, 60, 1, 60, false);}

void ScenePickRoom::setDenyAllInvite()
{
    setIsDenyAllInvite(true);
    //deny all
    boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
    boost::shared_ptr<int> _a(new int(0));
    boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("pai", _a, VARIABLETYPE_INT));
    collectionUserVariable->push_back(variable);	//
    boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
    GameServer::getSingleton().Send(request);
    //
    UserDefault::getInstance()->setBoolForKey("denyAllInvite", false);
    UserDefault::getInstance()->flush();
}

void ScenePickRoom::unSubscribeGame()
{
    //unsubscribe
    if (GameServer::getSingleton().getSmartFox()->RoomManager() == NULL) return;
    boost::shared_ptr<vector<string> > pRoomGroups = GameServer::getSingleton().getSmartFox()->RoomManager()->GetRoomGroups();
    if (pRoomGroups == NULL) return;
    string lstGroups = "";
    for (int i = 0; i < pRoomGroups->size(); i++){
        if (pRoomGroups->at(i).compare("118") == 0 || pRoomGroups->at(i).compare("Default") == 0)
            continue;
        boost::shared_ptr<IRequest> request(new UnsubscribeRoomGroupRequest(pRoomGroups->at(i)));
        GameServer::getSingleton().Send(request);
    }
    //
    boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
    boost::shared_ptr<int> _a(new int(-1));
    boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("cvg", _a, VARIABLETYPE_INT));
    collectionUserVariable->push_back(variable);	//
    boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
    GameServer::getSingleton().Send(request);
}

vector<boost::shared_ptr<Room> > ScenePickRoom::getListRooms()
{
    vector<boost::shared_ptr<Room> > lstRooms;
    std::string lst = this->getCurrGroupList();
    vector<string> lstRoomGroupsName = mUtils::splitString(lst, ';');
    for (int i = 0; i < lstRoomGroupsName.size(); i++){
        if (lstRoomGroupsName.at(i).compare("") == 0) continue;
        boost::shared_ptr<vector<boost::shared_ptr<Room> > > rooms
        = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(lstRoomGroupsName.at(i));
        for (int j = 0; j < rooms->size(); j++)
            lstRooms.push_back(rooms->at(j));
    }
    return lstRooms;
}

void ScenePickRoom::onScheduleReloadPlayRoom(float dt)
{
    int gameID = getGameID();
    boost::shared_ptr<SFSObject> param(new SFSObject());
    // 		param->PutInt(EXT_FIELD_BET_FROM, NULL);
    // 		param->PutInt(EXT_FIELD_BET_TO, NULL);
    //param->PutUtfString(EXT_FIELD_GAME_ID, boost::to_string(gameID));
    param->PutUtfString(EXT_FIELD_GAME_ID, StringUtils::format("%d", gameID));
    param->PutInt(EXT_FIELD_ROOM_TYPE, this->roomType);
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_GET_LIST_ROOM, param));
    GameServer::getSingleton().Send(request);
}

void ScenePickRoom::initFilterMoney(string listBet)
{
    lstFilterMoney.clear();
    arrBet.clear();
    vector<string> arrStr = mUtils::splitString(listBet, '|');
    for (int i = 0; i < arrStr.size(); i++)
    {
        vector<string> arr = mUtils::splitString(arrStr.at(i), ':');
        if (arr.size() > 1)
        {
            vector<string> arrStrBet = mUtils::splitString(arr.at(1), ',');
            for (int j = 0; j < arrStrBet.size(); j++)
            {
                arrBet.push_back(arrStrBet.at(j));
            }
            arrBet.push_back("All");
        }
    }
    // 	int gameID = SceneManager::getSingleton().getGameID();
    // 	if (gameID == kGameCoTuong || gameID == kGameCoUp)
    // 	{
    // 		switch (atoi(getCurrGroupList().substr(3).c_str())) {
    // 		case 5:
    // 			lstFilterMoney.push_back("10k");
    // 			lstFilterMoney.push_back("20k");
    // 			lstFilterMoney.push_back("50k");
    // 			lstFilterMoney.push_back("All");
    // 			break;
    // 		case 10:
    // 			lstFilterMoney.push_back("100k");
    // 			lstFilterMoney.push_back("200k");
    // 			lstFilterMoney.push_back("500k");
    // 			lstFilterMoney.push_back("All");
    // 			break;
    // 		default:
    // 			lstFilterMoney.push_back("1k");
    // 			lstFilterMoney.push_back("2k");
    // 			lstFilterMoney.push_back("5k");
    // 			lstFilterMoney.push_back("All");
    // 			break;
    // 		}
    // 	}
    // 	else{
    // 		switch (atoi(getCurrGroupList().substr(3).c_str())) {
    // 		case 5:
    // 			lstFilterMoney.push_back("5k");
    // 			lstFilterMoney.push_back("10k");
    // 			lstFilterMoney.push_back("20k");
    // 			lstFilterMoney.push_back("All");
    // 			break;
    // 		case 10:
    // 			lstFilterMoney.push_back("50k");
    // 			lstFilterMoney.push_back("100k");
    // 			lstFilterMoney.push_back("200k");
    // 			lstFilterMoney.push_back("All");
    // 			break;
    // 		default:
    // 			lstFilterMoney.push_back("100");
    // 			lstFilterMoney.push_back("500");
    // 			lstFilterMoney.push_back("2k");
    // 			lstFilterMoney.push_back("All");
    // 			break;
    // 		}
    // 	}
    // 	currFilterMoneyIndex = lstFilterMoney.size() - 1;
    // 	int _i = currFilterMoneyIndex;
    // 	//
    //
    // 	string m1 = lstFilterMoney[_i];
    // 	string m2 = ((_i + 1) == lstFilterMoney.size() ? ""
    // 		: lstFilterMoney[_i + 1]);
    // 	string m3 = (_i - 1) < 0 ? "" : lstFilterMoney[_i - 1];
    //
    // 	lblMoney1->setString((m1 != ""&&m1 != "All") ? *mUtils::format_money(atoi(m1.c_str())) : m1);
    // 	lblMoney2->setString((m2 != ""&&m2 != "All") ? *mUtils::format_money(atoi(m2.c_str())) : m2);
    // 	lblMoney3->setString((m3 != ""&&m3 != "All") ? *mUtils::format_money(atoi(m3.c_str())) : m3);
}

void ScenePickRoom::onScheduleRejoinRoom(float dt)
{
    RejoinInfoStruct sRejoinInfo = SceneManager::getSingleton().getRejoinInfo(); int a = -1;
    if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom()){
        a = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->Id();
    }
    //if (a == -1) return;
    boost::shared_ptr<long int> id2Left(new long int(a));
    if (sRejoinInfo.roomID != -1) {
        boost::shared_ptr<IRequest> request(new JoinRoomRequest(sRejoinInfo.roomID, sRejoinInfo.pass));
        GameServer::getSingleton().Send(request);
        return;
    }
    if (sRejoinInfo.roomName.length() > 0) {
        boost::shared_ptr<IRequest> request(new JoinRoomRequest(sRejoinInfo.roomName, sRejoinInfo.pass));
        GameServer::getSingleton().Send(request);
        return;
    }
}

void ScenePickRoom::onScheduleUpdateRoomList(float dt)
{
}

void ScenePickRoom::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell){
    
    
}
cocos2d::Size ScenePickRoom::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx){
    if (typePR == TableType::PlayTable)
        return Size(table->getContentSize().width, 120);
    else
    {
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
        {
            return Size(table->getContentSize().width, 120);
        }
        else{
            return Size(table->getContentSize().width, 200);
        }
    }
}
cocos2d::extension::TableViewCell* ScenePickRoom::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx){
    // 	bool isNew = getNewVersion();
    if (typePR == TableType::PlayTable)
        return this->createCellNewVersion(table, idx);
    else
        return this->createCellOldVersion(table, idx);
}

TableViewCell* ScenePickRoom::createCellOldVersion(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    int _cTag = 123;
    if (!cell) {
        cell = new TableViewCell();
        cell->setAnchorPoint(Vec2(0, 0));
        cell->autorelease();
        cell->setTag(_cTag);
        //
        int cols = 2;
        if (SceneManager::getSingleton().getGameID() == kGameChan)
            cols = 2;
        LayerPlayRoom* _child_ = LayerPlayRoom::create(getGameID());
        //
        Size visibleSize = Director::getInstance()->getVisibleSize();
        int width = visibleSize.width / cols;
        int heightItem = _child_->getContentSize().height;
        int startX = (visibleSize.width - _child_->getContentSize().width*cols) / 8;
        
        for (int i = 0; i < cols; i++){
            LayerPlayRoom* _child = LayerPlayRoom::create(getGameID());
            //_child->setPosition(Vec2(startX + (i*width), 0));
            _child->setPosition(Vec2(i * 358 + 220, 110));
            cell->addChild(_child, 0, _cTag + i);
        }
    }
    else
    {
    }
    //Change datas
    int col = 2;
    if (SceneManager::getSingleton().getGameID() == kGameChan)
        col = 2;
    int i = 0;
    for (i = 0; i < col; i++){
        int index = idx * col + i;
        if (index >= listRoomMain.size())
            break;
        LayerPlayRoom* _child = (LayerPlayRoom*)cell->getChildByTag(_cTag + i);
        if (_child){
            RoomInfo _info = listRoomMain.at(index);
            _child->setDataBet(_info.betvl, _info.type, _info.state);
            //_child->setDataMoney(_info._money, _info._type);
        }
    }
    
    return cell;
}
void ScenePickRoom::onButtonRoomScene(Ref* pSender)
{

   // return;
        double amf = 0;
        Button* _btn = (Button*)pSender;
        if (_btn != NULL) {
            int tag = _btn->getTag();
            RoomInfo _info = listRoomMain.at(tag);
            int nBetValue = _info.betvl;
            int roomType = SceneManager::getSingleton().getCurrRoomType();
            if (roomType == 0)
                amf = GameUtils::getAmfGoldUser(GameUtils::getMyName());
            else if (roomType == 1)
                amf = GameUtils::getAmfSilverUser(GameUtils::getMyName());
            
            double betUser = this->getMinBetByGame(getGameID(), amf);
            if (betUser < nBetValue) //khong du tien
            {
                
                if (SceneManager::getSingleton().getCurrRoomType() == 1){
                    auto _currScene = Director::getInstance()->getRunningScene();
                    if (!_currScene) return;
                    LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
                    popup->onBtnNapHao(NULL);
                    _currScene->addChild(popup,2000,9145);
                }
                else{
                    auto _currScene = Director::getInstance()->getRunningScene();
                    if (!_currScene) return;
                    SceneManager::getSingleton().showPopupNapTien(dataManager.GetSysString(965));
                }
                
                //            if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
                //            {
                //                auto _currScene = Director::getInstance()->getRunningScene();
                //                if (!_currScene) return;
                //                ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(tag_ScenePickRoom);
                //                LayerPopupHetTien* popup = LayerPopupHetTien::create();
                //                _scene->addChild(popup, ZORDER_POPUP_CHONTIEN);
                //
                //            }
                //            else{
                //                double ratioMoney = getRatioMinBetByGame(getGameID(), nBetValue - betUser);
                //                string strMoney = mUtils::format_money(ratioMoney)->c_str();
                //                auto _currScene = Director::getInstance()->getRunningScene();
                //                if (!_currScene) return;
                //                ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(tag_ScenePickRoom);
                //                _scene->onButtonCharge(NULL, ui::Widget::TouchEventType::ENDED);
                //                string mess = StringUtils::format("Tài khoản của bạn không đủ.\n Vui lòng nạp thêm %s %s để chơi tiếp !",
                //                    strMoney.c_str(), SceneManager::getSingleton().getCoinUnit().c_str());
                //                _scene->showToast(mess);
                //            }
                return;
            }
            
//            if (typePr == TableType::PlayTable)
//            {
            
                int _gameID = SceneManager::getSingleton().getGameID();
                if (_gameID == kGameXiTo || _gameID == kGamePoker){
                    vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
                    int bet_ratio = 1;
                    for (int i = 0; i < arrBet.size(); ++i)
                    {
                        if (arrBet.at(i).gameid == _gameID)
                        {
                            bet_ratio = arrBet.at(i).ratioBet;
                            break;
                        }
                    }
                    
                    XiToPopupChonTien* chontien = XiToPopupChonTien::create();
                    chontien->setMinMaxValue(nBetValue, bet_ratio*nBetValue);
                    auto _currScene = Director::getInstance()->getRunningScene();
                    if (!_currScene) return;
                    //Casi cux;
//                    _currScene->addChild(chontien, ZORDER_POPUP_RANK,9146);
                    _currScene->addChild(chontien, ZORDER_POPUP_RANK,91460);
                }
                else if (_gameID == kGameLieng){
                    vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
                    int bet_ratio = 1;
                    for (int i = 0; i < arrBet.size(); ++i)
                    {
                        if (arrBet.at(i).gameid == _gameID)
                        {
                            bet_ratio = arrBet.at(i).ratioBet;
                            break;
                        }
                    }
                    
                    LayerPopupChonTienLieng* chontien = LayerPopupChonTienLieng::create();
                    chontien->setMinMaxValue(nBetValue, bet_ratio*nBetValue);
                    auto _currScene = Director::getInstance()->getRunningScene();
                    if (!_currScene) return;
                    _currScene->addChild(chontien, ZORDER_POPUP_RANK,9147);
                }
                else{
                    int roomType = SceneManager::getSingleton().getCurrRoomType();
                    boost::shared_ptr<ISFSObject> param(new SFSObject());
                    param->PutInt(EXT_FIELD_ROOM_TYPE, roomType);
                    param->PutInt(AMOUNT_BET, nBetValue);
                    param->PutUtfString(GAME_ID, StringUtils::format("%d", _gameID));
                    boost::shared_ptr<IRequest> request(new ExtensionRequest(REQUEST_AUTO_JOIN_ROOM, param));
                    GameServer::getSingleton().Send(request);
                    SceneMain::getSingleton().prepareToPickGame();

                }
                
            //}//
        
        
    }
}
void ScenePickRoom::createScrollViewNew(){
    this->scrollView->setInnerContainerSize(Size(1190.00,(listRoomMain.size() / 4+2)*153));
    this->scrollView->removeAllChildren();
    //this->scrollView->setScrollBarColor(Color3B::WHITE);
    this->scrollView->setScrollBarAutoHideEnabled(true);
    this->scrollView->scrollToTop(1, false);
    int cols = 4;
    for (int i =0;i<this->listRoomMain.size();i++){
        LayerPlayRoom* _child = LayerPlayRoom::create(getGameID());
        _child->setAnchorPoint(Vec2(0,1));
        _child->setPosition(Vec2(20 + (i%cols)*257 + 20*(i%cols),(listRoomMain.size() / 4+2)*153 - 165* (i/cols+1)-15));
        _child->setVisible(true);
        int index =  i;
        RoomInfo _info = listRoomMain.at(index);
        _child->setDataRoom(_info.betvl, _info.usercount);
        Button* buttonRoom= Button::create();
        buttonRoom->loadTextureNormal("bg-table-trans.png");
        buttonRoom->loadTextureDisabled("bg-table-trans.png");
        //buttonRoom->set
        buttonRoom->setAnchorPoint(Vec2::ZERO);
        buttonRoom->setPosition(Vec2(20 + (i%cols)*257 + 20*(i%cols),(listRoomMain.size() / 4+2)*153 - 165 * (i/cols+1)-15));
        buttonRoom->setContentSize(Size(257,153));
        buttonRoom->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonRoomScene, this));
        buttonRoom->setTag(index);
        buttonRoom->setEnabled(true);
        //buttonRoom->setSwallowTouches(false);
        this->scrollView->addChild(_child);
        
        this->scrollView->addChild(buttonRoom);
        
    }
}
void ScenePickRoom::createScrollView(){
    if (SceneManager::getSingleton().isNagaWin){
        this->createScrollViewNew();
        return;
    }
    if (SceneManager::getSingleton().isNagaNew){
        this->scrollView->setInnerContainerSize(Size(734,(listRoomMain.size() / 3+2)*118));
        this->scrollView->removeAllChildren();
        //this->scrollView->setScrollBarColor(Color3B::WHITE);
        this->scrollView->setScrollBarAutoHideEnabled(true);
        this->scrollView->scrollToTop(1, false);
        int cols = 3;
        for (int i =0;i<this->listRoomMain.size();i++){
            LayerPlayRoom* _child = LayerPlayRoom::create(getGameID());
            _child->setAnchorPoint(Vec2(0,1));
            _child->setPosition(Vec2(20 + (i%cols)*206 + 80*(i%cols),(listRoomMain.size() / 3+2)*118 -100 * (i/cols+1)-5));
            _child->setVisible(true);
            int index =  i;
            RoomInfo _info = listRoomMain.at(index);
            _child->setDataRoom(_info.betvl, _info.usercount);
            Button* buttonRoom= Button::create();
            buttonRoom->loadTextureNormal("bg-table-trans.png");
            buttonRoom->loadTextureDisabled("bg-table-trans.png");
            //buttonRoom->set
            buttonRoom->setAnchorPoint(Vec2::ZERO);
            buttonRoom->setPosition(Vec2(20 + (i%cols)*206 + 80*(i%cols),(listRoomMain.size() / 3+2)*118 - 100 * (i/cols+1)-5));
            buttonRoom->setContentSize(Size(226,118));
            buttonRoom->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonRoomScene, this));
            buttonRoom->setTag(index);
            buttonRoom->setEnabled(true);
            //buttonRoom->setSwallowTouches(false);
            this->scrollView->addChild(_child);
            
            this->scrollView->addChild(buttonRoom);
            
        }
        return;
    }
    
    this->scrollView->setInnerContainerSize(Size(734,(listRoomMain.size() / 3+2)*118));
    this->scrollView->removeAllChildren();
    //this->scrollView->setScrollBarColor(Color3B::WHITE);
    this->scrollView->setScrollBarAutoHideEnabled(true);
    this->scrollView->scrollToTop(1, false);
    int cols = 3;
    for (int i =0;i<this->listRoomMain.size();i++){
        LayerPlayRoom* _child = LayerPlayRoom::create(getGameID());
        _child->setAnchorPoint(Vec2(0,1));
        _child->setPosition(Vec2(20 + (i%cols)*206 + 100*(i%cols),(listRoomMain.size() / 3+2)*118 -128 * (i/cols+1)-5));
        _child->setVisible(true);
        int index =  i;
        RoomInfo _info = listRoomMain.at(index);
        _child->setDataRoom(_info.betvl, _info.usercount);
        Button* buttonRoom= Button::create();
        buttonRoom->loadTextureNormal("bg-table-trans.png");
        buttonRoom->loadTextureDisabled("bg-table-trans.png");
        //buttonRoom->set
        buttonRoom->setAnchorPoint(Vec2::ZERO);
        buttonRoom->setPosition(Vec2(20 + (i%cols)*206 + 100*(i%cols),(listRoomMain.size() / 3+2)*118 -128 * (i/cols+1)-5));
        buttonRoom->setContentSize(Size(226,118));
        buttonRoom->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonRoomScene, this));
        buttonRoom->setTag(index);
        buttonRoom->setEnabled(true);
        //buttonRoom->setSwallowTouches(false);
        this->scrollView->addChild(_child);
        this->scrollView->addChild(buttonRoom);
    }
    
}
TableViewCell* ScenePickRoom::createCellNewVersion(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    int _cTag = 123;
    if (!cell) {
        cell = new TableViewCell();
        cell->setAnchorPoint(Vec2(0, 0));
        cell->autorelease();
        cell->setTag(_cTag);
        //
        int x = 40;
        int cols = 3;
        if (SceneManager::getSingleton().getGameID() == kGameChan){
            cols = 3;
            x = 50;
        }
        LayerPlayRoom* _child_ = LayerPlayRoom::create(getGameID());
        for (int i = 0; i < cols; i++){
            LayerPlayRoom* _child = LayerPlayRoom::create(getGameID());
            _child->setPosition(Vec2(i * 266 + 10 + x*i, 0));
            _child->setVisible(false);
            cell->addChild(_child, 0, _cTag + i);
            
            int index = idx * cols + i;
         

            Button* buttonRoom= Button::create();
            buttonRoom->loadTextureNormal("bg-table-trans.png");
            buttonRoom->loadTextureDisabled("bg-table-trans.png");
            buttonRoom->setAnchorPoint(Vec2::ZERO);
            buttonRoom->setPosition(Vec2(i * 266 + 10 + x*i, 0));
            buttonRoom->setContentSize(Size(266,118));
            buttonRoom->addClickEventListener(CC_CALLBACK_1(ScenePickRoom::onButtonRoomScene, this));
            buttonRoom->setTag(index);
            buttonRoom->setEnabled(true);
            buttonRoom->setSwallowTouches(false);
            cell->addChild(buttonRoom);
            
        }
    }
    else
    {
    }
    //Change datas
    int i = 0;
    int cols = 3;
    if (SceneManager::getSingleton().getGameID() == kGameChan)
        cols = 3;
    string betstr = "All";
    if (lstFilterMoney.size() > 0)
        betstr = lstFilterMoney.at(currFilterMoneyIndex);
    int betvl = -1;
    if (0 != betstr.compare("All"))
        betvl = atoi(betstr.c_str());
    for (i = 0; i < cols; i++){
        int index = idx * cols + i;
        if (index >= listRoomMain.size())
            break;
        LayerPlayRoom* _child = (LayerPlayRoom*)cell->getChildByTag(_cTag + i);
        if (_child){
            RoomInfo _info = listRoomMain.at(index);
            if (_info.betvl != betvl && betvl != -1)
                _child->setVisible(false);
            else
            {
                _child->setVisible(true);
                _child->setDataRoom(_info.betvl, _info.usercount);
            }
        }
    }
    
    for (int j = i; j < cols; j++){
        LayerPlayRoom* _child = (LayerPlayRoom*)cell->getChildByTag(_cTag + j);
        if (_child){
            _child->setVisible(false);
        }
    }
    return cell;
}
ssize_t ScenePickRoom::numberOfCellsInTableView(cocos2d::extension::TableView *table){
    if (listRoomMain.size() == 0)
        return 0;
    if (listRoomMain.size() < 3)
        return 1;
    int rows = listRoomMain.size() / 3;
    if (listRoomMain.size() % 3 != 0)
        rows += 1;
    return rows;
}

void ScenePickRoom::pickerChangeHandler(cocos2d::Node *pnode)
{
    //	auto picker = dynamic_cast<UINumberPicker*>(pnode);
    //	int tag_cell = picker->getSelectedItem()->getTag();
    //	log("TAG = %d", tag_cell);
    //	if (tag_cell != -1)
    //	{
    //		currFilterMoneyIndex = tag_cell;
    //		//neu la all thi request - con cac muc cuoc khac tu loc duoi client
    //		if (lstFilterMoney.size() < currFilterMoneyIndex)
    //			return;
    //		string betvl = lstFilterMoney.at(currFilterMoneyIndex);
    //		onButtonFilterRoom(betvl);
    //	}
}

void ScenePickRoom::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
        unSubscribeGame();
        SceneManager::getSingleton().gotoMain();
    }
}

void ScenePickRoom::onButtonCharge(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        /*LayerPopupRecharge* _child = LayerPopupRecharge::create();
         _child->setLocalZOrder(zorder_layer_nap_tien);
         this->addChild(_child, zorder_layer_nap_tien, tag_layer_nap_tien);*/
        LayerPopupCuaHang* _popupCuaHang = LayerPopupCuaHang::create();
        _popupCuaHang->setLocalZOrder(zorder_layer_nap_tien);
        _popupCuaHang->onBtnNapThe(NULL);
        this->addChild(_popupCuaHang, zorder_layer_nap_tien, tag_layer_nap_tien);
    }
}

void ScenePickRoom::onButtonSearchRoom(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        LayerPopupSearchRoom *_layer = LayerPopupSearchRoom::create();
        this->addChild(_layer, tag_layer_find_room, tag_layer_find_room);
    }
}

void ScenePickRoom::onButtonUserInfo(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        LayerPopupProfile* profile = LayerPopupProfile::create();
        this->getParent()->addChild(profile, tag_layer_avatar, tag_layer_avatar);
    }
}

void ScenePickRoom::onButtonPlayNow(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        int _gameID = SceneManager::getSingleton().getGameID();
        SceneMain::getSingleton().currTypeMoneyGo = this->roomType;
        //		if (_gameID == kGameXiTo){
        //			XiToPopupChonTien* chontien = XiToPopupChonTien::create();
        //
        //			auto _currScene = Director::getInstance()->getRunningScene();
        //			if (!_currScene) return;
        //			_currScene->addChild(chontien, 10000000);
        //			chontien->setJoinType(1);
        //
        //		}
        //		else{
        boost::shared_ptr<ISFSObject> param(new SFSObject());
        param->PutInt(AMOUNT_BET, -1);
        param->PutUtfString(GAME_ID, StringUtils::format("%d", _gameID));
        param->PutInt(EXT_FIELD_ROOM_TYPE, this->roomType);
        boost::shared_ptr<IRequest> request(new ExtensionRequest(REQUEST_AUTO_JOIN_ROOM, param));
        GameServer::getSingleton().Send(request);
        SceneMain::getSingleton().prepareToPickGame();

        //		}
    }
}

void ScenePickRoom::scheduleUpdateInfo(float dt)
{
    loadDataInfo();
}

void ScenePickRoom::loadDataInfo()
{
    return;
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    if (mySelf == NULL) return;
    if (mySelf->GetVariable("am") != NULL && mySelf->GetVariable("amf") != NULL && mySelf->GetVariable("amfs"))
    {
        string myName = SceneManager::getSingleton().getMyName();
        try{
            boost::shared_ptr<string> am = mySelf->GetVariable("am")->GetStringValue();
            boost::shared_ptr<double> amf = mySelf->GetVariable("amf")->GetDoubleValue();
            boost::shared_ptr<double> amfs = mySelf->GetVariable("amfs")->GetDoubleValue();
            boost::shared_ptr<string> aal = mySelf->GetVariable("aal")->GetStringValue();
            boost::shared_ptr<long> _exp = mySelf->GetVariable("exp")->GetIntValue();
            boost::shared_ptr<long> acctype = mySelf->GetVariable("accVipType")->GetIntValue();
            boost::shared_ptr<UserVariable> aN = mySelf->GetVariable("aN");
            boost::shared_ptr<long> lvl = mySelf->GetVariable("lvl")->GetIntValue();
            
            if (aN != NULL)
            {
                myName = aN->GetStringValue()->c_str();
            }
            avtVip->loadTexture(mUtils::getSrcVip((int)*acctype));
            lblMoney->setString(mUtils::convertMoneyEx((int)*amf));
            lblLevel->setString(StringUtils::format("Level %d", (int)*lvl));
            prgLevelProgress->setPercent(dataManager.getLevelProgress((int)*lvl, (int)*_exp));
            circleAvatar->setAvatarImage(aal->c_str());
        }
        catch (std::exception const &e)
        {
            
        }
        std::string _myName = "";
        (myName.length() > 15) ? _myName = myName.substr(0, 15) + "..." : _myName = myName;
        lblName->setString(_myName);
    }
}

void ScenePickRoom::showToast(string mess)
{
    Chat *toast = Chat::create(mess, -1);
    toast->setTag(tag_layer_toast);
    this->addChild(toast, ZORDER_LIST::ZORDER_TOAST_PICKROOM);
}

void ScenePickRoom::onEnter()
{
    Layer::onEnter();
    SceneManager::getSingleton().showLoading();
    GameServer::getSingleton().addListeners(this);
    
    bool isKickRoom = SceneManager::getSingleton().getKickRoom();
    bool isKickNem = SceneManager::getSingleton().getKickNem();
    if (isKickRoom)
    {
        SceneManager::getSingleton().setKickRoom(false);
        this->showToast(dataManager.GetSysString(405));
    }
    if (isKickNem)
    {
        SceneManager::getSingleton().setKickNem(false);
        if (SceneManager::getSingleton().getCurrRoomType() == 1){
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
            popup->onBtnNapHao(NULL);
            _currScene->addChild(popup,2000,9148);
        }
        else{
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
            popup->onBtnNapThe(NULL);
            _currScene->addChild(popup,2000,9149);
        }
        this->showToast(dataManager.GetSysString(406));
    }
}

void ScenePickRoom::onExit()
{
    Layer::onExit();
    
    //SceneManager::getSingleton().setTimeOutLoading(5);
}

void ScenePickRoom::createListData()
{
    if (typePR == TableType::BetTable)
    {
        listRoomMain.clear();
        vector<strGameCF> arrconfig = SceneManager::getSingleton().getArrConfig();
        int gameID = SceneManager::getSingleton().getGameID();
        string str_listcuoc;
        int ratio_bet = 1;//muc cuoc tung game
        int default_hide = 0;
        for (int i = 0; i < arrconfig.size(); ++i)
        {
            if (arrconfig.at(i).gameid == gameID)
            {
                str_listcuoc = arrconfig.at(i).list_cuoc;
                default_hide = arrconfig.at(i).default_hide;
                ratio_bet = arrconfig.at(i).ratioBet;
                break;
            }
        }
        //so tien nguoi choi
        double amf = 0;
        if (SceneManager::getSingleton().getCurrRoomType() == 0){
            boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
            if (mySelf != NULL)
            {
                boost::shared_ptr<UserVariable> amf_ptr = mySelf->GetVariable("amf");
                if (amf_ptr != NULL)
                {
                    amf = *(amf_ptr->GetDoubleValue());
                }
            }
        }
        else{
            boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
            if (mySelf != NULL)
            {
                boost::shared_ptr<UserVariable> amf_ptr = mySelf->GetVariable("amfs");
                if (amf_ptr != NULL)
                {
                    amf = *(amf_ptr->GetDoubleValue());
                }
            }
        }
        
        double betUser = amf / ratio_bet;
        vector<string> arrstr_listcuoc = mUtils::splitString(str_listcuoc, ',');
        int total_show = 0;
        for (int i = 0; i < arrstr_listcuoc.size(); ++i)
        {
            double bet_compare = atol(arrstr_listcuoc.at(i).c_str());
            if (betUser >= bet_compare)
                total_show++;
        }
        for (int i = 0; i < arrstr_listcuoc.size(); ++i)
        {
            double bet = atol(arrstr_listcuoc.at(i).c_str());//tien cuoc
            int type = 0;
            if (i >4 && i <= 8)
                type = 1;
            else if (i > 8 && i < 11)
                type = 2;
            else if (i >= 11)
                type = 3;
            bool _state = getStateByMoney(total_show, i, default_hide);
            RoomInfo gcf = RoomInfo(bet, type, _state);
            listRoomMain.push_back(gcf);
        }
        
        //tblPlayRooms->reloadData();
        //this->createScrollView();
    }
}

void ScenePickRoom::onButtonRank(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if (dataManager.getAppConfig().isHideBilling){
            Chat* toast = Chat::create(dataManager.GetSysString(437), -1);
            this->addChild(toast);
            return;
        }
        if (isWebView){
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            string strID = StringUtils::format("%d", getGameID());
            string strlink = RANK_PICKROOM + strID;
            
            LayerPopupWebView* _layer = LayerPopupWebView::create();
            _layer->setWebView(strlink);
            _currScene->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_WEBVIEW,9150);
            
        }
        else{
            Size sizeAdd = ChanUtils::getSizePos();
            LayerPopupRankGame* layerPopUpRank = LayerPopupRankGame::create();
            layerPopUpRank->setPositionY(layerPopUpRank->getPositionY() + sizeAdd.height * 2);
            this->addChild(layerPopUpRank);
        }
    }
}

bool ScenePickRoom::getStateByMoney(int total_show, int index, int default_hide)
{
    //amf so tien nguoi choi, ratio: ti le cuoc tung game, total_show: tong so muc cuoc duoc hien
    //index vi tri cua so tien nay trong mang tien cuoc
    //so ban config an? - default = 0
    
    int show_value = 0;
    if (total_show <= 4)//so tien qua it
    {
        if (index <= 3)//chi hien thi 4 o dau tien
            return true;
        else
            return false;
    }
    else
    {
        //gia tri duoc hien se tu default_hide den total show
        if (index >= default_hide && index <= total_show - 1)
            return true;
        else
            return false;
    }
}

double ScenePickRoom::getMinBetByGame(int gameID, double amf)
{
    double betUser = 0;
    vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
    int bet_ratio = 1;
    for (int i = 0; i < arrBet.size(); ++i)
    {
        if (arrBet.at(i).gameid == gameID)
        {
            bet_ratio = arrBet.at(i).ratioBet;
            break;
        }
    }
    betUser = amf / bet_ratio;
    return betUser;
}

std::string ScenePickRoom::getGameNameByID(int id)
{
    switch (id) {
        case 100:
            return dataManager.GetSysString(168);
        case 101:
            return dataManager.GetSysString(169);
        case 102:
            return dataManager.GetSysString(170);
        case 103:
            return dataManager.GetSysString(171);
        case 104:
            return dataManager.GetSysString(172);
        case 105:
            return dataManager.GetSysString(173);
        case 107:
            return dataManager.GetSysString(174);
        case 108:
            return dataManager.GetSysString(175);
        case 109:
            return dataManager.GetSysString(176);
        case 106:
            return dataManager.GetSysString(177);
        case 110:
            return dataManager.GetSysString(1204);
        case 111:
            return dataManager.GetSysString(1206);
        case 112:
            return dataManager.GetSysString(1205);
        case 113:
            return dataManager.GetSysString(1207);
        case 114:
            return dataManager.GetSysString(1260);
        case 115:
            return dataManager.GetSysString(1261);
        case kGameDemLaSolo:
            return dataManager.GetSysString(1257);
        case kGameSamSolo:
            return dataManager.GetSysString(1258);
        case kGamePoker:
            return dataManager.GetSysString(885);
        case kGameMauBinh:
            return dataManager.GetSysString(932);
        case kGameCatte:
            return dataManager.GetSysString(915);
        case kGame3Doi:
            return dataManager.GetSysString(916);
        default:
            break;
    }
    return StringUtils::format("Unknown gid: %d", id);
}

void ScenePickRoom::onButtonFilterPicker()
{
    //if (arrBet.size() == 0)
    //	return;
    //	SceneManager::getSingleton().showLoading();
    //	int jumpArr = arrBet.size() / 3;//3 muc
    //	try{
    //		lstFilterMoney.clear();
    //		for (int i = currFilterBet * jumpArr; i < jumpArr * (currFilterBet + 1); i++)
    //		{
    //			lstFilterMoney.push_back(arrBet.at(i));
    //		}
    //		for (int i = 0; i < this->lstFilterMoney.size(); i++) {
    //
    //			Text* label = (Text*)(this->Image_24->getChildByTag(1000 + i));
    //			if (label) {
    //				string str = mUtils::formatMoneySymbol(atof(this->lstFilterMoney[i].c_str()));
    //				if (i == this->lstFilterMoney.size() - 1)
    //					str = this->lstFilterMoney[i];
    //				label->setString(str);
    //			}
    //		}
    //		this->onButtonFilterMoney(this->lblMoneyAll);
    //	}
    //	catch (...)
    //	{
    //		log("EXCEPTION");
    //	}
}

void ScenePickRoom::onButtonFilterRoom(string betvl)
{
    int sizeFilter = lstFilterMoney.size();
   // log("onButtonFilterRoom  = %s size filter = %d", betvl.c_str(), lstFilterMoney.size());
    // 	if (lstFilterMoney.size() == 0)
    // 		return;
    if (0 == betvl.compare("All_All") || lstFilterMoney.size() == 0)
    {
        int gameID = getGameID();
        boost::shared_ptr<SFSObject> param(new SFSObject());
        // 		param->PutInt(EXT_FIELD_BET_FROM, NULL);
        // 		param->PutInt(EXT_FIELD_BET_TO, NULL);
        //param->PutUtfString(EXT_FIELD_GAME_ID, boost::to_string(gameID));
        param->PutUtfString(EXT_FIELD_GAME_ID, StringUtils::format("%d", gameID));
        param->PutInt(EXT_FIELD_ROOM_TYPE, this->roomType);
        boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_GET_LIST_ROOM, param));
        GameServer::getSingleton().Send(request);
    }
    else if (0 == betvl.compare("All"))
    {
        int gameID = getGameID();
        int betf = atoi(lstFilterMoney.at(0).c_str());
        int bett = atoi(lstFilterMoney.at(lstFilterMoney.size() - 2).c_str());
        boost::shared_ptr<SFSObject> param(new SFSObject());
        param->PutInt(EXT_FIELD_BET_FROM, betf);
        param->PutInt(EXT_FIELD_BET_TO, bett);
        param->PutInt(EXT_FIELD_ROOM_TYPE, this->roomType);
        param->PutUtfString(EXT_FIELD_GAME_ID, StringUtils::format("%d", gameID));
        boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_GET_LIST_ROOM, param));
        GameServer::getSingleton().Send(request);
        //SceneManager::getSingleton().hideLoading();
    }
    else{
        if (listRoom.size() == 0)
            return;
        listRoomMain.clear();
        listRoomMain = listRoom;
        int betvl_int = atoi(betvl.c_str());
        int q = 0;
        while (true)
        {
            if (q >= listRoomMain.size())
                break;
            if (listRoomMain.at(q).betvl != betvl_int)
                listRoomMain.erase(listRoomMain.begin() + q);
            else
                q++;
        }
        // 		for (int i = 0; i < listRoom.size(); i++)
        // 		{
        // 			if (listRoom.at(i).betvl != betvl_int)//xoa cac muc khong dung gia tri cuoc dang chon
        // 				listRoom.erase(listRoom.begin() + i);
        // 		}
        //tblPlayRooms->reloadData();
        //this->createScrollView();
        SceneManager::getSingleton().hideLoading();
    }
    
}

void ScenePickRoom::createLabelThongBao(string _str)
{
    Text* _lblThongBao = dynamic_cast<Text*>(pnlThongBao->getChildByName("lblThongBao"));
    lblThongBao = Label::create(_str, kBoldFont, 40);
    lblThongBao->setAnchorPoint(_lblThongBao->getAnchorPoint());
    Vec2 pos = _lblThongBao->getPosition();
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
        pos = Vec2(_lblThongBao->getPositionX(), _lblThongBao->getPositionY() - 10);
    lblThongBao->setPosition(pos);
    lblThongBao->setVisible(!dataManager.getAppConfig().isHideBilling);
    pnlThongBao->addChild(lblThongBao);
}

void ScenePickRoom::onScheduleThongBao(float dt)
{
    if (gListNotification.size() == 0)
    {
        gListNotification = SceneManager::getSingleton().gListNotificationPickRoom;
    }
    else{
        linkPopUp = gListNotification.at(0).linkShow;
    }
    
    int x = lblThongBao->getPositionX();
    if (x < -1 * lblThongBao->getContentSize().width || lblThongBao->getString().compare("   ") == 0) {
        x = pnlThongBao->getContentSize().width;
        //
        currIndexNotification++;
        if (currIndexNotification >= gListNotification.size())
            currIndexNotification = 0;
        string _strMsg = "   ";
        if (gListNotification.size() > 0)
            _strMsg = gListNotification[currIndexNotification].textShow;
        //
        //_strMsg = "Chào mừng bạn tới baifvip. Chúc bạn chơi game vui vẻ!";
        //txtThongBao = _strMsg;
        createLabelThongBao(_strMsg);
    }
    else
        x = lblThongBao->getPositionX() - 5;
    lblThongBao->setPositionX(x);
}

void ScenePickRoom::onButtonThongBao(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if (dataManager.getAppConfig().isHideBilling)return;
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return;
        
        if (0 != linkPopUp.compare(""))
        {
            LayerPopupWebView* _layer = LayerPopupWebView::create();
            _layer->setWebView(linkPopUp);
            _currScene->addChild(_layer, ZORDER_LIST::ZORDER_POPUP_WEBVIEW,9151);
        }
    }
}

void ScenePickRoom::getNotificationFromUrl()
{
    if (dataManager.getAppConfig().url_notification == "")
        return;
    int gameid = getGameID();
    string url_notif = dataManager.getAppConfig().url_notification;
    string urlNotifcation = StringUtils::format("%s?gameid=%d", url_notif.c_str(), gameid);
    if (url_notif.find('new') != std::string::npos) //contain string '-'
        urlNotifcation = StringUtils::format("%sgameid=%d", "https://gamebai.net/notifyServices?", gameid);
    
    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(urlNotifcation.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setTag("notification");
    request->setResponseCallback(CC_CALLBACK_2(ScenePickRoom::onHttpRequestCompleted, this));
    HttpClient::getInstance()->send(request);
    request->release();
    isDestroy = false;
}

void ScenePickRoom::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
   // log("SceneManager::onHttpRequestCompleted");
    if (!response)
    {
        return;
    }
    isDestroy = true;
    
    long statusCode = response->getResponseCode();
    char statusString[64] = {};
    
    std::vector<char> *buffer = response->getResponseData();
   // log("datas receive from [%s]: %s", response->getHttpRequest()->getTag(), buffer[0].data());
    if (strcmp(response->getHttpRequest()->getTag(), "notification") == 0){
        std::vector<char>* buffer = response->getResponseData();
        string datas = buffer[0].data();
        //  datas = decryptBytes(datas);
        unsigned vt1 = datas.find_first_of('{');
        unsigned vt = datas.find_last_of('}');
        if (vt != -1 && vt1 != -1)
            datas = datas.substr(vt1, vt - vt1 + 1);
        datas = mUtils::replaceAll(datas, "\r", "");
        datas = mUtils::replaceAll(datas, "\n", "");
        //
        rapidjson::Document readdoc;
        bool bRet = false;
        ssize_t size = 0;
        std::string load_str = datas;
        readdoc.Parse<0>(load_str.c_str());
        //log(datas.c_str());
        if (readdoc.HasParseError())
        {
            return;
        }
        if (readdoc.IsObject())
        {
            string textShow = "";
            string linkShow = "";
            string textPop = "";
            string linkPop = "";
            bool isShow = false;
            gListNotification.clear();
            if (readdoc.HasMember("text"))
            {
                textShow = readdoc["text"].GetString();
            }
            if (readdoc.HasMember("link"))
            {
                linkShow = readdoc["link"].GetString();
            }
            NotificationInfo notifiInfo;
            notifiInfo.textShow = textShow;
            notifiInfo.linkShow = linkShow;
            linkPopUp = linkShow;
            notifiInfo.isShow = isShow;
            gListNotification.push_back(notifiInfo);
        }
        
    }
}

void ScenePickRoom::setGListNotification(vector<NotificationInfo> notifi)
{
    gListNotification = notifi;
    linkPopUp = gListNotification.at(0).linkShow;
}

void ScenePickRoom::onButtonTypeRoom(Ref* pSender)
{
    if (this->roomType == 1)
    {
        this->btnMoneyGold->setVisible(true);
        this->btnMoneySilver->setVisible(false);

        this->roomType = 0;
        if (!dataManager.getAppConfig().isHideBilling){
            
            this->txtXH->setString(dataManager.GetSysString(980));
        }
        
    }
    else
    {
        this->btnMoneyGold->setVisible(false);
        this->btnMoneySilver->setVisible(true);
        this->roomType = 1;
        if (!dataManager.getAppConfig().isHideBilling){
            
            this->txtXH->setString(dataManager.GetSysString(979));
        }
        
    }
    this->bxh->loadTop(SceneManager::getSingleton().getGameID(),this->roomType);
    
    SceneManager::getSingleton().setCurrRoomType(this->roomType);
    this->sendListBetRequest();
    // this->showWebView();
}

void ScenePickRoom::sendListBetRequest()
{
    listRoom.clear();
    //tblPlayRooms->reloadData();
    this->createScrollView();
    int gameID = getGameID();
    boost::shared_ptr<SFSObject> param(new SFSObject());
    param->PutInt(EXT_FIELD_ROOM_TYPE, this->roomType);
    param->PutUtfString(EXT_FIELD_GAME_ID, StringUtils::format("%d", gameID));
    //param->PutInt(EXT_FIELD_GAME_ID, gameID);
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_GET_LIST_BET, param));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
}

void ScenePickRoom::onButtonFilterMoney(Ref* pSender)
{
    for (int i = 0; i < this->lstFilterMoney.size(); i++) {
        Text* label = (Text*)this->Image_24->getChildByTag(1000 + i);
        if (label) {
            label->setColor(Color3B::WHITE);
        }
    }
    ((Text*)pSender)->setColor(Color3B::YELLOW);
    this->spriteArrow->setPositionX(((Text*)pSender)->getPositionX());
    int _tag = ((Text*)pSender)->getTag();
    int idx = _tag - 1000;
    this->currFilterMoneyIndex = idx;
    string betvl = this->lstFilterMoney[idx];
    this->onButtonFilterRoom(betvl);
}


/************************************************************************/
/* Layer Join Room With Password                                                                     */
/************************************************************************/
LayerPopupJoinRoomPass::LayerPopupJoinRoomPass()
{
    mCallBack = NULL;
    mCallBackListener = NULL;
}

LayerPopupJoinRoomPass::~LayerPopupJoinRoomPass()
{
    
}

bool LayerPopupJoinRoomPass::init()
{
    if (!Layer::init())
        return false;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupJoinRoomPass.csb");
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    txtPassRoom = dynamic_cast<TextField*>(rootNode->getChildByName("txtPassRoom"));
    
    //add event listenner
    auto btnCancel = dynamic_cast<Button*>(rootNode->getChildByName("btnCancel"));
    if (btnCancel != NULL){
        btnCancel->addTouchEventListener(CC_CALLBACK_2(LayerPopupJoinRoomPass::onButtonCancelClicked, this));
        btnCancel->setPressedActionEnabled(true);
    }
    
    auto btnJoinRoom = dynamic_cast<Button*>(rootNode->getChildByName("btnJoinRoom"));
    if (btnJoinRoom != NULL){
        btnJoinRoom->addTouchEventListener(CC_CALLBACK_2(LayerPopupJoinRoomPass::onButtonJoinRoomClicked, this));
        btnJoinRoom->setPressedActionEnabled(true);
    }
    return true;
}

void LayerPopupJoinRoomPass::setRoomID(int roomID)
{
    this->roomID = roomID;
}

void LayerPopupJoinRoomPass::onButtonCancelClicked(Ref* pSender, Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        this->removeFromParentAndCleanup(true);
    }
}

void LayerPopupJoinRoomPass::onButtonJoinRoomClicked(Ref* pSender, Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
    }
}

void LayerPopupJoinRoomPass::onPassWordRoomCallBack(Ref *target, SEL_CallFunc listener)
{
    mCallBackListener = listener;
    mCallBack = target;
}

void LayerPopupJoinRoomPass::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupJoinRoomPass::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void LayerPopupJoinRoomPass::onExit()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    Layer::onExit();
}

bool LayerPopupJoinRoomPass::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}


