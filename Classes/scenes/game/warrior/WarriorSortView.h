//”¢–€≈≈–ÚΩÁ√Ê
#ifndef __WARRIOR_SORT_VIEW_H__
#define __WARRIOR_SORT_VIEW_H__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "WarriorListView.h"

class WarriorSortCell;
struct WarriorSortSelectDelegate
{
	virtual void setSelectCell(WarriorSortCell* pCell) = 0;
};
class WarriorSortView
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public WarriorSortSelectDelegate
{
public:
	WarriorSortView();
	virtual ~WarriorSortView();
	CREATE_FUNC(WarriorSortView);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;}
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;}

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){}
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};

	virtual void setSelectCell(WarriorSortCell* pCell);

public:
	void setDelegate(WarriorSortDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
protected:
	void onConfirmClick(cocos2d::CCObject * sender);
	void onBackClick(cocos2d::CCObject * sender);
private:
	WarriorSortDelegate   *m_pDelegate;
	DButton * m_pConfirmButton;
	DButton * m_pBackButton;
	cocos2d::CCNode* m_pContainer;

	cocos2d::extension::CCData *m_pData;
	warrior::WarriorSortType m_pCurType;
	WarriorSortCell*  m_pSortCell;
};

class WarriorSortCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
public:
	WarriorSortCell();
	~WarriorSortCell();
	static WarriorSortCell* create(cocos2d::extension::CCData*);
	virtual bool init(cocos2d::extension::CCData*);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
public:
	void setSelect(bool boo);
	void setType(warrior::WarriorSortType pType);

	void setDelegate(WarriorSortSelectDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
private:
	cocos2d::CCNode* m_pCheckBox;
	cocos2d::CCNode* m_pNodeBox[warrior::kSortTypeCount-1];

	WarriorSortSelectDelegate   *m_pDelegate;
protected:
	CC_SYNTHESIZE(warrior::WarriorSortType,m_pSortType,SortType);
	void onClick(cocos2d::CCObject *sender);
};
#endif  //__WARRIOR_SORT_VIEW_H__