//  Promotion.cpp
//  突破界面
//  Created by cll on 13-3-29
//  Copyright (c) 2013, thedream. All rights reserved.
//

#define SFX_PROMOTION "warrior_soulup"

#include "Promotion.h"
#include "Warrior.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "../Game.h"
#include "managers/TipsManager.h"

USING_NS_CC;
using namespace warrior;
using namespace sp;


PromotionView::PromotionView()
:m_pDelegate(NULL)
,m_pWarriorInfo(NULL)
,m_pWarriorModel(NULL)
,m_pContainer(NULL)
,m_pCurUpLevel(NULL)
,m_pToUpLevel(NULL)
,m_pTableNode(NULL)
,m_pCurProps(NULL)
,m_pToProps(NULL)
,m_pCurItems(NULL)
,m_pGrade(NULL)
,m_pLevel(NULL)
,m_pName(NULL)
,m_pConfirmButton(NULL)
,m_pCancelButton(NULL)
,m_pEffect(NULL)
,m_pEffectNode(NULL)
,m_pWarriorTypeMark(NULL)
,m_pHp(NULL)
,m_pAttack(NULL)
,m_pDefence(NULL)
//,m_pSpeed(NULL)
,m_pPotential(NULL)
,m_pAddHp(NULL)
,m_pAddAttack(NULL)
,m_pAddDefence(NULL)
//,m_pAddSpeed(NULL)
,m_pAddPotential(NULL)
,m_pAttackArrow(NULL)
,m_pDefenceArrow(NULL)
,m_pHpArrow(NULL)
,m_pSpeedArrow(NULL)
,m_pCurrentFight(NULL)
,m_pNextFight(NULL)
,m_pListView(NULL)
,m_pSettleView(NULL)
,m_pWarriorGradeBg(NULL)
,m_nextTalentName("")
{
    D_RESET_C_ARRAY(m_pColorGrade, WARRIOR_GRADE_NUM);
}


PromotionView::~PromotionView()
{
    removeAllChildren();
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
    
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pCurUpLevel);
    CC_SAFE_RELEASE(m_pToUpLevel);
	CC_SAFE_RELEASE(m_pTableNode);
    
    CC_SAFE_RELEASE(m_pCurProps);       //当前属性
    CC_SAFE_RELEASE(m_pToProps);        //提升后属性
    
    CC_SAFE_RELEASE(m_pCurItems);       //当前魂魄数
    
    CC_SAFE_RELEASE(m_pGrade);			//品阶
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pName);
    
    CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pCancelButton);
    CC_SAFE_RELEASE(m_pEffect);
    CC_SAFE_RELEASE(m_pEffectNode);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
    CC_SAFE_RELEASE(m_pWarriorGradeBg);

	CC_SAFE_RELEASE(m_pHp);
	CC_SAFE_RELEASE(m_pAttack);
	CC_SAFE_RELEASE(m_pDefence);
	//CC_SAFE_RELEASE(m_pSpeed);
    CC_SAFE_RELEASE(m_pPotential);
	CC_SAFE_RELEASE(m_pAddHp);
	CC_SAFE_RELEASE(m_pAddAttack);
	CC_SAFE_RELEASE(m_pAddDefence);
    CC_SAFE_RELEASE(m_pAddPotential);
	//CC_SAFE_RELEASE(m_pAddSpeed);
	CC_SAFE_RELEASE(m_pAttackArrow);
	CC_SAFE_RELEASE(m_pDefenceArrow);
	CC_SAFE_RELEASE(m_pHpArrow);
	CC_SAFE_RELEASE(m_pSpeedArrow);
	CC_SAFE_RELEASE(m_pCurrentFight);
	CC_SAFE_RELEASE(m_pNextFight);
	CC_SAFE_RELEASE(m_pListView);
	CC_SAFE_RELEASE(m_pSettleView);

	D_SAFE_RELEASE_C_ARRAY(m_pColorGrade, WARRIOR_GRADE_NUM);
}


bool PromotionView::init()//uint32_t wid)
{
	if(!DLayer::init())
	{
		return false;
	}
    
	m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_jinjie/PL_jinjie.ccbi");
    m_pConfirmButton->getButton()->setTarget(this, menu_selector(PromotionView::confirmButtonCallBack));
    m_pCancelButton->getButton()->setTarget(this, menu_selector(PromotionView::closeCallBack));

	m_pListView = PromotionItemListView::create();
	CC_SAFE_RETAIN(m_pListView);
	m_pTableNode->addChild(m_pListView);

	//预加载动画
	m_pEffect = ResourceManager::sharedResourceManager()->getUIAnimate("heroupgrade");
	m_pEffect->getAnimation()->addObserver(this, SPNotification_callfunc_selector(PromotionView::animateEnd), COMPLETE);
	CC_SAFE_RETAIN(m_pEffect);

	return true;
}



