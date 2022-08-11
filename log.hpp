#pragma once

#if 0
#define xdebug(x...) do {printf("[debug][%s %d %s]", \
	__FILE__,__LINE__,__FUNCTION__);printf(x);} while (0)
#define xinfo(x...) do {printf("[info][%s %d %s]", \
	__FILE__,__LINE__,__FUNCTION__);printf(x);} while (0)
#define xerror(x...) do {printf("[error][%s %d %s]", \
	__FILE__,__LINE__,__FUNCTION__);printf(x);} while (0)
#else
#define xdebug(x...) do {printf("[debug][%s]", \
	__FUNCTION__);printf(x);} while (0)
#define xinfo(x...) do {printf("[info][%s]", \
	__FUNCTION__);printf(x);} while (0)
#define xerror(x...) do {printf("[error][%s]", \
	__FUNCTION__);printf(x);} while (0)
#endif 
