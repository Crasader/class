/****************************************************************************
Copyright (c) 2013 Artavazd Barseghyan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCActivityIndicator.h"

bool CCActivityIndicator::init()
{
// 	SpriteFrameCache * spriteFramecache = SpriteFrameCache::getInstance();
// 
// 	spriteFramecache->addSpriteFramesWithFile("loadingindicator/ccactivityindicator.plist");
// 	SpriteBatchNode::initWithFile("loadingindicator/ccactivityindicator.png", 1);
// 
// 	Size winSize = Director::getInstance()->getWinSize();
// 	indicator = Sprite::createWithSpriteFrameName("ccactivityindicator_1.gif");
// 	indicator->setPosition(Point(winSize.width/2,winSize.height/2));
// 	addChild(indicator);
// 
 	animating = false;
 	hidesWhenStopped = true;
// 
// 	//load all sprite frames into array
// 	for (int i=1; i<=kActivityIndicatorFramesCount; i++) {
// 		SpriteFrame * frame = spriteFramecache->getSpriteFrameByName(StringUtils::format("ccactivityindicator_%d.gif",i));
// 		spriteFrames.pushBack(frame);
// 	}

	return true;
}

CCActivityIndicator::~CCActivityIndicator()
{
	spriteFrames.clear();
}

void CCActivityIndicator::updateVisibility()
{
	if (hidesWhenStopped && !isAnimating()) {
		indicator->setVisible(false);
	}
}

void CCActivityIndicator::setParent(Node*p)
{
	this->setParent(p);

	if (p!=NULL) {
		updateVisibility();
	}
}

void CCActivityIndicator::setPosition(const cocos2d::Point& pos)
{
	indicator->setPosition(pos);
}

void CCActivityIndicator::startAnimating()
{
	if (animating) return;
	animating = true;

	indicator->setVisible(true);
	Animation * anim = Animation::createWithSpriteFrames(spriteFrames, kActivityIndicatorDelayBetweenFrames);
	Animate * action = Animate::create(anim);
	indicator->runAction(RepeatForever::create(action));
}

void CCActivityIndicator::stopAnimating()
{
	if (!animating) return;
	animating = false;

	indicator->stopAllActions();
	updateVisibility();
}
