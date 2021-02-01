//
//  Notification.h
//  QSMY
//
//  Created by wanghejun on 13-4-1.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Notification__
#define __QSMY__Notification__

#include "cocoa/CCObject.h"
#include "cocoa/CCArray.h"
#include <string>
#include <list>



typedef void (cocos2d::CCObject::*Notification_CallFunc)();
typedef void (cocos2d::CCObject::*Notification_CallFuncO)(cocos2d::CCObject*);
typedef void (cocos2d::CCObject::*Notification_CallFunOO)(cocos2d::CCObject*,cocos2d::CCObject*);

#define notification_callfunc_selector(_SELECTOR) (Notification_CallFunc)(&_SELECTOR)
#define notification_callfuncO_selector(_SELECTOR) (Notification_CallFuncO)(&_SELECTOR)
#define notification_callfuncOO_selector(_SELECTOR) (Notification_CallFuncOO)(&_SELECTOR)

#define BINDING_EXEC(__source__,__target__,__call_back_func__,__notifyName__)		\
if(__source__ && __target__)                                                        \
{                                                                                   \
    if(__source__->addObserver(__target__, notification_callfunc_selector(__call_back_func__) , __notifyName__)){__call_back_func__();};          \
}else  __call_back_func__();                                                                                                      \


#define BINDING_ONLY(__source__,__target__,__call_back_func__,__notifyName__)		\
if(__source__ && __target__){	__source__->addObserver(__target__, notification_callfunc_selector(__call_back_func__) , __notifyName__); }	

#define BINDING_EXEC_O(__source__,__target__,__call_back_func__,__notifyName__)		\
if(__source__ && __target__)                                                        \
{                                                                                   \
    if(__source__->addObserver(__target__, notification_callfuncO_selector(__call_back_func__) , __notifyName__)){__call_back_func__();};          \
}else  __call_back_func__();

#define BINDING_ONLY_O(__source__,__target__,__call_back_func__,__notifyName__)		\
if(__source__ && __target__){	__source__->addObserver(__target__, notification_callfuncO_selector(__call_back_func__) , __notifyName__); }

#define BINDING_EXEC_OO(__source__,__target__,__call_back_func__,__notifyName__)		\
if(__source__ && __target__)                                                        \
{                                                                                   \
    if(__source__->addObserver(__target__, notification_callfuncOO_selector(__call_back_func__) , __notifyName__)){__call_back_func__();};          \
}else  __call_back_func__();

#define BINDING_ONLY_OO(__source__,__target__,__call_back_func__,__notifyName__)		\
if(__source__ && __target__){	__source__->addObserver(__target__, notification_callfuncOO_selector(__call_back_func__) , __notifyName__); }

#define UNBINDING_ALL(__source__,__target__)      \
if(__source__ && __target__){                     \
    __source__->removeAllObservers(__target__);       \
}                                                 

#define UNBINDING(__source__,__target__,__notifyName__)\
if(__source__ && __target__){                     \
__source__->removeObserver(__target__,__notifyName__);       \
}                                                   

#define SAFE_RELEASE_UNBINDING_ALL(__source__,__target__)      \
if(__source__){                     \
    if(__target__)                  \
    {                                   \
        __source__->removeAllObservers(__target__);\
    }\
__source__->release();                            \
}

class Notification;
typedef void (Notification::*NotificationInfo_CallFunc)(void *);
#define notificationInfo_callfunc_selector(_SELECTOR) (NotificationInfo_CallFunc)(&_SELECTOR)

class NotificationInfo : public cocos2d::CCObject
{
public:
	static NotificationInfo *create(const std::string &name, float fTime, cocos2d::CCObject* target, NotificationInfo_CallFunc selecter);

	explicit NotificationInfo(const std::string &name, float fTime, cocos2d::CCObject* target, NotificationInfo_CallFunc selecter);
    virtual ~NotificationInfo();

	std::string strName;

	void scheduleUpdate(float);
    
private:
	cocos2d::CCObject*			m_target;
	NotificationInfo_CallFunc	m_selector;
	std::string					m_name;
};

