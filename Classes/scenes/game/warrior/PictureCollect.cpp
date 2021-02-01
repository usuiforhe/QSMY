#include "PictureCollect.h"
#include "../../../managers/GameManager.h"
#include "../../../managers/ResourceManager.h"
#include "../common/gui/WarriorTypeMark.h"
#include "../../../model/WarriorModel.h"
#include "AppInclude.h"
#include <algorithm>
#include "managers/AnalyticsManager.h"
#include "utils/StringUtil.h"
#include "Warrior.h"
#include "../../../model/Model.h"
#include "PictureCollectCell.h"
using namespace  warrior;

#define COLLECT_MARGIN 15
#define COLLECT_SIZE_WIDTH (640 + COLLECT_MARGIN*2)

static PictureCollect *s_SharedPictureCollect = NULL;
PictureCollect* PictureCollect::sharedPictureCollect(void)
{
	if (!s_SharedPictureCollect)
	{
		s_SharedPictureCollect = new PictureCollect();
		s_SharedPictureCollect->init(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(CCSizeMake(640,763), -PL_MAX_HEIGHT_DIFF));
	}
	return s_SharedPictureCollect;
}
PictureCollect::PictureCollect()
:m_pFilterLayer(NULL)
,m_pTableViewContainer(NULL)
,m_pBg(NULL)
,m_pHeroNum1(NULL)
,m_pHeroNum2(NULL)
,m_pHeroNum3(NULL)
,m_pFenYe(NULL)
,m_pAllPicture(NULL)
,m_pAdded(NULL)
,m_pJapan(NULL)
,m_pMagic(NULL)
,m_pShenjie(NULL)
,m_pFenleiButton(NULL)
,m_pRoot(NULL)
,m_nTotal(0)
,m_pFilterView(NULL)
,m_pWarriorDelegate_Collect(NULL)
,m_pWarriorList_t(NULL)
,m_pWarriorList_copy(NULL)
,m_pCellData(NULL)
,m_pWarriorInfoViews(NULL)
,m_pPageView(NULL)
,m_iCurrentPage(0)
,m_pHandBookType(warrior::kWarFilterNone)
,m_pArrowLeft(NULL)
,m_pArrowRight(NULL)
{
};
PictureCollect::~PictureCollect()
{
	CC_SAFE_RELEASE(m_pBg);
	CC_SAFE_RELEASE(m_pHeroNum1);
	CC_SAFE_RELEASE(m_pHeroNum2);
	CC_SAFE_RELEASE(m_pHeroNum3);
	CC_SAFE_RELEASE(m_pFenYe);
	CC_SAFE_RELEASE(m_pAllPicture);
	CC_SAFE_RELEASE(m_pAdded);
	CC_SAFE_RELEASE(m_pJapan);
	CC_SAFE_RELEASE(m_pMagic);
	CC_SAFE_RELEASE(m_pShenjie);
	CC_SAFE_RELEASE(m_pRoot);
	CC_SAFE_RELEASE(m_pFenleiButton);
	CC_SAFE_RELEASE(m_pWarriorList_t);
	CC_SAFE_RELEASE(m_pWarriorList_copy);
	CC_SAFE_RELEASE(m_pFilterView);
	CC_SAFE_RELEASE(m_pTableViewContainer);
	CC_SAFE_RELEASE(m_pFilterLayer);
	CC_SAFE_RELEASE(m_pCellData);
	CC_SAFE_RELEASE(m_pWarriorInfoViews);
	CC_SAFE_RELEASE(m_pArrowLeft);
	CC_SAFE_RELEASE(m_pArrowRight);
}
PictureCollect* PictureCollect::create(WarriorDelegate* pWarriorDelegate, const cocos2d::CCSize & size)
{
	PictureCollect* pRet = new PictureCollect();
	pRet->setDelegate_Collect(pWarriorDelegate);
	if(pRet && pRet->init(size))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		pRet = NULL;
	}
	return pRet;
}
void PictureCollect::onEnter()
{
	DLayer::onEnter();

	this->m_pHeroNum1->setString(CCString::createWithFormat("%d",m_nTotal)->getCString());
	this->m_pHeroNum3->setString(CCString::createWithFormat("%d",DM_GET_WARRIOR_MODEL->getNewCount())->getCString());
	this->m_pHeroNum2->setString(CCString::createWithFormat("%d",DM_GET_WARRIOR_MODEL->getWarriorVec()->count())->getCString());
	
	m_pFenleiButton->getButton()->setTarget(this, menu_selector(PictureCollect::selectCallBack));

	setWarriorListFilter(m_pHandBookType);
}
void PictureCollect::onExit()
{
	DLayer::onExit();
}
bool PictureCollect::init(cocos2d::CCSize size)
{
	if(!DLayer::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_hero/PL_hero_tujian");
	
	m_pCellData = readCCData("PL_ui/ccb/pl_hero/PL_hero_tujian_cell_cell");
	CC_SAFE_RETAIN(m_pCellData);

	CC_SAFE_RELEASE(m_pWarriorInfoViews);
	m_pWarriorInfoViews = CCArray::create();
	CC_SAFE_RETAIN(m_pWarriorInfoViews);
	m_pWarriorList_t = CCArray::create();
	m_pWarriorList_copy = CCArray::create();
	m_pWarriorList_t->retain();
	m_pWarriorList_copy->retain();

	m_nTotal = DM_GET_WARRIOR_MODEL->getWarriorHandBookMap()->count();
	DM_GET_WARRIOR_MODEL->refreshWarriorHandBookData();

	warrior::WarriorHandBookInfo* pWarrior = NULL;
	CCDictElement *pEle = NULL;
	CCDictionary* warriorDic = DM_GET_WARRIOR_MODEL->getWarriorHandBookMap();
	CCDICT_FOREACH(warriorDic,pEle)
	{
		pWarrior = static_cast<warrior::WarriorHandBookInfo*>(pEle->getObject());
		m_pWarriorList_t->addObject(pWarrior);
		m_pWarriorList_copy->addObject(pWarrior);
	}

	m_pPageView = DPageView::create(this,m_pTableViewContainer->getContentSize());
	m_pPageView->setContentOffset(CCPointZero);
	m_pPageView->setTouchEnabled(false);
	m_pPageView->setFade(false);
	m_pPageView->setDelegate(this);
	m_pTableViewContainer->addChild(m_pPageView);

	return true;
}

float PictureCollect::cellSizeForTable(DPageView *view)
{
	return COLLECT_SIZE_WIDTH+COLLECT_MARGIN;
}

cocos2d::CCNode* PictureCollect::cellAtIndex(DPageView *view, unsigned int idx)
{
	return (cocos2d::CCNode*)m_pWarriorInfoViews->objectAtIndex(idx);
}

uint32_t PictureCollect::numberOfCellsInTableView(DPageView *view)
{
	//return m_pWarriorInfoViews->count();
	return ceil(float(m_pWarriorList_t->count()/NUM_OF_ITEMS));
}

#pragma mark - - CCScrollViewDelegate
void PictureCollect::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
	int32_t i = m_pPageView->getCurrentPage();
	if(i==m_iCurrentPage) return;
	m_iCurrentPage = i;
	setArrow();
	this->m_pFenYe->setString(CCString::createWithFormat("%d/%d",m_pPageView->getCurrentPage()+1,this->numberOfCellsInTableView(m_pPageView))->getCString());
}

