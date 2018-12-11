//
//  UINumberPicker.cpp
//  RoNCppGame
//
//  Created by Howe on 14/11/14.
//
// howe to use?
// UINumberPicker *picker = UINumberPicker::create();
// picker->setItemSize(100, 40);
// picker->setDataprovider(5, 1, 55,1);       // set the data show five items,the minimize value is 1,the max is 55, default show 1;
// picker->setPosition(100, 200);             //set the postions\
// container->addChild(picker);
//

#include "UINumberPicker.h"
#include "AllData.h"
#include "mUtils.h"
#include "layergames/PhomMessDef.h"

#define SCROLL_DEACCEL_RATE  0.95f
#define SCROLL_DEACCEL_DIST  8.0f
#define BOUNCE_DURATION      0.15f
#define INSET_RATIO          0.2f

bool containsTouchObject(const std::vector<Touch*> &m_pTouches, const Touch *touch)
{
	for (size_t i = 0; i < m_pTouches.size(); i++)
	{
		if (m_pTouches[i]->getID() == touch->getID())
		{
			return true;
		}
	}
	return false;
}

UINumberPicker::UINumberPicker()
{
	isInvaildDisplay = false;
	m_bIsScrolling = false;

	m_bDragging = false;

	m_bTouchMoved = false;
	m_fTouchLength = 0;
	moveState = "normal";//normal  up   down;

	scrollUniformSpeed = 1.5;
	//    m_tViewSize.width = 50;

	m_tScrollDistance.set(0, 0);

	clickMoveDisx = 0;
	clickMoveDisy = 0;

	item_height = 22;
	item_width = 22;

	_pickerDirection = PickerDirection::VERTICAL;

	_selectedItem = nullptr;

	_scissorRestored = false;

	valueChangeHandler = nullptr;
	_bgSprite = nullptr;
	_selectionIndicatorSprite = nullptr;
}

UINumberPicker::~UINumberPicker()
{}

