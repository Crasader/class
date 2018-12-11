#include "AppDelegate.h"
#include "HelloWorldScene.h"
//#ifdef SDKBOX_ENABLED
//#include "PluginAdMob/PluginAdMob.h"
//#endif
//#ifdef SDKBOX_ENABLED
//#include "PluginSdkboxAds/PluginSdkboxAds.h"
//#endif
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#include "Sdkbox/Sdkbox.h"
#include "PluginFacebook/PluginFacebook.h"
#endif
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS||CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#include "PluginIAP/PluginIAP.h"
#endif
#include "SceneManager.h"
#include "layercaro/FakeScene.h"
#include "layergames/PhomMessDef.h"
//anhnt only download
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#include "PluginAdMob/PluginAdMob.h"
#endif
USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1079);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
#ifdef SDKBOX_ENABLED
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    sdkbox::PluginAdMob::init();
//    sdkbox::PluginSdkboxAds::init();
#endif
#endif
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#if SDKBOX_ENABLED
	sdkbox::PluginFacebook::init();
#endif
#endif
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS||CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID )
	sdkbox::IAP::init();
#endif


	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {

		glview = cocos2d::GLViewImpl::create("BaiVipApp");
		//glview->setFrameSize((1920 * 0.85), (1280 * 0.85)); //16:9
		glview->setFrameSize(1334, 750);
		glview->setFrameZoomFactor(0.6);
		//glview->setFrameSize(1024, 768); //4:3
		//glview->setFrameSize(960 * 1.6, 640 * 1.6); //3:2

		director->setOpenGLView(glview);
	}
	/* set design resolution to same resolution as what you published it in Cocos Studio
	and use FIXED_WIDTH for landscape & FIXED_HEIGHT for portrait*/

//	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	/*float visibleRate = visibleSize.width / visibleSize.height;
	float designRate = designResolutionSize.width / designResolutionSize.height;
	if (visibleRate < designRate){
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
	}
	else {
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
	}*/

    const auto screenSize = glview->getFrameSize();
    const int screenRatio = screenSize.height * 10/screenSize.width;
   // log("screenSize--------width:%f----height:%f",screenSize.width,screenSize.height);
    if (screenRatio == 13) //1.333
    {
        
        glview->setDesignResolutionSize(800,600,ResolutionPolicy::FIXED_WIDTH);
    }
    else if (screenRatio == 15) //1.5
    {
        glview->setDesignResolutionSize(800,533,ResolutionPolicy::FIXED_WIDTH);
    }
    else if (screenRatio == 16) //1.6
    {
        glview->setDesignResolutionSize(800,480,ResolutionPolicy::FIXED_WIDTH);
    }
    else if (screenRatio == 17) //1.7
    {
        glview->setDesignResolutionSize(854,480,ResolutionPolicy::FIXED_WIDTH);
    }
    else if (screenRatio == 4 || screenRatio == 5) //1.7
    {
        glview->setDesignResolutionSize(1920,1080,ResolutionPolicy::SHOW_ALL);
    }
    else
    {
        glview->setDesignResolutionSize(1920,1080,ResolutionPolicy::FIXED_WIDTH);
    }
	// turn on display FPS
	director->setDisplayStats(false);
	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);
	//set encrypt key
	Configuration::getInstance()->setValue("non_en", Value("nonen@"));
	Configuration::getInstance()->setValue("encryptkey", Value(""));
	//game version 0 -bv, 1 -bm
	Configuration::getInstance()->setValue("gameversion", Value("0"));
	//Config goi y danh bai tien len + sam : 0 la bat , 1 la tat
//    bool isOn = UserDefault::getInstance()->getBoolForKey("chbRecommend", true);
//    if (isOn)
//        Configuration::getInstance()->setValue("onRecommend", Value("true"));
//    else
//        Configuration::getInstance()->setValue("onRecommend", Value("false"));

//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gamechan.plist");
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("card_small.plist");
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gold_coin.plist");
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("silver_coin.plist");
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("card_big.plist");
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animxocdia.plist");
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("chan_new.plist");
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("chess.plist");
    register_all_packages();
//	int _gamever = Configuration::getInstance()->getValue("gameversion").asInt();
//	if (_gamever == GameVersion::p1)
//	{
//		SceneManager* _sceneManager = SceneManager::create();
//		_sceneManager->retain();
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID|| CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//		_sceneManager->filterIP();
//#else(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//		_sceneManager->gotoLogin();
//#endif
//	}
//	else{
		SceneManager* _sceneManager = SceneManager::create();
		_sceneManager->retain();
    //anhnt test download resource
	//	_sceneManager->gotoLogin();
//	}

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

     //if you use SimpleAudioEngine, it must be pause
     //SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	 //SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
}

