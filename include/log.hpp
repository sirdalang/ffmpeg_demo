#pragma once

#if 1
#define xdebug(...) do {printf("[debug][%s]", \
	__FUNCTION__);printf(__VA_ARGS__);printf("\n");} while (0)
#define xinfo(...) do {printf("[info][%s]", \
	__FUNCTION__);printf(__VA_ARGS__);printf("\n");} while (0)
#define xerror(...) do {printf("[error][%s]", \
	__FUNCTION__);printf(__VA_ARGS__);printf("\n");} while (0)
#else
#define xdebug(...) do{}while(0)
#define xinfo(...) do{}while(0)
#define xerror(...) do{}while(0)
#endif