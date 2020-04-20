/*
 * VirtualFat.h
 *
 *  Created on: Apr 20, 2020
 *      Author: Administrator
 */

#ifndef SRC_VIRTUALFAT_H_
#define SRC_VIRTUALFAT_H_

#include "stdint.h"
#include "string.h"



//导入资源文件
#include "RC.h"


#define UNUSED_PARA(x) ((void)x)

//定义存储的块数量，需要与usbd_storage_if.c中的定义保持一致
#ifndef STORAGE_BLK_NBR
#define STORAGE_BLK_NBR                  0x10000
#endif

//定义存储的块大小，需要与usbd_storage_if.c中的定义保持一致
#ifndef STORAGE_BLK_SIZ
#define STORAGE_BLK_SIZ                  0x200
#endif

//使用FAT16
#define FAT16

#ifdef __cplusplus
extern "C"
{
#endif


#ifndef FAT16
#error "暂时只支持FAT16"
#endif

//定义数据类型,仅支持小端模式CPU
//定义BPB结构
typedef struct FAT_BPB_T
{
uint8_t BS_jmpBoot[3];
uint8_t BS_OEMName[8];
uint8_t BPB_BytsPerSec[2];
uint8_t BPB_SecPerClus;
uint8_t BPB_RsvdSecCnt[2];
uint8_t BPB_NumFATs;
uint8_t BPB_RootEntCnt[2];
uint8_t BPB_TotSec16[2];
uint8_t BPB_Media;
uint8_t BPB_FATSz16[2];
uint8_t BPB_SecPerTrk[2];
uint8_t BPB_NumHeads[2];
uint8_t BPB_HiddSec[4];
uint8_t BPB_TotSec32[4];

#if defined(FAT12) || defined(FAT16)
uint8_t BS_DrvNum;
uint8_t BS_Reserved1;
uint8_t BS_BootSig;
uint8_t BS_VolID[4];
uint8_t BS_VolLab[11];
uint8_t BS_FilSysType[8];

#else
uint8_t BPB_FATSz32[4];
uint8_t BPB_ExtFlags[2];
uint8_t BPB_FSVer[2];
uint8_t BPB_RootClus[4];
uint8_t BPB_FSInfo[2];
uint8_t BPB_BkBootSec[2];
uint8_t BPB_Reserved[12];
uint8_t BS_DrvNum;
uint8_t BS_Reserved1;
uint8_t BS_BootSig;
uint8_t BS_VolID[4];
uint8_t BS_VolLab[11];
uint8_t BS_FilSysType[8];
#endif


} FAT_BPB;


typedef struct FAT_DIR_t
{
uint8_t	DIR_Name[11];
uint8_t	DIR_Att;
uint8_t	DIR_NTRes;
uint8_t	DIR_CrtTimeTenth;
uint8_t	DIR_CrtTime[2];
uint8_t	DIR_CrtDate[2];
uint8_t	DIR_LstAccDate[2];
uint8_t	DIR_FstClusHI[2];
uint8_t	DIR_WrtTime[2];
uint8_t	DIR_WrtDat[2];
uint8_t	DIR_FstClusLO[2];
uint8_t	DIR_FileSize[4];
} FAT_DIR;

//定义初始化函数，一般由STORAGE_Init_FS调用
uint8_t VirtualFat_Init(uint8_t lun);

//定义读函数,一般由STORAGE_Read_FS调用
uint8_t VirtualFat_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);

//定义写函数，一般由STORAGE_Write_FS调用
int8_t VirtualFat_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);

#ifdef __cplusplus
};
#endif

#endif /* SRC_VIRTUALFAT_H_ */
