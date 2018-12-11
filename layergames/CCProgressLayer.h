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

#ifndef CCProgressLayer_h
#define CCProgressLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../Common.h"
#define kProgressLayerBackgroundShowOpacity 175

using namespace cocos2d;
using namespace std;

class CCActivityIndicator;

class CCProgressLayer : public cocos2d::LayerColor {
public:
	CREATE_FUNC(CCProgressLayer);
	CCProgressLayer();
	~CCProgressLayer();

	virtual	bool init();
	void showLoading(const float& timeOut = 10);
    void hideLoading(float dt);
    void timShowLoading(float dt);

	virtual void onEnter();
	virtual void onExit();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onLoadingCallBack(const std::function<void()> &listener);
	void setStatusString(string sStatus);
	void setCallBack(Ref* target, SEL_CallFunc listener);
private:
	Sprite* spriteLoading;
	std::function<void()> listener;
	float timeOut;
	EventListenerTouchOneByOne *touchListener;
	Label* lblStatus;
	SEL_CallFunc m_callbackTimerListener;
	Ref *m_callbackTimer;
    int currTimeOut = 2;
    int timeLoading = 0;
    bool shouldHide = false;

};


#endif
