#ifndef LayerPopupChapQuan_h__
#define LayerPopupChapQuan_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "Chess.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class LayerPopupChapQuan : public cocos2d::Layer
{
public:
	virtual bool init() override;
	LayerPopupChapQuan();
	~LayerPopupChapQuan();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerPopupChapQuan);

	typedef std::function<void(LayerPopupChapQuan*, string, string)> popupCb;
	void setCallBackConfirm(const popupCb& callback);
	//update 12/7
	void loadFromConfig(string configStr);
private:
	Layout* pnlMain;
	TableView* tblAttend;
	Sprite *spTable;
	string _strConfig;//chuoi string config chấp quân đóng lên server
	string _strDisplay;//chuỗi string display khi tạo phòng ví dụ: 2 tốt, 1 mã,...
	//chess 
	vector<Chess*> arrChess;

	CheckBox* cbXe1, *cbXe2, *cbPhao1, *cbPhao2, *cbMa1, *cbMa2, *cbTuong1, *cbTuong2, *cbSi1, *cbSi2, *cbTot1, *cbTot2, *cbTot3, *cbTot4, *cbTot5;
	void initTableChess();
	void refreshChess();
	Vec2 getPositionFromIndex(int id);

	void onButtonQuit(Ref* pSender);
	void onButtonSubmit(Ref* pSender);
	void onCheckboxConfig(Ref* pSender, CheckBox::EventType type);
	void handleConfig(bool isHide, int tag);
	void generateConfigString();
	popupCb callback;
	
};

#endif // LayerPopupChapQuan_h__



