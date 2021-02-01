//
//  UserLevelup.h
//  用户升级弹出框
//
//  Created by cll on 13-07-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__UserLevelup__
#define __QSMY__UserLevelup__

#include "AppInclude.h"
#include "../Game.h"
#include "components/components.h"
#include "model/UserModel.h"

class UserLevelup : 
	public DPopup, 
	public cocos2d::extension::CCBMemberVariableAssigner,
	public cocos2d::extension::CCBSelectorResolver
{      
public:
	UserLevelup();
	virtual ~UserLevelup();
	virtual bool init();
	CREATE_FUNC(UserLevelup);
    virtual void closePopup();
	virtual void onBackKey();
    
    D_POPUP_PRIORITY(DPOPUP_USER_LEVELUP_PRIORITY);

	virtual bool									 onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler				 onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName){return NULL;}
public:
    
    virtual void onEnter();
	void updateInfo();

	void onConfirmBut(cocos2d::CCObject* obj);
    void onNavBut(cocos2d::CCObject* obj);
private:
	cocos2d::CCLabelBMFont*			m_award1Num;			//奖励元宝数	-- 对应用户iPoints
	cocos2d::CCLabelBMFont*			m_award2Num;			//奖励银币数	-- 对应用户iCoins
	cocos2d::CCLabelBMFont*			m_maxPower;				//最大体力
	cocos2d::CCLabelBMFont*			m_pEndurance;			//最大耐力
	cocos2d::CCLabelBMFont*			m_unlockWarriorNum;		//最大上阵弟子
    cocos2d::CCLabelBMFont*			m_pLevel;               //等级
	cocos2d::CCNode*                m_pCloseButton;			//确定按钮
 //   cocos2d::CCNode*                m_pEffectNode;          //效果
 //   cocos2d::CCNode*                m_pLightEffectNode;     //光效果
	cocos2d::CCNode*					m_pConfirmNode;
	cocos2d::CCNode*					m_pFormationNode;
	DButton*									m_pConfirmButton;
	DButton*									m_pFormationButton;
};
#endif /* defined(__QSMY__UserLevelup__) */
