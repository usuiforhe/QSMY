//
//  EquipMultiSelect.h
//  装备多选列表
//  Created by mzp on 14-04-11.
//  Copyright (c) 2013, thedream. All rights reserved.
//
#ifndef __QSMY_Equip_View_Ex__
#define __QSMY_Equip_View_Ex__


#include "AppInclude.h"
#include "components/components.h"
#include "model/EquipModel.h"
#include "EquipMultiSelectView.h"
#include "EquipDelegate.h"
#include <map>


class EquipMultiSelecteDelegate
{
public:
	virtual void selectedEquips(cocos2d::CCArray*) = 0;
	//new interface
	virtual void showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param = NULL) = 0;
};


class EquipMultiSelect
	: public DLayer
	, public DTabViewDelegate
	, public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE(EquipMultiSelecteDelegate*, m_pEquipMultiSelecteDelegate, EquipMultiSelecteDelegate);

public:
	EquipMultiSelect();
	~EquipMultiSelect();
	virtual bool init();
	CREATE_FUNC(EquipMultiSelect);

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual void SelectTabAtIndex(DTabView* tabView, uint32_t idx);

	void selectEquip(equip::EquipCurInfo* pEquipCurInfo);
	void unselectEquip(equip::EquipCurInfo* pEquipCurInfo);
	bool isEquipSelected(equip::EquipCurInfo* pEquipCurInfo);
	cocos2d::CCArray* getEquipListByGrade(equip::EquipGrade grade);

	void onGradeSelect(std::vector<equip::EquipGrade> v);

	std::map<equip::EquipGrade, bool> m_pGradeMap;

private:
	void confirmCallback(cocos2d::CCObject* sender);
	void cancelCallback(cocos2d::CCObject* sender);
	void closeCallback(cocos2d::CCObject* sender);
	void showPopup(cocos2d::CCObject* sender);

	void equipListUpdate();
	void updateEquipList();
	void updateTips();
	// 去除已不可分解装备
	void siftEquiped(cocos2d::CCArray* pEquipVec);

private:
	//记录多少钱
	unsigned int m_uTotalPrice;
	EquipModel* m_pEquipModel;
	cocos2d::CCArray* m_pEquipSelectVec;

	cocos2d::CCArray* m_pEquipList;

	DTabView* m_pDTabView;
	EquipMultiSelectView* m_pView;

	cocos2d::CCNode* m_pContainer;
	DButton* m_pConfirmButton;
	DButton* m_pCancelButton;
	DButton* m_pCloseButton;	

	cocos2d::CCMenuItemImage	*m_pSellButton;
	cocos2d::CCMenuItemImage	*m_pGradeSellButton;
	cocos2d::CCLabelBMFont		*m_pSelectCount;
	cocos2d::CCLabelBMFont		*m_pTotalAmount;


	cocos2d::CCNode* m_pImageTips;
	cocos2d::CCNode* m_pImageTipsGreen;
	cocos2d::CCNode* m_pImageTipsBlue;
	cocos2d::CCNode* m_pImageTipsPurple;
	cocos2d::CCNode* m_pImageTipsOrange;

	//EquipInfoView *m_pDeletePopup;
};


#endif