bool PromotionView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurUpLevel", CCLabelBMFont *, this->m_pCurUpLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pToUpLevel", CCLabelBMFont *, this->m_pToUpLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableNode", CCNode *, this->m_pTableNode);
   /* CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurProps", CCLabelBMFont *, this->m_pCurProps);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pToProps", CCLabelBMFont *, this->m_pToProps);*/
    /*CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurItems", CCLabelBMFont *, this->m_pCurItems);*/
	/*CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBox", ItemBox *, this->m_pItemBox);*/
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, this->m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCancelButton", DButton *, this->m_pCancelButton);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode *, this->m_pEffectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg*, m_pWarriorGradeBg);
    
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHp", CCLabelBMFont *, this->m_pHp);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttack", CCLabelBMFont *, this->m_pAttack);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefence", CCLabelBMFont *, this->m_pDefence);
	 //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeed", CCLabelBMFont *, this->m_pSpeed);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPotential", CCLabelBMFont *, this->m_pPotential);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddHp", CCLabelBMFont *, this->m_pAddHp);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddAttack", CCLabelBMFont *, this->m_pAddAttack);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddDefence", CCLabelBMFont *, this->m_pAddDefence);
	 //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddSpeed", CCLabelBMFont *, this->m_pAddSpeed);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddPotential", CCLabelBMFont *, this->m_pAddPotential);

	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttackArrow", CCSprite *, this->m_pAttackArrow);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefenceArrow", CCSprite *, this->m_pDefenceArrow);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHpArrow", CCSprite *, this->m_pHpArrow);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedArrow", CCSprite *, this->m_pSpeedArrow);

	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurrentFight", CCLabelBMFont *, this->m_pCurrentFight);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNextFight", CCLabelBMFont *, this->m_pNextFight);

	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade1", CCNode *, this->m_pColorGrade[0]);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade2", CCNode *, this->m_pColorGrade[1]);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade3", CCNode *, this->m_pColorGrade[2]);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade4", CCNode *, this->m_pColorGrade[3]);

    return false;
}

SEL_MenuHandler PromotionView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}


void PromotionView::setWarrior(warrior::WarriorCurInfo *pWarrior)
{
	CC_SAFE_RETAIN(pWarrior);
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
	m_pWarriorInfo = pWarrior;
	m_pListView->setWarrior(m_pWarriorInfo);
	binding();
}

void PromotionView::binding()
{
	if (m_pWarriorInfo)
	{
		BINDING_EXEC(m_pWarriorInfo,this,PromotionView::updatePromotion,D_EVENT_WARRIORINFO_UPDATE);
	}
}


void PromotionView::onEnter()
{
	binding();
	DLayer::onEnter();
}


void PromotionView::onExit()
{
	setWarrior(NULL);
	DLayer::onExit();
}


