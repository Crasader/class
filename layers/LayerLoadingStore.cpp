//
//  LayerLoadingStore.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerLoadingStore.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../scenes/SceneMain.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "Requests/ExtensionRequest.h"
#include "LayerPopupThongBao.h"
#include "LayerGuideInGame.h"
#include "../readJSON.h"
//#include "../Animal/Scene/MainScene.hpp"
//#include "../popstar/StartLayer.h"
#include "../english/LayerMenuGame.hpp"
#include "../english/GameManager.hpp"

#include <fstream>
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif

template<> LayerLoadingStore* SingLeton<LayerLoadingStore>::mSingleton = 0;

LayerLoadingStore* LayerLoadingStore::getSingletonPtr(void)
{
    return mSingleton;
}

LayerLoadingStore& LayerLoadingStore::getSingleton(void)
{
    assert(mSingleton);
    return (*mSingleton);
}

LayerLoadingStore::LayerLoadingStore()
{
//    GameServer::getSingleton().addListeners(this);
    
}


LayerLoadingStore::~LayerLoadingStore()
{
//    GameServer::getSingleton().removeListeners(this);
    
}
cocos2d::Scene* LayerLoadingStore::createScene()
{
    auto scene = Scene::create();
    
    auto layer = LayerLoadingStore::create();
    layer->setTag(2468);//cheat for get scene main
    scene->addChild(layer);
    
    return scene;
}
bool LayerLoadingStore::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    
    auto rootNode = CSLoader::getInstance()->createNode("LayerLoadingStore.csb");
    auto imgBg = static_cast<ImageView*>(rootNode->getChildByName("imgBg"));
    
    imgBg->setScaleY(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
        
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());

    
    loadingBar = static_cast<LoadingBar*>(rootNode->getChildByName("loadingBar"));

    //loadingBar->setScale9Enabled(true);
    //loadingBar->set
    loadingBar->setPercent(0.00);
    //loadingBar->s
    txtLoading = static_cast<Text*>(rootNode->getChildByName("txtLoading"));
    SceneManager::getSingleton().hideLoading();
    
    bool isDownload = UserDefault::getInstance()->getBoolForKey("isdownload", false);
    if(!isDownload){
        txtLoading->setString("Loading");
    }else{
        txtLoading->setString("កំពុងធ្វើបច្ចុប្បន្នភាពទិន្នន័យ");
    }
    return true;
}

