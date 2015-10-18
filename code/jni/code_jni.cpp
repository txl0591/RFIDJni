/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		code_jni.cpp
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-10-8
 Description: 		
 History: 			
 					
   1.Date:	 		2015-10-8
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
#include "command.h"
#include "md5.h"
#include "zip_util.h"

#define CHECK_OPER  0
#define APK_MAX		10
#define USE_CLASS_PATH  "com/rfid/Client/MainClient"

namespace nlscan
{
bool mCheck = false;
JavaVM *g_jvm = NULL;
jobject g_obj = NULL;
static unsigned char Check[16] = {0x2d,0x7b,0x56,0x79,0xf3,0x47,0x8d,0xf5,0x7d,0x14,0x1f,0x38,0x33,0x21,0xac,0x46};
/*************************************************
 Function:		SetJavaEnv
 Descroption:	获取虚拟机环境
 Input: 		None
 Output:
 Return:
 Other:
*************************************************/
static int SetJavaEnv(JNIEnv* env, jobject thiz)
{
    g_obj= env->NewGlobalRef(thiz);
    env->GetJavaVM(&g_jvm);
    return 0;
}

/*************************************************
 Function:		nlscan.CallJavaFunc
 Descroption:	 
 Input: 
	1.env
	2.thiz
	3.Cmd
	4.Data
	5.len
 Output: 
 Return: 	
 Other:  
*************************************************/
static void CallJavaFunc(JNIEnv* env, jobject thiz, jint Cmd, jbyteArray Data, jint len)
{
	jclass mClass = env->GetObjectClass(thiz);  
    jmethodID mmethodID = env->GetMethodID(mClass, "SendAck", "(I[BI)V");
	env->CallVoidMethod(thiz, mmethodID, Cmd, Data, len);
}

/*************************************************
 Function:		nlscan.BeepOk
 Descroption:	 
 Input: 
	1.env
	2.thiz
 Output: 
 Return: 	
 Other:  
*************************************************/
static void BeepOk(JNIEnv* env, jobject thiz)
{
	int length = 1;
	jbyte buffer[1];
	jbyteArray OutData = env->NewByteArray(length);
    buffer[0] = RFID_BEEP_OK;
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_BEEP, OutData, length);
}

/*************************************************
 Function:		nlscan.BeepErr
 Descroption:	 
 Input: 
	1.env
	2.thiz
 Output: 
 Return: 	
 Other:  
*************************************************/
static void BeepErr(JNIEnv* env, jobject thiz)
{
	int length = 1;
	jbyte buffer[1];
	jbyteArray OutData = env->NewByteArray(length);
    buffer[0] = RFID_BEEP_ERR;
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_BEEP, OutData, length);
}

/*************************************************
 Function:		nlscan.ReadUserInfo
 Descroption:	 
 Input: 
	1.env
	2.thiz
 Output: 
 Return: 	
 Other:  
*************************************************/
static void ReadUserInfo(JNIEnv* env, jobject thiz)
{
	int length = 5;
	jbyte buffer[5];
	jbyteArray OutData = env->NewByteArray(length);
    buffer[0] = 0x00;
	buffer[1] = RFID_USER_START;
	buffer[2] = RFID_USER_LEN;
	buffer[3] = RFID_PWD_A;
	buffer[4] = RFID_PWD_GET;
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_READ_MULT_BLOCK, OutData, length);
}

/*************************************************
 Function:		nlscan.ReadUserInfoDefault
 Descroption:	 
 Input: 
	1.env
	2.thiz
 Output: 
 Return: 	
 Other:  
*************************************************/
static void ReadUserInfoDefault(JNIEnv* env, jobject thiz)
{
	int length = 5;
	jbyte buffer[5];
	jbyteArray OutData = env->NewByteArray(length);
    buffer[0] = 0x00;
	buffer[1] = RFID_USER_START;
	buffer[2] = RFID_USER_LEN;
	buffer[3] = RFID_PWD_A;
	buffer[4] = RFID_PWD_DEFAULT;
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_READ_MULT_BLOCK, OutData, length);
}