void PromotionView::updatePromotion()
{
	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("WarriorInfoError").c_str());
		return ;
	}
    
    m_pContainer->removeAllChildren();
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon(m_pWarriorInfo->getAvatarId(), 0.83f));
    
	m_pGrade->setColor(m_pWarriorModel->getWarriorColorByGrade(m_pWarriorInfo->getBaseInfo()->getRealGrade()));

	for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
		int color = (int)m_pWarriorInfo->getBaseInfo()->getColorGrade();
		m_pColorGrade[i]->setVisible( color == i+1);
	}


	m_pLevel->setString(CCString::createWithFormat("%i",m_pWarriorInfo->getWarriorLv())->getCString());
	m_pName->setString(m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
	
    m_pCurrentFight->setString(CCString::createWithFormat("%d", m_pWarriorInfo->computeWarriorForce())->getCString());
	m_pNextFight->setString(CCString::createWithFormat("%d", m_pWarriorInfo->computeNextUpLevelWarriorForce())->getCString());
    
	warrior::SoulInfo * soulInfo = m_pWarriorModel->getSoulInfoByWID(m_pWarriorInfo->getWid());
    //m_pCurItems->setString(CCString::createWithFormat("%d",soulInfo?soulInfo->getNum():0)->getCString());
    
   /* m_pItemBox->getContainer()->removeAllChildren();
    m_pItemBox->getContainer()->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon40P(m_pWarriorInfo->getAvatarId()));
    
    m_pItemBox->getGrade()->setColor(m_pWarriorModel->getWarriorColorByGrade(m_pWarriorInfo->getBaseInfo()->getRealGrade()));
    
    uint32_t needSoulNum = m_pWarriorModel->getWarriorUpSoul(m_pWarriorInfo);
    m_pItemBox->getCount()->setString(CCString::createWithFormat("X%d",needSoulNum)->getCString());
    m_pItemBox->getName()->setString(D_LOCAL_STRING("%sSoul",m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str()).c_str());*/
    
    m_pCurUpLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getWarriorUpLv())->getCString());
    //m_pCurProps->setString(CCString::createWithFormat("X%.1f%%",m_pWarriorInfo->getUpPercentCurrentLv())->getCString());
    
    if(!m_pWarriorInfo->checkPromoteLimit())
	{
        m_pToUpLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getWarriorUpLv()+1)->getCString());
        //m_pToProps->setString(CCString::createWithFormat("X%.1f%%",m_pWarriorInfo->getUpPercentNextLv())->getCString());
    }else
    {
        //m_pToProps->setString("");
    }
    
	uint32_t currentAttack = m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeAttak);
	uint32_t currentDefence = m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeDefence);
	uint32_t currentHp = m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeHp);
	uint32_t currentPotential = m_pWarriorInfo->getPotential();
	uint32_t nextAttack = m_pWarriorInfo->computeNextUpLevelWarriorProps(warrior::kPropTypeAttak);
	uint32_t nextDefence = m_pWarriorInfo->computeNextUpLevelWarriorProps(warrior::kPropTypeDefence);
	uint32_t nextHp = m_pWarriorInfo->computeNextUpLevelWarriorProps(warrior::kPropTypeHp);
	uint32_t nextPotential = m_pWarriorInfo->getWarriorNextUpLevelPotential();

	m_pAttack->setString(CCString::createWithFormat("%d" ,currentAttack)->getCString());
	m_pDefence->setString(CCString::createWithFormat("%d", currentDefence)->getCString());
	m_pHp->setString(CCString::createWithFormat("%d", currentHp)->getCString());
	//m_pSpeed->setString(CCString::createWithFormat("%d", currentSpeed)->getCString());
    m_pPotential->setString(CCString::createWithFormat("%d", currentPotential)->getCString());

	m_pAddAttack->setString(CCString::createWithFormat("%d", nextAttack)->getCString());
	m_pAddDefence->setString(CCString::createWithFormat("%d", nextDefence)->getCString());
	m_pAddHp->setString(CCString::createWithFormat("%d", nextHp)->getCString());
	//m_pAddSpeed->setString(CCString::createWithFormat("%d", nextSpeed)->getCString());
    m_pAddPotential->setString(CCString::createWithFormat("%d", nextPotential)->getCString());

	m_pAttackArrow->setVisible(nextAttack > currentAttack);
	m_pDefenceArrow->setVisible(nextDefence > currentDefence);
	m_pHpArrow->setVisible(nextHp > currentHp);
	m_pSpeedArrow->setVisible(nextPotential > currentPotential);

//	CCLog("attact:%d, defence:%d, hp:%d, speed:%d", 
//		m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeAttak),
//		m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeDefence),
//		m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeHp),
//		m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeSpeed));

    m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
    m_pWarriorGradeBg->setWarriorGrade(m_pWarriorInfo->getBaseInfo()->getColorGrade());
    
    m_pConfirmButton->getButton()->setEnabled(m_pWarriorInfo->isPromoteable() && !m_pWarriorInfo->checkPromoteLimit());
    
    if(m_pWarriorInfo->checkPromoteLimit())
    {
        closeCallBack(NULL);
    }
	
	//更新需要列表
	m_pListView->updateView();
}


void PromotionView::confirmButtonCallBack(CCObject * obj)
{
	WarriorUpErrorType errorType = DM_GET_WARRIOR_MODEL->canWarriorUpNow(m_pWarriorInfo);
	switch(errorType)
	{
	case kUpErrorNone:
		break;
	case warrior::kUpErrorCoinNotEnough:
		Tips_Alert(D_LOCAL_STRING("CoinNotEnoughGoMall").c_str());
		break;
	case warrior::kUpErrorItemNotEnough:
		Tips_Alert(D_LOCAL_STRING("$errorcode1406").c_str());
		break;
	case warrior::kUpErrorSoulNotEnough:
		Tips_Alert(D_LOCAL_STRING("NotEnoughSoul").c_str());
		break;
	case warrior::kUpErrorWarriorLevelLimit:
        {
			unsigned int needLevel = m_pWarriorInfo->getWarriorLevelForPromotion();
			//CCString* tipString = CCString::createWithFormat(D_LOCAL_STRING("PromotionLevelLimit").c_str(), needLevel);
			Tips_Alert(D_LOCAL_STRING("PromotionLevelLimit",needLevel).c_str());
        }
		break;
	case warrior::kUpErrorWarriorInfo:
		Tips_Alert(D_LOCAL_STRING("WarriorInfoError").c_str());
		break;
	case warrior::kUpErrorSoulInfo:
		Tips_Alert(D_LOCAL_STRING("SoulInfoError").c_str());
		break;
	default:
		Tips_Alert(D_LOCAL_STRING("unkonw error").c_str());
		break;
	}

	if (errorType != warrior::kUpErrorNone)
	{
		return;
	}
	
	if (m_pSettleView == NULL)
	{
		m_pSettleView = PromotionSettleView::create();
		CC_SAFE_RETAIN(m_pSettleView);
	}
	updateNextTalentName();
	m_pSettleView->setWarriorInfo(m_pWarriorInfo);

	HTTP_CLIENT->promoteWarrior(m_pWarriorInfo->getWid(),this,callfuncJson_selector(PromotionView::promotionCallBack));
}

