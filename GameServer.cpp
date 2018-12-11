//
//  GameServer.cpp
//  iCasino_v2
//
//  Created by AlexDao on 5/13/14.
//
//

#include "GameServer.h"

#include "Bitswarm/BitSwarmEvent.h"
#include "Bitswarm/BitSwarmClient.h"
#include "Util/EventDispatcher.h"
#include "Requests/LoginRequest.h"
#include "Requests/LogoutRequest.h"

#include "Requests/JoinRoomRequest.h"
#include "Requests/CreateRoomRequest.h"
#include "Requests/RoomSettings.h"
#include "Requests/PublicMessageRequest.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/SetUserVariablesRequest.h"
#include "SceneManager.h"

#include "Entities/Variables/SFSUserVariable.h"
#include "Core/SFSBuddyEvent.h"
#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
USING_NS_CC;
using namespace ui;
using namespace Sfs2X::Bitswarm;
#include <typeinfo>

static int _mTotalTime = 0;
static string currSessionToken = "";

template<> GameServer* SingLeton<GameServer>::mSingleton = 0;
GameServer* GameServer::getSingletonPtr(void)
{
    return mSingleton;
}
GameServer& GameServer::getSingleton(void)
{
    assert(mSingleton);  return (*mSingleton);
}

GameServer::GameServer()
{
    mSmartFox = NULL;
    mCallBack.clear();
    mCallBackIndex = 0;
    _mTotalTime = 0;
}

void GameServer::initServer()
{
    // Initialize Smart Fox
    mSmartFox = boost::shared_ptr<Sfs2X::SmartFox>(new Sfs2X::SmartFox(false));
    mSmartFox->ThreadSafeMode(true);
    //mSmartFox->UseBlueBox(true);
    mSmartFox->HttpPort(80);
    
    mSmartFox->Debug(false);
    mSmartFox->AddLogListener(LOGLEVEL_INFO, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnDebugMessage, (unsigned long long)this)));
    mSmartFox->AddLogListener(LOGLEVEL_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnDebugMessage, (unsigned long long)this)));
    mSmartFox->AddLogListener(LOGLEVEL_WARN, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnDebugMessage, (unsigned long long)this)));
    //mSmartFox->AddLogListener(LOGLEVEL_DEBUG, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnDebugMessage, (unsigned long long)this)));
    // Add event listeners
    mSmartFox->AddEventListener(SFSEvent::CONNECTION, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxConnection, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::CONNECTION_LOST, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxConnectionLost, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::SFS_SOCKET_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxSocketError, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::CONNECTION_RETRY, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxConnectionRetry, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::CONNECTION_RESUME, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxConnectionResume, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::HANDSHAKE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxHandShake, (unsigned long long)this)));
    
    mSmartFox->AddEventListener(SFSEvent::LOGIN, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxLogin, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::LOGIN_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxLoginError, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::LOGOUT, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxLogout, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::PING_PONG, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxPingPong, (unsigned long long)this)));
    
    mSmartFox->AddEventListener(SFSEvent::ROOM_JOIN, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxRoomJoin, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::ROOM_JOIN_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxRoomJoinError, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::ROOM_CREATION_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxRoomCreationError, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::ROOM_ADD, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxRoomAdd, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::ROOM_REMOVE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxRoomDestroy, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::ROOM_VARIABLES_UPDATE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxRoomVariableUpdate, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::ROOM_PASSWORD_STATE_CHANGE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxRoomPasswordStateChange, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::ROOM_PASSWORD_STATE_CHANGE_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxRoomPasswordStateChangeError, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::ROOM_FIND_RESULT, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxRoomFindResult, (unsigned long long)this)));
    
    mSmartFox->AddEventListener(SFSEvent::PUBLIC_MESSAGE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxPublicMessage, (unsigned long long)this)));
    
    mSmartFox->AddEventListener(SFSEvent::USER_VARIABLES_UPDATE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxUserVariableUpdate, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::USER_COUNT_CHANGE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxUserCountChange, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::USER_ENTER_ROOM, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxUserEnterRoom, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::USER_EXIT_ROOM, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxUserExitRoom, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::SPECTATOR_TO_PLAYER_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxSpectatorToPlayerError, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::PLAYER_TO_SPECTATOR_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxPlayerToSpectatorError, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::SPECTATOR_TO_PLAYER, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxSpectatorToPlayer, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::PLAYER_TO_SPECTATOR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxPlayerToSpectator, (unsigned long long)this)));
    
    mSmartFox->AddEventListener(SFSEvent::INVITATION, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxInvitation, (unsigned long long)this)));
    
    mSmartFox->AddEventListener(SFSEvent::EXTENSION_RESPONSE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnExtensionResponse, (unsigned long long)this)));
    
    mSmartFox->AddEventListener(SFSEvent::ROOM_GROUP_SUBSCRIBE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxSubscribeRoomGroup, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::ROOM_GROUP_SUBSCRIBE_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxSubscribeRoomGroupError, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::ROOM_GROUP_UNSUBSCRIBE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxUnSubscribeRoomGroup, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSEvent::ROOM_GROUP_UNSUBSCRIBE_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxUnSubscribeRoomGroupError, (unsigned long long)this)));
    
    mSmartFox->AddEventListener(SFSBuddyEvent::BUDDY_ADD, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxBuddyAdd, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSBuddyEvent::BUDDY_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxBuddyError, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSBuddyEvent::BUDDY_LIST_INIT, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxBuddyListInit, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSBuddyEvent::BUDDY_REMOVE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxBuddyRemove, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSBuddyEvent::BUDDY_ONLINE_STATE_UPDATE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxBuddyOnlineStateUpdate, (unsigned long long)this)));
    mSmartFox->AddEventListener(SFSBuddyEvent::BUDDY_MESSAGE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxBuddyMessage, (unsigned long long)this)));
    
    mSmartFox->AddEventListener(SFSEvent::ADMIN_MESSAGE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(GameServer::OnSmartFoxAdminMessage, (unsigned long long)this)));
    /*
     //load config file, and connect
     #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
     std::string fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sfs-config.xml");
     std::string * filename = new std::string(fullPath);
     //cocos2d::log("%s",fullPath.c_str());
     #else
     std::string* filename = new std::string(".\\sfs-config.xml");
     #endif
     
     mSmartFox->LoadConfig(filename, true);
     delete filename;
     filename = NULL;
     */
}

