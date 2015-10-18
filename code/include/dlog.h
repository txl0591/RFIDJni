/*********************************************************
 Copyright (C),2011-2017,Electronic Technology Co.,Ltd.
 File name: 		dlog.h
 Author: 			Tangxl
 Version: 			1.0
 Date: 				2014-6-15
 Description:
 History:

   1.Date:	 		2014-6-15
 	 Author:	 	Tangxl
 	 Modification:  Created file

*********************************************************/
#ifndef _DLOG_H_
#define _DLOG_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _ANDROID_HARDWARE_ /* Edit By Tangxl 2014-6-15 */
#include <utils/Log.h>
#else
#include <android/log.h>
#endif

#undef   LOG_TAG
#define  LOG_TAG    "CoreSoft"

#define _LOG_DEBUG_
//#define _LOG_DEBUG_STD_

#ifdef WIN32
#define vsnprintf _vsnprintf
#define LOGD printf
#define LOGI printf
#define LOGW printf
#define LOGE printf
#define LOGF printf
#else

#undef LOG
#ifdef _LOG_DEBUG_
#ifdef _LOG_DEBUG_STD_
#define LOGD printf
#define LOGI printf
#define LOGW printf
#define LOGE printf
#define LOGF printf
#else
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#endif
 #else
#define LOGI(...)
#define LOGD(...)
#define LOGE(...)
#define LOGF(...)
#define LOGW(...)
#endif

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _LOG_H_ Edit By Tangxl 2014-6-15 */

