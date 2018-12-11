#include "LayerChanToast.h"
#include "mUtils.h"
#include "AllData.h"
#include "../Common.h"
#include "ChanUtils.h"

ChanToast::~ChanToast() {

}

ChanToast* ChanToast::create(string& message) {
    ChanToast* Toast = new ChanToast(message);
    if(Toast && Toast->init()) {
        Toast->autorelease();
        return Toast;
    } else {
        delete Toast;
        Toast = NULL;
        return NULL;
    }
}

ChanToast::ChanToast(string& message) : StringToast(message) {

}

bool ChanToast::init() {
    if(!Layer::init())
        return false;

    float _scaleText = 1;
    Label* label = Label::createWithSystemFont(this->StringToast,"",30);
    label->setColor(Color3B::WHITE);
    label->setScale(_scaleText);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setLocalZOrder(3);

    Size lblSize = label->getContentSize();
    float wLabel = lblSize.width * _scaleText;
    float hLabel = lblSize.height * _scaleText;

    Size size = Size(wLabel + 20, hLabel + 10);

    ui::Scale9Sprite* Blocks = ui::Scale9Sprite::create("s_9sprite_toast.png");
    Blocks ->setContentSize(size);
    Blocks->setAnchorPoint(Vec2(0.5, 0.5));
    Blocks->setOpacity(200);

    this->addChild(Blocks);
    this->addChild(label);

    return true;
}

const string& ChanToast::GetStringToast() const {
    return this->StringToast;
}

bool ChanToast::CompareToast(const string& newStrToast) {
    return (this->StringToast == newStrToast);
}

LayerChanToast::LayerChanToast() {
    this->arrToasts.clear();
    this->StartY = (HEIGHT_DESIGN >> 1) + 100;
    this->Distance = 43;
}

LayerChanToast::~LayerChanToast() {
    this->arrToasts.clear();
}

bool LayerChanToast::init() {
    if(!Layer::init())
        return false;

    this->setAnchorPoint(Vec2::ZERO);

    return true;
}

//void LayerChanToast::ShowToast(string stringToast, const float& duration, const int& type) {
//    if(0 == strcmp(stringToast, "")) return;
//    std::string str = std::string(stringToast);
//    this->ShowToast(str, duration, type);
//}

void LayerChanToast::ShowToast(std::string stringToast, const float& duration, const int& type) {
    boost::algorithm::trim(stringToast);

    if(0 == stringToast.compare("")) return;
    std::vector<ChanToast*>::iterator iter = find_if(this->arrToasts.begin(), this->arrToasts.end(), bind2nd(mem_fun(&ChanToast::CompareToast), stringToast));
    if(iter != this->arrToasts.end())
        return;

    if(this->arrToasts.size() > 4) {
        ChanToast* toast = this->arrToasts.at(0);
        this->arrToasts.erase(this->arrToasts.begin());
        toast->removeFromParentAndCleanup(true);
        toast = NULL;

        for (unsigned int i = 0; i < this->arrToasts.size(); ++i) {
            ChanToast* Toast = this->arrToasts.at(i);
            Toast->setPosition(Vec2(WIDTH_DESIGN >> 1, this->StartY - this->Distance * i));
            Toast->setTag(i);
        }
    }

    ChanToast *Toast = ChanToast::create(stringToast);
    Toast->setPosition(Vec2(WIDTH_DESIGN >> 1, this->StartY - this->Distance * this->arrToasts.size()));
    Toast->setTag(this->arrToasts.size());
    this->addChild(Toast);
    this->arrToasts.push_back(Toast);

    FadeOut *fadeout = FadeOut::create(2);
    DelayTime *delay = DelayTime::create(2);
    Toast->runAction(CCSequence::create(delay, fadeout, CallFunc::create(CC_CALLBACK_0(LayerChanToast::DissmissToast, this, Toast)), nullptr));
}

void LayerChanToast::DissmissToast(Ref* sender) {
    if(this->arrToasts.size() > 0) this->arrToasts.erase(this->arrToasts.begin());
    ChanToast* toast = (ChanToast*)sender;
    toast->removeFromParentAndCleanup(true);

    for (unsigned int i = 0; i < this->arrToasts.size(); ++i) {
        ChanToast* Toast = this->arrToasts.at(i);
        Toast->setPositionY(this->StartY - this->Distance * i);
        Toast->setPosition(Vec2(WIDTH_DESIGN >> 1, this->StartY - this->Distance * i));
        Toast->setTag(i);
    }
}

void LayerChanToast::RemoveAllToast() {
    if(this->arrToasts.size() == 0) return;
    for_each(this->arrToasts.begin(), this->arrToasts.end(), bind2nd(mem_fun(&ChanToast::removeFromParentAndCleanup), true));
    this->arrToasts.clear();
}

ObjToast::ObjToast(std::vector<string> &array)
{
    this->arrayToast = array;
}

ObjToast::~ObjToast()
{
   // log("Object Toast Destructor !");
}

Object *ObjToast::createToast(std::vector<string> &array)
{
    ObjToast* toast = new ObjToast(array);
    if(toast && toast->init ()){
        toast->autorelease ();
        return toast;
    }else{
        delete toast;
        toast = NULL;
        return NULL;
    }
}

bool ObjToast::init()
{
    if(!Layer::init ()){
        return false;
    }

    this->setAnchorPoint (Vec2::ZERO);
    for(int i = 0; i < this->arrayToast.size (); ++i){
        ChanToast* toast = ChanToast::create (this->arrayToast.at (i));
        toast->setPosition (Vec2(0, 43 * i));
        this->addChild (toast);
    }
    return true;
}

int ObjToast::GetSize()
{
    return this->arrayToast.size ();
}
