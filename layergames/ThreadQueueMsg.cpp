#include "ThreadQueueMsg.h"
#include "_Card_inGame_.h"

ThreadQueueMsg::ThreadQueueMsg()
{
}

ThreadQueueMsg::~ThreadQueueMsg()
{
	//mtxRunningAction.unlock();
}

void ThreadQueueMsg::run()
{
	//while(!isSignalDestroy)
	{
		//boost::mutex::scoped_lock lock(mtxRunningAction);
		//
		if(mQueue.size()==0){
			//log("wait msg incoming");
			return;
		}
		//
		//boost::shared_ptr<ISFSObject> pObjs = deQueue();
		if (mTarget && mFunctionSelector)
		{
			//log("Send msg 2 class to process");
			(mTarget->*mFunctionSelector)();
		}
	}
}

boost::shared_ptr<ISFSObject> ThreadQueueMsg::deQueue()
{
	boost::shared_ptr<ISFSObject> objs = mQueue.front();
	//log("get message from queue");
	mQueue.pop();
	return objs;
}

void ThreadQueueMsg::addQueue( boost::shared_ptr<ISFSObject> objs)
{
	//boost::mutex::scoped_lock lock(mtxRunningAction);
	//log("add message to queue");
	mQueue.push(objs);
	//condVar.notify_one();
}

void ThreadQueueMsg::setFunCallBack(Ref *target, SEL_CallFunc listener)
{
	mTarget = target;
	mFunctionSelector = listener;
}

bool ThreadQueueMsg::isQueueEmpty()
{
	return mQueue.size()==0;
}

boost::shared_ptr<ISFSObject> ThreadQueueMsg::frontQueue()
{
	boost::shared_ptr<ISFSObject> objs = mQueue.front();
	return objs;
}

