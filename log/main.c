#include "log.h"

int main(void)
{
		FILE *fd = fopen("log","a+");

		int a = 10;
		char *s = "nima";
		LOG;
		if (fd > 0)
		{
		LOG;
				LOG2FILE(fd,"hello world!!!\na = %d,string = %s\n",a,s)
		LOG;
		}

		LOG;
		return 0;
}
