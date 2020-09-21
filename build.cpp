/*
 * build.cpp
 *
 *  Created on: Feb 20, 2019
 *      Author: bio
 */

#include "build.h"

void *bplusTree(void *arg)
{
	struct bplusTree_para * para_arg=(struct bplusTree_para *)arg;
	struct para_cmp para=para_arg->para;
	uint8_t threadID=para_arg->threadID;
	uint32_t level=para_arg->level;
	uint32_t thread_log=para_arg->thread_log;
	char *flag=para_arg->flag;
	char *c;
	c=(char*)malloc(sizeof(char)*thread_log);
	uint32_t plate=3;
	for(uint32_t i=1;i<=thread_log;i++)
	{
		uint32_t tmp;
		tmp=threadID>>(2*(thread_log-i));
		switch(tmp&plate)
		{
			case 0:
				c[i-1]='A';
				break;
			case 1:
				c[i-1]='C';
				break;
			case 2:
				c[i-1]='G';
				break;
			case 3:
				c[i-1]='T';
				break;
			default:
				break;
		}
	}

	struct Node * root;
	root=NodeInitial();
	for(uint32_t i=0;i<para.ref_len-level-thread_log+1;i++)
	{
		uint32_t f=1;
		for(uint32_t x=0;x<level;x++)
		{
			if(para.ref[i+x]!=flag[x])
			{
				f=0;
				break;
			}
		}
		for(uint32_t x=0;x<thread_log;x++)
		{
			if(para.ref[i+level+x]!=c[x])
			{
				f=0;
				break;
			}
		}
		if(f==1)
		{
			char tmp;
			if(i==0)
			{
				tmp='$';
			}
			else
			{
				tmp=para.ref[i-1];
			}
			insert(&root,root, i,tmp,para);
		}
	}
	for(uint32_t add=para.ref_len-level-thread_log+1;add<para.ref_len;add++)
	{
		uint32_t f=1;
		for(uint32_t x=0;x<level;x++)
		{
			if(add+x>=para.ref_len)
			{
				if(flag[x]!='A')
				{
					f=0;
					break;
				}
			}
			else if(para.ref[add+x]!=flag[x])
			{
				f=0;
				break;
			}

		}
		for(uint32_t x=0;x<thread_log;x++)
		{
			if(add+level+x>=para.ref_len)
			{
				if(c[x]!='A')
				{
					f=0;
					break;
				}
			}
			else if(para.ref[add+level+x]!=c[x])
			{
				f=0;
				break;
			}
		}
		if(f==1)
		{
			char tmp;
			if(add==0)
			{
				tmp='$';
			}
			else
			{
				tmp=para.ref[add-1];
			}
			insert(&root,root, add,tmp,para);
		}
	}
	*(para_arg->p)=root;
	return 0;
}

void *mult_bsa(void *arg)
{
	struct mult_bsa_para * para_arg=(struct mult_bsa_para*)arg;
	char *b=para_arg->b;
	uint32_t len_start=para_arg->len_start;
	uint32_t* p_sa=para_arg->sa;
	struct Node*p_start=para_arg->p_start;
	struct Node*p_end=para_arg->p_end;
	uint32_t *pa=para_arg->a;
	uint32_t sa_gap=para_arg->sa_gap;


	struct Node *mini;
	mini=p_start;
	uint32_t b_len_cur;
	b_len_cur=len_start;

	while(mini!=NULL)
	{
		for(int32_t i=0;i<mini->nodesize;i++)
		{
			b[b_len_cur]=mini->b[i];
			if(mini->b[i]=='$')
			{
				*pa=b_len_cur;
			}

			if(b_len_cur%sa_gap==0)
			{
				uint32_t pos_tmp_sa=b_len_cur/sa_gap;
				p_sa[pos_tmp_sa]=mini->data[i];
			}

			b_len_cur++;
		}
		if(mini==p_end)
		{
			break;
		}
		mini=mini->brother;
	}

	return 0;
}

