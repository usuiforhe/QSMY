//
//  WarriorUpgrade.h
//  英雄强化（升级）
//  Created by zhanglong on 14-7-21.
//  Copyright (c) 2014, thedream. All rights reserved.
//

#ifndef __QSMY_Equip_Refine__
#define __QSMY_Equip_Refine__

#include "AppInclude.h"
#include "../common/common.h"
#include "components/components.h"
#include "model/EquipModel.h"
#include "EquipDelegate.h"

class FormationDelegate;
class EquipRefineDelegate
{
public:
	virtual void itemUseCallBack(unsigned int itemId) = 0;
	virtual void itemKeepUseCallBack(unsigned int itemId, unsigned int amountPerUse = 1){};
	virtual void itemKeepUseEndCallBack(unsigned int itemId, unsigned int count){};
	virtual bool isLvlLimit(unsigned int itemId)=0;
};

class EquipTypeMark;
class EquipRefine : public  DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public EquipRefineDelegate
{
public:
	EquipRefine();
	~EquipRefine();

	CREATE_FUNC(EquipRefine);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void setEquip(equip::EquipCurInfo * pInfo);

	void setEquipDelegate(EquipDelegate* pDelegate)
	{
		m_pEquipDelegate = pDelegate;
	}
	//设置阵容信息
	void setFormationDelegate(FormationDelegate* pDelegate)
	{
		m_pFormationDelegate = pDelegate;
	}

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	//EquipRefineDelegate
	virtual void itemUseCallBack(unsigned int itemId);
	virtual void itemKeepUseCallBack(unsigned int itemId,  unsigned int amountPerUse = 1);
	virtual void itemKeepUseEndCallBack(unsigned int itemId, unsigned int count);
	virtual bool isLvlLimit(unsigned int itemId);

	//tableview delegate
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};

	void upgradeResultCallBack(const Json::Value &requestData, const Json::Value &responseData);
	void animationPlayEnd(sp::Armature * target, const char* event,const char* data);
private:
	void binding();
	void updateInfo();
	void updateItems();

	void closeCallBack(cocos2d::CCObject* sender);
	void itemButtonCallBack(cocos2d::CCObject* sender);

	FormationDelegate *m_pFormationDelegate;
	EquipDelegate * m_pEquipDelegate;
	equip::EquipCurInfo *m_pEquipInfo;
	EquipModel * m_pModel;
	DTableView*      m_pTableView;
	cocos2d::CCArray*   m_pRefineItemArray;
	cocos2d::CCArray*   m_pItemInfoArray;
	cocos2d::extension::CCData  *m_pCCBData;
	bool bProgressUpdate;

private:
	cocos2d::CCNode			*m_pTableNode;
	DButton     *m_pCloseButton;
	cocos2d::CCNode         *m_pContainer;
	WarriorGradeBg       *m_pGrade;
	cocos2d::CCLabelTTF     *m_pName;
	cocos2d::CCLabelBMFont  *m_pLevel;
	cocos2d::CCNode	*m_pUpLevelNode;
	cocos2d::CCLabelBMFont	*m_pUpLevel;
	cocos2d::CCLabelBMFont	*m_pFight;

	//宝物用
	cocos2d::CCLabelBMFont  *m_pProperty;
	cocos2d::CCLabelBMFont  *m_pPropertyResult;
	cocos2d::CCLabelBMFont  *m_pResist;
	cocos2d::CCLabelBMFont  *m_pResistResult;
	EquipTypeMark			*m_pTypeLeft;
	EquipTypeMark			*m_pTypeRight;
	EquipTypeMark			*m_pTypeLeftDown;
	EquipTypeMark			*m_pTypeRightDown;
	DProgressBar*m_pProgressBar[WARRIOR_GRADE_NUM];

	
};


class EquipRefineItemCell : public CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	EquipRefineItemCell();
	~EquipRefineItemCell();
	static EquipRefineItemCell* create(CCData* ccbData);
	virtual bool init(cocos2d::extension::CCData * pData);
	virtual void onEnter();
	virtual void onExit();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void setInfo(equip::EquipRefineItemInfo*);
	unsigned int getItemId(){return m_itemId;};
	void updateView();

	CC_SYNTHESIZE(EquipRefineDelegate*, m_pDelegate, Delegate);

	//CCObject*, CCControlEvent
	void upgradeButtonCallBack(cocos2d::CCObject* pSender, cocos2d::extension::CCControlEvent controlEvent);
	void upgradeButtonLongPressCallBack(cocos2d::CCObject* pSender, cocos2d::extension::CCControlEvent controlEvent);
	void upgradeButtonLongPressEndCallBack(cocos2d::CCObject* pSender, cocos2d::extension::CCControlEvent controlEvent);
private:
	equip::EquipRefineItemInfo* m_pInfo;
	unsigned int            m_itemId;

	unsigned int			m_itemUseCount;//长按时记录用
	cocos2d::CCNode*        m_pCountNode;
	cocos2d::CCNode*        m_pIconContainer;
	cocos2d::CCLabelTTF*    m_pCountLabel;
	cocos2d::extension::CCControlButton* m_pTouchMenu;
	cocos2d::CCNode*        m_pMark;
	unsigned int m_nSoundID;
};


#endif