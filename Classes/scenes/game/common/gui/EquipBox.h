//
//  EquipBox.h
//  装备框
//
//  Created by wanghejun on 13-8-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__EquipBox__
#define __QSMY__EquipBox__

#include "AppInclude.h"
#include "model/EquipModel.h"
#include "WarriorHeadGradeBg.h"

#define EQUIP_BOX_BG_NUM 4

class EquipBox
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCNodeRGBA*, m_pGrade, Grade);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pContainer, Container);
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pTouchEvent, TouchMenu);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pName, Name);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pLevel, Level);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pNameBg, NameBg);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNodeRGBA*, m_pEquipGrade, EquipGrade1);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pUpLevel, UpLevel);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pNoEquip, NoEquip);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pLevelContainer, LevelContainer);
    
    CC_SYNTHESIZE_READONLY(equip::EquipCurInfo*, m_pEquipInfo, EquipInfo);

	CC_SYNTHESIZE_READONLY(CCNode*, m_pCanEnhance, CanEnhance);
	CC_SYNTHESIZE_READONLY(CCNode*, m_pCanAdd, CanAdd);
	//CC_SYNTHESIZE_READONLY(CCNode*, m_pCanRefine, CanRefine);
    CC_SYNTHESIZE_READONLY(equip::EquipType, m_pEquipType, EquipType);

	CC_SYNTHESIZE_READONLY(bool, m_pChangeAble, ChangeAble);
	CC_SYNTHESIZE_READONLY(bool, m_pFateAble, FateAble);
	CC_SYNTHESIZE_READONLY(bool, m_pAddAble, AddAble);
	CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pEquipOnMenu, EquipOnMenu);
	CC_SYNTHESIZE(bool, m_bShowCanEnhance, ShowCanEnhance);	//tolua
	CC_SYNTHESIZE(unsigned int,m_nTypeNum,TypeNum); //默认显示第几个灰态bg

private:
	WarriorHeadGradeBg        *m_pEquipGradeBg;
	//std::vector<cocos2d::CCNode*>  m_vBgs;
	CCNode	*m_vBgs[EQUIP_BOX_BG_NUM];
	CCNode	*m_pFateNode;

	CCNode			*m_pEmpty;
	//CCNode			*m_pMark[equip::kEquipPropTotal];
	CCLabelBMFont	*m_pProperty;
	CCNode			*m_pBottom;
	CCNode			*m_pTxtQianghua;
	CCNode			*m_pTxtJinjie;
public:
    CREATE_FUNC(EquipBox);
    EquipBox();
    virtual ~EquipBox();
    //D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual bool init();
	virtual void onEnter();

	bool canChange();
	
    void setEquipInfo(equip::EquipCurInfo* pInfo, uint32_t wid, equip::EquipType pEquipType = equip::kEquipTypeALL);
    void setEquipInfo(equip::EquipBaseInfo* pInfo);
    void reset();

	//吞噬专用
	void setTreasureInfo(equip::EquipCurInfo* pInfo);
	bool isEmpty(){return m_pEquipInfo == NULL;};
};



class EquipBoxLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(EquipBoxLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(EquipBox);
};

#endif /* defined(__QSMY__EquipBox__) */
