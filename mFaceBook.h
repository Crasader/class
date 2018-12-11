//
//  FaceBook.hpp
//  iCasnio_v3
//
//  Created by Tuan Anh on 11/17/15.
//
//
#ifndef FaceBook_hpp
#define FaceBook_hpp

//#if ( CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 )
#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#include "cocos2d.h"
#include "PluginFacebook/PluginFacebook.h"
#include "cocos2d.h"
using namespace cocos2d;
class mFaceBook : public sdkbox::FacebookListener
{
public:
	void onLogin();
    void LoginForFriend();
	void onCheckStatus();
	void onGetMyInfo();
	void onGetMyFriends();
	void onCaptureScreen();
	void onShareLink();
	void onSharePhoto();
	void onDialogLink();
	void onDialogPhoto();
	void onRequestReadPermission();
    void onRequestPublishPermission(std::string url);
	void onInviteFriends();
	void onLogout();
	void onGetUID();
	void setFunCallBack(Ref *target, SEL_CallFuncND listener);
	

private:
    int action;
	SEL_CallFuncND mCallBackListener;
	Ref *mCallBack;

	//Facebook callback
	void onLogin(bool isLogin, const std::string& msg);
	void onSharedSuccess(const std::string& message);
	void onSharedFailed(const std::string& message);
	void onSharedCancel();
	void onAPI(const std::string& key, const std::string& jsonData);
	void onPermission(bool isLogin, const std::string& msg);
	void onFetchFriends(bool ok, const std::string& msg);
	void onRequestInvitableFriends(const sdkbox::FBInvitableFriendsInfo& friends);
	void onInviteFriendsWithInviteIdsResult(bool result, const std::string& msg);
	void onInviteFriendsResult(bool result, const std::string& msg);

	void onGetUserInfo(const sdkbox::FBGraphUser& userInfo);

	int LengthUtf8String(const char* s);

	void afterCaptureScreen(bool, const std::string& outputFilename);
	std::string _captureFilename;
    std::string _url_image_share;
    bool isShare = false;
	// static this = FaceBook//;

};
#endif
#endif /* FaceBook_hpp */
