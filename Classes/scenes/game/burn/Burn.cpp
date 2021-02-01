//
//  Burn.cpp
//  QSMY
//
//  Created by wyj on 13-8-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Burn.h"
#include "../common/AwardListView.h"
#include "../user/UserInfo.h"


USING_NS_CC;
using namespace sp;

bool BurnWarriorFilter::operator()(warrior::WarriorCurInfo *pWarrior)
{
	if(DM_GET_BURN_MODEL->isSelected(pWarrior->getWid() , BurnModel::kBurnTypeHero))
	{
		//选过的不能选
		return false;
	}
	if(DM_GET_FORMATION_MODEL->isWarriorBattle(pWarrior->getWid()))
	{
		//上阵的不能选
		return false;
	}
	if(DM_GET_GUNDAM_EF_MODEL->isEgineerBattle(pWarrior->getWid()))
	{
		//小伙伴 不能选
		return false;
	}
	//if(pWarrior->getBaseInfo()->getColorGrade() < warrior::kWarGradePurple)
	//{
	//	//颜色不够 不能选
	//	return false;
	//}
	return true;
}

Burn::Burn():
m_pWarriorPopup(NULL),
//m_pTreasurePopup(NULL),
m_pBook(NULL),
m_pBtnBurn(NULL),
m_pHideNode(NULL),
m_pConfirmButton(NULL),
m_pModel(NULL),
m_pEffect(NULL),
m_pAnimation(NULL),
m_pBtnAutoBurn(NULL),
m_pTxtHero(NULL),
m_pTxtTreasure(NULL),
m_pTitleHero(NULL),
m_pTitleTreasure(NULL),
m_pMenuTreasure(NULL),
m_pMenuHero(NULL),
m_BurnType(BurnModel::kBurnTypeHero),
m_pTxtTreasureDetail(NULL),
m_pTxtHeroDetail(NULL),
m_pTreasureAwardList(NULL),
m_pHeroAwardList(NULL),
m_pAwardNode(NULL),
m_pBoxTreasureItem(NULL),
m_pBoxTreasureCoin(NULL),
m_pRebirthBtn(NULL),
m_pRebirthTxt(NULL)
{
	for(uint8_t i(0); i < TOTAL_BURN_NUM; ++i)
	{
		m_pIcons[i] = NULL;
	}
	for(uint8_t i(0); i < HERO_BURN_PREVIEW_COUNT; ++i)
	{
		m_pBox[i] = NULL;
		m_pItemIdList[i] = 0;
	}
	
}

Burn::~Burn()
{
	for(uint8_t i(0); i < TOTAL_BURN_NUM; ++i)
	{
		CC_SAFE_RELEASE(m_pIcons[i]);
	}
	for(uint8_t i(0); i < HERO_BURN_PREVIEW_COUNT; ++i)
	{
		CC_SAFE_RELEASE(m_pBox[i]);
	}
	CC_SAFE_RELEASE(m_pBtnBurn);
	CC_SAFE_RELEASE(m_pBtnAutoBurn);
	CC_SAFE_RELEASE(m_pBook);
	CC_SAFE_RELEASE(m_pHideNode);
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pEffect);
	CC_SAFE_RELEASE(m_pAnimation);
	CC_SAFE_RELEASE(m_pTxtHero);
	CC_SAFE_RELEASE(m_pTxtTreasure);
	CC_SAFE_RELEASE(m_pTitleHero);
	CC_SAFE_RELEASE(m_pTitleTreasure);
	CC_SAFE_RELEASE(m_pMenuHero);
	CC_SAFE_RELEASE(m_pMenuTreasure);
	CC_SAFE_RELEASE(m_pWarriorPopup);
	//CC_SAFE_RELEASE(m_pTreasurePopup);
	CC_SAFE_RELEASE(m_pTxtHeroDetail);
	CC_SAFE_RELEASE(m_pTxtTreasureDetail);
	CC_SAFE_RELEASE(m_pHeroAwardList);
	CC_SAFE_RELEASE(m_pAwardNode);
	CC_SAFE_RELEASE(m_pTreasureAwardList);
	CC_SAFE_RELEASE(m_pBoxTreasureItem);
	CC_SAFE_RELEASE(m_pBoxTreasureCoin);
	CC_SAFE_RELEASE(m_pRebirthBtn);
	CC_SAFE_RELEASE(m_pRebirthTxt);
}

