//
//  EquipDeletePopup.h
//  装备展示
//  Created by fengkerong on 14-6-27.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__EQUIPDELETEPOPUP__
#define __QSMY__EQUIPDELETEPOPUP__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/EquipModel.h"
#include "../../../model/WarriorModel.h"
#include "EquipSelectView.h"
#include "Equip.h"
#include <map>
#include <vector>

class EquipDelegate;
class FormationDelegate;
class EquipGradeDelegate
{
public:
	virtual void setGradeByTag(int buttonTag , bool bSelect) = 0;
};
class EquipDeletePopup
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public EquipGradeDelegate
{

public:
	enum EquipInfoType
	{
		kTypeEnhanceOnly,
		kTypeEnhanceAndChange
	};

public:
	EquipDeletePopup();
	virtual ~EquipDeletePopup();

	static EquipDeletePopup *create(EquipInfoType type);
	CREATE_FUNC(EquipDeletePopup);
	virtual bool init(EquipInfoType type);
	virtual bool init();

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

	void setEquip(std::map<equip::EquipGrade, bool>&);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}


	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell){};

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

	virtual void setGradeByTag(int t , bool b);
	std::vector<equip::EquipGrade> m_pGradeList;
	std::map<equip::EquipGrade, bool> m_pGradeSelected;
private:
	void binding();
	void updateEquipInfo();
	void updateStatus();
	void closeCallBack(cocos2d::CCObject * obj);
	void confirmCallBack(cocos2d::CCObject * obj);

private:
	EquipDelegate           *m_pDelegate;
	FormationDelegate       *m_pFormationDelegate;
	uint32_t                m_formationID;

	equip::EquipCurInfo*    m_pEquipCurInfo;		//装备信息
	EquipModel              *m_pEquipModel;
	WarriorModel            *m_pWarriorModel;

	EquipInfoType           m_type;

private:
	cocos2d::CCArray            *m_pFateList;   //缘分
	DTableView                  *m_pFateListView;//缘分
	cocos2d::extension::CCData  *m_pFateCellData;

private:
	cocos2d::CCNode         *m_pContainer;

	cocos2d::CCSprite       *m_pGrade;
	cocos2d::CCLabelTTF     *m_pName;
	cocos2d::CCLabelBMFont  *m_pLevel;
	cocos2d::CCLabelTTF     *m_pDesc;
	cocos2d::CCNode			*m_pEquipUpLevel;

	cocos2d::CCLabelBMFont  *m_pProperty;

	cocos2d::CCNode         *m_pEnhanceOnly;
	cocos2d::CCNode         *m_pEhanceAndChange;
	cocos2d::CCNode         *m_pCloseOnly;

	DButton                 *m_pEnhanceButton0;
	DButton                 *m_pEnhanceButton;
	DButton                 *m_pRefineButton0;
	DButton                 *m_pRefineButton;
	DButton                 *m_pChangeButton;


	cocos2d::CCNode         *m_pFateContainer;

	DButton		*m_pCloseButton;
	DButton		*m_pConfirmButton;
};


class EquipGradeCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(uint8_t, m_type, CellType);

public:

public:
	EquipGradeCell();
	virtual ~EquipGradeCell();

	static EquipGradeCell* create(cocos2d::extension::CCData*);
	virtual bool init(cocos2d::extension::CCData*);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void setSelected(bool val);

	void setDelegate(EquipGradeDelegate* pDelegate)
	{
		m_pEquipListTableDelegate = pDelegate;
	}

	void setType(equip::EquipGrade);

private:
	void updateCellView();

	void onClickTest(cocos2d::CCObject* sender);

	void onCheckBoxClick(cocos2d::CCObject* sender);

private:
	equip::EquipCurInfo *m_pEquipInfo;      //装备信息
	EquipGradeDelegate* m_pEquipListTableDelegate;

private:
	cocos2d::CCNode         *m_pContainer;

	cocos2d::CCSprite       *m_pGrade;
	cocos2d::CCLabelTTF     *m_pName;
	cocos2d::CCLabelBMFont  *m_pLevel;
	cocos2d::CCNode			*m_pEquipUpLevel;

	cocos2d::CCLabelBMFont  *m_pProperty;

	cocos2d::CCMenuItem     *m_pSelectButton;

	cocos2d::CCNode         *m_pUnEquip;
	cocos2d::CCNode         *m_pEquiped;
	cocos2d::CCLabelTTF     *m_pEquipedOn;

	cocos2d::CCSprite       *m_pFateMark;

	cocos2d::CCSprite       *m_pGradeBg;

	cocos2d::CCNode			*m_pSellNode;
	cocos2d::CCNode			*m_pEnhanceNode;

	cocos2d::CCNode			*m_pGreenButton;
	cocos2d::CCNode			*m_pBlueButton;
	cocos2d::CCNode			*m_pPurpleButton;
	cocos2d::CCNode			*m_pOrangeButton;
	cocos2d::CCNode			*m_pTotalButton;

	cocos2d::CCMenuItem		*m_pCheckBox;
};


#endif /* defined(__QSMY__EQUIPDELETEPOPUP__) */
