#include "200RedPacket.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"
#include "model/ItemModel.h"
#include "model/MallModel.h"
#include "model/WarriorModel.h"
#include "model/EquipModel.h"
#include "../equip/EquipEnhance.h"

//
//RedPacket200View
//
RedPacket200View::RedPacket200View()
	: m_uPacketId(0)
    ,m_pProducts(NULL)
    ,m_pSelectProductId(0)
	,m_pItemModel(NULL)
    ,m_pWarriorModel(NULL)
    ,m_pEquipModel(NULL)
    ,m_pMallModel(NULL)
    ,m_pTableView(NULL)
    ,m_pWarriorNode(NULL)
    ,m_pEquipNode(NULL)
    ,m_pItemNode(NULL)
	,m_pTableContainer(NULL)
    ,m_pWarriorGradeBg(NULL)
    ,m_pCloseButton(NULL)
    ,m_pPrice(NULL)
    ,m_pWarriorContainer(NULL)
    ,m_pZizhiNum(NULL)
    ,m_pWarriorCapacity(NULL)
    ,m_pWarriorTypeMark(NULL)
    ,m_pName(NULL)
    ,m_pEquipCapacity(NULL)
    ,m_pEquipProperty(NULL)
    ,m_pEquipName(NULL)
    ,m_pEquipLevel(NULL)
    ,m_pEquipZizhiNum(NULL)
    ,m_pEquipContainer(NULL)
    ,m_pItemContainer(NULL)
    ,m_pPieceIcon(NULL)
    ,m_pItemName(NULL)
    ,m_pSoulNode(NULL)
    ,m_pItemAmount(NULL)
{
    D_RESET_C_ARRAY(m_pPropertyType, equip::kEquipPropTotal);
    D_RESET_C_ARRAY(m_pEquipMark, equip::kEquipPropTotal);
}

RedPacket200View::~RedPacket200View()
{
	CC_SAFE_RELEASE_NULL(m_pWarriorNode);
	CC_SAFE_RELEASE_NULL(m_pEquipNode);
	CC_SAFE_RELEASE_NULL(m_pItemNode);
    
    CC_SAFE_RELEASE_NULL(m_pTableContainer);
    CC_SAFE_RELEASE_NULL(m_pWarriorGradeBg);
    CC_SAFE_RELEASE_NULL(m_pCloseButton);
    CC_SAFE_RELEASE_NULL(m_pPrice);
    CC_SAFE_RELEASE_NULL(m_pWarriorContainer);
    CC_SAFE_RELEASE_NULL(m_pZizhiNum);
    CC_SAFE_RELEASE_NULL(m_pWarriorCapacity);
    CC_SAFE_RELEASE_NULL(m_pWarriorTypeMark);
    CC_SAFE_RELEASE_NULL(m_pName);
    CC_SAFE_RELEASE_NULL(m_pEquipCapacity);
    CC_SAFE_RELEASE_NULL(m_pEquipProperty);
    CC_SAFE_RELEASE_NULL(m_pEquipName);
    CC_SAFE_RELEASE_NULL(m_pEquipLevel);
    CC_SAFE_RELEASE_NULL(m_pEquipZizhiNum);
    CC_SAFE_RELEASE_NULL(m_pEquipContainer);
    CC_SAFE_RELEASE_NULL(m_pItemContainer);
    CC_SAFE_RELEASE_NULL(m_pPieceIcon);
    CC_SAFE_RELEASE_NULL(m_pItemName);
    CC_SAFE_RELEASE_NULL(m_pSoulNode);
    CC_SAFE_RELEASE_NULL(m_pItemAmount);
    
    D_SAFE_RELEASE_C_ARRAY(m_pPropertyType, equip::kEquipPropTotal);
    D_SAFE_RELEASE_C_ARRAY(m_pEquipMark, equip::kEquipPropTotal);
}

void RedPacket200View::setPacketId(unsigned int packetId)
{
    m_uPacketId = packetId;
    m_pProducts = m_pMallModel->getRedPacketListById(packetId);
    m_pTableView->reloadData();
}

bool RedPacket200View::init()
{
    if (!DPopup::init())
    {
        return false;
    }
    
	m_pItemModel = DM_GET_ITEM_MODEL;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    m_pEquipModel = DM_GET_EQUIP_MODEL;
    m_pMallModel = DM_GET_MALL_MODEL;
    
	 CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_200hb/pl_200hb");

	 m_pTableView = DTableView::create(this, m_pTableContainer->getContentSize());
	 m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	 m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	 m_pTableView->setAnchorPoint(CCPointZero);
	 m_pTableView->setPosition(CCPointZero);
	 m_pTableView->setDelegate(this);
	 m_pTableView->setFlyInStyle(kCCTableViewFlyInDisable);
	 m_pTableContainer->addChild(m_pTableView);
    
    m_pCloseButton->getButton()->setTarget(this, menu_selector(RedPacket200View::closeCallBack));
	 return true;
}

