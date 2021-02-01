//
//  EquipBurn.cpp
//  QSMY
//
//  Created by wyj on 13-8-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "EquipBurn.h"
#include "../common/AwardListView.h"
#include "../user/UserInfo.h"


USING_NS_CC;
using namespace sp;

bool BurnEquipFilter::operator()(warrior::WarriorCurInfo *pWarrior)
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

EquipBurn::EquipBurn():
m_pSelectView(NULL),
m_pBook(NULL),
m_pBtnBurn(NULL),
m_pCloseButton(NULL),
m_pAmount(NULL),
m_pBoxCoin(NULL),
m_pBoxItem(NULL),
m_pHideNode(NULL),
m_pConfirmButton(NULL),
m_pModel(NULL),
m_pEffect(NULL),
m_pEffectFly(NULL),
m_pAnimation(NULL),
m_pFlyNode(NULL),
m_pBtnAutoBurn(NULL),
m_pTreasureAwardList(NULL),
m_pAwardNode(NULL),
m_pMenuShop(NULL),
m_pDelegate(NULL)
{
	for(uint8_t i(0); i < TOTAL_BURN_NUM; ++i)
	{
		m_pIcons[i] = NULL;
		m_pShine[i] = NULL;
	}
	
}

EquipBurn::~EquipBurn()
{
	for(uint8_t i(0); i < TOTAL_BURN_NUM; ++i)
	{
		CC_SAFE_RELEASE(m_pIcons[i]);
		CC_SAFE_RELEASE(m_pShine[i]);
	}
	CC_SAFE_RELEASE(m_pBtnBurn);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pAmount);
	CC_SAFE_RELEASE(m_pBoxCoin);
	CC_SAFE_RELEASE(m_pBoxItem);
	CC_SAFE_RELEASE(m_pBtnAutoBurn);
	CC_SAFE_RELEASE(m_pBook);
	CC_SAFE_RELEASE(m_pHideNode);
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pEffect);
	CC_SAFE_RELEASE(m_pAnimation);
	CC_SAFE_RELEASE(m_pFlyNode);
	CC_SAFE_RELEASE(m_pSelectView);
	CC_SAFE_RELEASE(m_pAwardNode);
	CC_SAFE_RELEASE(m_pTreasureAwardList);
	CC_SAFE_RELEASE(m_pMenuShop);
}

bool EquipBurn::init()
{
	if(!DLayer::init())
	{
		return false;
	}
	
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zhuangbeilianhua/PL_zhuangbeilianhua");
	
	m_pModel = DM_GET_BURN_MODEL;

	m_pCloseButton->getButton()->setTarget(this, menu_selector(EquipBurn::onClose));
	m_pBtnBurn->getButton()->setTarget(this, menu_selector(EquipBurn::onBurnClick));
	m_pBtnAutoBurn->getButton()->setTarget(this, menu_selector(EquipBurn::onAutoClick));
	m_pConfirmButton->getButton()->setTarget(this, menu_selector(EquipBurn::onBookClick));
	m_pBook->setTarget(this, menu_selector(EquipBurn::onBookClick));
	m_pMenuShop->setTarget(this,menu_selector(EquipBurn::onOpenShop));
	
	for(int i = 0 ; i < TOTAL_BURN_NUM ; i++){
		//绑定点击事件
		m_pIcons[i]->getTouchEvent()->setTag(i);
		m_pIcons[i]->getTouchEvent()->setTarget(this, menu_selector(EquipBurn::onIconClick));
		//初始化
		m_pIcons[i]->updateBurnItem(NULL);
	}

	onIconRefresh();

	//预加载动画
	m_pEffect = ResourceManager::sharedResourceManager()->getUIAnimate("equipburn");
	CC_SAFE_RETAIN(m_pEffect);
	m_pAnimation->addChild(m_pEffect);

	//层级
	m_pFlyNode->removeFromParent();
	addChild(m_pFlyNode , 2);

	//user info , on page top
	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView(),1);

	return true;
}

void EquipBurn::animateEnd(sp::Armature * target, const char* event,const char* data)
{
	DM_GET_EQUIP_MODEL->sellEquip(m_pModel->getEquipList());
	m_pEffect->getAnimation()->removeAllObservers(this);
	m_pEffect->getAnimation()->stop();
	//恢复按钮可以点击
	setMenuTouchEnable(true);
}

void EquipBurn::onEnter()
{
	DLayer::onEnter();
	BINDING_ONLY(m_pModel, this, EquipBurn::resetAfterBurn, D_EVENT_BURN_ICON_UPDATE);
	BINDING_ONLY(m_pModel, this, EquipBurn::updateView, D_EVENT_BURN_UPDATE);
	BINDING_ONLY(m_pModel, this, EquipBurn::onAutoList, D_EVENT_BURN_AUTOLIST);
}

void EquipBurn::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	DLayer::onExit();
}

