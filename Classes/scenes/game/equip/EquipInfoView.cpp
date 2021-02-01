//  EquipInfoView.cpp
//  装备描述
//  Created by cll on 13-4-15
//  Copyright (c) 2013, thedream. All rights reserved.
//

#include "EquipInfoView.h"
#include "Equip.h"
#include "managers/ResourceManager.h"
#include "../formation/Formation.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"
#include "../common/common.h"

USING_NS_CC;
USING_NS_CC_EXT;


EquipInfoView::EquipInfoView()
:m_pDelegate(NULL)
,m_pveDelegate(NULL)
,m_pFormationDelegate(NULL)
,m_pEquipCurInfo(NULL)
,m_formationID(0)
,m_pEquipModel(NULL)
,m_pContainer(NULL)
,m_pGrade(NULL)
,m_pName(NULL)
,m_pLevel(NULL)
,m_pDesc(NULL)
,m_pEquipUpLevel(NULL)
,m_pProperty(NULL)
,m_pResist(NULL)
,m_pEnhanceOnly(NULL)
,m_pEnhanceAndChange(NULL)
,m_pEnhanceButton(NULL)
,m_pChangeButton(NULL)
,m_pFateContainer(NULL)
,m_pFateList(NULL)
,m_pFateListView(NULL)
,m_pWarriorModel(NULL)
,m_pFateCellData(NULL)
,m_pCloseOnly(NULL)
,m_pCloseButton(NULL)
,m_pNodeFate(NULL)
,m_pFateless(NULL)
,m_pNodeSuit(NULL)
,m_pPage(NULL)
,m_pNoTouchLayer(NULL)
,m_pEnhanceButton2(NULL)
,m_pRefineButton(NULL)
,m_pRefineButton2(NULL)
,m_pFight(NULL)
,m_pButtonData(NULL)
,m_pPosition(NULL)
,m_pZizhiNum(NULL)
,m_pPageLayer(NULL)
,m_pTypeLeft(NULL)
,m_pLevelDesc5(NULL)
,m_pLevelActive5(NULL)
,m_pLevelDesc10(NULL)
,m_pLevelActive10(NULL)
,m_pLevel5(NULL)
,m_pLevel10(NULL)
,m_pUpLevelNode(NULL)
{
	D_RESET_C_ARRAY(m_pPropertyType, equip::kEquipPropTotal);
	D_RESET_C_ARRAY(m_pEquipMark, equip::kEquipPropTotal);
	D_RESET_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}

EquipInfoView::~EquipInfoView()
{
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipCurInfo, this);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pGrade);
    
	CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pLevel);
    CC_SAFE_RELEASE(m_pDesc);
	CC_SAFE_RELEASE(m_pEquipUpLevel);
    CC_SAFE_RELEASE(m_pProperty);
    CC_SAFE_RELEASE(m_pResist);
    CC_SAFE_RELEASE(m_pEnhanceOnly);
    CC_SAFE_RELEASE(m_pEnhanceAndChange);

    CC_SAFE_RELEASE(m_pEnhanceButton);
    CC_SAFE_RELEASE(m_pChangeButton);
    CC_SAFE_RELEASE(m_pFateContainer);
    
    CC_SAFE_RELEASE(m_pCloseButton);
    CC_SAFE_RELEASE(m_pCloseOnly);

	CC_SAFE_RELEASE(m_pNodeFate);
	CC_SAFE_RELEASE(m_pFateless);
	CC_SAFE_RELEASE(m_pNodeSuit);
	CC_SAFE_RELEASE(m_pPage);
	CC_SAFE_RELEASE(m_pNoTouchLayer);

	CC_SAFE_RELEASE(m_pEnhanceButton2);
	CC_SAFE_RELEASE(m_pRefineButton);
	CC_SAFE_RELEASE(m_pRefineButton2);
	CC_SAFE_RELEASE(m_pFight);
	CC_SAFE_RELEASE(m_pPosition);
	CC_SAFE_RELEASE(m_pZizhiNum);
    
	D_SAFE_RELEASE_C_ARRAY(m_pPropertyType, equip::kEquipPropTotal);
	D_SAFE_RELEASE_C_ARRAY(m_pEquipMark, equip::kEquipPropTotal);
	D_SAFE_RELEASE_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
    
    CC_SAFE_RELEASE(m_pFateListView);
    CC_SAFE_RELEASE(m_pFateList);
    CC_SAFE_RELEASE(m_pFateCellData);
	CC_SAFE_RELEASE(m_pButtonData);
	CC_SAFE_RELEASE(m_pTypeLeft);
	CC_SAFE_RELEASE(m_pLevelDesc5);
	CC_SAFE_RELEASE(m_pLevelDesc10);
	CC_SAFE_RELEASE(m_pLevelActive5);
	CC_SAFE_RELEASE(m_pLevelActive10);
	CC_SAFE_RELEASE(m_pLevel5);
	CC_SAFE_RELEASE(m_pLevel10);
	CC_SAFE_RELEASE(m_pUpLevelNode);
}

