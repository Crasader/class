//
//  LayerPlayerInfo.cpp
//  demo
//
//  Created by AlexDao on 5/24/14.
//
//

#include "LayerPlayerInfo.h"
#include "Requests/ExtensionRequest.h"
#include "mUtils.h"
#include "AllData.h"
#include "SceneManager.h"

using namespace cocos2d;

LayerPlayerInfo::LayerPlayerInfo():
	lblAMF(NULL),
	lblName(NULL),
	lblSex(NULL),
	btnInviteFriend(NULL),
	btnUnFriend(NULL),
	btnKick(NULL),
	nodeAvatar(NULL)
{
	GameServer::getSingleton().addListeners(this);
}

LayerPlayerInfo::~LayerPlayerInfo()
{
	CC_SAFE_RELEASE(lblAMF);
	CC_SAFE_RELEASE(lblName);
	CC_SAFE_RELEASE(lblSex);
	CC_SAFE_RELEASE(btnInviteFriend);
	CC_SAFE_RELEASE(btnUnFriend);
	CC_SAFE_RELEASE(btnKick);
	CC_SAFE_RELEASE(nodeAvatar);
	//this->unregisterScriptTouchHandler();
	GameServer::getSingleton().removeListeners(this);
    SceneManager::getSingleton().getImageDownloadProcess()->stopDownload();
	log("Destructor LayerPlayerInfo");
}

// CCBSelectorResolver interface
SEL_MenuHandler LayerPlayerInfo::onResolveCCBCCMenuItemSelector(Ref *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnClose", LayerPlayerInfo::onButtonClose);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnInviteFriend", LayerPlayerInfo::onButtonInviteFriend);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnUnFriend", LayerPlayerInfo::onButtonUnFriend);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnKick", LayerPlayerInfo::onButtonKick);
    return NULL;
}

void LayerPlayerInfo::onButtonInviteFriend(Ref* pSender)
{
	log("onButtonInviteFriend");
	boost::shared_ptr<ISFSObject> params (new SFSObject());
	params->PutUtfString("aI", mFriendID.c_str());
	boost::shared_ptr<IRequest> request (new ExtensionRequest("raaf", params));
	GameServer::getSingleton().Send(request);
	//
	this->removeFromParentAndCleanup(true);
}

void LayerPlayerInfo::onButtonUnFriend(Ref* pSender)
{
	log("onButtonUnFriend");
	boost::shared_ptr<ISFSObject> params (new SFSObject());
	params->PutUtfString("aI", mFriendID.c_str());
	boost::shared_ptr<IRequest> request (new ExtensionRequest("rarf", params));
	GameServer::getSingleton().Send(request);
	//
	this->removeFromParentAndCleanup(true);
}

void LayerPlayerInfo::onButtonKick(Ref* pSender)
{
	log("onButtonKick");
	//Get uid for kick
	boost::shared_ptr<User> user = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(mFriendID);
	if (user == NULL || GameServer::getSingleton().getSmartFox()->LastJoinedRoom()==NULL)
		return;
	int roomID = atoi ( GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->c_str() );
	//
	boost::shared_ptr<ISFSObject> params (new SFSObject());
	if( roomID== atoi(SERVICE_TLMB.c_str()) || roomID== atoi(SERVICE_TLMN.c_str()) || roomID== atoi(SERVICE_XI_TO.c_str()) ){
		params->PutInt( "id", user->Id() );
		params->PutInt( "uid", user->Id() );
	}else{
		params->PutUtfString("uid", mFriendID.c_str());
		params->PutUtfString("id", mFriendID.c_str());
	}

	//kckrq: Kick player for TLMB, TLMN, XITO
	boost::shared_ptr<IRequest> request (new ExtensionRequest("kckrq", params, GameServer::getSingleton().getSmartFox()->LastJoinedRoom()));
	GameServer::getSingleton().Send(request);
	//
	this->removeFromParentAndCleanup(true);
}

void LayerPlayerInfo::onButtonClose(Ref* pSender)
{
   // log("onButtonClose");
	this->removeFromParentAndCleanup(true);
}

// CCBMemberVariableAssigner interface
bool LayerPlayerInfo::onAssignCCBMemberVariable(Ref *pTarget, const char *pMemberVariableName, cocos2d::Node *pNode)
{
	log("Imhere onAssignCCBMemberVariable: %s", pMemberVariableName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lblAMF", Label*, lblAMF);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lblName", Label*, lblName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lblSex", Label*, lblSex);
    
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnInviteFriend", MenuItem*, btnInviteFriend);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnUnFriend", MenuItem*, btnUnFriend);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnKick", MenuItem*, btnKick);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nodeAvatar", Node*, nodeAvatar);
    return true;
}

void LayerPlayerInfo::onNodeLoaded( Node * pNode,  NodeLoader * pNodeLoader)
{
}

void LayerPlayerInfo::reloadAllDatas()
{
	//
	boost::shared_ptr<ISFSObject> params (new SFSObject());
	params->PutUtfString("aI", mFriendID.c_str());
	boost::shared_ptr<IRequest> request (new ExtensionRequest("rgif", params));
	GameServer::getSingleton().Send(request);
}

void LayerPlayerInfo::OnExtensionResponse( unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent )
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if(strcmp("rgif", cmd->c_str())==0){ ////Ti.App.GETACCINFOR_CMD:
		//setName: moc3-change aN=>adn
		string _strName = *param->GetUtfString("adn");
		if (_strName.compare("") == 0)
			_strName = *param->GetUtfString("aI");
		lblName->setString(_strName);
		lblSex->setString( param->GetInt("aS")==0?"Giới tính: Nam":"Giới tính: Nữ" );
		lblAMF->setString( mUtils::convertMoneyEx(*param->GetLong("amf")).c_str()  );
        //
        if(strcmp(GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str(),param->GetUtfString("aI")->c_str())==0)
        {
            btnInviteFriend->setVisible(false);
            btnUnFriend->setVisible(false);
            return;
        }
      
		//Check is friend
		btnUnFriend->setVisible(GameServer::getSingleton().getSmartFox()->BuddyManager()->ContainsBuddy(mFriendID));
		btnInviteFriend->setVisible(!GameServer::getSingleton().getSmartFox()->BuddyManager()->ContainsBuddy(mFriendID));
      
	}
}

void LayerPlayerInfo::setFriendId( string friendID )
{ 
	mFriendID = friendID;
}

// void LayerPlayerInfo::registerWithTouchDispatcher( void )
// {
// 	Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
// }

bool LayerPlayerInfo::onTouchBegan( cocos2d::Touch *pTouch, cocos2d::Event *pEvent )
{
	return true;
}

void LayerPlayerInfo::onEnter()
{
	Layer::onEnter();
	this->runAction(mUtils::getActionOpenPopup());
	if(isPlaying)
		btnKick->setVisible(false);
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPlayerInfo::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerPlayerInfo::setAvatarImage(Sprite* avatar)
{
	if(avatar == NULL)
		return;
	return;
	nodeAvatar->addChild(avatar);
}

void LayerPlayerInfo::setIsBossRoom( bool isBoss )
{
	btnInviteFriend->setPositionX( isBoss?85:151 );
	btnUnFriend->setPositionX( isBoss?85:151 );
	btnKick->setVisible( isBoss );
}

void LayerPlayerInfo::setAvatarUrl( string url )
{
	SceneManager::getSingleton().getImageDownloadProcess()->downLoadImage(nodeAvatar, url);
}

void LayerPlayerInfo::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}