void UINumberPicker::invalidateDisplayList()
{
	isInvaildDisplay = true;
}
void UINumberPicker::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
	if (isInvaildDisplay)
	{
		//使用延迟渲染机制，draw之前，更新界面
		updateDisplayList();
		isInvaildDisplay = false;
	}
	Layer::draw(renderer, transform, flags);
}
//bool touchBeginFuc(Touch*touch,Event *evt)
//{
//    
//    return true;
//}
bool UINumberPicker::init()
{
	_itemsLayer = Node::create();

	if (!_bgSprite) {
		_bgSprite = Sprite::create();
		this->addChild(_bgSprite, -1);
	}
	if (!_selectionIndicatorSprite) {
		_selectionIndicatorSprite = Sprite::create();
		this->addChild(_selectionIndicatorSprite, 2);
	}
	this->addChild(_itemsLayer, 1);

	touchListener = EventListenerTouchOneByOne::create();

	//    1. 使用默认的std::bind
	touchListener->onTouchBegan = CC_CALLBACK_2(UINumberPicker::onTouchBegan, this);
	//    // 2. lambda表达式
	//    int aaa = 111;
	//    touchListener->onTouchBegan = [this,aaa](Touch *touch,Event *evt)->bool
	//    {
	//        return true;
	//    };
	//    
	//    //3 lambda + bind
	//    auto touchFuc =[this,aaa](Touch *touch,Event *evt)->bool
	//    {
	//        return true;
	//    };
	//    
	//    auto touchFucBind = std::bind(touchFuc, std::placeholders::_1,std::placeholders::_2);
	//    touchListener->onTouchBegan = touchFucBind;
	//4 .std::fuction
	//    touchListener->onTouchBegan = touchBeginFuc;

	this->setColorSelectedCell(Color3B(255, 227, 160));
	touchListener->onTouchMoved = CC_CALLBACK_2(UINumberPicker::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(UINumberPicker::onTouchEnded, this);
	touchListener->setSwallowTouches(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	return Layer::init();
}
void UINumberPicker::setSkin(const std::string &bgFile, const std::string &indicatorFile)
{
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
	{
		_bgSprite->setAnchorPoint(Vec2(0, 0));
		_bgSprite->setPosition(0, 0);
		_selectionIndicatorSprite->setPosition(getContentSize().width / 2, getContentSize().height / 2);

		_bgSprite->setTexture(bgFile);
		_selectionIndicatorSprite->setTexture(indicatorFile);
// 		Sprite *bg = Sprite::create(bgFile);
// 		bg->setAnchorPoint(Vec2(0, 0));
// 		bg->setPosition(0, 0);
// 		this->addChild(bg);
	}
	else{
		_bgSprite->setPosition(getContentSize().width / 2, getContentSize().height / 2);
		_selectionIndicatorSprite->setPosition(getContentSize().width / 2, getContentSize().height / 2);

		dynamic_cast<Sprite*>(_bgSprite)->setTexture(bgFile);
		dynamic_cast<Sprite*>(_selectionIndicatorSprite)->setTexture(indicatorFile);
		this->setContentSize(_bgSprite->getContentSize());
	}
}
void UINumberPicker::setDirection(PickerDirection eDirection)
{
	if (_pickerDirection == eDirection)
	{
		return;
	}
	_pickerDirection = eDirection;

	//延迟刷新UI
	invalidateDisplayList();
}

void UINumberPicker::setItemSize(float width, float height)
{
	item_height = height;
	item_width = width;
	switch (_pickerDirection)
	{
	case PickerDirection::HORIZONTAL:
	{
										setContentSize(Size(item_width * virtualItemCount, item_height));
										break;
	}
	case PickerDirection::VERTICAL:
	{
									  setContentSize(Size(item_width, item_height * virtualItemCount));
									  break;
	}
	default:
		break;
	}
	invalidateDisplayList();
}

void UINumberPicker::setDataprovider(int visibleItemnum, int minValue, int maxValue, int showValue)
{
	if (visibleItemnum % 2 == 0)
	{
		visibleItemnum -= 1;
		//        MessageBox("", const char *title)
	}
	m_minValue = minValue;
	m_maxValue = maxValue;
	show_value = showValue;

	virtualItemCount = visibleItemnum;
	switch (_pickerDirection)
	{
	case PickerDirection::HORIZONTAL:
	{
										setContentSize(Size(item_width * virtualItemCount, item_height));
										break;
	}
	case PickerDirection::VERTICAL:
	{
									  setContentSize(Size(item_width, item_height * virtualItemCount));
									  break;
	}
	default:
		break;
	}
	invalidateDisplayList();
}

void UINumberPicker::updateDisplayList()
{
	_selectedItem = nullptr;

	_itemsLayer->removeAllChildrenWithCleanup(true);
	sortChildrenArray.clear();

	displayRect.size = getContentSize();
	displayRect.origin = getParent()->convertToWorldSpace(getPosition());

	int numOfCell = virtualItemCount + 2;

	m_middleCellIndex = (int)numOfCell / 2;

	if (show_value < m_minValue || show_value > m_maxValue)
	{
		show_value = m_minValue;
	}
	int upIndex = show_value - 1;
	int downIndex = show_value;

	for (int i = 0; i < numOfCell; i++)
	{
		auto cell = PickerCell::create();
		cell->setContentSize(Size(item_width, item_height));
		cell->setDirection(_pickerDirection);
		switch (_pickerDirection)
		{
		case PickerDirection::VERTICAL:
		{
										  cell->setPositionX(getContentSize().width / 2);
										  if (i <= m_middleCellIndex)
										  {
											  cell->setPositionY((m_middleCellIndex - i) * item_height);
											  if (downIndex >= _arr.size())
												  return;
											  cell->setCellValue(_arr.at(downIndex));
											  cell->setCurrentValue(downIndex);
											  downIndex += 1;
											  if (downIndex > m_maxValue)
											  {
												  downIndex = m_minValue;
											  }
										  }
										  else
										  {
											  cell->setPositionY(i * item_height);

											  if (upIndex < m_minValue)
											  {
												  upIndex = m_maxValue;
											  }
											  if (upIndex >= _arr.size())
												  return;
											  cell->setCurrentValue(upIndex);
											  cell->setCellValue(_arr.at(upIndex));
											  upIndex -= 1;
										  }
										  break;
		}
		case PickerDirection::HORIZONTAL:
		{
											cell->setPositionY(0);
											if (i <= m_middleCellIndex)
											{
												cell->setPositionX((m_middleCellIndex - i) * item_width);

												cell->setCurrentValue(downIndex);
												if (downIndex >= _arr.size())
													return;
												cell->setCellValue(_arr.at(downIndex));
												downIndex += 1;
												if (downIndex > m_maxValue)
												{
													downIndex = m_minValue;
												}
											}
											else
											{
												cell->setPositionX(i * item_width - 5);

												if (upIndex < m_minValue)
												{
													upIndex = m_maxValue;
												}
												if (upIndex >= _arr.size())
													return;
												cell->setCurrentValue(upIndex);
												cell->setCellValue(_arr.at(upIndex));
												upIndex -= 1;
											}
											break;
		}
		default:
			break;
		}

		if (i == 0)
		{
			setCell(cell);
		}
		sortChildrenArray.push_back(cell);
		_itemsLayer->addChild(cell);
	}
}


bool UINumberPicker::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	do
	{
		CC_BREAK_IF(!isVisible());

		CC_BREAK_IF(m_pTouches.size() > 2);

		CC_BREAK_IF(m_bTouchMoved);
		// scrolling，手指移动多少距离，内容区域也移动多少，同步执行 CCScrollView
		//当前坐标转成世界坐标
		const Point &location = touch->getLocationInView();
		const Point &convertedLocation = Director::getInstance()->convertToGL(location);

		CC_BREAK_IF(!displayRect.containsPoint(convertedLocation));

		//unscheduleAllCallbacks();

		if (!containsTouchObject(m_pTouches, touch))
		{
			m_pTouches.push_back(touch);
		}
		if (m_pTouches.size() == 1)
		{
			m_tTouchPoint = convertTouchToNodeSpace(touch);
			m_bTouchMoved = false;

			m_bDragging = true;
			m_tScrollDistance.set(0, 0);
			m_fTouchLength = 0;
		}
		else if (m_pTouches.size() == 2)
		{
			const Point &p1 = convertTouchToNodeSpace(m_pTouches[0]);
			const Point &p2 = convertTouchToNodeSpace(m_pTouches[1]);
			m_tTouchPoint = p1.getMidpoint(p2);
			m_fTouchLength = p1.getDistance(p2);

			m_bDragging = false;
		}
		return true;
	} while (0);

	return false;
}

void UINumberPicker::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	do
	{
		CC_BREAK_IF(!isVisible());

		if (containsTouchObject(m_pTouches, touch))
		{
			if (m_pTouches.size() == 1 && m_bDragging)
			{
				m_bTouchMoved = true;

				const Point &newPoint = convertTouchToNodeSpace(m_pTouches[0]);

				//                Point moveDistance = Point(newPoint.x - m_tTouchPoint.x,
				//                                           newPoint.y - m_tTouchPoint.y);
				m_tScrollDistance.set(newPoint.x - m_tTouchPoint.x, newPoint.y - m_tTouchPoint.y);
				m_tTouchPoint = newPoint;

				if (displayRect.containsPoint(convertToWorldSpace(newPoint)))
				{
					moveAllCell(m_tScrollDistance);
				}
				//                m_tScrollDistance = moveDistance;
			}
		}
	} while (0);
}
void UINumberPicker::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	do
	{
		CC_BREAK_IF(!isVisible());

		if (containsTouchObject(m_pTouches, touch))
		{
			if (m_pTouches.size() == 1 && m_bTouchMoved)
			{
				//手指放开，自由移动
				schedule(schedule_selector(UINumberPicker::deaccelerateScrolling));
			}
			if (m_pTouches.size() == 1 && !m_bTouchMoved)
			{
				//判断当前时一个单击行为，获取当前点击的是哪一个Item；
				PickerCell *cell = itemForPoint(m_tTouchPoint);
				setSelectedItem(cell);
			}
			std::vector<Touch*>::iterator ite = m_pTouches.begin();
			while (ite != m_pTouches.end())
			{
				Touch *ptouch = *ite;
				if (ptouch->getID() == touch->getID())
				{
					m_pTouches.erase(ite);
					break;
				}
				ite++;
			}
		}
		if (m_pTouches.size() == 0)
		{
			m_bDragging = false;
			m_bTouchMoved = false;
		}
		m_bIsScrolling = true;
	} while (0);
}

