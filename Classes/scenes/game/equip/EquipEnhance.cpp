//
//  EquipEnhance.cpp
//  装备强化选择列表
//
//  Created by cll on 13-4-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//



#define SFX_EQUIP_ENHANCE CCString::create("equip_enhance")
#define ENHANCE_FLASH_DELTA_Y 100.0f
#define LEVEL_SHINE_TAG	101

#include "EquipEnhance.h"
#include "Equip.h"
#include "../../../managers/ResourceManager.h"
#include "../../../managers/GameManager.h"
#include "../formation/Formation.h"
#include "../../../managers/TipsManager.h"
#include "../Game.h"

USING_NS_CC;

#define  EQUIP_NAME_Y	550

EquipEnhance::EquipEnhance()
:m_pDelegate(NULL)
,m_pFormationDelegate(NULL)
,m_pEquipCurInfo(NULL)
,m_pAutoEnhanceButton(NULL)
,m_pCurrentEquipLv(0)
,m_pCurrentCoin(0)
,m_pContainer(NULL)
,m_pEnhanceButton(NULL)
,m_pGrade(NULL)
,m_pName(NULL)
,m_pEquipModel(NULL)
,m_pLevel(NULL)
,m_pUpLevelNode(NULL)
,m_pProperty(NULL)
,m_pItemBox(NULL)
,m_pItemName(NULL)
,m_pItemNeed(NULL)
,m_pMyCoin(NULL)
,m_pItemCount(NULL)
,m_pVipLvNeed(NULL)
,m_pPropertyResult(NULL)
,m_pCloseButton(NULL)
,m_totalLv(0)
,m_pPositionNode(NULL)
,m_pTreasureNode(NULL)
,m_pTreasureCost(NULL)
,m_pResist(NULL)
,m_pResistResult(NULL)
,m_pTypeLeft(NULL)
,m_pTypeRight(NULL)
,m_pLevelDesc5(NULL)
,m_pLevelActive5(NULL)
,m_pLevelDesc10(NULL)
,m_pLevelActive10(NULL)
,m_pLevel5(NULL)
,m_pLevel10(NULL)
,m_pTreasurePopup(NULL)
,m_uClickIndex(0)
,m_pButtonLeft(NULL)
,m_pButtonMiddle(NULL)
,m_pButtonRight(NULL)
,m_pLevelShine(NULL)
{
	D_RESET_C_ARRAY(m_pEquipBox, TREASURE_BURN_MAX);
	D_RESET_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}



EquipEnhance::~EquipEnhance()
{
    
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipCurInfo, this);
    
    CC_SAFE_RELEASE(m_pAutoEnhanceButton);
    CC_SAFE_RELEASE(m_pEnhanceButton);
    
    CC_SAFE_RELEASE(m_pContainer);
    
    CC_SAFE_RELEASE(m_pGrade);
	CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pUpLevelNode);
    
    CC_SAFE_RELEASE(m_pProperty);
    
    CC_SAFE_RELEASE(m_pItemBox);
    CC_SAFE_RELEASE(m_pItemName);
    CC_SAFE_RELEASE(m_pItemNeed);
	CC_SAFE_RELEASE(m_pMyCoin);
    CC_SAFE_RELEASE(m_pItemCount);
    CC_SAFE_RELEASE(m_pVipLvNeed);
    CC_SAFE_RELEASE(m_pPropertyResult);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pPositionNode);
	CC_SAFE_RELEASE(m_pTreasureNode);
	CC_SAFE_RELEASE(m_pTreasureCost);
	CC_SAFE_RELEASE(m_pResist);
	CC_SAFE_RELEASE(m_pResistResult);
	CC_SAFE_RELEASE(m_pTypeLeft);
	CC_SAFE_RELEASE(m_pTypeRight);
	CC_SAFE_RELEASE(m_pLevelDesc5);
	CC_SAFE_RELEASE(m_pLevelDesc10);
	CC_SAFE_RELEASE(m_pLevelActive5);
	CC_SAFE_RELEASE(m_pLevelActive10);
	CC_SAFE_RELEASE(m_pLevel5);
	CC_SAFE_RELEASE(m_pLevel10);
	D_SAFE_RELEASE_C_ARRAY(m_pEquipBox, TREASURE_BURN_MAX);
	CC_SAFE_RELEASE(m_pTreasurePopup);
	CC_SAFE_RELEASE(m_pButtonLeft);
	CC_SAFE_RELEASE(m_pButtonMiddle);
	CC_SAFE_RELEASE(m_pButtonRight);
	D_SAFE_RELEASE_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
	CC_SAFE_RELEASE(m_pLevelShine);
}


