//
//  MapInfoView.cpp
//  QSMY
//
//  Created by wanghejun on 13-4-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MapInfoView.h"
#include "Pve.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"
#include "network/HttpClient.h"
#include "../Game.h"
#include "RewardItemBox.h"
#include "./effect/ExploreResultEffect.h"
#include "./effect/ExploreBattleEffect.h"
#include "./effect/ExploreEffect.h"
#include "./effect/DropWonderEffect.h"
#include "managers/SoundManager.h"
#include "utils/TimerUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace sp;


MapInfoView::MapInfoView()
:m_pDelegate(NULL)
//,m_ChapterName(NULL)
,m_pRewardItemBox(NULL)
//,m_EquipGet(NULL)
//,m_EquipGotten(NULL)
,m_pLblPlan(NULL)
//,m_pBackground(NULL)
//,m_mapName(NULL)
//,m_pRichMapName(NULL)
//,m_pItemAward(NULL)
//,m_pEquipBox1(NULL)
,m_pMapInfo(NULL)
,m_pChapterInfo(NULL)
//,m_pProgress(NULL)
//,m_pPresEffect(NULL)
,m_nFightOpen(-1)
,m_nFightNum(-1)
,m_nMapType(-1)
//,m_pPresEffectNode(NULL)
,m_pEffectLayer(NULL)
,m_pExplorerEffect(NULL)
//,m_pEffectNode(NULL)
,oScale(0.f)
,m_pDialog(NULL)
,m_pDialogPopup(NULL)
//,m_pDialogContainer(NULL)
,m_nSoundID(0)
,m_bShowingDialog(false)
,m_pNodeGot(NULL)
,m_pBMFCoins(NULL)
,m_pBMFExp(NULL)
,m_pNodeFight(NULL)
,m_pTTFDes(NULL)
,m_pNodeViewOrEvade(NULL)
,m_pNodeHead(NULL)
,m_uFirstStep(0)
,m_pIconContainer(NULL)
, m_pBtnPve(NULL)
, m_pBtnWonder(NULL)
, m_pNodeExplore(NULL)
, m_pAnimateResult(NULL)
,m_pSuggestLevel(NULL)
,m_pButtonAnimationSW(NULL)
,m_pBtnElite(NULL)
{
	D_RESET_C_ARRAY(m_pBtnProgress, STEP_COUNT);
	D_RESET_C_ARRAY(m_pBtnPath, PATH_COUNT);
}

MapInfoView::~MapInfoView()
{
    setMapInfo(NULL);
    removeAllChildren();
//	CC_SAFE_RELEASE(m_ChapterName);
	CC_SAFE_RELEASE(m_pRewardItemBox);
//	CC_SAFE_RELEASE(m_EquipGet);
//    CC_SAFE_RELEASE(m_EquipGotten);
	CC_SAFE_RELEASE(m_pLblPlan);
//	CC_SAFE_RELEASE(m_pBackground);
//	CC_SAFE_RELEASE(m_mapName);
//   CC_SAFE_RELEASE(m_pRichMapName);
//	CC_SAFE_RELEASE(m_pItemAward);
//	CC_SAFE_RELEASE(m_pEquipBox1);
//	CC_SAFE_RELEASE(m_pProgress);
//    CC_SAFE_RELEASE(m_pPresEffectNode);
//    CC_SAFE_RELEASE(m_pPresEffect);
    
   CC_SAFE_RELEASE(m_pEffectLayer);
   CC_SAFE_RELEASE(m_pExplorerEffect);
//	CC_SAFE_RELEASE(m_pEffectNode);

	CC_SAFE_RELEASE(m_pDialog);
	CC_SAFE_RELEASE(m_pDialogPopup);
//	CC_SAFE_RELEASE(m_pDialogContainer);

	D_SAFE_RELEASE_C_ARRAY(m_pBtnProgress, STEP_COUNT);
	D_SAFE_RELEASE_C_ARRAY(m_pBtnPath, PATH_COUNT);

	CC_SAFE_RELEASE(m_pNodeGot);
	CC_SAFE_RELEASE(m_pBMFCoins);
	CC_SAFE_RELEASE(m_pBMFExp);

	CC_SAFE_RELEASE(m_pNodeFight);
	CC_SAFE_RELEASE(m_pTTFDes);
	CC_SAFE_RELEASE(m_pSuggestLevel);
	CC_SAFE_RELEASE(m_pNodeViewOrEvade);
	CC_SAFE_RELEASE(m_pNodeHead);

	CC_SAFE_RELEASE(m_pIconContainer);
	CC_SAFE_RELEASE(m_pBtnPve);
	CC_SAFE_RELEASE(m_pBtnWonder);
	CC_SAFE_RELEASE(m_pNodeExplore);
	CC_SAFE_RELEASE(m_pAnimateResult);
	CC_SAFE_RELEASE(m_pButtonAnimationSW);
	CC_SAFE_RELEASE(m_pBtnElite);

    ResourceManager::sharedResourceManager()->clearCache();
}

