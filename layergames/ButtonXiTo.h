#ifndef BUTTONXITO_H
#define BUTTONXITO_H

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::ui;

class ButtonXiTo : public Button
{
private:
    bool allow;
    std::string name;
    Label* money;

public:
    ButtonXiTo(const std::string& name);
    ~ButtonXiTo();

    bool init();
    static ButtonXiTo* create(const std::string& name);

    bool& isAllow();
    void setAllow(bool allow);

    void resetState();

    void createSuggest();
    void setMoney(const std::string& money, bool b = false);
};

#endif // BUTTONXITO_H
