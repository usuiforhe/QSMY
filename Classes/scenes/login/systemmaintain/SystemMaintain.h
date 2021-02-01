//
//  SystemMaintain.h
//  QSMY
//
//  Created by wanghejun on 13-9-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SystemMaintain__
#define __QSMY__SystemMaintain__

#include "AppInclude.h"
#include "utils/SystemUtil.h"
#include "components/components.h"

class SystemMaintain
:public cocos2d::CCLayer
,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
    
    static SystemMaintain* sharedSystemMaintain(void);
    static void purgeSystemMaintain(void);
    
    SystemMaintain();
    virtual ~SystemMaintain();
    //CREATE_FUNC(SystemMaintain);
    bool init();
    void onEnter();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
private:
    void close(cocos2d::CCObject*);
private:
    CCWebView       *m_pWebView;
private:
    DButton         *m_pCloseButton;
    cocos2d::CCNode *m_pContainer;
    
};

#endif /* defined(__QSMY__SystemMaintain__) */