bool EquipInfoView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnhanceOnly", CCNode *, this->m_pEnhanceOnly);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseOnly", CCNode *, this->m_pCloseOnly);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnhanceAndChange", CCNode *, this->m_pEnhanceAndChange);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateContainer", CCNode *, this->m_pFateContainer);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", WarriorGradeBg *, this->m_pGrade);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF *, this->m_pDesc);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipUpLevel", CCNode *, this->m_pEquipUpLevel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty", CCLabelBMFont *, this->m_pProperty);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pResist", CCLabelBMFont *, this->m_pResist);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnhanceButton", DButton *, this->m_pEnhanceButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChangeButton", DButton *, this->m_pChangeButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
    
    D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pEquipType",CCSprite*,this->m_pPropertyType,equip::kEquipPropTotal);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pEquipMark",CCSprite*,this->m_pEquipMark,equip::kEquipPropTotal);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFate", CCNode *, this->m_pNodeFate);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateless", CCNode *, this->m_pFateless);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeSuit", CCNode *, this->m_pNodeSuit);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPage", CCScrollView *,this->m_pPage);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoTouchLayer", CCLayer *,this->m_pNoTouchLayer);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRefineButton", DButton *, this->m_pRefineButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRefineButton2", DButton *, this->m_pRefineButton2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnhanceButton2", DButton *, this->m_pEnhanceButton2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCLabelBMFont *, this->m_pFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPosition", CCNode *, this->m_pPosition);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZizhiNum" , CCLabelBMFont* ,this->m_pZizhiNum);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTypeLeft", EquipTypeMark *, this->m_pTypeLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelDesc5", CCLabelTTF *, this->m_pLevelDesc5);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelDesc10", CCLabelTTF *, this->m_pLevelDesc10);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelActive5", CCNode *, this->m_pLevelActive5);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelActive10", CCNode *, this->m_pLevelActive10);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel5", CCNode *, this->m_pLevel5);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel10", CCNode *, this->m_pLevel10);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevelNode", CCNode *, this->m_pUpLevelNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar1", DProgressBar *, this->m_pProgressBar[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar2", DProgressBar *, this->m_pProgressBar[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar", DProgressBar *, this->m_pProgressBar[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar4", DProgressBar *, this->m_pProgressBar[3]);
    return false;
}

SEL_MenuHandler EquipInfoView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", EquipInfoView::closeCallBack);
    return NULL;
}

EquipInfoView * EquipInfoView::create(EquipInfoType type)
{
	EquipInfoView * pEquipInfo = new EquipInfoView();
    
	if(pEquipInfo && pEquipInfo->init(type))
	{
		pEquipInfo->autorelease();
		return pEquipInfo;
	}
	else
	{
		CC_SAFE_DELETE(pEquipInfo);
		return NULL;
	}
    
}

bool EquipInfoView::init()
{
    return init(kTypeViewOnly);
}


bool EquipInfoView::init(EquipInfoType type)
{
	if(!DLayer::init()) return false;
    
	m_type = type;
	m_pEquipModel = DM_GET_EQUIP_MODEL;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    
	m_pFateList = CCArray::create();
	CC_SAFE_RETAIN(m_pFateList);
	
	m_pFateCellData = readCCData("PL_ui/ccb/pl_common/WarriorHeadSmallName");
	CC_SAFE_RETAIN(m_pFateCellData);

	m_pButtonData = readCCData("PL_ui/ccb/pl_zhuangbei/PL_zhuangbei_button");
	CC_SAFE_RETAIN(m_pButtonData);

	return true;
}

