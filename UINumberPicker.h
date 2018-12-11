//
//  UINumberPicker.h
//  RoNCppGame
//
//  Created by Howe on 14/11/14.
//
//

#ifndef __RoNCppGame__UINumberPicker__
#define __RoNCppGame__UINumberPicker__

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

enum class PickerDirection
{
    HORIZONTAL = 0,
    VERTICAL = 1
};

class PickerCell : public Node
{
public:
    PickerCell();
    CREATE_FUNC(PickerCell);
    
public:
    void setDirection(PickerDirection dd);
	void setCellValue(string value);
    int getCellValue()
    {
        return currentValue;
    }
	void setCurrentValue(int value);
    
    void moveByDy(float dy);
    void moveByDx(float dx);
private:
    Label *indexLabel;
    int currentValue;
    PickerDirection m_direction;
};

class UINumberPicker : public Layer
{
public:
    UINumberPicker();
    virtual ~UINumberPicker();
    
    CREATE_FUNC(UINumberPicker);
    
    virtual bool init();
    
    /** 设置外观样式 */
    void setSkin(const std::string &bgFile,const std::string &indicatorFile);
	void setButtonIndicator(string button1, string button2);
    /*
     设置方向
     */
    void setDirection(PickerDirection eDirection);
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;

public:
    /*
     设置视图尺寸
     */
    void setViewSize(const Size &size);
    /*
     设置每个item的尺寸
     */
    void setItemSize(float width,float height);
    /*
     设置数据
     */
    void setDataprovider(int visibleItemCount,int minValue,int maxValue,int showValue = -1);
    /*
     设置当前选择的项
     */
    void setSelectedItem(PickerCell *cell);
    
    void setCell(PickerCell *cell);
	CC_SYNTHESIZE(Color3B, _color, ColorSelectedCell);
	CC_SYNTHESIZE(vector<string>, _arr, ArrBet);
	CC_SYNTHESIZE(int, _delta, SpeedScroll);
    /*
      刷新
     */
    void refreshValue(int value);
    /*
     设置当前显示的值
     */
    void setValue(int value);
    /*
     返回当前
     */
    int getValue();
public:
    /*
     值变化的回调函数
     */
    std::function<void(Node*)> valueChangeHandler;
	void setSwallowTouchEvent(bool isSwallow);
private:
    void invalidateDisplayList();
    void updateDisplayList();
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);

	EventListenerTouchOneByOne * touchListener;
    
private:
    PickerCell * itemForPoint(const Point &p);
    
    void relocateContainer(bool value);
    
    void moveAllCell(const Point &p);
    
    void deaccelerateScrolling(float t);
    
    void clickDeaccelerateScrolling(float t);
    
    void _changeSelectValue();
    /**
     * clip this view so that outside of the visible bounds can be hidden.
     */
    void beforeDraw();
    void onBeforeDraw();
    /**
     * retract what's done in beforeDraw so that there's no side effect to
     * other nodes.
     */
    void afterDraw();
    void onAfterDraw();
private:
    PickerCell *_selectedItem;
    
    Node *_itemsLayer;
    
    Sprite * _bgSprite;
	Sprite * _selectionIndicatorSprite;
    
    PickerDirection _pickerDirection;
    
    Rect displayRect;
    
    int m_middleCellIndex;
//    Point middlePoint;
    
    std::vector<PickerCell *> sortChildrenArray;
    //**************//
    bool isInvaildDisplay;
    
    bool m_bIsScrolling;
    
    int m_maxValue;
    int m_minValue;
    
    int show_max;
    int show_min;
    int show_value;
    
    int virtualItemCount;
    
    //澶勭悊婊氬姩鍔ㄧ敾
    bool m_bDragging;
    
    bool m_bTouchMoved;
    /**
     * length between two fingers
     */
    float m_fTouchLength;
    /**
     * UITouch objects to detect multitouch
     */
    //    CCArray* m_pTouches;
    std::vector<Touch*> m_pTouches;
    
    /**
     * scroll speed
     */
    Point m_tScrollDistance;
    /**
     * Touch point
     */
    Point m_tTouchPoint;
    
    
    //    Size m_tViewSize;
    
    std::string moveState;
    
    float item_height;
    float item_width;
    
    float scrollUniformSpeed;
    
    float clickMoveDisx;
    
    float clickMoveDisy;
    
    CustomCommand _beforeDrawCommand;
    CustomCommand _afterDrawCommand;
    
    Rect _parentScissorRect;

    bool _scissorRestored;
public:
    PickerDirection getDirection()
    {
        return _pickerDirection;
    }
    bool isScrolling()
    {
        return m_bIsScrolling;
    }
    PickerCell* getSelectedItem()
    {
        return _selectedItem;
    }
};


#endif /* defined(__RoNCppGame__UINumberPicker__) */