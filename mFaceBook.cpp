//
//  FaceBook.cpp
//  iCasnio_v3
//
//  Created by Tuan Anh on 11/17/15.
//
//
//#if ( CC_TARGET_PLATFORM != CC_PLATFORM_WINRT && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX )
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "mFaceBook.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "SceneManager.h"
#include "layergames/_Chat_.h"
#include "layergames/PhomMessDef.h"
#include "layers/LayerLogin.h"
#include "data_manager.h"
#include "ExtensionRequest.h"
using namespace rapidjson;
using namespace sdkbox;
using namespace cocos2d;

static bool checkFaceBookStatus()
{
    CCLOG("##FB> permission list: ");
    for (auto& permission : PluginFacebook::getPermissionList())
    {
        CCLOG("##FB>> permission %s", permission.data());
        if (strcmp(permission.data(),"user_friends")== 0 || strcmp(permission.data(),"publish_actions")== 0)
            return true;
    }
    return false;
    CCLOG("##FB> access token: %s", PluginFacebook::getAccessToken().c_str());
    CCLOG("##FB> user id: %s", PluginFacebook::getUserID().c_str());
    CCLOG("##FB> FBSDK version: %s", PluginFacebook::getSDKVersion().c_str());
}
static bool checkFaceBookShare()
{
    CCLOG("##FB> permission list: ");
    for (auto& permission : PluginFacebook::getPermissionList())
    {
        CCLOG("##FB>> permission %s", permission.data());
        if (strcmp(permission.data(),"publish_actions")== 0)
            return true;
    }
    return false;
    CCLOG("##FB> access token: %s", PluginFacebook::getAccessToken().c_str());
    CCLOG("##FB> user id: %s", PluginFacebook::getUserID().c_str());
    CCLOG("##FB> FBSDK version: %s", PluginFacebook::getSDKVersion().c_str());
}
void mFaceBook::LoginForFriend(){
    PluginFacebook::setListener(this);
    this->action = 100;
    CCLOG("##FB %s", __FUNCTION__);
    
    if (PluginFacebook::isLoggedIn())
    {
        onGetMyInfo();
        return;
        //  if (checkFaceBookStatus()){
        onGetMyFriends();
        //        }else{
        ////            PluginFacebook::logout();
        //            vector<string>per;
        //            per.push_back(sdkbox::FB_PERM_READ_EMAIL);
        //            per.push_back(sdkbox::FB_PERM_READ_PUBLIC_PROFILE);
        //           // per.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
        //            //per.push_back(sdkbox::FB_PERM_PUBLISH_POST);
        //            PluginFacebook::login(per);
        //        }
    }
    else
    {
        vector<string>per;
        per.push_back(sdkbox::FB_PERM_READ_EMAIL);
        per.push_back(sdkbox::FB_PERM_READ_PUBLIC_PROFILE);
        // per.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
        //per.push_back(sdkbox::FB_PERM_PUBLISH_POST);
        PluginFacebook::login(per);
        
    }
}
void mFaceBook::onLogin()
{
    //this->onLogout();
    //#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    PluginFacebook::setAppId(dataManager.getAppConfig().facebookappid);
    //#endif
    action = 1;
    PluginFacebook::setListener(this);
    CCLOG("##FB %s", __FUNCTION__);
    if (PluginFacebook::isLoggedIn())
    {
        //  if (checkFaceBookStatus()){
        onGetMyInfo();
        //log("login");
        //            }else{
        //                onGetMyInfo();
        //
        //                return;
        //                PluginFacebook::logout();
        //                vector<string>per;
        //                per.push_back(sdkbox::FB_PERM_READ_EMAIL);
        //                per.push_back(sdkbox::FB_PERM_READ_PUBLIC_PROFILE);
        //                //per.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
        //                per.push_back(sdkbox::FB_PERM_PUBLISH_POST);
        //                PluginFacebook::login(per);
        //            }
        
        
    }
    else
    {
        vector<string>per;
        per.push_back(sdkbox::FB_PERM_READ_EMAIL);
        per.push_back(sdkbox::FB_PERM_READ_PUBLIC_PROFILE);
        //per.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
        // per.push_back(sdkbox::FB_PERM_PUBLISH_POST);
        PluginFacebook::login(per);
        
        
    }
}

