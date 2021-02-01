//
// SoulHunterView.h
// 狩魂的主界面
//
//  Created by liuxiaogang on 13-10-22.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SoulHunterView__
#define __QSMY__SoulHunterView__

#include "AppInclude.h"
#include "components/components.h"
#include "SoulHunterPageView.h"


#pragma mark -SoulHunterView-
class SoulHunterView
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE(SoulHunterDelegate*, m_pTransferDelegate, TransferDelegate);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pSettleContainer, SettleContainer);					//弟子位置容器
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pDotContainer,DotContainer);							//页点位置容器
public:
	SoulHunterView();
	~SoulHunterView();
	virtual bool init();
	CREATE_FUNC(SoulHunterView);
	void setCurrentPage(uint8_t pageIndex);
	
	virtual void onEnter();
	virtual void onExit();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);


	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
		return cellSizeForTable(table);
	};
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){};
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};
	void warriorGrouping();			//弟子6个一页分组
private:
    void updateView();
    void soulUpdate();
private:
	DPageDot						*m_pPageDot;
	DStepTableView					*m_pTableView;
	cocos2d::extension::CCData		*m_pPageData;
	cocos2d::CCArray				*m_pPage;
	cocos2d::CCArray				*m_pWarriorList;
	uint8_t                         m_pCurrentPage;
    cocos2d::CCPoint                m_Pos;
};

#endif /* defined(__QSMY__SoulHunterView__) */
