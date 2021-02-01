//
//  TreasureSelectView.h
//  宝物选择
//
//

#ifndef __QSMY__TreasureSelectView__
#define __QSMY__TreasureSelectView__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../equip/EquipMultiSelectView.h"
//#include "../../../model/WarriorModel.h"

struct TreasureSelectDelegate
{
	//virtual void setSelectedTreasure(equip::EquipCurInfo * pInfo){};
	virtual void setSelectedTreasureList(cocos2d::CCArray * pList){};
};

class TreasureSelectView
: public DLayer
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCTableViewDataSource
, public cocos2d::extension::CCTableViewDelegate
, public EquipListTableDelegate
{
public:
    TreasureSelectView();
    
    ~TreasureSelectView();
    
    static TreasureSelectView * create();
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){}
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};

	virtual void setSelectedCell(EquipSelectViewCell * pCell);
	inline void setDelegate(TreasureSelectDelegate * pDelegate)
	{
		m_pDelegate = pDelegate;
	}
	//当前要升级的宝物ID（避免被选择）
	CC_SYNTHESIZE(unsigned int,m_uCurrentID,CurrentID);

protected:
    void onConfirmClick(cocos2d::CCObject * sender);
    void onBackClick(cocos2d::CCObject * sender);
	//处理宝物列表，去除已经选中的
	void makeList();
    
protected:

    cocos2d::CCNode * m_pContainer;
    DButton * m_pConfirmButton;
    DButton * m_pBackButton;
	cocos2d::CCNode	*m_pTitleTreasure;
	cocos2d::CCNode	*m_pTitleHero;

	DTableView	*m_pSelectTable;
	cocos2d::extension::CCData* m_pData;

	EquipModel::EquipCurInfoVec  m_EquipInfoVec;
	EquipModel::EquipCurInfoVec  m_EquipInfoVecSelected;

	TreasureSelectDelegate *m_pDelegate;
};

#endif /* defined(__QSMY__WarriorListView__) */
