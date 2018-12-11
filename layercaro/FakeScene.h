
#ifndef FakeScene_h__
#define FakeScene_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GameServer.h"
#include "SingLeton.h"

USING_NS_CC;


class FakeScene : public cocos2d::Layer,
	public SingLeton<FakeScene>,
	public PlayerCallBack
{
public:
	FakeScene();
	~FakeScene();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(FakeScene);
	static FakeScene& getSingleton(void);
	static FakeScene* getSingletonPtr(void);
	CC_SYNTHESIZE(bool, isShow, IsShow);
protected:
	virtual void OnSmartFoxLogin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxLoginError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxConnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	void onButtonClick(Ref* pSender);

	void loginToBotZone();
	void createGuideLayer();
};

#endif // FakeScene_h__