void PictureCollect::setArrow()
{
	int32_t i = m_pPageView->getCurrentPage();
	if(i == 0)
	{
		m_pArrowLeft->setVisible(false);
	}
	if(i+1 == this->numberOfCellsInTableView(m_pPageView))
	{
		m_pArrowRight->setVisible(false);
	}
	if(i>0 && i+1<this->numberOfCellsInTableView(m_pPageView))
	{
		m_pArrowRight->setVisible(true);
		m_pArrowLeft->setVisible(true);
	}
}

/**∆∑÷ ≈≈–Ú**/
static int sortWarriorByGrade(const CCObject* p1, const CCObject* p2)
{
	warrior::WarriorHandBookInfo * pWarrior1 = (warrior::WarriorHandBookInfo*)p1;
	warrior::WarriorHandBookInfo * pWarrior2 = (warrior::WarriorHandBookInfo*)p2;
	return static_cast<int>(pWarrior1->getColor_grade()) > static_cast<int>(pWarrior2->getColor_grade());
}

void PictureCollect::showWarriorView()
{
	//m_pWarriorInfoViews->removeAllObjects(); 
	std::sort(m_pWarriorList_t->data->arr, m_pWarriorList_t->data->arr + m_pWarriorList_t->data->num, sortWarriorByGrade);
	uint32_t count = ceil(float(m_pWarriorList_t->count()/NUM_OF_ITEMS));
	for(uint32_t i =0;i<count;i++)
	{
		PictureCollectCell_happy *m_pTableCell = NULL;
		if(i< m_pWarriorInfoViews->count())
		{
			m_pTableCell = static_cast<PictureCollectCell_happy*>(m_pWarriorInfoViews->objectAtIndex(i));
		}
		if(!m_pTableCell)
		{
			m_pTableCell = PictureCollectCell_happy::create(m_pCellData,i);
			m_pTableCell->setDelegate_happy(m_pWarriorDelegate_Collect);
			m_pWarriorInfoViews->addObject(m_pTableCell);
		}

		int k=0;
		warrior::WarriorHandBookInfo* m_pWarriorHandBookInfo = NULL;
		while(k< NUM_OF_ITEMS)
		{
			if(i*NUM_OF_ITEMS+k < m_pWarriorList_t->count())
			{
				m_pWarriorHandBookInfo =(warrior::WarriorHandBookInfo*)m_pWarriorList_t->objectAtIndex(i*NUM_OF_ITEMS+k);
				m_Vector.push_back(m_pWarriorHandBookInfo);
				PictureCollectCell *pCell = m_pTableCell->getCell(k);
				if(pCell)
				{
					pCell->setm_nID(m_pWarriorHandBookInfo->getWarrior_id());
					pCell->setRankNum(i*NUM_OF_ITEMS+k+1);
				}
			}
			k++;
		}
		m_pTableCell->fillAll(m_Vector);
		m_Vector.clear();
	}
	m_iCurrentPage = 0;
	m_pPageView->setCurrentPage(m_iCurrentPage);
	m_pPageView->reloadData();
	setArrow();
	this->m_pFenYe->setString(CCString::createWithFormat("%d/%d",m_pPageView->getCurrentPage()+1,this->numberOfCellsInTableView(m_pPageView))->getCString());
};

