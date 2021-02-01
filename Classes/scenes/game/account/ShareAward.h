//
//  ShareAward.h
//  分享
//
//  Created by mzp on 14-04-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#ifndef __QSMY_Share_Award__
#define __QSMY_Share_Award__


#include "AppInclude.h"
#include "components/components.h"


class ShareAward
	: public DPopup
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCTableViewDataSource
	, public cocos2d::extension::CCTableViewDelegate
{

public:
	ShareAward();
	virtual ~ShareAward();

	static ShareAward* create(const std::string& sShareMsg, const std::string& openUrl, cocos2d::CCArray* pSharePlugins);

	virtual bool init(const std::string& sShareMsg, const std::string& openUrl, cocos2d::CCArray* pSharePlugins);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table,cocos2d::extension::CCTableViewCell* cell);

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView* table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView* table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView* table);

private:
	void closeCallBack(cocos2d::CCObject* obj);

private:
	DTableView* m_pTableView;
	cocos2d::CCNode* m_pContainer;

private:
    std::string m_sShareMsg;
    std::string m_sOpenUrl;
    cocos2d::CCArray* m_pSharePlugins;
    
};



#endif