void LayerLoadingStore::gotoFakeLoading(float dt){
    this->LoadStrings("englishgame/data.conf");
    
    auto pScene = LayerMenuGame::createScene();
//    auto pScene = StartLayer::scene();
    if (pScene == NULL)
        return;
    Director::getInstance()->replaceScene(TransitionFade::create(0.0, pScene));
//    SceneManager::getSingleton().gotoLogin();
}
void LayerLoadingStore::gotoGame(float dt){
    auto scene = SceneMain::createScene();
    if (scene == NULL)
        return;
    Director::getInstance()->replaceScene(TransitionFade::create(0.0, scene));
    SceneManager::getSingleton().gotoLogin();
    UserDefault::getInstance()->setBoolForKey("isdownload", true);
    UserDefault::getInstance()->flush();
}
void LayerLoadingStore::startDownloadResouces()
{
    
 
    
    if (SceneManager::getSingleton().getIsFlagStore()){
        loadingBar->setPercent(100);
        this->scheduleOnce(schedule_selector(LayerLoadingStore::gotoFakeLoading),1);
        return;
    }
    
    if(this->isDownloading)
        return;
    this->isDownloading = true;
    
    
    bool isHasInternet = true;
#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    isHasInternet = NativeHelper::hasConnection();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    isHasInternet = IOS::tryIsInternetConnection();
#endif
    
    if (!isHasInternet)
    {
        this->showPopupFailed(1);
       
        return;
    }
    
    string manifestPath = "localedata/project.manifest";
    string storagePath = FileUtils::getInstance()->getWritablePath() + "project/";
    string name = FileUtils::getInstance()->fullPathForFilename(manifestPath);
    ssize_t size = 0;

    unsigned char* titlech = FileUtils::getInstance()->getFileData(name, "r", &size);
    if (titlech == NULL){
        log("deo co");
        std::vector<std::string> data = {"http://13.251.217.47/winclubupdate2/","http://13.251.217.47/winclubupdate2/project.manifest","http://13.251.217.47/winclubupdate2/version.manifest","1.0.0","3.0 beta"};
//        std::vector<std::string> data = {"http://13.251.217.47/winplayupdate/","http://13.251.217.47/winplayupdate/project.manifest","http://13.251.217.47/winplayupdate/version.manifest","1.0.0","3.0 beta"};
        readJSON::createJSON(data);
    }
    _am = AssetsManagerEx::create(manifestPath, storagePath);
    _am->retain();
    SceneManager::getSingleton().hideLoading();
    if (!_am->getLocalManifest()->isLoaded())
    {
        //txtLoading->setString("Fail to update assets, step skipped.");
        this->showPopupFailed(1);

        return;
    }
    else{
        int testIndex = 0;
        cocos2d::extension::EventListenerAssetsManagerEx *amListener = cocos2d::extension::EventListenerAssetsManagerEx::create(_am, [=](EventAssetsManagerEx* event){
            static int failCount = 0;
            switch (event->getEventCode())
            {
                case EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST:
                {
                    //txtLoading->setString("No local manifest file found, skip assets update.");
                    this->showPopupFailed(1);

                    //this->onLoadEnd();
                }
                    break;
                case EventAssetsManagerEx::EventCode::UPDATE_PROGRESSION:
                {
                    string assetID = event->getAssetId();
                    float percent = event->getPercent();
                    string str = "";
                    if (assetID == AssetsManagerEx::VERSION_ID)
                    {
                        str = "ធ្វើបច្ចុប្បន្នភាពទិន្នន័យសូមកុំបិទកម្មវិធី";

                    }
                    else if (assetID == AssetsManagerEx::MANIFEST_ID)
                    {
                        str = "ធ្វើបច្ចុប្បន្នភាពទិន្នន័យសូមកុំបិទកម្មវិធី";

                    }
                    else
                    {
                        str = StringUtils::format("កំពុងទាញយក game %.0f", percent) + "%";
                        if (percent >= 100){
                            this->unschedule(schedule_selector(LayerLoadingStore::timeOutDownload));
                            str = "ធ្វើបច្ចុប្បន្នភាពទិន្នន័យសូមកុំបិទកម្មវិធី";
                        }
                    }
                    
                    loadingBar->setPercent(percent);
                    SceneManager::getSingleton().hideLoading();
                    txtLoading->setString(str);
                    timeoutDownLoad = 0;
                
                }
                    break;
                case EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST:
                case EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST:
                {
                   // SceneManager::getSingleton().showToast("Fail to download manifest file, update skipped.");
                    this->showPopupFailed(1);

//
//                    this->setFlagStore(true);
//                    this->gotoLogin();
                }
                    break;
                case EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE:
                case EventAssetsManagerEx::EventCode::UPDATE_FINISHED:
                {
                    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("card_small.plist");
                    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gold_coin.plist");
                    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("silver_coin.plist");
                    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("card_big.plist");
                    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animxocdia.plist");
                    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("chan_new.plist");
                    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("chess.plist");
                    Sprite* sprite = Sprite::create("particles/bubble.png");
                    timeoutDownLoad = 0;
                    this->unschedule(schedule_selector(LayerLoadingStore::timeOutDownload));

                    if (sprite == NULL){
                        _am->setState(1);
                        loadingBar->setPercent(100);
                        txtLoading->setString("ធ្វើបច្ចុប្បន្នភាពទិន្នន័យសូមកុំបិទកម្មវិធី");
                        this->removeChildByTag(24);
                        SceneManager::getSingleton().hideLoading();
                        return;
                    }
                    
                    this->scheduleOnce(schedule_selector(LayerLoadingStore::gotoGame),1);


            
                }
                    break;
                case EventAssetsManagerEx::EventCode::UPDATE_FAILED:
                {
                    log("%s", event->getMessage().c_str());
                    //SceneManager::getSingleton().showToast(StringUtils::format("Update failed. %s", event->getMessage().c_str()));
                    
                    failCount++;
                    if (failCount < 5)
                    {
                        _am->downloadFailedAssets();
                    }
                    else
                    {
                       // SceneManager::getSingleton().showToast("Reach maximum fail count, exit update process");
                        this->showPopupFailed(1);

                        failCount = 0;
                    }
                }
                    break;
                case EventAssetsManagerEx::EventCode::ERROR_UPDATING:
                {
                    //SceneManager::getSingleton().showToast(StringUtils::format("Asset %s : %s", event->getAssetId().c_str(), event->getMessage().c_str()));
                   // this->showPopupFailed(1);

                }
                    break;
                case EventAssetsManagerEx::EventCode::ERROR_DECOMPRESS:
                {
                    this->showPopupFailed(1);

                   // SceneManager::getSingleton().showToast(StringUtils::format("%s", event->getMessage().c_str()));
                }
                    break;
                    
                default:
                    break;
            }
            
        });
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(amListener, 1);
        this->scheduleOnce(schedule_selector(LayerLoadingStore::delayUpDate),2);

    }
}
void LayerLoadingStore::delayUpDate(float dt){
    _am->update();

    this->schedule(schedule_selector(LayerLoadingStore::timeOutDownload),1);

}
void LayerLoadingStore::showPopupFailed(int tag){
    timeoutDownLoad = 0;
    this->unschedule(schedule_selector(LayerLoadingStore::timeOutDownload));
    SceneManager::getSingleton().hideLoading();
    this->removeChildByTag(24);
    LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
    layerPopup->setCallBackFromButton(this, callfuncO_selector(LayerLoadingStore::layerPopupThongBaoCallBack), tag);
    layerPopup->setMessage("ប្រកាស");
    layerPopup->setContentMess("មានកំហុសក្នុងការជួសជុលលើផ្លូវអីនធើណេត។សូមមេត្តាពិនិត្យលើបណ្តាញដែលឆ្លកាត");
    layerPopup->setPopupClose();
    layerPopup->setEndGameWhenCancle();
    if (layerPopup->getBtnOk()->getTitleText() == ""){
        layerPopup->getBtnOk()->setTitleText("យល់ព្រម");
        layerPopup->getBtnHuy()->setTitleText("ចេញ");
    }
    layerPopup->setTag(24);
    layerPopup->setEndGameWhenCancle();
    this->addChild(layerPopup);
    SceneManager::getSingleton().hideLoading();
}
void LayerLoadingStore::layerPopupThongBaoCallBack(Ref *pSender)
{
    SceneManager::getSingleton().showLoading();
    timeoutDownLoad = 0;
    int tag = ((Node*)pSender)->getTag();

    if (tag == 1){
        bool isHasInternet = true;
#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        isHasInternet = NativeHelper::hasConnection();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        isHasInternet = IOS::tryIsInternetConnection();
#endif
        
        if (!isHasInternet)
        {
            this->removeChildByTag(24);
            this->showPopupFailed(1);
            this->unschedule(schedule_selector(LayerLoadingStore::timeOutDownload));
            return;
        }
        //_am->release();
        //if (_am->)
        _am->setState(0);
        _am->update();
        this->removeChildByTag(24);
        this->schedule(schedule_selector(LayerLoadingStore::timeOutDownload),1);
    }else if (tag == 2){
        SceneManager::getSingleton().connect2Server(1);
    }else if (tag == 3){
        SceneManager::getSingleton().onScheduleGetConfig(1);
    }
//    else if (tag == 4){
//
//
//    }
    this->removeChildByTag(24);

}
void LayerLoadingStore::timeOutDownload(float dt){
    return;
    timeoutDownLoad++;
    if (timeoutDownLoad == 3){
        this->showPopupFailed(1);
    }
}