void mFaceBook::onLogout(){
    if (PluginFacebook::isLoggedIn())
    {
        PluginFacebook::logout();
        
        //log("login");
        
        PluginFacebook::login();
    }
}
void mFaceBook::onGetUID(){
    //this->onLogout();
    
    action = 2;
    PluginFacebook::setListener(this);
    
    if (PluginFacebook::isLoggedIn())
    {
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
            if (checkFaceBookStatus()){
                onGetMyInfo();
                //log("login");
            }else{
                PluginFacebook::logout();
                vector<string>per;
                per.push_back(sdkbox::FB_PERM_READ_EMAIL);
                per.push_back(sdkbox::FB_PERM_READ_PUBLIC_PROFILE);
                //per.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
                //per.push_back(sdkbox::FB_PERM_PUBLISH_POST);
                PluginFacebook::login(per);
            }
            
        }else{
            onGetMyInfo();
            //log("login");
        }
    }
    else
    {
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
            vector<string>per;
            per.push_back(sdkbox::FB_PERM_READ_EMAIL);
            per.push_back(sdkbox::FB_PERM_READ_PUBLIC_PROFILE);
            //per.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
            //per.push_back(sdkbox::FB_PERM_PUBLISH_POST);
            PluginFacebook::login(per);
        }
        else{
            vector<string>per;
            per.push_back(sdkbox::FB_PERM_READ_EMAIL);
            per.push_back(sdkbox::FB_PERM_READ_PUBLIC_PROFILE);
            //per.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
            //per.push_back(sdkbox::FB_PERM_PUBLISH_POST);
            PluginFacebook::login(per);
        }
        
        
    }
    
}

void mFaceBook::onCheckStatus()
{
    PluginFacebook::setListener(this);
    CCLOG("##FB %s", __FUNCTION__);
    checkFaceBookStatus();
}
void mFaceBook::onGetMyInfo()
{
    SceneManager::getSingleton().showLoading();
    
    PluginFacebook::setListener(this);
    CCLOG("##FB %s", __FUNCTION__);
    
    sdkbox::FBAPIParam params;
    params["fields"] = "name,email";
    sdkbox::PluginFacebook::api("me", "GET", params, "me");
}
void mFaceBook::onGetMyFriends()
{
    PluginFacebook::setListener(this);
    CCLOG("##FB %s", __FUNCTION__);
    SceneManager::getSingleton().showLoading(40);
    sdkbox::FBAPIParam params;
    std::string token = sdkbox::PluginFacebook::getAccessToken();
    CCLOG("Token %s", token.c_str());
    std::string path = "/";
    path += sdkbox::PluginFacebook::getUserID();
    path += "/invitable_friends?limit=5000&access_token=";
    path +=token;
    path +="&debug=all";
    CCLOG("Path %s", path.c_str());
    sdkbox::PluginFacebook::api(path, "GET", params, "__fetch_picture_tag__");
    //sdkbox::PluginFacebook::api("/me/invitable_friends", "GET", params, "__fetch_picture_tag__");
}
void mFaceBook::onCaptureScreen()
{
    PluginFacebook::setListener(this);
    CCLOG("##FB %s", __FUNCTION__);
    
    utils::captureScreen(CC_CALLBACK_2(mFaceBook::afterCaptureScreen, this), "screen.png");
}
void mFaceBook::afterCaptureScreen(bool yes, const std::string &outputFilename)
{
    PluginFacebook::setListener(this);
    CCLOG("##FB afterCaptureScreen: %s", outputFilename.c_str());
    if (yes)
    {
        _captureFilename = outputFilename;
    }
}
void mFaceBook::onShareLink()
{
    PluginFacebook::setListener(this);
    CCLOG("##FB %s", __FUNCTION__);
    
    //    FBShareInfo info;
    //    info.type = FB_LINK;
    //    info.link = "https://play.google.com/store/apps/details?id=mocquan.gamedanhbai.club";
    //    info.title = "Naga Club";
    //    info.text = "";
    //    info.image = "";
    //    PluginFacebook::share(info);
    sdkbox::FBShareInfo info;
    info.type  = sdkbox::FB_LINK;
    info.link  = "http://www.cocos2d-x.org";
    info.title = "cocos2d-x";
    info.text  = "Best Game Engine";
    info.image = "http://cocos2d-x.org/images/logo.png";
    sdkbox::PluginFacebook::share(info);
}
void mFaceBook::onSharePhoto()
{
    PluginFacebook::setListener(this);
    CCLOG("##FB %s", __FUNCTION__);
    
    if (!_captureFilename.empty() && FileUtils::getInstance()->isFileExist(_captureFilename))
    {
        CCLOG("##FB dialog photo: %s", _captureFilename.c_str());
        
        FBShareInfo info;
        info.type = FB_PHOTO;
        info.title = "capture screen";
        info.image = _captureFilename;
        PluginFacebook::share(info);
    }
    else
    {
        CCLOG("##FB capture screen first");
    }
}
void mFaceBook::onDialogLink()
{
    PluginFacebook::setListener(this);
    CCLOG("##FB %s", __FUNCTION__);
    
    FBShareInfo info;
    info.type = FB_LINK;
    info.type  = sdkbox::FB_LINK;
    info.link  = "https://nagaclub.net";
    info.title = "NagaClub - The #1 Cambodia FREE CASINO GAME! Play and Win Rewards!";
    info.text  = "Install Now to receive Wings FREE Welcome Bonus! Play Poker ★ Jackpot ★ Tien Len ★ Catte ★ 3 Pair ★ Slot Machine ★ Thin Thin";
    info.image = this->_url_image_share;
    PluginFacebook::dialog(info);
    //    sdkbox::FBShareInfo info;
    //    info.type  = sdkbox::FB_LINK;
    //    info.link  = "http://www.cocos2d-x.org";
    //    info.title = "cocos2d-x";
    //    info.text  = "Best Game Engine";
    //    info.image = "http://cocos2d-x.org/images/logo.png";
    //    sdkbox::PluginFacebook::share(info);
}
void mFaceBook::onDialogPhoto()
{
    PluginFacebook::setListener(this);
    CCLOG("##FB %s", __FUNCTION__);
    
    if (!_captureFilename.empty() && FileUtils::getInstance()->isFileExist(_captureFilename))
    {
        CCLOG("dialog photo: %s", _captureFilename.c_str());
        FBShareInfo info;
        info.type = FB_PHOTO;
        info.title = "capture screen";
        info.image = _captureFilename;
        PluginFacebook::dialog(info);
    }
    else
    {
        CCLOG("##FB capture screen first");
    }
}
void mFaceBook::onRequestReadPermission()
{
    PluginFacebook::setListener(this);
    CCLOG("##FB %s", __FUNCTION__);
    
    sdkbox::PluginFacebook::requestReadPermissions({FB_PERM_READ_PUBLIC_PROFILE});
}

