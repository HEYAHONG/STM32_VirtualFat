/*
 * VirtualFat.c
 *
 *  Created on: Apr 20, 2020
 *      Author: Administrator
 */
#include "VirtualFat.h"
#include "RC.H"

//fat文件系统各部分起始扇区地址

static uint32_t FAT1_START=4;//FAT表
static uint32_t FAT2_START=258;
static uint32_t	RootDir_START=512;//根目录
static uint32_t FileData_START=544;//文件数据

static void init_fatfs(FAT_BPB *bpb)
{
	//以下均是FAT16的计算方式
	FAT1_START=bpb->BPB_RsvdSecCnt[0]+bpb->BPB_RsvdSecCnt[1]*256;
	FAT2_START=FAT1_START+bpb->BPB_FATSz16[0]+bpb->BPB_FATSz16[1]*256;
	RootDir_START=FAT2_START*2-FAT1_START;
	FileData_START=RootDir_START+(bpb->BPB_RootEntCnt[0]+bpb->BPB_RootEntCnt[1]*256)*32/(bpb->BPB_BytsPerSec[0]+bpb->BPB_BytsPerSec[1]*256);
}

//处理FAT表
//fat_offset为fat表中的扇区偏移
static void read_fat_fatfs(uint32_t fat_offset,uint8_t *buf)
{
	if(fat_offset==0)
	{
		{//处理特殊的项
			//FAT表第一项与第二项
			buf[0]=0xf8;
			buf[1]=0xff;
			buf[2]=0xff;
			buf[3]=0xff;
		}
	}
}


//处理根目录
//rootdir_offset为扇区偏移
static void read_rootdir_fatfs(uint32_t rootdir_offset,uint8_t *buf)
{
	if(rootdir_offset==0)
	{
		{//设置卷标
			FAT_DIR *p=(FAT_DIR *)&(buf[0]);
			{
				//文件名用0x20填充
				memset(p->DIR_Name,0x20,sizeof(p->DIR_Name));
				//设置文件名
				memcpy(p->DIR_Name,"Flasher ",sizeof("Flasher "));
				p->DIR_Att=0x08;

			}

		}
	}

}

//定义初始化函数，一般由STORAGE_Init_FS调用
uint8_t VirtualFat_Init(uint8_t lun)
{
	//加载FAT16_DBR.bin
	if(RCGetSize("FAT16_DBR.bin"))
	{
		init_fatfs((FAT_BPB *)RCGetHandle("FAT16_DBR.bin"));
	}
	return 0;
}
//定义读函数,一般由STORAGE_Read_FS调用
uint8_t VirtualFat_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
	//清空buf
	memset(buf,0,blk_len*STORAGE_BLK_SIZ);

	//处理引导扇区与保留扇区
	if(blk_addr<FAT1_START)
	{  //返回DBR
		if(blk_addr==0)
		{
		//加载FAT16_DBR.bin
		if(RCGetSize("FAT16_DBR.bin"))
			{
				memcpy(buf,RCGetHandle("FAT16_DBR.bin"),RCGetSize("FAT16_DBR.bin"));
			}
		}
	}

	//处理FAT表
	if(blk_addr>=FAT1_START && blk_addr<RootDir_START)
	{
		uint32_t fat_offset=blk_addr-FAT1_START;
		if(blk_addr>=FAT2_START)
				 fat_offset-=(FAT2_START-FAT1_START);
		read_fat_fatfs(fat_offset,buf);
	}

	//处理根目录
	if(blk_addr >=RootDir_START && blk_addr <FileData_START)
	{
		read_rootdir_fatfs(blk_addr-RootDir_START,buf);
	}
	return 0;
}

//定义写函数，一般由STORAGE_Write_FS调用
int8_t VirtualFat_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
	return 0;
}
