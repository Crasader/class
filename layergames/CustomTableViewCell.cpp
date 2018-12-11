//
//  CustomTableViewCell.cpp
//  AppDemo
//
//  Created by DauA on 5/5/14.
//
//

#include "CustomTableViewCell.h"
USING_NS_CC;

void CustomTableViewCell::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags){
	TableViewCell::draw(renderer, transform, flags);
}

void CustomTableViewCell::setRoomId(const long& idx){
    roomID = idx;
}

const long& CustomTableViewCell::getRoomId() const{
    return roomID;
}

CustomTableViewCell::CustomTableViewCell(Size s) : TableViewCell()
{
	m_currSize= s;
	isSelected = false;
}

void CustomTableViewCell::setSelectedState(bool isSelected)
{
	this->isSelected = isSelected;
	if(isSelected)
	{
		if( this->getChildByTag(tag_NodeColor) != NULL )
			return;

		LayerColor* node = LayerColor::create(Color4B(0, 0, 0, 255));
		node->setTag(tag_NodeColor);
		node->setPosition(Point(0, 0));
		node->setAnchorPoint(Point(0, 0));
		node->setContentSize(m_currSize);
		node->ignoreAnchorPointForPosition(false);
		node->setLocalZOrder(0);
		this->addChild(node);
	}
	else
	{
		if( this->getChildByTag(tag_NodeColor) == NULL )
			return;
		this->removeChildByTag(tag_NodeColor, true);
	}
}

const bool& CustomTableViewCell::getSelectedState() const
{
	return this->isSelected;
}

void CustomTableViewCell::setStringObject(string& val)
{
	this->sValue = val;
}

const string& CustomTableViewCell::getStringObject() const
{
	return this->sValue;
}
