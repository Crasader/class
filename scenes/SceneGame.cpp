#include "SceneGame.h"

#include "layergames/LayerPlayGameTLMN.h"
#include "layergames/LayerPlayGamePhom.h"
#include "layergames/LayerGameTaiXiu.h"
#include "../layergames/LayerLauncherMiniGame.h"
#include "layergames/LayerChanGame.h"
#include "layergames/Sam.h"
#include "layergames/LayerChanGame.h"
#include "layergames/XiTo.h"
#include "layergames/ChanUtils.h"
#include "../layergames/LayerGameTaiXiuMini.h"
#include "../layergames/LayerMenuMiniGame.h"
#include "layergames/LayerPlayGameBaCay.h"
#include "../SceneManager.h"
#include "../layers/LayerHuThuong.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
#include "layergames/QuayThuong.h"
#include "layergames/LayerGameXocDia.h"
#include "layergames/PhomMessDef.h"
#include "layergames/LayerGameLieng.h"
#include "layergames/LayerGameMauBinh.h"
#include "layergames/PokerTexasGame.h"
#include "cotuong/LayerMainGame.h"
#include "bautom/LayerGameBauTom.h"
#include "layergames/LayerPlaySlotMachine.h"
#include "../layergames/LayerPlayGameCatte.h"
#include "../layergames/LayerPlayGame3Doi.h"
#include "Requests/LeaveRoomRequest.h"
#include "Requests/UnsubscribeRoomGroupRequest.h"
SceneGame::SceneGame()
{
    SceneManager::getSingleton().isInGame = true;
}


SceneGame::~SceneGame()
{
    GameServer::getSingleton().removeListeners(this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneGame::timeoutChatMaxtime),this);
    SceneManager::getSingleton().isInGame = false;

}

SceneGame* SceneGame::createScene(int typeGame)
{
    //auto scene = Scene::create();
    
    auto layer = SceneGame::create();
    layer->loadGameByTag(typeGame);
    
    //scene->addChild(layer);
    
    return layer;
}

bool SceneGame::init()
{
    if (!Layer::init())
    {
        return false;
    }
    //if (mUtils::isSoundOn()){
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    //}
    inviteCount = 0;
    //MINHDV -changes - get last betvalue room
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom)
    {
        // thông tin bàn chơi và mức cược
        boost::shared_ptr<RoomVariable> param = lastRoom->GetVariable("params");
        if (param != NULL){
            string paramString = param->GetStringValue()->c_str();
            vector<string> arrInfo = mUtils::splitString(paramString, '@');
            string money = arrInfo.at(0);
            double money_bet = atol(money.c_str());
            SceneManager::getSingleton().setBetValue(money_bet);
        }
    }
    //MINHDV CHANGES - 27/6 - set one more uck variable for user
    //set user chicken variable
    UserDefault *def = UserDefault::getInstance();
    bool uck = def->getBoolForKey("UCK", true);
    setUckVariable(uck);
    
    this->setKeyboardEnabled(false);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Size sizeAdd;
    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
    }
    else{
        sizeAdd = Size(0, 0);
    }
    
    
   
    
    GameServer::getSingleton().addListeners(this);
    this->autoInviteRoom();
    if (lastRoom != NULL)
    {
        if (0 == lastRoom->GroupId()->compare(boost::to_string(118)))
        {
            boost::shared_ptr<IRequest> request2(new UnsubscribeRoomGroupRequest(boost::to_string(118)));
            GameServer::getSingleton().Send(request2);
            boost::shared_ptr<IRequest> request(new LeaveRoomRequest(lastRoom));
            GameServer::getSingleton().Send(request);
        }
    }
    
    if (SceneManager::getSingleton().getIsFlagStore()){
        return true;
    }
    
    startTimeOutChat();
    LayerHuThuong* huthuong = LayerHuThuong::create();
    huthuong->setTag(19802);
    
    this->addChild(huthuong, 100000);
    
    
    
    
    
    if (SceneManager::getSingleton().getGameID() != kGameTaiXiu){
        LayerLauncherMiniGame* minigame = LayerLauncherMiniGame::create();
        minigame->setPosition(Vec2(sizeAdd.width, sizeAdd.height));
        minigame->setPosition(Vec2(sizeAdd.width, sizeAdd.height + 160));
        this->addChild(minigame, ZORDER_LIST::ZORDER_TAIXIU_MINI);
    }
    
    
    return true;
}