void EquipBurn::showSelectList()
{
	if(!m_pSelectView)
	{
		m_pSelectView = EquipBurnSelectView::create();
		m_pSelectView->setDelegate(this);
		//m_pSelectView->setFilter(BurnEquipFilter::create());
		//m_pSelectView->setMoreSelect(true);
		CC_SAFE_RETAIN(m_pSelectView);
	}	
	addChild(m_pSelectView,2);
}

bool EquipBurn::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAutoBurn", DButton *, this->m_pBtnAutoBurn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnBurn", DButton *, this->m_pBtnBurn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, this->m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmount", CCLabelBMFont *, this->m_pAmount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxCoin", ItemBox *, this->m_pBoxCoin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxItem", ItemBox *, this->m_pBoxItem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBook", CCMenuItemImage *, this->m_pBook);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHideNode", CCNode *, this->m_pHideNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimation", CCNode *, this->m_pAnimation);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFlyNode", CCNode *, this->m_pFlyNode);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTreasureAwardList", CCNode *, this->m_pTreasureAwardList);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAwardNode", CCNode *, this->m_pAwardNode);	
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pIcon",ItemBox*,this->m_pIcons,TOTAL_BURN_NUM);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pShine",UIAnimate*,this->m_pShine,TOTAL_BURN_NUM);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuShop" , CCMenuItemImage*, m_pMenuShop);

	return true;
}

void EquipBurn::onOpenShop(cocos2d::CCObject * sender)
{
	Game::sharedGame()->NavigateTo(navigation::kNavActivity, true, CCInteger::create(45));
}

void EquipBurn::updateView()
{
	//能炼化出来几种东西
	int count = DM_GET_BURN_MODEL->getEquipList()->count();

	//显示隐藏整体
	m_pAwardNode->setVisible(count!=0);

	if(count > 0)
	{
		drop::Drop*	pInfo = NULL;
		pInfo = drop::Drop::createCoin(m_pModel->m_EquipBurnPreview[0]);
		m_pBoxCoin->updateDropItem(pInfo);
		
		pInfo = drop::Drop::create(drop::kDropTypeItem , EQUIP_BURN_ITEM_ID , m_pModel->m_EquipBurnPreview[1]);
		m_pBoxItem->updateDropItem(pInfo);
	}
}

void EquipBurn::onIconClick(cocos2d::CCObject * sender)
{
	//获取点击对象tag
	CCNode* node = dynamic_cast<CCNode*>(sender);
	m_uCurrentTag = node->getTag();

	//处理英雄炼化的点击
	//icon是否已经有英雄
	bool selected = (m_pIcons[m_uCurrentTag]->getContainer()->getChildrenCount() != 0);
	if(selected){
		//取消选择
		uint32_t exWarriorID = m_pIcons[m_uCurrentTag]->getTag();
		uint8_t heroType = m_pIcons[m_uCurrentTag]->getHeroType();
		m_pIcons[m_uCurrentTag]->updateBurnItem(NULL);
		m_pModel->removeFromList(exWarriorID , BurnModel::kBurnTypeEquip);
	}
	else{
		//尚未选择
		//show warrior list
		showSelectList();
	}
}

void EquipBurn::onIconRefresh()
{
	m_pAmount->setString(D_CSTRING_FROM_UINT(m_pModel->getEquipRefineryItemNum()));

	for(int i = 0 ; i < TOTAL_BURN_NUM ; i++){
		m_pIcons[i]->updateBurnItem(NULL);
	}

	m_pAwardNode->setVisible(false);
	//重置model
	m_pModel->reset();
}

void EquipBurn::resetAfterBurn()
{
	onIconRefresh();

	//动画
	unsigned int itemnum = m_pModel->getItemShowNum();
	unsigned int coinnum = m_pModel->getCoinShowNum();

	m_pFlyNode->removeAllChildren();
	m_pEffectFly = NULL;
	m_pEffectFly = ResourceManager::sharedResourceManager()->getUIAnimate("EquipBurnFly" , sp::ZORDER);
	m_pFlyNode->addChild(m_pEffectFly);

	std::string coinName[2] = {"tqshuzi1","tqshuzi2"};
	std::string itemName[2] = {"jhshuzi1","jhshuzi2"};
	for( int i = 0 ; i < 2 ; i++ )
	{
		SPContainer* pContainer = SPContainer::create(sp::ZORDER);
		CCLabelBMFont* pNode = CCLabelBMFont::create(CCString::createWithFormat("+%d",coinnum)->getCString(), "fonts/number1.fnt");
		pNode->setAnchorPoint(ccp(0,0.5));
		pContainer->addChild(pNode);
		pNode->setScale(1.6f);
		m_pEffectFly->getBone(coinName[i].c_str())->replaceDisplayAt(pContainer, 0);

		pContainer = SPContainer::create(sp::ZORDER);
		pNode = CCLabelBMFont::create(CCString::createWithFormat("+%d",itemnum)->getCString(), "fonts/number1.fnt");
		pNode->setAnchorPoint(ccp(0,0.5));
		pContainer->addChild(pNode);
		pNode->setScale(1.6f);
		m_pEffectFly->getBone(itemName[i].c_str())->replaceDisplayAt(pContainer, 0);
	}
}

