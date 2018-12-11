#ifndef __NATIVE_HELPER_H
#define __NATIVE_HELPER_H

#include "cocos2d.h"
#include <string>

class NativeHelper
{
public:
	static void sendPushNotification();
	static void callVibrate(int number);
	static void callSendSMS(const char* dauSo, const char* cuPhap);
	static void inviteSMS(const char* phoneNumber, const char* content);
	static std::string getDeviceId();
	static void callSendEmail(const char* address, const char* subject, const char* content);
	static void googlePlusLogin();
	static bool hasConnection();
	static std::string getPartnerID();
    static std::string getIPAddress();
	static void callPhone(const char* phoneNumber);
    static void openUrl(const char* url);
    static void trackScreen(const char* screenName);
	static void callChooseImage(const char* token);
    static void openStore(const char* packageName);
    static void trackNapTien(const char* menhgia);
    static void trackDoiThuong(const char* menhgia);
    static void trackLogin(const char* uid);
	static void saveScreenShot(const char* filename);
    static void reloadLang(int lang);
    static void subcribeTopic(const char* phoneNumber);
    static void eventPurchase(int num);
    static void eventCashout(int num);
};

#endif // __NATIVE_HELPER_H
