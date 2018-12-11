#include "NativeHelper.h"
#include "../SceneManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
USING_NS_CC;
const char* AppActivityClassName = "org/cocos2dx/cpp/AppActivity";
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"

#endif
void NativeHelper::sendPushNotification(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "pushNotification", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

void NativeHelper::callVibrate(int number){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "callVibrate", "(I)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, number);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	IOS::vibrate(number);
#endif
}

void NativeHelper::inviteSMS(const char* phoneNumber, const char* content){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;

	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "inviteSMS", "(Ljava/lang/String;Ljava/lang/String;)V"))
	{
		jstring _phoneNum = t.env->NewStringUTF(phoneNumber);
		jstring _mess = t.env->NewStringUTF(content);

		t.env->CallStaticVoidMethod(t.classID, t.methodID, _phoneNum, _mess);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}
void NativeHelper::subcribeTopic(const char* phoneNumber){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
   // log("subscribe");
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "subcribeTopic", "(Ljava/lang/String;)V"))
    {
        jstring _phoneNum = t.env->NewStringUTF(phoneNumber);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, _phoneNum);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void NativeHelper::callPhone(const char* phoneNumber){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;

	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "callPhoneAuto", "(Ljava/lang/String;)V"))
	{
		jstring _phoneNum = t.env->NewStringUTF(phoneNumber);

		t.env->CallStaticVoidMethod(t.classID, t.methodID, _phoneNum);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}
void NativeHelper::openUrl(const char* url){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "openUrl", "(Ljava/lang/String;)V"))
    {
        jstring _url = t.env->NewStringUTF(url);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, _url);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void NativeHelper::trackNapTien(const char* menhgia){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //cocos2d::log("menh gia : %s", menhgia);
    cocos2d::JniMethodInfo t;
    
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "trackNapThe", "(Ljava/lang/String;)V"))
    {
        jstring _menhgia = t.env->NewStringUTF(menhgia);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, _menhgia);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void NativeHelper::reloadLang(int lang){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
   // //cocos2d::log("menh gia : %s", menhgia);
    cocos2d::JniMethodInfo t;
    
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "reloadLang", "(I)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, lang);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void NativeHelper::eventPurchase(int num){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // //cocos2d::log("menh gia : %s", menhgia);
    cocos2d::JniMethodInfo t;
    
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "eventPurchase", "(I)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, num);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void NativeHelper::eventCashout(int num){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // //cocos2d::log("menh gia : %s", menhgia);
    cocos2d::JniMethodInfo t;
    
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "eventCashout", "(I)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, num);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void NativeHelper::trackDoiThuong(const char* menhgia){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //cocos2d::log("menh gia : %s", menhgia);
    cocos2d::JniMethodInfo t;
    
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "trackDoiThuong", "(Ljava/lang/String;)V"))
    {
        jstring _menhgia = t.env->NewStringUTF(menhgia);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, _menhgia);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void NativeHelper::trackLogin(const char* uid){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "trackLogin", "(Ljava/lang/String;)V"))
    {
        jstring _uid = t.env->NewStringUTF(uid);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, _uid);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void NativeHelper::trackScreen(const char* screenName){
	//cocos2d::log("Screen name : %s", screenName);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;

	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "trackScreenView", "(Ljava/lang/String;)V"))
	{
		jstring _screenName = t.env->NewStringUTF(screenName);

		t.env->CallStaticVoidMethod(t.classID, t.methodID, _screenName);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}
void NativeHelper::callSendSMS(const char* dauSo, const char* cuPhap){

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;

	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "sendSMS", "(Ljava/lang/String;Ljava/lang/String;)V"))
	{
		jstring _phoneNum = t.env->NewStringUTF(dauSo);
		jstring _mess = t.env->NewStringUTF(cuPhap);

		t.env->CallStaticVoidMethod(t.classID, t.methodID, _phoneNum, _mess);
		t.env->DeleteLocalRef(t.classID);
	}

#endif
}

std::string NativeHelper::getDeviceId(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string _deviceId = "";
	if (_deviceId.empty())
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getDeviceID", "()Ljava/lang/String;"))
		{
			jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
			_deviceId = JniHelper::jstring2string(str);
			t.env->DeleteLocalRef(str);
			t.env->DeleteLocalRef(t.classID);
		}
	}
    if (SceneManager::getSingleton().isNagaWin){
        _deviceId+="_winclub";
    }
	return _deviceId;
#endif
	return "";
}

std::string NativeHelper::getPartnerID(){
	std::string _partnerId = "1-0";
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//if (_partnerId.empty())
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getPartnerId", "()Ljava/lang/String;"))
		{
			jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
			_partnerId = JniHelper::jstring2string(str);
			t.env->DeleteLocalRef(str);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	return _partnerId;
#endif
	return _partnerId;
}
std::string NativeHelper::getIPAddress(){
	std::string _ipAddress = "192.168.1.1";
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//if (_partnerId.empty())
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getLocalIpAddress", "()Ljava/lang/String;"))
		{
			jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
			_ipAddress = JniHelper::jstring2string(str);
			t.env->DeleteLocalRef(str);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	return _ipAddress;
#endif
	return _ipAddress;
}

bool NativeHelper::hasConnection(){

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bool isConnect;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, AppActivityClassName, "hasConnection", "()Z"))
	{
		jboolean retV = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
		isConnect = (bool)retV;
		t.env->DeleteLocalRef(t.classID);
	}
	return isConnect;
#endif
	return true;
}

void NativeHelper::callSendEmail(const char* address, const char* subject, const char* content)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, AppActivityClassName, "sendEmail", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
	{
		jstring _address = t.env->NewStringUTF(address);
		jstring _subject = t.env->NewStringUTF(subject);
		jstring _content = t.env->NewStringUTF(content);
		// call the method, with arguments
		t.env->CallStaticVoidMethod(t.classID, t.methodID, _address, _subject, _content);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

void NativeHelper::googlePlusLogin()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, AppActivityClassName, "loginGPlus", "()V"))
	{
		// call the method, with arguments
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

void NativeHelper::callChooseImage(const char* token){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, AppActivityClassName, "openImage", "(Ljava/lang/String;)V"))
	{
		jstring _token = t.env->NewStringUTF(token);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, _token);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}
void NativeHelper::openStore(const char* packageName){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;

	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "openStore", "(Ljava/lang/String;)V"))
	{
		jstring _packageName = t.env->NewStringUTF(packageName);

		t.env->CallStaticVoidMethod(t.classID, t.methodID, _packageName);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

void NativeHelper::saveScreenShot(const char* filename)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;

	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "saveScreenShot", "(Ljava/lang/String;)V"))
	{
		jstring _filepath = t.env->NewStringUTF(filename);

		t.env->CallStaticVoidMethod(t.classID, t.methodID, _filepath);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

