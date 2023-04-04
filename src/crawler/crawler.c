#include "crawler.h"

int max_con = 200;
int max_total = 200000;
int max_requests = 5000;
size_t max_link_per_page = 50;
int follow_relative_links = 1;

int pending_interrupt = 0;

void sighandler(int dummy)
{
	printf("Interrupted %i\n", dummy);
	pending_interrupt = 1;
}

/* resizable buffer */
typedef struct
{
	char *buf;
	size_t size;
} memory;

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
	CURL *handle = curl_easy_init();

	/* Important: use HTTP2 over HTTPS */
	curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
	curl_easy_setopt(handle, CURLOPT_URL, url);

	/* buffer body */
	memory *mem = malloc(sizeof(memory));
	mem->size = 0;
	mem->buf = malloc(1);
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
	curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64; rv:60.0) Gecko/20100101 Firefox/60.0");
	curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
	curl_easy_setopt(handle, CURLOPT_UNRESTRICTED_AUTH, 1L);
	curl_easy_setopt(handle, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
	curl_easy_setopt(handle, CURLOPT_EXPECT_100_TIMEOUT_MS, 0L);
	return handle;
}

/* HREF finder implemented in libxml2 but could be any HTML parser */
size_t follow_links(CURLM *multi_handle, memory *mem, char *url, char *domain)
{
	write_to_file(mem->buf, url);
	int opts = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET;
	htmlDocPtr doc = htmlReadMemory(mem->buf, mem->size, url, NULL, opts);
	if (!doc)
		return 0;
	xmlChar *xpath = (xmlChar *)"//a/@href";
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
		const xmlNode *node = nodeset->nodeTab[x]->xmlChildrenNode;
		xmlChar *href = xmlNodeListGetString(doc, node, 1);
		if (follow_relative_links)
		{
			xmlChar *orig = href;
			href = xmlBuildURI(href, (xmlChar *)url);
			xmlFree(orig);
		}
		char *link = (char *)href;
		if (!link || strlen(link) < 20)
			continue;
		if (!strncmp(link, "http://", 7) || !strncmp(link, "https://", 8))
		{
			if (is_domain(link, domain))
			{
				if (!exist_webpage(link))
				{
					if (strstr(link, ".php") == NULL)
					{
						curl_multi_add_handle(multi_handle, make_handle(link));
						if (count++ == max_link_per_page)
						{
							break;
						}
					}
					else
					{
						printf("Skipping \"%s\", url have a php format\n", link);
					}
				}
				else
				{
					printf("Skipping \"%s\", url already crawled\n", link);
				}
			}
			else
			{
				printf("Skipping %s\n", link);
			}
		}
		xmlFree(link);
	}
	xmlXPathFreeObject(result);
	return count;
}

int is_html(char *ctype)
{
	return ctype != NULL && strlen(ctype) > 10 && strstr(ctype, "text/html");
}

int crawl(char *url)
{
	char *domain = get_domain(url);
	printf("Crawling %s\n", domain);

	signal(SIGINT, sighandler);
	LIBXML_TEST_VERSION;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURLM *multi_handle = curl_multi_init();
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
	free(domain);
	curl_multi_cleanup(multi_handle);
	curl_global_cleanup();
	return 0;
}
