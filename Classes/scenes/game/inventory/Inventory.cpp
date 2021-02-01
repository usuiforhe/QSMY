//
//  Inventory.cpp
//  背包界面
//
//  Created by cll on 13-4-2.

//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "Inventory.h"
#include "../user/UserInfo.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "ItemInfoView.h"
#include "managers/TipsManager.h"
#include "ChangeName.h"
#include "BoxEffect.h"
#include "../equip/EquipInfoView.h"
#include "200RedPacket.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  INVENTORY_CELL_SIZE	CCSizeMake(640, 135)
#define  INVENTORY_CELL_SIZE1	CCSizeMake(640, 185)
/************************ inventory class begin*****************************/

Inventory::Inventory()
:m_pContainer(NULL)
,m_pItemInfo(NULL)
,m_pDTabView(NULL)
,m_pPopupContainer(NULL)
,m_pSellButton(NULL)
,m_pTableNode(NULL)
,m_pAvailableBg(NULL)
,m_pAvailableLabel(NULL)
{
	
}

Inventory::~Inventory()
{
	CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pItemInfo);
    CC_SAFE_RELEASE(m_pDTabView);
    CC_SAFE_RELEASE(m_pPopupContainer);
	CC_SAFE_RELEASE(m_pSellButton);
	CC_SAFE_RELEASE(m_pTableNode);
	CC_SAFE_RELEASE(m_pAvailableBg);
	CC_SAFE_RELEASE(m_pAvailableLabel);
}

bool Inventory::init()
{
	if(!DLayer::init())
	{
		return false;
	}
    
    m_pPopupContainer = CCNode::create();
    CC_SAFE_RETAIN(m_pPopupContainer);

    //CCB_READER_AND_ADDCHILD("ui/ccb/inventory/inventory");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao");

	this->m_pSellButton->setVisible(false);

    m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getPosition().y, PL_MAX_HEIGHT_DIFF / 2), m_pContainer->getContentSize().width, PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize().height+9, -PL_MAX_HEIGHT_DIFF)),50);
    //m_pDTabView->setPadding(15.0);
	//m_pDTabView->setTopMenuisLeftAlignment(false);
	/*m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, m_pContainer->getPosition().y, m_pContainer->getContentSize().width, m_pContainer->getContentSize().height+24)
		,ccWHITE
		,D_FONT_DEFAULT
		,D_FONT_SIZE_14
		,15
		,50);*/
	m_pDTabView->retain();

	std::string tabName[2] = {"pl_bb_wenzi_daoju","pl_bb_wenzi_suipian"};
    const std::set<uint32_t>& sheets = DM_GET_ITEM_MODEL->getSheets();
    std::set<uint32_t>::const_iterator itr;
	uint32_t i = 0;
    for (itr=sheets.begin(); itr!=sheets.end(); ++itr) {
        SelectItemView * pItemList = SelectItemView::create(*itr,PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
        if(pItemList)
        {
			
            pItemList->setDelegate(this);
            //m_pDTabView->addTabWithFontImg(pItemList,CCString::createWithFormat("ui/common/inventory_sheet_%d.png",*itr)->getCString());
			//m_pDTabView->addTab(pItemList,"","PL_ui/beibao/pl_bb_btn_xiaofenye.png","PL_ui/beibao/pl_bb_btn_xiaofenye_up.png",NULL,CCString::createWithFormat("PL_ui/beibao/%s.png",tabName[i].c_str())->getCString());
			m_pDTabView->addTab(pItemList,"","PL_ui/all/all_btn_fenye_lan.png","PL_ui/all/all_btn_fenye_lv.png",NULL,CCString::createWithFormat("PL_ui/beibao/%s.png",tabName[i].c_str())->getCString());
			i++;
		}
    }
    m_pTableNode->addChild(m_pDTabView);
	//m_pTableNode->removeFromParent();
	//addChild(m_pTableNode);

    UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView());	//添加基本信息title
    addChild(m_pPopupContainer,1);

	return true;
}

bool Inventory::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSellButton", CCMenuItemImage *, this->m_pSellButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableNode", CCNode *, this->m_pTableNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvailableBg", CCNode *, this->m_pAvailableBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvailableLabel", CCLabelBMFont *, this->m_pAvailableLabel);
    return false;
}

