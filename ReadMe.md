# 简介

本工程主要主要目的是以STM32提供的USB Device FS中的Mass Stroage Class为基础，虚拟一个FAT文件系统,使其能够在windows下识别为一个可以正常打开的U盘。在Fat文件系统内，windows对文件的操作将在STM32直接调用注册的回调函数，在回调函数内实现对硬件的操作，实现类似于Unix中的一切皆文件的思想。

## 特点
 * 依赖STM32 USB库。需要修改usbd_storage_if.c的STORAGE_Init_FS、STORAGE_Read_FS、STORAGE_Write_FS函数。
 * 不依赖额外的存储器件，完全是虚拟的。
## 使用方法

1.将src文件夹添加至工程中，注意：STM32CudeIDE光是添加文件夹是不能编译的，需要将新添加的文件夹的排除（Exclude）选项取消。

2.修改usbd_storage_if.c

# 进度

* 已验证方案的可行性。虚拟的FAT文件系统大小为32M,扇区大小为512字节，簇大小为512字节。
* 代码可用于片段截取

# 开发工具

* STM32CudeIDE

# 测试使用的硬件

* STM32F103C8T6

# 资源文件生成工具

* [FAT引导扇区转C代码(RC.c)使用的工具](https://github.com/HEYAHONG/RCFileToCSource)