void RedPacket200View::onEnter()
{
    DPopup::onEnter();
    
    CCInteger* integerId = dynamic_cast<CCInteger*>(m_pProducts->objectAtIndex(0));
    m_pSelectProductId = integerId->getValue();
	updateInfoView();
}

void RedPacket200View::onExit()
{
    DPopup::onExit();
}

bool RedPacket200View::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableContainer", CCNode *, this->m_pTableContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorNode", CCNode *, this->m_pWarriorNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipNode", CCNode *, this->m_pEquipNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemNode", CCNode *, this->m_pItemNode);
    
    D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pEquipType",CCSprite*,this->m_pPropertyType,equip::kEquipPropTotal);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pEquipMark",CCSprite*,this->m_pEquipMark,equip::kEquipPropTotal);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg *, this->m_pWarriorGradeBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorContainer", CCNode *, this->m_pWarriorContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPrice", CCLabelBMFont*, this->m_pPrice);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZizhiNum", CCLabelBMFont *, this->m_pZizhiNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorCapacity", CCLabelBMFont *, this->m_pWarriorCapacity);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipCapacity", CCLabelBMFont *, this->m_pEquipCapacity);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipProperty", CCLabelBMFont *, this->m_pEquipProperty);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipName", CCLabelTTF *, this->m_pEquipName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipLevel", CCLabelBMFont *, this->m_pEquipLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipZizhiNum", CCLabelBMFont *, this->m_pEquipZizhiNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipContainer", CCNode *, this->m_pEquipContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemContainer", CCNode *, this->m_pItemContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPieceIcon", CCSprite *, this->m_pPieceIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemName", CCLabelTTF *, this->m_pItemName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoulNode", CCNode *, this->m_pSoulNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemAmount", CCLabelBMFont *, this->m_pItemAmount);

	return false;
}

cocos2d::SEL_MenuHandler RedPacket200View::onResolveCCBCCMenuItemSelector( cocos2d::CCObject * pTarget, const char* pSelectorName )
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "purchaseCallBack", RedPacket200View::purchase);
    
	return NULL;
}

void RedPacket200View::purchase(cocos2d::CCObject *obj)
{
    HTTP_CLIENT->buy200RedPacketProduct(m_uPacketId, m_pSelectProductId, this,callfuncJson_selector(RedPacket200View::purchaseCallBack));
}

void RedPacket200View::purchaseCallBack(const Json::Value &requestData,const Json::Value &responseData)
{
    m_pItemModel->reduceItemNumByID(m_uPacketId);
    closePopup();
}

void RedPacket200View::closeCallBack( cocos2d::CCObject * obj )
{
	closePopup();
}

void RedPacket200View::updateInfoView()
{
    mall::ProductInfo* info = m_pMallModel->getProductById(m_pSelectProductId);
	if (info == NULL)
	{
        CCLOG("error product info in RedPacket200View update.");
		return;
	}
    
    unsigned int id = info->getBaseInfo()->getItemId();
	CCNode* pDisplayNode = NULL;
	switch (info->getBaseInfo()->getType())
	{
	case mall::kProductTypeItem:
            pDisplayNode = m_pItemNode;
            updateItemInfo(id);
            break;
	case mall::kProductTypeEquip:
            pDisplayNode = m_pEquipNode;
            updateEquipInfo(id);
            break;
	case mall::kProductTypeWarrior:
            pDisplayNode = m_pWarriorNode;
            updateWarriorInfo(id);
    case mall::kProductTypeSoul:
            pDisplayNode = m_pWarriorNode;
            updateWarriorInfo(id, info->getBaseInfo()->getType() == mall::kProductTypeSoul);
            break;
	default:
            return;
	}

	if(pDisplayNode == NULL)
	{
		return;
	}

	m_pWarriorNode->setVisible(false);
	m_pEquipNode->setVisible(false);
	m_pItemNode->setVisible(false);
	pDisplayNode->setVisible(true);
    
    m_pPrice->setString(CCString::createWithFormat("%d", info->getPrice())->getCString());
}

void RedPacket200View::updateWarriorInfo( unsigned int id , bool isSoul/* = false */)
{
    warrior::WarriorBaseInfo* baseInfo = m_pWarriorModel->getWarriorBaseByWID(id);
	if (baseInfo == NULL)
	{
		return;
	}
    
    warrior::WarriorCurInfo* info = warrior::WarriorCurInfo::create(baseInfo);
    if(info == NULL)
    {
        return;
    }

    m_pWarriorContainer->removeAllChildren();
    m_pWarriorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon(info->getBaseInfo()->getWarriorId(), 0.8f));
    m_pZizhiNum->setString(CCString::createWithFormat("%d", info->getBaseInfo()->getQuality())->getCString());
    m_pWarriorCapacity->setString(CCString::createWithFormat("%d", info->computeWarriorForce())->getCString());
    m_pWarriorTypeMark->setWarriorInfo(info->getBaseInfo());
    m_pName->setString(info->getBaseInfo()->getWarriorName().c_str());
    m_pSoulNode->setVisible(isSoul);
    
    m_pWarriorGradeBg->setWarriorGrade(info->getBaseInfo()->getColorGrade());
}