void EquipInfoView::loadLayer()
{
	//必然要有装备的信息
	if(m_pEquipCurInfo == NULL)return;

	removeAllChildren();
	//CCB_READER_AND_ADDCHILD("ui/ccb/equip/equipInfo");
	CCNode* pNode = NULL;
	if(m_pEquipCurInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure)
	{
		CCNodeLoaderLibrary * pNodeLoaderLibarary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		CUSTOM_CCB_LOADER(pNodeLoaderLibarary);
		pNodeLoaderLibarary->registerCCNodeLoader("EquipTypeMark", EquipTypeMarkLoader::loader());
		CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_baowutunshi/PL_baowuxinxi", pNodeLoaderLibarary);

		CCSize pagesize = m_pPage->getViewSize();
		pagesize.height -= PL_GET_HEIGHT_DIFF;
		m_pPage->setViewSize(pagesize);
		m_pPage->setContentOffset(m_pPage->minContainerOffset());
	}
	else
	{
		CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zhuangbei/PL_zhuangbei");
		m_pPage->setScrollEnabled(false);
		m_pRefineButton->getButton()->setTarget(this, menu_selector(EquipInfoView::refineCallBack));
		m_pRefineButton2->getButton()->setTarget(this, menu_selector(EquipInfoView::refineCallBack));
	}	
	//m_pPageLayer->addChild(pNode);

	m_pEnhanceButton->getButton()->setTarget(this, menu_selector(EquipInfoView::enhanceCallBack));
	m_pEnhanceButton2->getButton()->setTarget(this, menu_selector(EquipInfoView::enhanceCallBack));
	m_pChangeButton->getButton()->setTarget(this, menu_selector(EquipInfoView::changeCallBack));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(EquipInfoView::closeCallBack));
	//m_pDesc->setVerticalTextStyle(kCCVerticalTextStyleRightToLeft);

	//仅当套装可见的时候，才可以滑动
	//set default
	//m_pNoTouchLayer->setVisible(true);
	//m_pNodeFate->setVisible(false);
	//m_pNodeSuit->setVisible(false);

	

	m_pFateListView = DTableView::create(this, m_pFateContainer->getContentSize());
	m_pFateListView->setDirection(kCCScrollViewDirectionHorizontal);
	m_pFateListView->setDelegate(this);
	m_pFateContainer->addChild(m_pFateListView);
	CC_SAFE_RETAIN(m_pFateListView);

	
}

void EquipInfoView::onEnter()
{
	binding();
	DLayer::onEnter();
}

void EquipInfoView::setEquip(equip::EquipCurInfo* pEquipCurInfo)
{
    if(m_pEquipCurInfo==pEquipCurInfo) return;
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipCurInfo, this);
    CC_SAFE_RETAIN(pEquipCurInfo);
    m_pEquipCurInfo = pEquipCurInfo;

	loadLayer();

    updateStatus();
    binding();
}

void EquipInfoView::setEquip(equip::EquipBaseInfo* pInfo)
{
    equip::EquipCurInfo* pEquip = equip::EquipCurInfo::create(pInfo);
    setEquip(pEquip);
}

void EquipInfoView::updateStatus()
{
 //   if(!m_pEquipModel->isMyEquip(m_pEquipCurInfo))
 //   {
 //       m_pCloseOnly->setVisible(true);
 //       m_pCloseButton->getButton()->setTarget(this, menu_selector(EquipInfoView::closeCallBack));
 //       m_pEnhanceOnly->setVisible(false);
 //       m_pEnhanceAndChange->setVisible(false);
 //       return;
 //   }
 //   m_pCloseOnly->setVisible(false);
	m_pWarriorModel->getWarriorFatesByEquipId(m_pEquipCurInfo->getBaseInfo()->getEquipId(),m_pFateList);

	//显示暂无缘分
	m_pFateless->setVisible(m_pFateList->count() == 0);


	
	// 典籍类型
	bool bDianji = m_pEquipCurInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure;

    switch (m_type) {
        case kTypeEnhanceAndChange:
            m_pEnhanceOnly->setVisible(false);
            m_pEnhanceAndChange->setVisible(true);

            break;
        case kTypeEnhanceOnly:
            m_pEnhanceOnly->setVisible(true);
            m_pEnhanceAndChange->setVisible(false);

			break;
		case kTypeViewOnly:
			m_pEnhanceOnly->setVisible(false);
			m_pEnhanceAndChange->setVisible(false);
			break;
        default:
            break;
    }
}

