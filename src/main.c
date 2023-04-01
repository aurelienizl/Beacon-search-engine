#include "crawler/crawler.h"

int main(void)
{
	char *url = "http://www.scholarpedia.org/w/index.php?title=Out-of-time-order_correlations_and_quantum_chaos&curid=55237&diff=199487&oldid=199485";
	crawl(url);

	return 0;
}