bool Burn::init()
{
	if(!DLayer::init())
	{
		return false;
	}
	
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_liandan/PL_liandan");
	
	m_pModel = DM_GET_BURN_MODEL;
	//m_pModel->reset();//进来的时候 重置model

	m_pBtnBurn->getButton()->setTarget(this, menu_selector(Burn::onBurnClick));
	m_pBtnAutoBurn->getButton()->setTarget(this, menu_selector(Burn::onAutoClick));
	m_pConfirmButton->getButton()->setTarget(this, menu_selector(Burn::onBookClick));
	m_pBook->setTarget(this, menu_selector(Burn::onBookClick));
	m_pMenuHero->setTarget(this, menu_selector(Burn::onToggleClick));
	m_pMenuTreasure->setTarget(this, menu_selector(Burn::onToggleClick));
	m_pRebirthBtn->setTarget(this,menu_selector(Burn::onOpenRebirth));
	
	for(int i = 0 ; i < TOTAL_BURN_NUM ; i++){
		//绑定点击事件
		m_pIcons[i]->getTouchEvent()->setTag(i);
		m_pIcons[i]->getTouchEvent()->setTarget(this, menu_selector(Burn::onIconClick));
		//初始化
		m_pIcons[i]->updateBurnItem(NULL);
	}
	//初始化box
	m_pItemIdList[0] = D_CONFIG_UINT_FOR_KEY(ITEM_COIN);
	m_pItemIdList[1] = D_CONFIG_UINT_FOR_KEY(BURN_ITEM1);
	m_pItemIdList[2] = D_CONFIG_UINT_FOR_KEY(BURN_ITEM2);
	m_pItemIdList[3] = D_CONFIG_UINT_FOR_KEY(BURN_ITEM3);
	m_pItemIdList[4] = D_CONFIG_UINT_FOR_KEY(BURN_ITEM4);
	m_pItemIdList[5] = D_CONFIG_UINT_FOR_KEY(BURN_ITEM5);
	m_pItemIdList[6] = D_CONFIG_UINT_FOR_KEY(BURN_ITEM6);

	m_pBoxTreasureCoin->getContainer()->addChild(ResourceManager::sharedResourceManager()->getItemIcon40P(D_CONFIG_UINT_FOR_KEY(ITEM_COIN)));
	m_pBoxTreasureItem->getContainer()->addChild(ResourceManager::sharedResourceManager()->getItemIcon40P(TREASURE_ENHANCE_ITEM_ID));

	resetAfterBurn();
	//进来默认是英雄界面
	toggleViewByType(BurnModel::kBurnTypeHero);

	//预加载动画
	m_pEffect = ResourceManager::sharedResourceManager()->getUIAnimate("burn");
	//m_pEffect->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Burn::animateEnd), COMPLETE);
	CC_SAFE_RETAIN(m_pEffect);
	m_pRebirthTxt->setVisible(true);
	m_pRebirthBtn->setVisible(true);
	return true;
}

void Burn::animateEnd(sp::Armature * target, const char* event,const char* data)
{
	m_pModel->doBurn(m_BurnType);
	m_pEffect->getAnimation()->removeAllObservers(this);
	m_pEffect->getAnimation()->playByIndex(0);
	//恢复按钮可以点击
	setMenuTouchEnable(true);
}

void Burn::onEnter()
{
	DLayer::onEnter();
	BINDING_ONLY(m_pModel, this, Burn::resetAfterBurn, D_EVENT_BURN_ICON_UPDATE);
	BINDING_ONLY(m_pModel, this, Burn::updateView, D_EVENT_BURN_UPDATE);
	BINDING_ONLY(m_pModel, this, Burn::onAutoList, D_EVENT_BURN_AUTOLIST);

	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView());	//添加用户基本信息

	//播放动画
	m_pEffect->getAnimation()->playByIndex(0);
	m_pEffect->setPosition(D_DESIGN_POINT_CENTER);
	m_pAnimation->addChild(m_pEffect);
}

void Burn::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	DLayer::onExit();
}

