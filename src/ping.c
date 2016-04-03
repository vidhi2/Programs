#include "ping.h"

char buf[BUF_SZ] = {0};

int main(int argc, char** argv) {

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

	in_addr_t inaddr;
	struct hostent* host;
	struct icmp sendICMP;
	int nSend = 0;
	int nRecieved = 0;
	int n = 0;
	int len_sockaddr = sizeof(struct sockaddr_in);
	int from_len = 0;

	struct sockaddr_in from;
	memset(&from, 0, len_sockaddr);

	struct sockaddr_in to;
	memset(&to, 0, len_sockaddr);
	to.sin_family = AF_INET;
	if((inaddr = inet_addr(argv[1])) == INADDR_NONE) {
		if((host = gethostbyname(argv[1])) == NULL) {
			printf("Error: Host Not Found");
			exit(1);
		}
		to.sin_addr = *(struct in_addr*) host->h_addr_list[0];
	} else {
		to.sin_addr.s_addr = inaddr;
	}

	printf("ping %s(%s) : %d bytes of Data.\n",
		 argv[1], inet_ntoa(to.sin_addr), (int) ICMP_SZ);

	for(int i = 0; i < NUM; ++i) {
		nSend++;
		memset(&sendICMP, 0, ICMP_SZ);
		pack(&sendICMP, nSend);

		if(sendto(sockfd, &sendICMP, ICMP_SZ, 0, (struct sockaddr*)&to, len_sockaddr) == -1) {
			perror("sendto");
			continue;
		}

		if((n = recvfrom(sockfd, buf, BUF_SZ, 0, (struct sockaddr*)&from, &from_len)) < 0) {
			perror("recvfrom");
			continue;
		}
		nRecieved++;
		if(unPack(buf, n, inet_ntoa(from.sin_addr)) == -1) {
			printf("unPack Error\n");
		}
		sleep(1);
	}

	printf("--- %s ping Stats ---\n", argv[1]);
	printf("%d packets transmitted, %d recieved, %%%d packet loss\n\n",
			nSend, nRecieved, (nSend - nRecieved)/nSend * 100);
	return 0;
}