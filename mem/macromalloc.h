#ifndef __MACROMALLOC__H__
#define __MACROMALLOC__H__

#include <stdlib.h>

#define MALLOC(size)  malloc(size);printf("file:%s,function:%s,line:%d,it malloc %d bytes!\n",__FILE__,__FUNCTION__,__LINE__,size)
#define malloc(size) MALLOC(size)

#endif
