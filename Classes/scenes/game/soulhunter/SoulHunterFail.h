//
// SoulHunterFail.h
// 狩魂失败界面 战胜未抢到魂魄
//
//  Created by liuxiaogang on 13-10-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SoulHunterFail__
#define __QSMY__SoulHunterFail__

#include "AppInclude.h"
#include "components/components.h"
#include "model/SoulHunterModel.h"
#include "managers/DataManager.h"
#include "../common/AwardListView.h"

class SoulHunterFail
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	SoulHunterFail();
	~SoulHunterFail();
	virtual bool init();
	static SoulHunterFail*  create();

	virtual void onEnter();
	virtual void onExit();
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell) {};
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);   
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void updateView();
	void againCallBack(cocos2d::CCObject* sender);
	void closeCallBack(cocos2d::CCObject* sender);
private:
	cocos2d::CCNode				*m_pAwardContainer;
	DButton						*m_pBackButton;
	DButton						*m_pAgainButton;
	DTableView					*m_pListView;
	cocos2d::CCArray			*m_pAwardList;
	cocos2d::extension::CCData  *m_pCCData;
};

#endif /* defined(__QSMY__SoulHunterFail__) */