//
//  VersionModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "VersionModel.h"
#include "managers/GameManager.h"
#include "utils/FileUtil.h"
#include "utils/URLUtil.h"
#include "utils/SystemUtil.h"
#include "utils/base64.h"
#include "utils/StringUtil.h"

USING_NS_CC;
using namespace version;

#define VERSION "version"
#define RES     "res"
#define UPDATING_RES "updating_res"

#define VERSION_INFO        "platform/version.txt"
#define FILE_VERSION_INFO   "platform/version.json"

static std::string m_storagePath("");    //当前版本更新文件目录
static std::string m_storageRootPath(""); //更新文件根目录


void VersionModel::setStoragePath(const std::string &path)
{
    //把所有添加的路径全部指向更新目录
    m_storageRootPath = path;
    if (CCFileUtils::sharedFileUtils()->isFileExist(m_storageRootPath) == false) {
        FileUtil::mkdir(m_storageRootPath.c_str());
    }
    SystemUtil::addSkipBackupAttributeToItemAtPath(m_storageRootPath);

    m_storagePath = m_storageRootPath + SystemUtil::getVersion()+'/';
    std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
    std::vector<std::string> searchPathsN = searchPaths;
    std::vector<std::string>::reverse_iterator iter = searchPaths.rbegin();
    for (; iter!=searchPaths.rend(); ++iter)
    {
        std::vector<std::string>::iterator iterN = searchPathsN.begin();
        std::string path = *iter;
        
        size_t pos = path.find("assets/");
        if (pos == 0)
        {
            path = path.substr(strlen("assets/"));
        }
        
        searchPathsN.insert(iterN, m_storagePath+path);
    }
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPathsN);
}

const std::string VersionModel::getStoragePath()
{
	return m_storagePath;
}

static VersionModel *s_SharedVersionModel = NULL;

VersionModel* VersionModel::sharedVersionModel(void)
{
    if (!s_SharedVersionModel)
    {
        s_SharedVersionModel = new VersionModel();
        s_SharedVersionModel->init();
    }
    
    return s_SharedVersionModel;
}

void VersionModel::purgeVersionModel(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedVersionModel);
}


VersionModel::VersionModel()
:m_pDownloadList(NULL)
,m_fTotalSize(0)
{
    
}

VersionModel::~VersionModel()
{
    CC_SAFE_RELEASE(m_pDownloadList);
    
    
    if(!m_bInited) return;
    finalizestmt();
    sqlite3_close(_db);
}

void VersionModel::finalizestmt()
{
    sqlite3_finalize(_stmt_select);
    sqlite3_finalize(_stmt_remove);
    sqlite3_finalize(_stmt_update);
    sqlite3_finalize(_stmt_version_update);
    sqlite3_finalize(_stmt_version_select);
}