bool MapInfoView::init()
{
    if(!DLayer::init())
	{
		return false;
	}
    
	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("RewardItemBox", RewardItemBoxLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("MapProgress", MapProgressLoader::loader());
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_map/PL_map_ts", pNodeLoaderLibrary);
	
    
 	m_pRewardItemBox->getTouchEvent()->setTarget(this, menu_selector(MapInfoView::onTouchItem1));
//     
//     m_pPresEffect = ResourceManager::sharedResourceManager()->getUIAnimate("pres");
//     CC_SAFE_RETAIN(m_pPresEffect);
//     
    m_pEffectLayer = DEffectLayer::create();
    CC_SAFE_RETAIN(m_pEffectLayer);
    m_pExplorerEffect->addChild(m_pEffectLayer);
    
//    setAccelerometerEnabled(true);
//    setAccelerometerInterval(CCDirector::sharedDirector()->getAnimationInterval());

    oScale = m_pIconContainer->getScale();
    oPos = ccpSub(m_pIconContainer->getPosition(), ccp(0, PVE_BATTLE_BACK_Y_POS));
//     
//     if (NULL == m_pRichMapName) {
//         m_pRichMapName = CCRichLabelTTF::create();
//         CC_SAFE_RETAIN(m_pRichMapName);
//         
// 		replaceLabel(m_mapName,m_pRichMapName);
//         m_mapName->setString("");
//     }

	for (int i = 0; i < PATH_COUNT; i++)
	{
		m_pBtnPath[i]->setEnabled(false);
		m_pBtnPath[i]->unselected();
	}
	for (int i = 0; i < STEP_COUNT; i++)
	{
		m_pBtnProgress[i]->setEnabled(false);
		m_pBtnProgress[i]->unselected();
	}

	this->hideResult();
	m_pNodeFight->setVisible(false);
	m_pNodeViewOrEvade->setVisible(false);

	m_pBtnPve->setEnabled(false);
	m_pBtnPve->selected();
	m_pBtnWonder->setTarget(this, menu_selector(MapInfoView::callBackWonder));
	m_pNodeGot->setVisible(false);
	//精英赛
	m_pBtnElite->setTarget(this,menu_selector(MapInfoView::touchElite));
    return true;
}

bool MapInfoView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ChapterName",	CCLabelBMFont *,	m_ChapterName);
 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxPlan",RewardItemBox *,	m_pRewardItemBox);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_EquipGet",		CCNode *,			m_EquipGet);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_EquipGotten",  CCSprite*,			m_EquipGotten);
//     
 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLblPlan",		CCLabelBMFont *,	m_pLblPlan);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackground",	CCSprite *,			m_pBackground);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_mapName",		CCLabelTTF *,	m_mapName);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemAward",	CCNode *,			m_pItemAward);
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipBox1",	ItemBox *,          m_pEquipBox1);
// 	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgress",    CCNode *,           m_pProgress);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPresEffectNode",CCNode *,         m_pPresEffectNode);
     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExplorerEffect",	CCNode *,		m_pExplorerEffect);