void mFaceBook::onRequestPublishPermission(std::string url)
{
    action = 102;
    this->_url_image_share = url;
    PluginFacebook::setListener(this);
    this->isShare = false;
    //    CCLOG("##FB %s", __FUNCTION__);
    //    if (PluginFacebook::isLoggedIn())
    //    {
    //        if (!checkFaceBookShare())
    //        {
    //            PluginFacebook::logout();
    sdkbox::PluginFacebook::requestPublishPermissions({FB_PERM_PUBLISH_POST});
    //        }
    //        else
    //            this->onDialogLink();
    //    }
    //    else
    //    {
    //        sdkbox::PluginFacebook::requestPublishPermissions({FB_PERM_PUBLISH_POST});
    //    }
    
}

void mFaceBook::onInviteFriends()
{
    PluginFacebook::setListener(this);
    PluginFacebook::inviteFriends("https://fb.me/779436578852130",
                                  "https://fbcdn-sphotos-c-a.akamaihd.net/hphotos-ak-xft1/v/t1.0-9/10438533_1675461496025432_6277099128556671220_n.png?oh=b68d065330fb8a17991b0c994f785bde&oe=57038EE3&__gda__=1464227714_fc6c18be6770782e7aa0e2a460ede1a4");
}

/*********************
 * Facebook callbacks
 *********************/
