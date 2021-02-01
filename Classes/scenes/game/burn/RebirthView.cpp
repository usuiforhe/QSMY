//
//  Rebirth
//  英雄重生
//
#include "RebirthView.h"
#include "../common/gui/WarriorGradeBg.h"
#include "../user/UserInfo.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"

USING_NS_CC;
using namespace sp;

bool RebirthWarriorFilter::operator()(warrior::WarriorCurInfo *pWarrior)
{
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

	//初始英雄不可重生
	if(pWarrior->getWarriorLv() == 1 && pWarrior->getSlv()==1 && pWarrior->getWarriorUpLv() ==0)
	{
		return false;
	}
	return true;
}

RebirthView::RebirthView()
	:m_pWarriorPopup(NULL)
	,m_pHideNode(NULL)
	,m_pBook(NULL)
	,m_pConfirmButton(NULL)
	,m_pWarriorSprite(NULL)
	,m_pFight(NULL)
	,m_pName(NULL)
	,m_pWarriorGradeBg(NULL)
	,m_pZizhiNum(NULL)
	,m_pLevel(NULL)
	,m_pWarriorTypeMark(NULL)
	,m_pUpLevel(NULL)
	,m_pSelectBtn(NULL)
	,m_pNoneBox(NULL)
	,m_pHeroBox(NULL)
	,m_pMenuHero(NULL)
	,m_pWarriorInfo(NULL)
	,m_pHeroAwardList(NULL)
	,m_pAwardNode(NULL)
	,m_pBtnBurn(NULL)
	,m_pEffect(NULL)
	,m_pAnimation(NULL)
	,m_pSelectTxt(NULL)
	,m_pReselectTxt(NULL)
{
	for(uint8_t i(0); i < HERO_REBIRTH_ITEM_COUNT; ++i)
	{
		m_pBox[i] = NULL;
	}

	 D_RESET_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}

RebirthView::~RebirthView()
{
	if (m_pWarriorPopup && m_pWarriorPopup->getParent())
	{
		m_pWarriorPopup->removeFromParent();
	}
	for(uint8_t i(0);i<HERO_REBIRTH_ITEM_COUNT;i++)
	{
		CC_SAFE_RELEASE(m_pBox[i]);
	}
	CC_SAFE_RELEASE(m_pWarriorPopup);
	CC_SAFE_RELEASE(m_pHideNode);
	CC_SAFE_RELEASE(m_pBook);
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pWarriorSprite);
	CC_SAFE_RELEASE(m_pFight);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pWarriorGradeBg);
	CC_SAFE_RELEASE(m_pZizhiNum);
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pWarriorTypeMark);
	CC_SAFE_RELEASE(m_pUpLevel);
	CC_SAFE_RELEASE(m_pSelectBtn);
	CC_SAFE_RELEASE(m_pNoneBox);
	CC_SAFE_RELEASE(m_pHeroBox);
	CC_SAFE_RELEASE(m_pMenuHero);
	CC_SAFE_RELEASE_NULL(m_pWarriorInfo);
	CC_SAFE_RELEASE(m_pHeroAwardList);
	CC_SAFE_RELEASE(m_pAwardNode);
	CC_SAFE_RELEASE(m_pBtnBurn);
	CC_SAFE_RELEASE(m_pEffect);
	CC_SAFE_RELEASE(m_pAnimation);
	CC_SAFE_RELEASE(m_pSelectTxt);
	CC_SAFE_RELEASE(m_pReselectTxt);
	D_SAFE_RELEASE_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}

bool RebirthView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_yingxiongchongsheng/PL_yingxiongchongsheng");

	m_pConfirmButton->getButton()->setTarget(this, menu_selector(RebirthView::onBookClick));
	m_pBook->setTarget(this,menu_selector(RebirthView::onBookClick));
	m_pMenuHero->setTarget(this,menu_selector(RebirthView::onOpenBurn));
	m_pSelectBtn->getButton()->setTarget(this,menu_selector(RebirthView::showWarriorSelectList));
	//m_pReselectBtn->setTarget(this,menu_selector(RebirthView::onReselect));
	m_pBtnBurn->getButton()->setTarget(this,menu_selector(RebirthView::onRebirth));

	m_pEffect = ResourceManager::sharedResourceManager()->getUIAnimate("burn");
	CC_SAFE_RETAIN(m_pEffect);
	return true;
}

void RebirthView::onEnter()
{
	setState();
	DLayer::onEnter();
	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView());	//添加用户基本信息

	//播放动画
	m_pEffect->getAnimation()->playByIndex(0);
	m_pEffect->setPosition(D_DESIGN_POINT_CENTER);
	m_pAnimation->addChild(m_pEffect);

	if(!CCUserDefault::sharedUserDefault()->getBoolForKey(HERO_REBIRTH_OPEN_FIRST))
	{
		m_pHideNode->setVisible(true);
		CCUserDefault::sharedUserDefault()->setBoolForKey(HERO_REBIRTH_OPEN_FIRST,true);
	}
}