void Burn::showWarriorSelectList()
{
	if(!m_pWarriorPopup)
	{
		m_pWarriorPopup = BurnSelectView::create(NULL);
		m_pWarriorPopup->setDelegate(this);
		m_pWarriorPopup->setFilter(BurnWarriorFilter::create());
		m_pWarriorPopup->setMoreSelect(true);
		CC_SAFE_RETAIN(m_pWarriorPopup);
	}	
	addChild(m_pWarriorPopup);
}

void Burn::showTreasureSelectList()
{//不再使用

	//if(!m_pTreasurePopup)
	//{
	//	m_pTreasurePopup = TreasureSelectView::create();
	//	m_pTreasurePopup->setDelegate(this);
	//	CC_SAFE_RETAIN(m_pTreasurePopup);
	//}	
	//addChild(m_pTreasurePopup);
}

bool Burn::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAutoBurn", DButton *, this->m_pBtnAutoBurn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnBurn", DButton *, this->m_pBtnBurn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, this->m_pConfirmButton);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBook", CCMenuItemImage *, this->m_pBook);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHideNode", CCNode *, this->m_pHideNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimation", CCNode *, this->m_pAnimation);	

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtHero", CCNode *, this->m_pTxtHero);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtTreasure", CCNode *, this->m_pTxtTreasure);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleHero", CCNode *, this->m_pTitleHero);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleTreasure", CCNode *, this->m_pTitleTreasure);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuHero", CCMenuItemImage *, this->m_pMenuHero);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuTreasure", CCMenuItemImage *, this->m_pMenuTreasure);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtTreasureDetail", CCNode *, this->m_pTxtTreasureDetail);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtHeroDetail", CCNode *, this->m_pTxtHeroDetail);		
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTreasureAwardList", CCNode *, this->m_pTreasureAwardList);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHeroAwardList", CCNode *, this->m_pHeroAwardList);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAwardNode", CCNode *, this->m_pAwardNode);	
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pIcon",ItemBox*,this->m_pIcons,TOTAL_BURN_NUM);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pBox",ItemBox*,this->m_pBox,HERO_BURN_PREVIEW_COUNT);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBox11", ItemBox *, this->m_pBoxTreasureCoin);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBox12", ItemBox *, this->m_pBoxTreasureItem);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRebirthBtn" , CCMenuItemImage*, m_pRebirthBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRebirthTxt",CCNode*,m_pRebirthTxt);

	return true;
}

void Burn::toggleViewByType(BurnModel::BurnType burnType)
{
	m_BurnType = burnType;
	m_pTxtHero->setVisible(burnType == BurnModel::kBurnTypeHero);
	m_pTitleHero->setVisible(burnType == BurnModel::kBurnTypeHero);
	m_pHeroAwardList->setVisible(burnType == BurnModel::kBurnTypeHero);
	m_pMenuHero->setVisible(burnType == BurnModel::kBurnTypeHero);
	m_pMenuTreasure->setVisible(burnType == BurnModel::kBurnTypeEquip);
	m_pTxtTreasure->setVisible(burnType == BurnModel::kBurnTypeEquip);
	m_pTitleTreasure->setVisible(burnType == BurnModel::kBurnTypeEquip);	
	m_pTreasureAwardList->setVisible(burnType == BurnModel::kBurnTypeEquip);
	//重置界面中的框框
	resetAfterBurn();

	//屏蔽宝物炼化
	m_pMenuHero->setVisible(false);
	m_pMenuTreasure->setVisible(false);
	m_pTxtHero->setVisible(false);
	m_pTxtTreasure->setVisible(false);
}

void Burn::onToggleClick(cocos2d::CCObject * sender)
{
	if(m_BurnType == BurnModel::kBurnTypeHero)
		toggleViewByType(BurnModel::kBurnTypeEquip);
	else
		toggleViewByType(BurnModel::kBurnTypeHero);
}

void Burn::onOpenRebirth(cocos2d::CCObject * sender)
{
	Game::sharedGame()->NavigateTo(navigation::kNavRebirth);
}