bool PictureCollect::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pBg",CCSprite *,m_pBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pRoot", CCNode *,m_pRoot);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pHeroNum1",CCLabelBMFont *,m_pHeroNum1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pHeroNum2",CCLabelBMFont *,m_pHeroNum2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pHeroNum3",CCLabelBMFont *,m_pHeroNum3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pFenYe",CCLabelBMFont *,m_pFenYe);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pAllPicture",CCSprite *,m_pAllPicture);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pJapan",CCSprite *,m_pJapan);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pShenjie",CCSprite *,m_pShenjie);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pAdded",CCSprite *,m_pAdded);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pMagic",CCSprite *,m_pMagic);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pFenleiButton",DButton *,m_pFenleiButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pTableViewContainer",CCNode *,m_pTableViewContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pFilterLayer",CCNode *,m_pFilterLayer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pArrowLeft",CCNode*,m_pArrowLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pArrowRight",CCNode*,m_pArrowRight);
	return false;
}
void PictureCollect::setDelegate_Collect(WarriorDelegate* m_pWarriorDelegate_Collect_t)
{
	m_pWarriorDelegate_Collect=m_pWarriorDelegate_Collect_t;
}
void PictureCollect::selectCallBack(cocos2d::CCObject* obj)
{
	showPopView();
}
void PictureCollect::showPopView()
{
	if(m_pFilterView ==NULL)
	{
		m_pFilterView = PictureFilter::create();
		m_pFilterView->setDelegate_t(this);
		CC_SAFE_RETAIN(m_pFilterView);
	}
	m_pFilterView->setOpacity(220);
	Game::sharedGame()->openPopup(m_pFilterView);
}
void PictureCollect::setWarriorListFilter(warrior::WarriorHandBookFilter type)
{
	if(type == warrior::kWarFilterNone)
	{
		type = warrior::kWarFilterGreen;
	}
	else if(m_pHandBookType == type)
	{
		return;
	}

	m_pHandBookType = type;
	m_pWarriorList_t->removeAllObjects();
	CCObject * obj(NULL);
	CCARRAY_FOREACH(m_pWarriorList_copy, obj)
	{
		warrior::WarriorHandBookInfo * pWarrior = (warrior::WarriorHandBookInfo*)obj;
		m_pWarriorList_t->addObject(pWarrior);
	}
	updateFilterType(type);
	switch (type)
	{
	case warrior::kWarFilterGreen :
		{
			//updateFilterType(warrior::kWarFilterGreen);
		}
		break;
	case warrior::kWarFilterBlue :
		{
			//updateFilterType(warrior::kWarFilterBlue);
			for(int i=0;i<m_pWarriorList_t->count();i++)
			{
				if(((warrior::WarriorHandBookInfo*)m_pWarriorList_t->objectAtIndex(i))->getNew()==0)
				{
					m_pWarriorList_t->removeObjectAtIndex(i);
					i--;
				}
			}
		}
		break;
	case warrior::kWarFilterPurple :
		{
			//updateFilterType(warrior::kWarFilterPurple);
			for(int i=0;i<m_pWarriorList_t->count();i++)
			{
				if(((warrior::WarriorHandBookInfo*)m_pWarriorList_t->objectAtIndex(i))->getArea()!=2)
				{
					m_pWarriorList_t->removeObjectAtIndex(i);
					i--;
				}
			}
		}
		break;
	case warrior::kWarFilterOrange :
		{
			//updateFilterType(warrior::kWarFilterOrange);
			for(int i=0;i<m_pWarriorList_t->count();i++)
			{
				if(((warrior::WarriorHandBookInfo*)m_pWarriorList_t->objectAtIndex(i))->getArea()!=3)
				{
					m_pWarriorList_t->removeObjectAtIndex(i);
					i--;
				}
			}
		}
		break;
	case warrior::kWarFilterRed :
		{
			//updateFilterType(warrior::kWarFilterRed);
			for(int i=0;i<m_pWarriorList_t->count();i++)
			{
				if(((warrior::WarriorHandBookInfo*)m_pWarriorList_t->objectAtIndex(i))->getArea()!=4)
				{
					m_pWarriorList_t->removeObjectAtIndex(i);
					i--;
				}
			}
		}
		break;
	}
	showWarriorView();
}