void Inventory::onEnter()
{
	BINDING_EXEC(DM_GET_ITEM_MODEL,this,Inventory::updateAvailable,D_EVENT_AVAILABLE_EQUIP);
	DLayer::onEnter();
}

void Inventory::onExit()
{
	UNBINDING_ALL(DM_GET_ITEM_MODEL,this);
	DLayer::onExit();
}

void Inventory::updateAvailable()
{
	unsigned int num = DM_GET_ITEM_MODEL->getAvailableEquipNum();
	m_pAvailableBg->setVisible(num != 0);
	m_pAvailableLabel->setString(D_CSTRING_FROM_UINT(num));
}

void Inventory::closeInfoView(InventoryType type)
{
    m_pPopupContainer->removeAllChildren();
}

void Inventory::showInfoView(InventoryType type,item::ItemInfo* pItemInfo,cocos2d::CCObject* param)
{
	
	m_pPopupContainer->removeAllChildren();
	if(pItemInfo->getBaseInfo()->getItemType() == item::kItemTypeFragment)
	{
		//if(m_pEquipInfoView == NULL)
		//{
		//	m_pEquipInfoView = EquipInfoView::create(EquipInfoView::kTypeViewOnly);
		//	m_pEquipInfoView->retain();
		//	PL_MOVE_NODE_ABOVE_CENTER_Y(m_pEquipInfoView);			
		//}
		EquipInfoView* m_pEquipInfoView = EquipInfoView::create(EquipInfoView::kTypeViewOnly);
		PL_MOVE_NODE_ABOVE_CENTER_Y(m_pEquipInfoView);		

		equip::EquipCurInfo* pEquip = equip::EquipCurInfo::create(pItemInfo->getBaseInfo()->getResID());
		m_pEquipInfoView->setEquip(pEquip);
		m_pPopupContainer->addChild(m_pEquipInfoView);
	}
	else
	{
		if(m_pItemInfo == NULL)
		{
			m_pItemInfo = ItemInfoView::create();
			m_pItemInfo->setDelegate(this);
			m_pItemInfo->retain();
		}

		m_pItemInfo->setItemInfo(pItemInfo);
		m_pPopupContainer->addChild(m_pItemInfo);
	}
	

}

void Inventory::useItem(item::ItemInfo *pItem)
{
	//处理招募令的特殊情况
	if(pItem->getItemId() == ZHAO_MU_LING_ID || pItem->getItemId() == NORMAL_CARD_ITEM_ID || pItem->getItemId() == HARD_CARD_ITEM_ID){		
		Game::sharedGame()->NavigateTo(navigation::kNavMallLottery);
		return;
	}

	//处理改名卡
	if (pItem->isItemForRename()){
		showChangeNameView();
		return;
	}

	//处理神秘宝箱
	if(pItem->getBaseInfo()->getItemId() == 10097) {//神秘宝箱ID
		BoxEffect* pEffect = BoxEffect::create(pItem);
		pEffect->setCallBack(this,callfuncO_selector(Inventory::onBoxEffectEnd));
		Game::sharedGame()->playEffect(pEffect);
		return;
	}

	//处理宝箱
	if(pItem->getBaseInfo()->getItemType() == item::kItemTypeBox) {
		item::ItemInfo *pKey = DM_GET_ITEM_MODEL->getItemInfoByID(pItem->getBaseInfo()->getUseItemId());
		if(pKey && pKey->getNum() > 0){
			//能打开有钥匙
			BoxEffect* pEffect = BoxEffect::create(pItem);
			pEffect->setCallBack(this,callfuncO_selector(Inventory::onBoxEffectEnd));
			Game::sharedGame()->playEffect(pEffect);
			return;
		}		
	}
    
    if (pItem->getBaseInfo()->getItemType() == item::kItemType200) {
        RedPacket200View* packetView = RedPacket200View::create();
        packetView->setPacketId(pItem->getItemId());
        Game::sharedGame()->openPopup(packetView);
        return;
    }

	//一般处理
	DM_GET_ITEM_MODEL->useItem(pItem);
}

