
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#include <pcap.h>

#include "ethernet_header.h"


static void parse_ip_pkt(const u_char *sp, bpf_u_int32 len)
{
	ip_hdr* ih = (ip_hdr*)(sp);
	printf("ihl=%d, version=%d, tos=%d, tot_len=%u, id=%u, frag_off=%u, ttl=%u, protocol=%d, ",
		ih->ihl, ih->version, ih->tos, ntohs(ih->tot_len), ntohs(ih->id), ih->frag_off, ih->ttl, ih->protocol);



}

void char_to_hexstr(const unsigned char* source, char* dest, int sourceLen)
{
	short i;
	unsigned char highByte, lowByte;

	for (i = 0; i < sourceLen; i++)
	{
		highByte = source[i] >> 4;
		lowByte = source[i] & 0x0f;

		highByte += 0x30;

		if (highByte > 0x39)
			dest[i * 2] = highByte + 0x07;
		else
			dest[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			dest[i * 2 + 1] = lowByte + 0x07;
		else
			dest[i * 2 + 1] = lowByte;
	}
	return;
}

static int get_mac_addr_from_header(eth_hdr* eh, char dst_mac[17], char src_mac[17])
{
	memset(dst_mac, 0x00, 17);
	memset(src_mac, 0x00, 17);

	eh->dstmac;
	eh->srcmac;
	
	char hexstr[20] = { 0 };
	int i = 0;
	int mac_len = sizeof(eh->dstmac);
	for (; i < mac_len; ++i)
	{
		char_to_hexstr(eh->dstmac + i, hexstr, 1);
		strcpy(dst_mac + strlen(dst_mac), hexstr);
		
		if (i != mac_len - 1)
		{
			dst_mac[strlen(dst_mac)] = ':';
		}
		memset(hexstr, 0x00, sizeof(hexstr));
	}

	for ( i = 0 ; i < mac_len; ++i)
	{
		char_to_hexstr(eh->srcmac + i, hexstr, 1);
		strcpy(src_mac + strlen(src_mac), hexstr);

		if (i != mac_len - 1)
		{
			src_mac[strlen(src_mac)] = ':';
		}
		memset(hexstr, 0x00, sizeof(hexstr));
	}

	return 0;

}

int count = 0;
static void my_countme(u_char *user, const struct pcap_pkthdr *h, const u_char *sp)
{
	pcap_dumper_t * dumper = (pcap_dumper_t *)user;


	bpf_u_int32 len = h->caplen;

	eth_hdr* eh = (eth_hdr*)sp;

	char dst_mac[17], src_mac[17];
	get_mac_addr_from_header(eh, dst_mac, src_mac);

	eh->eth_type = ntohs(eh->eth_type);

	printf("dst=%s, src=%s, eth_type=%d, ", dst_mac, src_mac, eh->eth_type);

	switch (eh->eth_type)
	{
	case 0x0806:
		printf("proto: arp, ");
		break;
	case 0x0800:
		printf("proto: ip, ");
		parse_ip_pkt(sp + sizeof(eth_hdr), len - sizeof(eth_hdr));
		break;
	case 0x0835:
		printf("proto: rarp, ");
		break;
	default:
		printf("proto: unknown, ");
		break;
	}

	++count;
	printf("\n");
}

int main(int argc, char* argv[])
{
	int res = -1;
	if (argc < 2)
	{
		printf("usage: ./parse_pcap_pkt pcap_file\n");
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

	// pcap_dispatch(handle, -1, my_countme, dumper);
	pcap_loop(handle, -1, my_countme, NULL);
	printf("pcap_dispatch over , reason is %s!\n", pcap_geterr(handle));

	printf("total count=%d\n", count);
	res = 0;

	printf("read from %s over!\n", pcap_file);

err:

	if (handle != NULL)
	{
		pcap_close(handle);
	}

	return res;
}