/*************************************************
 Function:		nlscan.ReadSysInfo
 Descroption:	 
 Input: 
	1.env
	2.thiz
 Output: 
 Return: 	
 Other:  
*************************************************/
static void ReadSysInfo(JNIEnv* env, jobject thiz)
{
	int length = 5;
	jbyte buffer[5];
	jbyteArray OutData = env->NewByteArray(length);
    buffer[0] = 0x00;
	buffer[1] = RFID_SYS_START;
	buffer[2] = RFID_SYS_LEN;
	buffer[3] = RFID_PWD_A;
	buffer[4] = RFID_PWD_GET;
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_READ_MULT_BLOCK, OutData, length);
}
        
/*************************************************
 Function:		nlscan.WriteUserCardOper
 Descroption:	 
 Input: 
	1.env
	2.thiz
	3.blk
	4.blkindex
	5.InData
 Output: 
 Return: 	
 Other:  
*************************************************/
static void WriteUserCardOper(JNIEnv* env, jobject thiz, jint blk, jint blkindex, jbyteArray InData)
{
	int length = 20;
	jbyte buffer[20];
	jbyte InChar[100];
	jsize size = env->GetArrayLength(InData); 
	jbyteArray OutData = env->NewByteArray(length);
	env->GetByteArrayRegion(InData,0,size,InChar);
	
	buffer[0] = 0x00;
	buffer[1] = blk;
	buffer[2] = blkindex;
	buffer[3] = RFID_PWD_A;
	memcpy(buffer+4, InChar, 16);
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_WRITE_MULT_BLOCK, OutData, length);
}
            
/*************************************************
 Function:		nlscan.WriteCrcBlkToCard
 Descroption:	 
 Input: 
	1.env
	2.thiz
	3.CardType
 Output: 
 Return: 	
 Other:  
*************************************************/
static void WriteCrcBlkToCard(JNIEnv* env, jobject thiz, jint CardType)
{
	int length = 21;
	jbyte crc[18] ={0x2A,0x61,0x66,0x5E,0x66,0x39,0x6F,0x32,0x77,0x35,0x66,0x76,0x46,0x31,0x23,0x51,0xFF,0xFF};
	jbyte buffer[21];
	buffer[0] = 0x00;
	buffer[1] = RFID_CRC_START;
	buffer[2] = RFID_PWD_A;
	
	switch (CardType){
	case RFID_CARD_NONE:
		break;
	case RFID_CARD_INIT:
		crc[16] = 0x00;
		crc[17] = 0x00;
		break;	
	case RFID_CARD_USER:
		crc[16] = 0x01;
		break;
	case RFID_CARD_SYS:
		crc[17] = 0x01;
		break;
	case RFID_CARD_ALL:
		crc[16] = 0x01;
		crc[17] = 0x01;
		break;
	}
	memcpy(buffer+3,crc, 18);
	jbyteArray OutData = env->NewByteArray(length);
	env->SetByteArrayRegion(OutData, 0, length, buffer);
	CallJavaFunc(env, thiz, RFID_WRITE_BLOCK, OutData, length);
}

/*************************************************
 Function:		nlscan.WriteCrcBlkToRom
 Descroption:	 
 Input: 
	1.env
	2.thiz
 Output: 
 Return: 	
 Other:  
*************************************************/
static void WriteCrcBlkToRom(JNIEnv* env, jobject thiz)
{
	int length = 16;
	jbyte buffer[16] = {0x2A,0x61,0x66,0x5E,0x66,0x39,0x6F,0x32,0x77,0x35,0x66,0x76,0x46,0x31,0x23,0x51};
	jbyteArray OutData = env->NewByteArray(length);
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_SET_CRCBLK, OutData, length);
}
        
