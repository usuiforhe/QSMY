//
//  ActivenessTableView.h
//  活跃度预览界面
//
//  Created by cll on 13-6-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ActivenessTableView__
#define __QSMY__ActivenessTableView__

#include "AppInclude.h"
#include "components/components.h"
#include "model/ActivenessModel.h"

using namespace activeness;

#pragma mark - ActivenessCell -

class ActivenessCell:
public cocos2d::extension::CCTableViewCell,
public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    static ActivenessCell * create(cocos2d::extension::CCData * pData);
    
    ActivenessCell();
    ~ActivenessCell();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode);
    
    bool init(cocos2d::extension::CCData * pData);
    
    void updateView();
    
    void setActivenessInfo(ActivenessInfo * pInfo,UserActiveness *pUa);
    
private:
    void onGotoClick(cocos2d::CCObject * sender);
    
private:
    cocos2d::CCSprite * m_pCompletedLabel;
    cocos2d::extension::CCScale9Sprite * m_pBgInProgress;
    cocos2d::CCLabelBMFont * m_pPoint;
    cocos2d::CCLabelBMFont * m_pProgress;
    cocos2d::CCNode * m_pContainer;
    cocos2d::CCMenuItem * m_pButton;
    cocos2d::CCLabelTTF * m_pTitle;
    cocos2d::CCRichLabelTTF * m_pRichTitle;

    ActivenessInfo * m_pInfo;
    UserActiveness * m_pUa;
};

class ActivenessTableView:
public DLayer,
public cocos2d::extension::CCTableViewDataSource,
public cocos2d::extension::CCTableViewDelegate
{
public:
	ActivenessTableView();
    
	~ActivenessTableView();

	static ActivenessTableView * create(const cocos2d::CCSize & size);

	virtual bool init(const cocos2d::CCSize & size);

	virtual void onEnter();
	virtual void onExit();

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

private:
	void updateView();

private:
	DTableView *m_pListView;
    cocos2d::CCArray * m_pActivenessList;
    cocos2d::extension::CCData * m_pData;
};






#endif /* defined(__QSMY__ActivenessTableView__) */