void UINumberPicker::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	onTouchEnded(touch, unused_event);
}
void UINumberPicker::refreshValue(int value)
{
	if (value > m_maxValue || value < m_minValue)
	{
		value = m_minValue;
	}
	bool hasfind = false;
	for (size_t i = 0; i < sortChildrenArray.size(); i++)
	{
		PickerCell * cell = sortChildrenArray[i];
		if (cell->getCellValue() == value)
		{
			setSelectedItem(cell);
			hasfind = true;
			break;
		}
	}

	if (!hasfind)
	{
		show_value = value;
		invalidateDisplayList();
	}
}


void UINumberPicker::setValue(int value)
{
	if (value > m_maxValue || value < m_minValue)
	{
		value = m_minValue;
	}
	bool hasfind = false;

	for (size_t i = 0; i < sortChildrenArray.size(); i++)
	{
		PickerCell * cell = sortChildrenArray[i];
		if (cell->getCellValue() == value)
		{
			setSelectedItem(cell);
			hasfind = true;
			break;
		}

	}
	if (!hasfind)
	{
		refreshValue(value);
		return;
	}
	if (!hasfind)
	{
		int currentValue = getValue();
		int delta = value - currentValue;
		if (abs(delta) > (int)(m_maxValue - m_minValue) / 2)
		{
			int deltaCurrent = MIN(m_maxValue - currentValue, currentValue - m_minValue);
			int deltaValue = MIN(m_maxValue - value, value - m_minValue);

			delta = -(deltaCurrent + deltaValue);
		}

		switch (_pickerDirection)
		{
		case PickerDirection::VERTICAL:
		{
										  clickMoveDisy = delta * item_height;
										  if (clickMoveDisy > 0)
										  {
											  scrollUniformSpeed = -4;
										  }
										  else
										  {
											  scrollUniformSpeed = 4;
										  }
										  if (fabsf(clickMoveDisy) > SCROLL_DEACCEL_DIST)
										  {
											  //璁＄畻寰楀嚭璇tem璺濈涓績鐨勫瀭鐩村亸绉�
											  Director::getInstance()->getScheduler()->schedule(schedule_selector(UINumberPicker::clickDeaccelerateScrolling),this,1, false);
										  }
										  break;
		}
		case PickerDirection::HORIZONTAL:
		{
											clickMoveDisx = delta * item_width;
											if (clickMoveDisx > 0)
											{
												scrollUniformSpeed = -20;
											}
											else
											{
												scrollUniformSpeed = 20;
											}
											if (fabsf(clickMoveDisx) > SCROLL_DEACCEL_DIST)
											{
												//璁＄畻寰楀嚭璇tem璺濈涓績鐨勫瀭鐩村亸绉�
												Director::getInstance()->getScheduler()->schedule(schedule_selector(UINumberPicker::clickDeaccelerateScrolling),this,1, false);
											}
											break;
		}
		default:
			break;
		}
	}
	m_bIsScrolling = true;
}