void  PictureCollect::updateFilterType(warrior::WarriorHandBookFilter m_nType)
{
	if(m_nType==warrior::kWarFilterGreen)
	{
		m_pAllPicture->setVisible(true);
		m_pAdded->setVisible(false);
		m_pJapan->setVisible(false);
		m_pMagic->setVisible(false);
		m_pShenjie->setVisible(false);
	}
	if(m_nType==warrior::kWarFilterBlue)
	{
		m_pAllPicture->setVisible(false);
		m_pAdded->setVisible(true);
		m_pJapan->setVisible(false);
		m_pMagic->setVisible(false);
		m_pShenjie->setVisible(false);
	}
	if(m_nType==warrior::kWarFilterPurple)
	{
		m_pAllPicture->setVisible(false);
		m_pAdded->setVisible(false);
		m_pJapan->setVisible(true);
		m_pMagic->setVisible(false);
		m_pShenjie->setVisible(false);
	}
	if(m_nType==warrior::kWarFilterOrange)
	{
		m_pAllPicture->setVisible(false);
		m_pAdded->setVisible(false);
		m_pJapan->setVisible(false);
		m_pMagic->setVisible(false);
		m_pShenjie->setVisible(true);
	}
	if(m_nType==warrior::kWarFilterRed)
	{
		m_pAllPicture->setVisible(false);
		m_pAdded->setVisible(false);
		m_pJapan->setVisible(false);
		m_pMagic->setVisible(true);
		m_pShenjie->setVisible(false);
	}
}