bool EquipEnhance::init()
{
    
	if(!DLayer::init())
	{
		return false;
	}
    
	m_pEquipModel = DM_GET_EQUIP_MODEL;

	return true;
}


bool EquipEnhance::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAutoEnhanceButton", DButton *, this->m_pAutoEnhanceButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnhanceButton", DButton *, this->m_pEnhanceButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", WarriorGradeBg *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevelNode", CCNode*, this->m_pUpLevelNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty", CCLabelBMFont *, this->m_pProperty);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyResult", CCLabelBMFont *, this->m_pPropertyResult);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBox", ItemBox *, this->m_pItemBox);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemName", CCLabelTTF *, this->m_pItemName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemNeed", CCLabelBMFont *, this->m_pItemNeed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMyCoin", CCLabelBMFont *, this->m_pMyCoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemCount", CCLabelBMFont *, this->m_pItemCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipLvNeed", CCLabelBMFont *, this->m_pVipLvNeed);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyNode", CCSprite *, this->m_pPropertyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPositionNode", CCNode *, this->m_pPositionNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTreasureNode", CCNode *, this->m_pTreasureNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTreasureCost", CCLabelBMFont *, this->m_pTreasureCost);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pResist", CCLabelBMFont *, this->m_pResist);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pResistResult", CCLabelBMFont *, this->m_pResistResult);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTypeLeft", EquipTypeMark *, this->m_pTypeLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTypeRight", EquipTypeMark *, this->m_pTypeRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelDesc5", CCLabelTTF *, this->m_pLevelDesc5);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelDesc10", CCLabelTTF *, this->m_pLevelDesc10);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelActive5", CCNode *, this->m_pLevelActive5);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelActive10", CCNode *, this->m_pLevelActive10);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel5", CCNode *, this->m_pLevel5);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel10", CCNode *, this->m_pLevel10);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonLeft", DButton *, this->m_pButtonLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonMiddle", DButton *, this->m_pButtonMiddle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonRight", DButton *, this->m_pButtonRight);

	D_CCB_ASSIGN_ARRAY("m_pBox", EquipBox *, m_pEquipBox, TREASURE_BURN_MAX);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar1", DProgressBar *, this->m_pProgressBar[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar2", DProgressBar *, this->m_pProgressBar[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar", DProgressBar *, this->m_pProgressBar[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar4", DProgressBar *, this->m_pProgressBar[3]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelShine", CCLabelBMFont *, this->m_pLevelShine);
	return false;
}


void EquipEnhance::treasureBurnCallBack(cocos2d::CCObject * pObj)
{
	DM_GET_BURN_MODEL->doTreasureBurn(m_pEquipCurInfo->getId());
}

void EquipEnhance::treasureAutoCallBack(cocos2d::CCObject * pObj)
{
	if (m_pEquipCurInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure && m_pEquipCurInfo->getLevel() >= TREASURE_MAX_LEVEL)
	{
		//宝物已满级提示
		Tips_Alert(D_LOCAL_STRING("TreasureTips3").c_str());
		return;
	}

	DM_GET_BURN_MODEL->makeTreasureAutoList(m_pEquipCurInfo->getId());
}

