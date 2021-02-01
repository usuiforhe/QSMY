//
//  RewardItemBox.h
//  QSMY
//
//  Created by wanghejun on 13-9-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__RewardItemBox__
#define __QSMY__RewardItemBox__

#include "AppInclude.h"
#include "model/PveModel.h"


class WarriorHeadGradeBg;
class RewardItemBox
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(WarriorHeadGradeBg*, m_pGrade, Grade);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pContainer, Container);
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pTouchEvent, TouchEvent);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pName, Name);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pCount, Count);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pVipLevel, VipLevel);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pVip, Vip);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pCanGet, CanGet);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pGotten, Gotten);
    
public:
    CREATE_FUNC(RewardItemBox);
    RewardItemBox();
    virtual ~RewardItemBox();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    /**
     *	@brief	设置显示信息
     *
     *	@param 	pInfo 	章节信息
     *	@param 	vipLevel 	要显示奖励对应的vip等级
     */
    void setInfo(pve::ChapterInfo* pInfo,uint8_t vipLevel);
private:
    void updateStatus();
private:
    uint8_t             m_nVipLevel;
    pve::ChapterInfo*   m_pChapterInfo;

};


class RewardItemBoxLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(RewardItemBoxLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(RewardItemBox);
};


#endif /* defined(__QSMY__RewardItemBox__) */