static void reconnect2Server(float dt){
    
}
static long _lastTimeConnect = 0;
static bool isCanConnectAgain(){
    boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration duration(time.time_of_day());
    if (duration.total_seconds() - _lastTimeConnect > 1){
        _lastTimeConnect = duration.total_seconds();
        return true;
    }
    return false;
}
//connect
void GameServer::connectToServer(const char * strIP, const char * strPort)
{
    //    bool isV4 = true;
    //#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    string iptype = IOS::tryToGetIPAddress();
    //    isV4 = (iptype == "v4") ? true : false;
    //#endif
    try{
        if (!isCanConnectAgain()){
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }
        //remove old smartfox
        //log("remove old smartfox");
        double port = atof(strPort);
        if (mSmartFox){
            this->destroySmartfox();
        }
        //         else{
        //             initServer();
        //         }
       // log("connectToServer with IP: %s", strIP);
        //log("GameServer::connectToServer() - after remove smartfox(); ");
        //
        initServer();
        // mSmartFox->setUseTypeOfIp(isV4);
        //log("GameServer::connectToServer() - after initServer(); ");
        mSmartFox->Connect(strIP, (long int)port);
        //log("GameServer::connectToServer() - after mSmartFox->Connect(); ");
    }
    catch (boost::shared_ptr<SFSValidationError> ex) {
       // log("GameServer::connectToServer() - throw exception: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSError> ex) {
       // log("GameServer::connectToServer() - throw exception: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSCodecError> ex) {
       // log("GameServer::connectToServer() - throw exception: %s", ex->what());
    }
    catch (std::exception ex) {
       // log("GameServer::connectToServer() - throw exception: %s", ex.what());
    }
    catch (...) {
       // log("GameServer::connectToServer() - unknown exception");
    }
}