void SceneGame::timeoutChatMaxtime(float dt){
    SceneManager::getSingleton().timerchat++;
    //log("timer chat couting!");
    if (SceneManager::getSingleton().timerchat == dataManager.getAppConfig().chattimeout*SceneManager::getSingleton().sec) {
        SceneManager::getSingleton().timerchat = 0;
        SceneManager::getSingleton().chatcounttime = 0;
        SceneManager::getSingleton().arrCurrentChat.clear();
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(SceneGame::timeoutChatMaxtime),this);
    }
}
void SceneGame::startTimeOutChat(){
    if (SceneManager::getSingleton().timerchat > 0){
        Director::getInstance()->getScheduler()->schedule(schedule_selector(SceneGame::timeoutChatMaxtime),this,1, false);
    }
}
void SceneGame::loadGameByTag(int typeGame)
{
    //Director::getInstance()->setAnimationInterval(1.0 / 45.0);
    //log("typeGame: %d", typeGame);
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOS::logScreenToGA("Chơi game " + *mUtils::getGameNameByID(typeGame));
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string screen = "Chơi " + *mUtils::getGameNameByID(typeGame);
    NativeHelper::trackScreen(screen.c_str());
#endif
    SceneManager::getSingleton().setLastGameID(typeGame);
    switch (typeGame)
    {
        case kGamePhom:
        {
            LayerPlayGamePhom *gamePhom = LayerPlayGamePhom::create();
            if (gamePhom)
                this->addChild(gamePhom);
        }
            break;
        case kGameLieng:
        {
            LayerGameLieng *gameLieng = LayerGameLieng::create();
            this->addChild(gameLieng);
        }
            break;
            //
        case kGameXiTo:
        {
            XiTo *gameXiTo = XiTo::create();
            //                      Size size = ChanUtils::getSizePos();
            //                      gameXiTo->setPosition(Vec2(size.width, size.height));
            this->addChild(gameXiTo);
        }
            break;
            //
        case kGameCoTuong:
        case kGameCoUp:
        {
            LayerMainGame* layerGame = LayerMainGame::create();
            this->addChild(layerGame);
        }
            break;
            
        case kGameChan:
        {
            LayerChanGame* pChanGame = LayerChanGame::create();
            Size size = ChanUtils::getSizePos();
            pChanGame->setPosition(Vec2(size.width, size.height));
            this->addChild(pChanGame);
        }
            break;
            
            // 	case kGameTomCuaCa:
            // 	{
            // 						  TomCuaCa *gameTC = TomCuaCa::create();
            // 						  //gameTC->setPosition(Point(WIDTH_DESIGN / 2, HEIGHT_DESIGN / 2));
            // 						  this->addChild(gameTC);
            // 	}
            // 		break;
            
        case kGameTaiXiu:
        {
            Size size = ChanUtils::getSizePos();
            LayerGameTaiXiu* gameTaiXiu = LayerGameTaiXiu::create();
            gameTaiXiu->setAnchorPoint(Vec2::ZERO);
            gameTaiXiu->setPosition(Vec2(size.width, size.height));
            this->addChild(gameTaiXiu, zorder_SceneGaming, tag_SceneGaming);
        }
            break;
        case kGameTienLenMienBac:
        case kGameTienLenMienNam:
        case kGameSam:
        case kGameDemLaSolo:
        case kGameSamSolo:
        case kGameTLMNDemLa:
        {
            Sam* gameTLMN = Sam::create();
            if (gameTLMN)
                this->addChild(gameTLMN);
        }
            break;
        case kGameBaCay:
        case kGameBaCayChuong:
        {
            LayerPlayGameBaCay* game3Cay = LayerPlayGameBaCay::create();
            this->addChild(game3Cay);
        }
            break;
        case kGameQuayThuong:
        {
            QuayThuong* gameQuayThuong = QuayThuong::create();
            this->addChild(gameQuayThuong);
        }
            break;
        case kGameXocDia:
        {
            LayerGameXocDia *gameXocDia = LayerGameXocDia::create();
            this->addChild(gameXocDia);
        }
            break;
        case kGameMauBinh:
        {
            LayerGameMauBinh *maubinh = LayerGameMauBinh::create();
            if (maubinh == NULL) return;
            this->addChild(maubinh);
        }
            break;
            //update 2/8
        case kGameBauTom:
        {
            LayerGameBauTom* gameBautom = LayerGameBauTom::create();
            this->addChild(gameBautom);
        }
            break;
        case kGamePoker:
        {
            PokerTexasGame* gamePoker = PokerTexasGame::create();
            this->addChild(gamePoker);
        }
            break;
        case kGameSlot:
        {
            LayerPlayGameSlotMachine* gameSlot = LayerPlayGameSlotMachine::create();
            this->addChild(gameSlot);
        }
            break;
        case kGameCatte:
        {
            LayerPlayGameCatte* gameCatte = LayerPlayGameCatte::create();
            this->addChild(gameCatte);
            gameCatte->setAnchorPoint(Vec2::ZERO);
            gameCatte->setPosition(Vec2(0,0));
            //only for test
//            PokerTexasGame* gamePoker = PokerTexasGame::create();
//            this->addChild(gamePoker);
        }
            break;
        case kGame3Doi:
        {
            LayerPlayGame3Doi* game3Doi = LayerPlayGame3Doi::create();
            this->addChild(game3Doi);
            game3Doi->setAnchorPoint(Vec2::ZERO);
            game3Doi->setPosition(Vec2(0,0));
        }
            break;
            
            
    }
    
}

