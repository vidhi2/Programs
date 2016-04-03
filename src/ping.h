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
#define NUM 5

#define uChar unsigned char
#define uShort unsigned short
#define uInt unsigned int

#define Time struct timeval


// ICMP Struct Header
struct icmp {
	uChar type;
	uChar code;
	uShort id;
	uShort checksum;
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

uShort ICMP_CheckSum(const void* buffer, int len) {
	u_int32_t sum;
	const uShort* ptr = (uShort *)buffer;
	for(sum = 0; len > 0; len-= 2) {
		sum += *ptr++;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	return (uShort)(~(sum + (sum >> 16)));
}

double cmpTime(Time startTime, Time endTime) {
	double diffSec = (endTime.tv_sec - startTime.tv_sec);
	double diffUSec = (endTime.tv_usec - startTime.tv_usec);

	return diffSec*1000 + diffUSec/1000;
}

void pack(struct icmp* icmp, int sequence) {
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->id = getpid();
	icmp->sequence = sequence;
	gettimeofday(&icmp->timestamp, 0);
	icmp->checksum = ICMP_CheckSum((uShort *)icmp, ICMP_SZ);
}

int unPack(char* buffer, int len, char* addr) {
	int ipHeaderLen;
	double rtt;
	Time endT;

	struct ip* ip;
	struct icmp* icmp;

	ip = (struct ip*) buffer;
	ipHeaderLen = ip->hlen << 2;

	icmp = (struct icmp*) (buffer + ipHeaderLen);
	len -= ipHeaderLen;

	if(len < 8) {
		printf("ICMP Packet\'s length is less than 8 \n");
		return -1;
	}

	if(icmp->type != ICMP_ECHOREPLY ||
		icmp->id != getpid()) {
		printf("INVALID ICMP Packets Recieved \n");
		return -1;
	}

	gettimeofday(&endT, 0);
	rtt = cmpTime(icmp->timestamp, endT);

	printf("%d bytes from %s: icmp_seq: %u, ttl: %d, rtt: %fms\n",
			len, addr, icmp->sequence, ip->ttl, rtt);

	return 0;
}






