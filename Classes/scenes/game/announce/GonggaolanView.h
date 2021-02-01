//公告栏 游戏内
#ifndef __GONGGAOLAN_VIEW_H__
#define __GONGGAOLAN_VIEW_H__

#include "AppInclude.h"
#include "components/components.h"

namespace announce {
	class AnnounceInfo;
}
class GonggaolanCell;
class GonggaolanView
	:public DPopup
	,public cocos2d::extension::CCTableViewDelegate
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
public:
	GonggaolanView();
	virtual ~GonggaolanView();

	CREATE_FUNC(GonggaolanView);

	virtual bool init();


	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode);

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);

	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){ return NULL;};

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);

	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);

	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);

	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
private:
	void closeCallBack(cocos2d::CCObject *sender);
	void updateInfo(announce::AnnounceInfo * pAnnounce);
	void setSelectedCell(GonggaolanCell* pCell);
private:
	cocos2d::CCNode *m_pContainer;
	cocos2d::extension::CCData *m_pData;
	DTableView       *m_pTableView;
	CCRichLabelTTF   *m_pInfo;
	CCScrollView     *m_pScrollView;
	DButton          *m_pCloseButton;

	GonggaolanCell   *m_pCell;
	CCNode  *m_pArrow;

	announce::AnnounceInfo* m_pCurInfo;
};

class GonggaolanCell
	: public cocos2d::extension::CCTableViewCell
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCBSelectorResolver
{
public:
	GonggaolanCell();
	virtual ~GonggaolanCell();
	static GonggaolanCell* create(cocos2d::extension::CCData * pData);

	bool init(cocos2d::extension::CCData * pData);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode);

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);

	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){ return NULL;};

	void setAnnounce(announce::AnnounceInfo * pAnnounce);
public:
	void setSelected(bool isSelect);
private:
	cocos2d::CCMenuItemImage *m_pBtn;
	cocos2d::CCLabelTTF      *m_pTxt;
};
#endif  //__GONGGAOLAN_VIEW_H__