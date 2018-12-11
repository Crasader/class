#ifndef THREADQUEUEMSG_H
#define THREADQUEUEMSG_H

#include <boost/thread.hpp>
#include "Requests/ExtensionRequest.h"
#include "Core/BaseEvent.h"
#include <queue>
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace std;
using namespace boost::this_thread;

using namespace cocos2d;
using namespace cocos2d::ui;

class ThreadQueueMsg{
private:
	//boost::mutex mtxRunningAction;
	//boost::condition_variable condVar;
	std::queue<boost::shared_ptr<ISFSObject>> mQueue;

	Ref* mTarget;
	SEL_CallFunc mFunctionSelector;
	//void* 
public:
	ThreadQueueMsg();
	~ThreadQueueMsg();
	void setFunCallBack(Ref *target, SEL_CallFunc listener);
	boost::shared_ptr<ISFSObject> deQueue();
	boost::shared_ptr<ISFSObject> frontQueue();
	void addQueue(boost::shared_ptr<ISFSObject> ptrEvent);
	void run();
	bool isQueueEmpty();
};
#endif //THREADQUEUEMSG_H