void EquipInfoView::binding()
{
	BINDING_EXEC(m_pEquipCurInfo,this,EquipInfoView::updateEquipInfo,D_EVENT_EQUIPINFO_UPDATE);
}

void EquipInfoView::onExit()
{
	//Game::sharedGame()->setNavigatorVisable(true);
    UNBINDING_ALL(m_pEquipCurInfo,this);
	DLayer::onExit();
}


void EquipInfoView::updateEquipInfo()
{
	if (!m_pEquipCurInfo || !m_pEquipCurInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("EquipInfoError").c_str());
		return ;
	}
    
    m_pWarriorModel->getWarriorFatesByEquipId(m_pEquipCurInfo->getBaseInfo()->getEquipId(),m_pFateList);
    m_pFateListView->reloadData();
    
    
    m_pContainer->removeAllChildren();
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon(m_pEquipCurInfo->getEquipId()));
	
    m_pName->setString(m_pEquipCurInfo->getBaseInfo()->getEquipName().c_str());
    
    m_pDesc->setString(m_pEquipCurInfo->getBaseInfo()->getEquipDesc().c_str());
    
    m_pLevel->setString(CCString::createWithFormat("%i",m_pEquipCurInfo->getLevel())->getCString());

	if(m_pZizhiNum)
	{
		m_pZizhiNum->setString(CCString::createWithFormat("%d",m_pEquipCurInfo->getBaseInfo()->getQuality())->getCString());
	}
    
    //m_pGrade->setColor(m_pEquipModel->getEquipColorByGrade(m_pEquipCurInfo->getBaseInfo()->getEquipGrade()));
    int iGrade = (int)m_pEquipCurInfo->getBaseInfo()->getEquipGrade();
	warrior::WarriorGrade wGrade = (warrior::WarriorGrade)iGrade;
	m_pGrade->setWarriorGrade(wGrade);

    //////m_pEnhanceButton->getButton()->setEnabled(m_pEquipCurInfo->isCanEnhance());
    //////m_pEnhanceButton0->getButton()->setEnabled(m_pEquipCurInfo->isCanEnhance());
    
	equip::EquipPropType type = equip::kEquipPropAttack;
	uint32_t prop_value = 0;
	float times = 1.0f;
    std::string pecent;
    
	switch(m_pEquipCurInfo->getBaseInfo()->getEquipType())
	{
        case equip::kEquipTypeWeapon:	//武器
            type = equip::kEquipPropAttack;
			times = 0.51f;
            break;
        case equip::kEquipTypeArmor:	//防具
            type = equip::kEquipPropDefence;
			times = 0.99f;
            break;
        case equip::kEquipTypeOrnament:	//饰品
            type = equip::kEquipPropHp;
            break;
        case equip::kEquipTypeTreasure:		//典籍 + 速度/躲闪/暴击/格挡
        {
            if(m_pEquipCurInfo->getBaseInfo()->getSpeed()>0)
            {
                type = equip::kEquipPropSpeed;
				times = 0.1f;
            }
            else if(m_pEquipCurInfo->getBaseInfo()->getDodgeRate()>0)
            {
                type = equip::kEquipPropDodge;
				times = 0.2f;
                pecent = "%";
            }
            else if(m_pEquipCurInfo->getBaseInfo()->getCriticalRate()>0)
            {
                type = equip::kEquipPropCritical;
				times = 0.2f;
                pecent = "%";
            }
			else if(m_pEquipCurInfo->getBaseInfo()->getBlockRate()>0)
			{
				type = equip::kEquipPropBlock;
				times = 0.2f;
				pecent = "%";
			}
			else if(m_pEquipCurInfo->getBaseInfo()->getHitRate()>0)
			{
				type = equip::kEquipPropHit;
				times = 0.2f;
				pecent = "%";
			}
			else
            {
                CCLOG("error prop");
            }
            break;
        }
        default:
            CCLOG("error type");
            break;
	}
    
    prop_value = m_pEquipModel->computeEquipInfo(m_pEquipCurInfo,type);
    
	
    for (uint8_t i =0; i<equip::kEquipPropTotal; ++i)
    {
        if(m_pPropertyType[i]) m_pPropertyType[i]->setVisible(i==type);
		if(m_pEquipMark[i]) m_pEquipMark[i]->setVisible(i==m_pEquipCurInfo->getBaseInfo()->getEquipType() - 1);
    }

    m_pProperty->setString(CCString::createWithFormat("+%d%s",prop_value,pecent.c_str())->getCString());
    int iFight = m_pEquipModel->computeEquipFight(m_pEquipCurInfo);//(int)prop_value*times;
	m_pFight->setString(CCString::createWithFormat("%d",iFight)->getCString());

	if(m_pEquipCurInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure)
	{
		//更新宝物
		m_pTypeLeft->setEquipType(type);

		unsigned int treasurelevel = m_pEquipCurInfo->getLevel();
		CCDictionary* pResistDict = DM_GET_BURN_MODEL->getTreasureResistForLevel();
		int resist = 0;
		if(pResistDict->objectForKey(treasurelevel))
		{
			CCArray* plist = dynamic_cast<CCArray*>(pResistDict->objectForKey(treasurelevel));
			resist = dynamic_cast<CCInteger*>(plist->objectAtIndex(iGrade - 1))->getValue();
		}
		m_pResist->setString(CCString::createWithFormat("+%d%s",resist,"%")->getCString());

		CCString* pDesc5 = CCString::createWithFormat("TreasureType%d" , m_pEquipCurInfo->getBaseInfo()->level_type5);
		CCString* pDesc10 = CCString::createWithFormat("TreasureType%d" , m_pEquipCurInfo->getBaseInfo()->level_type10);

		m_pLevelDesc5->setString(D_LOCAL_STRING(pDesc5->getCString() , m_pEquipCurInfo->getBaseInfo()->getLevelRate5()).c_str());
		m_pLevelDesc10->setString(D_LOCAL_STRING(pDesc10->getCString() , m_pEquipCurInfo->getBaseInfo()->getLevelRate10()).c_str());
		m_pLevelActive5->setVisible(treasurelevel >= 5);
		m_pLevelActive10->setVisible(treasurelevel >= 10);
		m_pLevelDesc5->setColor(m_pLevelActive5->isVisible() ? ccYELLOW : ccWHITE);
		m_pLevelDesc10->setColor(m_pLevelActive10->isVisible() ? ccYELLOW : ccWHITE);
		//没有10阶的属性
		m_pLevel10->setVisible(m_pEquipCurInfo->getBaseInfo()->level_type10 != 0);
		m_pLevel5->setPositionY(m_pLevel10->isVisible() ? 35 : 15);

		m_pUpLevelNode->setVisible(false);
		for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
			int color = (int)m_pEquipCurInfo->getBaseInfo()->getEquipGrade();
			m_pProgressBar[i]->setVisible( color == i+1);
			if(color == i+1)
			{
				float percent = 0.0f;
				m_pProgressBar[i]->setPercent(percent);
				int exp = m_pEquipCurInfo->getExp();
				//默认percent是100% 仅当没升满时计算
				if(DM_GET_BURN_MODEL->getTreasureLevel()->objectForKey(m_pEquipCurInfo->getLevel()+1))
				{
					CCArray* pCurrentLevel = dynamic_cast<CCArray*>(DM_GET_BURN_MODEL->getTreasureLevel()->objectForKey(m_pEquipCurInfo->getLevel()));
					CCArray* pNextLevel = dynamic_cast<CCArray*>(DM_GET_BURN_MODEL->getTreasureLevel()->objectForKey(m_pEquipCurInfo->getLevel() + 1));
					CCInteger* pCurrentLevelExp = dynamic_cast<CCInteger*>(pCurrentLevel->objectAtIndex(color - 1));
					CCInteger* pNextLevelExp = dynamic_cast<CCInteger*>(pNextLevel->objectAtIndex(color - 1));
					int delta = exp - pCurrentLevelExp->getValue();
					if(delta > 0)percent = (float)delta/(float)(pNextLevelExp->getValue() - pCurrentLevelExp->getValue());
				}
				m_pProgressBar[i]->setPercent(percent , 0.2f);
			}
		}
	}
}