void UINumberPicker::setSelectedItem(PickerCell *cell)
{
	if (cell == nullptr)
	{
		return;
	}
	switch (_pickerDirection)
	{
	case PickerDirection::VERTICAL:
	{
									  clickMoveDisy = -cell->getPositionY() + (1 + (int)virtualItemCount / 2) * item_height;
									  if (clickMoveDisy > 0)
									  {
										  scrollUniformSpeed = 2;
									  }
									  else
									  {
										  scrollUniformSpeed = -2;
									  }
									  if (fabsf(clickMoveDisy) > SCROLL_DEACCEL_DIST)
									  {
										  //璁＄畻寰楀嚭璇tem璺濈涓績鐨勫瀭鐩村亸绉�
										  Director::getInstance()->getScheduler()->schedule(schedule_selector(UINumberPicker::clickDeaccelerateScrolling),this,1, false);
									  }
									  else
									  {
										  setCell(cell);
									  }
									  break;
	}
	case PickerDirection::HORIZONTAL:
	{
										clickMoveDisx = -cell->getPositionX() + (1 + (int)virtualItemCount / 2) * item_width;
										if (clickMoveDisx > 0)
										{
											scrollUniformSpeed = 8;
										}
										else
										{
											scrollUniformSpeed = -8;
										}
										if (fabsf(clickMoveDisx) > SCROLL_DEACCEL_DIST)
										{
											//璁＄畻寰楀嚭璇tem璺濈涓績鐨勫瀭鐩村亸绉�
											Director::getInstance()->getScheduler()->schedule(schedule_selector(UINumberPicker::clickDeaccelerateScrolling),this,1, false);
										}
										else
										{
											setCell(cell);
										}
										break;
	}
	default:
		break;
	}

}

