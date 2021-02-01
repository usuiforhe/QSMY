//  WarriorDevelop.cpp
//
//  Created by cll on 13-5-22
//  Copyright (c) 2013, thedream. All rights reserved.
//

#include "WarriorDevelop.h"
#include "Warrior.h"
#include "../../../managers/ResourceManager.h"
#include "../../../managers/GameManager.h"
#include "../../../model/UserModel.h"
#include "../../../managers/TipsManager.h"
#include "../user/UserInfo.h"

USING_NS_CC;

using namespace  warrior;

enum DevelopViewButtonTag
{
	kDVBTReturn = 87653,
	kDVBTTen,
	kDVBTOne,
	kDVBTDevelop,
	kDVBTConfirm,
	kDVBTGiveUp,
	kDVBTMax,
};

#define IF_NOT_NULL_THEN_SETVISIBLE(node, visible) if(NULL !=(node)) (node)->setVisible((visible));
#define WARRIOR_DEV_SOUND_ACTION "warrior_dev"

WarriorDevelopView::WarriorDevelopView()
:m_pDelegate(NULL)
,m_pWarriorModel(NULL)
,m_pWarriorGradeBg(NULL)
,m_pContainer(NULL)
,m_pUpLevel(NULL)
,m_pAttack(NULL)
,m_pDefence(NULL)
,m_pSpeed(NULL)
,m_pHp(NULL)
,m_pAddedAttack(NULL)
,m_pAddedDefence(NULL)
,m_pAddedSpeed(NULL)
,m_pAddedHp(NULL)
,m_pAttackUpArrow(NULL)
,m_pAttackDownArrow(NULL)
,m_pDefenceUpArrow(NULL)
,m_pDefenceDownArrow(NULL)
,m_pSpeedUpArrow(NULL)
,m_pSpeedDownArrow(NULL)
,m_pHpUpArrow(NULL)
,m_pHpDownArrow(NULL)
,m_pGrade(NULL)
,m_pLevel(NULL)
,m_pName(NULL)
,m_pPot(NULL)
,m_pPill(NULL)
,m_pExpendPot(NULL)
,m_pExpendPill(NULL)
//,m_pPointsCost(NULL)
,m_pWarriorInfo(NULL)
,m_pWarriorTypeMark(NULL)
,m_DevType(PotBuff::kTypeNormal)
,m_pFight(NULL)
,m_pEffectNode(NULL)
,m_isWiattingForConfirm(false)
,m_pCloseButton(NULL)
,m_pLeftButton(NULL)
,m_pMiddleButton(NULL)
,m_pRightButton(NULL)
{
    for (uint8_t i =0; i<PotBuff::kTypeSpecial; ++i)
    {
        m_pSelectButtons[i] = NULL;
    }

	for (unsigned int i = kDVBTReturn; i < kDVBTMax; i++)
	{
		m_buttonTextMap[i] = NULL;
	}
	
	D_RESET_C_ARRAY(m_pColorGrade, WARRIOR_GRADE_NUM);
}

