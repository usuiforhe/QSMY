
#include "AppDelegate.h"

#include "managers/GameManager.h"
#include "managers/DataManager.h"
#include "AppConsts.h"
#include "utils/Lang.h"
#include "utils/UIUtil.h"
#include "managers/SoundManager.h"
#include "managers/PlatformManager.h"
#include "managers/AnalyticsManager.h"
#include "utils/SystemUtil.h"
#include "model/VersionModel.h"

#define QSMY_LANG "tw"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

static bool didFinishLaunching = false;

AppDelegate::AppDelegate()
{
    
}

AppDelegate::~AppDelegate()
{
    //GameManager::purgeGameManager();
}

bool AppDelegate::applicationDidFinishLaunching()
{
	CCLOG("AppDelegate::applicationDidFinishLaunching");
	didFinishLaunching = true;
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    
    CCSize designSize = DESIGN_SIZE;
    
	if ((screenSize.height / screenSize.width) < (PL_MIN_RESOLUTION_HEIGHT / DESIGN_SIZE.width))
	{
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width,
			PL_MIN_RESOLUTION_HEIGHT,
			kResolutionFixedHeight);
	}
	else
	{
		 CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width,
                                                           designSize.height,
                                                           /*designSize.height/designSize.width>screenSize.height/screenSize.width?kResolutionFixedHeight:*/kResolutionFixedWidth);
	}
    
    CCLOG("%f,%f",CCDirector::sharedDirector()->getWinSize().width,CCDirector::sharedDirector()->getWinSize().height);


    pDirector->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    CCFileUtils::sharedFileUtils()->setPopupNotify(false);
    

	//自定义目录需要在指定更新目录之前指定
#ifdef WIN32
	CCFileUtils::sharedFileUtils()->addSearchPath("LangsResources/");
	CCFileUtils::sharedFileUtils()->addSearchPath("Resources/");
	CCFileUtils::sharedFileUtils()->addSearchPath("Resources/platforms/ios/internal/");
	CCFileUtils::sharedFileUtils()->addSearchPath(CCString::createWithFormat("LangsResources/%s/lang/", QSMY_LANG)->getCString());
	CCFileUtils::sharedFileUtils()->addSearchPath(CCString::createWithFormat("LangsResources/%s/lang/scripts/", QSMY_LANG)->getCString());
	CCFileUtils::sharedFileUtils()->addSearchPath(CCString::createWithFormat("LangsResources/%s/lang/ui/ccb/", QSMY_LANG)->getCString());
	CCFileUtils::sharedFileUtils()->addSearchPath(CCString::createWithFormat("LangsResources/%s/lang/fonts/", QSMY_LANG)->getCString());
	CCFileUtils::sharedFileUtils()->addSearchPath("Resources/ui/ccb/");
	CCFileUtils::sharedFileUtils()->addSearchPath("Resources/PL_ui/ccb/");
	
	CCFileUtils::sharedFileUtils()->addSearchPath("Resources/scripts/");
#else
    vector<string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
    vector<string>::iterator iter = searchPaths.begin();
    searchPaths.insert(iter, "lang/");
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
    
    CCFileUtils::sharedFileUtils()->addSearchPath("lang/PL_ui/ccb/");
    CCFileUtils::sharedFileUtils()->addSearchPath("ui/ccb/");
	CCFileUtils::sharedFileUtils()->addSearchPath("PL_ui/ccb/");
	CCFileUtils::sharedFileUtils()->addSearchPath("lang/scripts/");
    CCFileUtils::sharedFileUtils()->addSearchPath("scripts/");
#endif
    
    pDirector->getScheduler()->scheduleSelector(schedule_selector(AppDelegate::initAPP), this, 0.0f, 0, 0, false);
    
    return true;
}

void AppDelegate::initAPP(float dt)
{
    std::string storagePath = CCFileUtils::sharedFileUtils()->getWritablePath()+"Resources/";
    VersionModel::setStoragePath(storagePath);
    VersionModel::sharedVersionModel();
    
    AnalyticsManager::sharedAnalyticsManager();//初始化bi统计
    PlatformManager::sharedPlatformManager(); //init platform
    
    GameManager::sharedGameManager()->initScriptEngine();
    
    
    UIUtil::shaderForKey(D_SHADER_MASK_KEY,ccPositionTextureColor_vert, CCString::createWithContentsOfFile("shader/mask.fsh")->getCString());
    UIUtil::shaderForKey(D_SHADER_GRAY_KEY,ccPositionTextureColor_vert, CCString::createWithContentsOfFile("shader/gray.fsh")->getCString());
    UIUtil::shaderForKey(D_SHADER_MASK_GRAY_KEY,ccPositionTextureColor_vert, CCString::createWithContentsOfFile("shader/mask_gray.fsh")->getCString());
    
    Lang::setLangFile("data/local");
    
    SoundManager::sharedSoundManager()->stopBackgroundMusic();
    SoundManager::sharedSoundManager()->stopAllEffects();
    
    GameManager::sharedGameManager()->goScene(SceneManager::kSceneTagSplash);
    
    SystemUtil::setIdleTimerDisabled(true);

}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    SystemUtil::setIdleTimerDisabled(false);
    CCDirector::sharedDirector()->stopAnimation();
    //GameManager::purgeGameManager();
    
    // if you use SimpleAudioEngine, it must be pause
    SoundManager::sharedSoundManager()->pauseBackgroundMusic();
    SoundManager::sharedSoundManager()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	if (!didFinishLaunching)
	{
		return;
	}
	
    SystemUtil::setIdleTimerDisabled(true);
    //GameManager::sharedGameManager();
    CCDirector::sharedDirector()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    SoundManager::sharedSoundManager()->resumeBackgroundMusic();
    SoundManager::sharedSoundManager()->resumeAllEffects();
    
    //同步服务器
	GameManager::sharedGameManager()->SyncGame();
}
