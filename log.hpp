#pragma once

#define xdebug(...) do {printf("[debug][%s]", \
	__FUNCTION__);printf(__VA_ARGS__);} while (0)
#define xinfo(...) do {printf("[info][%s]", \
	__FUNCTION__);printf(__VA_ARGS__);} while (0)
#define xerror(...) do {printf("[error][%s]", \
	__FUNCTION__);printf(__VA_ARGS__);} while (0)