void UINumberPicker::setCell(PickerCell *cell)
{
	_selectedItem = cell;
	cell->getChildByTag(246)->setColor(this->getColorSelectedCell());
	cell->getChildByTag(246)->setOpacity(255);
	m_bIsScrolling = false;
	if (valueChangeHandler != nullptr)
	{
		valueChangeHandler(this);
	}
}
void UINumberPicker::clickDeaccelerateScrolling(float t)
{
	switch (_pickerDirection)
	{
	case PickerDirection::VERTICAL:
	{
									  moveAllCell(Point(0, scrollUniformSpeed));

									  float oldV = clickMoveDisy;
									  clickMoveDisy = clickMoveDisy - scrollUniformSpeed;
									  if (oldV * clickMoveDisy <= 0)
									  {
										  unschedule(schedule_selector(UINumberPicker::clickDeaccelerateScrolling));
										  relocateContainer(true);
									  }
									  break;
	}
	case PickerDirection::HORIZONTAL:
	{
										moveAllCell(Point(scrollUniformSpeed, 0));
										float oldV = clickMoveDisx;
										clickMoveDisx = clickMoveDisx - scrollUniformSpeed;
										if (oldV * clickMoveDisx <= 0)
										{
											unschedule(schedule_selector(UINumberPicker::clickDeaccelerateScrolling));
											relocateContainer(true);
										}
										break;
	}
	default:
		break;
	}
}

PickerCell *UINumberPicker::itemForPoint(const Point &p)
{
	for (size_t i = 0; i < sortChildrenArray.size(); i++)
	{
		auto cell = sortChildrenArray[i];

		float dy = cell->getPositionY();
		float dx = cell->getPositionX();

		switch (_pickerDirection)
		{
		case PickerDirection::VERTICAL:
		{
										  if (((dy - item_height) <= p.y) && (dy >= p.y))
										  {
											  return cell;
										  }
										  break;
		}
		case PickerDirection::HORIZONTAL:
		{
											if (((dx - item_width) <= p.x) && (dx >= p.x))
											{
												return cell;
											}
											break;
		}
		default:
			break;
		}
	}
	return nullptr;
}

void UINumberPicker::deaccelerateScrolling(float t)
{
	if (m_bDragging)
	{
		unschedule(schedule_selector(UINumberPicker::deaccelerateScrolling));
		return;
	}
	//乘以一个衰减系数，即执行匀减速运动
	m_tScrollDistance.set(m_tScrollDistance.x * SCROLL_DEACCEL_RATE,
		m_tScrollDistance.y * SCROLL_DEACCEL_RATE);
	moveAllCell(m_tScrollDistance);

	if (m_tScrollDistance.getLength() <= SCROLL_DEACCEL_DIST)
	{
		m_tScrollDistance.set(0, 0);
		unschedule(schedule_selector(UINumberPicker::deaccelerateScrolling));
		relocateContainer(true);
	}
}

void UINumberPicker::relocateContainer(bool value)
{
	//重定位子组件位置坐标
	switch (_pickerDirection)
	{
	case PickerDirection::VERTICAL:
	{
									  PickerCell *cell = itemForPoint(Point(0, getContentSize().height / 2));
									  setSelectedItem(cell);
									  break;
	}
	case PickerDirection::HORIZONTAL:
	{
										PickerCell *cell = itemForPoint(Point(getContentSize().width / 2, 0));
										setSelectedItem(cell);
										break;
	}
	default:
		break;
	}
}

