#ifndef MyAnimation_h__
#define MyAnimation_h__

#include "cocos2d.h"

class MyAnimation{
public:
	static cocos2d::Vec2 getScaleSmoothly(float scale)
	{
		auto vizibleSize = Director::getInstance()->getOpenGLView()->getFrameSize();
		// 		float x0 = designResolutionSize.width / designResolutionSize.height;
		// 		float x1 = vizibleSize.width / vizibleSize.height;
		// 		if (x1 > x0)
		// 		{
		// 			float scaleX = x0 / x1;
		// 			return Vec2(scaleX*scale, scale);
		// 		}
		// 		else
		// 		{
		// 			float scaleY = x1 / x0;
		// 			return Vec2(scale, scale*scaleY);
		// 		}
		return Vec2(scale, scale);
	}

	static void createActionShowPopup(Node * node, float delta)
	{
		node->setCascadeOpacityEnabled(true);
		auto scale1 = MyAnimation::getScaleSmoothly(0.5);
		node->setScaleX(scale1.x);
		node->setScaleY(scale1.y);
		node->setOpacity(0);
		node->setVisible(true);
		auto scale2 = getScaleSmoothly(1.2f);
		auto scale3 = getScaleSmoothly(1.0f);
		auto se = Sequence::createWithTwoActions(
			ScaleTo::create(delta*0.6f, scale2.x, scale2.y),
			ScaleTo::create(delta*0.4f, scale3.x, scale3.y)
			);
		auto action = Spawn::createWithTwoActions(se, FadeIn::create(delta));
		node->runAction(action);
	}

	static void createActionHiddenPopup(Node * node, float delta)
	{
		node->stopAllActions();
		node->setCascadeOpacityEnabled(true);
		auto action = Spawn::create(ScaleTo::create(delta, 0.5f), FadeOut::create(delta), nullptr);
		node->runAction(Sequence::create(
			action,
			Hide::create(),
			nullptr));
	}

	static void createActionRemovePopup(Node * node, float delta)
	{
		node->stopAllActions();
		node->setCascadeOpacityEnabled(true);
		auto action = Spawn::create(ScaleTo::create(delta, 0.5f), FadeOut::create(delta), nullptr);
		node->runAction(Sequence::create(
			action,
			Hide::create(),
			RemoveSelf::create(true),
			nullptr));
	}

	static void createActionFadeLoop(Node* node)
	{
		node->stopAllActions();
		auto ActionFade = RepeatForever::create(Sequence::create(FadeTo::create(0.7, 50), FadeTo::create(0.7, 254), nullptr));
		node->runAction(ActionFade);
	}
	static void createActionTintLoop(Node* node, Color3B _color)
	{
		auto _tint = TintTo::create(0.7, _color);
		auto _revers = TintTo::create(0.7, Color3B::WHITE);
		auto actionTint = RepeatForever::create(Sequence::create(_tint, _revers, nullptr));
		node->runAction(actionTint);
	}

	static void createActionFadeIn(Node* node)
	{
		node->stopAllActions();
		auto ActionFade = Sequence::create(FadeIn::create(0.2), Show::create(), nullptr);
		node->runAction(ActionFade);
	}

	static void createActionFadeOut(Node* node)
	{
		node->stopAllActions();
		auto ActionFade = Sequence::create(FadeOut::create(0.2), Hide::create(), nullptr);
		node->runAction(ActionFade);
	}
};

#endif // MyAnimation_h__
