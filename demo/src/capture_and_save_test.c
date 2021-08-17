
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#include <pcap.h>
#include <signal.h>



int count = 0;
static void my_countme(u_char *user, const struct pcap_pkthdr *h, const u_char *sp)
{
	++count;

	pcap_dumper_t * dumper = (pcap_dumper_t *)user;

	
	bpf_u_int32 len = h->caplen;
	bpf_u_int32 i = 0;
	for (; i < len; ++i)
	{
		printf("%02x ", *(sp + i));
	}
	printf("\n");

	pcap_dump(user, h, sp);
	pcap_dump_flush(dumper);
}

pcap_t * g_pcap = NULL;

void break_loop(int sig)
{
	if (sig == SIGINT )
	{
		if (g_pcap != NULL)
		{
			pcap_breakloop(g_pcap);
		}
		
	}
}

int main(int argc, char* argv[])
{

	signal(SIGINT, break_loop);
	int res = -1;
	if (argc < 3)
	{
		printf("usage: ./capture_and_save_test iface pcap_file\n");
		return res;
	}

	const char* iface = argv[1];
	const char* pcap_file = argv[2];
	int snaplen = 65535;
	int promisc = 1;
	int to_ms = 1000;
	char ebuf[PCAP_ERRBUF_SIZE] = { 0 };
	pcap_t * handle = pcap_open_live(iface, snaplen, promisc, to_ms, ebuf);
	if (handle == NULL)
	{
		printf("pcap_open_live failed, reason is %s!\n", ebuf);
		goto err;
	}

	printf("pcap_open_live succeed!\n");

	g_pcap = handle;

	// 编译规则
	struct bpf_program program;
	//char bpf_buf[512] = "dst port 80 and dst port 443";
	// char bpf_buf[512] = "tcp[tcpflags]=tcp-syn and (dst port 80 or dst port 443 )";
	//char bpf_buf[512] = "tcp[tcpflags] & tcp-ack!=0";
	// char bpf_buf[512] = "ip[2:2]>40 and (tcp[20:2]=0x4745 or tcp[20:2]=0x4854)"; // total_len > 40 而且 开头是GE 或者 HT
	char bpf_buf[512] = "ip[2:2]>40 and (dst port 443 or dst port 80)"; // http 和 https的包（不包含握手包)
	// udp or (tcp and ip[2:2]>40) // udp 包 和 tcp长度>0的包（不包含握手包)
	
	int optimize = 0; // 是否需要优化表达式
	if (pcap_compile(handle, &program, bpf_buf, optimize, 0) != 0)
	{
		printf("pcap_compile failed, reason is %s\n", pcap_geterr(handle));
		goto err;
	}

	printf("pcap_compile succeed!\n");

	if (pcap_setfilter(handle, &program) != 0)
	{
		printf("pcap_setfilter failed, reason is %s\n", pcap_geterr(handle));
		goto err;
	}

	printf("pcap_setfilter succeed!\n");


	pcap_dumper_t * dumper = pcap_dump_open(handle, pcap_file);
	if (dumper == NULL)
	{
		printf("pcap_dump_open failed, reason is %s", pcap_geterr(handle));
		goto err;
	}
	printf("pcap_dump_open succeed!\n");

	// pcap_dispatch(handle, -1, pcap_dump, dumper);
	// pcap_dispatch(handle, 0, my_countme, dumper); // linux 实现中，cnt没有用
	pcap_loop(handle, 0, my_countme, dumper);
	printf("pcap_dispatch over , reason is %s!\n", pcap_geterr(handle));
	pcap_dump_flush(dumper);
	//pcap_breakloop(handle); 

	printf("total capture packet num %d!\n", count);
	res = 0;
	
	printf("capture over!\n");

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

