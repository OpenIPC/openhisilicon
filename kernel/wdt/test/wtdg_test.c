/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "watchdog.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif 
#endif/*__cplusplus*/


#define DEV_FILE "/dev/watchdog"

void print_help()
{
	fprintf(stdout, 
			"\n"
			"Usage: ./test [options] ...\n"
			"Options: \n"
			"    -s(set)  e.g. -s timeout 30\n"
			"    -g(get)  e.g. -g timeout\n"
			"    -f(feed) e.g. -f\n"
			);
}

int parse_args_and_proc(int fd, int argc, char* argv[])
{
	int ret = 0;
	int timeout;

	if (1==argc){
		print_help();
		return -1;
	}
    int i=argc-1;
    while(i>0)
    {
        printf("%d: %s  ",i, argv[i]);
        i--;
    }
    printf("\n");
	if ((0 == strcmp("-s", argv[1]))&&(4 == argc)){
		if (0 == strcmp("timeout", argv[2])){
			timeout = atoi(argv[3]);
			ret = ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
			if (0 != ret){
				fprintf(stderr, "[Error]%s(%d): ioctl ret =%d\n", __FUNCTION__, __LINE__, ret);
				return -1;
			}
		}
        else if (0 == strcmp("option", argv[2])){
			timeout = atoi(argv[3]);
            printf("set new option :%d \n", timeout);
			ret = ioctl(fd, WDIOC_SETOPTIONS, &timeout);
			if (0 != ret){
				fprintf(stderr, "[Error]%s(%d): ioctl ret =%d\n", __FUNCTION__, __LINE__, ret);
				return -1;
			}
		}
		else{
			print_help();
		}
	}
	else if ((0 == strcmp("-g", argv[1]))&&(3 == argc)){
		if (0 == strcmp("timeout", argv[2])){
			ret = ioctl(fd, WDIOC_GETTIMEOUT, &timeout);
			if (0 != ret){
				fprintf(stderr, "[Error]%s(%d): ioctl ret =%d\n", __FUNCTION__, __LINE__, ret);
				return -1;
			}
			else{
				fprintf(stdout, "Resp: timeout=%d\n", timeout);
			}
		}
        if (0 == strcmp("option", argv[2])){
            printf("Line:%d \n", __LINE__);
			ret = ioctl(fd, WDIOC_GETSTATUS, &timeout);
			if (0 != ret){
				fprintf(stderr, "[Error]%s(%d): ioctl ret =%d\n", __FUNCTION__, __LINE__, ret);
				return -1;
			}
			else{
				fprintf(stdout, "Resp: option=%d\n", timeout);
			}
		}
	}
	else if ((0 == strcmp("-f", argv[1])) && (2==argc)){
		ret = ioctl(fd, WDIOC_KEEPALIVE);
		if (0 != ret){
			fprintf(stderr, "[Error]%s(%d): ioctl ret =%d\n", __FUNCTION__, __LINE__, ret);
			return -1;
		}
	}
	else{
		print_help();
	}
	return ret;
}

#ifdef __LITEOS__
int app_main(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	
	int fd  = open(DEV_FILE, O_RDWR);
	if (fd < 0){
		fprintf(stderr, "fail to open file:%s\n", DEV_FILE);
		return -1;
	}

	parse_args_and_proc(fd, argc, argv); 

	close(fd);
	return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif 
#endif/*__cplusplus*/