WarriorDevelopView::~WarriorDevelopView()
{
    removeAllChildren();
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
	UNBINDING_ALL(m_pWarriorModel,this);
    
    
    CC_SAFE_RELEASE(m_pContainer);      //弟子头像
	CC_SAFE_RELEASE(m_pUpLevel);		//突破等级
	CC_SAFE_RELEASE(m_pAttack);
    CC_SAFE_RELEASE(m_pWarriorGradeBg);
	CC_SAFE_RELEASE(m_pDefence);
	CC_SAFE_RELEASE(m_pSpeed);
	CC_SAFE_RELEASE(m_pHp);
	CC_SAFE_RELEASE(m_pAddedAttack);
	CC_SAFE_RELEASE(m_pAddedDefence);
	CC_SAFE_RELEASE(m_pAddedSpeed);
	CC_SAFE_RELEASE(m_pAddedHp);

	CC_SAFE_RELEASE(m_pAttackUpArrow);
	CC_SAFE_RELEASE(m_pAttackDownArrow);
	CC_SAFE_RELEASE(m_pDefenceUpArrow);
	CC_SAFE_RELEASE(m_pDefenceDownArrow);
	CC_SAFE_RELEASE(m_pSpeedUpArrow);
	CC_SAFE_RELEASE(m_pSpeedDownArrow);
	CC_SAFE_RELEASE(m_pHpUpArrow);
	CC_SAFE_RELEASE(m_pHpDownArrow);

	CC_SAFE_RELEASE(m_pGrade);			//品阶
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pPot);            //潜力
	CC_SAFE_RELEASE(m_pPill);           //培养丹
	CC_SAFE_RELEASE(m_pExpendPot); 
	CC_SAFE_RELEASE(m_pExpendPill); 
	//CC_SAFE_RELEASE(m_pPointsCost);		//消耗点券
	CC_SAFE_RELEASE(m_pFight);
	CC_SAFE_RELEASE(m_pEffectNode);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
    CC_SAFE_RELEASE(m_pWarriorModel);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pLeftButton);
	CC_SAFE_RELEASE(m_pMiddleButton);
	CC_SAFE_RELEASE(m_pRightButton);

	for (unsigned int i = kDVBTReturn; i < kDVBTMax; i++)
	{
		CC_SAFE_RELEASE(m_buttonTextMap[i]);
	}
	
    for (uint8_t i =0; i<warrior::PotBuff::kTypeSpecial; ++i)
    {
        CC_SAFE_RELEASE(m_pSelectButtons[i]);
    }

	D_SAFE_RELEASE_C_ARRAY(m_pColorGrade, WARRIOR_GRADE_NUM);
}

bool WarriorDevelopView::init()
{
    
	if(!DLayer::init())
	{
		return false;
	}
    
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    CC_SAFE_RETAIN(m_pWarriorModel);
    //CCB_READER_AND_ADDCHILD("ui/ccb/warrior/warriorDevelop");
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_peiyang/PL_peiyang");

	m_pLeftButton->getButton()->setTarget(this, menu_selector(WarriorDevelopView::BottomButtonsCallBack));
	m_pMiddleButton->getButton()->setTarget(this, menu_selector(WarriorDevelopView::BottomButtonsCallBack));
	m_pRightButton->getButton()->setTarget(this, menu_selector(WarriorDevelopView::BottomButtonsCallBack));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(WarriorDevelopView::closeCallBack));   
	changeDevelopButtonState(true);
    updateSelectButtons();
	setDevelopResultInfoVisible(false);
	return true;
}


