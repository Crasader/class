#include "PopStarLayer.h"
#include "PopStarDataMgr.h"
#include "GameScene.h"

#include "GameLevel.h"
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#include "PluginAdMob/PluginAdMob.h"
#endif
USING_NS_CC;


// on "init" you need to initialize your instance
bool GameLevel::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float scale = visibleSize.height/1080;
    Sprite* popStarBg = Sprite::create("backgrounds/bg.jpg");
    popStarBg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    popStarBg->setScale(scale);
    this->addChild(popStarBg, 0);
	char szData[32] = {0};
	int level = getPopStarDataMgr().getLevel();
	int score = getPopStarDataMgr().getTargetScoreByLevel(level);
	snprintf(szData, 32, "Level : %d", level);
	labelLevel = Label::createWithSystemFont(szData, "Arial", 44);

	labelLevel->setPosition(Vec2(960, 550));
    labelLevel->setScale(0);
	this->addChild(labelLevel, 1);

	snprintf(szData, 32, "Target Score : %d", score);
	labelTargetScore = Label::createWithSystemFont(szData, "Arial", 44);

	labelTargetScore->setPosition(Vec2(960,500));
    labelTargetScore->setScale(0);
	this->addChild(labelTargetScore, 1);

	this->schedule(schedule_selector(GameLevel::onTimeUp), 4);

	Vec2 pos1 = labelLevel->getPosition();
//    MoveTo* move1 = MoveTo::create( 1, Vec2(pos1.x-250, pos1.y) );
    ScaleTo* sc1 = ScaleTo::create(0.5, 1);
    ScaleTo* sc2 = ScaleTo::create(0.5, 1);
	labelLevel->runAction(sc2);

	Vec2 pos2 = labelTargetScore->getPosition();
//    MoveTo* move2 = MoveTo::create( 2, Vec2(pos2.x-500, pos2.y) );
	labelTargetScore->runAction(sc1);
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    sdkbox::PluginAdMob::cache("top_banner");
#endif
	return true;
}

void GameLevel::onTimeUp(float delta)
{
	changeLayer( this, PopStarLayer::create() );
}
