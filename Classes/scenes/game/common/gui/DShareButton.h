//
//  DShareButton.h
//  分享按钮类
//
//  Created by heven on 14-4-29.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__DShareButton__
#define __QSMY__DShareButton__

#include "components/components.h"

class ShareModel;
class DShareButton
:public DButton
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pAward, AwardMark);
    
public:
    CREATE_FUNC(DShareButton);
    DShareButton();
    virtual ~DShareButton();
    D_INIT_DEFAULT(DButton);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    virtual void onEnter();
    virtual void onExit();
private:
    void updateView();
private:
    ShareModel * m_pModel;
};


class DShareButtonLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(DShareButtonLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(DShareButton);
};


#endif /* defined(__QSMY__DShareButton__) */
