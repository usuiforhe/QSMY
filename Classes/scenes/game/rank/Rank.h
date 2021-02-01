#ifndef __PILI_RANK__
#define __PILI_RANK__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "model/RankModel.h"
#include "RankView.h"

class Rank
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public DTabViewDelegate
	,public RankDelegate
{
public:
	Rank();
	~Rank();

	static Rank* shareRank();

	virtual bool init();
	//DTabViewDelegate
	virtual void SelectTabAtIndex(DTabView *tabView, uint32_t idx);

	void updateView();
	virtual void onEnter();
	virtual void onExit();

	void closeView(cocos2d::CCObject*);

	//ccb method
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;};

	void menuCallback(cocos2d::CCObject *);
	void showOtherFormation(uint32_t uUserId);

private:
	CCNode      *m_pContainer;
	DTabView    *m_pDTabView;
	DButton     *m_pCloseButton;
	cocos2d::CCLabelBMFont  *m_pMyRankVal;
	cocos2d::CCLabelBMFont  *m_pMyLevel;
	cocos2d::CCNode        *m_pLabel[RANK_TYPE_COUNT];
	cocos2d::CCNode		*m_pTableNode;
	uint8_t m_uCurrIdx;
	RankModel  *m_pRankModel;
};


#endif