void Burn::updateView()
{
	if(m_BurnType == BurnModel::kBurnTypeHero)
	{
		//能炼化出来几种东西
		int count = 0;
		for(int i = 0 ; i < HERO_BURN_PREVIEW_COUNT ; i++)
		{
			if(m_pModel->m_HeroBurnPreview[i] > 0)
			{
				//能练出来这货
				m_pBox[count]->setVisible(true);
				drop::Drop*	pInfo = NULL;
				pInfo = (i == 0 ? drop::Drop::createCoin(m_pModel->m_HeroBurnPreview[i]) : drop::Drop::create(drop::kDropTypeItem , m_pItemIdList[i] , m_pModel->m_HeroBurnPreview[i]));
				m_pBox[count]->updateDropItem(pInfo);
				count++;
			}
		}

		//隐藏掉没有的
		for(int i = count ; i < HERO_BURN_PREVIEW_COUNT ; i++)
		{
			m_pBox[i]->setVisible(false);
		}

		//显示隐藏整体
		m_pAwardNode->setVisible(count!=0);
		m_pHeroAwardList->setPositionX((HERO_BURN_PREVIEW_COUNT - count) * 42);
	}
	else if(m_BurnType == BurnModel::kBurnTypeEquip)
	{
		m_pBoxTreasureCoin->getCount()->setString(D_CSTRING_FROM_UINT(m_pModel->m_EquipBurnPreview[0]));
		m_pBoxTreasureItem->getCount()->setString(D_CSTRING_FROM_UINT(m_pModel->m_EquipBurnPreview[1]));
	}
	
}

void Burn::onIconClick(cocos2d::CCObject * sender)
{
	//获取点击对象tag
	CCNode* node = dynamic_cast<CCNode*>(sender);
	m_uCurrentTag = node->getTag();

	//处理英雄炼化的点击
	if(m_BurnType == BurnModel::kBurnTypeHero)
	{
		//icon是否已经有英雄
		bool selected = (m_pIcons[m_uCurrentTag]->getContainer()->getChildrenCount() != 0);
		if(selected){
			//取消选择
			uint32_t exWarriorID = m_pIcons[m_uCurrentTag]->getTag();
			uint8_t heroType = m_pIcons[m_uCurrentTag]->getHeroType();
			m_pIcons[m_uCurrentTag]->updateBurnItem(NULL);
			m_pModel->removeFromList(exWarriorID , BurnModel::kBurnTypeHero,heroType);
		}
		else{
			//尚未选择
			//show warrior list
			showWarriorSelectList();
		}
	}
	else{
		//处理宝物炼化的点击
		bool selected = (m_pIcons[m_uCurrentTag]->getContainer()->getChildrenCount() != 0);
		if(selected){
			//取消选择
			uint32_t exEquipID = m_pIcons[m_uCurrentTag]->getTag();
			m_pIcons[m_uCurrentTag]->updateBurnItem(NULL);
			//model中将该物品移除
			uint8_t heroType = m_pIcons[m_uCurrentTag]->getHeroType();
			m_pModel->removeFromList(exEquipID , BurnModel::kBurnTypeEquip ,heroType);
		}
		else{
			showTreasureSelectList();
		}
	}
}

void Burn::resetAfterBurn()
{
	for(int i = 0 ; i < TOTAL_BURN_NUM ; i++){
		m_pIcons[i]->updateBurnItem(NULL);
	}

	m_pAwardNode->setVisible(false);

	//for(int i = 0 ; i < HERO_BURN_PREVIEW_COUNT ; i++){
	//	if(m_pBox[i]){
	//		m_pBox[i]->getCount()->setString("0");
	//		m_pBox[i]->getName()->setString("");
	//	}	
	//}
	//m_pBoxTreasureCoin->getCount()->setString("0");
	//m_pBoxTreasureItem->getCount()->setString("0");
	//m_pBoxTreasureCoin->getName()->setString("");
	//m_pBoxTreasureItem->getName()->setString("");

	//重置model
	m_pModel->reset();
	
}

