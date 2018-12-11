#include "CircleAvatar.h"
#include "../SceneManager.h"
#include "mUtils.h"
#include "../cocos/base/CCData.h"
#include "../cotuong/gameUtils.h"
#include "../cotuong/MyAnimation.h"
#include "_Chat_.h"
#define  tag_layer_chat 250
#define tag_emotion 252
CircleAvatar::CircleAvatar() :
_clipping(NULL),
_pTagChild(203),
_iconDefault(NULL),
_iconVip(NULL)
{
	this->_sizeAvatar = Size(123, 123);
	this->isReverse = false;
	this->listener = nullptr;
	this->_callback = nullptr;
}

CircleAvatar::~CircleAvatar()
{
	log("circle avatar destructor");
	SceneManager::getSingleton().getImageDownloadProcess()->stopDownload();
}

bool CircleAvatar::init(){
	if (!Layer::init())
		return false;
	this->setContentSize(SIZE_AVATAR);

	this->_iconDefault = ImageView::create();
	this->_iconDefault->loadTexture(DEFAULT_AVATAR);
	this->_iconDefault->setTouchEnabled(false);
	this->addChild(this->_iconDefault);

	this->_clipping = this->clipplingAvatar();
	this->_clipping->setScale(0.95);

	this->_iconVip = ImageView::create();
	this->_iconVip->setAnchorPoint(Vec2(0.5, 0.5));
	this->_iconVip->loadTexture("input_transparent.png");
	//this->_iconVip->setScale(0.4, 0.4);
	this->_iconVip->setPosition(Vec2(0, 0));

	this->border = ImageView::create("icon_transparent_default.png");
	this->border->setAnchorPoint(Vec2(0.5, 0));
	this->border->setPosition(Vec2(0, -this->_iconDefault->getContentSize().height / 2 - 15));
	this->border->setTouchEnabled(true);
	this->border->addClickEventListener(CC_CALLBACK_1(CircleAvatar::onButtonAvatarClick, this));
	//timer loading
	Sprite* frame_timerLoader = Sprite::create("frame_time.png");
	this->timerFrame = ProgressTimer::create(frame_timerLoader);
	this->timerFrame->setType(ProgressTimer::Type::RADIAL);
	this->timerFrame->setAnchorPoint(Vec2(0.5, 0));
	this->timerFrame->setPosition(Vec2(0, -this->_iconDefault->getContentSize().height / 2 - 20));
	this->timerFrame->setPercentage(100);
	this->timerFrame->setVisible(false);

	this->alertAvatar = Sprite::create("alert_avatar.png");
	this->alertAvatar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->alertAvatar->setPosition(Vec2(0, -5));
	this->alertAvatar->setVisible(false);

	this->spWin = Sprite::create("thang-1.png");
	this->spWin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->spWin->setPosition(Vec2::ZERO);
	this->spWin->setVisible(false);
	this->addChild(spWin, 5);

	this->addChild(this->_clipping);
	this->addChild(this->border, 2);
	this->addChild(this->_iconVip, 4);
	this->addChild(this->timerFrame, 3);
	this->addChild(this->alertAvatar, 1);

	return true;
}

ClippingNode* CircleAvatar::clipplingAvatar() {
	ClippingNode* clipper = ClippingNode::create();
    string path = "avatars/avatar/avatar_1.png";
//    if (SceneManager::getSingleton().getIsFlagStore()){
//        path = "avatars/icon_default.png";
//    }
	Sprite* gameTitle = Sprite::create(path);
	gameTitle->setScale(1);
	clipper->setStencil(gameTitle);
	clipper->setAlphaThreshold(0);
	clipper->setTag(3737);
	clipper->setContentSize(Size(gameTitle->getContentSize().width, gameTitle->getContentSize().height));
	return clipper;
}

//An icon default
void CircleAvatar::hideIconDefault(bool state) {
	this->_iconDefault->setVisible(state);
}

