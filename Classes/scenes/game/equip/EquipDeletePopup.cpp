#include "EquipDeletePopup.h"
#include "Equip.h"
#include "managers/ResourceManager.h"
#include "../formation/Formation.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"
#include "../common/common.h"

USING_NS_CC;
USING_NS_CC_EXT;


EquipDeletePopup::EquipDeletePopup()
	:m_pDelegate(NULL)
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
	,m_pEnhanceOnly(NULL)
	,m_pEhanceAndChange(NULL)
	,m_pEnhanceButton0(NULL)
	,m_pEnhanceButton(NULL)
	,m_pRefineButton0(NULL)
	,m_pRefineButton(NULL)
	,m_pChangeButton(NULL)
	,m_pFateContainer(NULL)
	,m_pFateList(NULL)
	,m_pFateListView(NULL)
	,m_pWarriorModel(NULL)
	,m_pFateCellData(NULL)
	,m_pCloseOnly(NULL)
	,m_pCloseButton(NULL)
	,m_pConfirmButton(NULL)
{
	//grade map
	m_pGradeSelected[equip::kEquipGradeGreen] = false;
	m_pGradeSelected[equip::kEquipGradeBlue] = false;
	m_pGradeSelected[equip::kEquipGradePurple] = false;
	m_pGradeSelected[equip::kEquipGradeRed] = false;
}

EquipDeletePopup::~EquipDeletePopup()
{
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipCurInfo, this);
	CC_SAFE_RELEASE(m_pContainer);
	//CC_SAFE_RELEASE(m_pGrade);

	/*CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pDesc);
	CC_SAFE_RELEASE(m_pEquipUpLevel);
	CC_SAFE_RELEASE(m_pProperty);

	CC_SAFE_RELEASE(m_pEnhanceOnly);
	CC_SAFE_RELEASE(m_pEhanceAndChange);

	CC_SAFE_RELEASE(m_pEnhanceButton0);
	CC_SAFE_RELEASE(m_pEnhanceButton);
	CC_SAFE_RELEASE(m_pRefineButton0);
	CC_SAFE_RELEASE(m_pRefineButton);
	CC_SAFE_RELEASE(m_pChangeButton);
	CC_SAFE_RELEASE(m_pFateContainer);
	CC_SAFE_RELEASE(m_pCloseOnly);*/

	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pConfirmButton);



	CC_SAFE_RELEASE(m_pFateListView);
	CC_SAFE_RELEASE(m_pFateList);
	CC_SAFE_RELEASE(m_pFateCellData);
}

bool EquipDeletePopup::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, this->m_pConfirmButton);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);

	return false;
}

SEL_MenuHandler EquipDeletePopup::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onClick", EquipDeletePopup::onClickTest);
	return NULL;
}

