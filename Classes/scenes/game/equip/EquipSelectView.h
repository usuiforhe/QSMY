//
//  EquipList.h
//  装备选择列表
//  Created by cll on 13-5-7.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__EQUIPLIST__
#define __QSMY__EQUIPLIST__

#include "AppInclude.h"
#include "components/components.h"
#include "model/EquipModel.h"
#include "model/WarriorModel.h"
#include "EquipMultiSelectView.h"

class EquipDelegate;
class FormationDelegate;
class EquipView;
class FormationEquipSelectViewCell;

class EquipSelecteDelegate
{
public:
    virtual void selectedEquip(equip::EquipCurInfo*) = 0;
};

class FormationEquipListDelegate
{
public :
	virtual void setSelectedCell(FormationEquipSelectViewCell * pCell) = 0;
};


class EquipSelectView
:public DLayer
,public FormationEquipListDelegate
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{

    CC_SYNTHESIZE(EquipSelecteDelegate*, m_pEquipSelecteDelegate, EquipSelecteDelegate);	
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pEquipList, EquipList);
    
public:
	CC_SYNTHESIZE(uint32_t, m_pEquipLastProperty, EquipLastProperty);
	EquipSelectView();
	~EquipSelectView();
	
	static EquipSelectView* create();
	virtual bool init();

	virtual void onExit();
    /**
     *	@brief	设置要选择的装备列表
     *
     *	@param 	cocos2d::CCArray*pList 	装备列表
     *	@param 	wid  弟子编号 用于计算是否有缘
     */
    void setEquipList(cocos2d::CCArray*pList,uint32_t wid);

    virtual void setSelectedCell(FormationEquipSelectViewCell* pCell);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
	void menuCallback(cocos2d::CCObject *sender);
private:
    
	void updateEquipView();
	void updateEquipList();
    
private:
	void callbackReturn(cocos2d::CCObject *sender);

private:
    int32_t m_selectIndex;
    DTableView *m_pListView;
private:
    cocos2d::extension::CCData  *m_pData;
    cocos2d::CCArray            *m_pFates;
	cocos2d::CCNode        *m_pChangeEquipLabel;
	cocos2d::CCNode        *m_pChangeHeroLabel;
    cocos2d::CCNode        *m_pPageContainer;
	cocos2d::CCNode        *m_pPageContainerEnd;
};

class FormationEquipSelectViewCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(uint8_t, m_type, CellType);

public:

public:
	FormationEquipSelectViewCell();
	virtual ~FormationEquipSelectViewCell();

	static FormationEquipSelectViewCell* create(cocos2d::extension::CCData*);
	virtual bool init(cocos2d::extension::CCData*);

	void setEquipInfo(equip::EquipCurInfo *pEquipInfo);
	equip::EquipCurInfo* getEquipInfo() const;

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void setSelected(bool val);

	void setFate(bool val);

	void setRecommond(bool val);

	void setDelegate(EquipSelectView* pDelegate)
	{
		m_pEquipListTableDelegate = pDelegate;
	}

private:
	void updateCellView();

	void onSelectClick(cocos2d::CCObject* sender);
	void menuCallback(cocos2d::CCObject *sender);

private:
	equip::EquipCurInfo *m_pEquipInfo;      //装备信息
	EquipSelectView* m_pEquipListTableDelegate;

private:
	cocos2d::CCNode         *m_pContainer;

	cocos2d::CCSprite       *m_pGrade;
	cocos2d::CCLabelTTF     *m_pEquipName;
	cocos2d::CCLabelBMFont  *m_pLevel;
	cocos2d::CCNode			*m_pEquipUpLevel;

	cocos2d::CCLabelBMFont  *m_pProperty;//装备的属性值
	cocos2d::CCSprite       *m_pPropertyType[equip::kEquipPropTotal];

	cocos2d::CCNode         *m_pUnEquip;
	cocos2d::CCNode         *m_pEquiped;
	cocos2d::CCLabelTTF     *m_pEquipedOn;

	cocos2d::CCSprite       *m_pRecommondMark;
	cocos2d::CCSprite       *m_pFateMark;
	cocos2d::CCSprite       *m_pPropertyAddMark;
	cocos2d::CCSprite       *m_pPropertyReduceMark;

	cocos2d::CCSprite       *m_pGradeBg;

	cocos2d::CCNode			*m_pGradeGreen;
	cocos2d::CCNode			*m_pGradeBlue;
	cocos2d::CCNode			*m_pGradePurple;
	cocos2d::CCNode			*m_pGradeOrange;
	cocos2d::CCLabelBMFont  *m_pAddAmount;//可穿的装备相对现有装备的属性变动值
	cocos2d::CCLabelBMFont  *m_pReduceAmount;//可穿的装备相对现有装备的属性变动值
	cocos2d::CCLabelBMFont     *m_pPropNoChange;
	DButton *m_pAssignButton;//装备
};



#endif /* defined(__QSMY__EQUIPLIST__) */
