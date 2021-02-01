//
//  FileUtil.cpp
//  QSMY
//
//  Created by wanghejun on 13-10-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "cocos2d.h"
#include "FileUtil.h"
#include "StringUtil.h"
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>

#if defined(__BCPLUSPLUS__) || (defined(_MSC_VER) && (_MSC_VER < 1300))
typedef struct _stat structstat;
#else
typedef struct _stat64i32 structstat;
#endif

#else

#include <dirent.h>
typedef struct stat structstat;

#endif

#include "support/zip_support/unzip.h"
#define N_BUFFER_SIZE    8192
#define N_MAX_FILENAME   512
using namespace std;
using namespace cocos2d;

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "openssl/md5.h"


std::string FileUtil::filehash_md5str(FILE* fp, char* buf, size_t buf_size, double* pnow /*= NULL*/, double* ptotal /*= NULL*/)
{
	if ( ptotal )
	{
		fseek(fp, 0, SEEK_END);
		*ptotal = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	}
    
	MD5_CTX state;
	unsigned char digest[MD5_DIGEST_LENGTH];
	char hex_output[16*2 + 1];
	size_t file_size = 0;
	MD5_Init(&state);
	do
	{
		size_t readsize = fread(buf, 1, buf_size, fp);
		file_size += readsize;
		MD5_Update(&state, (const unsigned char *)buf, readsize);
        
		if ( pnow )
		{
			*pnow = file_size;
		}
	} while( !feof(fp) && !ferror(fp) );
	MD5_Final(digest,&state);
    
	for (int di = 0; di < 16; ++di)
		sprintf(hex_output + di * 2, "%02x", digest[di]);
    
	hex_output[16*2] = 0;
    
	return std::string(hex_output);

}

#else
std::string FileUtil::filehash_md5str(FILE* fp, char* buf, size_t buf_size, double* pnow /*= NULL*/, double* ptotal /*= NULL*/)
{
	return "";
}


#endif


// directory access
bool FileUtil::access(const char* path, int mode)
{
#ifdef _WIN32
	return false;
#else
	return ::access(path, mode) == 0;
#endif
}

// make directory
bool FileUtil::mkdir(const char* path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = ::mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}


// build path directorys
bool FileUtil::builddir(const char* path)
{
	std::string str(path);
    
	string_replace_char(str, '\\', '/');
    
	//size_t end = str.find_last_not_of('/');
	bool dummy = false;
	for ( size_t i = 1; // ignore root
         i < str.size(); i++ )
	{
		if ( str[i] == '/' && !dummy )
		{
			std::string bpath = str.substr(0, i);
			if ( !FileUtil::access(bpath.c_str(), 0) )
			{
				if ( !FileUtil::mkdir(bpath.c_str()) )
				{
					return false;
				}
			}
			dummy = true;
		}
		else
		{
			dummy = false;
		}
	}
    
	return true;
}

bool FileUtil::removeDirectory(const std::string& path)
{
	structstat buf;
#ifdef _WIN32
	if (::_stat(path.c_str(), &buf) == -1)
#else
	if (::stat(path.c_str(), &buf) == -1)
#endif
	{
		return false;
	}

	if (buf.st_mode & S_IFDIR)
	{
		std::vector<std::string> paths;

#ifdef _WIN32

		const std::string fs = path + "/*";

		struct _finddata_t data;
		intptr_t h = _findfirst(fs.c_str(), &data);
		if (h == -1)
		{
			return ::remove(path.c_str()) == 0;
		}

		while (true)
		{
			std::string name = data.name;

			if (name != ".." && name != ".")
			{
				paths.push_back(name);
			}

			if (_findnext(h, &data) == -1)
			{
				break;
			}
		}

		_findclose(h);

#else

		struct dirent** namelist;
		int n = scandir(path.c_str(), &namelist, 0, alphasort);

		if (n < 0)
		{
			return ::remove(path.c_str()) == 0;
		}

		paths.reserve(n - 2);

		for (int i = 0; i < n; ++i)
		{
			std::string name = namelist[i]->d_name;
			free(namelist[i]);
			if (name != ".." && name != ".")
			{
				paths.push_back(name);
			}
		}

		free(namelist);

#endif

		for (std::vector<std::string>::const_iterator p = paths.begin(); p != paths.end(); ++p)
		{
			if (!removeDirectory(path + '/' + *p))
			{
				return false;
			}
		}

#ifdef _WIN32
		if (::_rmdir(path.c_str()) == -1)
#else
		if (::rmdir(path.c_str()) == -1)
#endif
		{
			return false;
		}
	}
	else
	{
		if (::remove(path.c_str()) == -1)
		{
			return false;
		}
	}

	return true;
}

bool FileUtil::rename(const char* from, const char* to)
{
	return 0 == ::rename(from, to);
}

// remove file
bool FileUtil::remove(const char* path)
{
	return 0 == ::remove(path);
}


bool FileUtil::uncompress(std::string _storagePath, std::string zipFileName)
{
    // Open the zip file
    string outFileName = _storagePath + zipFileName;
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", outFileName.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[N_BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[N_MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  N_MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        string fullPath = _storagePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!builddir(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            CCLOG("zipfile writepath %s", fullPath.c_str());
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, N_BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing and remove zip file: %s", outFileName.c_str());
    remove(outFileName.c_str());
    return true;
}

void FileUtil::checkLogFileSize()
{
    std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "__pili";
    FILE* fileHandle = fopen(filePath.c_str(), "r");
    if (fileHandle != NULL) {
        bool bNeedRemove = false;
        long long fileSize = 0;
        fseek(fileHandle, 0, SEEK_END);
        fileSize = ftell(fileHandle);
        fseek(fileHandle, 0, SEEK_SET);
        fclose(fileHandle);
        if (fileSize >= 10000000) {
            bNeedRemove = true;
        }
        
        if (bNeedRemove) {
            remove(filePath.c_str());
        }
    }
}

///*
// * Create a direcotry is platform depended.
// */
//bool FileUtil::createDirectory(const char *path)
//{
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//    mode_t processMask = umask(0);
//    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
//    umask(processMask);
//    if (ret != 0 && (errno != EEXIST))
//    {
//        return false;
//    }
//    
//    return true;
//#else
//    BOOL ret = CreateDirectoryA(path, NULL);
//	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
//	{
//		return false;
//	}
//    return true;
//#endif
//}