void build(struct build_para para_build)
{
	char * ref_path=para_build.ref_path;
	uint32_t thread_log=para_build.thread_num;
	uint32_t sa_gap=para_build.sa_gap;
	uint32_t level=para_build.level;
	uint32_t max_len=para_build.max_len;

	FILE * out_C = NULL;
	FILE * out_SA = NULL;
	FILE * out_B = NULL;
	out_C=fopen("./index/C","wb+");
	out_B=fopen("./index/B","wb+");
	out_SA=fopen("./index/SA","wb+");

	char * ref;
	uint32_t len;

	ReadSeq(&ref,&len,ref_path);

	uint32_t *p_sa;

	p_sa=(uint32_t*)malloc(sizeof(uint32_t)*(len/sa_gap+1));

	struct para_cmp para;
	para.ref=ref;
	para.ref_len=len;
	para.max_len=max_len;

	char *b;
	b=(char*)malloc(sizeof(char)*(len+1));

	uint32_t C[4]={0};
	for(uint32_t i=0;i<len;i++)
	{
		switch(ref[i])
		{
			case 'A':
				C[0]++;
				break;
			case 'C':
				C[1]++;
				break;
			case 'G':
				C[2]++;
				break;
			case 'T':
				C[3]++;
				break;
			default:
				break;
		}
	}
	fwrite(C,sizeof(uint32_t),4,out_C);
	uint32_t sa_len_cur=0;

	uint32_t dollar_pos;
	b[0]=ref[len-1];
	uint32_t b_len_cur=1;

	p_sa[sa_len_cur]=len;
	sa_len_cur++;

	uint32_t block_num=pow(4,level);
	char * flag;
	flag=(char*)malloc(sizeof(char)*level);
	uint32_t block_start=1;
	for(uint32_t l=0;l<block_num;l++)
	{
		uint32_t plate=3;
		for(uint32_t k=0;k<level;k++)
		{
			uint32_t tmp=l>>(2*(level-1-k));
			switch (tmp&plate)
			{
				case 0:
					flag[k]='A';
					break;
				case 1:
					flag[k]='C';
					break;
				case 2:
					flag[k]='G';
					break;
				case 3:
					flag[k]='T';
					break;
				default:
					break;
			}
		}
		cout << flag << endl;

		if(thread_log==0)
		{
			struct timeval tvs,tve;
			gettimeofday(&tvs,NULL);

			struct Node * root;
			root=NodeInitial();

			for(uint32_t i=0;i<len;i++)
			{
				uint32_t f=1;
				for(uint32_t x=0;x<level;x++)
				{
					if(i+x<len)
					{
						if(ref[i+x]!=flag[x])
						{
							f=0;
							break;
						}
					}
					else
					{
						if(flag[x]!='A')
						{
							f=0;
							break;
						}
					}
				}
				if(f==1)
				{
					char tmp;
					if(i==0)
					{
						tmp='$';
					}
					else
					{
						tmp=para.ref[i-1];
					}
					insert(&root,root, i,tmp,para);
				}
			}

			gettimeofday(&tve,NULL);
			double span = tve.tv_sec-tvs.tv_sec + (tve.tv_usec-tvs.tv_usec)/1000000.0;
			cout <<"time of construct B+tree is: "<<span<<endl;

			struct Node * mini;
			mini=Find_Minimal_Node(root);

			while(mini!=NULL)
			{
				for(uint8_t i=0;i<mini->nodesize;i++)
				{
					b[b_len_cur]=mini->b[i];
					if(mini->b[i]=='$')
					{
						dollar_pos=b_len_cur;
					}

					if(b_len_cur%sa_gap==0)
					{
						p_sa[sa_len_cur]=mini->data[i];
						sa_len_cur++;
					}

					b_len_cur++;
				}
				mini=mini->brother;
			}
			treeDestroy(root);
		}
		else
		{
			struct timeval tvs,tve;
			gettimeofday(&tvs,NULL);

			uint32_t thread_num=pow(4,thread_log);
			pthread_t *t;
			t=(pthread_t *)malloc(sizeof(pthread_t)*thread_num);
			struct Node ** root;
			root=(Node **)malloc(sizeof(Node *)*thread_num);
			struct bplusTree_para * a;
			a=(struct bplusTree_para *)malloc(sizeof(struct bplusTree_para)*thread_num);

			for(uint32_t i=0;i<thread_num;i++)
			{
				a[i].p=root+i;
				a[i].threadID=i;
				a[i].para=para;
				a[i].level=level;
				a[i].flag=flag;
				a[i].thread_log=thread_log;
			}
			for(uint32_t i=0;i<thread_num;i++)
			{
				if(pthread_create(t+i, NULL, bplusTree, (void*)(a+i))!=0)
				{
					cout << "error!" << endl;
				}
			}
			for(uint32_t i=0;i<thread_num;i++)
			{
				pthread_join(t[i], NULL);
			}

			gettimeofday(&tve,NULL);
			double span = tve.tv_sec-tvs.tv_sec + (tve.tv_usec-tvs.tv_usec)/1000000.0;
			cout <<"time of construct B+tree is: "<<span<<endl;

			struct Node * mini;
			mini=Find_Minimal_Node(root[0]);
			struct Node * mini_total;
			mini_total=mini;
			uint32_t block_size=0;
			while(1)
			{
				if(mini->brother==NULL)
				{
					break;
				}
				block_size=block_size+mini->nodesize;
				mini=mini->brother;
			}
			for(uint32_t j=1;j<thread_num;j++)
			{
				mini->brother=Find_Minimal_Node(root[j]);
				while(1)
				{
					if(mini->brother==NULL)
					{
						break;
					}
					block_size=block_size+mini->nodesize;
					mini=mini->brother;
				}
			}
			block_size=block_size+mini->nodesize;

			struct Node ** p_start;
			struct Node ** p_end;

			p_start=(struct Node **)malloc(sizeof(struct Node *)*thread_num);
			p_end=(struct Node **)malloc(sizeof(struct Node *)*thread_num);

			uint32_t *len_start;
			len_start=(uint32_t *)malloc(sizeof(uint32_t)*thread_num);


			mini=mini_total;
			p_start[0]=mini;
			len_start[0]=block_start;
			uint32_t len_cur=0;
			for(uint32_t i_thread=0;i_thread<thread_num-1;i_thread++)
			{
				while(1)
				{
					len_cur=len_cur+mini->nodesize;
					if(len_cur>i_thread*block_size/thread_num)
					{
						p_end[i_thread]=mini;
						p_start[i_thread+1]=mini->brother;
						len_start[i_thread+1]=len_cur+block_start;
						mini=mini->brother;
						break;
					}
					mini=mini->brother;
				}
			}
			while(1)
			{
				if(mini->brother==NULL)
				{
					break;
				}
				mini=mini->brother;
			}
			p_end[thread_num-1]=mini;

			block_start=block_start+block_size;

			struct mult_bsa_para *para_bsa;
			para_bsa=(struct mult_bsa_para *)malloc(sizeof(struct mult_bsa_para)*thread_num);

			for(uint32_t j=0;j<thread_num;j++)
			{
				para_bsa[j].b=b;
				para_bsa[j].len_start=len_start[j];
				para_bsa[j].sa=p_sa;
				para_bsa[j].p_start=p_start[j];
				para_bsa[j].p_end=p_end[j];
				para_bsa[j].a=&dollar_pos;
				para_bsa[j].sa_gap=sa_gap;
			}

			for(uint32_t i=0;i<thread_num;i++)
			{
				if(pthread_create(t+i, NULL, mult_bsa, (void*)(para_bsa+i))!=0)
				{
					cout << "error!" << endl;
				}
			}
			for(uint32_t i=0;i<thread_num;i++)
			{
				pthread_join(t[i], NULL);
			}

			free(t);
			free(a);
			free(para_bsa);
			free(p_start);
			free(p_end);
			free(len_start);

			for(uint32_t j=0;j<thread_num;j++)
			{
				treeDestroy(root[j]);
			}
		}
	}

	fwrite(&para_build.sa_gap,sizeof(uint32_t),1,out_C);
	fwrite(&para_build.occ_gap,sizeof(uint32_t),1,out_C);
	fwrite(b,sizeof(char),len+1,out_B);
	fwrite(p_sa,sizeof(uint32_t),len/sa_gap+1,out_SA);

	free(b);
	free(p_sa);
	fclose(out_C);
	fclose(out_B);
	fclose(out_SA);
}
void build_occ(char c,struct build_para para_build)
{
	char * b = NULL;
	uint32_t len;

	FILE * out_OCC;
	out_OCC=fopen("./index/OCC","wb+");

	ReadSeq(&b,&len,"./index/B");

	uint32_t *a;
	a=(uint32_t*)malloc(sizeof(uint32_t)*len);
	a[0]=0;
	if(b[0]==c)
	{
		a[0]=1;
	}
	fwrite(a,sizeof(uint32_t),1,out_OCC);
	for(uint32_t i=1;i<len;i++)
	{
		if(b[i]==c)
		{
			a[i]=a[i-1]+1;
		}
		else
		{
			a[i]=a[i-1];
		}
		if(i % para_build.occ_gap==0)
		{
			fwrite(a+i,sizeof(uint32_t),1,out_OCC);
		}
	}
}