void GameServer::updateEvent()
{
    //handle the message manually
    if (mSmartFox == NULL)
    return;
    try{
        mSmartFox->ProcessEvents();
    }
    catch (boost::shared_ptr<SFSValidationError> ex) {
       // log("GameServer::updateEvent() - throw exception: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSError> ex) {
       // log("GameServer::updateEvent() - throw exception: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSCodecError> ex) {
       // log("GameServer::updateEvent() - throw exception: %s", ex->what());
    }
    catch (std::exception ex) {
       // log("GameServer::updateEvent() - throw exception: %s", ex.what());
    }
    catch (...) {
       // log("GameServer::updateEvent() - unknown exception");
    }
}
/*
 All events
 */
void GameServer::OnSmartFoxConnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventConnection, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventConnectionLost, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxDisconnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventDisconnected, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxSocketError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventSocketError, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxConnectionRetry(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventConnectionRetry, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxConnectionRetryFail(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventConnectionRetryFail, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxConnectionResume(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventConnectionResume, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxLogin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventLogin, ptrContext, ptrEvent);
    GameServer::getSingleton().getSmartFox()->EnableLagMonitor(true, 5);
    if (GameServer::getSingleton().getSmartFox()->SessionToken() != NULL)
    currSessionToken = *GameServer::getSingleton().getSmartFox()->SessionToken();
    //
    _mTotalTime = 0;
}
void GameServer::OnSmartFoxLoginError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventLoginError, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxLogout(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
   // log("GameServer::OnSmartFoxLogout");
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventLogout0, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxPingPong(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{//
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["lagValue"];
    boost::shared_ptr<int> ptrNotifiedCmd = ((boost::static_pointer_cast<int>)(ptrEventParamValueCmd));
    //log("GameServer::OnSmartFoxPingPong %d ", *ptrNotifiedCmd);
    //     Label *lbl = (Label*)SceneManager::getSingleton().getChildByTag(12311);
    //     if (lbl == NULL)
    //         return;
    //     lbl->setString(StringUtils::format("lag: %d ms", *ptrNotifiedCmd));
    _mTotalTime = 0;
    //GameServer::getSingleton().OnSmartFoxNtf(tag_EventPingPong, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    try{
        GameServer::getSingleton().OnSmartFoxNtf(tag_EventRoomJoin, ptrContext, ptrEvent);
    }
    catch (...) {
       // log("GameServer::updateEvent() - unknown exception");
    }
}
void GameServer::OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventRoomJoinError, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxRoomCreationError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventCreationError, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxRoomAdd(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventRoomAdd, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventRoomVariableUpdate, ptrContext, ptrEvent);
    //     boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    //     boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
    //     boost::shared_ptr<Room> room = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
    //     //
    //     boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
    //     string s = *rv->GetStringValue();
    //    // log("Room %s update RoomVariables: %s", room->Name()->c_str(), s.c_str());
}
void GameServer::OnSmartFoxRoomPasswordStateChange(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventRoomPasswordStateChange, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxRoomPasswordStateChangeError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventRoomPasswordStateChangeError, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxRoomFindResult(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventRoomFindResult, ptrContext, ptrEvent);
}


void GameServer::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventPublicMessage, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventUserVariableUpdate, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxUserCountChange(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventUserCountChange, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxUserEnterRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventUserEnterRoom, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventUserExitRoom, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxSpectatorToPlayerError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventSpectatorToPlayerError, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxPlayerToSpectatorError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventPlayerToSpectatorError, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxSpectatorToPlayer(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventSpectatorToPlayer, ptrContext, ptrEvent);
}
void GameServer::OnSmartFoxPlayerToSpectator(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventPlayerToSpectator, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxInvitation(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventInvitation, ptrContext, ptrEvent);
}

void GameServer::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    
    log ("ten cai ban tin chet tiet : %s",ptrNotifiedCmd->c_str());
    if (strcmp("hbc", ptrNotifiedCmd->c_str()) == 0){
        boost::shared_ptr<ISFSObject> parameter(new SFSObject());
        //sendRequest
        boost::shared_ptr<IRequest> request(new ExtensionRequest("ghbres", parameter));
        GameServer::getSingleton().Send(request);
        return;
    }
    {
        boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
        boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
        if (pObjs->GetUtfString("_json_") != NULL){
           // log("json: %s", pObjs->GetUtfString("_json_")->c_str());
        }
    }
    try{
        GameServer::getSingleton().OnSmartFoxNtf(tag_EventExtensionResponse, ptrContext, ptrEvent);
    }
    catch (boost::shared_ptr<SFSValidationError> ex) {
       // log("GameServer::updateEvent() - throw exception: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSError> ex) {
       // log("GameServer::updateEvent() - throw exception: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSCodecError> ex) {
       // log("GameServer::updateEvent() - throw exception: %s", ex->what());
    }
    catch (std::exception ex) {
       // log("GameServer::updateEvent() - throw exception: %s", ex.what());
    }
    catch (...) {
       // log("GameServer::updateEvent() - unknown exception");
    }
}