void PromotionView::promotionCallBack(const Json::Value &requestData,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;

	//屏蔽按钮
	m_pConfirmButton->getButton()->setEnabled(false);
	m_pCancelButton->getButton()->setEnabled(false);

	if(m_pEffect==NULL)
    {
		//理论上不会走到这里 m_pEffect已经初始化
		m_pEffect = ResourceManager::sharedResourceManager()->getUIAnimate("heroupgrade");
        m_pEffect->getAnimation()->addObserver(this, SPNotification_callfunc_selector(PromotionView::animateEnd), COMPLETE);
        CC_SAFE_RETAIN(m_pEffect);
    }
    if(m_pEffect)
    {
        //m_pEffect->removeFromParent();
        //m_pEffect->getAnimation()->stop();
        m_pEffect->getAnimation()->playByIndex(0 , -1 , -1 , true);
        m_pEffectNode->addChild(m_pEffect);
		//m_pEffect->getAnimation()->addObserver(this, SPNotification_callfunc_selector(PromotionView::animateEnd), COMPLETE);
		//runAction(CCSequence::create(CCDelayTime::create(4.0f) , CCCallFunc::create(this, callfunc_selector(PromotionView::animateEnd)) , NULL));
    }
    //CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, SFX_PROMOTION);
	DM_GET_SOUND_MODEL->playCommonEffectSound(SFX_PROMOTION, 0);
	/*if (m_pSettleView)
	{
		Game::sharedGame()->openPopup(m_pSettleView);
	}*/
}

void PromotionView::animateEnd(sp::Armature * target, const char* event,const char* data)
{
	DM_GET_SOUND_MODEL->playCommonEffectSound(SFX_PROMOTION, 1);
	//恢复按钮
	m_pConfirmButton->getButton()->setEnabled(true);
	m_pCancelButton->getButton()->setEnabled(true);

    //if(target) target->removeFromParent();
	if(m_pEffect){
		m_pEffect->getAnimation()->stop();
		m_pEffect->removeFromParent();
	} 
	
	if (m_pSettleView)
	{
		Game::sharedGame()->openPopup(m_pSettleView);
		if (m_nextTalentName.length() > 0)
		{
			//调整tips位置，不要遮盖到动画
            std::string tip(CCString::createWithFormat(D_LOCAL_STRING("TalentOpenTip").c_str())->getCString());
            tip.append(m_nextTalentName);
            //CCLOG("%s", D_LOCAL_STRING("TalentOpenTip").c_str());
            //CCLOG("%s", m_nextTalentName.c_str());
            //CCLOG("%s", tip.c_str());
			TIPSMANAGER->append(tip, cocos2d::ccc3(255,255,255) , 150.0f);
			//Tips_Alert( CCString::createWithFormat(D_LOCAL_STRING("TalentOpenTip").c_str(), m_nextTalentName.c_str())->getCString());
		}
	}
}

void PromotionView::closeCallBack(cocos2d::CCObject * obj)
{
    if(m_pDelegate) m_pDelegate->closeInfoView(kPromotionView);
    else removeFromParent();
}

void PromotionView::updateNextTalentName()
{
	if (m_pWarriorInfo == NULL)
	{
		return;
	}
	
	CCDictionary* talentsInfo = m_pWarriorModel->getWarriorTalentsDic(m_pWarriorInfo->getWid());
	if (talentsInfo == NULL || talentsInfo->count() == 0)
	{
		return;
	}
	
	CCInteger* talentId = dynamic_cast<CCInteger*>(talentsInfo->objectForKey(m_pWarriorInfo->getWarriorUpLv() + 1));
	if (talentId)
	{
		warrior::TalentInfo* info = m_pWarriorModel->getTalentInfoById(talentId->getValue());
			if (info)
			{
				m_nextTalentName.assign(info->name);
			}
	}
}


/////////////////////////////////////////////////////

PromotionItemListView::PromotionItemListView() :
m_pTableView(NULL)
,m_pCellData(NULL)
,m_pWarriorInfo(NULL)
,m_uTableCellsCount(0)
{

}

