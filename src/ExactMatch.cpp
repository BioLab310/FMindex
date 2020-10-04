/*
 * ExactMatch.cpp
 *
 *  Created on: 2019年7月13日
 *      Author: jupiter
 */

#include "ExactMatch.h"

char * get_queryfrag(uint32_t k,char *query)
{
	int32_t len = strlen(query);
	char *frag = NULL;
	frag = (char *)malloc(len*sizeof(char));
	memset(frag,0,len);
	strcpy(frag,query+k);
	return frag;
}

char C_next(char c)
{
	switch(c)
	{
	case 'A':
		return 'C';
	case 'C':
		return 'G';
	case 'G':
		return 'T';
	case 'T':
		return 'U';
	default:
		return 'W';
	}
}

uint32_t calc_C(const sFMindex &mem,char c)
{
	uint32_t rslt;
	switch(c)
	{
		case 'A' :
			rslt = 0;
			break;
		case 'C' :
			rslt = mem.c[0];
			break;
		case 'G' :
			rslt = mem.c[0] + mem.c[1];
			break;
		case 'T' :
			rslt = mem.c[0] + mem.c[1] + mem.c[2];
			break;
		default:
			rslt = mem.c[0] + mem.c[1] + mem.c[2] + mem.c[3];
			break;
	}
	return rslt;
}

uint32_t calc_offset(char c)
{
	uint32_t offset;
	switch(c)
	{
		case 'A' :
				offset = 0;
				break;
		case 'C' :
				offset = 1;
				break;
		case 'G' :
				offset = 2;
				break;
		case 'T' :
				offset = 3;
				break;
		default:
				offset = -1;
				break;
	}
	return offset;
}

uint32_t calc_OCC(const sFMindex &mem,char c,uint32_t pos)
{
	//calc the occ_array  input : pos(pos in bseq) gap(every gap pos save a occa)
	uint32_t offset = calc_offset(c);
	if(pos % mem.occ_gap == 0)
	{
		return *(mem.occa + (pos/mem.occ_gap)*4 + offset);
	}
	else
	{
		uint32_t rslt = *(mem.occa + (pos/mem.occ_gap)*4 + offset);
		uint32_t len = pos > mem.occ_gap ? pos - pos % mem.occ_gap : 0;
		for(uint32_t i = 0; i < pos % mem.occ_gap; i++)
		{
			if(mem.b[len+i] == c)
			{
				rslt++;
			}
		}
		return rslt;
	}
}

uint32_t LF_Mapping(const sFMindex &mem,char c,uint32_t pos)
{
	//last first mapping
	return calc_C(mem,c) + calc_OCC(mem,c,pos) + 1;
}

uint32_t LF_Mapping_l(const sFMindex &mem,char c,uint32_t pos)
{
	//last first mapping
	return calc_C(mem,c) + calc_OCC(mem,c,pos) + 1;
}

uint32_t LF_Mapping_h(const sFMindex &mem,char c,uint32_t pos)
{
	//last first mapping
//	return calc_C(mem,c) + calc_OCC(mem,c,pos) + 1 - 1;
	return calc_C(mem,c) + calc_OCC(mem,c,pos+1);
}



uint32_t* calc_SArangeSeq(const sFMindex &mem,char *read)
{
	char ch;
	int32_t i = strlen(read) - 1;
	uint32_t *p = (uint32_t *)malloc(sizeof(uint32_t)*2);
	if(0 > i)
	{
		p[0] = p[1] = 0;
		return p;
	}
	ch = read[i];
	p[0] = calc_C(mem,ch) + 1;
	p[1] = calc_C(mem,C_next(ch)) + 1;
	i--;
	while(p[0] <= p[1] && i >= 0)
	{
		ch = read[i];
		p[0] = LF_Mapping_l(mem,ch,p[0]);
		p[1] = LF_Mapping_h(mem,ch,p[1]);
		i--;
	}
	return p;
}

uint32_t* calc_SArangeChar(const sFMindex &mem,uint32_t *pre, char ch)
{
	uint32_t *p = (uint32_t *)malloc(sizeof(uint32_t)*2);
	p[0] = LF_Mapping_l(mem,ch,pre[0]);
	p[1] = LF_Mapping_h(mem,ch,pre[1]);
	return p;
}

uint32_t calc_SA(const sFMindex &mem,uint32_t pos)
{
	if(pos % mem.sa_gap == 0)
	{
		return mem.sa[pos/mem.sa_gap];
	}
	else
	{
		uint32_t tmp = pos;
		uint32_t cnt = 0;
		char ch = mem.b[tmp];
		while(tmp % mem.sa_gap != 0)
		{
			if(ch == '$')
			{
				return cnt;
			}
			tmp = LF_Mapping(mem,ch,tmp);
			ch = mem.b[tmp];
			cnt++;
		}
		return mem.sa[tmp/mem.sa_gap] + cnt;
	}
}