void EquipInfoView::enhanceCallBack(cocos2d::CCObject * obj)
{
	if (m_pEquipCurInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure && m_pEquipCurInfo->getLevel() >= TREASURE_MAX_LEVEL)
	{
		//宝物已满级提示
		Tips_Alert(D_LOCAL_STRING("TreasureTips3").c_str());
		return;
	}

	if (!m_pEquipCurInfo->isCanEnhance())
	{
		Tips_Alert(D_LOCAL_STRING("EquipMaxLevel").c_str());
		return;
	}
	
    if(m_pFormationDelegate)
    {
        m_pFormationDelegate->showEquipEnhanceView(m_pEquipCurInfo->getId());
        return;
    }
    if(m_pDelegate) m_pDelegate->showInfoView(kEquipEnhance,m_pEquipCurInfo);
}

void EquipInfoView::refineCallBack(cocos2d::CCObject * obj)
{
	if (!DM_GET_EQUIP_MODEL->getEquipRefineIsCanUpLevel(m_pEquipCurInfo , true))
	{
		return;
	}

	//if (!m_pEquipCurInfo->isCanRefine())
	//{
	//	Tips_Alert(D_LOCAL_STRING("EquipCantRefine").c_str());
	//	return;
	//}

	//if (DM_GET_EQUIP_MODEL->isRefineMaxLevel(m_pEquipCurInfo->getExp(), m_pEquipCurInfo->getBaseInfo()->getEquipGrade()))
	//{
	//	Tips_Alert(D_LOCAL_STRING("EquipRefineMax").c_str());
	//	return;
	//}
	
	if(m_pFormationDelegate)
	{
		m_pFormationDelegate->showEquipRefineView(m_pEquipCurInfo->getId());
		return;
	}

	if(m_pDelegate) m_pDelegate->showInfoView(kEquipRefine,m_pEquipCurInfo);
}

