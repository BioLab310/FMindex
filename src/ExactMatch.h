/*
 * ExactMatch.h
 *
 *  Created on: 2019年7月13日
 *      Author: jupiter
 */

#ifndef EXACTMATCH_H_
#define EXACTMATCH_H_

#include "basic.h"
#include "read.h"

char * get_queryfrag(uint32_t k,char *query);
uint32_t calc_C(const sFMindex &mem,char c);
uint32_t calc_OCC(const sFMindex &mem,char c,uint32_t pos);
uint32_t LF_Mapping(const sFMindex &mem,char c,uint32_t pos);
uint32_t* calc_SArangeSeq(const sFMindex &mem,char *read);
uint32_t* calc_SArangeChar(const sFMindex &mem,uint32_t *pre, char ch);
uint32_t calc_SA(const sFMindex &mem,uint32_t pos);

#endif /* EXACTMATCH_H_ */
