#ifndef __QSMY__PICTURECOLLECT__
#define __QSMY__PICTURECOLLECT__
#include "../common/common.h"
#include "../common/gui/WarriorGradeBg.h"
#include "../common/gui/WarriorTypeMark.h"
#include "PictureCollectCell.h"
#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/WarriorModel.h"
#include "PictureCollectCell_happy.h"
#include "PictureFilter.h"
#define RATIO 7.573
#define NUM_OF_ITEMS 6
#define HEIGHT_OF_PAGE 626
#define WARRIOR_HANDBOOK_CELL_SIZE CCSizeMake(640,626)
class WarriorListFilterDelegate
{
public:
	virtual void setWarriorListFilter(warrior::WarriorHandBookFilter type)=0;
};

class WarriorHandBookInfo;
class PictureCollectCell;
class WarriorDelegate;
class PictureFilter;

class PictureCollect
:public DLayer
,public DPageViewDataSource
,public cocos2d::extension::CCScrollViewDelegate
,public cocos2d::extension::CCBMemberVariableAssigner
,public WarriorListFilterDelegate
{
public:
	PictureCollect();
	virtual ~PictureCollect();
	static PictureCollect* create(WarriorDelegate* pWarriorDelegate, const cocos2d::CCSize & size);
	static PictureCollect* sharedPictureCollect(void);
	virtual bool init(cocos2d::CCSize size);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual void onEnter();
	virtual void onExit();

	virtual float cellSizeForTable(DPageView *view);
	virtual cocos2d::CCNode* cellAtIndex(DPageView *view, unsigned int idx);
	virtual uint32_t numberOfCellsInTableView(DPageView *view);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};
	//virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	//virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
	//virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	//virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	//virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){}
	//virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){}
	//virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){}
	void setDelegate_Collect(WarriorDelegate* m_pWarriorDelegate_Collect);
	virtual void setWarriorListFilter(warrior::WarriorHandBookFilter Grade);
	void updateFilterType(warrior::WarriorHandBookFilter);
	void updateHappyVector();
	std::vector<cocos2d::CCSprite*>              m_Vector_sprite;
protected:
	void selectCallBack(cocos2d::CCObject* obj);
	void showPopView();
	void setArrow();
private:
	void showWarriorView();
private:
	cocos2d::CCNode*        m_pFilterLayer;
	cocos2d::CCNode*        m_pTableViewContainer;
	cocos2d::CCSprite*      m_pBg;
	cocos2d::CCLabelBMFont* m_pHeroNum1;
	cocos2d::CCLabelBMFont* m_pHeroNum2;
	cocos2d::CCLabelBMFont* m_pHeroNum3;
	cocos2d::CCLabelBMFont* m_pFenYe;
	cocos2d::CCSprite*      m_pAllPicture;
	cocos2d::CCSprite*      m_pAdded;
	cocos2d::CCSprite*      m_pJapan;
	cocos2d::CCSprite*      m_pMagic;
	cocos2d::CCSprite*      m_pShenjie;
	DButton*                m_pFenleiButton;
	cocos2d::CCNode*        m_pRoot;
	int                     m_nTotal;
	PictureFilter*          m_pFilterView;
	WarriorDelegate*                             m_pWarriorDelegate_Collect;
	cocos2d::CCArray*                            m_pWarriorList_t;
    cocos2d::CCArray*                            m_pWarriorList_copy;
	std::vector<warrior::WarriorHandBookInfo*>   m_Vector;
	std::vector<PictureCollectCell_happy*>       m_Vector_happy;
	cocos2d::CCNode*                             m_pNode;
	cocos2d::extension::CCData* m_pCellData;

	cocos2d::CCArray            *m_pWarriorInfoViews;

	int8_t m_iCurrentPage;
	DPageView                   *m_pPageView;
	warrior::WarriorHandBookFilter m_pHandBookType;
	CCNode* m_pArrowLeft;
	CCNode* m_pArrowRight;
};
#endif