void EquipEnhance::updatePreview()
{
	int expAdded = 0;
	CCDictionary* pDict = DM_GET_BURN_MODEL->getTreasureDict();
	for(int round = 0 ; round < TREASURE_BURN_MAX ; round++)
	{
		if(pDict->objectForKey(round))
		{
			equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pDict->objectForKey(round));
			expAdded += DM_GET_BURN_MODEL->getTreasureExpAdd(pInfo);
		}
	}

	//先把数字的动画停了
	stopActionByTag(LEVEL_SHINE_TAG);
	m_pLevelShine->setVisible(false);
	m_pLevel->setOpacity(255);
	//进度条
	for(int i = 0 ; i < WARRIOR_GRADE_NUM ; i++)
	{
		m_pProgressBar[i]->setShining(expAdded>0);
	}

	int level = m_pEquipCurInfo->getLevel();
	int levelFinal = level;
	int exp = m_pEquipCurInfo->getExp();
	int expFinal = exp + expAdded;
	int iGrade = (int)m_pEquipCurInfo->getBaseInfo()->getEquipGrade();	

	if(expAdded > 0)
	{
		CCDictionary* pLevelExp = DM_GET_BURN_MODEL->getTreasureLevel();
		for(int i = level ; i <= TREASURE_MAX_LEVEL ; i++)
		{
			CCArray* pArray = dynamic_cast<CCArray*>(pLevelExp->objectForKey(i));
			CCInteger* pValue = dynamic_cast<CCInteger*>(pArray->objectAtIndex(iGrade-1));
			if(pValue->getValue() <= expFinal) levelFinal = i;
			else break;
		}

		float percent = 0.0f;
		if(levelFinal > level)
		{
			//可以升级啊 数字闪闪闪
			percent = 1.0f;
			m_pLevelShine->setVisible(true);
			doShine(levelFinal);			
		}
		else
		{
			//不可以升级
			CCArray* pCurrentLevel = dynamic_cast<CCArray*>(DM_GET_BURN_MODEL->getTreasureLevel()->objectForKey(m_pEquipCurInfo->getLevel()));
			CCArray* pNextLevel = dynamic_cast<CCArray*>(DM_GET_BURN_MODEL->getTreasureLevel()->objectForKey(m_pEquipCurInfo->getLevel() + 1));
			CCInteger* pCurrentLevelExp = dynamic_cast<CCInteger*>(pCurrentLevel->objectAtIndex(iGrade - 1));
			CCInteger* pNextLevelExp = dynamic_cast<CCInteger*>(pNextLevel->objectAtIndex(iGrade - 1));
			int delta = expFinal - pCurrentLevelExp->getValue();
			if(delta > 0)percent = (float)delta/(float)(pNextLevelExp->getValue() - pCurrentLevelExp->getValue());
		}
		
		//进度条闪烁
		for(int i = 0 ; i < WARRIOR_GRADE_NUM ; i++)
		{
			m_pProgressBar[i]->setPercentWithShine(percent);
		}
	}
	//下一级属性（至少加1级）
	CCDictionary* pResistDict = DM_GET_BURN_MODEL->getTreasureResistForLevel();
	int resistresult = 0;
	int resultlevel = MAX(levelFinal , level+1);
	if(pResistDict->objectForKey(resultlevel))
	{
		CCArray* plist = dynamic_cast<CCArray*>(pResistDict->objectForKey(resultlevel));
		resistresult = dynamic_cast<CCInteger*>(plist->objectAtIndex(iGrade - 1))->getValue();
	}
	m_pResistResult->setString(CCString::createWithFormat("+%d%s",resistresult,"%")->getCString());

	std::string strPercent = "%";
	equip::EquipPropType type = DM_GET_EQUIP_MODEL->getEquipPropType(m_pEquipCurInfo, strPercent);
	uint32_t result_value = m_pEquipModel->computeEquipInfo(m_pEquipCurInfo,type,true,resultlevel);
	m_pPropertyResult->setString(CCString::createWithFormat("+%d%s",result_value,strPercent.c_str())->getCString());
}

void EquipEnhance::doShine(unsigned int levelTo)
{
	m_pLevelShine->setString(D_CSTRING_FROM_UINT(levelTo));

	CCNumberTo* pShine = CCNumberTo::create(1.6f,0,255,tween_num_selector(EquipEnhance::shining));
	CCNumberTo* pShineReverse = CCNumberTo::create(1.6f,255,0,tween_num_selector(EquipEnhance::shining));
	CCRepeatForever* pShineRepeat = CCRepeatForever::create(CCSequence::createWithTwoActions(pShine , pShineReverse));
	pShineRepeat->setTag(LEVEL_SHINE_TAG);
	runAction(pShineRepeat);
}

void EquipEnhance::shining(float val)
{
	m_pLevelShine->setOpacity(val);
	m_pLevelShine->setOpacityModifyRGB(true);
	m_pLevel->setOpacity(255-val);
	m_pLevel->setOpacityModifyRGB(true);
	//m_pLevelShine->setOpacity(255 - val);
}