bool WarriorDevelopView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode *, this->m_pEffectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg*, this->m_pWarriorGradeBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCLabelBMFont *, this->m_pUpLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttack", CCLabelTTF *, this->m_pAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefence", CCLabelTTF *, this->m_pDefence);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeed", CCLabelTTF *, this->m_pSpeed);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHp", CCLabelTTF *, this->m_pHp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddedAttack", CCLabelTTF *, this->m_pAddedAttack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddedDefence", CCLabelTTF *, this->m_pAddedDefence);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddedSpeed", CCLabelTTF *, this->m_pAddedSpeed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddedHp", CCLabelTTF *, this->m_pAddedHp);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttackUpArrow", CCSprite *, this->m_pAttackUpArrow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttackDownArrow", CCSprite *, this->m_pAttackDownArrow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefenceUpArrow", CCSprite *, this->m_pDefenceUpArrow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefenceDownArrow", CCSprite *, this->m_pDefenceDownArrow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedUpArrow", CCSprite *, this->m_pSpeedUpArrow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedDownArrow", CCSprite *, this->m_pSpeedDownArrow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHpUpArrow", CCSprite *, this->m_pHpUpArrow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHpDownArrow", CCSprite *, this->m_pHpDownArrow);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCLabelBMFont *, this->m_pFight);
	
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPot", CCLabelTTF *, this->m_pPot);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPill", CCLabelTTF *, this->m_pPill);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpendPill", CCLabelTTF *, this->m_pExpendPill);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpendPot", CCLabelTTF *, this->m_pExpendPot);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPointsCost", CCLabelBMFont *, this->m_pPointsCost);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLeftButton", DButton *, this->m_pLeftButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMiddleButton", DButton *, this->m_pMiddleButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRightButton", DButton *, this->m_pRightButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton*, this->m_pCloseButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectButton1", DButton *, this->m_pSelectButtons[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectButton2", DButton *, this->m_pSelectButtons[1]);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectButton3", DButton *, this->m_pSelectButtons[2]);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectButton4", DButton *, this->m_pSelectButtons[3]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
   
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "TextReturn", CCSprite *, this->m_buttonTextMap[kDVBTReturn]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "TextTen", CCSprite *, this->m_buttonTextMap[kDVBTTen]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "TextOne", CCSprite *, this->m_buttonTextMap[kDVBTOne]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "TextDevelop", CCSprite *, this->m_buttonTextMap[kDVBTDevelop]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "TextConfirm", CCSprite *, this->m_buttonTextMap[kDVBTConfirm]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "TextGiveUp", CCSprite *, this->m_buttonTextMap[kDVBTGiveUp]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade1", CCNode *, this->m_pColorGrade[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade2", CCNode *, this->m_pColorGrade[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade3", CCNode *, this->m_pColorGrade[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pColorGrade4", CCNode *, this->m_pColorGrade[3]);

    return false;
}

SEL_MenuHandler WarriorDevelopView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", WarriorDevelopView::closeCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "selectCallBack", WarriorDevelopView::selectCallBack);
    return NULL;
}


void WarriorDevelopView::setWarrior(warrior::WarriorCurInfo *pWarrior)
{
	if (pWarrior == m_pWarriorInfo) return;
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
	CC_SAFE_RETAIN(pWarrior);
	m_pWarriorInfo = pWarrior;
	binding();

	//如果有培养buff未处理，切换到结算界面
	warrior::PotBuff* pPotBuff = DM_GET_WARRIOR_MODEL->getWarriorPotBuff(m_pWarriorInfo);
	if(pPotBuff)
	{
		m_isWiattingForConfirm = true;
		changeDevelopButtonState(false);
	}
	else
	{
		setDevelopResultInfoVisible(false);
		m_isWiattingForConfirm = false;
		changeDevelopButtonState(true);
	}
}

void WarriorDevelopView::binding()
{
	BINDING_EXEC(m_pWarriorInfo,this,WarriorDevelopView::updateWarriorDevelop,D_EVENT_WARRIORINFO_UPDATE);
}


void WarriorDevelopView::onEnter()
{
	binding();
	DLayer::onEnter();

	warrior::PotBuff* pPotBuff = DM_GET_WARRIOR_MODEL->getWarriorPotBuff(m_pWarriorInfo);
	//如果有培养buff未处理，切换到结算界面
	if (pPotBuff)
	{
		m_isWiattingForConfirm = true;
		updateDevelopResult();
	}
	else
	{
		//初始化普通培养
		m_DevType = PotBuff::kTypeNormal;
		updateSelectButtons();
	}

	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	UserInfoView::sharedUserInfoView()->setDisplayType(user::kUserInfoSimple);
	addChild(UserInfoView::sharedUserInfoView(), 100);	//添加用户基本信息
}



void WarriorDevelopView::setDevelopResultInfoVisible(bool visible)
{
	IF_NOT_NULL_THEN_SETVISIBLE(m_pAddedAttack, visible);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pAddedDefence, visible);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pAddedSpeed, visible);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pAddedHp, visible);

	IF_NOT_NULL_THEN_SETVISIBLE(m_pAttackUpArrow, visible);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pAttackDownArrow, visible);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pDefenceUpArrow, visible);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pDefenceDownArrow, visible);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pSpeedUpArrow, visible);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pSpeedDownArrow, visible);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pHpUpArrow, visible);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pHpDownArrow, visible);

	IF_NOT_NULL_THEN_SETVISIBLE(m_pExpendPot, visible);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pExpendPill, visible);
}