void Burn::onAutoList()
{
	//处理自动炼化返回 更新界面

	//清空当前的ICON
	for(int i = 0 ; i < TOTAL_BURN_NUM ; i++){
		m_pIcons[i]->updateBurnItem(NULL);
	}
	//区别宝物自动炼化还是英雄自动炼化
	if(m_BurnType == BurnModel::kBurnTypeHero)
	{
		//读autolist返回的列表
		CCDictionary* dict = DM_GET_BURN_MODEL->getBurnDict();
		CCDictElement *pEle = NULL;
		uint32_t index = 0;
		CCDICT_FOREACH(dict, pEle)
		{
			warrior::WarriorCurInfo* warriorInfo = (warrior::WarriorCurInfo*)pEle->getObject();
			//使用单选的回调接口处理
			m_uCurrentTag = index;
			setSelectedWarrior(warriorInfo);
			index++;
		}
		CCDictionary* souldict = DM_GET_BURN_MODEL->getSoulDict();
		CCLog(CCString::createWithFormat("souldict length%d",souldict->count())->getCString());
		CCDICT_FOREACH(souldict, pEle)
		{
			warrior::SoulInfo* warriorInfo = (warrior::SoulInfo*)pEle->getObject();
			//使用单选的回调接口处理
			m_uCurrentTag = index;
			setSelectedSoul(warriorInfo);
			index++;
		}
	}
	else if(m_BurnType == BurnModel::kBurnTypeEquip)
	{
		CCObject* pObj = NULL;
		CCArray* pList = NULL;
		pList = DM_GET_BURN_MODEL->getTreasureList();
		uint32_t index = 0;
		CCARRAY_FOREACH(pList, pObj)
		{		
			CCInteger* pInt = dynamic_cast<CCInteger*>(pObj);
			equip::EquipCurInfo* pEquipInfo = DM_GET_EQUIP_MODEL->getEquipCurInfoByID(pInt->getValue());
			if(pEquipInfo)
			{
				m_uCurrentTag = index;
				setSelectedTreasure(pEquipInfo);
				index++;
			}
			else{
				CCLOG("ERROR: Burn::onAutoList");
			}			
		}
	}
	
}

void Burn::confirmBurn(MessageType type, DMessageBox* target)
{
	if (type == kMessageTypeCancel)
	{
		return;
	}
	//确认了就烧！！
	m_pEffect->getAnimation()->playByIndex(1);
	m_pEffect->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Burn::animateEnd), COMPLETE);
	setMenuTouchEnable(false);
}

void Burn::onBurnClick(cocos2d::CCObject * sender)
{
	//确定玩家已经选择了英雄 再炼化，动画播放完成后再调借口
	bool selected = false;

	if(m_BurnType == BurnModel::kBurnTypeHero)
	{
		selected = (m_pModel->getBurnDict()->count() > 0);
		if(!selected) selected = (m_pModel->getSoulDict()->count() > 0);
	}
	else if(m_BurnType == BurnModel::kBurnTypeEquip)
	{
		selected = (m_pModel->getTreasureList()->count() > 0);
	}
	
	if(selected){
		
		bool bShowConfirm = m_pModel->willShowConfirm();
		if (bShowConfirm)
		{
			//二次验证
			DMessageBox* pPBox = DMessageBox::showMessageBox(D_LOCAL_STRING("$GoldHeroLianhua").c_str(), this, messageBox_callfunc_selector(Burn::confirmBurn));
		}
		else
		{
			//无需2次验证
			m_pEffect->getAnimation()->playByIndex(1);
			m_pEffect->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Burn::animateEnd), COMPLETE);
			setMenuTouchEnable(false);
		}	
	}
	else{
		if(m_BurnType == BurnModel::kBurnTypeHero)
		{
			Tips_Alert(D_LOCAL_STRING("$LianhuaNoneHero").c_str());
		}
		else if(m_BurnType == BurnModel::kBurnTypeEquip)
		{
			Tips_Alert(D_LOCAL_STRING("$LianhuaNoneBaowu").c_str());
		}
	}
}

void Burn::onAutoClick(cocos2d::CCObject * sender)
{
	if(m_BurnType == BurnModel::kBurnTypeHero) m_pModel->getAutoList();
	//if(m_BurnType == BurnModel::kBurnTypeEquip) m_pModel->makeTreasureAutoList();
}

void Burn::onBookClick(cocos2d::CCObject * sender)
{
	m_pTxtTreasureDetail->setVisible(m_BurnType == BurnModel::kBurnTypeEquip);
	m_pTxtHeroDetail->setVisible(m_BurnType == BurnModel::kBurnTypeHero);
	bool v = m_pHideNode->isVisible();
	m_pHideNode->setVisible(!v);
}