PromotionItemListView::~PromotionItemListView()
{
	CC_SAFE_RELEASE(m_pCellData);
	CC_SAFE_RELEASE(m_pTableView);
}

bool PromotionItemListView::init()
{
	if (!DLayer::init())
	{
		return false;
	}

	m_pCellData = readCCData("PL_ui/ccb/pl_jinjie/PL_jinjie_cell");
	CC_SAFE_RETAIN(m_pCellData);

	m_pTableView = DTableView::create(this, CCSizeMake(500, 134));
	m_pTableView->setDirection(kCCScrollViewDirectionHorizontal);
	m_pTableView->setDelegate(this);
	CC_SAFE_RETAIN(m_pTableView);
	addChild(m_pTableView);

	return true;
}

void PromotionItemListView::updateView()
{
	if (m_pTableView)
	{
        computeTableCellsCount();
		m_pTableView->reloadData();
	}
	
}

void PromotionItemListView::computeTableCellsCount()
{
    m_uTableCellsCount = 0;
	if (m_pWarriorInfo)
	{
		WarriorModel  *pWarriorModel = DM_GET_WARRIOR_MODEL;
		//warrior::SoulInfo * soulInfo = pWarriorModel->getSoulInfoByWID(m_pWarriorInfo->getWid());
		uint32_t needSoulNum = pWarriorModel->getWarriorUpSoul(m_pWarriorInfo);
        
		if(needSoulNum>0)
		{
			m_uTableCellsCount++;
		}
        
		warrior::WarriorUpSoul* upinfo = pWarriorModel->getWarriorUpInfo(m_pWarriorInfo);
		uint32_t needItemNum = upinfo->getUpItemCount(m_pWarriorInfo->getBaseInfo()->getColorGrade());
		if(needItemNum>0)
		{
			m_uTableCellsCount++;
		}
        
        if (upinfo->getUpCoinCount(m_pWarriorInfo->getBaseInfo()->getColorGrade()) > 0) {
            m_uTableCellsCount++;
        }
	}
}

void PromotionItemListView::tableCellTouched( cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell )
{

}

cocos2d::CCSize PromotionItemListView::tableCellSizeForIndex( cocos2d::extension::CCTableView *table, unsigned int idx )
{
	return CCSizeMake(105, 132);
}

