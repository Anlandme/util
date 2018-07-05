#include <stdio.h>
#include "macromalloc.h"

int main(void)
{
		int *p = (void *)malloc(4);
		if (p == NULL)
		{
				//错误处理
				;
		}

		*p  =4;

		return 0;
}
