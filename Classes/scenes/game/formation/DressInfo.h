//
//  DressInfo.h
//  时装展示
//  Created by mzp on 14-5-15.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_Dress_Info__
#define __QSMY_Dress_Info__

#include "AppInclude.h"
#include "components/components.h"
#include "../mall/Mall.h"
#include "../equip/EquipEnhance.h"

class EquipPropertyType;

class DressInfo
	: public DLayer
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCBSelectorResolver
{
public:
	DressInfo();
	~DressInfo();

	CREATE_FUNC(DressInfo);

	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName){return NULL;}

	void setDressId(unsigned int id);

	void setDelegate(MallDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

private:
	void updateView();

	void closeCallBack(cocos2d::CCObject* obj);

private:
	cocos2d::CCNode				*m_pContainer;
	cocos2d::CCLabelTTF			*m_pName;
	cocos2d::CCLabelTTF			*m_pName1;
	cocos2d::CCLabelTTF			*m_pDesc;
	DButton						*m_pButton;
	cocos2d::CCNode				*m_pPropertyGroup;
	cocos2d::CCNode				*m_pPropertyGroup1;
	cocos2d::CCLabelBMFont		*m_pProperty;
	EquipTypeMark			*m_pEquipPropertyType;
	cocos2d::CCLabelBMFont		*m_pProperty1;
	EquipTypeMark			*m_pEquipPropertyType1;

	unsigned int m_nId;
	MallDelegate* m_pDelegate;
};



#endif	// __QSMY_Dress_Info__