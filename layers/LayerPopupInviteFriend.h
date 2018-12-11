#ifndef LayerPopupInviteFriend_h__
#define LayerPopupInviteFriend_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../layergames/NativeHelper.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class LayerPopupInviteFriend : public cocos2d::Layer
{
public:
	LayerPopupInviteFriend();
	~LayerPopupInviteFriend();

	virtual bool init();
	void onBtnFaceBook(Ref* pSender);
	void onBtnGMail(Ref* pSender);
    void onBtnSMS(Ref* pSender);
	void onBtnClose(Ref* pSender);
	CREATE_FUNC(LayerPopupInviteFriend);

};
#endif // LayerPopupInviteFriend_h__
