//
//  kingTombGuildList.h
//  LibQSMY
//  选择公会成员列表
//  Created by jk on 14-8-7.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#ifndef __LibQSMY__KingTombGuildList__
#define __LibQSMY__KingTombGuildList__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "model/KingTombModel.h"
#include "managers/DataManager.h"
#include "KingTombGuildListCell.h"
#include "KingTombfight.h"

class KingTombGuildList
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public KingTombGuildListCellDelegate
{
    
    CC_SYNTHESIZE( uint32_t, m_pos, Pos ) ;
    CC_SYNTHESIZE( uint32_t, m_pIndex, Index ) ;
    CC_SYNTHESIZE(KingTombDelegate *, m_pDelegate, Delegate);

    
public:
    KingTombGuildList();
    ~KingTombGuildList();
    CREATE_FUNC( KingTombGuildList ) ;
    virtual bool init() ;
    virtual void onEnter();
	virtual void onExit() ;
    virtual void onBackKey(); //for android back key

    
    
public:
    
    virtual bool onAssignCCBMemberVariable( cocos2d::CCObject *pTarget,
                                           const char *pMemberVariableName, cocos2d::CCNode *pNode ) ;
    
#pragma mark CCTableViewDataSource
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    
#pragma mark CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
    
#pragma mark KingTombGuildListCellDelegate
    virtual void guildPlaySelete(guild::GuildUserBasics *pdata);

    
private:
    CCNode *m_pTableContainer;
    CCLabelTTF *m_pTitleTf;
	DButton	*m_pCloseButton;
    DTableView *m_pTableView;
	CCSize m_pCellSize;
    CCArray *m_pList;
    KingTombModel *m_pModel;
    
    void closeCallBack(cocos2d::CCObject *sender);
public:
    void refresh();

};


#endif /* defined(__LibQSMY__KingTombGuildList__) */