void UINumberPicker::moveAllCell(const Point &deltaDis)
{
	//为提升组件的体验，该方法的执行效率必须尽可能的高效,尽可能减少循环次数
	for (size_t i = 0; i < sortChildrenArray.size(); i++)
	{
		auto cell = sortChildrenArray[i];
		cell->getChildByTag(246)->setColor(Color3B(255, 227, 160));
		cell->getChildByTag(246)->setOpacity(76.5);
		switch (_pickerDirection)
		{
		case PickerDirection::VERTICAL:
		{
										  //                float oldY = cell->getPositionY();
										  cell->moveByDy(deltaDis.y);
										  if (cell->getPositionY() < -item_height)
										  {
											  //                    float dItemY = fabsf( cell->getPositionY() - item_height );//(dy < 0)
											  cell->setPositionY((virtualItemCount + 2) * item_height + cell->getPositionY());
										  }
										  if (cell->getPositionY() > (virtualItemCount + 1) * item_height)
										  {
											  float dItemY = cell->getPositionY() - (virtualItemCount + 1) * item_height;//(dy < 0)
											  cell->setPositionY(-item_height + dItemY);
										  }
										  break;
		}
		case PickerDirection::HORIZONTAL:
		{
											//                float oldX = cell->getPositionX();
											cell->moveByDx(deltaDis.x);
											if (cell->getPositionX() < -item_width)
											{
												cell->setPositionX((virtualItemCount + 2) * item_width + cell->getPositionX());
											}
											if (cell->getPositionX() > (virtualItemCount + 1) * item_width)
											{
												float dItemX = cell->getPositionX() - (virtualItemCount + 1) * item_width;//(dy < 0)
												cell->setPositionX(-item_width + dItemX);
											}
											break;
		}
		default:
			break;
		}
	}

	switch (_pickerDirection)
	{
	case PickerDirection::VERTICAL:
	{
									  //对sortChildrenArray按坐标排序，由上到下
									  std::sort(sortChildrenArray.begin(), sortChildrenArray.end(),
										  [](const Node * a, const Node * b)->bool
									  {
										  if (a->getPositionY() < b->getPositionY())
										  {
											  return false;
										  }
										  return true;
									  }
									  );
									  break;
	}
	case PickerDirection::HORIZONTAL:
	{
										//对sortChildrenArray按坐标排序，由左到右
										std::sort(sortChildrenArray.begin(), sortChildrenArray.end(),
											[](const Node *a, const Node *b)->bool
										{
											if (a->getPositionX() > b->getPositionX())
											{
												return false;
											}
											return true;
										});
										break;
	}
	default:
		break;
	}
	//给cell赋值
	//ps: 位于middleCellIndex索引处的cell不会重新赋值
	for (int i = 1; i <= m_middleCellIndex; i++)
	{
		if (m_middleCellIndex + i - 1 > sortChildrenArray.size())
			return;
		if (sortChildrenArray[m_middleCellIndex + i - 1]->getCellValue() == m_maxValue)
		{
			sortChildrenArray[m_middleCellIndex + i]->setCurrentValue(m_minValue);
			sortChildrenArray[m_middleCellIndex + i]->setCellValue(_arr.at(m_minValue));
		}
		else
		{
			sortChildrenArray[m_middleCellIndex + i]->setCurrentValue(sortChildrenArray[m_middleCellIndex + i - 1]->getCellValue() + 1);
			sortChildrenArray[m_middleCellIndex + i]->setCellValue(_arr.at(sortChildrenArray[m_middleCellIndex + i - 1]->getCellValue() + 1));
		}
		if (sortChildrenArray[m_middleCellIndex - i + 1]->getCellValue() == m_minValue)
		{
			sortChildrenArray[m_middleCellIndex - i]->setCurrentValue(m_maxValue);
			sortChildrenArray[m_middleCellIndex - i]->setCellValue(_arr.at(m_maxValue));
		}
		else
		{
			sortChildrenArray[m_middleCellIndex - i]->setCurrentValue(sortChildrenArray[m_middleCellIndex - i + 1]->getCellValue() - 1);
			sortChildrenArray[m_middleCellIndex - i]->setCellValue(_arr.at(sortChildrenArray[m_middleCellIndex - i + 1]->getCellValue() - 1));
		}
	}
}
//界面裁剪这段直接copy scrollview代码
void UINumberPicker::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
	// quick return if not visible
	if (!isVisible() || !isVisitableByVisitingCamera())
	{
		return;
	}

	uint32_t flags = processParentFlags(parentTransform, parentFlags);

	// IMPORTANT:
	// To ease the migration to v3.0, we still support the Mat4 stack,
	// but it is deprecated and your code should not rely on it
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);

	this->beforeDraw();
	bool visibleByCamera = isVisitableByVisitingCamera();

	if (!_children.empty())
	{
		int i = 0;

		// draw children zOrder < 0
		for (; i < _children.size(); i++)
		{
			auto child = _children.at(i);
			if (child->getLocalZOrder() < 0)
			{
				child->visit(renderer, _modelViewTransform, flags);
			}
			else
			{
				break;
			}
		}

		// this draw
		if (visibleByCamera)
			this->draw(renderer, _modelViewTransform, flags);

		// draw children zOrder >= 0
		for (; i < _children.size(); i++)
		{
			auto child = _children.at(i);
			child->visit(renderer, _modelViewTransform, flags);
		}
	}
	else if (visibleByCamera)
	{
		this->draw(renderer, _modelViewTransform, flags);
	}

	this->afterDraw();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