cocos2d::extension::CCTableViewCell* PromotionItemListView::tableCellAtIndex( cocos2d::extension::CCTableView *table, uint32_t idx )
{
	PromotionListItemCell* cell = static_cast<PromotionListItemCell*>(table->dequeueCell());
	if (!cell)
	{
		cell = PromotionListItemCell::create(m_pCellData);
	}

	if (m_pWarriorInfo)
	{
		//if(idx == 0)
		//{
		//	//第一格 魂魄
		//	WarriorModel  *pWarriorModel = DM_GET_WARRIOR_MODEL;
		//	warrior::SoulInfo * soulInfo = pWarriorModel->getSoulInfoByWID(m_pWarriorInfo->getWid());
		//	uint32_t needSoulNum = pWarriorModel->getWarriorUpSoul(m_pWarriorInfo);
		//	uint32_t currentCount = soulInfo?soulInfo->getNum():0;
		//	cell->setItemCountString(CCString::createWithFormat("%d/%d", currentCount, needSoulNum));
		//	cell->setItemName(D_LOCAL_STRING("%sSoul",m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str()));
		//	cell->setIconSprite(ResourceManager::sharedResourceManager()->getWarriorHeadIcon(m_pWarriorInfo->getAvatarId(), CCSizeMake(88, 88)));
		//}
		//else if(idx == 1)
		//{
		//	//第二格 进阶丹
		//	WarriorModel  *pWarriorModel = DM_GET_WARRIOR_MODEL;
		//	warrior::WarriorUpSoul* upinfo = pWarriorModel->getWarriorUpInfo(m_pWarriorInfo);

		//	uint32_t itemid = D_CONFIG_UINT_FOR_KEY(WARRIORUP_ITEMID);
		//	item::ItemBase* itembase = DM_GET_ITEM_MODEL->getItemBaseByID(itemid);
		//	item::ItemInfo* iteminfo = DM_GET_ITEM_MODEL->getItemInfoByID(itemid);

		//	uint32_t currentCount = iteminfo?iteminfo->getNum():0;
		//	uint32_t needItemNum = upinfo->getUpItemCount(m_pWarriorInfo->getBaseInfo()->getColorGrade());

		//	cell->setItemCountString(CCString::createWithFormat("%d/%d", currentCount, needItemNum));
		//	cell->setItemName(itembase->getItemName());
		//	cell->setIconSprite(ResourceManager::sharedResourceManager()->getItemIcon40P(itemid));
		//}

		WarriorModel  *pWarriorModel = DM_GET_WARRIOR_MODEL;
		uint32_t needSoulNum = pWarriorModel->getWarriorUpSoul(m_pWarriorInfo);
		warrior::WarriorUpSoul* upinfo = pWarriorModel->getWarriorUpInfo(m_pWarriorInfo);
		uint32_t needItemNum = upinfo->getUpItemCount(m_pWarriorInfo->getBaseInfo()->getColorGrade());
        
        if (idx == m_uTableCellsCount - 1) {
            //最后一格 铜币
            int coinCount = upinfo->getUpCoinCount(m_pWarriorInfo->getBaseInfo()->getColorGrade());
            cell->setItemCountString(CCString::createWithFormat("%d", coinCount));
            uint32_t itemid = D_CONFIG_UINT_FOR_KEY(ITEM_COIN);
            item::ItemBase* itembase = DM_GET_ITEM_MODEL->getItemBaseByID(itemid);
            cell->setItemName(itembase->getItemName());
            cell->setIconSprite(ResourceManager::sharedResourceManager()->getItemIcon40P(itemid));
        }
		else if(needSoulNum>0 && idx == 0)
		{
			//第一格 若需求魂魄
			warrior::SoulInfo * soulInfo = pWarriorModel->getSoulInfoByWID(m_pWarriorInfo->getWid());
			uint32_t currentCount = soulInfo?soulInfo->getNum():0;
			cell->setItemCountString(CCString::createWithFormat("%d/%d", currentCount, needSoulNum));
			cell->setItemName(D_LOCAL_STRING("%sSoul",m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str()));
			cell->setIconSprite(ResourceManager::sharedResourceManager()->getWarriorHeadIcon(m_pWarriorInfo->getAvatarId(), CCSizeMake(88, 88)));
		}
		else if(needItemNum>0)
		{
			//进阶丹
			uint32_t itemid = D_CONFIG_UINT_FOR_KEY(WARRIORUP_ITEMID);
			item::ItemBase* itembase = DM_GET_ITEM_MODEL->getItemBaseByID(itemid);
			item::ItemInfo* iteminfo = DM_GET_ITEM_MODEL->getItemInfoByID(itemid);

			uint32_t currentCount = iteminfo?iteminfo->getNum():0;

			cell->setItemCountString(CCString::createWithFormat("%d/%d", currentCount, needItemNum));
			cell->setItemName(itembase->getItemName());
			cell->setIconSprite(ResourceManager::sharedResourceManager()->getItemIcon40P(itemid));
		}
	}
	
  return cell;
}

uint32_t PromotionItemListView::numberOfCellsInTableView( cocos2d::extension::CCTableView *table )
{
    return m_uTableCellsCount;
}

void PromotionItemListView::setWarrior( warrior::WarriorCurInfo *pWarriorInfo )
{
	m_pWarriorInfo = pWarriorInfo;
	updateView();
}



PromotionListItemCell::PromotionListItemCell()
	:m_pItemBox(NULL)

{

}

PromotionListItemCell::~PromotionListItemCell()
{
	CC_SAFE_RELEASE(m_pItemBox);
}

PromotionListItemCell* PromotionListItemCell::create( cocos2d::extension::CCData *pData )
{
	PromotionListItemCell* pRet = new PromotionListItemCell();
	if (pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}
	
	CC_SAFE_RELEASE_NULL(pRet);
	return pRet;
}

bool PromotionListItemCell::init( cocos2d::extension::CCData *pData )
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	CCB_READER_DATA_AND_ADDCHILD(pData);

	updateView();
	return true;
}

bool PromotionListItemCell::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBox", ItemBox *, this->m_pItemBox);
	return false;
}

void PromotionListItemCell::updateView()
{

}

void PromotionListItemCell::setIconSprite( cocos2d::CCSprite* icon )
{
	if (icon == NULL || m_pItemBox == NULL)
	{
		if (icon)
		{
			CC_SAFE_RELEASE(icon);
		}
		return;
	}

	if (m_pItemBox->getContainer() == NULL)
	{
		return;
	}
	
	m_pItemBox->getContainer()->removeAllChildren();
	m_pItemBox->getContainer()->addChild(icon);
	
}

void PromotionListItemCell::setItemCountString( CCString* countString )
{
	if (m_pItemBox == NULL || m_pItemBox->getCount() == NULL)
	{
		return;
	}
	
	m_pItemBox->getCount()->setString(countString->getCString());
}

void PromotionListItemCell::setItemName( std::string nameString )
{
	if (m_pItemBox == NULL || m_pItemBox->getName() == NULL)
	{
		return;
	}
	
	m_pItemBox->getName()->setString(nameString.c_str());
}





