//
//  DButtonSelect.h
//  可设置选择状态按钮
//
//  Created by wanghejun on 13-12-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DButtonSelect__
#define __QSMY__DButtonSelect__

#include "AppInclude.h"

class DButtonSelect
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItemImage*, m_pButton, Button);
    
public:
    CREATE_FUNC(DButtonSelect);
    DButtonSelect();
    virtual ~DButtonSelect();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    /**
     *	@brief	设置是否选中
     *
     *	@param 	b 	
     */
    void setSelected(bool b);
    
    
private:
    cocos2d::CCMenuItemImage* m_pMark;

};


class DButtonSelectLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(DButtonSelectLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(DButtonSelect);
};

#endif /* defined(__QSMY__DButtonSelect__) */