//     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode",	CCNode *,		m_pEffectNode);
//     
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgress", MapProgress *, m_pProgress);
// 
// 	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDialogContainer", CCNode *, m_pDialogContainer);

	D_CCB_ASSIGN_ARRAY("m_pBtnProgress", CCMenuItem *, m_pBtnProgress, STEP_COUNT);
	D_CCB_ASSIGN_ARRAY("m_pBtnPath", CCMenuItem *, m_pBtnPath, PATH_COUNT);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeGot", CCNode *, m_pNodeGot);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCoins", CCLabelBMFont *, m_pBMFCoins);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFExp", CCLabelBMFont *, m_pBMFExp);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFight", CCNode *, m_pNodeFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFDes", CCLabelTTF *, m_pTTFDes);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSuggestLevel", CCRichLabelTTF*, m_pSuggestLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeViewOrEvade", CCNode *, m_pNodeViewOrEvade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeHead", CCNode *, m_pNodeHead);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconContainer", CCNode *, m_pIconContainer);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnPve", CCMenuItem *, m_pBtnPve);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnWonder", CCMenuItem *, m_pBtnWonder);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeExplore", CCNode *, m_pNodeExplore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateResult", UIAnimate *, m_pAnimateResult);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonAnimationSW", CCNode* ,m_pButtonAnimationSW);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnElite" , CCMenuItem*,m_pBtnElite);
	return false;
}

SEL_MenuHandler MapInfoView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchArrow",		MapInfoView::onTouchArrow);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchExplore",	MapInfoView::onTouchExplore);
    
	return NULL;
}

void MapInfoView::onEnter()
{
	DLayer::onEnter();
    binding();
    
    onMapInfoUpdated();

	Game::sharedGame()->setNavigatorVisable(false);
}

void MapInfoView::onExit()
{
    m_nFightOpen = -1;
    m_nFightNum = -1;
    m_nMapType = -1;
    unbinding();

	Game::sharedGame()->setNavigatorVisable(true);
    DLayer::onExit();
}

void MapInfoView::binding()
{
    BINDING_ONLY(m_pMapInfo, this, MapInfoView::onMapInfoUpdated, D_EVENT_PVE_MAP_UPDATED);
    BINDING_ONLY(m_pChapterInfo, this, MapInfoView::onMapInfoUpdated, D_EVENT_PVE_CHAPTER_UPDATED);
    BINDING_ONLY(m_pChapterInfo, this, MapInfoView::showPresAdd, D_EVENT_PVE_CHAPTER_PRES_ADD);
}

void MapInfoView::showPresAdd()
{
     m_pLblPlan->setString(CCString::createWithFormat("%d/%d", m_pMapInfo->chapterInfo->pres,m_pMapInfo->chapterInfo->getMaxPrestige())->getCString());
//     m_pPresEffect->removeFromParent();
//     m_pPresEffectNode->addChild(m_pPresEffect);
//     m_pPresEffect->getAnimation()->addObserver(this, SPNotification_callfunc_selector(MapInfoView::removePresEffect), COMPLETE);
//     m_pPresEffect->getAnimation()->playByIndex(0,-1,-1,true);
}

void MapInfoView::removePresEffect(sp::Armature * target, const char* event,const char* data)
{
    target->removeFromParent();
}

void MapInfoView::unbinding()
{
    UNBINDING_ALL(m_pMapInfo, this);
    UNBINDING_ALL(m_pChapterInfo, this);
}

