//
//  ChangeFormationSelectView.h
//  ”¢–€…œ’Û—°‘Ò
//  Created by zl on 14-7-3.
//  Copyright (c) 2014, thedream. All rights reserved.
//

#ifndef __QSMY__FORMATION_SELECT_VIEW__
#define __QSMY__FORMATION_SELECT_VIEW__

#include "AppInclude.h"
#include "components/components.h"
#include "model/WarriorModel.h"
#include "model/FormationModel.h"

class FormationDelegate;
class FormationSelectCell;

class FormationSelectView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
public:
	FormationSelectView();
	~FormationSelectView();
	
	static FormationSelectView* create(formation::PositionInfo* pPosInfo);
	virtual bool init(formation::PositionInfo* pPosInfo);
	virtual void onEnter();
	virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
	void returnButtonCallback(cocos2d::CCObject *sender);
	void setPosInfo(formation::PositionInfo*);

	CC_SYNTHESIZE(FormationDelegate*, m_pDelegate, Delegate);
private:
	void updateWarriorList();
	void onFormationJoined();
	void callbackReturn(cocos2d::CCObject *sender);

	WarriorModel::WarriorCurInfoVec * m_pWarriorList;
	WarriorModel                    *m_pWarriorModel;
	FormationModel                  *m_pFormationModel;
	DTableView                      *m_pTableView;
	cocos2d::extension::CCData      *m_pCellData;
	cocos2d::CCNode        *m_pChangeEquipLabel;
	cocos2d::CCNode        *m_pChangeHeroLabel;
    cocos2d::CCNode        *m_pPageContainer;
	cocos2d::CCNode        *m_pPageContainerEnd;
	cocos2d::CCNode        *m_pZhaomuBox;
	DButton                *m_pZhaomuBtn;
	formation::PositionInfo* m_pPosInfo;

private:
	void openMallLottery(CCObject*);
};


class WarriorTypeMark;
class FormationSelectCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(uint8_t, m_type, CellType);

public:

public:
	FormationSelectCell();
	virtual ~FormationSelectCell();

	static FormationSelectCell* create(cocos2d::extension::CCData*);
	virtual bool init(cocos2d::extension::CCData*);

	void setWarrior(warrior::WarriorCurInfo * pWarrior, bool _updateView = true);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void showWarriorInfo(cocos2d::CCObject* pTarget);

	CC_SYNTHESIZE(FormationDelegate*, m_pDelegate, Delegate);
	CC_SYNTHESIZE(formation::PositionInfo*, m_pPosInfo, PosInfo);
private:
	void binding();
	void updateCellView();
	void joinButtonCallback(cocos2d::CCObject *sender);

	warrior::WarriorCurInfo *m_pWarrior;     
	cocos2d::CCNode         *m_pContainer;

	cocos2d::CCLabelTTF     *m_pName;
	cocos2d::CCLabelBMFont  *m_pLevel;
	cocos2d::CCLabelBMFont* m_pCurrentFight;
	cocos2d::CCLabelBMFont* m_pAddFight;
    cocos2d::CCLabelBMFont* m_pSubFight;
	cocos2d::CCSprite*			m_pUpArrow;
	cocos2d::CCSprite*			m_pDownArrow;
	WarriorTypeMark*				m_pWarriorTypeMark;
	std::vector<cocos2d::CCNode*> m_gradeBgNodes;

	DButton *m_pJoinButton;//…œ’Û
};



#endif /* defined(__QSMY__CHANGE_FORMATION_SELECT_VIEW__) */