void mFaceBook::onLogin(bool isLogin, const std::string& error)
{
    CCLOG("##FB isLogin: %d, error: %s", isLogin, error.c_str());
    
    if (isLogin)
    {
        //log("success");
        if (action == 1)
            onGetMyInfo();
        else if (action == 2)
            onGetMyInfo();
        else if (action == 100)
            onGetMyFriends();
        else if (action == 102)
            sdkbox::PluginFacebook::requestPublishPermissions({FB_PERM_PUBLISH_POST});
        
        
    }
    else if (strcmp(error.c_str(), "cancelled") == 0){
        std::string* id = new std::string("cancel", 0, 1000);
        if (mCallBack && mCallBackListener){
            (mCallBack->*mCallBackListener)(NULL, (void*)id);
        }
    }
    
    //    std::string title = "login ";
    //    title.append((isLogin ? "success" : "failed"));
    //    MessageBox(error.c_str(), title.c_str());
    
}
void mFaceBook::onAPI(const std::string& tag, const std::string& jsonData)
{
    rapidjson::Document document;
    document.Parse<0>(jsonData.c_str());
    if (document.HasParseError()) {
        return;
    }
    if (tag == "__fetch_picture_tag__") {
        if (document.IsObject()) {
            SceneManager::getSingleton().showLoading();
            //invited facebook get list Friend
            rapidjson::Value& data = document["data"];
            FriendInfo friendInfo;
            if (data.Size() <=25){
                for (rapidjson::SizeType i = 0; i < data.Size(); ++i){
                    CCLOG("id: %s - name: %s", data[i]["id"].GetString(), data[i]["name"].GetString());
                    friendInfo.uid = data[i]["id"].GetString();
                    friendInfo.name = data[i]["name"].GetString();
                    friendInfo.isInvite = false;
                    rapidjson::Value& picture = data[i]["picture"];
                    //CCLOG("url: %s", picture["data"]["url"].GetString());
                    friendInfo.url_picture = picture["data"]["url"].GetString();
                    SceneManager::getSingleton().pushInviteableUsers(friendInfo);
                }
                
            }else{
                for (rapidjson::SizeType i = 0; i < data.Size(); ++i){
                    CCLOG("id: %s - name: %s", data[i]["id"].GetString(), data[i]["name"].GetString());
                    
                    friendInfo.uid = data[i]["id"].GetString();
                    friendInfo.name = data[i]["name"].GetString();
                    friendInfo.isInvite = false;
                    rapidjson::Value& picture = data[i]["picture"];
                    //CCLOG("url: %s", picture["data"]["url"].GetString());
                    friendInfo.url_picture = picture["data"]["url"].GetString();
                    SceneManager::getSingleton().pushInviteableUsers(friendInfo);
                }
                
            }
            if (mCallBack && mCallBackListener){
                (mCallBack->*mCallBackListener)(NULL, NULL);
            }
            
        }
    }
    
    if (action == 1) {
        CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
        //log("on api login");
        string namedf = "bvnet";
        string emaildf = "";
        if (document.HasParseError()) {
            return;
        }
        if (document.IsObject()) {
            std::string* id = new std::string("");
            std::string* name = new std::string("");
            std::string* email = new std::string("");
            if (!document.HasMember("id")){
                SceneManager::getSingleton().showChatWithCurrentScene(dataManager.GetSysString(601), -1);
                return;
            }
            else{
                rapidjson::Value& _id = document["id"];
                id = new std::string(_id.GetString(), 0, 1000);
            }
            if (document.HasMember("name")){
                rapidjson::Value& _name = document["name"];
                name = new std::string(_name.GetString(), 0, 1000);
            }
            else{
                name = new std::string(namedf, 0, 1000);
            }
            if (document.HasMember("email")){
                rapidjson::Value& _name = document["email"];
                email = new std::string(_name.GetString(), 0, 1000);
            }
            else{
                email = new std::string(emaildf, 0, 1000);
            }
            
            LayerLogin::getInstance()->socialInfo.uid = *id;//
            LayerLogin::getInstance()->socialInfo.name = *name;
            LayerLogin::getInstance()->socialInfo.email = *email;
            //            if (GameServer::getSingleton().getSmartFox()->IsConnecting()){
            //
            //                return;
            //            }
            LayerLogin::getInstance()->currLoginType = LayerLogin::facebookType;
            if (!GameServer::getSingleton().getSmartFox()->IsConnected())
            {
                SceneManager::getSingleton().connect2Server(0);
            }
            else
            {
                //GameServer::getSingleton().getSmartFox()->Disconnect();
                SceneManager::getSingleton().connect2Server(0);
            }
            //action = 2;
            //CCLOG("Facebook login id:'%s' name:'%s'", SceneLogin::getInstance()->socialInfo.name.c_str(), SceneLogin::getInstance()->socialInfo.uid.c_str());
        }
    }
    else if (action == 2) {
        //  CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
        // //log("on api profile");
        SceneManager::getSingleton().hideLoading();
        CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
        //log("on api login");
        if (document.HasParseError()) {
            return;
        }
        if (document.IsObject()) {
            std::string* id= new std::string("");
            if (!document.HasMember("id")){
                SceneManager::getSingleton().showChatWithCurrentScene(dataManager.GetSysString(618), -1);
                return;
            }
            else{
                rapidjson::Value& _id = document["id"];
                id = new std::string(_id.GetString(), 0, 1000);
            }
            if (mCallBack && mCallBackListener){
                (mCallBack->*mCallBackListener)(NULL, (void*)id);
            }
            
        }
        
    }
}
void mFaceBook::setFunCallBack(Ref *target, SEL_CallFuncND listener)
{
    mCallBack = target;
    mCallBackListener = listener;
}
void mFaceBook::onSharedSuccess(const std::string& message)
{
    CCLOG("##FB onSharedSuccess:%s", message.c_str());
    this->isShare = false;
    //MessageBox(message.c_str(), "share success");
    string* id = new string("2");
    if (mCallBack && mCallBackListener){
        (mCallBack->*mCallBackListener)(NULL, (void*)id);
    }
}
void mFaceBook::onSharedFailed(const std::string& message)
{
    CCLOG("##FB onSharedFailed:%s", message.c_str());
    this->isShare =  false;
    //MessageBox(message.c_str(), "share failed");
    auto currScene = Director::getInstance()->getRunningScene();
    if (currScene != NULL)
    {
        Chat* toast = Chat::create(dataManager.GetSysString(423), -1);
        currScene->addChild(toast);
    }
}
void mFaceBook::onSharedCancel()
{
    CCLOG("##FB onSharedCancel");
    this->isShare = false;
    if (dataManager.getAppConfig().isShareCancel){
        string* id = new string("2");
        if (mCallBack && mCallBackListener){
            (mCallBack->*mCallBackListener)(NULL, (void*)id);
        }
    }
    //MessageBox("", "share cancel");
}
void mFaceBook::onPermission(bool isLogin, const std::string& error)
{
    CCLOG("##FB onPermission: %d, error: %s", isLogin, error.c_str());
    
    std::string title = "permission ";
    title.append((isLogin ? "success" : "failed"));
    //cocos2d::MessageBox(error.c_str(), title.c_str());
    //if (isLogin)
    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (this->isShare) return;
    if (!this->isShare){
        this->onDialogLink();
        this->isShare = true;
    }
    //#endif
    // this->onShareLink();
}
void mFaceBook::onFetchFriends(bool ok, const std::string& msg)
{
    CCLOG("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());
    
    const std::vector<sdkbox::FBGraphUser>& friends = PluginFacebook::getFriends();
    for (int i = 0; i < friends.size(); i++)
    {
        const sdkbox::FBGraphUser& user = friends.at(i);
        CCLOG("##FB> -------------------------------");
        CCLOG("##FB>> %s", user.uid.data());
        CCLOG("##FB>> %s", user.firstName.data());
        CCLOG("##FB>> %s", user.lastName.data());
        CCLOG("##FB>> %s", user.name.data());
        CCLOG("##FB>> %s", user.isInstalled ? "app is installed" : "app is not installed");
        CCLOG("##FB");
    }
    
    //cocos2d::MessageBox("", "fetch friends");
}

void mFaceBook::onRequestInvitableFriends(const FBInvitableFriendsInfo& friends)
{
    for (auto it = friends.begin(); it != friends.end(); ++it) {
        CCLOG("Invitable friend: %s", it->getFirstName().c_str());
    }
}

void mFaceBook::onInviteFriendsWithInviteIdsResult(bool result, const std::string& msg)
{
    CCLOG("on invite friends with invite ids %s= '%s'", result ? "ok" : "error", msg.c_str());
    if (result){
        
        string* id = new std::string("1");
        if (mCallBack && mCallBackListener){
            (mCallBack->*mCallBackListener)(NULL, (void*)id);
        }
        
    }else{
        auto currScene = Director::getInstance()->getRunningScene();
        if (currScene != NULL)
        {
            Chat* toast = Chat::create(dataManager.GetSysString(423), -1);
            currScene->addChild(toast);
        }
    }
}

void mFaceBook::onInviteFriendsResult(bool result, const std::string& msg)
{
    CCLOG("on invite friends %s= '%s'", result ? "ok" : "error", msg.c_str());
    
}

void mFaceBook::onGetUserInfo(const sdkbox::FBGraphUser& userInfo)
{
    CCLOG("Facebook id:'%s' name:'%s' last_name:'%s' first_name:'%s' email:'%s' installed:'%d'",
          userInfo.getUserId().c_str(),
          userInfo.getName().c_str(),
          userInfo.getFirstName().c_str(),
          userInfo.getLastName().c_str(),
          userInfo.getEmail().c_str(),
          userInfo.isInstalled ? 1 : 0
          );
    //LayerLogin::getInstance()->socialInfo.name = userInfo.getName();
    //LayerLogin::getInstance()->socialInfo.uid = userInfo.getUserId();
    //LayerLogin::getInstance()->socialInfo.email = userInfo.getEmail();
    //
    // CCLOG("Facebook login id:'%s' name:'%s'",LayerLogin::getInstance()->socialInfo.name.c_str(),LayerLogin::getInstance()->socialInfo.uid.c_str());
    //LayerLogin::getInstance()->doLogin(1);
}
int mFaceBook::LengthUtf8String(const char* s) {
    int i = 0, j = 0;
    while (s[i]) {
        if ((s[i] & 0xC0) != 0x80)
            j++;
        i++;
    }
    return j;
}
#endif
