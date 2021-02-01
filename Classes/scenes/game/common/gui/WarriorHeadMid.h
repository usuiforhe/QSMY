//
//  WarriorHeadMid.h
//  弟子中头像公共
//
//  Created by cll on 13-7-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__WarriorHeadMid__
#define __QSMY__WarriorHeadMid__

#include "AppInclude.h"
#include "model/WarriorModel.h"
#include "model/FriendsModel.h"

class WarriorHeadMid
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCNodeRGBA*, m_pHeadBgSprite, HeadBgSprite);			//填充色
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pBgColor1Sprite, BgColor1Sprite);		//背景框 橙品质弟子
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pBgColor2Sprite, BgColor2Sprite);		//背景框 紫品质弟子
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pBgColor3Sprite, BgColor3Sprite);		//背景框 蓝品质弟子
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pBgColor4Sprite, BgColor4Sprite);		//背景框 绿品质弟子
	CC_SYNTHESIZE_READONLY(cocos2d::CCNodeRGBA*, m_pGrade, Grade);		//品阶
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pWarriorHead, WarriorHead);	//头像
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pWarriorName, WarriorName);	//弟子名
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pUpLevel, UpLevel);	//突破等级
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pLevel, Level);	//等级数字
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pExpAdd, ExpAdd);		//经验增加数
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pLevelupEffect, LevelupEffect);	//升级特效
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pWarriorContainer, WarriorContainer);	//弟子容器
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pExpAddNode, ExpAddNode);
    //CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pUnlockLevel, UnlockLevel);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pUnlockLevel, UnlockLevel);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pLock, Lock);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pUnlock, Unlock);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pCanChange, CanChange);
    CC_SYNTHESIZE_READONLY(cocos2d::extension::CCBValue*, m_bFlash, IsFlash);
    
    
public:
    CREATE_FUNC(WarriorHeadMid);
    WarriorHeadMid();
    virtual ~WarriorHeadMid();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	/**
	 *	@brief	更新信息
	 *
	 *	@param 	warrior::WarriorCurInfo* 	弟子信息
	 *	@param 	expAdd 	增加经验
	 *	@param 	isLevelUp 	是否升级
	 *	@param 	positionIndex 	阵型位置
	 */
	void updateView(warrior::WarriorCurInfo*,int expAdd = 0,bool isLevelUp = false,int iPosId = -1);
    
	void updateView(friends::FriendWarrior * friendWarrior, int level);

    void setFlash(bool isFlash);

private:
    void updateAddExp(float num);
};


class WarriorHeadMidLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(WarriorHeadMidLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(WarriorHeadMid);
};

#endif /* defined(__QSMY__WarriorHeadMid__) */