void GameServer::addListeners(PlayerCallBack * callBack)
{
    //    boost::mutex::scoped_lock lock(mtxAddAction);
    //    mQueueAdd.push(callBack);
    //    callBack->className = string(typeid(*callBack).name());
    //    mtxAddAction.unlock();
    //boost::mutex::scoped_lock lock(mtxRunningAction);
    if (callBack == NULL)
        return;
    //printf("the name of class: %s\n", typeid(*callBack).name() );
    callBack->className = (string(typeid(*callBack).name())).c_str();
    callBack->isAddListener = true;
    PlayerCallBackMapIter iTer = mCallBack.begin();
    
    for (; iTer != mCallBack.end(); iTer++)
    {
        //if callback already exist, return
        if (iTer->second == callBack)
        {
            
            //mtxRunningAction.unlock();
            return;
        }
    }
    callBack->className = "1";
    callBack->index = mCallBackIndex;
    mCallBack.insert(PlayerCallBackMapPair(mCallBackIndex++, callBack));
    //mtxRunningAction.unlock();
}
void GameServer::removeListeners(PlayerCallBack * callBack){
    
    //    boost::mutex::scoped_lock lock(mtxRemoveAction);
    //    callBack->className = "";
    //    mQueueRemoveIndex.push(callBack->index);
    //    mtxRemoveAction.unlock();
    if (callBack == NULL)
    return;
    //boost::mutex::scoped_lock lock(mtxRunningAction);
    //printf("the name of class: %s\n", typeid(*callBack).name() );
    //log("GameServer::removeListeners() - BEGIN");
    PlayerCallBackMapIter iTer = mCallBack.begin();
    callBack->className = "";
    callBack->isAddListener = false;
    for (; iTer != mCallBack.end(); iTer++)
    {
        //if callback already exist, return
        if (iTer->first == callBack->index){
            mCallBack.erase(iTer);
            
            break;
        }
    }
    callBack = NULL;    
}

/*
 Notify
 */

