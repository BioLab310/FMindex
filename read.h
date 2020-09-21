/*
 * read.h
 *
 *  Created on: 2019年7月3日
 *      Author: jupiter
 */

#ifndef READ_H_
#define READ_H_
#include "basic.h"
struct sFMindex
{
	uint32_t sa_gap;
	uint32_t occ_gap;
	char *b;
	uint32_t *c;
	uint32_t *occa;
	uint32_t *sa;
	uint32_t occ_num;
	uint32_t sa_num;
};

void *read_binfile(char *path);
void read_bfile2mem(char *path,sFMindex *memptr, uint32_t num);


#endif /* READ_H_ */