/*************************************************
 Function:		nlscan.RecoverSys
 Descroption:	 
 Input: 
	1.env
	2.thiz
 Output: 
 Return: 	
 Other:  
*************************************************/
static void RecoverSys(JNIEnv* env, jobject thiz)
{
	int length = 1;
	jbyte buffer[1];
	jbyteArray OutData = env->NewByteArray(length);
    buffer[0] = 0;
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_RECOVER_SYS, OutData, length);
}
    
/*************************************************
 Function:		nlscan.GetVersion
 Descroption:	 
 Input: 
	1.env
	2.thiz
 Output: 
 Return: 	
 Other:  
*************************************************/
static void GetVersion(JNIEnv* env, jobject thiz)
{
	int length = 1;
	jbyte buffer[1];
	jbyteArray OutData = env->NewByteArray(length);
    buffer[0] = 0;
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_GET_VERSION, OutData, length);
}
    
/*************************************************
 Function:		nlscan.PowerOnInit
 Descroption:	 
 Input: 
	1.env
	2.thiz
 Output: 
 Return: 	
 Other:  
*************************************************/
static void PowerOnInit(JNIEnv* env, jobject thiz)
{
	int length = 1;
	jbyte buffer[1];
	jbyteArray OutData = env->NewByteArray(length);
    buffer[0] = ADDR_ANDROID;
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_POWER_SYNC, OutData, length);
}
    
/*************************************************
 Function:		nlscan.GetCardId
 Descroption:	 
 Input: 
	1.env
	2.thiz
 Output: 
 Return: 	
 Other:  
*************************************************/
static void GetCardId(JNIEnv* env, jobject thiz)
{	
	int length = 1;
	jbyte buffer[1];
	jbyteArray OutData = env->NewByteArray(length);
    buffer[0] = 0;
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_READ_CARDID, OutData, length);
}
    
/*************************************************
 Function:		nlscan.GetCardType
 Descroption:	 
 Input: 
	1.env
	2.thiz
 Output: 
 Return: 	
 Other:  
*************************************************/
static void GetCardType(JNIEnv* env, jobject thiz)
{
	int length = 4;
	jbyte buffer[4];
	jbyteArray OutData = env->NewByteArray(length);
   	buffer[0] = 0x00;
	buffer[1] = RFID_PWD_A;
	env->SetByteArrayRegion(OutData, 0, length, buffer);
    CallJavaFunc(env, thiz, RFID_GET_CRCBLK, OutData, length);
}

/*************************************************
 Function:		nlscan.JniSendCommond
 Descroption:	 
 Input: 
	1.env
	2.thiz
	3.Cmd
 Output: 
 Return: 	
 Other:  
*************************************************/
static jboolean JniSendCommondV(JNIEnv* env, jobject thiz, jint Cmd)
{
	#if CHECK_OPER
	if(mCheck == false)
	{
		return false;
	}
	#endif

	switch(Cmd)
	{	    
	    case JNI_BEEP_OK:
			BeepOk(env, thiz);
			break;
			
		case JNI_BEEP_ERR:
			BeepErr(env, thiz);
			break;

		case JNI_READ_USER:
			ReadUserInfo(env, thiz);
			break;

		case JNI_READ_USERDEFAULT:
			ReadUserInfoDefault(env, thiz);
			break;

		case JNI_W_CRCBLK_ROM:
			WriteCrcBlkToRom(env, thiz);
			break;
			
	   case JNI_RECOVERY_SYS:
	   		RecoverSys(env, thiz);
			break;
			
	   case JNI_GET_VERSION:
	   		GetVersion(env, thiz);
			break;
			
	   case JNI_POWER_ON:
	   		PowerOnInit(env, thiz);
			break;
			
	   case JNI_GET_CARDID:
	   		GetCardId(env, thiz);
			break;
			
	   case JNI_GET_CARDTYPE:
	   		GetCardType(env, thiz);
			break;

	    default:	        
	        break;
	}
	    
	return true;
}