void CircleAvatar::setSizeIcon(const Size& size) {
	this->_sizeAvatar = size;
}

void CircleAvatar::setAvatarImage(const std::string& url, const int& type) {

	this->_clipping->removeChildByTag(this->_pTagChild, true);
	this->_clipping->removeAllChildrenWithCleanup(true);

	if (0 == url.compare("") || 0 == url.compare("null")){
        UserDefault *def = UserDefault::getInstance();
        int avatar = def->getIntegerForKey("avatar");
        string src= StringUtils::format("avatars/avatar/avatar_%d.png",avatar);
		this->avatarFromLocal(src);
		return;
	}


	//log("avatar link is: %s", url.c_str());

	if (type == 1) {
		this->avatarFromLocal(url);
	}
	else if (type == 0) {
		//truyen this->clipping di o day
		string urlCut = mUtils::replaceAll(url, "https", "http");

		//Check filename existed ?
		string fileName = mUtils::getFileName(urlCut);

		std::string writablePath = FileUtils::getInstance()->getWritablePath();

		writablePath.append(fileName);



		bool ret = false;
		cocos2d::Data data = FileUtils::getInstance()->getDataFromFile(writablePath);

		Image* img = new Image();
		//         if (!data.isNull())
		//         {
		//             ret = img->isPng(data.getBytes(), data.getSize()) | img->isJpg(data.getBytes(), data.getSize());
		//         }

		delete img;

		if (!ret){
			try{
				std::remove(writablePath.c_str());
			}
			catch (...){
				log("deo xoa duoc");
			}
		}

//        ImageFromUrl *gImgDownloadProcess = new ImageFromUrl();
//        gImgDownloadProcess->loadImage(this->_clipping,urlCut);
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        SceneManager::getSingleton().getImageDownloadProcess()->downLoadImage(this->_clipping, urlCut, 0, this->_sizeAvatar);
//#endif
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        string src= StringUtils::format("avatars/avatar/%s",mUtils::getFileName(url).c_str());
        this->avatarFromLocal(src);
//#endif
		//this->_iconDefault->setVisible(false);
	}
}

void CircleAvatar::setPlayerRegist() {
	this->_iconDefault->setOpacity(GUESS_OPACITY);
	Node* child = (Node*)this->_clipping->getChildByTag(this->_pTagChild);
	if (NULL != child) {
		child->setOpacity(GUESS_OPACITY);
		this->_iconDefault->setVisible(false);
	}
	this->border->setOpacity(GUESS_OPACITY);
	this->_iconVip->setOpacity(GUESS_OPACITY);
}

void CircleAvatar::resetPlayerRegist() {
	this->_iconDefault->setOpacity(255);
	this->_iconDefault->setVisible(true);
	Node* child = (Node*)this->_clipping->getChildByTag(this->_pTagChild);
	if (NULL != child) {
		child->setOpacity(255);
	}
	this->border->setOpacity(255);
	this->_iconVip->setOpacity(225);
}

const Size &CircleAvatar::GetSizeAvatar() const
{
	return this->_sizeAvatar;
}

void CircleAvatar::avatarFromLocal(const std::string& src) {
    

	Sprite* spark = Sprite::create(src);
	Size size = spark->getContentSize();
	float w = size.width > size.height ? size.height : size.width;
	float sc = this->_sizeAvatar.width / w;
	spark->setTag(this->_pTagChild);
	this->_clipping->addChild(spark, 2);
	// 	if (this->isReverse)
	// 		spark->setFlippedX(true);
}

void CircleAvatar::setTypeVip(const int& type){

	std::string src = mUtils::getSrcVip(type);
	if (type <= 0){
		this->_iconVip->setVisible(false);
	}

	this->_iconVip->loadTexture(src);
	this->_iconVip->setVisible(true);
	//change border vip here
}

void CircleAvatar::displayTypeVip(bool state){
	this->_iconVip->setVisible(state);
}

void CircleAvatar::setClickCallBack(const std::function<void()> &listener)
{
	this->listener = listener;
}