void EquipEnhance::onAutoList()
{
	CCDictionary* pDict = DM_GET_BURN_MODEL->getTreasureDict();
	for(int round = 0 ; round < TREASURE_BURN_MAX ; round++)
	{
		if(pDict->objectForKey(round))
		{
			equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pDict->objectForKey(round));
			m_pEquipBox[round]->setTreasureInfo(pInfo);
		}
	}
	updatePreview();
}

void EquipEnhance::setSelectedTreasureList(cocos2d::CCArray * pList)
{
	//选择了空
	if(pList == NULL)return;

	//选择了一项或多项
	int count = pList->count();
	for(int i = m_uClickIndex , round = 0 , index = 0; round < TREASURE_BURN_MAX ; round++)
	{
		if(m_pEquipBox[i]->isEmpty())
		{
			equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pList->objectAtIndex(index));
			//model view 分别添加
			CCDictionary* pDict = DM_GET_BURN_MODEL->getTreasureDict();
			pDict->setObject(pInfo , i);
			m_pEquipBox[i]->setTreasureInfo(pInfo);

			index++;
			if(index == count)break;
		}
		i++;
		i = i % TREASURE_BURN_MAX;
	}
	updatePreview();
}

void EquipEnhance::showTreasureSelectList()
{
	if(!m_pTreasurePopup)
	{
		m_pTreasurePopup = TreasureSelectView::create();
		m_pTreasurePopup->setDelegate(this);
		CC_SAFE_RETAIN(m_pTreasurePopup);
	}	
	m_pTreasurePopup->setCurrentID(m_pEquipCurInfo->getId());
	addChild(m_pTreasurePopup);
}

void EquipEnhance::touchCallBack(cocos2d::CCObject* obj)
{
	if (m_pEquipCurInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure && m_pEquipCurInfo->getLevel() >= TREASURE_MAX_LEVEL)
	{
		//宝物已满级提示
		Tips_Alert(D_LOCAL_STRING("TreasureTips3").c_str());
		return;
	}

	//CCLOG("EquipEnhance::touchCallBack%d" , (dynamic_cast<CCNode*>(obj))->getTag());
	m_uClickIndex = (dynamic_cast<CCNode*>(obj))->getTag();

	//model view 分别添加
	CCDictionary* pDict = DM_GET_BURN_MODEL->getTreasureDict();
	if(pDict->objectForKey(m_uClickIndex))
	{
		//已经添加了 去掉
		m_pEquipBox[m_uClickIndex]->setTreasureInfo(NULL);
		pDict->removeObjectForKey(m_uClickIndex);
		updatePreview();
	}
	else
	{
		showTreasureSelectList();
	}
}

SEL_MenuHandler EquipEnhance::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", EquipEnhance::closeCallBack);
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "pauseCallBack", EquipEnhance::pauseCallBack);
    return NULL;
}

void EquipEnhance::closeCallBack(cocos2d::CCObject* obj)
{
	//如果有动画的话，把动画先停了
	this->stopAllActions();
	removeFromParent();
    if (m_pFormationDelegate)
    {
        m_pFormationDelegate->showFormationView();
    }
    else if (m_pDelegate)
        m_pDelegate->closeInfoView(kEquipEnhance);
}

void EquipEnhance::pauseCallBack(cocos2d::CCObject* obj)
{
	this->stopAllActions();
	EnhanceEffect* pEffect = EnhanceEffect::create(m_totalLv,ccp(m_pContainer->getPositionX() , m_pContainer->getPositionY() - ENHANCE_FLASH_DELTA_Y ) , false);
	Game::sharedGame()->playEffect(pEffect , true);
}


void EquipEnhance::onEnter()
{
	binding();
	BINDING_ONLY(DM_GET_BURN_MODEL, this, EquipEnhance::onAutoList, D_EVENT_BURN_AUTOLIST);
	BINDING_ONLY(DM_GET_BURN_MODEL, this, EquipEnhance::updatePreview, D_EVENT_BURN_ICON_UPDATE);
	DLayer::onEnter();
}

void EquipEnhance::onExit()
{
    UNBINDING_ALL(m_pEquipCurInfo,this);
	UNBINDING_ALL(DM_GET_BURN_MODEL, this);
	DM_GET_BURN_MODEL->getTreasureDict()->removeAllObjects();

	Game::sharedGame()->removeEffct();
	DLayer::onExit();
}

