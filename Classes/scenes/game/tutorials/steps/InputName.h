//
//  InputName.h
//  输入名字
//
//  Created by wanghejun on 13-9-17.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__InputName__
#define __QSMY__InputName__

#include "../Tutorial.h"
#include "json/json.h"

class InputName
:public StepEnForce
,public cocos2d::extension::CCEditBoxDelegate
{
public:
    InputName();
    virtual ~InputName();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    
    virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);
    
protected:
    void submitCallBack(cocos2d::CCObject* obj);
    void randNameCallBack(cocos2d::CCObject* obj);
    void randNameCallBack(const Json::Value&,const Json::Value&);
protected:
    virtual void loadUI();
    virtual void finishStep();
    
private:
    cocos2d::extension::CCEditBox   *m_pName;
    DButton                         *m_pSubmitButton;    //性格
    cocos2d::CCNode                 *m_pContainer;        //特点

};


#endif /* defined(__QSMY__InputName__) */
