//
// Notification.cpp
//  QSMY
//
//  Created by wanghejun on 13-4-1.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Notification.h"
#include "cocoa/CCArray.h"
#include "cocos2d.h"
#include <string>
#include "../AppMacros.h"
#include "CCLuaEngine.h"

using namespace std;

USING_NS_CC;

#pragma mark NotificationCenter
static NotificationCenter *s_SharedNotificationCenter = NULL;

NotificationCenter* NotificationCenter::sharedNotificationCenter(void)
{
    if (!s_SharedNotificationCenter)
    {
        s_SharedNotificationCenter = new NotificationCenter();
        s_SharedNotificationCenter->init();
    }
    
    return s_SharedNotificationCenter;
}


void NotificationCenter::purgeNotificationCenter(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedNotificationCenter);
}

#pragma mark NotificationInfo

NotificationInfo* NotificationInfo::create(const string &name, float fTime, cocos2d::CCObject* target, NotificationInfo_CallFunc selecter)
{
	NotificationInfo *pInfo = new NotificationInfo(name, fTime, target, selecter);
	if (pInfo)
	{
		pInfo->autorelease();
	}else
    {
        CC_SAFE_DELETE(pInfo);
    }
    
	return pInfo;
}

NotificationInfo::NotificationInfo(const string &name, float fTime,cocos2d::CCObject* target, NotificationInfo_CallFunc selecter)
{
	m_target	= target;
	m_selector	= selecter;
	m_name		= name;
    CC_SAFE_RETAIN(m_target);
    cocos2d::CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(NotificationInfo::scheduleUpdate), this, fTime,0,0.f, false);
}

NotificationInfo::~NotificationInfo()
{
    CC_SAFE_RELEASE(m_target);
}

void NotificationInfo::scheduleUpdate(float dt)
{
	if (m_target)
	{
        Notification* pTarget = dynamic_cast<Notification*>(m_target);
		if(pTarget)(pTarget->*m_selector)((void*)m_name.c_str());
	}
}


#pragma mark Notification
Notification::Notification()
:m_pThis(NULL)
{
    m_observers = CCArray::createWithCapacity(3);
    m_observers->retain();
}

Notification::Notification(Notification const &src)
:m_pThis(NULL)
{
	m_observers = CCArray::createWithCapacity(3);
    m_observers->retain();
}

Notification::~Notification()
{
    m_observers->release();
}

bool Notification::init()
{
    m_pThis = dynamic_cast<cocos2d::CCObject*>(this);
    return m_pThis!=NULL;
}


//
// internal functions
//
bool Notification::observerExisted(CCObject *target,const char *name)
{
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        Observer* observer = (Observer*) obj;
        if (!observer)
            continue;
        
        if (!strcmp(observer->getName(),name) && observer->getTarget() == target)
            return true;
    }
    return false;
}

void Notification::registerScriptObserver( CCObject *target, int handler,const char* name)
{
    
    if (this->observerExisted(target, name))
        return;
    
    Observer *observer = new Observer(target, name);
    if (!observer)
        return;
    
    observer->setHandler(handler);
    observer->autorelease();
    m_observers->addObject(observer);
}

void Notification::unregisterScriptObserver(CCObject *target,const char* name)
{
    removeObserver(target,name);
}

//
// observer functions
//
bool Notification::addObserver(CCObject *target,
                               Notification_CallFunOO selector,
                               const char *name)
{
    if (this->observerExisted(target, name))
        return false;
    
    Observer *observer = new Observer(target, selector, name);
    if (!observer)
        return false;
    
    observer->autorelease();
    m_observers->addObject(observer);
    
    return true;
}

//
// observer functions
//
bool Notification::addObserver(CCObject *target,
                               Notification_CallFuncO selector,
                               const char *name)
{
    if (this->observerExisted(target, name))
        return false;
    
    Observer *observer = new Observer(target, selector, name);
    if (!observer)
        return false;
    
    observer->autorelease();
    m_observers->addObject(observer);
    return true;
}

bool Notification::addObserver(CCObject *target,
                               Notification_CallFunc selector,
                               const char *name)
{
    if (this->observerExisted(target, name))
        return false;
    
    Observer *observer = new Observer(target, selector, name);
    if (!observer)
        return false;
    
    observer->autorelease();
    m_observers->addObject(observer);
    return true;
}

void Notification::removeObserver(CCObject *target,const char *name)
{
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_observers, obj)
    {
        Observer* observer = (Observer*) obj;
        if (!observer)
            continue;
        
        if (!strcmp(observer->getName(),name) && observer->getTarget() == target)
        {
            m_observers->removeObject(observer);
            return;
        }
    }
}


int Notification::removeAllObservers(CCObject *target)
{
    CCObject *obj = NULL;
    CCArray *toRemove = CCArray::create();
    
    CCARRAY_FOREACH(m_observers, obj)
    {
        Observer *observer = (Observer *)obj;
        if (!observer)
            continue;
        
        if (observer->getTarget() == target)
        {
            toRemove->addObject(observer);
        }
    }
    
    m_observers->removeObjectsInArray(toRemove);
    return toRemove->count();
}


//void Notification::postNotification(const char *name)
//{
//    CCArray* ObserversCopy = CCArray::createWithCapacity(m_observers->count());
//    ObserversCopy->addObjectsFromArray(m_observers);
//    CCObject* obj = NULL;
//    CCARRAY_FOREACH(ObserversCopy, obj)
//    {
//        Observer* observer = (Observer*) obj;
//        if (!observer)
//            continue;
//
//        if (!strcmp(name,observer->getName()))
//            observer->performSelector();
//    }
//}