void EquipEnhance::loadLayer()
{
	if(m_pEquipCurInfo == NULL)return;

	removeAllChildren();
	CCNodeLoaderLibrary * pNodeLoaderLibarary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibarary);
	pNodeLoaderLibarary->registerCCNodeLoader("EquipTypeMark", EquipTypeMarkLoader::loader());

	if(m_pEquipCurInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure)
	{
		CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_baowutunshi/PL_baowutunshi", pNodeLoaderLibarary);

		for(int i = 0 ; i < TREASURE_BURN_MAX ; i++)
		{
			m_pEquipBox[i]->getTouchMenu()->setTag(i);
			m_pEquipBox[i]->getTouchMenu()->setTarget(this , menu_selector(EquipEnhance::touchCallBack));
			m_pEquipBox[i]->setTreasureInfo(NULL);
		}
		
		m_pButtonLeft->getButton()->setTarget(this, menu_selector(EquipEnhance::closeCallBack));
		m_pButtonMiddle->getButton()->setTarget(this, menu_selector(EquipEnhance::treasureAutoCallBack));
		m_pButtonRight->getButton()->setTarget(this, menu_selector(EquipEnhance::treasureBurnCallBack));
	}
	else
	{
		CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_zbqh/PL_zbqh", pNodeLoaderLibarary);

		m_pAutoEnhanceButton->getButton()->setTarget(this, menu_selector(EquipEnhance::autoEnhanceCallBack));
		m_pEnhanceButton->getButton()->setTarget(this, menu_selector(EquipEnhance::enhanceCallBack));
	}
	m_pCloseButton->getButton()->setTarget(this, menu_selector(EquipEnhance::closeCallBack));
}

void EquipEnhance::setEquip(equip::EquipCurInfo* pEquipCurInfo)
{
    if(m_pEquipCurInfo==pEquipCurInfo) return;
    
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipCurInfo, this);
    
    CC_SAFE_RETAIN(pEquipCurInfo);
    m_pEquipCurInfo = pEquipCurInfo;

	loadLayer();

    binding();
}


void EquipEnhance::binding()
{
    BINDING_EXEC(m_pEquipCurInfo,this,EquipEnhance::updateEquipInfo,D_EVENT_EQUIPINFO_UPDATE);
}

