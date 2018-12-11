//
//  TBTextfield.cpp
//  TaiXiu
//
//  Created by Tiến Bảo Nguyễn on 10/10/15.
//
//

#include "TBTextField.h"
#include "SceneManager.h"
TBTextField::TBTextField()
{
	isFocus = false;
}

TBTextField::~TBTextField()
{
	_eventCallback(NULL, cocos2d::ui::TextField::EventType::DETACH_WITH_IME);
	_eventCallback(NULL, cocos2d::ui::TextField::EventType::INSERT_TEXT);
	_eventCallback(NULL, cocos2d::ui::TextField::EventType::DELETE_BACKWARD);
	_eventCallback(NULL, cocos2d::ui::TextField::EventType::ATTACH_WITH_IME);
	editBox->removeFromParentAndCleanup(true);
}

bool TBTextField::allowAutoFocus = true;

void TBTextField::dismissTf()
{
	//log("[TBTextField] request dismiss tf");

		auto glView = Director::getInstance()->getOpenGLView();
		if (glView)
		{
			glView->setIMEKeyboardState(false);
		}
		allowAutoFocus = false;
		//log("[TBTextField] done dismiss tf");
   
}
void TBTextField::convert(TextField * _textField)
{
    textField = _textField;
    if  (textField == nullptr) return;
	textField->setVisible(false);
	ignoreAnchorPointForPosition(false);
    setContentSize(Size(textField->getContentSize().width, textField->getContentSize().height));
    setAnchorPoint(textField->getAnchorPoint());
    setScale(textField->getScale());
	//set TAG FOR LAYER TBTEXTFIELD
	//log("TAG TEXTFIELD = %d", textField->getTag());
    textField->getParent()->addChild(this, - textField->getLocalZOrder());
    setPosition(textField->getPosition());
    
    ////log("size=(%d, %d)", (int)textField->getContentSize().width, (int)textField->getContentSize().height);
    editBox = ui::EditBox::create(Size(textField->getContentSize().width,
		textField->getContentSize().height) ,ui::Scale9Sprite::create("input_transparent.png"));
    //editBox->retain();
    editBox->ignoreAnchorPointForPosition(false);
    editBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    
    editBox->setFontName(textField->getFontName().c_str());
	editBox->setFontSize(textField->getFontSize());
   // editBox->setFontColor(textField->getTextColor());
    editBox->setText(textField->getString().c_str());
    
    editBox->setPlaceholderFontName(textField->getFontName().c_str());
	editBox->setPlaceholderFontSize(textField->getFontSize()-5);
    editBox->setPlaceholderFontColor(textField->getPlaceHolderColor());
    editBox->setPlaceHolder(textField->getPlaceHolder().c_str());
    //editBox->
    
    editBox->setPosition(Vec2(0, getContentSize().height/2));
    if (textField->getMaxLength() > 0)
        editBox->setMaxLength(textField->getMaxLength());
    editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	editBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
    //anhnt change color
    if (SceneManager::getSingleton().isNagaWin){
        editBox->setFontColor(cocos2d::Color3B::BLACK);
    }else{
        editBox->setFontColor(cocos2d::Color3B::WHITE);
    }
    if (textField->isPasswordEnabled())
		editBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	editBox->setReturnType(ui::EditBox::KeyboardReturnType::GO);
	if (textField->getTag() == -1)
		textField->setTag(123456);
	editBox->setTag(textField->getTag());
	this->setTag(textField->getTag()*1000);
    addChild(editBox, 10);
    editBox->setDelegate(this);
	addEventListener(CC_CALLBACK_2(TBTextField::textFieldEvent, this));
    textField->setVisible(false);
    textField->setString("");
    textField->setPlaceHolder("");
    textField->setEnabled(false);
}

void TBTextField::addEventListener(const cocos2d::ui::TextField::ccTextFieldCallback& callback)
{
    _eventCallback = callback;
}