void WarriorDevelopView::onExit()
{
	UNBINDING_ALL(m_pWarriorInfo,this);
	DLayer::onExit();
}

void WarriorDevelopView::submitCallback(const Json::Value &requestData,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
	//m_pDelegate->showInfoView(kDevSettleView, m_pWarriorInfo);

	m_isWiattingForConfirm = true;
	updateDevelopResult();
}

void WarriorDevelopView::updateDevelopResult()
{
	setDevelopResultInfoVisible(true);

	warrior::PotBuff * pPotBuff = m_pWarriorModel->getWarriorPotBuff(m_pWarriorInfo);
	uint32_t num = pPotBuff->type>warrior::PotBuff::kTypeSpecial?K_WARRIOR_DEV_ONCE_MAX:1;
	//item::ItemInfo * itemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(PEIYANG_DAN_ID);
	m_pExpendPot->setString(CCString::createWithFormat("- %d",pPotBuff->cost)->getCString());
	m_pExpendPill->setString(CCString::createWithFormat("- %d",num)->getCString());
	//m_pPill->setString(CCString::createWithFormat("%d",itemInfo?itemInfo->getNum()+num:num)->getCString());
	m_pAddedAttack->setString(CCString::createWithFormat("%s%d",pPotBuff->att >= 0 ? "+" : "", pPotBuff->att)->getCString());
	m_pAddedAttack->setColor(pPotBuff->att < 0 ? ccRED : ccGREEN);
	m_pAddedDefence->setString(CCString::createWithFormat("%s%d",pPotBuff->def >= 0 ? "+" : "", pPotBuff->def)->getCString());
	m_pAddedDefence->setColor(pPotBuff->def < 0 ? ccRED : ccGREEN);

	m_pAddedSpeed->setString(CCString::createWithFormat("%s%d",0 >= 0 ? "+" : "", 0)->getCString());
	m_pAddedSpeed->setColor(ccGREEN);
	m_pAddedHp->setString(CCString::createWithFormat("%s%d", pPotBuff->hp >= 0 ? "+" : "", pPotBuff->hp)->getCString());
	m_pAddedHp->setColor(pPotBuff->hp < 0 ? ccRED : ccGREEN);

	IF_NOT_NULL_THEN_SETVISIBLE(m_pAttackUpArrow,  pPotBuff->att > 0);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pAttackDownArrow, pPotBuff->att < 0);

	IF_NOT_NULL_THEN_SETVISIBLE(m_pDefenceUpArrow, pPotBuff->def > 0);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pDefenceDownArrow, pPotBuff->def < 0);

	IF_NOT_NULL_THEN_SETVISIBLE(m_pHpUpArrow, pPotBuff->hp > 0);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pHpDownArrow, pPotBuff->hp < 0);

	IF_NOT_NULL_THEN_SETVISIBLE(m_pSpeedUpArrow, false);
	IF_NOT_NULL_THEN_SETVISIBLE(m_pSpeedDownArrow, false);
}

void WarriorDevelopView::closeCallBack(CCObject * obj)
{
    if(m_pDelegate)
        m_pDelegate->closeInfoView(kDevelopView);
    else
        removeFromParent();
}

void WarriorDevelopView::giveUpCallBack( cocos2d::CCObject * obj )
{
	if (m_isWiattingForConfirm)
	{
		m_isWiattingForConfirm = false;
		HTTP_CLIENT->confDevelopWarrior(m_pWarriorInfo->getWid(),WARRIOR_DEV_CANCEL,this,callfuncJson_selector(WarriorDevelopView::confirmDevelopCallback));
	}

	changeDevelopButtonState(true);
}

void WarriorDevelopView::selectCallBack(CCObject* obj)
{
    CCMenuItem* pItem = (CCMenuItem*)obj;
    m_DevType = (PotBuff::PotbuffType)pItem->getTag();
    updateSelectButtons();
}

