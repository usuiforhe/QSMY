//
//  UserFeedback.h
//  反馈
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__UserFeedback__
#define __QSMY__UserFeedback__

#include "AppInclude.h"
#include "components/components.h"
#include "json/json.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
typedef cocos2d::extension::DEditBoxDelegate	EditBoxDelegate;
typedef cocos2d::extension::DEditBox			EditBox;
#else
typedef cocos2d::extension::CCEditBoxDelegate	EditBoxDelegate;
typedef cocos2d::extension::CCEditBox			EditBox;
#endif

class MyTextFieldTTF : public EditBox
{
public:
    static MyTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const cocos2d::CCSize& dimensions, cocos2d::CCTextAlignment alignment, const char *fontName, float fontSize);
    void setString(const char *label);
    const char* getString();
    bool attachWithIME();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS || CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)	
	void setDEditBoxContainer(cocos2d::CCNode * var) {};//windows
#endif
};

// #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// 
// class MyTextFieldTTF : public cocos2d::extension::DEditBox
// {
// public:
//     static MyTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const cocos2d::CCSize& dimensions, cocos2d::CCTextAlignment alignment, const char *fontName, float fontSize);
//     void setString(const char *label);
//     const char* getString();
//     bool attachWithIME();
// };
// 
// #else
// 
// class MyTextFieldTTF : public cocos2d::extension::CCEditBox
// {
// public:
//     /** creates a CCTextFieldTTF from a fontname, alignment, dimension and font size */
//     static MyTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const cocos2d::CCSize& dimensions, cocos2d::CCTextAlignment alignment, const char *fontName, float fontSize);
// 	void setString(const char *label);
// 	const char* getString();
//     void setFontColor(const cocos2d::ccColor3B&);
// 	void setDEditBoxContainer(cocos2d::CCNode * var) {};//windows
// };
// 
// #endif



class UserFeedback
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
public:
    UserFeedback();
    virtual ~UserFeedback();
    CREATE_FUNC(UserFeedback);
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

    
    virtual void onExit();
    
private:
    void inputCallBack(cocos2d::CCObject*);
    void resetCallBack(cocos2d::CCObject*);
    void sendCallBack(cocos2d::CCObject*);
    void sendCallBack(const Json::Value &,const Json::Value &);
    
    void reset();
private:
    DButton                     *m_pSend;
    DButton                     *m_pReset;
    cocos2d::CCNode             *m_pContainer;
    MyTextFieldTTF              *m_pTextField;
	CCNode						*m_pNodeService;
    
};


#endif /* defined(__QSMY__UserFeedback__) */