PromotionSettleView::PromotionSettleView()
	:
m_pContainer(NULL)
,m_pRealUpLevel(NULL)
,m_pCurUpLevel(NULL)
,m_pToUpLevel(NULL)
,m_pGrade(NULL)
,m_pLevel(NULL)
,m_pName(NULL)
,m_pHp(NULL)
,m_pAttack(NULL)
,m_pDefence(NULL)
//,m_pSpeed(NULL)
,m_pPotential(NULL)
,m_pAddHp(NULL)
,m_pAddAttack(NULL)
,m_pAddDefence(NULL)
//,m_pAddSpeed(NULL)
,m_pAddPotential(NULL)
,m_pAttackArrow(NULL)
,m_pDefenceArrow(NULL)
,m_pHpArrow(NULL)
,m_pSpeedArrow(NULL)
,m_pWarriorTypeMark(NULL)
,m_pWarriorGradeBg(NULL)
,m_pFadeInNode(NULL)
{
	D_RESET_C_ARRAY(m_pColorGrade, WARRIOR_GRADE_NUM);
}

PromotionSettleView::~PromotionSettleView()
{
	CC_SAFE_RELEASE_NULL(m_pContainer);
	CC_SAFE_RELEASE_NULL(m_pRealUpLevel);
	CC_SAFE_RELEASE_NULL(m_pCurUpLevel);
	CC_SAFE_RELEASE_NULL(m_pToUpLevel);
	CC_SAFE_RELEASE_NULL(m_pGrade);
	CC_SAFE_RELEASE_NULL(m_pLevel);
	CC_SAFE_RELEASE_NULL(m_pName);
	CC_SAFE_RELEASE_NULL(m_pHp);
	CC_SAFE_RELEASE_NULL(m_pAttack);
	CC_SAFE_RELEASE_NULL(m_pDefence);
	//CC_SAFE_RELEASE_NULL(m_pSpeed);
    CC_SAFE_RELEASE_NULL(m_pPotential);
	CC_SAFE_RELEASE_NULL(m_pAddHp);
	CC_SAFE_RELEASE_NULL(m_pAddAttack);
	CC_SAFE_RELEASE_NULL(m_pAddDefence);
	//CC_SAFE_RELEASE_NULL(m_pAddSpeed);
    CC_SAFE_RELEASE_NULL(m_pAddPotential);
	CC_SAFE_RELEASE_NULL(m_pAttackArrow);
	CC_SAFE_RELEASE_NULL(m_pDefenceArrow);
	CC_SAFE_RELEASE_NULL(m_pHpArrow);
	CC_SAFE_RELEASE_NULL(m_pSpeedArrow);
	CC_SAFE_RELEASE_NULL(m_pWarriorTypeMark);
    CC_SAFE_RELEASE_NULL(m_pWarriorGradeBg);
	CC_SAFE_RELEASE_NULL(m_pFadeInNode);

	D_SAFE_RELEASE_C_ARRAY(m_pColorGrade, WARRIOR_GRADE_NUM);
}

bool PromotionSettleView::init()
{
	setTouchEnabled(true);
	if(!DPopup::init())
	{
		return false;
	}

	 CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_jinjie/PL_jinjiechenggong.ccbi");
	return true;
}

void PromotionSettleView::onEnter()
{
	DPopup::onEnter();

	//加载入场动画
	//sp::Armature* m_pFadeInEffect = ResourceManager::sharedResourceManager()->getUIAnimate("upgradebg");
	//m_pFadeInEffect->getAnimation()->playByIndex(0);
	//m_pFadeInNode->addChild(m_pFadeInEffect);
}

void PromotionSettleView::onExit()
{
	DPopup::onExit();
}

bool PromotionSettleView::onAssignCCBMemberVariable( CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCLabelBMFont *, this->m_pRealUpLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurUpLevel", CCLabelBMFont *, this->m_pCurUpLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pToUpLevel", CCLabelBMFont *, this->m_pToUpLevel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);

    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode *, this->m_pEffectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg*, this->m_pWarriorGradeBg);
    
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHp", CCLabelBMFont *, this->m_pHp);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttack", CCLabelBMFont *, this->m_pAttack);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefence", CCLabelBMFont *, this->m_pDefence);
	 //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeed", CCLabelBMFont *, this->m_pSpeed);
     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPotential", CCLabelBMFont *, this->m_pPotential);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddHp", CCLabelBMFont *, this->m_pAddHp);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddAttack", CCLabelBMFont *, this->m_pAddAttack);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddDefence", CCLabelBMFont *, this->m_pAddDefence);
	 //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddSpeed", CCLabelBMFont *, this->m_pAddSpeed);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddPotential", CCLabelBMFont *, this->m_pAddPotential);

	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttackArrow", CCSprite *, this->m_pAttackArrow);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefenceArrow", CCSprite *, this->m_pDefenceArrow);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHpArrow", CCSprite *, this->m_pHpArrow);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedArrow", CCSprite *, this->m_pSpeedArrow);

	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFadeInNode", CCNode *, this->m_pFadeInNode);

	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade1", CCNode *, this->m_pColorGrade[0]);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade2", CCNode *, this->m_pColorGrade[1]);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade3", CCNode *, this->m_pColorGrade[2]);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade4", CCNode *, this->m_pColorGrade[3]);

	return false;
}

