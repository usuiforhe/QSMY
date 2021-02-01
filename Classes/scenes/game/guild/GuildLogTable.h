//
//  GuildLogTable.h
//  LibQSMY
//
//  Created by jk on 14-10-10.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __LibQSMY__GuildLogTable__
#define __LibQSMY__GuildLogTable__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class GuildLogTable
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
public:
	GuildLogTable();
	~GuildLogTable();
    
	static GuildLogTable* create(const cocos2d::CCSize tableSize);
	virtual bool init(const cocos2d::CCSize tableSize);
    
	virtual void onEnter();
	virtual void onExit();
    
#pragma mark CCTableViewDataSource
    
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    
#pragma mark CCTableViewDelegate
    
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
    
//#pragma mark setter
//	void setType(uint32_t iType);
    
private:
    cocos2d::extension::CCData* m_pData;

    
//	int32_t m_iSelectIndex;
//	uint32_t m_iType;
	GuildModel *m_pModel;
	DTableView *m_pListView;
    
	void updateView();
	void onTouch(cocos2d::CCObject*);
    
    
};


class GuildLogTableCell
: public cocos2d::extension::CCTableViewCell
, public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE(float, m_fHeight, Height);
    
public:
	GuildLogTableCell();
	virtual ~GuildLogTableCell();
    
	static GuildLogTableCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);
    
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
public:
    void setData(CCArray * data);
//	void setChatInfo(chat::ChatChannel channel, chat::ChatInfo* pInfo);
    
private:
	cocos2d::CCNode* m_pSelfBj;
	cocos2d::CCRichLabelTTF* m_pText;
    cocos2d::CCNode* m_pTextBg;
	cocos2d::CCLabelTTF* m_pGuildName;

};

#endif /* defined(__LibQSMY__GuildLogTable__) */
