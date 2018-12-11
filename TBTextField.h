//
//  TBTextfield.h
//  TaiXiu
//
//  Created by Tiến Bảo Nguyễn on 10/10/15.
//
//

#ifndef __TaiXiu__TBTextfield__
#define __TaiXiu__TBTextfield__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;


class TBTextField : public Layer, public ui::EditBoxDelegate
{
public:
	CC_SYNTHESIZE_READONLY(bool, isFocus, IsFocus);
	ui::EditBox * editBox;
	TBTextField();
	virtual ~TBTextField();
	static bool allowAutoFocus;

	cocos2d::ui::TextField::ccTextFieldCallback _eventCallback;
	void textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
	// Editbox delegates
public:
	void editBoxEditingDidBegin(ui::EditBox* editBox);
	void editBoxEditingDidEnd(ui::EditBox* editBox);
	void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);
	void editBoxReturn(ui::EditBox* editBox);
public:
	CREATE_FUNC(TBTextField);
	TextField * textField;
	bool onTouchBegan(Touch *touch, Event *unusedEvent);
	void setString(const std::string& text);
	void setText(const std::string& text);
	void convert(TextField * _textField);
	void setFontSize(int size);
	void setPlaceHolder(const std::string& value);
	std::string getPlaceHolder();
	std::string getString();
	void setPlaceHolderColor(const cocos2d::Color3B &color);
	void setPlaceHolderColor(const cocos2d::Color4B &color);
	Color3B getTextColor();
	void setTextColor(const cocos2d::Color4B &textColor);
	void setTextColor(const cocos2d::Color3B &textColor);
	void setFontName(const std::string& name);
	void setColor(const cocos2d::Color3B &textColor);
	void addEventListener(const cocos2d::ui::TextField::ccTextFieldCallback& callback);
	void setFocusEnabled(bool focus);
	void setMaxLength(int maxLength);
	void setInputMode(ui::EditBox::InputMode inpuMode);
	void setInputFlag(ui::EditBox::InputFlag inpuFlag);
	void focus();
	void dismiss();
	static void dismissTf();
	void setContentSizeCustom(Size _size);
};


#endif /* defined(__TaiXiu__TBTextfield__) */
