//#include "WarriorListView.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
//#include "../common/gui/WarriorTypeMark.h"
//#include "Warrior.h"
//#include <algorithm>
#include "PictureCollectCell_happy.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define  WARRIOR_CELL_SIZE                      CCSizeMake(640, 130)    //单元大小
#define  WARRIOR_EXPLODE_CELL_SIZE              CCSizeMake(640, 210)    //展开单元大小
#define  WARRIOR_LIST_SELECT_SIZE               CCSizeMake(640, 535)    //弟子选择列表大小

#define D_EVENT_WARRIOR_SELECT_CHANGED     "warrior select changed"

#pragma mark ----------------PictureCollectCell_happy 弟子单元基类------------------
PictureCollectCell_happy * PictureCollectCell_happy::create(cocos2d::extension::CCData *pData,uint32_t pid)
{
	PictureCollectCell_happy * pRet = new PictureCollectCell_happy();
	if(pRet && pRet->init(pData,pid))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		//pRet = NULL;
	}
	return pRet;
}

PictureCollectCell_happy::PictureCollectCell_happy():
m_pWarriorDelegate_happy(NULL)
//,m_pHero1(NULL)
//,m_pHero2(NULL)
//,m_pHero3(NULL)
//,m_pHero4(NULL)
//,m_pHero5(NULL)
//,m_pHero6(NULL)
{
	m_Vector.insert(m_Vector.begin(), 6, (PictureCollectCell*)NULL);
}

PictureCollectCell_happy::~PictureCollectCell_happy()
{
	for (unsigned int i = 0; i < m_Vector.size(); i++)
	{
		CC_SAFE_RELEASE(m_Vector[i]);
	}
	//CC_SAFE_RELEASE(m_pHero1);
	//CC_SAFE_RELEASE(m_pHero2);
	//CC_SAFE_RELEASE(m_pHero3);
	//CC_SAFE_RELEASE(m_pHero4);
	//CC_SAFE_RELEASE(m_pHero5);
	//CC_SAFE_RELEASE(m_pHero6);
}

bool PictureCollectCell_happy::init(cocos2d::extension::CCData *pData,uint32_t pid)
{
	m_nPid = pid;
	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("PictureCollectCell", PictureCollectCellLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorGradeBg", WarriorGradeBgLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorTypeMark", WarriorTypeMarkLoader::loader());
	CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pNodeLoaderLibrary);

	//CCB_READER_DATA_AND_ADDCHILD(pData);
	//m_Vector.push_back(m_pHero1);
	//m_Vector.push_back(m_pHero2);
	//m_Vector.push_back(m_pHero3);
	//m_Vector.push_back(m_pHero4);
	//m_Vector.push_back(m_pHero5);
	//m_Vector.push_back(m_pHero6);
	return true;
}

bool PictureCollectCell_happy::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHero1",PictureCollectCell*, m_Vector[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHero2",PictureCollectCell*, m_Vector[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHero3",PictureCollectCell*, m_Vector[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHero4",PictureCollectCell*, m_Vector[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHero5",PictureCollectCell*, m_Vector[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHero6",PictureCollectCell*, m_Vector[5]);
	return true;
}

void PictureCollectCell_happy::fillAll(std::vector<warrior::WarriorHandBookInfo*> m_Vector_t)
{
	int count=0;
	while(count++<m_Vector_t.size())
	{
		if(NUM_OF_CELLS<count)
		return;
		if(m_Vector[count-1])
		{
			m_Vector[count-1]->setDelegate_t(m_pWarriorDelegate_happy);
			m_Vector[count-1]->setPictureCell(m_Vector_t.at(count-1));
		}
	}
	if(count<NUM_OF_CELLS)
	{
		while(count++<=NUM_OF_CELLS)
		{
			if(m_Vector[count-2])
			{
				m_Vector[count-2]->whetherShow(false);
			}
		}
	}
}
void PictureCollectCell_happy::setDelegate_happy(WarriorDelegate* m_pWarriorDelegate_happy_t)
{
	m_pWarriorDelegate_happy=m_pWarriorDelegate_happy_t;
}

SEL_MenuHandler PictureCollectCell_happy::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "chooseMenuCallBack", PictureCollectCell_happy::chooseMenuCallBack);
	return NULL;
}
void PictureCollectCell_happy::chooseMenuCallBack(cocos2d::CCObject* obj)
{

}

uint32_t PictureCollectCell_happy::getPid()
{
	return m_nPid;
}

PictureCollectCell* PictureCollectCell_happy::getCell(int id)
{
	return m_Vector[id];
}