void WarriorDevelopView::updateSelectButtons()
{
	if (m_pSelectButtons[0])
	{
		if (m_DevType == PotBuff::kTypeNormal || m_DevType == PotBuff::kType10Normal)
		{
			m_pSelectButtons[0]->getButton()->selected();
		}
		else
		{
			m_pSelectButtons[0]->getButton()->unselected();
		}
	}

	if (m_pSelectButtons[1])
	{
		if (m_DevType == PotBuff::kTypeSpecial || m_DevType == PotBuff::kType10Special)
		{
			m_pSelectButtons[1]->getButton()->selected();
		}
		else
		{
			m_pSelectButtons[1]->getButton()->unselected();
		}
		
	}
}

void WarriorDevelopView::developOneButtonCallBack(CCObject* obj)
{
	if (m_DevType == PotBuff::kType10Normal)
	{
		m_DevType = PotBuff::kTypeNormal;
	}
	else if (m_DevType == PotBuff::kType10Special)
	{
		m_DevType = PotBuff::kTypeSpecial;
	}

	changeDevelopButtonState(false);
	developWarrior();
}

void WarriorDevelopView::confirmButtonCallBack( cocos2d::CCObject* obj )
{
	if (m_isWiattingForConfirm)
	{
		m_isWiattingForConfirm = false;
		DM_GET_SOUND_MODEL->playCommonEffectSound(WARRIOR_DEV_SOUND_ACTION, 1);
		HTTP_CLIENT->confDevelopWarrior(m_pWarriorInfo->getWid(),WARRIOR_DEV_CONFIRM,this,callfuncJson_selector(WarriorDevelopView::confirmDevelopCallback));
	}

	changeDevelopButtonState(true);
}

void WarriorDevelopView::developTenButtonCallBack( cocos2d::CCObject * obj )
{
	if (m_DevType == PotBuff::kTypeNormal)
	{
		m_DevType = PotBuff::kType10Normal;
	}
	else if (m_DevType == PotBuff::kTypeSpecial)
	{
		m_DevType = PotBuff::kType10Special;
	}
	
	changeDevelopButtonState(false);
	developWarrior();
}

void WarriorDevelopView::developWarrior()
{
	//    if(m_pWarriorInfo->potential<=0)
	//    {
	//        Tips_Alert(D_LOCAL_STRING("NeedPotential").c_str());
	//        return;
	//    }
	

	if(!m_pWarriorInfo->checkDevelopLevel())
	{
		Tips_Alert(D_LOCAL_STRING("WarriorLevel%dCantDev",WARRIOR_DEV_LV_MIN).c_str());
		changeDevelopButtonState(true);
		return;
	}

	uint32_t addParam = m_DevType>PotBuff::kTypeSpecial?K_WARRIOR_DEV_ONCE_MAX:1;
	if(DM_GET_ITEM_MODEL->getItemNum(PEIYANG_DAN_ID) < addParam)
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_ITEMS,CCInteger::create(PEIYANG_DAN_ID));
		changeDevelopButtonState(true);
		return;
	}

	addParam *= K_WARRIOR_DEV_COST;

	if(m_DevType%PotBuff::kTypeSpecial==0)
	{
		if(DM_GET_USER_MODEL->getUserInfo()->getPoints() < addParam)
		{
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
			return;
		}
	}
    DM_GET_SOUND_MODEL->playCommonEffectSound(WARRIOR_DEV_SOUND_ACTION, 0);

    //修改成连续培养的模式
    if (m_isWiattingForConfirm)
	{
		//m_isWiattingForConfirm = false;
		HTTP_CLIENT->confDevelopWarrior(m_pWarriorInfo->getWid(),WARRIOR_DEV_CONFIRM,this,callfuncJson_selector(WarriorDevelopView::confirmDevelopCallback));
        HTTP_CLIENT->developWarrior(m_pWarriorInfo->getWid(),m_DevType,this,callfuncJson_selector(WarriorDevelopView::submitCallback));
		return;
	}

	HTTP_CLIENT->developWarrior(m_pWarriorInfo->getWid(),m_DevType,this,callfuncJson_selector(WarriorDevelopView::submitCallback));
}