void build_occA(struct build_para para_build)
{
	char * b = NULL;
	uint32_t len = 0;

	FILE * B_file = NULL;
	B_file = fopen("./index/B","r");

	FILE * out_OCCA = NULL;
	out_OCCA = fopen("./index/OCCA","wb+");

	if(B_file)//打开文件一定要判断是否成功
	{
		fseek(B_file,0,SEEK_END);//将文件内部的指针指向文件末尾
		len = ftell(B_file);//获取文件长度，（得到文件位置指针当前位置相对于文件首的偏移字节数）
		rewind(B_file);//将文件内部的指针重新指向一个流的开头
		b = (char *) malloc(len*sizeof(char));//申请内存空间，len*sizeof(char)是为了更严谨，16位上char占一个字符，其他机器上可能变化

		//用malloc申请的内存是没有初始值的，如果不赋值会导致写入的时候找不到结束标志符而出现内存比实际申请值大，写入数据后面跟随乱码的情况
		memset(b,0,len*sizeof(char));//将内存空间都赋值为‘\0’

		fread(b,sizeof(char),len,B_file);
	}
	uint32_t *a;
	a = (uint32_t*)malloc(sizeof(uint32_t)*4);
	memset(a,0,sizeof(uint32_t)*4);
	for(uint32_t i = 0; i <= len; i++)
	{
		if(i != 0)
		{
			switch(b[i-1])
			{
				case 'A':
					a[0]++;
					break;
				case 'C':
					a[1]++;
					break;
				case 'G':
					a[2]++;
					break;
				case 'T':
					a[3]++;
					break;
				default:
					break;
			}
		}
		if(i % para_build.occ_gap == 0)
		{
//			fprintf(out_OCCA, "%b ", a[0]);
//			fprintf(out_OCCA, "%b ", a[1]);
//			fprintf(out_OCCA, "%b ", a[2]);
//			fprintf(out_OCCA, "%b ", a[3]);
			fwrite(a,sizeof(uint32_t)*4,1,out_OCCA);
		}
	}
	free(a);
	free(b);
	fclose(out_OCCA);
	fclose(B_file);
}




