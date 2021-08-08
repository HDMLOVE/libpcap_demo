cc=gcc

target=capture_and_save_test

cflags=-Wall -g -I../include/ libpcap.a -ldbus-1 -ldl

all:$(target)
	@echo "compile success"

capture_and_save_test: capture_and_save_test.c
	$(cc) $(cfalgs) $< -o $@  $(cflags)
	

read_pcap_test: read_pcap_test.c
	$(cc) $(cfalgs) $< -o $@  $(cflags) 

parse_pcap_pkt: parse_pcap_pkt.c 
	$(cc) $(cfalgs) $< -o $@  $(cflags) 

clean:
	rm *.o *.pcap $(target) core* a.out -f 
