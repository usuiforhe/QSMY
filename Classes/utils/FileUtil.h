//
//  FileUtil.h
//  QSMY
//
//  Created by wanghejun on 13-10-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__FileUtil__
#define __QSMY__FileUtil__

#include "cocos2d.h"

class FileUtil
{
public:
    // hash utils
    static std::string filehash_md5str(FILE* fp, char* buf, size_t buf_size, double* pnow = NULL, double* ptotal = NULL);
    
    // directory access
    static bool access(const char* path, int mode);
    
    // make directory
    static bool mkdir(const char* path);
    
    // build path directorys
    static bool builddir(const char* path);

	// remove directory
	static bool removeDirectory(const std::string& path);
    
    // rename file
    static bool rename(const char* from, const char* to);
    
    // remove file
    static bool remove(const char* path);

	static bool uncompress(std::string _storagePath, std::string zipFileName);
    
    static void checkLogFileSize();

	//static bool createDirectory(const char *path);
};



#endif /* defined(__QSMY__FileUtil__) */
