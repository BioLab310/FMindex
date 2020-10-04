/*
 * read.c
 *
 *  Created on: 2019年7月8日
 *      Author: jupiter
 */
#include "read.h"

void *read_binfile(char *path)
{
	void *ptr;
	FILE * file = NULL;
	file = fopen(path,"r");
	uint32_t len;
	uint32_t pathlen;
	pathlen = strlen(path);
	if(path[pathlen-1] == 'B')
	{
		if(file)
		{
			fseek(file,0,SEEK_END);//将文件内部的指针指向文件末尾
			len = ftell(file);//获取文件长度，（得到文件位置指针当前位置相对于文件首的偏移字节数）
			rewind(file);//将文件内部的指针重新指向一个流的开头
			ptr = malloc(len*sizeof(char));//申请内存空间，len*sizeof(char)是为了更严谨，16位上char占一个字符，其他机器上可能变化

			//用malloc申请的内存是没有初始值的，如果不赋值会导致写入的时候找不到结束标志符而出现内存比实际申请值大，写入数据后面跟随乱码的情况
			memset(ptr,0,len*sizeof(char));//将内存空间都赋值为‘\0’

			fread(ptr,sizeof(char),len,file);
		}
//		for(uint32_t i = 0; i < len; i++)
//		{
//			printf("%c",((char*)ptr)[i]);
//		}
	}
	else
	{
		if(file)
		{
			fseek(file,0,SEEK_END);//将文件内部的指针指向文件末尾
			len = ftell(file);//获取文件长度，（得到文件位置指针当前位置相对于文件首的偏移字节数）
			rewind(file);//将文件内部的指针重新指向一个流的开头
			ptr = malloc(len);//申请内存空间，len*sizeof(char)是为了更严谨，16位上char占一个字符，其他机器上可能变化
			//用malloc申请的内存是没有初始值的，如果不赋值会导致写入的时候找不到结束标志符而出现内存比实际申请值大，写入数据后面跟随乱码的情况
			memset(ptr,0,len);//将内存空间都赋值为‘\0’

			fread(ptr,1,len,file);
		}
//		for(uint32_t i = 0; i < len/4; i++)
//		{
//			printf("%d ",((uint32_t*)ptr)[i]);
//		}
	}
//	printf("\n");
	fclose(file);
	return (void *)(ptr);
}

void read_bfile2mem(char *path,sFMindex *memptr, uint32_t num)
{
	//num is the number of file :  SA1 SA2 SA3...
	FILE * file = NULL;
	char filename[20];
	uint32_t len;
	if(num == 0)
	{
		sprintf(filename,"%s%s",path,"/B");
	}
	else
	{
		sprintf(filename,"%s%s%d",path,"/B",num);
	}
	file = fopen(filename,"r");
	if(file)
	{
		fseek(file,0,SEEK_END);//将文件内部的指针指向文件末尾
		len = ftell(file);//获取文件长度，（得到文件位置指针当前位置相对于文件首的偏移字节数）
		rewind(file);//将文件内部的指针重新指向一个流的开头
		memptr->b = (char *)malloc(len+1*sizeof(char));//申请内存空间，len*sizeof(char)是为了更严谨，16位上char占一个字符，其他机器上可能变化
		//用malloc申请的内存是没有初始值的，如果不赋值会导致写入的时候找不到结束标志符而出现内存比实际申请值大，写入数据后面跟随乱码的情况
		memset(memptr->b,0,len+1);//将内存空间都赋值为‘\0’

		fread(memptr->b,1,len,file);
//		printf("%s\n",memptr->b);
	}
	if(num == 0)
	{
		sprintf(filename,"%s%s",path,"/C");
	}
	else
	{
		sprintf(filename,"%s%s%d",path,"/C",num);
	}
	file = fopen(filename,"r");
	if(file)
	{
		fseek(file,0,SEEK_END);//将文件内部的指针指向文件末尾
		len = ftell(file);//获取文件长度，（得到文件位置指针当前位置相对于文件首的偏移字节数）
		rewind(file);//将文件内部的指针重新指向一个流的开头
		memptr->c = (uint32_t *)malloc(len);//申请内存空间，len*sizeof(char)是为了更严谨，16位上char占一个字符，其他机器上可能变化
		//用malloc申请的内存是没有初始值的，如果不赋值会导致写入的时候找不到结束标志符而出现内存比实际申请值大，写入数据后面跟随乱码的情况
		memset(memptr->c,0,len);//将内存空间都赋值为‘\0’

		fread(memptr->c,1,len,file);
//		for(uint32_t i = 0; i < len / 4; i++)
//		{
//			printf("%d ",(memptr->c)[i]);
//		}
//		printf("\n");
	}
	if(num == 0)
	{
		sprintf(filename,"%s%s",path,"/OCCA");
	}
	else
	{
		sprintf(filename,"%s%s%d",path,"/OCCA",num);
	}
	file = fopen(filename,"r");
	if(file)
	{
		fseek(file,0,SEEK_END);//将文件内部的指针指向文件末尾
		len = ftell(file);//获取文件长度，（得到文件位置指针当前位置相对于文件首的偏移字节数）
		rewind(file);//将文件内部的指针重新指向一个流的开头
		memptr->occa = (uint32_t *)malloc(len);//申请内存空间，len*sizeof(char)是为了更严谨，16位上char占一个字符，其他机器上可能变化
		//用malloc申请的内存是没有初始值的，如果不赋值会导致写入的时候找不到结束标志符而出现内存比实际申请值大，写入数据后面跟随乱码的情况
		memset(memptr->occa,0,len);//将内存空间都赋值为‘\0’

		fread(memptr->occa,1,len,file);
		memptr->occ_num = len / 4;
//		for(uint32_t i = 0; i < len / 4; i++)
//		{
//			printf("%d ",(memptr->occa)[i]);
//		}
//		printf("\n");
	}
	if(num == 0)
	{
		sprintf(filename,"%s%s",path,"/SA");
	}
	else
	{
		sprintf(filename,"%s%s%d",path,"/SA",num);
	}
	file = fopen(filename,"r");
	if(file)
	{
		fseek(file,0,SEEK_END);//将文件内部的指针指向文件末尾
		len = ftell(file);//获取文件长度，（得到文件位置指针当前位置相对于文件首的偏移字节数）
		rewind(file);//将文件内部的指针重新指向一个流的开头
		memptr->sa = (uint32_t *)malloc(len);//申请内存空间，len*sizeof(char)是为了更严谨，16位上char占一个字符，其他机器上可能变化
		//用malloc申请的内存是没有初始值的，如果不赋值会导致写入的时候找不到结束标志符而出现内存比实际申请值大，写入数据后面跟随乱码的情况
		memset(memptr->sa,0,len);//将内存空间都赋值为‘\0’

		fread(memptr->sa,1,len,file);
		memptr->sa_num = len / 4;
//		for(uint32_t i = 0; i < len / 4; i++)
//		{
//			printf("%d ",(memptr->sa)[i]);
//		}
//		printf("\n");
	}
}



