//
//  ServerListView.h
//  QSMY
//
//  Created by wanghejun on 13-9-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ServerListView__
#define __QSMY__ServerListView__

#include "AppInclude.h"
#include "components/components.h"
#include "ServerCell.h"
#include "ServerStatus.h"

class LoginDelegate;
class ServerCell;

class ServerListView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public cocos2d::extension::CCBSelectorResolver
{
    CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);
public:
    ServerListView();
    virtual ~ServerListView();
    CREATE_FUNC(ServerListView);
    
    void onEnter();
    void onExit();
    
private:
    
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

    
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    void selectCallBack(cocos2d::CCObject*);
    void closeCallBack(cocos2d::CCObject*);
    
    void updateView();
private:
    DTableView      *m_pServerList;
    DTableView      *m_pMyServerList;
    
private:
    cocos2d::CCNode *m_pContainer;
    cocos2d::CCNode *m_pContainer2;
    cocos2d::extension::CCData  *m_pCellData;
};

#define SERVER_CELL_COUNT 2
class ServerListCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{
    ServerCell* m_pServerCell[SERVER_CELL_COUNT];
public:
    ServerListCell();
    ~ServerListCell();
    static ServerListCell* create(cocos2d::extension::CCData* pData);
    bool init(cocos2d::extension::CCData* pData);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    ServerCell* getServerCell(uint8_t i);
};

#endif /* defined(__QSMY__ServerListView__) */