void Inventory::onBoxEffectEnd(CCObject* obj)
{
	//开箱子动画播放完毕，请求掉落
	BoxEffect * effect = dynamic_cast<BoxEffect *>(obj);
	item::ItemInfo * info = effect->getItemInfo();
	DM_GET_ITEM_MODEL->useItem(info);
}

void Inventory::showChangeNameView()
{
	//m_pPopupContainer->removeAllChildren();
	ChangeName *pChange = ChangeName::create();
	m_pPopupContainer->addChild(pChange);
}

void Inventory::onBackKey()
{
	if (m_pPopupContainer && m_pPopupContainer->getChildrenCount())
	{
		m_pPopupContainer->removeAllChildren();
		return;
	}

	BackKeyInterface::onBackKey();
}

/************************ inventory class end*****************************/



/************************ inventory cell class begin**********************/

SelectItemViewCell::SelectItemViewCell()
:m_pItemInfo(NULL)
,m_pAvatorContainer(NULL)
,m_pName(NULL)
,m_pDesc(NULL)
//,m_pForce(0)
,m_pUseFont(NULL)
,m_pAmount(0)
,m_pDelegate(NULL)
,m_pApply(NULL)
,m_pUseCompoundFont(NULL)
,m_pUseTenFont(NULL)
,m_pGradeBg(NULL)
,m_pGradeGreen(NULL)
,m_pGradeBlue(NULL)
,m_pGradePurple(NULL)
,m_pGradeOrange(NULL)
,m_pDaojuNode(NULL)
,m_pSuipianNode(NULL)
,m_pButton(NULL)
,m_pSpriteTen(NULL)
,m_pOpen(NULL)
,m_pSpriteUse(NULL)
,m_pSpriteBuild(NULL)
,m_pSpriteNotEnough(NULL)
,m_pSelectView(NULL)
,m_pInfoContainer(NULL)
{

}


SelectItemViewCell::~SelectItemViewCell()
{
    CC_SAFE_RELEASE(m_pAvatorContainer);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pDesc);
	CC_SAFE_RELEASE(m_pAmount);
    CC_SAFE_RELEASE(m_pUseFont);
    CC_SAFE_RELEASE(m_pUseCompoundFont);
    CC_SAFE_RELEASE(m_pUseTenFont);
    CC_SAFE_RELEASE(m_pApply);
    CC_SAFE_RELEASE(m_pGradeBg);
	CC_SAFE_RELEASE(m_pGradeGreen);
	CC_SAFE_RELEASE(m_pGradeBlue);
	CC_SAFE_RELEASE(m_pGradePurple);
	CC_SAFE_RELEASE(m_pGradeOrange);
	CC_SAFE_RELEASE(m_pDaojuNode);
	CC_SAFE_RELEASE(m_pSuipianNode);
	CC_SAFE_RELEASE(m_pButton);
	CC_SAFE_RELEASE(m_pSpriteTen);
	CC_SAFE_RELEASE(m_pOpen);
	CC_SAFE_RELEASE(m_pSpriteUse);
	CC_SAFE_RELEASE(m_pSpriteBuild);
	CC_SAFE_RELEASE(m_pSpriteNotEnough);
	CC_SAFE_RELEASE(m_pInfoContainer);
	SAFE_RELEASE_UNBINDING_ALL(m_pItemInfo, this);

}

SelectItemViewCell* SelectItemViewCell::create(cocos2d::extension::CCData* pData)
{
    SelectItemViewCell* pRet = new SelectItemViewCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool SelectItemViewCell::init(cocos2d::extension::CCData* pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);

	m_pButton->getButton()->setTarget(this, menu_selector(SelectItemViewCell::useCallBack));

	return true;
}

void SelectItemViewCell::setItemInfo(item::ItemInfo *pItemInfo)
{
	/*if(!pItemInfo || pItemInfo == m_pItemInfo) return;

	SAFE_RELEASE_UNBINDING_ALL(m_pItemInfo, this);
    
    CC_SAFE_RETAIN(pItemInfo);
    m_pItemInfo = pItemInfo;
	BINDING_EXEC(m_pItemInfo,this,SelectItemViewCell::updateCellView,D_EVENT_ITEMINFO_UPDATE);*/

	
	if(!pItemInfo /*|| pItemInfo == m_pItemInfo*/) return;
	CC_SAFE_RETAIN(pItemInfo);
	SAFE_RELEASE_UNBINDING_ALL(m_pItemInfo, this);

	
	m_pItemInfo = pItemInfo;
	BINDING_EXEC(m_pItemInfo,this,SelectItemViewCell::updateCellView,D_EVENT_ITEMINFO_UPDATE);
}

