//
//  MatchBetView.h
//  下注竞猜界面
//
//  Created by cll on 13-4-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MatchBetView__
#define __QSMY__MatchBetView__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/GameManager.h"
#include "../common/common.h"

class MatchBetView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
	enum
	{
		kBtnBetLeft     = 0,		//支持按钮
		kBtnBetRight    = 1,
        kBtnReplay      = 2,        //回放
	};
    
    CC_SYNTHESIZE_RETAIN(match::VSInfo *, m_pVSInfo, VSInfo);//比赛对阵信息
    
public:
	MatchBetView();
 	virtual ~MatchBetView();
    
	CREATE_FUNC(MatchBetView);
    virtual bool init();

	virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;}
   
private:
    
	void updateItemView();  //更新天灯数量
    void updateItemCount();//更新天灯数量
    void updateVSInfo();//更新对阵支持率
    
    void updateBetView();//更新竞猜列表
    
    void updateView();
	void updateReplay();
	void closeCallBack(cocos2d::CCObject * obj);
	void menuCallBack(cocos2d::CCObject * obj);
	void leftCallBack(cocos2d::CCObject * obj);
	void rightCallBack(cocos2d::CCObject * obj);
	void middleCallBack(cocos2d::CCObject * obj);
    
    void updateWarriors(WarriorHeadSmall** heads,cocos2d::CCArray *infos,bool isLeft);
	// 根据位置获取阵法信息
	match::FormationInfo* getFormationInfoByPid(cocos2d::CCArray* infos, uint8_t pid);
    
private:
    void doCupBet(uint32_t finalNum, uint32_t mid, uint32_t betUser);
    
#pragma mark ccb
private:
    cocos2d::CCLabelBMFont      *m_pTfItemHas;      //天灯剩余
    cocos2d::CCLabelBMFont      *m_pTfItemCost;     //下注额度
    
    cocos2d::CCLabelTTF         *m_pTfNameLeft;     //左边玩家的名字
    cocos2d::CCLabelTTF         *m_pTfNameRight;    //右边玩家的名字
    
    cocos2d::CCLabelBMFont      *m_pTfLevelLeft;    //左边玩家等级
    cocos2d::CCLabelBMFont      *m_pTfLevelRight;   //右边玩家等级
    
    
    cocos2d::CCLabelBMFont      *m_pTfRateLeft;     //支持率
    cocos2d::CCLabelBMFont      *m_pTfRateRight;    //支持率
    
    cocos2d::CCNode             *m_pBetBtnLeft;     //下注
    cocos2d::CCNode             *m_pBetBtnRight;    //下注
    cocos2d::CCNode             *m_pReplayMenu;     //回放
    
    
    
    WarriorHeadSmall            *m_pWarriorLeft[FORMATION_POS_MAX];   //头像
    WarriorHeadSmall            *m_pWarriorRight[FORMATION_POS_MAX];
    cocos2d::CCLabelBMFont	*m_pLL[FORMATION_POS_MAX];
	cocos2d::CCLabelBMFont	*m_pLR[FORMATION_POS_MAX];

    cocos2d::CCSprite           *m_pImgBetedLeft;
    cocos2d::CCSprite           *m_pImgBetedRight;
    
	cocos2d::CCSprite			*m_pImgLive;
	cocos2d::CCSprite			*m_pImgReplay;

	DButton		*m_pButtonLeft;
	DButton		*m_pButtonRight;
	DButton		*m_pButtonMiddle;
    DButton		*m_pCloseButton;

private:
    ItemModel                   *m_pItemModel;
    MatchModel                  *m_pMatchModel;
    item::ItemInfo              *m_pItem;
    WarriorModel                *m_pWarriorModel;
};



#endif /* defined(__QSMY__MatchBetView__) */