void PromotionSettleView::closeButtonCallBack( cocos2d::CCObject* obj )
{
	removeFromParentAndCleanup(true);
}

void PromotionSettleView::updateWarriorSettle()
{
	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("WarriorInfoError").c_str());
		return ;
	}
    
	m_pContainer->removeAllChildren();
	//m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon(m_pWarriorInfo->getAvatarId(), 0.83f));
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getUnitAnimate2XAsync(m_pWarriorInfo->getAvatarId()));

	WarriorModel* pWarriorModel = DM_GET_WARRIOR_MODEL;
	m_pGrade->setColor(pWarriorModel->getWarriorColorByGrade(m_pWarriorInfo->getBaseInfo()->getRealGrade()));

	for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
		int color = (int)m_pWarriorInfo->getBaseInfo()->getColorGrade();
		m_pColorGrade[i]->setVisible( color == i+1);
	}

	m_pLevel->setString(CCString::createWithFormat("%i",m_pWarriorInfo->getWarriorLv())->getCString());
	m_pName->setString(m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
  
    m_pCurUpLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getWarriorUpLv())->getCString());
  

    if(!m_pWarriorInfo->checkPromoteLimit())
	{
        m_pToUpLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getWarriorUpLv()+1)->getCString());
		m_pRealUpLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getWarriorUpLv() + 1)->getCString());
    }
	else
	{
		m_pToUpLevel->setString("");
		m_pRealUpLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getWarriorUpLv())->getCString());
	}
    
	uint32_t currentAttack = m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeAttak);
	uint32_t currentDefence = m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeDefence);
	uint32_t currentHp = m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeHp);
	//uint32_t currentSpeed = m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeSpeed);
    uint32_t currentPotential = m_pWarriorInfo->getPotential();
	uint32_t nextAttack = m_pWarriorInfo->computeNextUpLevelWarriorProps(warrior::kPropTypeAttak);
	uint32_t nextDefence = m_pWarriorInfo->computeNextUpLevelWarriorProps(warrior::kPropTypeDefence);
	uint32_t nextHp = m_pWarriorInfo->computeNextUpLevelWarriorProps(warrior::kPropTypeHp);
	//uint32_t nextSpeed = m_pWarriorInfo->computeNextUpLevelWarriorProps(warrior::kPropTypeSpeed);
    uint32_t nextPotential = m_pWarriorInfo->getWarriorNextUpLevelPotential();
    
	m_pAttack->setString(CCString::createWithFormat("%d" ,currentAttack)->getCString());
	m_pDefence->setString(CCString::createWithFormat("%d", currentDefence)->getCString());
	m_pHp->setString(CCString::createWithFormat("%d", currentHp)->getCString());
	//m_pSpeed->setString(CCString::createWithFormat("%d", currentSpeed)->getCString());
     m_pPotential->setString(CCString::createWithFormat("%d", currentPotential)->getCString());

	m_pAddAttack->setString(CCString::createWithFormat("%d", nextAttack)->getCString());
	m_pAddDefence->setString(CCString::createWithFormat("%d", nextDefence)->getCString());
	m_pAddHp->setString(CCString::createWithFormat("%d", nextHp)->getCString());
	//m_pAddSpeed->setString(CCString::createWithFormat("%d", nextSpeed)->getCString());
    m_pAddPotential->setString(CCString::createWithFormat("%d", nextPotential)->getCString());

	m_pAttackArrow->setVisible(nextAttack > currentAttack);
	m_pDefenceArrow->setVisible(nextDefence > currentDefence);
	m_pHpArrow->setVisible(nextHp > currentHp);
	m_pSpeedArrow->setVisible(nextPotential > currentPotential);

    m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
    m_pWarriorGradeBg->setWarriorGrade(m_pWarriorInfo->getBaseInfo()->getColorGrade());
}

bool PromotionSettleView::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	return true;
}

void PromotionSettleView::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	closeButtonCallBack(NULL);
}

void PromotionSettleView::setWarriorInfo(warrior::WarriorCurInfo *pWarriorInfo)
{
	m_pWarriorInfo = pWarriorInfo;
	updateWarriorSettle();
}
