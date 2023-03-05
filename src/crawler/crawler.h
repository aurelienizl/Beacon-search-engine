#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Default user agent max size
#define USER_AGENT_MAX_SIZE 255

// Default user agent
#define DEFAULT_UAGENT "Mozilla/5.0 (Linux; Android 11; SM-A525M) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.0.0 Mobile Safari/537.36"

/**
 * @brief Memory struct
 * @param data Pointer to data
 * @param size Size of data
 **/

typedef struct
{
  char *data;
  size_t size;
} Memory;

/**
 * @brief Callback function for curl
 * @param contents Pointer to data
 * @param size Size of data
 * @param nmemb Number of members
 * @param userdata Pointer to userdata
 * @return Size of data
 **/

size_t HandleCallback(void *content, size_t size, size_t nmemb, void *userp);

/**
 * @brief Get page from url
 * @param url URL
 * @param user_agent User agent
 * @param Memory Pointer to Memory struct
 * @return Error code
 **/

int GetPage(const char *url, const char *user_agent, Memory *Memory);
