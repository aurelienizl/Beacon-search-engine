#include "crawler.h"
#include <signal.h>

#pragma region Parameters

int max_con = 20;
int max_total = 20000;
int max_requests = 5000;
size_t max_link_per_page = 50;
int follow_relative_links = 1;

volatile sig_atomic_t pending_interrupt = 0;

pthread_mutex_t mutex_io = PTHREAD_MUTEX_INITIALIZER;

#pragma endregion Parameters

#pragma region Crawler

void sighandler(int dummy)
{
	printf("Interrupted %i\n", dummy);
	pending_interrupt = 1;
}

size_t grow_buffer(void *contents, size_t sz, size_t nmemb, void *ctx)
{
	size_t realsize = sz * nmemb;
	memory *mem = (memory *)ctx;
	char *ptr = realloc(mem->buf, mem->size + realsize);
	if (!ptr)
	{
		/* out of memory */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}
	mem->buf = ptr;
	memcpy(&(mem->buf[mem->size]), contents, realsize);
	mem->size += realsize;
	return realsize;
}

CURL *make_handle(char *url)
{
	if (url == NULL)
	{
		printf("URL is NULL\n");
		return NULL;
	}

	CURL *handle = curl_easy_init();

	if (!handle)
	{
		printf("Cannot initialize CURL handle\n");
		return NULL;
	}

	/* Important: use HTTP2 over HTTPS */
	curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
	curl_easy_setopt(handle, CURLOPT_URL, url);

	memory *mem = malloc(sizeof(memory));
	if (!mem)
	{
		printf("Cannot allocate memory\n");
		curl_easy_cleanup(handle);
		return NULL;
	}

	mem->size = 0;
	mem->buf = malloc(1);
	if (!mem->buf)
	{
		printf("Cannot allocate memory for buffer\n");
		curl_easy_cleanup(handle);
		free(mem);
		return NULL;
	}

	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, grow_buffer);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, mem);
	curl_easy_setopt(handle, CURLOPT_PRIVATE, mem);

	/* For completeness */
	curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");
	curl_easy_setopt(handle, CURLOPT_TIMEOUT, 5L);
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 10L);
	curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 2L);
	curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt(handle, CURLOPT_FILETIME, 1L);
	curl_easy_setopt(handle, CURLOPT_USERAGENT, "Googlebot/2.1 (+http://www.google.com/bot.html)");
	curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
	curl_easy_setopt(handle, CURLOPT_UNRESTRICTED_AUTH, 1L);
	curl_easy_setopt(handle, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
	curl_easy_setopt(handle, CURLOPT_EXPECT_100_TIMEOUT_MS, 0L);

	return handle;
}

void log_message(const char* message)
{
    printf("%s\n", message);
}

int is_php_url(const char* url)
{
    return strstr(url, ".php") != NULL;
}

int is_already_crawled(const char* url)
{
	// Lock the mutex before accessing the file
	pthread_mutex_lock(&mutex_io);
    if (exist_webpage((char*) url))
	{
		// Unlock the mutex after accessing the file
		pthread_mutex_unlock(&mutex_io);
		return 1;
	}
	// Unlock the mutex after accessing the file
	pthread_mutex_unlock(&mutex_io);
	return 0;
}

size_t follow_links(CURLM* multi_handle, memory* mem, char* url, char* domain)
{
    write_html_to_file(mem->buf, url);
    int opts = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET;
    htmlDocPtr doc = htmlReadMemory(mem->buf, mem->size, url, NULL, opts);
    if (!doc)
    {
        log_message("Cannot read memory");
        return 0;
    }
    
    xmlChar* xpath = (xmlChar*)"//a/@href";
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
    xmlXPathFreeContext(context);
    
    if (!result)
        return 0;
    
    xmlNodeSetPtr nodeset = result->nodesetval;
    if (xmlXPathNodeSetIsEmpty(nodeset))
    {
        xmlXPathFreeObject(result);
        return 0;
    }
    
    size_t count = 0;
    int i;
    for (i = 0; i < nodeset->nodeNr; i++)
    {
        double r = rand();
        int x = r * nodeset->nodeNr / RAND_MAX;
        const xmlNode* node = nodeset->nodeTab[x]->xmlChildrenNode;
        xmlChar* href = xmlNodeListGetString(doc, node, 1);
        
        if (follow_relative_links)
        {
            xmlChar* orig = href;
            href = xmlBuildURI(href, (xmlChar*)url);
            xmlFree(orig);
        }
        
        char* link = (char*)href;
        if (!link || strlen(link) < 1)
            continue;
        
        if (strncmp(link, "http://", 7) == 0 || strncmp(link, "https://", 8) == 0)
        {
            if (is_domain(link, domain))
            {
                if (!is_already_crawled(link))
                {
                    if (!is_php_url(link))
                    {
                        curl_multi_add_handle(multi_handle, make_handle(link));
                        if (++count == max_link_per_page)
                            break;
                    }
                    else
                    {
                        log_message("Skipping URL, it has a PHP format");
                    }
                }
                else
                {
                    log_message("Skipping URL, it has already been crawled");
                }
            }
            else
            {
                log_message("Skipping URL, it does not belong to the specified domain");
            }
        }
        
        xmlFree(link);
    }
    xmlXPathFreeObject(result);
    xmlFreeDoc(doc);
    return count;
}

int is_html(char *ctype)
{
	return ctype != NULL && strlen(ctype) > 10 && strstr(ctype, "text/html");
}

