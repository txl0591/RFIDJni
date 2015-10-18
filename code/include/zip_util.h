/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		zip_util.h
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-10-11
 Description: 		
 History: 			
 					
   1.Date:	 		2015-10-11
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _ZIP_UTIL_H_
#define _ZIP_UTIL_H_

#include "zip.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*************************************************
 Function:		openZip
 Descroption:	 
 Input: 
	1.filename
 Output: 
 Return: 	
 Other:  
*************************************************/
struct zip* openZip(char* filename);

/*************************************************
 Function:		closeZip
 Descroption:	 
 Input: 
	1.zip* z
 Output: 
 Return: 	
 Other:  
*************************************************/
int closeZip(struct zip* z);

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
int readZipFileName(struct zip* z, char* filetype);

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
long readDexFile(struct zip* z, char* filename, unsigned char** buf);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _ZIP_UTIL_H_ Edit By Txl 2015-10-11 */