/*************************************************
 Function:		nlscan.JniSendCommond
 Descroption:	 
 Input: 
	1.env
	2.thiz
	3.Cmd
 Output: 
 Return: 	
 Other:  
*************************************************/
static jboolean JniSendCommondI(JNIEnv* env, jobject thiz, jint Cmd, jint param)
{
	#if CHECK_OPER
	if(mCheck == false)
	{
		return false;
	}
	#endif

	switch(Cmd)
	{
		case JNI_W_CRCBLK_CARD:
			WriteCrcBlkToCard(env, thiz, param);
			break;
		default:	        
	        break;
	}

	return true;
}

/*************************************************
 Function:		nlscan.JniSendCommond
 Descroption:	 
 Input: 
	1.env
	2.thiz
	3.Cmd
 Output: 
 Return: 	
 Other:  
*************************************************/
static jboolean JniSendCommondO(JNIEnv* env, jobject thiz, jint Cmd, jint param1, jint param2, jbyteArray InData)
{
	#if CHECK_OPER
	if(mCheck == false)
	{
		return false;
	}
	#endif

	switch(Cmd)
	{
		case JNI_W_USER_CARD:
			WriteUserCardOper(env, thiz, param1, param2, InData);
			break;
			
		default:	        
	        break;
	}

	return true;
}


/*************************************************
 Function:		nlscan.JniDecrypt
 Descroption:	 
 Input: 
	1.char* InChar
	2.char* OutChar
	3.length
 Output: 
 Return: 	
 Other:  
*************************************************/
static jbyteArray JniDecrypt(JNIEnv* env, jobject thiz, jbyteArray InData)
{   
	int i,c;     
    int jiemibox[]={15,18,3,20,21,16,10,11,14,8,24,12,9,5,1,6,19,2,22,17,13,7,23,4};

	#if CHECK_OPER
	if(mCheck == false)
	{
		return NULL;
	}
	#endif
	
	jsize length  = env->GetArrayLength(InData); 
	jbyte* InChar = (jbyte*)malloc(length+1);
	jbyte* OutChar = (jbyte*)malloc(length+1);

	env->GetByteArrayRegion(InData,0,length,InChar);
	for(i = 0; i < length; i++)   
	{   
		c = i/24;   
		if (InChar[i] > jiemibox[i%24])
		{
			OutChar[jiemibox[i%24]+c*24-1]= (InChar[i]-jiemibox[i%24]+1);   
		}
		else
		{
			OutChar[jiemibox[i%24]+c*24-1]= (InChar[i]+256-jiemibox[i%24]+1);   
		}
	}   

	jbyteArray OutData = env->NewByteArray(length);
	env->SetByteArrayRegion(OutData, 0, length, OutChar);

	
	if(InChar)
	{
		free(InChar);
		InChar = NULL;
	}

	if(OutChar)
	{
		free(OutChar);
		OutChar = NULL;
	}

	return OutData;
}

/*************************************************
 Function:		nlscan.JniEncryption
 Descroption:	 
 Input: 
	1.char* InChar
	2.char* OutChar
	3.length
 Output: 
 Return: 	
 Other:  
*************************************************/
static jbyteArray JniEncryption(JNIEnv* env, jobject thiz, jbyteArray InData)
{   
	int i,c;     
    int jiemibox[]={15,18,3,20,21,16,10,11,14,8,24,12,9,5,1,6,19,2,22,17,13,7,23,4};

	#if CHECK_OPER
	if(mCheck == false)
	{
		return NULL;
	}
	#endif
	
	jsize length  = env->GetArrayLength(InData); 
	jbyte* InChar = (jbyte*)malloc(length+1);
	jbyte* OutChar = (jbyte*)malloc(length+1);

	env->GetByteArrayRegion(InData,0,length,InChar);
	for(i = 0; i < length; i++)   
	{   
		c = i/24;   
		OutChar[i] = (InChar[jiemibox[i%24]+c*24-1]+jiemibox[i%24]-1);
	}   

	jbyteArray OutData = env->NewByteArray(length);
	env->SetByteArrayRegion(OutData, 0, length, OutChar);

	
	if(InChar)
	{
		free(InChar);
		InChar = NULL;
	}

	if(OutChar)
	{
		free(OutChar);
		OutChar = NULL;
	}

	return OutData;
}

