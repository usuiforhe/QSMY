//
//  EquipRefineCell.h
//  装备精炼cell
//  Created by mzp on 14-04-09.
//	Modified 
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#ifndef __QSMY_Equip_Refine_Cell__
#define __QSMY_Equip_Refine_Cell__


#include "AppInclude.h"
#include "../../../components/components.h"
#include "model/ItemModel.h"
#include "../common/common.h"


#define D_EVENT_EQUIP_REFINE_UPDATED "equip refine update"
#define D_EVENT_EQUIP_REFINE_ADD "equip refine add"


class EquipRefineData : public ValueObjcet
{
	CC_SYNTHESIZE(uint32_t, m_nId, ID);
	CC_SYNTHESIZE(std::string, m_name, Name);
	CC_SYNTHESIZE_READONLY(uint32_t, m_nCount, Count);

public:
	EquipRefineData();
	~EquipRefineData();

	static EquipRefineData* create(unsigned int id);
	virtual bool init(unsigned int id);

	void setCount(uint32_t val);

	int getAdded();
	int getRemain();

	void add();
	void fixAdd(int num);

private:
	int m_nAdd;
};


class EquipRefineCell
	: public DLayer
	, public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE_READONLY(EquipRefineData*, m_pData, Data);

public:
	CREATE_FUNC(EquipRefineCell);
	EquipRefineCell();
	virtual ~EquipRefineCell();
	D_INIT_DEFAULT(DLayer);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual bool dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches = NULL);
	virtual void dTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void dTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void update(float dt);
	void setRefineMax(bool bMax);

public:
	void setData(EquipRefineData* pData);

private:
	void updateView();

private:
	cocos2d::CCLabelTTF* m_pLabelName;
	cocos2d::CCLabelBMFont* m_pLabelCount;
	ItemBox* m_pItemBox;
	cocos2d::extension::CCScale9Sprite* m_pMark;
    bool m_bIgnoreTouchEnd;

	bool m_bMax;
};


class EquipRefineCellLoader : public cocos2d::extension::CCNodeLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(EquipRefineCellLoader, loader);

protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(EquipRefineCell);
};


#endif	// __QSMY_Equip_Refine_Cell__