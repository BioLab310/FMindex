/*
 * Main.cpp
 *
 *  Created on: Feb 19, 2019
 *      Author: bio
 */
#include "basic.h"
#include "build.h"
#include "read.h"
#include "ExactMatch.h"
#include "Bit_sseInstruction.h"
#include <ctime>
#include "BitVec_index.h"
#include "print.h"
#include <bitset>

int main(int argc, char** argv)
{
	struct build_para para;
	char c;
	for(int32_t i=1;i<argc;i=i+2)
	{
		if(argv[i][0]=='-'&&argv[i][1]=='h')	//path of reference
		{
			para.ref_path=argv[i+1];
		}
		else if(argv[i][0]=='-'&&argv[i][1]=='s')//sa_gap
		{
			para.sa_gap=atoi(argv[i+1]);
		}
		else if(argv[i][0]=='-'&&argv[i][1]=='o')//sa_gap
		{
			para.occ_gap=atoi(argv[i+1]);
		}
		else if(argv[i][0]=='-'&&argv[i][1]=='l')//level
		{
			para.level=atoi(argv[i+1]);
		}
		else if(argv[i][0]=='-'&&argv[i][1]=='t')//thread_num
		{
			para.thread_num=atoi(argv[i+1]);
		}
	}
	char *dir = "./index";
	if(access(dir,0) == -1)
	{
		printf("%s not exist.\n",dir);
		mkdir(dir, 0755);
	}
	struct timeval tvs,tve;
	double span;

	gettimeofday(&tvs,NULL);
	build(para);

	build_occA(para);
	char index[] = "./index";
//	sFMindex prm;
//	read_bfile2mem(index,&prm,0);


	gettimeofday(&tve,NULL);
	span = tve.tv_sec-tvs.tv_sec + (tve.tv_usec-tvs.tv_usec)/1000000.0;
	cout <<"time of construct is: "<<span<<endl;

	return 0;
}