void SelectItemViewCell::updateCellView()
{
	if (!m_pItemInfo || !m_pItemInfo->getBaseInfo() )
	{
		Tips_Error(D_LOCAL_STRING("ItemInfoError").c_str());
		return;
	}

	
    setApplyEnabled(false);
		
    m_pAvatorContainer->removeAllChildren();
    m_pAvatorContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(m_pItemInfo->getItemId()));

	m_pAmount->setString(CCString::createWithFormat("%i",m_pItemInfo->getNum())->getCString());
	ccColor3B fontColor = {255, 0, 0};

    if(m_pItemInfo->getBaseInfo())
    {
        m_pName->setString(m_pItemInfo->getBaseInfo()->getItemName().c_str());
        m_pDesc->setString(m_pItemInfo->getBaseInfo()->getItemDesc().c_str());
        if(m_pItemInfo->getBaseInfo()->getCanBeUsed())
        {
            //m_pApply->setVisible(true);
			m_pButton->setVisible(true);
            switch (m_pItemInfo->getBaseInfo()->getItemType()) {
                case item::kItemTypeBox://bao xiang
                    //m_pUseTenFont->setVisible(true);
					m_pDaojuNode->setVisible(true);
					m_pSuipianNode->setVisible(false);
					m_pOpen->setVisible(true);
					m_pSpriteTen->setVisible(false);
					m_pSpriteUse->setVisible(false);

					//10
					if(m_pItemInfo->getNum() >= 10)
					{
						item::ItemInfo *pItem = DM_GET_ITEM_MODEL->getItemInfoByID(m_pItemInfo->getBaseInfo()->getUseItemId());
						if(pItem && pItem->getNum() >= 10)
						{
							m_pSpriteTen->setVisible(true);
							m_pOpen->setVisible(false);
						}
					}
					break;
                case item::kItemTypeFragment://sui pian

					m_pAvatorContainer->removeAllChildren();
					m_pAvatorContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon50PForList(m_pItemInfo->getBaseInfo()->getResID()));

                    //m_pUseCompoundFont->setVisible(true);
					m_pDaojuNode->setVisible(false);
					m_pSuipianNode->setVisible(true);
					m_pSpriteBuild->setVisible(true);
					m_pSpriteNotEnough->setVisible(false);
					m_pAmount->setString(CCString::createWithFormat("%d/%d",m_pItemInfo->getNum(),m_pItemInfo->getBaseInfo()->getUseItemNum())->getCString());					

					if(m_pItemInfo->getNum() < m_pItemInfo->getBaseInfo()->getUseItemNum())
					{
						//not enough , can't be build
						m_pButton->getButton()->setEnabled(false);
						//m_pAmount->setColor(fontColor);
						m_pSpriteBuild->setVisible(false);
						m_pSpriteNotEnough->setVisible(true);
					}
					else{
						//can use
						m_pButton->getButton()->setEnabled(true);
					}
                    break;
                default:
                    //m_pUseFont->setVisible(true);
					m_pDaojuNode->setVisible(true);
					m_pSuipianNode->setVisible(false);
					m_pSpriteTen->setVisible(false);
					m_pOpen->setVisible(false);
					m_pSpriteUse->setVisible(true);
                    break;
            }
        }
		//else{
		//	//can't be used,no button
		//	m_pButton->setVisible(false);
		//	m_pDaojuNode->setVisible(false);
		//	m_pSuipianNode->setVisible(false);
		//}
    }
    //m_pGradeBg->setVisible(m_pItemInfo->getBaseInfo()->isNeedShowGradeColor());
    //m_pGradeBg->setColor(DM_GET_ITEM_MODEL->getItemColorByGrade(m_pItemInfo->getBaseInfo()->getItemGrade()));

	//set cell color
	item::ItemGrade itemGrade = m_pItemInfo->getBaseInfo()->getItemGrade();
	this->m_pGradeGreen->setVisible( itemGrade == item::kItemGradeGreen );
	this->m_pGradeBlue->setVisible( itemGrade == item::kItemGradeBlue );
	this->m_pGradePurple->setVisible( itemGrade == item::kItemGradePurple );
	this->m_pGradeOrange->setVisible( itemGrade == item::kItemGradeRed );

    
	
}


