#include "TextInputTest.h"

USING_NS_CC;

#define FONT_NAME                       "fonts/Thonburi.ttf"
#define FONT_SIZE                       36

static Rect getRect(Node * node)
{
    Rect rc;
    rc.origin = node->getPosition();
    rc.size = node->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;
    return rc;
}

KeyboardNotificationLayer::KeyboardNotificationLayer()
    : _trackNode(0)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(KeyboardNotificationLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(KeyboardNotificationLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void KeyboardNotificationLayer::keyboardWillShow(IMEKeyboardNotificationInfo& info)
{
    if (! _trackNode)return;

    auto rectTracked = getRect(_trackNode);

    if (! rectTracked.intersectsRect(info.end))return;


    float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
    auto& children = getChildren();

    Node * node = 0;
    ssize_t count = children.size();
    Vec2 pos;
    for (int i = 0; i < count; ++i)
    {
        node = children.at(i);
        pos = node->getPosition();
        pos.y += adjustVert;
        node->setPosition(pos);
    }
}

bool KeyboardNotificationLayer::onTouchBegan(Touch  *touch, Event  *event)
{
    _beginPos = touch->getLocation();
    return true;
}

void KeyboardNotificationLayer::onEnter()
{
    Layer::onEnter();
}

bool KeyboardNotificationLayer::init()
{
    if(!Layer::init()) return false;
    return true;
}

void KeyboardNotificationLayer::onTouchEnded(Touch  *touch, Event  *event)
{
    if (! _trackNode)
    {
        return;
    }

    auto endPos = touch->getLocation();

    float delta = 5.0f;
    if (std::abs(endPos.x - _beginPos.x) > delta
            || std::abs(endPos.y - _beginPos.y) > delta)
    {
        _beginPos.x = _beginPos.y = -1;
        return;
    }

    Rect rect;
    auto point = _trackNode->convertTouchToNodeSpace(touch);
    rect.size = _trackNode->getContentSize();
    this->onClickTrackNode(rect.containsPoint(point));
}

void TextFieldTTFDefaultTest::onClickTrackNode(bool bClicked)
{
    auto pTextField = (TextFieldTTF*)_trackNode;
    if (bClicked)
    {
        pTextField->attachWithIME();
    }
    else
    {
        pTextField->detachWithIME();
    }
}

void TextFieldTTFDefaultTest::setHolder(const std::string& title)
{
    this->title = title;
}

std::string TextFieldTTFDefaultTest::getString()
{
    return this->pTextField->getString();
}

void TextFieldTTFDefaultTest::setString(const std::string& txt)
{
    if(this->pTextField != nullptr)
        this->pTextField->setString(txt);
    else
        log("this->pTextField null");
}

bool TextFieldTTFDefaultTest::init(){
    if(!KeyboardNotificationLayer::init()) return false;

auto s = Director::getInstance()->getWinSize();

    this->pTextField = TextFieldTTF::textFieldWithPlaceHolder("<Enter Text>",
                       FONT_NAME,
                       FONT_SIZE);

    addChild(pTextField);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    pTextField->setPosition(Vec2(s.width / 2, s.height/2 + 50));
#else
    pTextField->setPosition(Vec2(s.width / 2, s.height / 2));
#endif
    _trackNode = pTextField;

    return true;
}

void TextFieldTTFDefaultTest::onEnter()
{
    KeyboardNotificationLayer::onEnter();
}

void TextFieldTTFDefaultTest::menuCloseCallback(Ref* pSender)
{
   // log("hreer %s", this->pTextField->getString().c_str());
}

void TextFieldTTFDefaultTest::printText()
{
   // log("hreer %s", this->pTextField->getString().c_str());
}

