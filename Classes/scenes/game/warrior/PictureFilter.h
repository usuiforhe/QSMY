//TU JIAN FENLEI XIANSHI	
#ifndef __PICTUREFILTER_H__
#define __PICTUREFILTER_H__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/WarriorModel.h"
#include "PictureCollect.h"
class PictureFilterCell;
class WarriorListFilterDelegate;
struct PictureFilterDelegate
{
	virtual void setSelectCell(PictureFilterCell* pCell) = 0;
};
class PictureFilter
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public PictureFilterDelegate
{
public:
	PictureFilter();
	virtual ~PictureFilter();
	CREATE_FUNC(PictureFilter);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;}
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;}
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){};
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};
	virtual void setSelectCell(PictureFilterCell* pCell);
	void setDelegate_t(WarriorListFilterDelegate* pDelegate)
	{
		m_pDelegate_t = pDelegate;
	}
protected:
	void onConfirmClick(cocos2d::CCObject * sender);
	void onBackClick(cocos2d::CCObject * sender);
private:
	WarriorListFilterDelegate  *m_pDelegate_t;
	DButton * m_pConfirmButton;
	DButton * m_pBackButton;
	cocos2d::CCNode* m_pContainer;
	cocos2d::extension::CCData *m_pData;
	warrior::WarriorHandBookFilter m_pCurGrade;
	PictureFilterCell*  m_pSortCell;
};
class PictureFilterCell:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
public:
	PictureFilterCell();
	~PictureFilterCell();
	static PictureFilterCell* create(cocos2d::extension::CCData*);
	virtual bool init(cocos2d::extension::CCData*);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
public:
	void setSelect(bool boo);
	void setType(warrior::WarriorHandBookFilter Grade);
	void setDelegate(PictureFilterDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
private:
	cocos2d::CCNode* m_pCheckBox;
	cocos2d::CCNode* m_pNodeBox[warrior::kWarFilterCount-1];
	PictureFilterDelegate* m_pDelegate;
protected:
	CC_SYNTHESIZE(warrior::WarriorHandBookFilter,m_pFilterGrade,FilterGrade);
	void onClick(cocos2d::CCObject *sender);
};
#endif
