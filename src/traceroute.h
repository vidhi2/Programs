/*
 *   Simple Implementation of Traceroute in C
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define ERROR 0
#define ICMP_LEN 8
#define TTL_MAX 30
#define BUF_SZ 128
#define TIMEOUT 1000

typedef struct timeval Time;

double cmpTime(Time startTime, Time endTime) {
	double diffSec = (endTime.tv_sec - startTime.tv_sec);
	double diffUSec = (endTime.tv_usec - startTime.tv_usec);

	return diffSec*1000 + diffUSec/1000;
}

u_int16_t ICMP_CheckSum(const void* buffer, int len) {
	u_int32_t sum;
	const u_int16_t* ptr = buffer;
	for(sum = 0; len > 0; len-= 2) {
		sum += *ptr++;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

int host_to_ip(char* hostname, char* ip) {
    struct hostent *hent;
    struct in_addr **addr_list;

    if((hent = gethostbyname(hostname)) == NULL) {
        return -1;
    }

    addr_list = (struct in_addr **) hent->h_addr_list;
    
    int i = 0;
    for(;addr_list[i] != NULL; ++i) {
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return 1;
    }
    return 0;
}

char* ip_to_host(char* ip) {
	struct hostent* hent;
	struct in_addr addr;

	inet_pton(AF_INET, ip, &addr);
	hent = gethostbyaddr(&addr, sizeof(addr), AF_INET);
	if(hent == NULL) {
		return ip;
	}
	return hent->h_name;
}
void outputPath(int ttl, int numReplies, char* ip[3]
	, Time sendTime[3], Time recvTime[3]) {

	// Print ttl
	printf("%2d. ", ttl);

	char* host[3];
	for(int i = 0; i < 3; ++i) {
		host[i] = ip_to_host(ip[i]);
	}
	// Print * if no Reply
	if(numReplies == 0) {
		printf("* * * \n");
	} else {
		// Print IP Addresses
		if(ip[0] == ip[1] && ip[1] == ip[2]) {
			printf("%-20s ", host[0]);
			printf("%-16s", ip[0]);
		}

		// Print Response Time
		if(numReplies < 3) {
			printf("$ \t$ \t$\n");
		} else {
			for(int i = 0; i < 3; ++i) {
				double diffTime = cmpTime(sendTime[i], recvTime[i]);
				printf(" %.3f ms", diffTime);
			}
			printf("\n");
		}
	}
}


