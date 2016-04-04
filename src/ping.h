/*
	Implementation of ping in C
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define ICMP_SZ (sizeof(struct icmp))

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define BUF_SZ 1024
#define NUM 10

#define uChar unsigned char
#define uShort unsigned short
#define uInt unsigned int

#define Time struct timeval

// ICMP Struct Header
struct icmp {
	uChar type;
	uChar code;
	uShort checksum;
	uShort id;
	uShort sequence;
	Time timestamp;
};

// IP Struct Header
struct ip {
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		uChar hlen: 4;
		uChar version: 4;
	#endif
	#if __BYTE_ORDER == __BIG_ENDIAN
		uChar version: 4;
		uChar hlen: 4;
	#endif
	
	uChar tos;
	uShort len;
	uShort offset;
	uShort id;
	uChar ttl;
	uChar protocol;
	uShort checksum;

	struct in_addr ipsrc;
	struct in_addr ipdst;
};

char buf[BUF_SZ] = {0};
double totalTime = 0;

uShort ICMP_CheckSum(uShort* addr, int len);
double cmpTime(Time startTime, Time endTime);
void pack(struct icmp* icmp, int sequence);
int unPack(char* buf, int len, char* addr);
char* ip_to_host(char* ip);
