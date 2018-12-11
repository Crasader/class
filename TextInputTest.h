#ifndef __TEXT_INPUT_TEST_H__
#define __TEXT_INPUT_TEST_H__

#include "cocos2d.h"

class KeyboardNotificationLayer : public cocos2d::IMEDelegate, public cocos2d::Layer
{
public:
    KeyboardNotificationLayer();
    virtual void onClickTrackNode(bool bClicked) = 0;

    virtual void onEnter() override;
    virtual bool init() override;

    virtual void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info)override;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

protected:
    cocos2d::Node*  _trackNode;
    cocos2d::Vec2  _beginPos;
};

class TextFieldTTFDefaultTest : public KeyboardNotificationLayer
{
public:

    CREATE_FUNC(TextFieldTTFDefaultTest);
    virtual void onClickTrackNode(bool bClicked) override;
    virtual void onEnter() override;
    virtual bool init();
    void menuCloseCallback(Ref* pSender);

    void setHolder(const std::string& title);

    std::string getString();
    void setString(const std::string& txt);

    void printText();

    cocos2d::TextFieldTTF* pTextField;
    cocos2d::TextFieldTTF* pPassword;
    std::string title;
};
#endif
