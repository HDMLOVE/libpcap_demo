
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#ifdef _WIN32
#include "getopt.h"
#else
#include <unistd.h>
#endif
#include <errno.h>
#include <sys/types.h>

#include <pcap.h>


static void my_countme(u_char *user, const struct pcap_pkthdr *h, const u_char *sp)
{
	pcap_dumper_t * dumper = (pcap_dumper_t *)user;


	bpf_u_int32 len = h->caplen;
	bpf_u_int32 i = 0;
	for (; i < len; ++i)
	{
		printf("%02x ", *(sp + i));
	}
	printf("\n");

	// pcap_dump(user, h, sp);

}

int main(int argc, char* argv[])
{
	int res = -1;
	if (argc < 2)
	{
		printf("usage: ./a.out pcap_file\n");
		return res;
	}

	const char* pcap_file = argv[1];
	char ebuf[PCAP_ERRBUF_SIZE] = { 0 };
	pcap_t * handle = pcap_open_offline(pcap_file, ebuf);
	if (handle == NULL)
	{
		printf("pcap_open_offline failed, reason is %s!\n", ebuf);
		goto err;
	}

	printf("pcap_open_offline succeed!\n");

	pcap_dumper_t * dumper = pcap_dump_open(handle, pcap_file);
	if (dumper == NULL)
	{
		printf("pcap_dump_open failed, reason is %s", pcap_geterr(handle));
		goto err;
	}
	printf("pcap_dump_open succeed!\n");

	// pcap_dispatch(handle, -1, pcap_dump, dumper);
	pcap_dispatch(handle, -1, my_countme, dumper);

	res = 0;

	printf("read from %s over!\n", pcap_file);

err:
	if (dumper != NULL)
	{
		pcap_dump_close(dumper);
	}

	if (handle != NULL)
	{
		pcap_close(handle);
	}


	return res;
}