class Notification
{
public:
    /** Notification constructor */
    Notification();
    
    Notification(Notification const &src);
    /** Notification destructor */
    virtual ~Notification();
    
    virtual bool init();
    
    /** @brief Registers one hander for script binding.
     *  @note Only supports Lua Binding now.
     *  @param handler The lua handler.
     */
    void registerScriptObserver(cocos2d::CCObject *target,int handler,const char* name);
    
    /** Unregisters script observer */
    void unregisterScriptObserver(cocos2d::CCObject *target,const char* name);
    
    /** @brief Adds an observer for the specified target.
     *  @param target The target which wants to observe notification events.
     *  @param selector The callback function which will be invoked when the specified notification event was posted.
     *  @param name The name of this notification.
     *  @param obj The extra parameter which will be passed to the callback function.
     */
    virtual bool addObserver(cocos2d::CCObject *target,
                     Notification_CallFuncO selector,
                     const char *name);
    
    virtual bool addObserver(cocos2d::CCObject *target,
                             Notification_CallFunc selector,
                             const char *name);
    
    virtual bool addObserver(cocos2d::CCObject *target,
                             Notification_CallFunOO selector,
                             const char *name);
    
    /** @brief Removes the observer by the specified target and name.
     *  @param target The target of this notification.
     *  @param name The name of this notification.
     */
    virtual void removeObserver(cocos2d::CCObject *target,const char *name);

    /** @brief Removes all notifications registered by this target
     *  @param target The target of this notification.
     *  @returns the number of observers removed
     */
    int removeAllObservers(cocos2d::CCObject *target);
    
    /** @brief Posts one notification event by name.
     *  @param name The name of this notification.
     */
    virtual void postNotification(const char *name, cocos2d::CCObject *pData = NULL);
	virtual void postNotification(const char *name, float fTimeout);
    
    // Check whether the observer exists by the specified target and name.
    bool observerExisted(cocos2d::CCObject *target,const char *name);
    
private:
    // internal functions
	void _postNotification(void *name);
    // variables
    //
    cocos2d::CCArray *m_observers;
    cocos2d::CCObject *m_pThis;
};

class Observer : public cocos2d::CCObject
{
public:
    /** @brief Observer constructor
     *  @param target The target which wants to observer notification events.
     *  @param selector The callback function which will be invoked when the specified notification event was posted.
     *  @param name The name of this notification.
     *  @param obj The extra parameter which will be passed to the callback function.
     */
    Observer(cocos2d::CCObject *target,
                           Notification_CallFuncO selector,
                           const char *name);
    Observer(cocos2d::CCObject *target,
             Notification_CallFunOO selector,
             const char *name);
    Observer(cocos2d::CCObject *target,
             Notification_CallFunc selector,
             const char *name);
    Observer(cocos2d::CCObject *target,
             const char *name);
    /** Observer destructor function */
    ~Observer();
    
    /** Invokes the callback function of this observer */
    void performSelector(cocos2d::CCObject* pTarget);
    void performSelector(cocos2d::CCObject* pTarget,cocos2d::CCObject* pData);
private:
    CC_PROPERTY_READONLY(cocos2d::CCObject *, m_target, Target);
    CC_PROPERTY_READONLY(Notification_CallFunc, m_selector, Selector);      //没有参数
    CC_PROPERTY_READONLY(Notification_CallFuncO, m_selectorO, SelectorO);   //一个ccobj参数 一个target或data
    CC_PROPERTY_READONLY(Notification_CallFunOO, m_selectorOO, SelectorOO);//两个ccobj参数 一个target 一个data
    CC_PROPERTY_READONLY(char *, m_name, Name);
    CC_PROPERTY(int, m_nHandler,Handler);
};


class NotificationCenter
:public cocos2d::CCObject
,public Notification
{
public:
    
    static NotificationCenter* sharedNotificationCenter(void);
    static void purgeNotificationCenter(void);
    
};


#endif /* defined(__QSMY__Notification__) */