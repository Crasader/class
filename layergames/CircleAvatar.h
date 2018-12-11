#ifndef CIRCLEAVATAR_H
#define CIRCLEAVATAR_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cotuong/CommonChess.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocos2d;

class CircleAvatar : public Layer
{
private:
	std::function<void()> listener;
	void onButtonAvatarClick(Ref* pSender);
	typedef std::function<void(CircleAvatar*, string)> avatarCallback;
	avatarCallback _callback;
public:
	~CircleAvatar();
	CREATE_FUNC(CircleAvatar);

	//An icon default
	void hideIconDefault(bool state);
	void setSizeIcon(const cocos2d::Size& size);
	void setAvatarImage(const std::string& url, const int& type = 0);
	const cocos2d::Size& GetSizeAvatar() const;

	void setPlayerRegist();
	void resetPlayerRegist();

	void setTypeVip(const int& type);
	void displayTypeVip(bool state);

	void setClickCallBack(const std::function<void()> &listener);
	void setClickCallBackWithParam(const avatarCallback& callback);
	void startTimer(const float& duration_ = DEFAULT_TIME_1TURN);
	void stopTimer();
	void avatarFromLocal(const std::string& src);
	void reverseAvatar();
	void showAlertAnim(bool isShow);
	void setScaleClipping(float scale);
	//for bau tom
	void showAnimationWin(bool isShow);
	void showChat(string _msg);
protected:
	bool init();
	CircleAvatar();

	ClippingNode* clipplingAvatar();
private:
	bool isReverse;
	ClippingNode* _clipping;
	ImageView*   _iconDefault;
	ImageView*   _iconVip;
	cocos2d::Size   _sizeAvatar;
	ImageView* border;
	int   _pTagChild;
	ProgressTimer *timerFrame;
	Sprite* alertAvatar;
	Sprite* spWin;

	bool isImageExist(string url);
};

#endif // CIRCLEAVATAR_H
