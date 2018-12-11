#ifndef NotificationCallBack_h_
#define NotificationCallBack_h_

#include <stdio.h>
class NotificationCallBack{
public:
	virtual void notificationCallBack(bool isOK, int tag) {};
};

#endif //NotificationCallBack_h