void RebirthView::onExit()
{
	DLayer::onExit();
}

bool RebirthView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pBtnBurn" , DButton*,m_pBtnBurn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pHideNode",CCNode*,m_pHideNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBook" ,CCMenuItemImage* ,m_pBook);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton",DButton*,m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorSprite",CCNode* ,m_pWarriorSprite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCLabelBMFont* ,m_pFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName" , CCLabelTTF* , m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg",WarriorGradeBg*,m_pWarriorGradeBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZizhiNum" , CCLabelBMFont* ,this->m_pZizhiNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont*,m_pLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark*, m_pWarriorTypeMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCLabelBMFont *, this->m_pUpLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectBtn", DButton*, m_pSelectBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoneBox", CCNode*, m_pNoneBox);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHeroBox" ,CCNode*, m_pHeroBox);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuHero" , CCMenuItemImage*,m_pMenuHero);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReselectBtn",CCMenuItem* ,m_pReselectBtn);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pBox",ItemBox*,this->m_pBox,HERO_REBIRTH_ITEM_COUNT);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pHeroAwardList",CCNode*,m_pHeroAwardList);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pAwardNode",CCNode*,m_pAwardNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimation", CCNode *, this->m_pAnimation);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectTxt", CCNode *, this->m_pSelectTxt);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReselectTxt", CCNode *, this->m_pReselectTxt);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar1", DProgressBar *, this->m_pProgressBar[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar2", DProgressBar *, this->m_pProgressBar[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar", DProgressBar *, this->m_pProgressBar[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar4", DProgressBar *, this->m_pProgressBar[3]);
	return false;
}

void RebirthView::setSelectedWarrior(warrior::WarriorCurInfo * pWarrior)
{
	CC_SAFE_RELEASE_NULL(m_pWarriorInfo);
	m_pWarriorInfo = pWarrior;
	CC_SAFE_RETAIN(m_pWarriorInfo);
	setState();
}

void RebirthView::showWarriorSelectList(cocos2d::CCObject * sender)
{
	if(!m_pWarriorPopup)
	{
		m_pWarriorPopup = WarriorSelectPopup::create(NULL);
		m_pWarriorPopup->setFilter(RebirthWarriorFilter::create());
		m_pWarriorPopup->setDelegate(this);
		CC_SAFE_RETAIN(m_pWarriorPopup);
	}	
	addChild(m_pWarriorPopup);
}

void RebirthView::updateInfo(warrior::WarriorCurInfo * pWarrior)
{
	m_pWarriorSprite->removeAllChildren();
	m_pWarriorSprite->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon(pWarrior->getAvatarId(), 0.8f));
	m_pFight->setString(CCString::createWithFormat("%d", pWarrior->computeWarriorForce())->getCString());
	m_pName->setString(pWarrior->getBaseInfo()->getWarriorName().c_str());
	m_pWarriorGradeBg->setWarriorGrade(pWarrior->getBaseInfo()->getColorGrade());
	m_pLevel->setString(CCString::createWithFormat("%i",pWarrior->getWarriorLv())->getCString());
	if(m_pZizhiNum)
	{
		m_pZizhiNum->setString(CCString::createWithFormat("%d",pWarrior->getBaseInfo()->getQuality())->getCString());
	}
	m_pWarriorTypeMark->setWarriorInfo(pWarrior->getBaseInfo());
	m_pUpLevel->setString(CCString::createWithFormat("%d", pWarrior->getWarriorUpLv())->getCString());

	//设置name底框颜色
	for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
		int color = (int)pWarrior->getBaseInfo()->getColorGrade();
		m_pProgressBar[i]->setVisible( color == i+1);
	}
}

void RebirthView::onBookClick(cocos2d::CCObject * sender)
{
	bool v = m_pHideNode->isVisible();
	m_pHideNode->setVisible(!v);
}

void RebirthView::onReselect(cocos2d::CCObject * sender)
{
	if(m_pWarriorInfo) CC_SAFE_RELEASE_NULL(m_pWarriorInfo);
	setState();
}

void RebirthView::onOpenBurn(cocos2d::CCObject * sender)
{
	Game::sharedGame()->NavigateTo(navigation::kNavBurn);
}

