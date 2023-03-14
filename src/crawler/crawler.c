#include "crawler.h"

size_t HandleCallback(void *contents, size_t size, size_t nmemb, void *userdata)
{

  // Calculate total size
  size_t total_size = size * nmemb;

  // Cast userdata to Memory
  Memory *content = (Memory *)userdata;

  // Reallocate memory
  content->data = realloc(content->data, content->size + total_size + 1);

  // Check if realloc() failed
  if (content->data == NULL)
  {
    printf("Error: realloc() failed\n");
    return 0;
  }

  // Copy contents to data
  memcpy(&(content->data[content->size]), contents, total_size);

  // Set last byte to 0
  content->size += total_size;
  content->data[content->size] = 0;

  return total_size;
}

int GetPage(const char *url, const char *user_agent, Memory *content)
{

  CURL *request;
  CURLcode res;

  // Current user agent
  char current_user_agent[USER_AGENT_MAX_SIZE];

  // Check if url is valid
  if (url == NULL || strlen(url) == 0)
  {
    return -1;
  }

  // Set current user agent to 0
  explicit_bzero((char *)&current_user_agent, USER_AGENT_MAX_SIZE);

  // Check if user agent is valid
  if (user_agent == NULL)
  {
    strcpy(current_user_agent, DEFAULT_UAGENT);
  }
  else
  {
    if (strlen(user_agent) >= USER_AGENT_MAX_SIZE)
    {
      return -1;
    }
    strcpy(current_user_agent, user_agent);
  }

  curl_global_init(CURL_GLOBAL_ALL);

  // Initialize request
  request = curl_easy_init();

  curl_easy_setopt(request, CURLOPT_URL, url);
  curl_easy_setopt(request, CURLOPT_WRITEFUNCTION, HandleCallback);
  curl_easy_setopt(request, CURLOPT_WRITEDATA, (void *)content);
  curl_easy_setopt(request, CURLOPT_USERAGENT, current_user_agent);

  // Perform request
  res = curl_easy_perform(request);

  // Check if request failed
  if (res != CURLE_OK)
  {
    return -1;
  }

  // Cleanup
  curl_easy_cleanup(request);
  curl_global_cleanup();

  return 0;
}

char* crawl(char* url)
{
  Memory content;
  content.data = malloc(1);
  content.size = 0;

  GetPage(url, NULL, &content);

  return content.data;
}