void EquipEnhance::updateEquipInfo()
{
    
	if (!m_pEquipCurInfo || !m_pEquipCurInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("EquipInfoError").c_str());
		return ;
	}
    
    m_pContainer->removeAllChildren();
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon(m_pEquipCurInfo->getEquipId()));
	
    m_pName->setString(m_pEquipCurInfo->getBaseInfo()->getEquipName().c_str());
    
    m_pLevel->setString(CCString::createWithFormat("%i",m_pEquipCurInfo->getLevel())->getCString());

	//m_pRefineLevel->removeAllChildren();
	//m_pRefineLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(m_pEquipCurInfo->getRefineLevel()));
    
    //m_pGrade->setColor(m_pEquipModel->getEquipColorByGrade(m_pEquipCurInfo->getBaseInfo()->getEquipGrade()));
	int iGrade = (int)m_pEquipCurInfo->getBaseInfo()->getEquipGrade();
	warrior::WarriorGrade wGrade = (warrior::WarriorGrade)iGrade;
	m_pGrade->setWarriorGrade(wGrade);
    
    equip::EquipPropType type = equip::kEquipPropAttack;
    std::string pecent;
    
	switch(m_pEquipCurInfo->getBaseInfo()->getEquipType())
	{
        case equip::kEquipTypeWeapon:	//武器
            type = equip::kEquipPropAttack;
            break;
        case equip::kEquipTypeArmor:	//防具
            type = equip::kEquipPropDefence;
            break;
        case equip::kEquipTypeOrnament:	//饰品
            type = equip::kEquipPropHp;
            break;
        case equip::kEquipTypeTreasure:		//典籍 + 速度/躲闪/暴击/格挡
        {
            if(m_pEquipCurInfo->getBaseInfo()->getSpeed()>0)
            {
                type = equip::kEquipPropSpeed;
            }
            else if(m_pEquipCurInfo->getBaseInfo()->getDodgeRate()>0)
            {
                type = equip::kEquipPropDodge;
                pecent = "%";
            }
            else if(m_pEquipCurInfo->getBaseInfo()->getCriticalRate()>0)
            {
                type = equip::kEquipPropCritical;
                pecent = "%";
            }
            else if(m_pEquipCurInfo->getBaseInfo()->getBlockRate()>0)
            {
                type = equip::kEquipPropBlock;
                pecent = "%";
            }
			else if(m_pEquipCurInfo->getBaseInfo()->getHitRate()>0)
			{
				type = equip::kEquipPropHit;
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
    
    uint32_t prop_value = m_pEquipModel->computeEquipInfo(m_pEquipCurInfo,type);
    uint32_t result_value = m_pEquipModel->computeEquipInfo(m_pEquipCurInfo,type,true);

	m_pTypeLeft->setEquipType(type);
	m_pTypeRight->setEquipType(type);
    
    m_pProperty->setString(CCString::createWithFormat("+%d%s",prop_value,pecent.c_str())->getCString());
	m_pPropertyResult->setString(CCString::createWithFormat("+%d%s",result_value,pecent.c_str())->getCString());

	
	//最后处理一下宝物
	if(m_pEquipCurInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure)
	{
		//m_pPositionNode->setPositionY(-15.0f);
		//m_pTreasureNode->setVisible(true);
		//m_pTreasureCost->setString(D_CSTRING_FROM_UINT(m_pEquipModel->getTreasureEnhanceCostItem(m_pEquipCurInfo)));
	
		//更新宝物
		m_pTypeLeft->setEquipType(type);
		m_pTypeRight->setEquipType(type);

		unsigned int treasurelevel = m_pEquipCurInfo->getLevel();

		CCDictionary* pResistDict = DM_GET_BURN_MODEL->getTreasureResistForLevel();
		int resist = 0;
		int resistresult = 0;
		if(pResistDict->objectForKey(treasurelevel))
		{
			CCArray* plist = dynamic_cast<CCArray*>(pResistDict->objectForKey(treasurelevel));
			resist = dynamic_cast<CCInteger*>(plist->objectAtIndex(iGrade - 1))->getValue();
		}
		if(pResistDict->objectForKey(treasurelevel + 1))
		{
			CCArray* plist = dynamic_cast<CCArray*>(pResistDict->objectForKey(treasurelevel + 1));
			resistresult = dynamic_cast<CCInteger*>(plist->objectAtIndex(iGrade - 1))->getValue();
		}

		m_pResist->setString(CCString::createWithFormat("+%d%s",resist,"%")->getCString());
		m_pResistResult->setString(CCString::createWithFormat("+%d%s",resistresult,"%")->getCString());

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
		m_pLevel5->setPositionY(m_pLevel10->isVisible() ? 0 : -20);

		for(int i = 0 ; i < TREASURE_BURN_MAX ; i++)
		{
			m_pEquipBox[i]->setTreasureInfo(NULL);
		}

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
	else
	{
		m_pPositionNode->setPositionY(0.0f);
		m_pTreasureNode->setVisible(false);

		//我的铜币
		unsigned int mycoin = DM_GET_USER_MODEL->getUserInfo()->getCoins();
		mycoin < MAX_SHOW_COIN ? m_pMyCoin->setString(D_CSTRING_FROM_UINT(mycoin)) : m_pMyCoin->setString(D_CSTRING_WITH10K_FROM_UINT(mycoin));
		//强化所需铜币
		unsigned int needcoin = m_pEquipModel->getEquipEnhanceCost(m_pEquipCurInfo);
		needcoin < MAX_SHOW_COIN ? m_pItemNeed->setString(D_CSTRING_FROM_UINT(needcoin)) : m_pItemNeed->setString(D_CSTRING_WITH10K_FROM_UINT(needcoin));

	}


    //m_pAutoEnhanceButton->getButton()->setEnabled(m_pEquipCurInfo->isCanEnhance());
    //m_pEnhanceButton->getButton()->setEnabled(m_pEquipCurInfo->isCanEnhance());
	
}

void EquipEnhance::enhanceCallBack(cocos2d::CCObject * obj)
{
	//屏蔽掉宝物强化界面
	if(m_pEquipCurInfo->getBaseInfo()->getEquipType()== equip::kEquipTypeTreasure)
	{
		Tips_Alert(D_LOCAL_STRING("EquipTypeCanNotEnhance").c_str());
		return;
	}

	//检验是否能强化，并且提示
	bool canenhance = m_pEquipModel->doEnhanceCheck(m_pEquipCurInfo , false);
	if(canenhance)
	{
		m_pCurrentCoin = DM_GET_USER_MODEL->getUserInfo()->getCoins();
		m_pCurrentEquipLv = m_pEquipCurInfo->getLevel();
		HTTP_CLIENT->enhanceEquip(m_pEquipCurInfo->getId(),this,callfuncJson_selector(EquipEnhance::doEnhanceEquipCallBack),false);
	}  
}

void EquipEnhance::autoEnhanceCallBack(cocos2d::CCObject * obj)
{
	//屏蔽掉宝物强化界面
	if(m_pEquipCurInfo->getBaseInfo()->getEquipType()== equip::kEquipTypeTreasure)
	{
		Tips_Alert(D_LOCAL_STRING("EquipTypeCanNotEnhance").c_str());
		return;
	}

	//检验是否能强化，并且提示
	bool canenhance = m_pEquipModel->doEnhanceCheck(m_pEquipCurInfo , true);
	if(canenhance)
	{
		m_pCurrentCoin = DM_GET_USER_MODEL->getUserInfo()->getCoins();
		m_pCurrentEquipLv = m_pEquipCurInfo->getLevel();
		HTTP_CLIENT->autoEnhanceEquip(m_pEquipCurInfo->getId(),this,callfuncJson_selector(EquipEnhance::doAutoEnhanceCallBack),false);
	}    
}


void EquipEnhance::doEnhanceEquipCallBack(const Json::Value & requestData,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;

	//处理宝物强化 强化石计算
	if(responseData["data"].isMember("item"))
	{
		DM_GET_ITEM_MODEL->parseOneItem(responseData["data"]["item"]);
	}
    
    uint32_t lv =  responseData["data"]["equip"]["lv"].asUInt() - m_pCurrentEquipLv;
    bool crit = lv > 1;
    bool free = m_pCurrentCoin-responseData["data"]["user"]["coin"].asUInt()==0;

	//可以触摸，强化过程中，玩家可以继续点击强化
    EnhanceEffect* pEffect = EnhanceEffect::create(lv , ccp(m_pContainer->getPositionX() , m_pContainer->getPositionY() - ENHANCE_FLASH_DELTA_Y ) , false);

    Game::sharedGame()->playEffect(pEffect);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, SFX_EQUIP_ENHANCE);
}

void EquipEnhance::doAutoEnhanceCallBack(const Json::Value & requestData,const Json::Value &responseData)
{
	//auto enhance
	if(responseData["code"].asString().compare(SUC_CODE)!=0 || !isRunning())
	{
		return;
	}

	//处理宝物强化 强化石计算
	if(responseData["data"].isMember("item"))
	{
		DM_GET_ITEM_MODEL->parseOneItem(responseData["data"]["item"]);
	}

	uint32_t lv =  responseData["data"]["equip"]["lv"].asUInt() - m_pCurrentEquipLv;
	m_totalLv = lv;//最後一次顯示 縂強化等級

	CCArray* pActionArray = CCArray::create();
	for (uint32_t i=0; i<responseData["data"]["process"].size(); ++i)
	{		
		uint32_t lv =  responseData["data"]["process"][i]["lv"].asUInt();
		m_totalLv = lv;//修改了！！！最後一次顯示 單次的強化等級
		bool crit = lv > 1;
		bool free = false;
		EnhanceEffect* pEffect = EnhanceEffect::create(lv, ccp(m_pContainer->getPositionX() , m_pContainer->getPositionY() - ENHANCE_FLASH_DELTA_Y ));
		pActionArray->addObject(CCDelayTime::create(i==0?0.0f:0.5f));
		pActionArray->addObject(CCCallFuncO::create(this, callfuncO_selector(EquipEnhance::playEffect), pEffect));		
	}

	if(pActionArray->count() > 0)
	{
		//数组不为空，防止异常
		CCSequence * pSequence = CCSequence::create(pActionArray);
		this->runAction(pSequence);
		CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, SFX_EQUIP_ENHANCE);
	}
}
void EquipEnhance::playEffect(CCObject* pEffect)
{
	//动画联播 覆盖
	EnhanceEffect* effect = dynamic_cast<EnhanceEffect*>(pEffect);
	Game::sharedGame()->playEffect(effect , true);

	CCMenu* pMenu = CCMenu::create();
	pMenu->setTouchEnabled(false);
	pMenu->setAnchorPoint(CCPointZero);
	pMenu->setPosition(CCPointZero);
	effect->addChild(pMenu);
	CCMenuItem* pItem = DMenuItemImageSound::create(this, menu_selector(EquipEnhance::pauseCallBack));
	pItem->setContentSize(DESIGN_SIZE);
	pItem->setAnchorPoint(CCPointZero);
	pMenu->addChild(pItem);
}


using namespace sp;

EnhanceEffect::EnhanceEffect()
:m_pEnhance(NULL)
,index(0)
,iPriority(DTOUCH_EFFECT_PRIORITY)
{
    
}

EnhanceEffect::~EnhanceEffect()
{
    CC_SAFE_RELEASE(m_pEnhance);
}

EnhanceEffect* EnhanceEffect::create(uint32_t lv , CCPoint pos , bool isAuto )
{
    EnhanceEffect* pRet = new EnhanceEffect();
	pRet->pos = pos;
	if(!isAuto) pRet->iPriority = DPOPUP_USER_LEVELUP_PRIORITY;

    if(pRet && pRet->init(lv))
    {
        pRet->autorelease();
        return pRet;
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

bool EnhanceEffect::init(uint32_t lv)
{
	if(!DEffectNode::init()) return false;

	CCString * animateName = NULL;

	if(lv <= 2){
		index = 0;
		animateName = CCString::create("enhance1");
	}
	else if(lv <= 4){
		index = 1;
		animateName = CCString::create("enhance2");
	}
	else{
		index = 2;
		animateName = CCString::create("enhance3");
	}

	m_pEnhance =  ResourceManager::sharedResourceManager()->getUIAnimate(animateName->getCString(),sp::ZORDER);
	m_pEnhance->getAnimation()->addObserver(this, SPNotification_callfunc_selector(EnhanceEffect::animateEnd), COMPLETE);
	
	SPContainer* pContainer = SPContainer::create(sp::ZORDER);
	CCLabelBMFont* pNode = CCLabelBMFont::create(CCString::createWithFormat("+%d",lv)->getCString(), "fonts/level.fnt");
	pNode->setAnchorPoint(ccp(0.f,0.5f));
	pContainer->addChild(pNode);
	m_pEnhance->getBone("number")->replaceDisplayAt(pContainer, 0);

	//m_pEnhance->setPosition(D_DESIGN_POINT_CENTER);
	m_pEnhance->setPosition(pos);
	CC_SAFE_RETAIN(m_pEnhance);

	showEnhance(NULL,NULL,NULL);
	return true;
}

void EnhanceEffect::showEnhance(sp::Armature * target, const char* event,const char* data)
{
    m_pEnhance->getAnimation()->playByIndex(0);
    addChild(m_pEnhance);
}

void EnhanceEffect::animateEnd(sp::Armature * target, const char* event,const char* data)
{
    callback();
    removeFromParent();
}



EquipTypeMark::EquipTypeMark()
{
	D_RESET_C_ARRAY(m_pMark, equip::kEquipPropTotal);
}

EquipTypeMark::~EquipTypeMark(){
	D_SAFE_RELEASE_C_ARRAY(m_pMark, equip::kEquipPropTotal);
}

void EquipTypeMark::setEquipType(equip::EquipPropType val){
	int ival = (int)val;
	for(int i = 0 ; i < equip::kEquipPropTotal ; i++)
	{
		m_pMark[i]->setVisible(ival == i);
	}
}
void EquipTypeMark::setEquipType(equip::TreasurePropType val){
	if(val == equip::kTreasurePropPercentAttack)val = equip::kTreasurePropAttack;
	if(val == equip::kTreasurePropPercentDefence)val = equip::kTreasurePropDefence;
	if(val == equip::kTreasurePropPercentHp)val = equip::kTreasurePropHp;
	int ival = (int)val;
	for(int i = 0 ; i < equip::kEquipPropTotal ; i++)
	{
		m_pMark[i]->setVisible(ival == i);
	}
}

bool EquipTypeMark::onAssignCCBMemberVariable(CCObject *pTarget,const char *pMemberVariableName, CCNode *pNode)
{
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pEquipType", CCNode*, m_pMark, equip::kEquipPropTotal);
	return false;
}