void RebirthView::onRebirth(cocos2d::CCObject * sender)
{
	if(m_pWarriorInfo !=NULL)
	{
		//花钱
		DMessageBox* pPBox = DMessageBox::showMessageBox(D_LOCAL_STRING("$RebornCost").c_str(), this, messageBox_callfunc_selector(RebirthView::confirmBirth));
	}
	else
	{
		Tips_Alert(D_LOCAL_STRING("$LianhuaNoneHero").c_str());
	}
}

void RebirthView::confirmBirth(MessageType type, DMessageBox* target)
{
	if (type == kMessageTypeCancel)
	{
		return;
	}
	//确认了
	m_pEffect->getAnimation()->playByIndex(1);
	m_pEffect->getAnimation()->addObserver(this, SPNotification_callfunc_selector(RebirthView::animateEnd), COMPLETE);
	setMenuTouchEnable(false);
}

void RebirthView::animateEnd(sp::Armature * target, const char* event,const char* data)
{
	HTTP_CLIENT->reBirth(m_pWarriorInfo->getWid(),this,callfuncJson_selector(RebirthView::onBirthBack));
	m_pEffect->getAnimation()->removeAllObservers(this);
	m_pEffect->getAnimation()->playByIndex(0);
	//恢复按钮可以点击
	setMenuTouchEnable(true);

}
void RebirthView::setState()
{
	bool isShow = (m_pWarriorInfo != NULL);
	m_pNoneBox->setVisible(!isShow);
	m_pHeroBox->setVisible(isShow);
	m_pSelectTxt->setVisible(!isShow);
	m_pReselectTxt->setVisible(isShow);
	if(isShow) 
	{
			updateInfo(m_pWarriorInfo);
			HTTP_CLIENT->getRebirthDrop(m_pWarriorInfo->getWid(),this,callfuncJson_selector(RebirthView::getDropItem));
	}
	else
	{
		m_pHeroAwardList->setVisible(false);
	}
	
}

void RebirthView::onBirthBack(const Json::Value &requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	//0.保存 掉落id

	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
	CCArray* warriorlist = DM_GET_WARRIOR_MODEL->getWarriorVec();

	//掉落库在此之前调用（无法更改原有的英雄），因此先删再加
	CCObject* obj = NULL;
	warrior::WarriorCurInfo* wInfo = NULL;
	CCARRAY_FOREACH(warriorlist, obj)
	{
		wInfo = (warrior::WarriorCurInfo*)obj;
		if(wInfo->getWid() == m_pWarriorInfo->getWid())
		{
			warriorlist->removeObject(wInfo);
			break;
		}
	}

	if(responseData["data"].isMember("drops"))
	{
		for (uint32_t i=0; i<responseData["data"]["drops"].size(); ++i)
		{
			drop::DropType dropType = (drop::DropType)responseData["data"]["drops"][i]["type"].asUInt();
			if(dropType == drop::kDropTypeWarrior)
			{
				warrior::WarriorCurInfo *pWarriorCurInfo = DM_GET_WARRIOR_MODEL->parseWarrior(responseData["data"]["drops"][i]["warrior"]);
				DM_GET_WARRIOR_MODEL->addWarrior(pWarriorCurInfo);
				pWarriorCurInfo->computeWarriorProps();
				break;
			}
		}
	}

	CC_SAFE_RELEASE_NULL(m_pWarriorInfo);
	setState();
}
void RebirthView::getDropItem(const Json::Value &requestData,const Json::Value &responseData)
{
	//{"code":"0000","data":{"rewards":[{"type":1,"itemId":10002,"num":85}]},"ts":1419177640}
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;

	int count=0;
	for(int i = 0 ; i < responseData["data"]["rewards"].size() ; i++)
	{
		const Json::Value &val  = responseData["data"]["rewards"][i];
		m_pBox[i]->setVisible(true);
		drop::DropType itemType = static_cast<drop::DropType>(val["type"].asUInt());
		uint32_t itemId = val["itemId"].asUInt();
		int num = val["num"].asUInt();
		m_pBox[i]->updateDropItem(drop::Drop::create(itemType,itemId,num));
		count++;
	}

	for(int i = count;i<HERO_REBIRTH_ITEM_COUNT;i++)
	{
		m_pBox[i]->setVisible(false);
	}

	m_pBtnBurn->getButton()->setEnabled(count!=0);
	m_pHeroAwardList->setVisible(count!=0);
	m_pHeroAwardList->setPositionX((HERO_REBIRTH_ITEM_COUNT - count) * 42);
}

void RebirthView::setMenuTouchEnable(bool val)
{
	m_pConfirmButton->getButton()->setEnabled(val);
	m_pBook->setEnabled(val);
	m_pMenuHero->setEnabled(val);
	//m_pSelectBtn->getButton()->setEnabled(val);
	//m_pReselectBtn->setEnabled(val);
	m_pBtnBurn->getButton()->setEnabled(val);
}