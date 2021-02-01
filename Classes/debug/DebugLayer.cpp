//
//  Debug.cpp
//  QSMY
//
//  Created by wanghejun on 13-5-25.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DebugLayer.h"
#include "../scenes/game/Game.h"
#include "../managers/GameManager.h"
#include "../utils/base64.h"
#include "../utils/SystemUtil.h"
#include "AppConsts.h"

USING_NS_CC;
USING_NS_CC_EXT;


#define MARGIN_TOP 100

enum DebugType {
    kTestBattle = 0,
    kTestPve,
    kChangeServer,
    kDumpTexture,
    kShowFPS,
    kHideMouseCursor,
    kRemoveDebug,
    kSetFPS,
    kLoadBattle,
	kTestSkillName,
	kTestTotal
};


static std::string names[] = {"TestBattle","TestPve","ChangeServer","DumpTexture","ShowFPS","HideMouseCursor","RemoveDebug","setFPS","LoadBattle", "TestSkillName"};

bool DebugLayer::init()
{
    if(!DPopup::init()) return false;
    
    CCMenu * pMenu = CCMenu::create();
    pMenu->setPosition(CCPointZero);
    addChild(pMenu);
    
    
    //登陆输入框
    m_pPveInput = CCEditBox::create(CCSize(60, 40), CCScale9Sprite::create("PL_ui/denglu/bg_choiceServers.png"));
    m_pPveInput->setFont(D_FONT_DEFAULT,D_FONT_SIZE_12);
    m_pPveInput->setFontColor(ccRED);
    //m_pPveInput->setPlaceHolder(D_LOCAL_STRING("Username"));
    m_pPveInput->setPlaceholderFontColor(ccWHITE);
    m_pPveInput->setMaxLength(10);
    m_pPveInput->setReturnType(kKeyboardReturnTypeDone);
    m_pPveInput->setText("");
    addChild(m_pPveInput);
    
    CCMenuItemFont* pButton = NULL;
    
    for (int i=kTestBattle; i<kTestTotal; ++i) {
        pButton = CCMenuItemFont::create(names[i].c_str(), this, menu_selector(DebugLayer::execute));
        pButton->setTag(i);
        pButton->setPosition(ccpSub(D_DESIGN_POINT_TOP,
                                           ccp(0,(pButton->getContentSize().height)*i+MARGIN_TOP)));
        pMenu->addChild(pButton);
        if(i==kTestPve)
        {
            m_pPveInput->setPosition(ccpSub(pButton->getPosition(),ccp(200,0)));
        }
    }
    
    createCloseMenu();

	m_pSkillNameBMFont = CCLabelBMFont::create("", D_FONT_SUPPER_SKILL_NAME);
	addChild(m_pSkillNameBMFont);
	m_pSkillNameBMFont->setPosition(D_WIN_CENTER_POINT);

	m_pSkillNameTTF = CCLabelTTF::create("", D_FONT_DEFAULT, 30);
	addChild(m_pSkillNameTTF);
	m_pSkillNameTTF->setPosition(ccp(m_pSkillNameBMFont->getPositionX(), m_pSkillNameBMFont->getPositionY() - 50));
    return true;
}

void DebugLayer::execute(cocos2d::CCObject *sender)
{
    CCMenuItem* item = (CCMenuItem*)sender;
    switch (item->getTag()) {
        case kTestBattle:
            testBattle();
            break;
        case kTestPve:
            testPve();
            break;
        case kChangeServer:
            changeServer();
            break;
        case kDumpTexture:
            CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
            break;
        case kShowFPS:
            CCDirector::sharedDirector()->setDisplayStats(!CCDirector::sharedDirector()->isDisplayStats());
            break;
        case kHideMouseCursor:
            SystemUtil::hideMouseCursor();
            break;
        case kRemoveDebug:
            if(m_pCtrlButton) m_pCtrlButton->removeFromParent();
            break;
        case kSetFPS:
            CCDirector::sharedDirector()->setAnimationInterval(1/2.f);
            break;
        case kLoadBattle:
            loadBattle();
			break;
		case kTestSkillName:
			testSkillName();
			break;
        default:
            break;
    }
    
}

void DebugLayer::loadBattle()
{
#define battle_file "battle.json"
    if(CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(battle_file)))
    {
        Json::Value root;
        unsigned long size(0);
        unsigned char* pContent = CCFileUtils::sharedFileUtils()->getFileData(battle_file, "rb", &size);
        std::string jsonStr((char*)pContent,size);
        
        /*for (uint32_t i(0); i<size; ++i) {
            printf("%d,",*(pContent+i));
        }*/
        
        CC_SAFE_DELETE_ARRAY(pContent);
        Json::Reader reader;
        reader.parse(jsonStr, root);
        DM_GET_BATTLE_MODEL->parseInfo(root);
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE,cocos2d::CCInteger::create(battle::kSkipBattleTypeSkip));
    }else
    {
        Tips_Alert(D_LOCAL_STRING("not found battle.json!").c_str());
    }
}

void DebugLayer::changeServer()
{
    if(HTTP_CLIENT->getURL()==DM_GET_PLATFORM_MODEL->getServerURL())
    {
        HTTP_CLIENT->setURL("");
    }else
    {
        HTTP_CLIENT->setURL(DM_GET_PLATFORM_MODEL->getServerURL());
    }
}

void DebugLayer::testPve()
{
    json_serializer s;
    HTTP_CLIENT->doBattleRequset(this,callfuncJson_selector(DebugLayer::startBattle),false,"Test","enterStage",s<<"mid"<<m_pPveInput->getText(),false);
}

void DebugLayer::testBattle()
{
    unsigned long size = 0;
    char* pBuffer = (char*)CCFileUtils::sharedFileUtils()->getFileData("debug.json", "rt", &size);
    if (pBuffer != NULL && size > 0)
    {
        json_serializer s;
        HTTP_CLIENT->doBattleRequset(this,callfuncJson_selector(DebugLayer::startBattle),false,"Test","battle",s<<"filecontent"<<base64_encode(std::string(pBuffer,size)).c_str(),false);
    }
    CC_SAFE_DELETE_ARRAY(pBuffer);
    
}

void DebugLayer::startBattle(const Json::Value &,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_START_BATTLE);
}

void DebugLayer::testSkillName()
{
	if (strlen(m_pPveInput->getText()) == 0)
	{
		Tips_Alert("input skill name, man!");
		return;
	}
	
	CCString* skillString  = CCString::createWithFormat("$skillname%s", m_pPveInput->getText());
	m_pSkillNameBMFont->setString(D_LOCAL_STRING(skillString->getCString()).c_str());
	m_pSkillNameTTF->setString(D_LOCAL_STRING(skillString->getCString()).c_str());
}



#pragma mark button
bool DebugButton::init()
{
    if(!CCNode::init()) return false;
    
    CCMenuItemFont* pButton = CCMenuItemFont::create("Debug", this, menu_selector(DebugButton::toggleDebugLayer));
    pButton->setPosition(ccpSub(D_DESIGN_POINT_RIGHTTOP,
                                       ccp(pButton->getContentSize().width,pButton->getContentSize().height)));
    CCMenu * pMenu = CCMenu::create(pButton,NULL);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu);

    m_pDebugLayer = DebugLayer::create();
    m_pDebugLayer->setCtrlButton(this);
    m_pDebugLayer->retain();
    
    return true;
}

void DebugButton::toggleDebugLayer(cocos2d::CCObject*)
{
    if(m_pDebugLayer->isOpening())m_pDebugLayer->closePopup();
        else Game::sharedGame()->openPopup(m_pDebugLayer);
}
