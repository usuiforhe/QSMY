//
//  VersionModel.h
//  版本管理
//
//  Created by wanghejun on 13-9-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__VersionModel__
#define __QSMY__VersionModel__

#include "Model.h"
#include "sqlite3.h"

class VersionModel;

#define D_EVENT_VERSION_FORCE_UPDATE "start force version"  //强制更新新版本（大版本更新）
#define D_EVENT_VERSION_START_UPDATE "start update version" //开始更新版本
#define D_EVENT_VERSION_CHECK_FINISH "check version finish" //完成检查版本

namespace version {
    
    enum {
        kDownloaded = 0,    //已经同步
        kNeedDownload = 1,  //需要更新
        kDownloading = 2,   //正在更新
    };
    
    class FileInfo:public ValueObjcet
    {
        CC_SYNTHESIZE_READONLY(std::string, m_file, File);
        CC_SYNTHESIZE_READONLY(std::string, m_md5, MD5);
        CC_SYNTHESIZE_READONLY(int, m_size, Size);
        CC_SYNTHESIZE_READONLY(uint8_t, m_download, DownloadStatus);
        CC_SYNTHESIZE_READONLY(bool, m_bResume, Resume);
        
        CC_SYNTHESIZE_READONLY(std::string, m_remoteURL, RemoteURL);
        CC_SYNTHESIZE_READONLY(std::string, m_localURL, LocalURL);
    public:
        FileInfo()
        :m_size(0)
        ,m_download(0)
        ,m_bResume(false)
        {
        }
        
        CREATE_FUNC(FileInfo);
        friend class ::VersionModel;
    };
}

class VersionModel
: public Model
{
public:
    static VersionModel* sharedVersionModel(void);
    static void purgeVersionModel(void);
    
    static void setStoragePath(const std::string& path);
	static const std::string getStoragePath();

    
    
public:
    /**
     *	@brief	获取远端版本号 并开始更新
     */
    void SyncToRemoteVersion();

    
    /**
     *	@brief	设置文件下载状态
     *
     *	@param 	pInfo 	文件信息
     *	@param 	download 	状态
     */
    void setFileDownloadStatus(version::FileInfo* pInfo,uint8_t download);

    
    /**
     *	@brief	刷新下载列表
     */
    void makeDownloadFileList();

    
    /**
     *	@brief	强制更新版本
     */
    void forceUpdateVersion();
    
    
    /**
     *	@brief	检查资源版本
     */
    void checkResVersion();

    
private:
    
    VersionModel();
    virtual ~VersionModel();
    
    

private:
    virtual bool init();
    bool checkInit(const Json::Value &val);
    void loadData();
    void parseGameVersion(const Json::Value &val);
    void parseInfo(const CSVDecoder::CSV &val);
    
private:
    void removeFiles();
    void createTable();
    void dropTable();
    void finalizestmt();
    
    void setItem( const char *key, const char *value);
    const char* getItem( const char *key );
    void removeItem( const char *key );
    
    void updateFileInfo(const char*file,const char* md5,int size,uint8_t download);
    bool isFileExist(const char*file,const char* md5,int size);
    cocos2d::CCArray* getNeedUpdateFileList();
    
    
private:
    void onCheckResVersion(const Json::Value &requestData, const Json::Value &responseData);
    void onGetVersion(const Json::Value &requestData, const Json::Value &responseData);
    void onGetFileList(const Json::Value &requestData, const Json::Value &responseData);
    void doUpdate();
public:
    void updateComplete();
    
private:
    void begin();
    void commit();
    void exec(const char* sql);
private:
    std::string versionURL; //版本远程路径
    std::string ipaURL;     //新版本包路径
    
    std::string  db_path;   //数据库路径
    sqlite3 *_db;
    
    sqlite3_stmt *_stmt_select;
    sqlite3_stmt *_stmt_remove;
    sqlite3_stmt *_stmt_update;
    
    sqlite3_stmt *_stmt_version_update;
    sqlite3_stmt *_stmt_version_select;
    
private: //临时数据
    std::string         remoteRes;
    CC_SYNTHESIZE(cocos2d::CCArray*, m_pDownloadList, DownloadList);//文件下载列表
    CC_SYNTHESIZE_READONLY(float, m_fTotalSize, TotalSize);
};

#endif /* defined(__QSMY__VersionModel__) */