void LayerLoadingStore::onEnter()
{
    Layer::onEnter();
    
}

void LayerLoadingStore::onExit()
{
    Layer::onExit();
    //_am->release();
    this->unschedule(schedule_selector(LayerLoadingStore::gotoFakeLoading));
    this->unschedule(schedule_selector(LayerLoadingStore::timeOutDownload));
    this->unschedule(schedule_selector(LayerLoadingStore::delayUpDate));

}


///Đọc dữ liệu con game fake English

void LayerLoadingStore::LoadStrings(const char* file) {
    string str = FileUtils::getInstance()->getStringFromFile(file);
    
    for(int i = 0; i < 51; ++i)
        _sysStrings[i] = "";
    
    vector<string> lstStrings = this->splitString(str, '\n');
    for (int i = 3; i < lstStrings.size(); i++){
        string sDatas = lstStrings.at(i);
        if (sDatas.at(0) == '#')
            continue;
        size_t f = sDatas.find_first_of(' ');
        if (f == string::npos)
            continue;
        sDatas = sDatas.substr(f + 1, sDatas.size());
        f = sDatas.find_first_of(' ');
        if (f == string::npos)
            continue;
        
        int val = atoi(sDatas.substr(0, f).c_str());
        GameManager::getInstance()->_dataStrings[val] = sDatas.substr(f + 1, sDatas.size());
    }
    
}

vector<string> LayerLoadingStore::splitString(string &S, const char &str){
    vector<string> arrStr;
    string::iterator t, t2;
    for (t = S.begin(); t < S.end();){
        t2 = find(t, S.end(), str);
        if (t != t2)
            arrStr.push_back(string(t, t2));
        else  if (t2 != S.end())
            arrStr.push_back("");
        if (t2 == S.end())
            break;
        t = t2 + 1;
    }
    return arrStr;
}
