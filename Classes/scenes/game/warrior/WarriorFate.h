//
//  WarriorFateView.h
//  缘列表
//
//  Created by cll on 13-7-24.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__WarriorFateView__
#define __QSMY__WarriorFateView__

#include "AppInclude.h"
#include "components/components.h"
#include "model/WarriorModel.h"


class WarriorFateCell;


class WarriorFateView :public DLayer,  
	public cocos2d::extension::CCTableViewDataSource,
	public cocos2d::extension::CCTableViewDelegate
{

public:
	WarriorFateView();

	~WarriorFateView();
    
	CREATE_FUNC(WarriorFateView);

    virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
	void setFateArray(cocos2d::CCArray *pFateArray,warrior::WarriorCurInfo* pWarrior);
    void setFateArray(cocos2d::CCArray *pFateArray,warrior::WarriorBaseInfo* pWarrior);
private:

	void updateView();

private:
    cocos2d::extension::CCData  *m_pCCData;	
    DTableView *m_pListView;
	cocos2d::CCArray *m_pFateArray;
    warrior::WarriorCurInfo* m_pWarrior;
};




class WarriorFateCell : public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    
	WarriorFateCell();
	~WarriorFateCell();
    
	static WarriorFateCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);

	void setFateInfo(warrior::FateInfo *pFateInfo,warrior::WarriorCurInfo* pWarrior);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

private:
    void updateCellView();


public:
		
	cocos2d::extension::CCScale9Sprite	*m_pStateAct;			//激活状态
	cocos2d::extension::CCScale9Sprite	*m_pStateNotAct;		//没有激活
	cocos2d::CCLabelTTF					*m_pFateName;			//缘名
	cocos2d::CCNode						*m_pPropWord;			//攻防血
	cocos2d::CCLabelBMFont				*m_pPropAdd;			//属性增加
	cocos2d::CCLabelTTF					*m_pFateDesc;			//缘描述


private:				
	warrior::FateInfo *m_pFateInfo;
    warrior::WarriorCurInfo* m_pWarrior;
};



#endif /* defined(__QSMY__WarriorFateView__) */