void MapInfoView::setMapInfo(pve::MapInfo *pMapInfo)
{
    if(pMapInfo == NULL) return;
    
	unbinding();
    
	m_pIconContainer->removeAllChildrenWithCleanup(true);
	CCNode *pNode = ResourceManager::sharedResourceManager()->getSceneIcon(pMapInfo->chapter_id);
	pNode->setPositionY(PVE_BATTLE_BACK_Y_POS);
	m_pIconContainer->addChild(pNode);

    m_pMapInfo = pMapInfo;
    m_pChapterInfo = pMapInfo->chapterInfo;
	bool isActivity = false; //是否是活动副本
	PveModel *m_pPveModel = DM_GET_PVE_MODEL;
    if(m_pChapterInfo && m_pChapterInfo->chapterType == m_pPveModel->getActivityID())
	{
		isActivity = true;
	}
	
	if (m_pPveModel)
	{
		if (m_pMapInfo)
		{
			if(isActivity)
			{
				m_pPveModel->setLastShowActivityMapID(m_pMapInfo->getMapId());
				m_pPveModel->setShowActivityChapter(false);
				m_pPveModel->setShowActivityMap(false);
			}
			else
			{
				m_pPveModel->setLastShowMapID(m_pMapInfo->getMapId());
				m_pPveModel->setShowChapter(false);
				m_pPveModel->setShowMap(false);
			}
			
		}
        
		if (m_pChapterInfo)
		{
			if(isActivity)
			{
				m_pBtnElite->setEnabled(false);
				//m_pBtnElite->selected();
				m_pPveModel->setLastShowActivityChapterID(m_pChapterInfo->getCid());
			}
			else
			{
				m_pBtnElite->setEnabled(true);
				//m_pBtnElite->selected();
				m_pPveModel->setLastShowChapterID(m_pChapterInfo->getCid());
			}
		}
	}
    
//     m_pEffectNode->removeAllChildren();
//     
//     battle::SceneInfo* pSceneInfo = DM_GET_BATTLE_MODEL->getSceneInfo(m_pChapterInfo->getCid());
//     Armature* pArmature = ResourceManager::sharedResourceManager()->getEffect(pSceneInfo->effect);
//     //pArmature->setPosition(ccp(0,120-480));
//     m_pEffectNode->addChild(pArmature);
    

	if (m_pMapInfo->fight_open != 5)
	{
		m_uFirstStep = 1;
		m_pBtnProgress[0]->setVisible(false);
		m_pBtnProgress[STEP_COUNT - 1]->setVisible(false);
		m_pBtnPath[0]->setVisible(false);
		m_pBtnPath[PATH_COUNT - 1]->setVisible(false);
	}
	

//	m_pTTFDes->setString(DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pMapInfo->enemy_id)->getWarriorName().c_str());
	std::string sName = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pMapInfo->enemy_id)->getWarriorName();
	m_pTTFDes->setString(D_LOCAL_STRING("PvpEnemy%s", sName.c_str()).c_str());
	m_pSuggestLevel->setString(D_LOCAL_STRING("PveSuggestLevel%d", m_pMapInfo->mapLv).c_str());
	m_pNodeHead->removeAllChildrenWithCleanup(true);
	m_pNodeHead->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForDetail(m_pMapInfo->enemy_id));

    binding();
    onMapInfoUpdated();
}


void MapInfoView::AdjustExplorePoint(int nFightNum, int nFightOpen, int nMapType)
{
    if(nFightNum == m_nFightNum && m_nFightOpen == nFightOpen && nMapType == m_nMapType) return;
    
    if (nFightOpen > MAX_STEP)
    {
        nFightOpen = MAX_STEP;
    }
    
    if (nFightNum > nFightOpen)
    {
        nFightNum = nFightOpen;
    }
    
// 	m_pProgress->setTotalStep(nFightOpen);
// 	m_pProgress->setIsBoss(nMapType != 0);
// 	m_pProgress->moveToNthStep(nFightNum);
    
	for (int i = 0; i < PATH_COUNT; i++)
	{
		if(nFightNum + m_uFirstStep > i)
		{
			m_pBtnPath[i]->unselected();
		}
		else
		{
			m_pBtnPath[i]->selected();
		}
		
	}

	for (int i = 0; i < STEP_COUNT; i++)
	{
		if(nFightNum + m_uFirstStep == i)
		{
			m_pBtnProgress[i]->selected();
		}
		else if(nFightNum + m_uFirstStep > i)
		{
			m_pBtnProgress[i]->setEnabled(true);
			m_pBtnProgress[i]->setEnabled(false);
		}
		else
		{
			m_pBtnProgress[i]->unselected();
		}
	}
	

    m_nFightOpen = nFightOpen;
    m_nFightNum = nFightNum;
    m_nMapType = nMapType;
}

