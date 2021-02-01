//
//  ChangeName.h
//  修改名字
//
//  Created by Eci on 13-11-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ChangeName__
#define __QSMY__ChangeName__


#include "AppInclude.h"
#include "components/components.h"
#include "json/json.h"

class ChangeName
	:public DLayer
	,public cocos2d::extension::CCEditBoxDelegate
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
public:
    ChangeName();
    virtual ~ChangeName();
    
	CREATE_FUNC(ChangeName);
	
	virtual bool init();

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
    virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);
    
protected:
    void submitCallBack(cocos2d::CCObject* obj);
    void randNameCallBack(cocos2d::CCObject* obj);
    void randNameCallBack(const Json::Value&,const Json::Value&);

	void cancelCallBack(cocos2d::CCObject *obj);
protected:
    virtual void loadUI();
	virtual void finishStep();
	virtual void finishStep(const Json::Value&,const Json::Value&);
    
private:
    cocos2d::extension::CCEditBox   *m_pName;
    DButton                         *m_pSubmitButton;
    cocos2d::CCNode                 *m_pContainer;
	DButton        				    *m_pCancel;

};


#endif /* defined(__QSMY__InputName__) */
