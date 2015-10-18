/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		zip_util.c
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-10-11
 Description: 		
 History: 			
 					
   1.Date:	 		2015-10-11
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

#include "jni.h"
#include "config.h"
#include "dlog.h"
#include "zip.h"
#include "zip_util.h"

/*************************************************
 Function:		openZip
 Descroption:	 
 Input: 
	1.filename
 Output: 
 Return: 	
 Other:  
*************************************************/
struct zip* openZip(char* filename)
{
    int error;
    struct zip *z = NULL;    
    z = zip_open(filename, ZIP_RDONLY, &error);
    return z;
}

/*************************************************
 Function:		closeZip
 Descroption:	 
 Input: 
	1.zip* z
 Output: 
 Return: 	
 Other:  
*************************************************/
int closeZip(struct zip* z)
{
    int err = zip_close(z);
    return err;
}

/*************************************************
 Function:		readZipFileName
 Descroption:	 
 Input: 
	1.zip* z
	2.filetype
 Output: 
 Return: 	
 Other:  
*************************************************/
int readZipFileName(struct zip* z, char* filetype)
{
    int i;
    struct zip_stat fstat;  

    if(z != NULL)
    {
        zip_stat_init(&fstat);  
        int c = zip_get_num_files(z);
        if(c > 0)
        {
            for (i = 0 ; i < c; i++)
            {
                const char* name = zip_get_name(z, i, 0);  
                if(name != NULL)
                {
                    zip_stat(z, name,0,&fstat);  
                    //LOGI("File %i:%s Size: %lld Size2: %lld\n", i,fstat.name,fstat.size ,fstat.comp_size);  
                }        
            }
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

/*************************************************
 Function:		readDexFile
 Descroption:	 
 Input: 
	1.zip* z
	2.filename
	3.char* buf
 Output: 
 Return: 	
 Other:  
*************************************************/
long readDexFile(struct zip* z, char* filename, unsigned char** buf)
{
    int i;
    long ReadNum = 0;
    struct zip_stat fstat;  

    if(z != NULL && NULL != buf)
    {
        zip_stat_init(&fstat);  
        int c = zip_get_num_files(z);
        if(c > 0)
        {
            for (i = 0 ; i < c; i++)
            {
                const char* name = zip_get_name(z, i, 0);  
                if(0 == strcmp(name,filename))
                {
                    zip_stat(z, name,0,&fstat);  
                    //LOGI("File %i:%s Size1: %lld Size2: %lld\n", i,fstat.name,fstat.size ,fstat.comp_size);  

                    struct zip_file* file = zip_fopen(z, filename, 0);  
                    if (file)
                    {
                        *buf =(unsigned char *)malloc(fstat.size+1);
                        memset(*buf,0,(fstat.size+1));
                        ReadNum = zip_fread(file, *buf,fstat.size);
                        zip_fclose(file);  
                    }
                    break;
                }        
            }
        }
    }
    else
    {
        return 0;
    }

    return ReadNum;
}