void CircleAvatar::onButtonAvatarClick(Ref* pSender)
{
	if (listener != nullptr)
		this->runAction(CallFunc::create(listener));
	if (this->_callback != nullptr)
		this->runAction(CallFunc::create([=]{_callback(this, this->getName()); }));
}

void CircleAvatar::startTimer(const float& duration_ /*= 15.0*/)
{
	this->stopTimer();
	this->timerFrame->setVisible(true);
	ProgressTo* actionTimer = ProgressTo::create(duration_, 0);
	this->timerFrame->runAction(actionTimer);
}

void CircleAvatar::stopTimer()
{
	this->stopAllActions();
	if (this->timerFrame == NULL) {
		return;
	}

	if (this->timerFrame->getNumberOfRunningActions() > 0) {
		this->timerFrame->stopAllActions();
	}
	this->timerFrame->setPercentage(100);
	this->timerFrame->setVisible(false);
}

void CircleAvatar::reverseAvatar()
{
	this->isReverse = true;
	// 	Sprite* spriteAva = (Sprite*)this->_clipping->getChildByTag(this->_pTagChild);
	// 	if (spriteAva)
	// 	{
	// 		spriteAva->setRotation(180);
	// 	}
}

void CircleAvatar::showAlertAnim(bool isShow)
{
	if (isShow)
	{
		this->alertAvatar->setVisible(true);
		MyAnimation::createActionFadeLoop(this->alertAvatar);
	}
	else{
		this->alertAvatar->setVisible(false);
		this->alertAvatar->stopAllActions();
	}
}

void CircleAvatar::setScaleClipping(float scale)
{
	this->_clipping->setScale(scale);
	this->_iconVip->setPositionY(this->_iconVip->getPositionY()*scale);
}

void CircleAvatar::showAnimationWin(bool isShow)
{
	this->spWin->setVisible(isShow);
	if (isShow)
	{
		RotateTo* rotateLeft = RotateTo::create(0.5, -15);
		RotateTo* rotateRight = RotateTo::create(0.5, 15);
		this->spWin->runAction(RepeatForever::create(Sequence::create(rotateRight, rotateLeft, nullptr)));
	}
	else{
		this->spWin->stopAllActions();
	}
}

void CircleAvatar::showChat(string _msg)
{
	string path = _msg + ".png";
	Vec2 point = Vec2(0, W_AVATAR / 2);
	bool isExist = isImageExist(path);

	if (isExist)
	{
		Sprite* spriteEmo = Sprite::create(path);
		this->removeChildByTag(tag_emotion);
		spriteEmo->setAnchorPoint(Vec2(0.5, 0.5));
		spriteEmo->setPosition(point);
		Spawn *acpawn = Spawn::create(ScaleTo::create(1, 2, 2), JumpBy::create(0.3, Vec2(0, 0), 30, 1), nullptr);
		Sequence* actionse = Sequence::create(Repeat::create(acpawn, 3), RemoveSelf::create(true), nullptr);
		this->addChild(spriteEmo, tag_emotion, tag_emotion);
		spriteEmo->runAction(actionse);
		return;
	}
	else{
		///Change for game p2
		this->removeChildByTag(tag_layer_chat);

		Chat* message = Chat::create(_msg, "khungchat.png");
		message->setAnchorPoint(Vec2(0.5, 0.5));
		message->setPosition(point);
		//message->setTag(tag_layer_chat);
		this->addChild(message, tag_layer_chat, tag_layer_chat);
	}
}

bool CircleAvatar::isImageExist(string url)
{
	int imgNum = 45;
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	{
		imgNum = 20;
	}
	for (int i = 0; i < imgNum; i++)
	{
		string imgPath = StringUtils::format("emo_%d.png", (i + 1));
		if (0 == imgPath.compare(url))
			return true;
	}
	return false;
}

void CircleAvatar::setClickCallBackWithParam(const avatarCallback& callback)
{
	this->_callback = callback;
}