void Notification::postNotification(const char *name, cocos2d::CCObject *pData)
{
    CCAssert(m_pThis,"" );
    CCPoolManager::sharedPoolManager()->push();
    CCArray* ObserversCopy = CCArray::createWithCapacity(m_observers->count());
    ObserversCopy->addObjectsFromArray(m_observers);
    
    CCArray* observers = CCArray::create();
    CCObject* obj = NULL;
    
    CCARRAY_FOREACH(ObserversCopy, obj)
    {
        Observer* observer = (Observer*) obj;
        if (!observer)
            continue;
        
        if (!strcmp(name,observer->getName()))
        {
            observers->addObject(observer);
            observer->getTarget()->retain();
            observer->getTarget()->autorelease();
        }
    }
    
    CCARRAY_FOREACH(observers, obj)
    {
        Observer* observer = (Observer*) obj;
        if(pData)
        {
            if (0 != observer->getHandler())
            {
                CCScriptEngineProtocol* engine = CCScriptEngineManager::sharedManager()->getScriptEngine();
                CCLuaEngine* luaEngine = dynamic_cast<CCLuaEngine* >(engine);
                if(luaEngine)
                {
                    cocos2d::CCLuaStack* stack = luaEngine->getLuaStack();
                    stack->clean();
                    stack->pushCCObject(pData,"CCObject");
                    stack->pushString(name);
                    stack->executeFunctionByHandler(observer->getHandler(), 2);
                }else engine->executeEvent(observer->getHandler(), name);
            }else observer->performSelector(m_pThis,pData);
        }else
        {
            if (0 != observer->getHandler())
            {
                CCScriptEngineProtocol* engine = CCScriptEngineManager::sharedManager()->getScriptEngine();
                engine->executeEvent(observer->getHandler(), name);
                
            }else observer->performSelector(m_pThis);
        }
        
    }
    
    CCPoolManager::sharedPoolManager()->pop();
}

void Notification::_postNotification(void *name)
{
	return postNotification((const char *)name);
}

void Notification::postNotification(const char *name, float fTimeout)
{
	if (!name || strlen(name)==0)
	{
		return;
	}
    
  	if (D_FLOAT_EQUALS(fTimeout, 0.f))
	{
		postNotification(name);
		return;
	}
	NotificationInfo::create(name, fTimeout, m_pThis, notificationInfo_callfunc_selector(Notification::_postNotification));
}

#pragma mark Observer


Observer::Observer(CCObject *target,
                   const char *name)
:m_target(target)
,m_selectorO(NULL)
,m_selector(NULL)
,m_selectorOO(NULL)
{
    m_name = new char[strlen(name)+1];
    memset(m_name,0,strlen(name)+1);
    
    string orig (name);
    orig.copy(m_name,strlen(name),0);
    m_nHandler = 0;
}

Observer::Observer(CCObject *target,
                   Notification_CallFuncO selector,
                   const char *name)
:m_target(target)
,m_selectorO(selector)
,m_selector(NULL)
,m_selectorOO(NULL)
{
    m_name = new char[strlen(name)+1];
    memset(m_name,0,strlen(name)+1);
    
    string orig (name);
    orig.copy(m_name,strlen(name),0);
    m_nHandler = 0;
}

Observer::Observer(CCObject *target,
                   Notification_CallFunOO selector,
                   const char *name)
:m_target(target)
,m_selectorOO(selector)
,m_selector(NULL)
,m_selectorO(NULL)
{
    m_name = new char[strlen(name)+1];
    memset(m_name,0,strlen(name)+1);
    
    string orig (name);
    orig.copy(m_name,strlen(name),0);
    m_nHandler = 0;
}

Observer::Observer(CCObject *target,
                   Notification_CallFunc selector,
                   const char *name)
:m_target(target)
,m_selectorO(NULL)
,m_selector(selector)
,m_selectorOO(NULL)
{
    m_name = new char[strlen(name)+1];
    memset(m_name,0,strlen(name)+1);
    
    string orig (name);
    orig.copy(m_name,strlen(name),0);
    m_nHandler = 0;
}

Observer::~Observer()
{
    CC_SAFE_DELETE_ARRAY(m_name);
}

void Observer::performSelector(CCObject* pTarget)
{
    if (m_target)
    {
        if(m_selector) (m_target->*m_selector)();
		if(m_selectorO) (m_target->*m_selectorO)(pTarget);
    }
}

void Observer::performSelector(CCObject* pTarget,CCObject* pData)
{
    if (m_target)
    {
        if(m_selectorO) (m_target->*m_selectorO)(pData);//优先传送数据
		if(m_selectorOO) (m_target->*m_selectorOO)(pTarget,pData);
    }
}

CCObject *Observer::getTarget()
{
    return m_target;
}

Notification_CallFunOO Observer::getSelectorOO()
{
    return m_selectorOO;
}

Notification_CallFuncO Observer::getSelectorO()
{
    return m_selectorO;
}

Notification_CallFunc Observer::getSelector()
{
    return m_selector;
}

char *Observer::getName()
{
    return m_name;
}

int Observer::getHandler()
{
    return m_nHandler;
}

void Observer::setHandler(int var)
{
    m_nHandler = var;
}

