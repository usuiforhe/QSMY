//
//  DMessageBox.h
//  提示框
//
//  Created by ldr123 on 13-7-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DMessageBox__
#define __QSMY__DMessageBox__

#include "AppInclude.h"
#include "DLayer.h"
#include "gui/DButton.h"

enum MessageType
{
    kMessageTypeOk = 0,
    kMessageTypeCancel
};

class DMessageBox;

typedef void (cocos2d::CCObject::*MessageBox_CallFunc)(MessageType,DMessageBox*);
#define messageBox_callfunc_selector(_SELECTOR) (MessageBox_CallFunc)(&_SELECTOR)

class DMessageBox : public DLayerModal, public cocos2d::extension::CCBMemberVariableAssigner, public cocos2d::extension::CCBSelectorResolver
{
public:
    DMessageBox();
    virtual ~DMessageBox();
    
public:
    static void setContainer(cocos2d::CCNode*);
    
    static DMessageBox * showMessageBox(const char *pMsg);
    static DMessageBox * showMessageBox(const char *pMsg,cocos2d::CCObject* target,MessageBox_CallFunc callFn);
public:
    static DMessageBox * create(const char *pMsg);
    static DMessageBox * create(const char *pMsg,cocos2d::CCObject* target,MessageBox_CallFunc callFn);
    virtual bool init(const char *pMsg,cocos2d::CCObject* target,MessageBox_CallFunc callFn);
public:
	virtual bool										onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler					onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler	onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName){return NULL;}
public:
	void onTouchOk(CCObject*);
	void onTouchCancel(CCObject*);
    D_TOUCH_REGISTER_DEFAULT( DTOUCH_ALERT_PRIORITY );
    
private:
	cocos2d::CCObject       *m_pTarget;
    MessageBox_CallFunc     m_pCallBack;
private:
	cocos2d::CCLabelTTF*		m_msg;
	DButton*		m_pOK;
	DButton*		m_pCancel;
};

#endif /* defined(__QSMY__DMessageBox__) */