void MapInfoView::onMapInfoUpdated()
{
    if(m_pMapInfo->remainTimes()==0 && !m_bShowingDialog)
    {
        if(m_pDelegate) m_pDelegate->showMapView(m_pChapterInfo->getCid());
        return;
    }
    updateView();
}

void MapInfoView::updateView()
{
	if (m_pMapInfo->chapterInfo)
	{
//		m_ChapterName->setString(m_pMapInfo->chapterInfo->chapter_name.c_str());
        
        pve::PrestigeReward* pPrestigeReward = m_pMapInfo->chapterInfo->getAwardToShow();
        
        if (pPrestigeReward)
		{
			m_pRewardItemBox->setInfo(m_pMapInfo->chapterInfo, pPrestigeReward->prestige_reward_need_viplv);
		}

		//是否可以领取声望奖励
		if (m_pChapterInfo->canGetPrestigeReward())
		{
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVNET_PVE_CAN_GET_REPUTATION);
		}
		
		m_pButtonAnimationSW->setVisible(m_pChapterInfo->canGetPrestigeReward());
	}
//     
//     m_pRichMapName->setFontFillColor(m_mapName->getColor());
//     m_pRichMapName->enableStroke(m_mapName->getColor(), 1.0);
//     m_pRichMapName->setString(CCString::createWithFormat("%d.%s",m_pMapInfo->getMapId()%100,m_pMapInfo->map_name.c_str())->getCString());
//     
	int nTimes		= m_pMapInfo->iTimes;
    int nFightOpen	= m_pMapInfo->fight_open;
    int nFightMod	= nTimes%nFightOpen;
    
     AdjustExplorePoint(nFightMod, nFightOpen, m_pMapInfo->mapType);

	 showBattleIfNeeded();
	 m_pNodeExplore->setVisible(!m_pNodeFight->isVisible());
	 
//     
//     
     m_pLblPlan->setString(CCString::createWithFormat("%d/%d", m_pMapInfo->chapterInfo->pres,m_pMapInfo->chapterInfo->getMaxPrestige())->getCString());
//     
//     
//     m_pItemAward->setVisible(true);
//     m_pEquipBox1->getGrade()->setVisible(true);
//     m_pEquipBox1->getContainer()->removeAllChildren();
//     
//     switch (m_pMapInfo->prize_type)
//     {
//         case pve::kPveRewardTypeEquip:
//         {
//             equip::EquipBaseInfo* pInfo = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(m_pMapInfo->prize_id);
//             m_pEquipBox1->getGrade()->setColor(DM_GET_EQUIP_MODEL->getEquipColorByGrade(pInfo->getEquipGrade()));
//             m_pEquipBox1->getContainer()->addChild(ResourceManager::sharedResourceManager()->getEquipIcon50P(m_pMapInfo->prize_id));
//             m_pEquipBox1->getName()->setString(pInfo->getEquipName().c_str());
//             break;
//         }
//         case pve::kPveRewardTypeItem:
//         {
//             item::ItemBase* pInfo = DM_GET_ITEM_MODEL->getItemBaseByID(m_pMapInfo->prize_id);
//             m_pEquipBox1->getGrade()->setVisible(false);
//             m_pEquipBox1->getContainer()->addChild(ResourceManager::sharedResourceManager()->getItemIcon50P(m_pMapInfo->prize_id));
//             m_pEquipBox1->getName()->setString(pInfo->getItemName().c_str());
//             break;
//         }
//         case pve::kPveRewardTypeSoul:
//         case pve::kPveRewardTypeWarrior:
//         {
//             warrior::WarriorBaseInfo* pInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pMapInfo->prize_id);
//             m_pEquipBox1->getGrade()->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(m_pMapInfo->prize_id));
//             m_pEquipBox1->getContainer()->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon40P(m_pMapInfo->prize_id));
//             m_pEquipBox1->getName()->setString(pInfo->getWarriorName().c_str());
//             break;
//         }
//         case pve::kPveRewardTypeNone:
//         default:
//             m_pItemAward->setVisible(false);
//             break;
//     }
//     
//     
// 	CCSprite *pBackground = ResourceManager::sharedResourceManager()->getSceneIcon(m_pMapInfo->chapterInfo->getCid());
// 	if (pBackground)
// 	{
// 		m_pBackground->setDisplayFrame(pBackground->displayFrame());
// 	}
}

