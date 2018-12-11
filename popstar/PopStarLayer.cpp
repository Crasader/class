#include "PopStar.h"
//#include "../extensions/CocoStudio/gui/UIWidgets/UILabel.h"
#include "GameScene.h"
#include "PopStarLayer.h"
#include "PopStarDataMgr.h"
#include "GameLevel.h"
#include "StartLayer.h"
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#include "PluginAdMob/PluginAdMob.h"
#endif
USING_NS_CC;

PopStarLayer::~PopStarLayer()
{
	if (popStar)
	{
		delete popStar;
		popStar = NULL;
	}
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
}

// on "init" you need to initialize your instance
bool PopStarLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	srand( (unsigned)time( NULL ) );

	float scale = visibleSize.height/1080;
	Sprite* popStarBg = Sprite::create("backgrounds/bg.jpg");
	popStarBg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	popStarBg->setScale(scale);
	this->addChild(popStarBg, 0);
    
    
    MenuItemImage* item = MenuItemImage::create("buttons/default/exit.png",
                                                "buttons/default/exit.png",
                                                "buttons/default/exit.png",
                                                this,
                                                menu_selector(PopStarLayer::onClickMenu));
    item->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    item->setPosition(Vec2(10, 10));
    item->setScale(1.0, 1.0);
    
    Menu* menu = Menu::create(item, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    historyTotalScore = Label::createWithSystemFont("0", "Arial", 44);
	if (historyTotalScore)
	{
        historyTotalScore->setHorizontalAlignment(TextHAlignment::CENTER);

        historyTotalScore->setAnchorPoint(Vec2(0,1));
		historyTotalScore->setPosition(Vec2(10, visibleSize.height-100));
		this->addChild(historyTotalScore);
        historyTotalScore->setVisible(false);

	}

	historyLevelScore = Label::createWithSystemFont("0", "Arial", 44);
	if (historyLevelScore)
	{
        historyLevelScore->setHorizontalAlignment(TextHAlignment::CENTER);

        historyLevelScore->setAnchorPoint(Vec2(0,1));
		historyLevelScore->setPosition(Vec2(10, visibleSize.height-200));
		this->addChild(historyLevelScore);
        historyLevelScore->setVisible(false);
	}

	gameLevel = Label::createWithSystemFont("0", "Arial", 44);
	if (gameLevel)
	{
        gameLevel->setHorizontalAlignment(TextHAlignment::CENTER);

        gameLevel->setAnchorPoint(Vec2(0,1));
		gameLevel->setPosition(Vec2(10, 1080-300));
		this->addChild(gameLevel);
	}

	targetScore = Label::createWithSystemFont("0", "Arial", 44);
	if (targetScore)
	{
        targetScore->setHorizontalAlignment(TextHAlignment::CENTER);

        targetScore->setAnchorPoint(Vec2(0,1));
		targetScore->setPosition(Vec2(10, visibleSize.height-200));
		this->addChild(targetScore);
	}

	curScore = Label::createWithSystemFont("0", "Arial", 44);
	if (curScore)
	{
        curScore->setAnchorPoint(Vec2(1,1));
        curScore->setHorizontalAlignment(TextHAlignment::CENTER);

		curScore->setPosition(Vec2(visibleSize.width-10, visibleSize.height-200));
		this->addChild(curScore);
	}

	reduceScore = Label::createWithSystemFont("", "Arial", 44);
	if (reduceScore)
	{
        reduceScore->setAnchorPoint(Vec2(1,1));
		reduceScore->setPosition(Vec2(visibleSize.width-10, visibleSize.height-300));
        reduceScore->setHorizontalAlignment(TextHAlignment::CENTER);
		this->addChild(reduceScore);
	}

	popStar = new PopStar(this);

	this->schedule(schedule_selector(PopStarLayer::update));
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    sdkbox::PluginAdMob::show("top_banner");
#endif
    //sdkbox::PluginAdMob::
	return true;
}
void PopStarLayer::onClickMenu(Ref* obj)
{
    changeLayer( this, StartLayer::create() );
}

void PopStarLayer::onEnter()
{
	Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(PopStarLayer::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
	//this->setTou(true);
}

bool PopStarLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
	Vec2 locInView = pTouch->getLocation();
	if (popStar)
	{
		popStar->onClick(locInView);
	}
    return true;
}

void PopStarLayer::update(float delta)
{
	if (!popStar)
	{
		return;
	}

	popStar->onUpdate(delta);
}

void PopStarLayer::onGuiEvent(GUI_EVENT_TYPE event, int nValue, unsigned int uValue)
{
	switch (event)
	{
	case EVENT_UPDATE_SCORE:
		{
			if (curScore)
			{
				char szBuf[32] = {0};
				//itoa(nValue, szBuf, 10);
                sprintf(szBuf, "Current Score :\n %d", nValue);
				curScore->setString(szBuf);
			}
            
		}
		break;
	case EVENT_UPDATE_TARGET_SCORE:
		{
			if (targetScore)
			{
				char szBuf[32] = {0};
				//itoa(nValue, szBuf, 10);
                sprintf(szBuf, "Target Score :\n %d", nValue);
				targetScore->setString(szBuf);
			}
		}
		break;
	case EVENT_UPDATE_LEVEL:
		{
			if (gameLevel)
			{
				char szBuf[32] = {0};
				//itoa(nValue, szBuf, 10);
                sprintf(szBuf, "Game Level :\n %d", nValue);
				gameLevel->setString(szBuf);
			}
		}
		break;
	case EVENT_UPDATE_REDUCE_SCORE:
		{
			if (reduceScore)
			{
				if (nValue == 0)
				{
					reduceScore->setString("");
				}
				else
				{
					char szBuf[32] = {0};
					snprintf(szBuf, 32, "Number:\n %d \n Score:\n %u", nValue, uValue);
					reduceScore->setString(szBuf);
				}
			}
		}
		break;
	case EVENT_UPDATE_TOTAL_HISTORY_SCORE:
		{
			if (historyTotalScore)
			{
				char szBuf[32] = {0};
                snprintf(szBuf, 32, "History Score : %d", getPopStarDataMgr().getHistoryScore());
				historyTotalScore->setString(szBuf);
			}
		}
		break;
	case EVENT_UPDATE_LEVEL_HISTORY_SCORE:
		{
			if (historyLevelScore)
			{
				char szBuf[32] = {0};
                snprintf(szBuf, 32, "History Level : %d", getPopStarDataMgr().getHistoryScore());
				historyLevelScore->setString(szBuf);
			}
		}
		break;
	}
}
