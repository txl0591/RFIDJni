/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		command.h
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-10-18
 Description: 		
 History: 			
 					
   1.Date:	 		2015-10-18
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _COMMAND_H_
#define _COMMAND_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum
{
    RFID_ANTENNA            = 0x00,
    RFID_CLR_PASSWD         = 0x11,
    RFID_SET_PASSWD         = 0x12,
    RFID_GET_PASSWD         = 0x13,

    RFID_SET_CRCBLK         = 0x14,
    RFID_GET_CRCBLK         = 0x15,

    RFID_WRITE_BLOCK        = 0x21,
    RFID_READ_BLOCK         = 0x22,
    RFID_WRITE_MULT_BLOCK   = 0x23,
    RFID_READ_MULT_BLOCK    = 0x24,
    RFID_READ_CARDID        = 0x25,

    RFID_GET_VERSION        = 0x30,
    RFID_POWER_SYNC         = 0x31,
    RFID_BEEP		        = 0x32,
    RFID_RECOVER_SYS        = 0x33,
}FrameCommond;

typedef enum
{
    JNI_BEEP_OK		        = 0x00,
    JNI_BEEP_ERR            = 0x01,    
    JNI_READ_USER           = 0x02,
    JNI_READ_USERDEFAULT    = 0x03,
    JNI_READ_SYSINFO        = 0x04,
    JNI_W_USER_CARD         = 0x05,
	JNI_W_CRCBLK_CARD       = 0x06,
	JNI_W_CRCBLK_ROM        = 0x07,
    JNI_RECOVERY_SYS        = 0x08,
    JNI_GET_VERSION         = 0x09,
    JNI_POWER_ON            = 0x0A,
    JNI_GET_CARDID          = 0x0B,
    JNI_GET_CARDTYPE        = 0x0C,
}JNICommond;

typedef enum 
{
    RFID_BEEP_OK = 0x00,
    RFID_BEEP_ERR = 0x01,
}RFIDBeep;

typedef enum 
{
    RFID_ANTENNA_ON = 0x00,
    RFID_ANTENNA_OFF = 0x01,
}RFIDAntenna;

typedef enum 
{
    READ_MULT_START = 0x01,
    READ_MULT_CENTER = 0x10,
    READ_MULT_END = 0x11,
}RFIDMult;

typedef enum 
{
    RFID_PWD_A = 0x60,
    RFID_PWD_B = 0x61,
}RFIDPwdMode;

typedef enum 
{
    RFID_PWD_DEFAULT = 0x01,
    RFID_PWD_GET = 0x00,
}RFIDPwdType;

 typedef enum 
{
    RFID_USER_START = 2,
    RFID_USER_LEN = 11,
}RFIDUserBlk;

typedef enum 
{
    RFID_CRC_START = 1,
    RFID_CRC_LEN = 1,
}RFIDCrcBlk;

typedef enum 
{
    RFID_SYS_START = 13,
    RFID_SYS_LEN = 3,
}RFIDSysBlk;

typedef enum  
{
	ADDR_NONE = 0xFF,
    ADDR_RFID = 0,
    ADDR_PC = 1,
    ADDR_ANDROID = 2,
}FrameAddr;

typedef enum 
{
	RFID_CARD_NONE	    = 0x00,
	RFID_CARD_INIT	    = 0x01,
	RFID_CARD_USER	    = 0x02,
	RFID_CARD_SYS	    = 0x03,
	RFID_CARD_ALL	    = 0x04,
}RFID_CARD_TYPE;    

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _COMMAND_H_ Edit By Txl 2015-10-18 */