bool TBTextField::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    return false;
}

void TBTextField::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    log("begin editting");

    if (_eventCallback) {
        Sequence * se = Sequence::create(DelayTime::create(0.2f), CallFuncN::create([&](Ref* obj) {
            _eventCallback(this, cocos2d::ui::TextField::EventType::ATTACH_WITH_IME);
        }), nullptr);
        runAction(se);
		isFocus = true;
    }
}
void TBTextField::editBoxEditingDidEnd(ui::EditBox* editBox) {
    log("end editting");
    if (_eventCallback) {
        _eventCallback(this, cocos2d::ui::TextField::EventType::DETACH_WITH_IME);
		isFocus = false;
    }
}
void TBTextField::editBoxTextChanged(ui::EditBox* editBox, const std::string& text) {
    if (_eventCallback) {
        _eventCallback(this, cocos2d::ui::TextField::EventType::INSERT_TEXT);
    }
}
void TBTextField::editBoxReturn(ui::EditBox* editBox)
{
    log("return editting");

    if (_eventCallback) {
        _eventCallback(this, cocos2d::ui::TextField::EventType::DELETE_BACKWARD);
    }
}

void TBTextField::setText(const std::string& text)
{
    setString(text);
}
void TBTextField::setString(const std::string& text)
{
    editBox->setText(text.c_str());
}

void TBTextField::setFontSize(int size)
{
    editBox->setFontSize(size);
}

void TBTextField::setPlaceHolder(const std::string& value)
{
    editBox->setPlaceHolder(value.c_str());
}

std::string TBTextField::getPlaceHolder()
{
    return editBox->getPlaceHolder();
}

void TBTextField::setPlaceHolderColor(const cocos2d::Color3B &color)
{
    editBox->setPlaceholderFontColor(color);
}

void TBTextField::setPlaceHolderColor(const cocos2d::Color4B &color)
{
    editBox->setPlaceholderFontColor(color);
}

Color3B TBTextField::getTextColor()
{
    return editBox->getColor();
}

void TBTextField::setTextColor(const cocos2d::Color3B &textColor)
{
    editBox->setFontColor(textColor);
}

void TBTextField::setTextColor(const cocos2d::Color4B &textColor)
{
    editBox->setFontColor(textColor);
}

void TBTextField::setFontName(const std::string& name)
{
    editBox->setFontName(name.c_str());
}
void TBTextField::setColor(const cocos2d::Color3B &textColor)
{
    //anhnt change color textfield
    editBox->setFontColor(textColor);
}

void TBTextField::setFocusEnabled(bool focus)
{
//    editBox->setFocusEnabled(false);
}
void TBTextField::setMaxLength(int maxLength)
{
    editBox->setMaxLength(maxLength);
}
std::string TBTextField::getString()
{
    return editBox->getText();
}

void TBTextField::setInputMode(ui::EditBox::InputMode inpuMode)
{
    editBox->setInputMode(inpuMode);
}

void TBTextField::setInputFlag(ui::EditBox::InputFlag inpuFlag)
{
    editBox->setInputFlag(inpuFlag);
}

void TBTextField::focus()
{
	if (allowAutoFocus)
	{
		editBox->touchDownAction(editBox, cocos2d::ui::Widget::TouchEventType::ENDED);
	}
}

void TBTextField::dismiss()
{
	//log("TBTextField::dismiss()");
}

void TBTextField::textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type)
{
//	switch (type)
//	{
//	case TextField::EventType::ATTACH_WITH_IME:
//
//	case TextField::EventType::DETACH_WITH_IME:
//
//	case TextField::EventType::INSERT_TEXT:
//
//	case TextField::EventType::DELETE_BACKWARD:
//
//		textField->setString(editBox->getText());
//	}
}

void TBTextField::setContentSizeCustom(Size _size)
{
	editBox->setContentSize(_size);
}