void Burn::setSelectedWarrior(warrior::WarriorCurInfo * pWarrior)
{

	if(m_pIcons[m_uCurrentTag]){
		//确保元素不为空

		if(pWarrior){
			//选择了英雄
			m_pIcons[m_uCurrentTag]->updateBurnItem(drop::Drop::create(pWarrior));
			//暂存ID(取消选择时用于显示)
			m_pIcons[m_uCurrentTag]->setTag(pWarrior->getWid());
			m_pIcons[m_uCurrentTag]->setHeroType(1);
			//在model中记录这个英雄
			m_pModel->addToList(pWarrior->getWid() , BurnModel::kBurnTypeHero);
		}
		else{
			//没选英雄 返回，不做处理			
		}		
	}
}

void Burn::setSelectedSoul(warrior::SoulInfo * pWarrior)
{
	if(m_pIcons[m_uCurrentTag]){
		//确保元素不为空

		if(pWarrior){
			//选择了英雄
			m_pIcons[m_uCurrentTag]->updateBurnItem(drop::Drop::create(pWarrior));
			//暂存ID(取消选择时用于显示)
			m_pIcons[m_uCurrentTag]->setTag(pWarrior->getWid());
			m_pIcons[m_uCurrentTag]->setHeroType(2);
			//在model中记录这个英雄
			m_pModel->addToSoul(pWarrior->getWid());
		}
		else{
			//没选英雄 返回，不做处理			
		}		
	}
}

void Burn::setSelectedWarriorMore(cocos2d::CCArray *pWarrior,uint8_t heroType)
{
	if(pWarrior && pWarrior->count()>0)
	{
		int index = 0;
		if(heroType == 1)
		{
			warrior::WarriorCurInfo* info = NULL;
			if(m_pIcons[m_uCurrentTag])
			{
				info = static_cast<warrior::WarriorCurInfo*>(pWarrior->objectAtIndex(0));
				setSelectedWarrior(info);

				pWarrior->removeObjectAtIndex(0);
			}

			if(pWarrior->count()>0)
			{
				for(int i = 0 ; i < TOTAL_BURN_NUM ; i++){
					if(index>= pWarrior->count())
					{
						break;
					}
					if(!m_pIcons[i]->getBurnData())
					{
						info = static_cast<warrior::WarriorCurInfo*>(pWarrior->objectAtIndex(index));
						m_uCurrentTag = i;
						setSelectedWarrior(info);
						index++;
					}
				}
			}
		}
		if(heroType == 2)
		{
			warrior::SoulInfo* soulInfo = NULL;
			if(m_pIcons[m_uCurrentTag])
			{
				soulInfo = static_cast<warrior::SoulInfo*>(pWarrior->objectAtIndex(0));
				setSelectedSoul(soulInfo);

				pWarrior->removeObjectAtIndex(0);
			}

			if(pWarrior->count()>0)
			{
				for(int i = 0 ; i < TOTAL_BURN_NUM ; i++){
					if(index>=pWarrior->count())
					{
						break;
					}
					if(!m_pIcons[i]->getBurnData())
					{
						soulInfo = static_cast<warrior::SoulInfo*>(pWarrior->objectAtIndex(index));
						m_uCurrentTag = i;
						setSelectedSoul(soulInfo);
						index++;
					}
				}
			}
		}
	}
}

void Burn::setSelectedTreasure(equip::EquipCurInfo * pInfo)
{

	if(m_pIcons[m_uCurrentTag]){
		if(pInfo){
			m_pIcons[m_uCurrentTag]->updateBurnItem(drop::Drop::create(pInfo));
			//暂存ID(取消选择时用于显示)
			m_pIcons[m_uCurrentTag]->setTag(pInfo->getId());
			//在model中记录
			m_pModel->addToList(pInfo->getId() , BurnModel::kBurnTypeEquip);
		}
		else{
			//没选英雄 返回，不做处理			
		}		
	}
}

void Burn::setMenuTouchEnable(bool val)
{
	m_pMenuHero->setEnabled(val);
	m_pMenuTreasure->setEnabled(val);
	m_pBook->setEnabled(val);
	m_pBtnBurn->getButton()->setEnabled(val);
	m_pBtnAutoBurn->getButton()->setEnabled(val);
}

void Burn::onBackKey()
{
	if (m_pWarriorPopup && m_pWarriorPopup->getParent())
	{
		m_pWarriorPopup->removeFromParent();
		return;
	}
	//if (m_pTreasurePopup && m_pTreasurePopup->getParent())
	//{
	//	m_pTreasurePopup->removeFromParent();
	//	return;
	//}
	BackKeyInterface::onBackKey();
}