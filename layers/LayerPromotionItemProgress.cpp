#include "LayerPromotionItemProgress.h"
#include "cocostudio/CocoStudio.h"

#include "Requests/ExtensionRequest.h"

#include "GameServer.h"

USING_NS_CC;


LayerPromotionItemProgress::LayerPromotionItemProgress()
{
}


LayerPromotionItemProgress::~LayerPromotionItemProgress()
{
}

bool LayerPromotionItemProgress::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto rootNode = CSLoader::getInstance()->createNode("LayerItemPopupPromotion.csb");

	//ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());

	lblPromoBody = dynamic_cast<Text*>(rootNode->getChildByName("lblPromoBody"));
	lblProgressInfo = dynamic_cast<Text*>(rootNode->getChildByName("lblProgressInfo"));
	btnClainReward = dynamic_cast<Button*>(rootNode->getChildByName("btnClainReward"));
	btnClainReward->addClickEventListener(CC_CALLBACK_1(LayerPromotionItemProgress::onButtionPress, this));

	return true;
}

void LayerPromotionItemProgress::onButtionPress(Ref* _ref)
{
	auto _btn = dynamic_cast<Button*>(_ref);
	int proID = _btn->getTag();
	//
	boost::shared_ptr<ISFSObject> parameters(new SFSObject());
	parameters->PutInt("kmid", proID);//PROMOTION_ID = "kmid";
	parameters->PutInt("kmgid", getGameID());//PROMOTION_GAME_ID = "kmgid";
	boost::shared_ptr<IRequest> request(new ExtensionRequest("kmcmd", parameters));// PROMOTION_CMD = "kmcmd";
	GameServer::getSingleton().Send(request);
}

void LayerPromotionItemProgress::setDatas(string sBody, string sProgress, int proID, int gameID)
{
//	btnClainReward->setTag(proID);
//	lblProgressInfo->setString(sProgress);
//	lblPromoBody->setString(sBody);
//	setGameID(gameID);
}