void UINumberPicker::beforeDraw()
{
	_beforeDrawCommand.init(_globalZOrder);
	_beforeDrawCommand.func = CC_CALLBACK_0(UINumberPicker::onBeforeDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_beforeDrawCommand);
}

/**
 * clip this view so that outside of the visible bounds can be hidden.
 */
void UINumberPicker::onBeforeDraw()
{
	_scissorRestored = false;
	const Rect &frame = displayRect;
	auto glview = Director::getInstance()->getOpenGLView();

	if (glview->isScissorEnabled())
	{
		_scissorRestored = true;
		_parentScissorRect = glview->getScissorRect();
		//set the intersection of _parentScissorRect and frame as the new scissor rect
		if (frame.intersectsRect(_parentScissorRect))
		{
			float x = MAX(frame.origin.x, _parentScissorRect.origin.x);
			float y = MAX(frame.origin.y, _parentScissorRect.origin.y);
			float xx = MIN(frame.origin.x + frame.size.width, _parentScissorRect.origin.x + _parentScissorRect.size.width);
			float yy = MIN(frame.origin.y + frame.size.height, _parentScissorRect.origin.y + _parentScissorRect.size.height);
			glview->setScissorInPoints(x, y, xx - x, yy - y);
		}
	}
	else
	{
		glEnable(GL_SCISSOR_TEST);
		glview->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
	}
}

void UINumberPicker::afterDraw()
{
	_afterDrawCommand.init(_globalZOrder);
	_afterDrawCommand.func = CC_CALLBACK_0(UINumberPicker::onAfterDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_afterDrawCommand);
}

/**
 * retract what's done in beforeDraw so that there's no side effect to
 * other nodes.
 */
void UINumberPicker::onAfterDraw()
{
	if (_scissorRestored)
	{
		//restore the parent's scissor rect
		auto glview = Director::getInstance()->getOpenGLView();

		glview->setScissorInPoints(_parentScissorRect.origin.x, _parentScissorRect.origin.y,
			_parentScissorRect.size.width, _parentScissorRect.size.height);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
	}
}

int UINumberPicker::getValue()
{
	if (_selectedItem != nullptr)
	{
		return _selectedItem->getCellValue();
	}
	return m_minValue;
}

void UINumberPicker::setButtonIndicator(string button1, string button2)
{

}

void UINumberPicker::setSwallowTouchEvent(bool isSwallow)
{
	touchListener->setSwallowTouches(isSwallow);
}

PickerCell::PickerCell()
{
	indexLabel = NULL;
	currentValue = -1;
	m_direction = PickerDirection::VERTICAL;
}
void PickerCell::moveByDy(float dy)
{
	this->setPositionY(getPositionY() + dy);
}
void PickerCell::moveByDx(float dx)
{
	this->setPositionX(getPositionX() + dx);
}
void PickerCell::setDirection(PickerDirection dd)
{
	m_direction = dd;
}
void PickerCell::setCellValue(string value)
{
	string value_cell = value;
	if (0 != value.compare("All"))
	{
		double value_double = atol(value.c_str());
		value_cell = formatMoneyPicker(value_double);
	}
	if (indexLabel == NULL)
	{
		indexLabel = Label::createWithTTF(value_cell, "__Roboto-Bold.ttf", 45);
		indexLabel->setOpacity(76.5);
		indexLabel->setColor(Color3B(255, 227, 160));
		indexLabel->setTag(246);
		this->addChild(indexLabel);
		switch (m_direction)
		{
		case PickerDirection::VERTICAL:
		{
										  //                indexLabel->setPositionX(getContentSize().width/2);
										  indexLabel->setPositionY(-getContentSize().height / 2);
										  break;
		}
		case PickerDirection::HORIZONTAL:
		{
											indexLabel->setPositionX(-getContentSize().width / 2);
											indexLabel->setPositionY(getContentSize().height / 2 + 10);
											break;
		}
		default:
			break;
		}
	}
	else
	{
		indexLabel->setString(value_cell);
	}
}

void PickerCell::setCurrentValue(int value)
{
	this->setTag(value);
	currentValue = value;
}









