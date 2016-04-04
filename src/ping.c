#include "ping.h"

int main(int argc, char** argv) {
	in_addr_t inaddr;
	struct hostent* host;
	struct icmp sendICMP;
	int nSend = 0;
	int nRecieved = 0;
	int n = 0;
	int len_sockaddr = sizeof(struct sockaddr_in);
	int from_len = 0;

	if(argc < 2) {
		printf("Args: Hostname/IP not Found\n");
		exit(1);
	}

	int sockfd;
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(sockfd == -1) {
		perror("socket");
		exit(1);
	}

	struct sockaddr_in to;
	to.sin_family = AF_INET;
	if(inaddr = inet_addr(argv[1]) == INADDR_NONE) {
		host = gethostbyname(argv[1]);
		if(host == NULL) {
			printf("Error: Host Not Found\n");
			exit(1);
		}
		to.sin_addr = *(struct in_addr *) host->h_addr_list[0];
	} else {
		to.sin_addr.s_addr = inaddr;
	}

	struct sockaddr_in from;
	memset(&from, 0, len_sockaddr);

	printf("PING %s (%s): %d Bytes of data.\n",
	 argv[1], inet_ntoa(to.sin_addr), (int) ICMP_SZ);

	for(int i = 0; i < NUM; ++i) {
		nSend++;
		memset(&sendICMP, 0, ICMP_SZ);
		pack(&sendICMP, nSend);

		if(sendto(sockfd, &sendICMP, ICMP_SZ, 0, (struct sockaddr*) &to, sizeof(to)) == -1) {
			perror("sendto");
			continue;
		}

		if((n = recvfrom(sockfd, buf, BUF_SZ, 0, (struct sockaddr*) &from, &from_len)) < 0) {
			perror("recvfrom");
			continue;
		}
		if(unPack(buf, n, inet_ntoa(from.sin_addr)) == -1) {
			printf("unPack Error\n");
			continue;
		}
		nRecieved++;
		sleep(1);

	}

	printf("--- %s ping Statistics ---\n", argv[1]);
	printf("%d packets transmitted, %d recieved, %%%.2f packet loss, time %.3fms\n\n",
			nSend, nRecieved, 100.0 - 100.0*nRecieved/nSend, totalTime);

	return 0;
}

uShort ICMP_CheckSum(uShort* addr, int len) {
	uInt sum = 0;
	while(len > 1) {
		sum += *addr++;
		len -= 2;
	}

	if(len == 1) {
		sum += *(uChar *) addr;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

	return (uShort) ~sum;
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

int unPack(char* buf, int len, char* addr) {
	int ipHeaderLen;
	double rtt;
	Time endT;

	struct ip* ip;
	struct icmp* icmp;

	ip = (struct ip*) buf;
	ipHeaderLen = ip->hlen << 2;

	icmp = (struct icmp*) (buf + ipHeaderLen);
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
	totalTime += rtt;

	printf("%d bytes from %16s(%16s): icmp_seq: %2u, ttl: %d, rtt: %fms\n",
			len, addr, ip_to_host(addr), icmp->sequence, ip->ttl, rtt);

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
