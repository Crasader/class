#include "LayerConfigInGame.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "mUtils.h"
#include "layergames/PhomMessDef.h"
LayerConfigInGame::LayerConfigInGame()
{

}

LayerConfigInGame::~LayerConfigInGame()
{

}

bool LayerConfigInGame::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->gameId = SceneManager::getSingleton().getGameID();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupSettingInGame.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	if (gameId == kGameChan || gameId == kGameCoTuong || gameId == kGameCoUp){
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


    auto label_setting_music = static_cast<Text*> (rootNode->getChildByName("label_setting_music"));
    label_setting_music->setString(dataManager.GetSysString(751));
    label_setting_music->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    
    auto title_setting_invite = static_cast<Text*> (rootNode->getChildByName("label_setting_moichoi"));
    title_setting_invite->setString(dataManager.GetSysString(752));
    title_setting_invite->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    
    auto title_setting_rung = static_cast<Text*> (rootNode->getChildByName("label_setting_rung"));
    title_setting_rung->setString(dataManager.GetSysString(933));
    title_setting_rung->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    
    auto title_setting = static_cast<Text*> (rootNode->getChildByName("title_setting"));
    title_setting->setString(dataManager.GetSysString(749));
    
    auto title_setting_goiy = static_cast<Text*> (rootNode->getChildByName("lbl_goiy"));
    if (SceneManager::getSingleton().currLang == 1)
        title_setting_goiy->setFontSize(42);
    title_setting_goiy->setString(dataManager.GetSysString(1038));
    title_setting_goiy->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
//case kGameDemLaSolo:
//case kGameSamSolo:
//case kGameTLMNDemLa:
    UserDefault *def = UserDefault::getInstance();

    CheckBox *chbGoiY = dynamic_cast<CheckBox*>(rootNode->getChildByName("chbGoiY"));
    if (chbGoiY){
        chbGoiY->setTag(10);
        chbGoiY->setSelected(def->getBoolForKey("chbRecommend", false));
        chbGoiY->addEventListener(CC_CALLBACK_2(LayerConfigInGame::selectedEvent, this));
    }
    
    if ((SceneManager::getSingleton().getGameID() != kGameTLMNDemLa && SceneManager::getSingleton().getGameID() != kGameDemLaSolo) || !dataManager.getAppConfig().isrecommend){
        title_setting_goiy->setVisible(false);
        chbGoiY->setVisible(false);
    }

	auto btnClose = dynamic_cast<Button*>(rootNode->getChildByName("btnClose"));
	if (btnClose){
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerConfigInGame::onBtnClose, this));
	}
	CheckBox *chbMusic = dynamic_cast<CheckBox*>(rootNode->getChildByName("chbMusic"));
	if (chbMusic){
		chbMusic->setTag(0);
		chbMusic->setSelected(def->getBoolForKey("sound", true));
		chbMusic->addEventListener(CC_CALLBACK_2(LayerConfigInGame::selectedEvent, this));
	}
	auto labelVibrate = dynamic_cast<Text*>(rootNode->getChildByName("lblRung"));
	if (labelVibrate)
	{
		labelVibrate->setString("Rung");
	}
	CheckBox *chbVibrate = dynamic_cast<CheckBox*>(rootNode->getChildByName("chbRung"));
	if (chbVibrate){
		chbVibrate->setTag(1);
		chbVibrate->setSelected(def->getBoolForKey("vibrate", true));
		chbVibrate->addEventListener(CC_CALLBACK_2(LayerConfigInGame::selectedEvent, this));
	}
	Text* label_setting_moichoi_0 = dynamic_cast<Text*>(rootNode->getChildByName("label_setting_moichoi_0"));
	CheckBox* chbRecommend = dynamic_cast<CheckBox*>(rootNode->getChildByName("chbInvite"));
	if (chbRecommend){
//		if (gameId == kGameSam || gameId == kGameTienLenMienNam || gameId == kGameTLMNDemLa ||
//			gameId == kGameSamSolo || gameId == kGameDemLaSolo)
//		{
			chbRecommend->setTag(2);
			chbRecommend->setSelected(def->getBoolForKey("denyAllInvite", false));
			chbRecommend->addEventListener(CC_CALLBACK_2(LayerConfigInGame::selectedEvent, this));
//		}
//		else{
//			chbRecommend->setVisible(false);
//			if (label_setting_moichoi_0)
//				label_setting_moichoi_0->setVisible(false);
//		}

	}
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

	return true;
}

void LayerConfigInGame::onBtnClose(Ref* pSender)
{
	//bool isOn = isRecommendOn();
	this->removeFromParentAndCleanup(true);
}
void LayerConfigInGame::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerConfigInGame::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void LayerConfigInGame::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(listener);
	Layer::onExit();
}

void LayerConfigInGame::selectedEvent(Ref *pSender, CheckBox::EventType type)
{
	CheckBox *chb = dynamic_cast<CheckBox*>(pSender);
	string key;
	switch (chb->getTag())
	{
	case 0://checkbox am thanh
		key.append("sound");
		break;
	case 1://check box rung
		key.append("vibrate");
		break;
	case 2:
		key.append("denyAllInvite");
		break;
    case 10:
        key.append("chbRecommend");
        break;
	default:
		break;
	}
	UserDefault *def = UserDefault::getInstance();
	def->setBoolForKey(key.c_str(), type == cocos2d::ui::CheckBox::EventType::SELECTED);
	def->flush();

}