void GameServer::OnSmartFoxNtf(eEventTags tag, unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    //xu ly cho queue add
    //    boost::mutex::scoped_lock lock(mtxAddAction);
    //    while(mQueueAdd.size()!=0){
    //        PlayerCallBack* callBack = mQueueAdd.front();
    //        mQueueAdd.pop();
    //        if (callBack == NULL || callBack->className=="")
    //            continue;
    //        printf("Add PlayerCallback of class: %s x with index = %d  \n", "", (mCallBackIndex) );
    //        callBack->className = "1";
    //
    //        PlayerCallBackMapIter iTer = mCallBack.begin();
    //        bool check = false;
    //        for (; iTer != mCallBack.end(); iTer++)
    //        {
    //            //if callback already exist, return
    //            if (iTer->second == callBack){
    //                check = true;
    //                break;
    //            }
    //        }
    //        callBack->index = mCallBackIndex;
    //        if(!check)
    //            mCallBack.insert(PlayerCallBackMapPair(mCallBackIndex++, callBack));
    //    }
    //    mtxAddAction.unlock();
    //    //xu ly cho queue remove
    //    boost::mutex::scoped_lock lock1(mtxRemoveAction);
    //    while(mQueueRemoveIndex.size()!=0){
    //        int callBackIndex = mQueueRemoveIndex.front();
    //        mQueueRemoveIndex.pop();
    //            if (callBackIndex == -1)
    //                continue;
    //            //log("GameServer::removeListeners() - BEGIN");
    //            PlayerCallBackMapIter iTer = mCallBack.begin();
    //
    //            for (; iTer != mCallBack.end(); iTer++)
    //            {
    //                //if callback already exist, return
    //                if (iTer->first == callBackIndex){
    //                    printf("Remove PlayerCallback of class  with index = %d \n", iTer->first );
    //                    mCallBack.erase(iTer);
    //
    //                    break;
    //                }
    //            }
    //    }
    //    mtxRemoveAction.unlock();
    
    
    PlayerCallBackMap _backup;
    _backup.clear();
    while (true){
        if (mCallBack.size() == 0) break;
        PlayerCallBackMapIter iTer = mCallBack.begin();
        while (iTer != mCallBack.end()){
            bool isExisted = false;
            int counter = 0;
            PlayerCallBackMapIter _iTerBackup = _backup.begin();
            while (_iTerBackup != _backup.end()){
                if (_iTerBackup->second == iTer->second){ //existed
                    isExisted = true;
                }
                _iTerBackup++;
                counter++;
            }
            {//Check if end of list
                PlayerCallBackMapIter _iTer = iTer;
                ++_iTer;
                if (_iTer == mCallBack.end() && isExisted){
                    return;
                }
            }
            PlayerCallBack* _tmp = static_cast<PlayerCallBack*>(iTer->second);
            if (!_tmp->isAddListener){
                isExisted = true;
            }
            
//            try {
//               // log("class name = %s",_tmp->className);
//            } catch (exception e) {
//                log ("exception :%s",e.what());
//                //iTer++;
//                return;
//            }
            if (_tmp->className == "")
            {
                iTer++;
                return;
            }

            if (_tmp == NULL)
                isExisted = true;
            else if (iTer->first != _tmp->index)
                isExisted = true;
            if (isExisted){
                iTer++;
                continue;
            }
//            if (_tmp->className == "" || _tmp->className == "/0x"){
////                for (; iTer != mCallBack.end(); iTer++)
////                {
////                    //if callback already exist, return
////                   // if (iTer->first == callBack->index){
////                        mCallBack.erase(iTer);
////
////                   //     break;
////                   // }
////                }
////                break;
//            }
            if (iTer->second == NULL){
                iTer++;
                continue;
            }
           // if (typeid(*iTer->second))
           // if (typeid(*iTer->second).name() == NULL)
            //    continue;
            //
            //insert to stack
            _backup.insert(PlayerCallBackMapPair(_backup.size(), iTer->second));
            //
            boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
            boost::shared_ptr<Room> currRoom;
            
            boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
            if (ptrEvetnParams != NULL){
                boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
                if (ptrEventParamValueRoom != NULL)
                    currRoom = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
            }
            //do work
            try {
                switch (tag)
                {
                    case tag_EventConnection:
                        iTer->second->OnSmartFoxConnection(ptrContext, ptrEvent);
                        break;
                    case tag_EventConnectionLost:
                        iTer->second->OnSmartFoxConnectionLost(ptrContext, ptrEvent);
                        break;
                    case tag_EventDisconnected:
                        iTer->second->OnSmartFoxDisconnection(ptrContext, ptrEvent);
                        break;
                    case tag_EventSocketError:
                        iTer->second->OnSmartFoxSocketError(ptrContext, ptrEvent);
                        break;//
                    case tag_EventConnectionRetry:
                        iTer->second->OnSmartFoxConnectionRetry(ptrContext, ptrEvent);
                        break;
                    case tag_EventConnectionRetryFail:
                        iTer->second->OnSmartFoxConnectionRetryFail(ptrContext, ptrEvent);
                        break;
                    case tag_EventConnectionResume:
                        iTer->second->OnSmartFoxConnectionResume(ptrContext, ptrEvent);
                        break;
                    case tag_EventLogin:
                        iTer->second->OnSmartFoxLogin(ptrContext, ptrEvent);
                        break;
                    case tag_EventLoginError:
                        iTer->second->OnSmartFoxLoginError(ptrContext, ptrEvent);
                        break;
                    case tag_EventLogout0:
                       // log("tag_EventLogout");
                        iTer->second->OnSmartFoxLogout(ptrContext, ptrEvent);
                        break;
                    case tag_EventPingPong:
                        iTer->second->OnSmartFoxPingPong(ptrContext, ptrEvent);
                        break;
                    case tag_EventRoomJoin:
                        iTer->second->OnSmartFoxRoomJoin(ptrContext, ptrEvent);
                        break;
                    case tag_EventRoomJoinError:
                        iTer->second->OnSmartFoxRoomJoinError(ptrContext, ptrEvent);
                        break;
                    case tag_EventCreationError:
                        iTer->second->OnSmartFoxRoomCreationError(ptrContext, ptrEvent);
                        break;
                    case tag_EventRoomAdd:
                        //                 if( lastRoom!=NULL && currRoom!=NULL )
                        //                     if( lastRoom->Id() == currRoom->Id() )
                        iTer->second->OnSmartFoxRoomAdd(ptrContext, ptrEvent);
                        break;
                    case tag_EventRoomDestroy:
                        iTer->second->OnSmartFoxRoomDestroy(ptrContext, ptrEvent);
                        break;
                    case tag_EventRoomVariableUpdate:
                        if (lastRoom != NULL && currRoom != NULL)
                            if (lastRoom->Id() == currRoom->Id())
                                iTer->second->OnSmartFoxRoomVariableUpdate(ptrContext, ptrEvent);
                        break;
                    case tag_EventRoomPasswordStateChange:
                        if (lastRoom != NULL && currRoom != NULL)
                            if (lastRoom->Id() == currRoom->Id())
                                iTer->second->OnSmartFoxRoomPasswordStateChange(ptrContext, ptrEvent);
                        break;
                    case tag_EventRoomPasswordStateChangeError:
                        if (lastRoom != NULL && currRoom != NULL)
                            if (lastRoom->Id() == currRoom->Id())
                                iTer->second->OnSmartFoxRoomPasswordStateChangeError(ptrContext, ptrEvent);
                        break;
                    case tag_EventRoomFindResult:
                        iTer->second->OnSmartFoxRoomFindResult(ptrContext, ptrEvent);
                        break;
                    case tag_EventPublicMessage:
                        if (lastRoom != NULL && currRoom != NULL)
                            if (lastRoom->Id() == currRoom->Id())
                                iTer->second->OnSmartFoxPublicMessage(ptrContext, ptrEvent);
                        break;
                    case tag_EventUserVariableUpdate:
                        iTer->second->OnSmartFoxUserVariableUpdate(ptrContext, ptrEvent);
                        break;
                    case tag_EventUserCountChange:
                        if (lastRoom != NULL && currRoom != NULL)
                            if (lastRoom->Id() == currRoom->Id())
                                iTer->second->OnSmartFoxUserCountChange(ptrContext, ptrEvent);
                        break;
                    case tag_EventUserEnterRoom:
                        if (lastRoom != NULL && currRoom != NULL)
                            if (lastRoom->Id() == currRoom->Id())
                                iTer->second->OnSmartFoxUserEnterRoom(ptrContext, ptrEvent);
                        break;
                    case tag_EventUserExitRoom:
                        //                 if (lastRoom != NULL && currRoom != NULL)
                        //                 if (lastRoom->Id() == currRoom->Id())
                        iTer->second->OnSmartFoxUserExitRoom(ptrContext, ptrEvent);
                        break;
                    case tag_EventSpectatorToPlayerError:
                        if (lastRoom != NULL && currRoom != NULL)
                            if (lastRoom->Id() == currRoom->Id())
                                iTer->second->OnSmartFoxSpectatorToPlayerError(ptrContext, ptrEvent);
                        break;
                    case tag_EventPlayerToSpectatorError:
                        if (lastRoom != NULL && currRoom != NULL)
                            if (lastRoom->Id() == currRoom->Id())
                                iTer->second->OnSmartFoxPlayerToSpectatorError(ptrContext, ptrEvent);
                        break;
                    case tag_EventSpectatorToPlayer:
                        if (lastRoom != NULL && currRoom != NULL)
                            if (lastRoom->Id() == currRoom->Id())
                                iTer->second->OnSmartFoxSpectatorToPlayer(ptrContext, ptrEvent);
                        break;
                    case tag_EventPlayerToSpectator:
                        if (lastRoom != NULL && currRoom != NULL)
                            if (lastRoom->Id() == currRoom->Id())
                                iTer->second->OnSmartFoxPlayerToSpectator(ptrContext, ptrEvent);
                        break;
                    case tag_EventInvitation:
                        iTer->second->OnSmartFoxInvitation(ptrContext, ptrEvent);
                        break;
                    case tag_EventExtensionResponse:
                        iTer->second->OnExtensionResponse(ptrContext, ptrEvent);
                        break;
                    case tag_EventSubscribeRoomGroup:
                        iTer->second->OnSmartFoxSubscribeRoomGroup(ptrContext, ptrEvent);
                        break;
                    case tag_EventSubscribeRoomGroupError:
                        iTer->second->OnSmartFoxSubscribeRoomGroupError(ptrContext, ptrEvent);
                        break;
                    case tag_EventUnSubscribeRoomGroup:
                        iTer->second->OnSmartFoxUnSubscribeRoomGroup(ptrContext, ptrEvent);
                        break;
                    case tag_EventUnSubscribeRoomGroupError:
                        iTer->second->OnSmartFoxUnSubscribeRoomGroupError(ptrContext, ptrEvent);
                        break;
                    case tag_EventBuddyListInit:
                        iTer->second->OnSmartFoxBuddyListInit(ptrContext, ptrEvent);
                        break;
                    case tag_EventBuddyRemove:
                        iTer->second->OnSmartFoxBuddyRemove(ptrContext, ptrEvent);
                        break;
                    case tag_EventBuddyError:
                        iTer->second->OnSmartFoxBuddyError(ptrContext, ptrEvent);
                        break;
                    case tag_EventBuddyAdd:
                        iTer->second->OnSmartFoxBuddyAdd(ptrContext, ptrEvent);
                        break;
                    case tag_EventBuddyMessage:
                        iTer->second->OnSmartFoxBuddyMessage(ptrContext, ptrEvent);
                        break;
                    case tag_EventBuddyOnlineStateUpdate:
                        iTer->second->OnSmartFoxBuddyOnlineStateUpdate(ptrContext, ptrEvent);
                        break;
                    case tag_EventAdminMessage:
                        iTer->second->OnSmartFoxAdminMessage(ptrContext, ptrEvent);
                        break;
                    case tag_EventHandShake:
                        iTer->second->OnSmartFoxHandShake(ptrContext, ptrEvent);
                        break;
                };
                //
                break;
            }catch(...){
               // log("eo biet nguyen nhan!");
            }
          
        }
    }
}
static int counterErrorRetry = 0;
void GameServer::OnDebugMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["message"];
    boost::shared_ptr<string> ptrNotifiedMsg = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
   // log("GameServer::OnDebugMessage() - %s", ptrNotifiedMsg->c_str());
    if (ptrNotifiedMsg->compare("TCPSocketLayer:  Error sending data to remote side over the established connection") == 0){
        //if (!GameServer::getSingleton().getSmartFox()->IsConnected())
        counterErrorRetry++;
        if (counterErrorRetry > 20){
            GameServer::getSingleton().destroySmartfox();
            GameServer::getSingleton().initServer();
            counterErrorRetry = 0;
        }
    }
    else if (ptrNotifiedMsg->compare("TCPSocketLayer:  Connection error: ") == 0){
        SceneManager::getSingleton().hideLoading();
        SceneManager::getSingleton().nextStepConn();
    }
}