void EquipBurn::onAutoList()
{
	//处理ICON

	for(int i = 0 ; i < TOTAL_BURN_NUM ; i++){
		m_pIcons[i]->updateBurnItem(NULL);
	}

	CCArray* pList = DM_GET_BURN_MODEL->getEquipList();
	int index = 0;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(pList, pObj)
	{
		equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pObj);
		m_uCurrentTag = index;
		setSelectedEquip(pInfo);
		index++;
	}
}
void EquipBurn::playShine()
{
	for(int i = 0 ; i < TOTAL_BURN_NUM ; i++)
	{
		if(m_pIcons[i]->getBurnData())
		{
			m_pShine[i]->playAtIndex(0);
		}
	}
}
void EquipBurn::confirmBurn(MessageType type, DMessageBox* target)
{
	if (type == kMessageTypeCancel)
	{
		return;
	}
	//确认了就烧！！
	playShine();
	m_pEffect->getAnimation()->playByIndex(0 , -1 , -1 , true);
	m_pEffect->getAnimation()->addObserver(this, SPNotification_callfunc_selector(EquipBurn::animateEnd), COMPLETE);
	setMenuTouchEnable(false);
}

void EquipBurn::onBurnClick(cocos2d::CCObject * sender)
{
	//确定玩家已经选择了英雄 再炼化，动画播放完成后再调借口
	bool selected = (m_pModel->getEquipList()->count() > 0);

	
	if(selected){
		
		//DM_GET_EQUIP_MODEL->sellEquip(m_pModel->getEquipList());

		bool bShowConfirm = m_pModel->willShowConfirm(BurnModel::kBurnTypeEquip);
		if (bShowConfirm)
		{
			//二次验证
			DMessageBox* pPBox = DMessageBox::showMessageBox(D_LOCAL_STRING("EquipRefineTip3").c_str(), this, messageBox_callfunc_selector(EquipBurn::confirmBurn));
		}
		else
		{
			//无需2次验证
			playShine();
			m_pEffect->getAnimation()->playByIndex(0 , -1 , -1 , true);
			m_pEffect->getAnimation()->addObserver(this, SPNotification_callfunc_selector(EquipBurn::animateEnd), COMPLETE);
			setMenuTouchEnable(false);
		}	
	}
	else{
		Tips_Alert(D_LOCAL_STRING("EquipRefineTip2").c_str());
	}
}

void EquipBurn::onAutoClick(cocos2d::CCObject * sender)
{
	m_pModel->makeEquipAutoList();
}

void EquipBurn::onBookClick(cocos2d::CCObject * sender)
{
	bool v = m_pHideNode->isVisible();
	m_pHideNode->setVisible(!v);
}

void EquipBurn::onSelectConfirm()
{
	CCArray* pArray = DM_GET_BURN_MODEL->getEquipList();
	CCArray* pEquipList = CCArray::create();
	pEquipList->addObjectsFromArray(pArray);

	for(int i = 0 ; i < TOTAL_BURN_NUM ; i++)
	{
		m_pIcons[i]->updateBurnItem(NULL);
		if(pEquipList->count() > i)
		{
			equip::EquipCurInfo* info = static_cast<equip::EquipCurInfo*>(pEquipList->objectAtIndex(i));	
			m_uCurrentTag = i;
			setSelectedEquip(info);
		}		
	}
}

void EquipBurn::setSelectedEquip(equip::EquipCurInfo * pInfo)
{

	if(m_pIcons[m_uCurrentTag]){
		//确保元素不为空

		if(pInfo){
			//选择了英雄
			m_pIcons[m_uCurrentTag]->updateBurnItem(drop::Drop::create(pInfo));
			//暂存ID(取消选择时用于显示)
			m_pIcons[m_uCurrentTag]->setTag(pInfo->getId());
			//m_pIcons[m_uCurrentTag]->setHeroType(1);
			//在model中记录这个英雄
			//m_pModel->addToList(pWarrior->getWid() , BurnModel::kBurnTypeHero);
		}
		else{
			//没选英雄 返回，不做处理			
		}		
	}
}

void EquipBurn::setMenuTouchEnable(bool val)
{
	m_pBook->setEnabled(val);
	m_pBtnBurn->getButton()->setEnabled(val);
	m_pBtnAutoBurn->getButton()->setEnabled(val);
}

void EquipBurn::onBackKey()
{
	if (m_pSelectView && m_pSelectView->getParent())
	{
		m_pSelectView->removeFromParent();
		return;
	}
	removeFromParent();
	//BackKeyInterface::onBackKey();
}

void EquipBurn::onClose(CCObject* sender)
{
	removeFromParent();
	Game::sharedGame()->NavigateTo(navigation::kNavEquip);
	//m_pDelegate->onBurnClose();
}