void EquipInfoView::changeCallBack(cocos2d::CCObject * obj)
{
    if(m_pFormationDelegate)
    {
        cocos2d::CCArray* pList = CCArray::create();
        m_pEquipModel->getEquipListByTypeIgnoreID(m_pEquipCurInfo->getBaseInfo()->getEquipType(),m_pEquipCurInfo->getId(),*pList);
		uint32_t equipProperty = 0;
		if(m_pEquipCurInfo)
		{
			std::string valueTypeString;
			equipProperty = DM_GET_EQUIP_MODEL->computeEquipInfo(m_pEquipCurInfo,DM_GET_EQUIP_MODEL->getEquipPropType(m_pEquipCurInfo, valueTypeString));
		}
        m_pFormationDelegate->showEquipSelectView(pList, NULL, equipProperty);
    }
}

void EquipInfoView::closeCallBack(cocos2d::CCObject * obj)
{
	removeFromParent();
    if(m_pFormationDelegate)
    {
        m_pFormationDelegate->showFormationView();
        return;
    }
    if(m_pDelegate)
    {
        m_pDelegate->closeInfoView(kEquipInfo);
        return;
    }
    if(m_pveDelegate)
	{
		m_pveDelegate->closePopup();
		return;
	}
    
}


CCSize EquipInfoView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return CCSizeMake(105, 165);
}

#define tag 0
CCTableViewCell* EquipInfoView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	CCTableViewCell *cell = (CCTableViewCell*)table->dequeueCell();
    WarriorHeadSmall *pNode = NULL;
	if (!cell)
	{
		cell = new CCTableViewCell();
		cell->autorelease();
        
        cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
        CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
        DCCBReader * pReader = new DCCBReader(pNodeLoaderLibrary);
        pNode= (WarriorHeadSmall*)pReader->readNodeGraphFromData(m_pFateCellData, this, CCDirector::sharedDirector()->getWinSize());
        pReader->release();
        if(pNode)
        {
            pNode->setTag(tag);
            cell->addChild(pNode);
        }
    }else
    {
        pNode = (WarriorHeadSmall*)cell->getChildByTag(tag);
    }
    
    if(pNode)
    {
        warrior::FateInfo *pFate = (warrior::FateInfo *)m_pFateList->objectAtIndex(idx);
        pNode->setWarriorInfo(m_pWarriorModel->getWarriorBaseByWID(pFate->owner_id));
    }
	return cell;
}

uint32_t EquipInfoView::numberOfCellsInTableView(CCTableView *table)
{
	return m_pFateList->count();
}

