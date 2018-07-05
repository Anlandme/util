#ifndef __LOG__H__
#define __LOG__H__

#include <stdio.h>

//fp:file descipter
#define LOG2FILE(fp, ...)  if (fp) fprintf(fp, __VA_ARGS__ );

#define LOG  printf("automan debug!!!FUN:%s,LINE:%d\n",__FUNCTION__,__LINE__)

#endif
