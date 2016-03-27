#include "traceroute.h"

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("Error: <IP> not Specified\n");
		exit(1);
	}

	struct icmp icmpPacket;
	int rplyFrom;
	int stop = 0;
	int retStat = 0;

	char destIP[20];
	char *hostname = argv[1];
	retStat = host_to_ip(hostname, destIP);
	if(retStat <= ERROR) {
        printf("Error: Unable to resolve Hostname\n");
        exit(-1);
    }

	Time sendTime[3];
	Time recvTime[3];
	Time currTime;
	char *ipAddr[3];

	int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(sock < ERROR) {
		perror("socket");
		exit(-1);
	}

	currTime.tv_sec = 0;
	currTime.tv_usec = 1000;

	retStat = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &currTime, sizeof(currTime));
	if(retStat < ERROR) {
		perror("setsockopt");
		exit(-1);
	}

	struct sockaddr_in destAddr;
	bzero(&destAddr, sizeof(destAddr));
	destAddr.sin_family = AF_INET;
	inet_pton(AF_INET, destIP, &destAddr.sin_addr);

	char buffer[BUF_SZ];
	int id = getpid();
	int recvCode;

	printf("*****************************************************************\n");
	printf("traceroute to %s (%s), %d hops max\n", 
			ip_to_host(destIP), destIP, TTL_MAX);
	for(int ttl = 1; ttl <= TTL_MAX; ++ttl) {
		rplyFrom = 0;

		icmpPacket.icmp_type = ICMP_ECHO;
		icmpPacket.icmp_id = id;
		icmpPacket.icmp_code = 0;
		icmpPacket.icmp_seq = ttl;
		icmpPacket.icmp_cksum = 0;
		icmpPacket.icmp_cksum = ICMP_CheckSum((u_int16_t*) &icmpPacket, 8);
	
		for(int seq = 0; seq < 3; ++seq) {
			retStat = setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
			if(retStat < ERROR) {
				perror("setsockopt");
				exit(-1);
			}

			retStat = sendto(sock, &icmpPacket, ICMP_LEN, 0, (struct sockaddr*) &destAddr, sizeof(destAddr));
			if(retStat < ERROR) {
				perror("sendto");
				exit(-1);
			}

			gettimeofday(&sendTime[seq], NULL);
		}

		while(rplyFrom < 3) {
			recvCode = recvfrom(sock, buffer, BUF_SZ, 0, 0, 0);
			gettimeofday(&currTime, NULL);
			if(recvCode < 0) {
				if(cmpTime(sendTime[rplyFrom], currTime) > TIMEOUT) {
					break;
				}
				continue;
			}

			struct ip* buffIP = (struct ip*) buffer;
			if(buffIP->ip_p != IPPROTO_ICMP) {
				continue;
			}

			struct icmp *buffICMP = (struct icmp*) (buffer + (buffIP->ip_hl * 4));
			if(buffICMP->icmp_type != ICMP_ECHOREPLY &&
				!(buffICMP->icmp_type == ICMP_TIME_EXCEEDED &&
				buffICMP->icmp_code == ICMP_EXC_TTL)) {

				continue;
			}

			if(buffICMP->icmp_type == ICMP_TIME_EXCEEDED) {
				buffICMP = (struct icmp *) (buffICMP->icmp_data + 
							((struct ip*) (buffICMP->icmp_data))->ip_hl*4);
			}

			if(buffICMP->icmp_id != id) {
				continue;
			}

			ipAddr[rplyFrom] = inet_ntoa(buffIP->ip_src);
			gettimeofday(&recvTime[rplyFrom], NULL);
			rplyFrom++;

			if(buffICMP->icmp_type == ICMP_ECHOREPLY) {
				stop = 1;
			}
		}
		outputPath(ttl, rplyFrom, ipAddr, sendTime, recvTime);

		if(stop == 1) {
			break;
		}
	}

	printf("*****************************************************************\n");
	return 0;
}