EquipDeletePopup * EquipDeletePopup::create(EquipInfoType type)
{
	EquipDeletePopup * pEquipInfo = new EquipDeletePopup();

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

bool EquipDeletePopup::init()
{
	return init(kTypeEnhanceOnly);
}


bool EquipDeletePopup::init(EquipInfoType type)
{
	if(!DLayer::init()) return false;

	//ccColor4B c = ccc4(0,0,0,100);
	//CCLayerColor* l = CCLayerColor::create(c);
	//addChild(l);

	m_pEquipModel = DM_GET_EQUIP_MODEL;
	m_pWarriorModel = DM_GET_WARRIOR_MODEL;

	//CCB_READER_AND_ADDCHILD("ui/ccb/equip/equipInfo");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang");	

	m_pCloseButton->getButton()->setTarget(this, menu_selector(EquipDeletePopup::closeCallBack));
	m_pConfirmButton->getButton()->setTarget(this, menu_selector(EquipDeletePopup::confirmCallBack));

	m_pFateList = CCArray::create();
	CC_SAFE_RETAIN(m_pFateList);

	m_pFateCellData = readCCData("PL_ui/ccb/pl_beibao/PL_pinzhi_btn");
	CC_SAFE_RETAIN(m_pFateCellData);

	m_pFateListView = DTableView::create(this, m_pContainer->getContentSize());
	m_pFateListView->setDirection(kCCScrollViewDirectionVertical);
	m_pFateListView->setDelegate(this);
	m_pContainer->addChild(m_pFateListView);
	CC_SAFE_RETAIN(m_pFateListView);

	m_type = type;

	/*CCLayerColor * newlayer = CCLayerColor::create(ccc4(100,100,100,100));
	addChild(newlayer);*/

	return true;
}

void EquipDeletePopup::onEnter()
{
	binding();
	DLayer::onEnter();
}

void EquipDeletePopup::setEquip(std::map<equip::EquipGrade, bool>& mp)
{
	m_pGradeList.clear();
	//for each
	for (std::map<equip::EquipGrade, bool>::iterator iter = mp.begin(); iter != mp.end(); iter++)
	{
		if(iter->second == true){
			m_pGradeList.push_back(iter->first);
		}
	}
	//add total button at last
	m_pGradeList.push_back(equip::kEquipGradeTotal);
}

void EquipDeletePopup::updateStatus()
{

}


void EquipDeletePopup::binding()
{
	BINDING_EXEC(m_pEquipCurInfo,this,EquipDeletePopup::updateEquipInfo,D_EVENT_EQUIPINFO_UPDATE);
}



void EquipDeletePopup::onExit()
{
	UNBINDING_ALL(m_pEquipCurInfo,this);
	DLayer::onExit();
}


void EquipDeletePopup::updateEquipInfo()
{
	m_pFateListView->reloadData();
}

void EquipDeletePopup::closeCallBack(cocos2d::CCObject * obj)
{
	//removeFromParent();
	this->setVisible(false);
}

void EquipDeletePopup::confirmCallBack(cocos2d::CCObject * obj)
{
	removeFromParent();

	std::vector<equip::EquipGrade> selectedList;
	if(m_pGradeSelected[equip::kEquipGradeGreen]) selectedList.push_back(equip::kEquipGradeGreen);
	if(m_pGradeSelected[equip::kEquipGradeBlue]) selectedList.push_back(equip::kEquipGradeBlue);
	if(m_pGradeSelected[equip::kEquipGradePurple]) selectedList.push_back(equip::kEquipGradePurple);
	if(m_pGradeSelected[equip::kEquipGradeRed]) selectedList.push_back(equip::kEquipGradeRed);

	m_pDelegate->onGradeSelect(selectedList);
}

CCSize EquipDeletePopup::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return CCSizeMake(352, 76);
}

#define tag 0
CCTableViewCell* EquipDeletePopup::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	EquipGradeCell *cell = (EquipGradeCell*)table->dequeueCell();
	if (!cell)
	{
		cell = EquipGradeCell::create(m_pFateCellData);
		cell->setDelegate(this);
	}
	int count = m_pGradeList.size();
	cell->setType(m_pGradeList[count - idx - 1]);
	//初始化不打勾
	cell->setSelected(m_pGradeSelected[m_pGradeList[count - idx - 1]]);

	return cell;
}

uint32_t EquipDeletePopup::numberOfCellsInTableView(CCTableView *table)
{
	//return m_pFateList->count();
	int count = m_pGradeList.size();
	return count;
}

void EquipDeletePopup::setGradeByTag(int t , bool b){

	switch(t){
	case 0:
		m_pGradeSelected[equip::kEquipGradeGreen] = b;
		m_pGradeSelected[equip::kEquipGradeBlue] = b;
		m_pGradeSelected[equip::kEquipGradePurple] = b;
		m_pGradeSelected[equip::kEquipGradeRed] = b;
		updateEquipInfo();
		break;
	case 1:
		m_pGradeSelected[equip::kEquipGradeGreen] = b;
		break;
	case 2:
		m_pGradeSelected[equip::kEquipGradeBlue] = b;
		break;
	case 3:
		m_pGradeSelected[equip::kEquipGradePurple] = b;
		break;
	case 4:
		m_pGradeSelected[equip::kEquipGradeRed] = b;
		break;
	}
}





EquipGradeCell::EquipGradeCell()
:m_pEquipInfo(NULL)
,m_pEquipListTableDelegate(NULL)
,m_pGrade(NULL)
,m_pName(NULL)
,m_pLevel(NULL)
,m_pEquipUpLevel(NULL)
,m_pProperty(NULL)
,m_pSelectButton(NULL)
,m_pUnEquip(NULL)
,m_pEquiped(NULL)
,m_pEquipedOn(NULL)
,m_pGradeBg(NULL)
,m_pContainer(NULL)
,m_pFateMark(NULL)
,m_pSellNode(NULL)
,m_pEnhanceNode(NULL)
,m_pGreenButton(NULL)
,m_pBlueButton(NULL)
,m_pPurpleButton(NULL)
,m_pOrangeButton(NULL)
,m_pTotalButton(NULL)
,m_pCheckBox(NULL)
{
}