void SceneGame::autoInviteRoom()
{
    if (SceneManager::getSingleton().getCurrRoomType() == 0){
        bool isAuto = SceneManager::getSingleton().getAutoCreateRoom();
        if (isAuto)
        {
            if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL
                || GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") == NULL
                || GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") == NULL)
                return;
            int gid = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0, 3).c_str());
            boost::shared_ptr<string> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue();
            string paramString = param->c_str();
            vector<string> arrInfo = mUtils::splitString(paramString, '@');
            double gameBet = atof(arrInfo.at(0).c_str());
            int amf_value = (int)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetIntValue();
            //
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutInt("maqp", 10);
            params->PutDouble("mimp", gameBet);
            params->PutInt("amf ", amf_value);
            params->PutInt("gid", gid);
            boost::shared_ptr<IRequest> request(new ExtensionRequest("rglip", params));
            GameServer::getSingleton().Send(request);
        }
    }
    else{
        bool isAuto = SceneManager::getSingleton().getAutoCreateRoom();
        if (isAuto)
        {
            if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL
                || GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") == NULL
                || GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") == NULL)
                return;
            int gid = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0, 3).c_str());
            boost::shared_ptr<string> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue();
            string paramString = param->c_str();
            vector<string> arrInfo = mUtils::splitString(paramString, '@');
            double gameBet = atof(arrInfo.at(0).c_str());
            int amf_value = (int)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetIntValue();
            //
            boost::shared_ptr<ISFSObject> params(new SFSObject());
            params->PutInt("maqp", 10);
            params->PutDouble("mimp", gameBet);
            params->PutInt("amfs", amf_value);
            params->PutInt("gid", gid);
            boost::shared_ptr<IRequest> request(new ExtensionRequest("rglip", params));
            GameServer::getSingleton().Send(request);
        }
    }
    
    
}

void SceneGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    if (0 == cmd->compare("hbc")) return;
    if (inviteCount > 0) return; //chi moi 1 lan
    
    if (0 == cmd->compare("lip")) {
        inviteCount++;
        boost::shared_ptr<string> listUser = pObjs->GetUtfString("lipv");
        if (listUser != NULL)
        {
            //log("MOI CHOI = %s", listUser->c_str());
            vector<string> lstFriends = mUtils::splitString(*listUser, '|');
            vector<StructFriendInfo> lstPlayerInfos;
            for (int i = 0; i < lstFriends.size(); ++i)
            {
                if (0 == lstFriends.at(i).compare("")) continue;
                vector<string> friendInfo = mUtils::splitString(lstFriends.at(i), ',');
                StructFriendInfo info;
                info.uid = atoi(friendInfo.at(0).c_str());
                info.aI = friendInfo.at(1);
                info.aN = friendInfo.at(2);
                info.amf = atol(friendInfo.at(3).c_str());
                lstPlayerInfos.push_back(info);
            }
            
            int lengthInvite = lstPlayerInfos.size() >= 5 ? 5 : lstPlayerInfos.size();
            for (int i = 0; i < lengthInvite; ++i)
            {
                StructFriendInfo objInfo = lstPlayerInfos.at(i);
                
                boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
                if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL
                    || GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params") == NULL)
                    return;
                int groupId = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->c_str());
                vector<string> str_params = mUtils::splitString(*GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue(), '@');
                //double mb = atof(lstParams.at(0).c_str());
                //Send request
                
                boost::shared_ptr<ISFSObject> params(new SFSObject());
                params->PutUtfString("gid", GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->c_str());
                params->PutUtfString("mb", str_params.at(0).c_str());
                params->PutUtfString("roomid", StringUtils::format("%d", GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->Id()));
                params->PutUtfString("lu", objInfo.aI.c_str());
                boost::shared_ptr<IRequest> request(new ExtensionRequest("rilu", params));
                GameServer::getSingleton().Send(request);
            }
            
        }
    }
}