void WarriorDevelopView::updateWarriorDevelop()
{
	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("WarriorInfoError").c_str());
		return;
	}
	
    m_pContainer->removeAllChildren();
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon(m_pWarriorInfo->getAvatarId(), 0.83f));
    
    m_pWarriorGradeBg->setWarriorGrade(m_pWarriorInfo->getBaseInfo()->getColorGrade());
    
	m_pUpLevel->setString(CCString::createWithFormat("+%d", m_pWarriorInfo->getWarriorUpLv())->getCString());
	
	m_pGrade->setColor(m_pWarriorModel->getWarriorColorByGrade(m_pWarriorInfo->getBaseInfo()->getRealGrade()));

	for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
		int color = (int)m_pWarriorInfo->getBaseInfo()->getColorGrade();
		m_pColorGrade[i]->setVisible( color == i+1);
	}

	m_pLevel->setString(CCString::createWithFormat("%i",m_pWarriorInfo->getWarriorLv())->getCString());
	m_pName->setString(m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
	m_pAttack->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeAttak))->getCString());
	m_pDefence->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeDefence))->getCString());
	m_pHp->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeHp))->getCString());
	m_pSpeed->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeSpeed))->getCString());
	m_pFight->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorForce())->getCString());
    
    
    item::ItemInfo * itemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(PEIYANG_DAN_ID);
    
    m_pPill->setString(CCString::createWithFormat("%d",itemInfo?itemInfo->getNum():0)->getCString());
    
    m_pPot->setString(CCString::createWithFormat("%d",m_pWarriorInfo->getPotential())->getCString());
    
    m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
    updateSelectButtons();
    
}

void WarriorDevelopView::confirmDevelopCallback( const Json::Value &requestData, const Json::Value &responseData )
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	setDevelopResultInfoVisible(false);

}

void WarriorDevelopView::changeDevelopButtonState(bool isReset)
{
	for (unsigned int  i =kDVBTReturn; i<kDVBTMax; ++i)
	{
		m_buttonTextMap[i]->setVisible(false);
	}

	if (isReset)
	{
		m_pLeftButton->getButton()->setTag(kDVBTReturn);
		m_buttonTextMap[kDVBTReturn]->setVisible(true);
		m_pMiddleButton->getButton()->setTag(kDVBTTen);
		m_buttonTextMap[kDVBTTen]->setVisible(true);
		m_pRightButton->getButton()->setTag(kDVBTOne);
		m_buttonTextMap[kDVBTOne]->setVisible(true);
	}
	else
	{
		m_pLeftButton->getButton()->setTag(kDVBTGiveUp);
		m_buttonTextMap[kDVBTGiveUp]->setVisible(true);
		m_pMiddleButton->getButton()->setTag(kDVBTDevelop);
		m_buttonTextMap[kDVBTDevelop]->setVisible(true);
		m_pRightButton->getButton()->setTag(kDVBTConfirm);
		m_buttonTextMap[kDVBTConfirm]->setVisible(true);
	}
}

void WarriorDevelopView::BottomButtonsCallBack( cocos2d::CCObject * obj )
{
	CCNode* pNode = dynamic_cast<CCNode*>(obj);
	if (!pNode)
	{
		return;
	}

	unsigned buttonTag = pNode->getTag();
	switch(buttonTag)
	{
	case kDVBTReturn:
		closeCallBack(obj);
		break;
	case kDVBTTen:
		developTenButtonCallBack(obj);
		break;
	case kDVBTDevelop:
		developWarrior();
		break;
	case kDVBTOne:
		developOneButtonCallBack(obj);
		break;
	case kDVBTGiveUp:
		giveUpCallBack(obj);
		break;
	case kDVBTConfirm:
		confirmButtonCallBack(obj);
		break;
	default:
		CCLOG("error button tag in WarriorDevelopView::BottomButtonsCallBack");
		break;
	}
}