void SelectItemViewCell::setApplyEnabled(bool value)
{
	//m_pApply->setEnabled(value);

	//m_pApply->setVisible(value);
	//m_pUseFont->setVisible(value);
    //m_pUseCompoundFont->setVisible(value);
    //m_pUseTenFont->setVisible(value);
	m_pButton->setVisible(value);
	m_pDaojuNode->setVisible(value);
	m_pSuipianNode->setVisible(value);
}

void SelectItemViewCell::viewCallBack(cocos2d::CCObject *sender)
{
    if(m_pDelegate)
    {
        m_pDelegate->showInfoView(kItemInfo, m_pItemInfo);
    }
}

void SelectItemViewCell::useCallBack(cocos2d::CCObject *sender)
{
	//不要亂飛！
	m_pSelectView->m_bNoFly = true;
	if (m_pDelegate)
	{
		m_pDelegate->useItem(m_pItemInfo);
	}
	


// 	if (m_pItemInfo->getItemId() == ITEM_CHANGE_NAME_ID)
// 	{
// 		
// 	}
// 	else
// 	{
// 		DM_GET_ITEM_MODEL->useItem(m_pItemInfo);
// 	}
}


bool SelectItemViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvatorContainer", CCNode *, this->m_pAvatorContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemInfo", CCLabelTTF *, this->m_pDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmount", CCLabelBMFont *, this->m_pAmount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUseFont", CCNode *, this->m_pUseFont);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUseTenFont", CCNode *, this->m_pUseTenFont);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUseCompoundFont", CCNode *, this->m_pUseCompoundFont);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pApply", CCMenuItem *, this->m_pApply);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBg", CCSprite *, this->m_pGradeBg);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeGreen", CCNode *, this->m_pGradeGreen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBlue", CCNode *, this->m_pGradeBlue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradePurple", CCNode *, this->m_pGradePurple);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeOrange", CCNode *, this->m_pGradeOrange);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDaojuNode", CCNode *, this->m_pDaojuNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSuipianNode", CCNode *, this->m_pSuipianNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, this->m_pButton);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteUse", CCSprite *, this->m_pSpriteUse);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteTen", CCSprite *, this->m_pSpriteTen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOpen", CCSprite *, this->m_pOpen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteBuild", CCSprite *, this->m_pSpriteBuild);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteNotEnough", CCSprite *, this->m_pSpriteNotEnough);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pInfoContainer", CCNode *, this->m_pInfoContainer);
    
    return false;
}

cocos2d::SEL_MenuHandler SelectItemViewCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "viewCallBack", SelectItemViewCell::viewCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "useCallBack", SelectItemViewCell::useCallBack);
    return NULL;
}

void SelectItemViewCell::adjust(bool last)
{
	float dy = last ? INVENTORY_CELL_SIZE1.height - INVENTORY_CELL_SIZE.height : 0.0f;
	m_pInfoContainer->setPositionY(dy);
}
/************************ inventory cell class end**********************/




/************************ inventory select class begin*******************/

SelectItemView::SelectItemView()
:m_pListView(NULL)
,m_pDelegate(NULL)
,m_pItemModel(NULL)
,m_pCCData(NULL)
,m_uSheet(0)
,m_bNoFly(false)
{

}

SelectItemView::~SelectItemView()
{
    CC_SAFE_RELEASE(m_pCCData);
}