/*************************************************
 Function:		ScanInit
 Descroption:	扫描初始化
 Input:
	1.env
	2.thiz
 Output:
 Return:
 Other:
*************************************************/
static jboolean JniInit(JNIEnv* env, jobject thiz)
{
	if(NULL == g_obj)
	{
		SetJavaEnv(env,thiz);
	}

    return true;
}

static const JNINativeMethod gMethods[] = {
    { "JNI_Init",          			"()Z",                               	 (void *)JniInit },
	{ "JNI_SendCommondV",          	"(I)Z",                              	 (void *)JniSendCommondV },
	{ "JNI_SendCommondI",          	"(II)Z",                              	 (void *)JniSendCommondI },
	{ "JNI_SendCommondO",          	"(III[B)Z",                              (void *)JniSendCommondO },
	{ "JNI_Encryption",          	"([B)[B",                       	     (void *)JniEncryption },
	{ "JNI_Decrypt",          		"([B)[B",                       	     (void *)JniDecrypt },
};

/*************************************************
 Function:		nlscan.CheckClassDex
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static bool CheckClassDex(void)
{
	char filename[200];
	int i;
	struct zip* z = NULL;
	unsigned char* buf = NULL;
    MD5_CTX context;
    unsigned char digest[16];
	bool ret = true;

	for (i = 0; i < APK_MAX; i++)
	{
		memset(filename, 0, sizeof(filename));
		sprintf(filename, "/data/app/com.rfid.app-%d.apk",i);
		z = openZip(filename);
		if(NULL != z)
		{
			break;
		}
	}

	if(z != NULL)		
	{
		long len = readDexFile(z, "classes.dex" ,&buf);
	    MD5Init (&context);
	    MD5Update (&context, (unsigned char*)buf, len);
	    MD5Final (&context,digest);

		for (i = 1 ; i < 16; i++)
		{
			if(Check[i] != digest[i])
			{
				ret = false;
				break;
			}
		}
		return ret;
	}
    else
   	{
   		return false;
   	}
	
}

/*************************************************
 Function:		register_methods
 Descroption:	注册扫描部分
 Input:
	1.env
 Output:
 Return:
 Other:
*************************************************/
static int register_methods(JNIEnv* env)
{
    static const char* const kClassName = USE_CLASS_PATH;
    jclass clazz;
    clazz = env->FindClass(kClassName);
    if (clazz == NULL)
    {
        return -1;
    }
    /* register all the methods */
    if (env->RegisterNatives(clazz, gMethods,
            sizeof(gMethods) / sizeof(gMethods[0])) != JNI_OK)
    {
        LOGE("Failed registering methods for %s\n", kClassName);
        return -1;
    }

    return 0;
}

/*************************************************
 Function:		JNI_OnLoad
 Descroption:	JNI Loader
 Input:
	1.vm
	2.reserved
 Output:
 Return:
 Other:
*************************************************/
extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* temp = NULL;
    JNIEnv* env = NULL;
    jint result = -1;
	bool check = false;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE("ERROR: GetEnv failed/n");
        goto fail;
    }
    if (register_methods(env) != 0)
    {
        LOGE("ERROR: PlatformLibrary native registration failed/n");
        goto fail;
    }

	LOGD("Version[%s Build Time %s]",_JNI_BUILD_VERSION_,_JNI_BUILD_TIME_);
    result = JNI_VERSION_1_4;

	mCheck = CheckClassDex();
	#if CHECK_OPER
	if(false == check)
	{
		LOGD("去死吧你!");
		jclass clazz = temp->FindClass(USE_CLASS_PATH);
	}
	#endif
    fail:
    return result;
}

}

