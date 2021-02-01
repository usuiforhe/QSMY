//
//  EquipInfoView.h
//  装备展示
//  Created by cll on 13-4-15.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__EQUIPINFOVIEW__
#define __QSMY__EQUIPINFOVIEW__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/EquipModel.h"
#include "../../../model/WarriorModel.h"
#include "../common/gui/WarriorGradeBg.h"
#include "../pve/Pve.h"
#include "EquipEnhance.h"

class EquipDelegate;
class FormationDelegate;



class EquipInfoView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
    
    
private:
    
public:
	enum EquipInfoType
	{
		kTypeEnhanceOnly,
		kTypeEnhanceAndChange,
		kTypeViewOnly
	};
    
public:
	EquipInfoView();
	virtual ~EquipInfoView();
    
	static EquipInfoView *create(EquipInfoType type);
    CREATE_FUNC(EquipInfoView);
	virtual bool init(EquipInfoType type);
    virtual bool init();
    void loadLayer();
	virtual void onEnter();
    
	virtual void onExit();
    
	void setDelegate(EquipDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
    
	//设置阵容信息
	void setFormationInfo(FormationDelegate* pDelegate)
	{
		m_pFormationDelegate = pDelegate;
	}

	void setPveDelegate(PveDelegate* pDelegate)
	{
		m_pveDelegate = pDelegate;
	}
    
    void setEquip(equip::EquipCurInfo*);
    void setEquip(equip::EquipBaseInfo*);
    
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
    
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell){};
    
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
private:
    void binding();
	void updateEquipInfo();
    void updateStatus();
	void enhanceCallBack(cocos2d::CCObject * obj);
	void refineCallBack(cocos2d::CCObject * obj);
    void changeCallBack(cocos2d::CCObject * obj);
    void closeCallBack(cocos2d::CCObject * obj);
    
private:
	EquipDelegate           *m_pDelegate;
	FormationDelegate       *m_pFormationDelegate;
	PveDelegate             *m_pveDelegate;
	uint32_t                m_formationID;
    
    equip::EquipCurInfo*    m_pEquipCurInfo;		//装备信息
	EquipModel              *m_pEquipModel;
    WarriorModel            *m_pWarriorModel;
    
    EquipInfoType           m_type;
    
private:
    cocos2d::CCArray            *m_pFateList;   //缘分
    DTableView                  *m_pFateListView;//缘分
    cocos2d::extension::CCData  *m_pFateCellData;
	cocos2d::extension::CCData  *m_pButtonData;
    
private:
	cocos2d::CCNode         *m_pPageLayer;
	cocos2d::CCNode         *m_pContainer;
    
    WarriorGradeBg       *m_pGrade;
	cocos2d::CCLabelTTF     *m_pName;
    cocos2d::CCLabelBMFont  *m_pLevel;
    cocos2d::CCLabelTTF     *m_pDesc;
	cocos2d::CCNode			*m_pEquipUpLevel;
    
    cocos2d::CCLabelBMFont  *m_pProperty;
    cocos2d::CCSprite       *m_pPropertyType[equip::kEquipPropTotal];
	cocos2d::CCSprite       *m_pEquipMark[equip::kEquipPropTotal];
	//宝物用
	cocos2d::CCLabelBMFont  *m_pResist;
	EquipTypeMark			*m_pTypeLeft;
	cocos2d::CCLabelTTF     *m_pLevelDesc5;
	cocos2d::CCLabelTTF     *m_pLevelDesc10;
	cocos2d::CCNode         *m_pLevelActive5;
	cocos2d::CCNode         *m_pLevelActive10;
	cocos2d::CCNode			*m_pLevel5;
	cocos2d::CCNode			*m_pLevel10;
	cocos2d::CCNode			*m_pUpLevelNode;
	DProgressBar			*m_pProgressBar[WARRIOR_GRADE_NUM];

    cocos2d::CCNode         *m_pEnhanceOnly;
    cocos2d::CCNode         *m_pEnhanceAndChange;
    cocos2d::CCNode         *m_pCloseOnly;
    
    DButton                 *m_pEnhanceButton;
    DButton                 *m_pChangeButton;
    DButton                 *m_pCloseButton;
	DButton					*m_pEnhanceButton2;
    DButton					*m_pRefineButton;
	DButton					*m_pRefineButton2;

    cocos2d::CCNode         *m_pFateContainer;
    
	cocos2d::CCNode         *m_pNodeFate;
	cocos2d::CCNode         *m_pNodeSuit;
	cocos2d::extension::CCScrollView *m_pPage;
	cocos2d::CCLayer         *m_pNoTouchLayer;
	cocos2d::CCNode         *m_pPosition;
	cocos2d::CCLabelBMFont  *m_pZizhiNum;

	cocos2d::CCLabelBMFont  *m_pFight;
	cocos2d::CCNode         *m_pFateless;
};



#endif /* defined(__QSMY__EQUIPINFOVIEW__) */