void VersionModel::createTable()
{
	const char *sql_createSettingTable = "CREATE TABLE IF NOT EXISTS setting(key TEXT PRIMARY KEY,value TEXT);";
	sqlite3_stmt *stmt;
    
	int ok=sqlite3_prepare_v2(_db, sql_createSettingTable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
    
    
    const char *sql_createVersionTable = "CREATE TABLE IF NOT EXISTS version(file TEXT PRIMARY KEY UNIQUE,md5 TEXT,size INTEGER,download SMALLINT DEFAULT 0);";
    
	ok |= sqlite3_prepare_v2(_db, sql_createVersionTable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		CCLOG("Error in CREATE TABLE\n");
    
    
    ok = SQLITE_OK;
    // SELECT
    const char *sql_select = "SELECT value FROM setting WHERE key=?;";
    ok |= sqlite3_prepare_v2(_db, sql_select, -1, &_stmt_select, NULL);
    
    // REPLACE
    const char *sql_update = "REPLACE INTO setting (key, value) VALUES (?,?);";
    ok |= sqlite3_prepare_v2(_db, sql_update, -1, &_stmt_update, NULL);
    
    // DELETE
    const char *sql_remove = "DELETE FROM setting WHERE key=?;";
    ok |= sqlite3_prepare_v2(_db, sql_remove, -1, &_stmt_remove, NULL);
    
    
    const char* sql_version_update = "REPLACE INTO version (file,md5,size,download) VALUES (?,?,?,?);";
    ok |= sqlite3_prepare_v2(_db, sql_version_update, -1, &_stmt_version_update, NULL);
    
    const char *sql_version_select = "SELECT * FROM version WHERE file=? AND md5=? AND size=?";
    ok |= sqlite3_prepare_v2(_db, sql_version_select, -1, &_stmt_version_select, NULL);
    
    if( ok != SQLITE_OK ) {
        printf("Error initializing DB\n");
        // report error
    }
}


void VersionModel::dropTable()
{
    finalizestmt();
    
	const char *sql_dropSettingTable = "DROP TABLE IF EXISTS setting;";
	sqlite3_stmt *stmt;
    
	int ok=sqlite3_prepare_v2(_db, sql_dropSettingTable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
    
    
    const char *sql_dropVersionTable = "DROP TABLE IF EXISTS version;";
    
	ok |= sqlite3_prepare_v2(_db, sql_dropVersionTable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		CCLOG("Error in DROP TABLE\n");
}

void VersionModel::begin()
{
    exec("BEGIN TRANSACTION");
}

void VersionModel::commit()
{
    exec("COMMIT");
}

void VersionModel::exec(const char* sql)
{
    char* _msg;
    sqlite3_exec(_db, sql, NULL, NULL, &_msg);
    if(_msg)
    {
        CCLOG(_msg);
        sqlite3_free(_msg);
    }
}

void VersionModel::updateFileInfo(const char*file,const char* md5,int size,uint8_t download)
{
    assert( m_bInited );
	
	int ok = sqlite3_bind_text(_stmt_version_update, 1, file, -1, SQLITE_TRANSIENT);
	ok |= sqlite3_bind_text(_stmt_version_update, 2, md5, -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_int(_stmt_version_update, 3, size);
    ok |= sqlite3_bind_int(_stmt_version_update, 4, download);
	ok |= sqlite3_step(_stmt_version_update);
	
	ok |= sqlite3_reset(_stmt_version_update);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		CCLOG("Error in VersionModel.updateFileInfo() %s \n",file);
}

bool VersionModel::isFileExist(const char*file,const char* md5,int size)
{
    assert( m_bInited );
    int ok = sqlite3_bind_text(_stmt_version_select, 1, file, -1, SQLITE_TRANSIENT);
	ok |= sqlite3_bind_text(_stmt_version_select, 2, md5, -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_int(_stmt_version_select, 3, size);
	int ret = sqlite3_step(_stmt_version_select);
	ok |= sqlite3_reset(_stmt_version_select);
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		CCLOG("Error in VersionModel.isFileExist()\n");
    return ret == SQLITE_ROW;
}

bool isBattleAnimation(const std::string& fileName)
{
    if (fileName.find("battle/skill") != std::string::npos) {
        return true;
    }
    
    if (fileName.find("battle/unit") != std::string::npos) {
        return true;
    }
    
    return false;
}


cocos2d::CCArray* VersionModel::getNeedUpdateFileList()
{
    cocos2d::CCArray* pRet = cocos2d::CCArray::create();
    const char *sql_select = "SELECT * FROM version WHERE download<>0";
	sqlite3_stmt *stmt;
    
	int ok=sqlite3_prepare_v2(_db, sql_select, -1, &stmt, NULL);
	//ok |= sqlite3_step(stmt);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string fileName = (char*)sqlite3_column_text(stmt, 0);
        if (isBattleAnimation(fileName))
        {
            if (SystemUtil::isNBDevice())
            {
                if (fileName.find(LOW_QUALITY_PREFIX) != std::string::npos)
                {
                    continue;
                }
            }
            else
            {
                if (fileName.find(LOW_QUALITY_PREFIX) == std::string::npos)
                {
                    continue;
                }
            }
        }
        
        FileInfo* pFileInfo = FileInfo::create();
        pFileInfo->m_file = fileName;
        pFileInfo->m_md5 = (char*)sqlite3_column_text(stmt, 1);
        pFileInfo->m_size = sqlite3_column_int(stmt, 2);
        pFileInfo->m_download = sqlite3_column_int(stmt, 3);
        pRet->addObject(pFileInfo);
    }
    
	ok |= sqlite3_finalize(stmt);
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
		CCLOG("Error in VersionModel.getNeedUpdateFileList()\n");
    
    return pRet;
}


void VersionModel::setItem( const char *key, const char *value)
{
	assert( m_bInited );
	
	int ok = sqlite3_bind_text(_stmt_update, 1, key, -1, SQLITE_TRANSIENT);
	ok |= sqlite3_bind_text(_stmt_update, 2, value, -1, SQLITE_TRANSIENT);
    
	ok |= sqlite3_step(_stmt_update);
	
	ok |= sqlite3_reset(_stmt_update);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		CCLOG("Error in VersionModel.setItem()\n");
}

const char* VersionModel::getItem( const char *key )
{
	assert( m_bInited );
    
	int ok = sqlite3_reset(_stmt_select);
    
	ok |= sqlite3_bind_text(_stmt_select, 1, key, -1, SQLITE_TRANSIENT);
	ok |= sqlite3_step(_stmt_select);
	const unsigned char *ret = sqlite3_column_text(_stmt_select, 0);
	
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE && ok != SQLITE_ROW)
		CCLOG("Error in VersionModel.getItem()\n");
    
	return (const char*)ret;
}

void VersionModel::removeItem( const char *key )
{
	assert( m_bInited );
    
	int ok = sqlite3_bind_text(_stmt_remove, 1, key, -1, SQLITE_TRANSIENT);
	
	ok |= sqlite3_step(_stmt_remove);
	
	ok |= sqlite3_reset(_stmt_remove);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		CCLOG("Error in VersionModel.removeItem()\n");
}

bool VersionModel::init()
{
    if(getInited()) return true;
    if(!Model::init()) return false;
    db_path = CCFileUtils::sharedFileUtils()->getWritablePath()+"version";
    
    int ret = 0;
    ret = sqlite3_open(db_path.c_str(), &_db);
    SystemUtil::addSkipBackupAttributeToItemAtPath(db_path);
    if( ret != SQLITE_OK ) {
        printf("Error open DB\n");
        // report error
    }
    
    //dropTable(); // test
    createTable();
    
    Json::Value root;
    Json::Reader reader;
    reader.parse(CCString::createWithContentsOfFile(FILE_VERSION_INFO)->getCString(), root);
    
    if(!checkInit(root))
    {
        removeFiles();
        dropTable();
        createTable();
        parseGameVersion(root);
        loadData();
    }
    
    return true;
}

void VersionModel::removeFiles()
{
    FileUtil::removeDirectory(m_storageRootPath);
    CCFileUtils::sharedFileUtils()->purgeCachedEntries();
}


bool VersionModel::checkInit(const Json::Value &val)
{
    if(getItem(RES)==NULL) return false;
    if(getItem(VERSION)==NULL) return false;
    int db_res = atoi(getItem(RES));
    int pkg_res = atoi(val[RES].asString().c_str());
    std::string db_ver = getItem(VERSION);
    std::string pkg_ver = SystemUtil::getVersion();
    return db_ver==pkg_ver && db_res>=pkg_res;
}

void VersionModel::loadData()
{
    parseInfo( CSVDecoder::create(VERSION_CONFIG_PATH)->getData());
    
}

void VersionModel::parseGameVersion(const Json::Value &val)
{
    setItem(RES, val[RES].asString().c_str());
    setItem(VERSION,SystemUtil::getVersion().c_str());
}

void VersionModel::parseInfo(const CSVDecoder::CSV &val)
{
    begin();
    for (uint32_t i=0; i<val.size(); ++i)
    {
        updateFileInfo(val[i][0].c_str(),val[i][1].c_str(),atoi(val[i][2].c_str()),kDownloaded);
    }
    commit();
}

void VersionModel::setFileDownloadStatus(version::FileInfo* pInfo,uint8_t download)
{
    if(pInfo==NULL) return;
    if(pInfo->m_download==download) return;
    if(pInfo->m_download==kNeedDownload && download==kDownloading)
    {
        FileUtil::remove(pInfo->getLocalURL().c_str());
    }
    pInfo->m_download = download;
	CCLOG("%s download status, %d", pInfo->m_file.c_str(), download);
    updateFileInfo(pInfo->m_file.c_str(), pInfo->m_md5.c_str(), pInfo->m_size, download);
}

void VersionModel::checkResVersion()
{
    HTTP_CLIENT->getVersion(this,callfuncJson_selector(VersionModel::onCheckResVersion));
}

void VersionModel::onCheckResVersion(const Json::Value &requestData, const Json::Value &responseData)
{
    std::string remoteRes = responseData["data"][RES].asString();
    const char* localRes = getItem((RES));
    assert(localRes);
    
    int remote = atoi(remoteRes.c_str());
    int local = atoi(localRes);
    
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_CHECK_RES_VERSION_COMPLETE,CCBool::create(remote>local));
}

void VersionModel::SyncToRemoteVersion()
{
    HTTP_CLIENT->getVersion(this,callfuncJson_selector(VersionModel::onGetVersion));
}

void VersionModel::forceUpdateVersion()
{
    CCLOG("update game!");
    SystemUtil::updateGame(ipaURL.c_str(),D_LOCAL_STRING("updateGame").c_str());
}

void VersionModel::onGetVersion(const Json::Value &requestData, const Json::Value &responseData)
{
    isSynced = true;
    
    std::string version = responseData["data"][VERSION].asString();
    remoteRes = responseData["data"][RES].asString();

    versionURL = CCString::createWithFormat(DM_GET_PLATFORM_MODEL->getConfig().getVersionURL().c_str(),DM_GET_PLATFORM_MODEL->getConfig().getPlatform().c_str(),(version+"."+remoteRes).c_str())->getCString();
    ipaURL = CCString::createWithFormat(DM_GET_PLATFORM_MODEL->getConfig().getIPAURL().c_str(),DM_GET_PLATFORM_MODEL->getConfig().getPlatform().c_str(),(version+"."+remoteRes).c_str())->getCString();
    
    std::string localVersion = SystemUtil::getVersion();
    assert(!localVersion.empty());
    
    //如果版本不同 强制更新
    if(version.compare(localVersion.c_str())!=0)
    {
        //forceUpdateVersion();
        postNotification(D_EVENT_VERSION_FORCE_UPDATE);
        return;
    }
    
    const char* localRes = getItem((RES));
    assert(localRes);
    
    int remote = atoi(remoteRes.c_str());
    int local = atoi(localRes);
    
    if(remote>local)
    {
        const char* updatingRes = getItem(UPDATING_RES);
        if(updatingRes!=NULL && remoteRes.compare(updatingRes)==0)
        {
            doUpdate();
            return;
        }
        //如果资源不一样 获取文件版本列表
        HTTP_CLIENT->getFileVersion(this,callfuncJson_selector(VersionModel::onGetFileList),(versionURL+VERSION_INFO+SUFFIX).c_str());
        return;
    }
    
    postNotification(D_EVENT_VERSION_CHECK_FINISH);
}

void VersionModel::onGetFileList(const Json::Value &requestData, const Json::Value &responseData)
{
    std::string data = base64_decode(responseData["data"]["data"].asString());
    const CSVDecoder::CSV &val = CSVDecoder::createWithBuffer(data.c_str(),data.length())->getData();
    begin();
    for (uint32_t i=0; i<val.size(); ++i)
    {
        if(!isFileExist(val[i][0].c_str(),val[i][1].c_str(),atoi(val[i][2].c_str())))
        {
            updateFileInfo(val[i][0].c_str(),val[i][1].c_str(),atoi(val[i][2].c_str()),kNeedDownload);
        }
    }
    setItem(UPDATING_RES, remoteRes.c_str());
    commit();
    doUpdate();
}

void VersionModel::makeDownloadFileList()
{
    CC_SAFE_RELEASE(m_pDownloadList);
    m_pDownloadList = getNeedUpdateFileList();
    CC_SAFE_RETAIN(m_pDownloadList);
    
    cocos2d::CCObject* obj = NULL;
    version::FileInfo* pFileInfo = NULL;
    m_fTotalSize= 0.f;
    CCLOG("----storeagepath------%s",m_storagePath.c_str());
    CCARRAY_FOREACH(m_pDownloadList, obj)
    {
        pFileInfo = static_cast<version::FileInfo*>(obj);
        
        std::string localFileName = m_storagePath+pFileInfo->m_file;
        if (isBattleAnimation(pFileInfo->m_file) && SystemUtil::isNBDevice() == false)
        {
            if (localFileName.find(LOW_QUALITY_PREFIX) != std::string::npos) {
                replace(localFileName, LOW_QUALITY_PREFIX, "");
            }
        }

        pFileInfo->m_localURL = localFileName;
        pFileInfo->m_remoteURL = versionURL+UrlEncode(pFileInfo->m_file);
        
        if(pFileInfo->m_download==version::kDownloading) pFileInfo->m_bResume = true;
        m_fTotalSize+=pFileInfo->getSize()/1024.f/1024.f;
		CCLOG("f:%s, size:%d, localurl:%s, remoteurl:%s", pFileInfo->m_file.c_str(), pFileInfo->m_size, pFileInfo->m_localURL.c_str(), versionURL.c_str());
    }
}

void VersionModel::doUpdate()
{
    makeDownloadFileList();
    if(m_pDownloadList->count()==0)
    {
        updateComplete();
        postNotification(D_EVENT_VERSION_CHECK_FINISH);
        return;
    }
    postNotification(D_EVENT_VERSION_START_UPDATE);
}

void VersionModel::updateComplete()
{
    setItem(RES, remoteRes.c_str());
    removeItem(UPDATING_RES);
    CC_SAFE_RELEASE_NULL(m_pDownloadList);
    SystemUtil::addSkipBackupAttributeToItemAtPath(m_storageRootPath);
    //bool skiped = SystemUtil::isFileSkipedBackUp(m_storageRootPath);
    //CCLOG("%s, skiped:%d", m_storageRootPath.c_str(), (int)skiped);
}



