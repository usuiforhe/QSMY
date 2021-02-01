//
//  EquipMultiSelectView.h
//  装备多选列表
//  Created by mzp on 14-04-12.
//  Copyright (c) 2013, thedream. All rights reserved.
//
#ifndef __QSMY_Equip_Multi_Select_View__
#define __QSMY_Equip_Multi_Select_View__


#include "AppInclude.h"
#include "components/components.h"
#include "../../../model/EquipModel.h"


class EquipMultiSelect;
class EquipSelectViewCell;
class EquipListTableDelegate
{
public :
	virtual void setSelectedCell(EquipSelectViewCell * pCell) = 0;
};

class EquipMultiSelectView
	: public DLayer
	, public EquipListTableDelegate
	, public cocos2d::extension::CCTableViewDataSource
	, public cocos2d::extension::CCTableViewDelegate
{
public:
	EquipMultiSelectView();
	~EquipMultiSelectView();

	static EquipMultiSelectView* create(equip::EquipGrade equipGrade, const cocos2d::CCSize& size);
	virtual bool init(equip::EquipGrade equipGrade, const cocos2d::CCSize& size);
	static EquipMultiSelectView* create(equip::EquipType equipType, const cocos2d::CCSize& size);
	virtual bool init(equip::EquipType equipType, const cocos2d::CCSize& size);
	virtual bool init(const cocos2d::CCSize& size);
	virtual void onEnter();
	virtual void onExit();

	virtual void setSelectedCell(EquipSelectViewCell* pCell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView* table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table,cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView* table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView* table);

	void setEquipGrade(equip::EquipGrade equipGrade);
	equip::EquipGrade getEquipGrade() const;
	void setEquipMultiSelect(EquipMultiSelect* pEquipMultiSelect);
	void shiftList(cocos2d::CCArray* pList);
	void updateEquipView(bool fly = true);

private:
	equip::EquipGrade m_equipGrade;		// 装备品质
	equip::EquipType	m_equipType;	// 装备类型
	cocos2d::CCArray*	m_pDataList;	// 数据

	DTableView* m_pListView;

	cocos2d::extension::CCData* m_pData;
	EquipMultiSelect* m_pEquipMultiSelect;
};




class EquipSelectViewCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(uint8_t, m_type, CellType);

public:

public:
	EquipSelectViewCell();
	virtual ~EquipSelectViewCell();

	static EquipSelectViewCell* create(cocos2d::extension::CCData*);
	virtual bool init(cocos2d::extension::CCData*);

	void setEquipInfo(equip::EquipCurInfo *pEquipInfo);
	//宝物炼化专用
	void setTreasureInfo(equip::EquipCurInfo *pEquipInfo);
	equip::EquipCurInfo* getEquipInfo() const;

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void setSelected(bool val);

	void setFate(bool val);

	void setDelegate(EquipListTableDelegate* pDelegate)
	{
		m_pEquipListTableDelegate = pDelegate;
	}

private:
	void updateCellView();

	void onSelectClick(cocos2d::CCObject* sender);

private:
	equip::EquipCurInfo *m_pEquipInfo;      //装备信息
	EquipListTableDelegate* m_pEquipListTableDelegate;

private:
	cocos2d::CCNode         *m_pContainer;
	cocos2d::CCNode			*m_pCoinSprite;
	cocos2d::CCSprite       *m_pGrade;
	cocos2d::CCLabelTTF     *m_pName;
	cocos2d::CCLabelBMFont  *m_pLevel;
	cocos2d::CCNode			*m_pEquipUpLevel;
	cocos2d::CCLabelBMFont  *m_pAmount;
	cocos2d::CCLabelBMFont  *m_pProperty;
	cocos2d::CCSprite       *m_pPropertyType[equip::kEquipPropTotal];

	cocos2d::CCMenuItem     *m_pSelectButton;

	cocos2d::CCNode         *m_pUnEquip;
	cocos2d::CCNode         *m_pEquiped;
	cocos2d::CCLabelTTF     *m_pEquipedOn;

	cocos2d::CCSprite       *m_pFateMark;

	cocos2d::CCSprite       *m_pGradeBg;

	cocos2d::CCNode			*m_pSellNode;
	cocos2d::CCNode			*m_pEnhanceNode;
	cocos2d::CCNode			*m_pGradeGreen;
	cocos2d::CCNode			*m_pGradeBlue;
	cocos2d::CCNode			*m_pGradePurple;
	cocos2d::CCNode			*m_pGradeOrange;
	cocos2d::CCLabelBMFont  *m_pZizhiNum;
	cocos2d::CCNode			*m_pFightNode;
	cocos2d::CCNode			*m_pBottom;
};

#endif	// __QSMY_Equip_Multi_Select_View__