void* crawl(void *url_void)
{
	char* url = (char*) url_void;
	char *domain = get_domain(url);
	if (!domain)
	{
		printf("Cannot get domain\n");
		return NULL;
	}

	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_handler = sighandler;
	if (sigaction(SIGINT, &action, NULL) < 0)
	{
		printf("Cannot set signal handler\n");
		free(domain);
		return NULL;
	}

	LIBXML_TEST_VERSION;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURLM *multi_handle = curl_multi_init();
	if (!multi_handle)
	{
		printf("Cannot initialize CURL multi handle\n");
		free(domain);
		return NULL;
	}
	curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_con);
	curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, 6L);

	/* enables http/2 if available */
#ifdef CURLPIPE_MULTIPLEX
	curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
#endif

	/* sets html start page */
	curl_multi_add_handle(multi_handle, make_handle(url));

	int msgs_left;
	int pending = 0;
	int complete = 0;
	int still_running = 1;
	while (still_running && !pending_interrupt)
	{
		int numfds;
		curl_multi_wait(multi_handle, NULL, 0, 1000, &numfds);
		curl_multi_perform(multi_handle, &still_running);

		/* See how the transfers went */
		CURLMsg *m = NULL;
		while ((m = curl_multi_info_read(multi_handle, &msgs_left)))
		{
			if (m->msg == CURLMSG_DONE)
			{
				CURL *handle = m->easy_handle;
				char *url;
				memory *mem;
				curl_easy_getinfo(handle, CURLINFO_PRIVATE, &mem);
				curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &url);
				if (m->data.result == CURLE_OK)
				{
					long res_status;
					curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &res_status);
					if (res_status == 200)
					{
						char *ctype;
						curl_easy_getinfo(handle, CURLINFO_CONTENT_TYPE, &ctype);
						printf("[%d] HTTP 200 (%s): %s\n", complete, ctype, url);
						if (is_html(ctype) && mem->size > 100)
						{
							if (pending < max_requests && (complete + pending) < max_total)
							{
								pending += follow_links(multi_handle, mem, url, domain);
								still_running = 1;
							}
						}
					}
					else
					{
						printf("[%d] HTTP %d: %s\n", complete, (int)res_status, url);
					}
				}
				else
				{
					printf("[%d] Connection failure: %s\n", complete, url);
				}
				curl_multi_remove_handle(multi_handle, handle);
				curl_easy_cleanup(handle);
				free(mem->buf);
				free(mem);
				complete++;
				pending--;
			}
		}
	}
	printf("Crawling %s complete\n", domain);
	curl_multi_cleanup(multi_handle);
	curl_global_cleanup();
	free(domain);
	xmlCleanupParser();
	return NULL;
}

#pragma endregion Crawler

#pragma region UrlOperations

int is_domain(const char *url, const char *domain)
{
    if (url == NULL || domain == NULL) {
        return 0;
    }
    return (strstr(url, domain) != NULL) ? 1 : 0;
}

char *get_domain(const char *link)
{
    if (link == NULL) {
        return NULL;
    }

    // Check if link starts with "http" or "https"
    if (strncasecmp(link, "http", 4) != 0) {
        return NULL;
    }

    size_t i = (link[4] == 's') ? 5 : 4;
    if (link[i] != ':' || link[i + 1] != '/' || link[i + 2] != '/') {
        return NULL;
    }

    i += 3;  // Skip "://"
    const char *domain_start = &link[i];
    const char *slash = strchr(domain_start, '/');

    size_t domain_len = (slash != NULL) ? (size_t)(slash - domain_start) : strlen(domain_start);
    char *domain = (char *)malloc(sizeof(char) * (domain_len + 1));

    if (domain != NULL) {
        strncpy(domain, domain_start, domain_len);
        domain[domain_len] = '\0';
    }

    return domain;
}

#pragma endregion UrlOperations

#pragma region FileOperations

void rek_mkdir(char *path)
{
    char *sep = strrchr(path, '/');
    if (sep != NULL)
    {
        *sep = 0;
        rek_mkdir(path);
        *sep = '/';
    }
    if (mkdir(path, 0777) && errno != EEXIST)
        printf("error while trying to create '%s'\n%m\n", path);
}

void create_directory(char *url)
{
    char *db_directory = string_to_heap("db/");
    //char *folder_name = sha1_hash((const unsigned char *)url, strlen(url));
    char *folder_name = base64_encode(url);
    char *path = string_concat(db_directory, folder_name);

    rek_mkdir(path);

    free(folder_name);
    free(path);
    
}

void write_to_path(char* path, char* data)
{
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
    {
        printf("Failed to open file %s\n", path);
        return;
    }

    fprintf(fp, "%s", data);
    fclose(fp);
}

int write_html_to_file(char* data, char* url)
{
    char *db_directory = string_to_heap("db/");
    //char *folder_name = sha1_hash((const unsigned char *)url, strlen(url));
    char *folder_name = base64_encode(url);
    char *path = string_concat(db_directory, folder_name);
    char *filename = string_concat(path, "/index.html");

    create_directory(url);
    
    write_to_path(filename, data);

    free(db_directory);
    free(folder_name);
    free(path);
    free(filename);
    return 1;
}

int exist_webpage(char *url)
{
    char *db_directory = string_to_heap("db/");
    //char *folder_name = sha1_hash((const unsigned char *)url, strlen(url));
    char *folder_name = base64_encode(url);
    char *path = string_concat(db_directory, folder_name);

    DIR *dir = opendir(path);
    if (dir)
    {
        free(path);
        free(db_directory);
        free(folder_name);
        closedir(dir);
        return 1;
    }
    else
    {
        free(path);
        free(db_directory);
        free(folder_name);
        closedir(dir);
        return 0;
    }
}

#pragma endregion FileOperations