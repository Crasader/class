#include "LayerSettingMain.h"
#include "cocostudio/CocoStudio.h"
#include "../scenes/SceneMain.h"
#include "../mUtils.h"
#include "layergames/LayerGa.h"
#include "Entities/Variables/SFSUserVariable.h"
#include "Requests/SetUserVariablesRequest.h"
#include "../scenes/ScenePickRoom.h"
USING_NS_CC;


LayerSettingMain::LayerSettingMain()
{

}


LayerSettingMain::~LayerSettingMain()
{

}

bool LayerSettingMain::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupSettingMain.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	if (gameID == kGameChan || gameID == kGameXiTo){
		rootNode->setAnchorPoint(Point::ZERO);
		rootNode->setPosition(Point::ZERO);
	}
	else
	{
		rootNode->setAnchorPoint(Point(0.5, 0.5));
		rootNode->setPosition(Point(visibleSize / 2));
	}
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());

    auto title_setting = static_cast<Text*> (rootNode->getChildByName("title_setting"));
    title_setting->setString(dataManager.GetSysString(749));
    
    auto label_setting_ga = static_cast<Text*> (rootNode->getChildByName("label_setting_music_1"));
    label_setting_ga->setString(dataManager.GetSysString(750));
    label_setting_ga->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    auto label_setting_music = static_cast<Text*> (rootNode->getChildByName("label_setting_music"));
    label_setting_music->setString(dataManager.GetSysString(751));
    label_setting_music->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);

    auto title_setting_invite = static_cast<Text*> (rootNode->getChildByName("label_setting_music_0"));
    title_setting_invite->setString(dataManager.GetSysString(752));
    title_setting_invite->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);

	//add event listenner
	//tranng thai setSelectedState = true hien tai dang la tat 
	_buttonToggleMusic = static_cast<CheckBox*>(rootNode->getChildByName("chbMusic"));
	UserDefault *def = UserDefault::getInstance();
    auto a = def->getBoolForKey("sound", false);
	if (_buttonToggleMusic != NULL){
		_buttonToggleMusic->addEventListener(CC_CALLBACK_2(LayerSettingMain::selectedEvent, this));
		_buttonToggleMusic->setSelected(def->getBoolForKey("sound", false));
        _buttonToggleMusic->setTag(100);
	}
	_buttonToggleRung = static_cast<CheckBox*>(rootNode->getChildByName("chbVibrate"));
	if (_buttonToggleRung != NULL){
		_buttonToggleRung->addEventListener(CC_CALLBACK_2(LayerSettingMain::selectedEvent, this));
		_buttonToggleRung->setSelected(def->getBoolForKey("denyAllInvite", true));
        _buttonToggleRung->setTag(101);

	}
	_btnClose = static_cast<Button*>(rootNode->getChildByName("btnClose"));
	if (_btnClose != NULL){
		_btnClose->addClickEventListener(CC_CALLBACK_1(LayerSettingMain::onButtonCloseClicked, this));
	}
	auto logout = dynamic_cast<Button*>(rootNode->getChildByName("btnExit"));
	if (logout)
	{
		logout->addClickEventListener(CC_CALLBACK_1(LayerSettingMain::onBtnLogout, this));
        logout->setTitleText(dataManager.GetSysString(753));
	}
	CheckBox* chbChicken = (CheckBox*)rootNode->getChildByName("chbChicken");
	if (chbChicken)
	{
		bool uck = def->getBoolForKey("UCK", true);
		chbChicken->setSelected(uck);
		chbChicken->setTag(102);
		chbChicken->addEventListener(CC_CALLBACK_2(LayerSettingMain::selectedEvent, this));
		// 		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		// 		if (mySelf)
		// 		{
		// 			boost::shared_ptr<UserVariable> uckptr = mySelf->GetVariable("uck");
		// 			if (uckptr)
		// 			{
		// 				int uck = *uckptr->GetIntValue();
		// 				//if (uck == LayerGa::CONFIG::ON)
		// 				chbChicken->setSelectedState(uck == LayerGa::CONFIG::ON);
		// 			}
		// 			else{
		// 				chbChicken->setSelectedState(true);
		// 			}
		// 		}
	}
	Text* lbl_mail_support = (Text*)rootNode->getChildByName("lbl_mail_support");
	if (lbl_mail_support)
	{
		lbl_mail_support->setTextHorizontalAlignment(TextHAlignment::LEFT);
		lbl_mail_support->setString(dataManager.getAppConfig().mailSup);
	}

	CheckBox* chbRecommend = dynamic_cast<CheckBox*>(rootNode->getChildByName("chbRecommend"));
	if (chbRecommend){
		chbRecommend->setTag(103);
		chbRecommend->setSelected(def->getBoolForKey("chbRecommend", false));
		chbRecommend->addEventListener(CC_CALLBACK_2(LayerSettingMain::selectedEvent, this));
	}
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return true;
    ScenePickRoom* _scene = (ScenePickRoom*)_currScene->getChildByTag(8866);
    if (_scene)
    {
        _scene->hideWebView();
    }
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
	return true;
}

bool LayerSettingMain::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerSettingMain::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerSettingMain::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerSettingMain::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
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

void LayerSettingMain::onButtonCloseClicked(Ref* pSender){
	this->removeFromParentAndCleanup(true);
}

void LayerSettingMain::onBtnLogout(Ref* pSender)
{
	/*SceneMain* _scene = (SceneMain*)this->getParent();
	_scene->onButtonBackPress();*/
    SceneManager::getSingleton().showLoading();
	UserDefault *def = UserDefault::getInstance();
	def->setBoolForKey("UCK", true);
	def->flush();
	boost::shared_ptr<IRequest> request(new LogoutRequest());
	GameServer::getSingleton().Send(request);
	//
	SceneManager::getSingleton().isLogout = true;
	SceneManager::getSingleton().rcForLogin = false;
	SceneManager::getSingleton().gotoLogin();
    this->removeFromParent();
}

void LayerSettingMain::selectedEvent(Ref *pSender, CheckBox::EventType type)
{
	CheckBox *chb = dynamic_cast<CheckBox*>(pSender);
	/*tagMusic: 100,
	tagVibrate: 101,*/
	UserDefault *def = UserDefault::getInstance();
	switch (chb->getTag()) {
	case 100:
		def->setBoolForKey("sound", type == cocos2d::ui::CheckBox::EventType::SELECTED);
        if (!mUtils::isSoundOn()){
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        }else{
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bm-theme_v3.mp3",true);
        }
		def->flush();
		break;
	case 101:
		def->setBoolForKey("denyAllInvite", type == cocos2d::ui::CheckBox::EventType::SELECTED);
		def->flush();
		break;
	case 102://bat tag ga
	{
				 boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
				 boost::shared_ptr<int> _a(new int(type == cocos2d::ui::CheckBox::EventType::SELECTED ? LayerGa::CONFIG::ON : LayerGa::CONFIG::OFF));
				 boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("uck", _a, VARIABLETYPE_INT));
				 collectionUserVariable->push_back(variable);	//
				 boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
				 GameServer::getSingleton().Send(request);
        
        
				 def->setBoolForKey("UCK", type == cocos2d::ui::CheckBox::EventType::SELECTED);
				 def->flush();
	}
		break;
	case 103:{//bat tat goi y danh bai
				 def->setBoolForKey("chbRecommend", type == cocos2d::ui::CheckBox::EventType::SELECTED);
				 def->flush();
	}
		break;
	}
}
