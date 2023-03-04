#ifndef _CRAWLER_H_
#define _CRAWLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>


char* get_page(const char *host);

#endif