SelectItemView* SelectItemView::create(uint32_t sheet,const cocos2d::CCSize& size)
{
    SelectItemView* pRet = new SelectItemView();
    if(pRet && pRet->init(sheet,size))
    {
        pRet->autorelease();
        return pRet;
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

bool SelectItemView::init(uint32_t sheet,const cocos2d::CCSize& size)
{
	if(!DLayer::init())
	{
		return false;
	}

    m_uSheet = sheet;
    
	m_pItemModel = DM_GET_ITEM_MODEL;
    
    //m_pCCData = readCCData("ui/ccb/inventory/inventoryCell");
	m_pCCData = readCCData("PL_ui/ccb/pl_beibao/PL_beibao_daoju_cell");
    CC_SAFE_RETAIN(m_pCCData);
    
	m_pListView = DTableView::create(this, size);
	m_pListView->setFlyInStyle(kCCTableViewFlyInOnEnter);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	this->addChild(m_pListView);
    
	
	return true;
}


void SelectItemView::onEnter()
{
	
	BINDING_EXEC(m_pItemModel,this,SelectItemView::updateItemList,D_EVENT_ITEMLIST_UPDATE);
	DLayer::onEnter();

}

void SelectItemView::onExit()
{

	UNBINDING_ALL(m_pItemModel,this);
	DLayer::onExit();

}

void SelectItemView::updateItemList()
{
	if(!m_pItemModel->isSynced)
		return;
	m_pItemModel->getItemsBySheet(m_uSheet, &m_ItemDataList);

	/************************************************************************/
	/* 寳箱放在最上面                                                                     */
	/************************************************************************/
	CCObject *pBlueBox = NULL;
	CCObject *pPurpleBox = NULL;
	CCObject *pOrangeBox = NULL;
	CCObject *pBlueKey = NULL;
	CCObject *pPurpleKey = NULL;
	CCObject *pOrangeKey = NULL;

	item::ItemInfo *pIter = NULL;
	CCObject* obj = NULL;
	CCARRAY_FOREACH(&m_ItemDataList, obj)
	{
		pIter = dynamic_cast<item::ItemInfo*>(obj);
		if(pIter->getItemId() == 10010)
			pBlueBox = obj;
		if(pIter->getItemId() == 10009)
			pPurpleBox = obj;
		if(pIter->getItemId() == 10008)
			pOrangeBox = obj;
		if(pIter->getItemId() == 10007)
			pBlueKey = obj;
		if(pIter->getItemId() == 10006)
			pPurpleKey = obj;
		if(pIter->getItemId() == 10005)
			pOrangeKey = obj;
	}
	if(pBlueKey) {m_ItemDataList.removeObject(pBlueKey);m_ItemDataList.insertObject(pBlueKey , 0);}
	if(pBlueBox) {m_ItemDataList.removeObject(pBlueBox);m_ItemDataList.insertObject(pBlueBox , 0);}
	if(pPurpleKey) {m_ItemDataList.removeObject(pPurpleKey);m_ItemDataList.insertObject(pPurpleKey , 0);}
	if(pPurpleBox) {m_ItemDataList.removeObject(pPurpleBox);m_ItemDataList.insertObject(pPurpleBox , 0);}
	if(pOrangeKey) {m_ItemDataList.removeObject(pOrangeKey);m_ItemDataList.insertObject(pOrangeKey , 0);}
	if(pOrangeBox) {m_ItemDataList.removeObject(pOrangeBox);m_ItemDataList.insertObject(pOrangeBox , 0);}
	/************************************************************************/
	/* END                                                                     */
	/************************************************************************/

	if(m_bNoFly == true){
		m_bNoFly = false;
		m_pListView->reloadData();
	}
	else{
		m_pListView->reloadData();
	}
}


void SelectItemView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	//CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize SelectItemView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	if(m_ItemDataList.count() == idx + 1)
	{
		return INVENTORY_CELL_SIZE1;
	}
	return INVENTORY_CELL_SIZE;
}

CCTableViewCell* SelectItemView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	SelectItemViewCell *cell = (SelectItemViewCell*)table->dequeueCell();
	if (!cell) 
	{
		cell = SelectItemViewCell::create(m_pCCData);
	}
	
	item::ItemInfo *pItem = (item::ItemInfo *)m_ItemDataList.objectAtIndex(idx);
	cell->setDelegate(m_pDelegate);
    cell->setItemInfo(pItem);
	cell->setTableView(this);
	cell->adjust(idx == m_ItemDataList.count() - 1);
	return cell;
}

uint32_t SelectItemView::numberOfCellsInTableView(CCTableView *table)
{
	return m_ItemDataList.count();
}





/************************ inventory select class end*******************/