EquipGradeCell::~EquipGradeCell()
{
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipInfo, this);
    
    
    CC_SAFE_RELEASE(m_pContainer);
    
    CC_SAFE_RELEASE(m_pGrade);
	CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pEquipUpLevel);
    
    CC_SAFE_RELEASE(m_pProperty);
    
    CC_SAFE_RELEASE(m_pSelectButton);
    
    CC_SAFE_RELEASE(m_pUnEquip);
    CC_SAFE_RELEASE(m_pEquiped);
    CC_SAFE_RELEASE(m_pEquipedOn);
    CC_SAFE_RELEASE(m_pGradeBg);
    
    CC_SAFE_RELEASE(m_pFateMark);

	CC_SAFE_RELEASE(m_pSellNode);
	CC_SAFE_RELEASE(m_pEnhanceNode);

	CC_SAFE_RELEASE(m_pGreenButton);
	CC_SAFE_RELEASE(m_pBlueButton);
	CC_SAFE_RELEASE(m_pPurpleButton);
	CC_SAFE_RELEASE(m_pOrangeButton);
	CC_SAFE_RELEASE(m_pTotalButton);
    CC_SAFE_RELEASE(m_pCheckBox);
}

EquipGradeCell* EquipGradeCell::create(cocos2d::extension::CCData* pData)
{
    EquipGradeCell* pRet = new EquipGradeCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

bool EquipGradeCell::init(cocos2d::extension::CCData* pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);

	m_pCheckBox->setTarget(this, menu_selector(EquipGradeCell::onCheckBoxClick));
	return true;
}

bool EquipGradeCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGreenButton", CCNode *, this->m_pGreenButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBlueButton", CCNode *, this->m_pBlueButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPurpleButton", CCNode *, this->m_pPurpleButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOrangeButton", CCNode *, this->m_pOrangeButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTotalButton", CCNode *, this->m_pTotalButton);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCheckBox", CCMenuItem *, this->m_pCheckBox);

    return false;
    
}

void EquipGradeCell::setType(equip::EquipGrade type)
{
	//
	m_pGreenButton->setVisible( type == equip::kEquipGradeGreen );
	m_pBlueButton->setVisible( type == equip::kEquipGradeBlue );
	m_pPurpleButton->setVisible( type == equip::kEquipGradePurple );
	m_pOrangeButton->setVisible( type == equip::kEquipGradeRed );
	m_pTotalButton->setVisible( type == equip::kEquipGradeTotal );
}

cocos2d::SEL_MenuHandler EquipGradeCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onClick", EquipGradeCell::onClickTest);

	return NULL;
}

void EquipGradeCell::onClickTest(cocos2d::CCObject* sender){

	CCNode* node = dynamic_cast<CCNode*>(sender);
	int buttonTag = node->getTag();
	bool v = m_pCheckBox->isVisible();
	
	//getFather()->setGradeByTag(buttonTag , !v);
	m_pEquipListTableDelegate->setGradeByTag(buttonTag , !v);
	m_pCheckBox->setVisible(!v);
}

void EquipGradeCell::onCheckBoxClick(cocos2d::CCObject* sender){

	int buttonTag = 0;
	if(m_pGreenButton->isVisible()) buttonTag = 1;
	if(m_pBlueButton->isVisible()) buttonTag = 2;
	if(m_pPurpleButton->isVisible()) buttonTag = 3;
	if(m_pOrangeButton->isVisible()) buttonTag = 4;
	if(m_pTotalButton->isVisible()) buttonTag = 0;

	//getFather()->setGradeByTag(buttonTag , false);
	m_pEquipListTableDelegate->setGradeByTag(buttonTag , false);
	m_pCheckBox->setVisible(false);

}

void EquipGradeCell::updateCellView()
{
	 
}

void EquipGradeCell::setSelected(bool val)
{
    val?m_pCheckBox->setVisible(val):m_pCheckBox->setVisible(val);
}