void GameServer::OnSmartFoxRoomDestroy(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventRoomDestroy, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventSubscribeRoomGroup, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventSubscribeRoomGroupError, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxUnSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventUnSubscribeRoomGroup, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxUnSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventUnSubscribeRoomGroupError, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxBuddyListInit(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventBuddyListInit, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxBuddyError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventBuddyError, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxBuddyRemove(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventBuddyRemove, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxBuddyAdd(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventBuddyAdd, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxBuddyOnlineStateUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventBuddyOnlineStateUpdate, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxBuddyMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventBuddyMessage, ptrContext, ptrEvent);
}

void GameServer::Send(boost::shared_ptr<IRequest> request)
{
    if (mSmartFox == NULL)
    return;
    if (!mSmartFox->IsConnected() || mSmartFox->IsConnecting()){
        if (!mSmartFox->IsConnected())
        mSmartFox->Disconnect();
        return;
    }
    try {
        boost::shared_ptr<ExtensionRequest> _req = ((boost::static_pointer_cast<ExtensionRequest>))(request);
        if (_req != NULL){
            _req->IsEncrypted(true);
            mSmartFox->Send(_req);
        }
        else
        mSmartFox->Send(request);
    }
    catch (boost::shared_ptr<SFSValidationError> ex) {
       // log("GameServer::Send() - throw exception 1: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSError> ex) {
       // log("GameServer::Send() - throw exception 2: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSCodecError> ex) {
       // log("GameServer::Send() - throw exception 3: %s", ex->what());
    }
    catch (std::exception ex) {
       // log("GameServer::Send() - throw exception 4: %s", ex.what());
    }
    catch (...) {
       // log("GameServer::Send() - unknown exception");
    }
}

void GameServer::SendLogin(boost::shared_ptr<IRequest> request)
{
    if (mSmartFox == NULL)
    return;
    if (!mSmartFox->IsConnected() || mSmartFox->IsConnecting()){
        if (!mSmartFox->IsConnected())
        mSmartFox->Disconnect();
        return;
    }
    try {
        mSmartFox->Send(request);
        _mTotalTime = -10;
    }
    catch (boost::shared_ptr<SFSValidationError> ex) {
       // log("GameServer::SendLogin() - throw exception: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSError> ex) {
       // log("GameServer::SendLogin() - throw exception: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSCodecError> ex) {
       // log("GameServer::SendLogin() - throw exception: %s", ex->what());
    }
    catch (std::exception ex) {
       // log("GameServer::SendLogin() - throw exception: %s", ex.what());
    }
    catch (...) {
       // log("GameServer::SendLogin() - unknown exception");
    }
}

void GameServer::updateEvent4CheckConnection()
{
    if (mSmartFox == NULL || mSmartFox->ConnectionMode() == NULL || mSmartFox->ConnectionMode()->compare("http") == 0)
    return;
    try {
        //Check time
        if (mSmartFox->MySelf() == NULL){
            _mTotalTime++;
            //log("Im  in updateEvent4CheckConnection - _mTotalTime=%d", _mTotalTime);
            if (_mTotalTime > 20){//lon hon 20s, thi try reconnect
               // log("GameServer::updateEvent( ) - Im reconnecting - time Reconnect: %d - old session token: %s - currSession: %s"
//                    , mSmartFox->GetReconnectionSeconds()
//                    , currSessionToken.c_str()
//                    , mSmartFox->SessionToken()->c_str());
                _mTotalTime = -20;
                mSmartFox->Disconnect();
                //mSmartFox->HandleReconnectionFailure();
                //mSmartFox->Connect(mSmartFox->Config()->Host(), mSmartFox->Config()->Port());
            }
            return;
        }
        //truong hop dac biet
        //         if (!mSmartFox->IsConnected()){
        //             if (!SceneManager::getSingleton().getLayerLogin()->isVisible()){
        //                 SceneManager::getSingleton().gotoMain();
        //                 SceneManager::getSingleton().gotoLogin();
        //             }
        //         }
    }
    catch (boost::shared_ptr<SFSValidationError> ex) {
       // log("GameServer::updateEvent4CheckConnection() - throw exception: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSError> ex) {
       // log("GameServer::updateEvent4CheckConnection() - throw exception: %s", ex->what());
    }
    catch (boost::shared_ptr<SFSCodecError> ex) {
       // log("GameServer::updateEvent4CheckConnection() - throw exception: %s", ex->what());
    }
    catch (std::exception ex) {
       // log("GameServer::updateEvent4CheckConnection() - throw exception: %s", ex.what());
    }
    catch (...) {
       // log("GameServer::updateEvent4CheckConnection() - unknown exception");
    }
}

void GameServer::OnSmartFoxHandShake(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
   // log("GameServer::OnSmartFoxHandShake() %s", currSessionToken.c_str());
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventHandShake, ptrContext, ptrEvent);
}

void GameServer::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    GameServer::getSingleton().OnSmartFoxNtf(tag_EventAdminMessage, ptrContext, ptrEvent);
}

void GameServer::destroySmartfox()
{
    //if (mSmartFox == NULL) return;
    mSmartFox->RemoveAllEventListeners();
    //log("GameServer::connectToServer() - after remove smartfox RemoveAllEventListeners(); ");
    mSmartFox->Disconnect();
    mSmartFox->Dispose();
    //log("GameServer::connectToServer() - after remove smartfox Dispose()(); ");
    //mSmartFox.reset();
    //log("GameServer::connectToServer() - after remove smartfox.reset(); ");
    mSmartFox = NULL;
}
