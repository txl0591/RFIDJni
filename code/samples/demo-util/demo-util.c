/*********************************************************
 Copyright (C),2011-2017,NewLand Auto-ID Tech.,Ltd.
 File name: 		scan-util.c
 Author: 			txl
 Version: 			1.0
 Date: 				2014-9-11
 Description: 		
 History: 			
 					
   1.Date:	 		2014-9-11
 	 Author:	 	txl
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
#include "md5.h"
#include "zip_util.h"

/*************************************************
 Function:		main
 Descroption:	Ö÷³ÌÐò
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
int main(void)
{
    int i;
    unsigned char* buf = NULL;
    MD5_CTX context;
    unsigned char digest[16];
    int size = 0;
    struct zip* z = openZip("/data/app/com.rfid.app-2.apk");
    long len = readDexFile(z, "classes.dex" ,&buf);
    LOGD("************************************* %ld\n",len);
    MD5Init (&context);
    MD5Update (&context, (unsigned char*)buf, len);
    MD5Final (&context,digest);

    LOGD("digest [");
    
    for (i = 0 ; i < 16; i++)
    {
        LOGD("%x ",digest[i]);
    }

    LOGD("]\n");
    if(NULL != buf)
    free(buf);
    closeZip(z);
    return 0;
}

