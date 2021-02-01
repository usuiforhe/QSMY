#ifndef __PILI_RANKVIEW__
#define __PILI_RANKVIEW__
#include "AppInclude.h"
#include "components/components.h"
#include "model/RankModel.h"
#include "../Game.h"


#define RANK_LEVEL_HIGHT_LIGHT_NUM 3

class RankDelegate
{
public:
	virtual void showOtherFormation(uint32_t uUserId)=0;
};

class RankView
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	
{
	
public:
	RankView();
	~RankView();
	static RankView* create(rank::RankType rankType, const cocos2d::CCSize& size);
	virtual bool init(rank::RankType rankType, const cocos2d::CCSize& size);

	//table view
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	

	void updateRankList();
	virtual void onEnter();
	virtual void onExit();
	

private:
	DTableView* m_pRankTable;
	cocos2d::extension::CCData* m_pCCBData;
	cocos2d::CCArray* m_pRankList;
	rank::RankType m_eRankType;
	RankModel  *m_pRankModel;
};

class RankCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	
public:
	RankCell();
	~RankCell();

	static RankCell* create(cocos2d::extension::CCData* ccbData);
	virtual bool init(cocos2d::extension::CCData* ccbData);

	//ccb method
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;};

	void setRankInfo(rank::UserRankInfo* pRankInfo);
	void menuCallback(cocos2d::CCObject *obj);
	
private:
	cocos2d::CCNode *m_pLevel[RANK_LEVEL_HIGHT_LIGHT_NUM];
	cocos2d::CCNode *m_pNormal;
	cocos2d::CCLabelTTF *m_pName;
	cocos2d::CCLabelBMFont *m_pLevelVal;
	cocos2d::CCLabelBMFont *m_pRankVal;
	cocos2d::CCLabelBMFont *m_pVipLevel;
	cocos2d::CCNode        *m_pTypeLabel[RANK_TYPE_COUNT];
	rank::UserRankInfo* m_pRankInfo;
};


#endif