void RedPacket200View::updateEquipInfo( unsigned int id )
{
    equip::EquipBaseInfo* baseInfo = m_pEquipModel->getEquipBaseInfoByID(id);
    if (baseInfo == NULL)
    {
        return;
    }
    
    equip::EquipCurInfo* info = equip::EquipCurInfo::create(baseInfo);
	if (info == NULL)
	{
		return;
	}
    
    int capacity = m_pEquipModel->computeEquipFight(info);
    m_pEquipCapacity->setString(CCString::createWithFormat("%d", capacity)->getCString());
    
    equip::EquipPropType type = info->getBaseInfo()->getPropertyType();
    
    std::string suffix;
    switch (type) {
        case equip::kEquipPropDodge:
        case equip::kEquipPropCritical:
        case equip::kEquipPropBlock:
        case equip::kEquipPropHit:
            suffix = "%";
        default:
            break;
    }
    
    int property = m_pEquipModel->computeEquipInfo(info,type);
    m_pEquipProperty->setString(CCString::createWithFormat("%d%s", property, suffix.c_str())->getCString());
    
    for (uint8_t i =0; i<equip::kEquipPropTotal; ++i)
    {
        if(m_pPropertyType[i]) m_pPropertyType[i]->setVisible(i==type);
        if(m_pEquipMark[i]) m_pEquipMark[i]->setVisible(i==info->getBaseInfo()->getEquipType() - 1);
    }

    m_pEquipName->setString(info->getBaseInfo()->getEquipName().c_str());
    m_pEquipLevel->setString(CCString::createWithFormat("%i",info->getLevel())->getCString());
    m_pZizhiNum->setString(CCString::createWithFormat("%d",info->getBaseInfo()->getQuality())->getCString());
    m_pEquipContainer->removeAllChildren();
    m_pEquipContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon(info->getEquipId()));
    m_pWarriorGradeBg->setWarriorGrade((warrior::WarriorGrade)info->getBaseInfo()->getEquipGrade());
}

void RedPacket200View::updateItemInfo( unsigned int id )
{
	item::ItemBase* info = m_pItemModel->getItemBaseByID(id);
	if (info == NULL)
	{
		return;
	}
    
    m_pSoulNode->setVisible(false);
    m_pPieceIcon->setVisible(false);
    
    m_pItemContainer->removeAllChildren();
    if(info->getItemType() == item::kItemTypeFragment)
    {
        //处理碎片显示
        m_pItemContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon(info->getResID()));
    }
    else{
        //默认显示
        m_pItemContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon(info->getItemId()));
    }
    
    if (info->getItemType() == item::kItemTypeFragment) {
        m_pPieceIcon->setVisible(true);
    }
    
    m_pWarriorGradeBg->setWarriorGrade(warrior::WarriorGrade(info->getItemGrade()));
}

void RedPacket200View::middleCellChanged( unsigned int index )
{

}

cocos2d::CCSize RedPacket200View::tableCellSizeForIndex( cocos2d::extension::CCTableView *table, unsigned int idx )
{
	return CCSizeMake(280, 80);
}

cocos2d::extension::CCTableViewCell* RedPacket200View::tableCellAtIndex( cocos2d::extension::CCTableView *table, unsigned int idx )
{
	PacketListCell* pCell = (PacketListCell*)table->dequeueCell();
	if (pCell == NULL)
	{
		pCell = PacketListCell::create();
	}

	unsigned productId =((CCInteger*)(m_pProducts->objectAtIndex(idx)))->getValue();
	mall::ProductBase * productBaseInfo = m_pMallModel->getProductBaseByID(productId);
	if (productBaseInfo)
	{
		pCell->setItemName(productBaseInfo->getProductName());
	}

	return pCell;
}

void RedPacket200View::tableCellTouched( cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell )
{
	CCInteger* integerId = dynamic_cast<CCInteger*>(m_pProducts->objectAtIndex(cell->getIdx()));
	if (integerId)
	{
		m_pSelectProductId = integerId->getValue();
		updateInfoView();
	}
}

unsigned int RedPacket200View::numberOfCellsInTableView( cocos2d::extension::CCTableView *table )
{
	if (m_pProducts)
	{
		return m_pProducts->count();
	}

	return 0; 
}


//
//PacketListCell
//
PacketListCell::PacketListCell()
	: m_pName(NULL)
{

}

PacketListCell::~PacketListCell()
{
	CC_SAFE_RELEASE_NULL(m_pName);
}

bool PacketListCell::init()
{
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_200hb/pl_200hb_biaoqian");
    
    return true;
}

bool PacketListCell::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
	return false;
}

cocos2d::SEL_MenuHandler PacketListCell::onResolveCCBCCMenuItemSelector( cocos2d::CCObject * pTarget, const char* pSelectorName )
{
	return NULL;
}

void PacketListCell::updateView()
{
	if (m_pName)
	{
		m_pName->setString(m_itemName.c_str());
	}
}

void PacketListCell::setItemName( const std::string& name )
{
	m_itemName = name;
	updateView();
}
