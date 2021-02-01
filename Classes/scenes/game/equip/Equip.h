//
//  Equip.h
//  装备模块
//  Created by wanghejun on 13-3-12.
//	Modified by cll on 13-4-1
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Equip__
#define __QSMY__Equip__

#include "AppInclude.h"
#include "components/components.h"
#include "model/EquipModel.h"
#include "EquipDelegate.h"
#include "../Game.h"
#include "EquipMultiSelect.h"
#include "EquipBurn.h"

class EquipView;
class EquipInfoView;
class EquipRefine;
class EquipMultiSelect;
class EquipDeletePopup;

class Equip :public DLayer,public EquipDelegate
	,public BackKeyInterface
	,public EquipMultiSelecteDelegate
	,public EquipBurnCloseDelegate
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	Equip();
	~Equip();
	virtual bool init();
	CREATE_FUNC(Equip);

	virtual void onGradeSelect(std::vector<equip::EquipGrade> v);
	virtual void onBurnClose();
private:
    virtual void closeInfoView(EquipViewType type);
    virtual void showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param = NULL);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
	virtual void showEquipList();
	virtual void showEquipInfo(equip::EquipCurInfo*);
	virtual void showEquipEnhance(equip::EquipCurInfo*);
	virtual void showEquipRefine(equip::EquipCurInfo*);
	virtual void doDecompose(equip::EquipCurInfo*);

	void showDeletePopup();
	

	void decomposeCallBack(cocos2d::CCObject* obj);
	void sellCallBack(cocos2d::CCObject* obj);
	void burnCallBack(cocos2d::CCObject* obj);
	virtual void selectedEquips(cocos2d::CCArray* pArray);

private:
    CCNode      *m_pContainer;
	CCNode		*m_pEnhanceNode;
	DTabView    *m_pDTabView;
	DButton		*m_pDecompose;
	cocos2d::CCMenuItemImage		*m_pSellButton;
	cocos2d::CCNode *m_pTableNode;
	cocos2d::CCNode *m_pContainerPos;
	cocos2d::CCNode *m_pBottom;

	EquipRefine			*m_pEquipRefine;
	EquipMultiSelect	*m_pEquipMultiSelect;
	EquipBurn			*m_pEquipBurn;
	EquipDeletePopup	*m_pEquipDeletePopup;

	cocos2d::CCArray* m_pTabViewArray;

public:
	virtual void onBackKey();
};

#endif /* defined(__QSMY__Equip__) */