void MapInfoView::onTouchExplore(cocos2d::CCObject *obj)
{
	m_pNodeGot->setVisible(false);
    if(!DM_GET_USER_MODEL->getUserInfo()->hasPower())
    {
        if(m_pDelegate) m_pDelegate->showPowerNotEnough();
        return;
    }
	else if (m_pNodeFight->isVisible())
	{
		if(m_pDelegate) m_pDelegate->startExploreRequest(m_pMapInfo->getMapId());
	}
	else
	{
		//开始探索
		displayExploreAnimation();
//		if(m_pDelegate) m_pDelegate->startExploreRequest(m_pMapInfo->getMapId());
	}
}

void MapInfoView::removeNode(CCNode *obj)
{
    CCNode *pNode = dynamic_cast<CCNode *>(obj);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }
}

void MapInfoView::onTouchArrow(CCObject *obj)
{
    if(m_pDelegate) m_pDelegate->hideMapInfoView();
}

void MapInfoView::displayExploreAnimation()
{
	m_pNodeExplore->setVisible(false);
     ExploreEffect* pEffect = ExploreEffect::create(getMapInfo());
	pEffect->setCallBack(this,callfuncO_selector(MapInfoView::exploreEnd));
    pEffect->setOffset(ccpSub(m_pIconContainer->getPosition(), oPos), oScale);
    m_pEffectLayer->addNode(pEffect);
//     if(m_pDelegate) m_pDelegate->startExploreRequest(m_pMapInfo->getMapId());
}

void MapInfoView::exploreEnd(CCObject *pObj)
{
	int nTimes		= m_pMapInfo->iTimes;
	int nFightOpen	= m_pMapInfo->fight_open;
	int nFightMod	= nTimes%nFightOpen;
	if (nFightMod == nFightOpen - 1)
	{
		showBattleIfNeeded();
		//探索遭遇战斗时播放音效
		DM_GET_SOUND_MODEL->playCommonEffectSound(SFX_PVE_EXPLORE_N, 4);
	}
	else
	{
		if(m_pDelegate) m_pDelegate->startExploreRequest(m_pMapInfo->getMapId());
	}
}

void MapInfoView::onTouchItem1(cocos2d::CCObject *obj)
{
	if (m_pDelegate)
	{
		m_pDelegate->showTouchItem(m_pChapterInfo->getCid());
	}
}


void MapInfoView::didAccelerate(CCAcceleration* pAccelerationValue)
{
    DLayer::didAccelerate(pAccelerationValue);
    //CCLog("%f,%f,%f,%f",pAccelerationValue->x,pAccelerationValue->y,pAccelerationValue->z,pAccelerationValue->timestamp);
    
    pAccelerationValue->y+=0.5f;
    
    pAccelerationValue->x = MAX(MIN(pAccelerationValue->x, 1),-1);
    pAccelerationValue->y = MAX(MIN(pAccelerationValue->y, 1),-1);
    
    CCPoint p = ccpAdd(oPos, ccp(-pAccelerationValue->x*64,-pAccelerationValue->y*60));
    CCPoint cp = m_pIconContainer->getPosition();
    cp.x = cp.x +(p.x-cp.x)*0.3f;
    cp.y = cp.y +(p.y-cp.y)*0.3f;
    m_pIconContainer->setPosition(cp);
}






