//
//  KingTombHistoryCell.h
//  皇陵探秘历史记录
//
//  Created by mzp on 14-8-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_KingTomb_History_Cell___
#define __QSMY_KingTomb_History_Cell___

#include "AppInclude.h"
#include "components/components.h"
#include "../../../model/KingTombModel.h"


class KingTombAssistBonusCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
public:
	KingTombAssistBonusCell();
	~KingTombAssistBonusCell();

	static KingTombAssistBonusCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void setAssistBonusInfo(kingTomb::AssistBonusInfo* pInfo);

private:
	void receiveCallBack(cocos2d::CCObject* sender);

private:
	kingTomb::AssistBonusInfo* m_pAssistBonusInfo;
	cocos2d::CCMenuItem* m_pBtnAdd;
	cocos2d::CCLabelTTF* m_pContent;
};



class KingTombLogCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
public:
	KingTombLogCell();
	~KingTombLogCell();

	static KingTombLogCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void setLogInfo(kingTomb::LogInfo* pInfo);

private:
	cocos2d::CCLabelTTF* m_pContent;
};


#endif	// __QSMY_KingTomb_History_Cell___
