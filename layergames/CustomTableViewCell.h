//
//  CustomTableViewCell.h
//  AppDemo
//
//  Created by DauA on 5/5/14.
//
//

#ifndef __AppDemo__CustomTableViewCell__
#define __AppDemo__CustomTableViewCell__

#include "cocos-ext.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace std;

class CustomTableViewCell : public cocos2d::extension::TableViewCell{
private:
    long roomID;
	string sValue;

	enum
	{
		tag_NodeColor = 11212
	};
	Size m_currSize;
	bool isSelected;

public:
	CustomTableViewCell(Size s);
	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    void setRoomId(const long& idx);
    const long& getRoomId() const;

	void setStringObject(string& val);
	const string& getStringObject() const;

	void setSelectedState(bool isSelected);
	const bool& getSelectedState() const;
};

#endif /* defined(__AppDemo__CustomTableViewCell__) */