void MapInfoView::playDialog(pve::MapDialogInfo* pInfo)
{
// 	if(m_pDialog==NULL && m_pDialogPopup == NULL)
// 	{
// 		CCNode* pNode = NULL;
// 		CCB_READER("ui/ccb/common/storyTalk",pNode);
// 		m_pDialog = static_cast<StoryTalk*>(pNode);
// 		CC_SAFE_RETAIN(m_pDialog);
// 		m_pDialog->getNextDialogButton()->setTarget(this, menu_selector(MapInfoView::playNextDialog));
// 		m_pDialogPopup = DPopup::create();
// 		m_pDialogPopup->setOpacity(180);
// 		m_pDialogPopup->setPopupDelegate(this);
// 		m_pDialogPopup->addChild(m_pDialog);
// 		CC_SAFE_RETAIN(m_pDialogPopup);
// 	}
// 	m_pDialog->showInfo(pInfo);
// 	//CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, ccs(pInfo->getSound()));
// 	m_nSoundID = SoundManager::sharedSoundManager()->playSFX(ccs(pInfo->getSound()));
// 	if (!m_pDialogPopup->isOpening())
// 	{
// 		Game::sharedGame()->openPopup(m_pDialogPopup);
// 	}
}

void MapInfoView::playNextDialog(CCObject *sender)
{
// 	if (m_pDelegate)
// 	{
// 		SoundManager::sharedSoundManager()->stopSFX(m_nSoundID);
// 		if (m_pDelegate->isLastDialog())
// 		{
// 			m_pDialogPopup->closePopup();
// 			return;
// 		}
// 		m_pDelegate->playNextDialog();
// 	}
	
}

void MapInfoView::onPopupClose(DPopup*)
{
// 	if (m_pDelegate)
// 	{
// 		m_pDelegate->endDialog();
// 	}
}

void MapInfoView::onPopupBack(DPopup* pPopup)
{
//	this->playNextDialog(NULL);
}


void MapInfoView::showResult()
{
	settlement::SettlementInfoPve *pSettlementInfo = DM_GET_SETTLEMENT_MODEL->getSettlementInfoPve();
	m_pNodeGot->setVisible(true);
	m_pBMFCoins->setString(D_CSTRING_FROM_INT(pSettlementInfo->coin));
	m_pBMFExp->setString(D_CSTRING_FROM_INT(pSettlementInfo->exp));
//	m_pNodeExplore->setVisible(true);
	showBattleIfNeeded();
	m_pAnimateResult->setVisible(false);
}

void MapInfoView::hideResult()
{
//	m_pNodeGot->setVisible(false);
	showBattleIfNeeded();
	m_pAnimateResult->setVisible(true);
}

void MapInfoView::showBattleIfNeeded()
{
	if (m_pMapInfo == NULL)
	{
		return;
	}
	
	int nTimes		= m_pMapInfo->iTimes;
	int nFightOpen	= m_pMapInfo->fight_open;
	int nFightMod	= nTimes%nFightOpen;
	bool bVisible = nFightMod == nFightOpen - 1 && !m_pNodeGot->isVisible();
	m_pNodeFight->setVisible(bVisible);
	
}
void MapInfoView::callBackWonder(CCObject *pObj)
{
	Game::sharedGame()->NavigateTo(navigation::kNavWonder);
}

void MapInfoView::touchElite(CCObject *obj)
{
	if(m_pDelegate)
	{
		m_pDelegate->showEliteView();
	}
}