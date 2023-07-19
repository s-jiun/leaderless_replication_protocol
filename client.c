#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_FOLLOWER 3
#define MAX_STRLEN 256

int main(int argc, char *argv[])
{
	if (argc < 1)
	{
		return 1;
	}
	/* localhost에서 통신할 것이므로 서버 ip주소도 그냥 localhost */
	const char *server_name = "localhost";		  // 127.0.0.1
	struct sockaddr_in server_addr[NUM_FOLLOWER]; // Create socket structure

	int server_ports[NUM_FOLLOWER] = {5001, 5002, 5003};
	for (int i = 0; i < NUM_FOLLOWER; i++)
	{
		memset(&server_addr[i], 0, sizeof(server_addr[i]));
		server_addr[i].sin_family = AF_INET;
		server_addr[i].sin_port = htons(server_ports[i]);
		inet_pton(AF_INET, server_name, &server_addr[i].sin_addr);
	}

	int sock_to_server[NUM_FOLLOWER];
	for (int i = 0; i < NUM_FOLLOWER; i++)
	{
		if ((sock_to_server[i] = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		{
			printf("Could not create socket\n");
			exit(1);
		}
	}

	int maxlen = 1024;
	char RecvBuffer1[maxlen];
	char RecvBuffer2[maxlen];
	char RecvBuffer3[maxlen];
	char SendBuffer[maxlen];

	struct sockaddr_in cli_addr;
	int cli_addr_len = sizeof(cli_addr);
	struct sockaddr_in cli_addr2;
	int cli_addr_len2 = sizeof(cli_addr2);
	struct sockaddr_in cli_addr3;
	int cli_addr_len3 = sizeof(cli_addr3);

	int rsp_server1 = 0;
	int rsp_server2 = 0;
	int rsp_server3 = 0;
	int cnt = 0;

	while (1)
	{
		if (fgets(SendBuffer, maxlen, stdin) != NULL)
		{
			for (int i = 0; i < NUM_FOLLOWER; i++)
				sendto(sock_to_server[i], &SendBuffer, sizeof(SendBuffer), 0, (struct sockaddr *)&server_addr[i], sizeof(server_addr[i]));
		}

		int n1 = recvfrom(sock_to_server[0], RecvBuffer1, sizeof(RecvBuffer1) - 1, 0, (struct sockaddr *)&cli_addr, &cli_addr_len);
		int n2 = recvfrom(sock_to_server[1], &RecvBuffer2, sizeof(RecvBuffer2), 0, (struct sockaddr *)&cli_addr2, &cli_addr_len2);
		int n3 = recvfrom(sock_to_server[2], &RecvBuffer3, sizeof(RecvBuffer3), 0, (struct sockaddr *)&cli_addr3, &cli_addr_len3);

		if (n1 > 0)
		{
			RecvBuffer1[n1] = '\0';
			printf("%s\n", RecvBuffer1);
			rsp_server1 = atoi(RecvBuffer1);
			cnt++;
		}
		if (n2 > 0)
		{
			RecvBuffer2[n2] = '\0';
			printf("%s\n", RecvBuffer2);
			rsp_server2 = atoi(RecvBuffer2);
			cnt++;
		}
		if (n3 > 0)
		{
			RecvBuffer3[n3] = '\0';
			printf("%s\n", RecvBuffer3);
			rsp_server3 = atoi(RecvBuffer3);
			cnt++;
		}
		if (cnt >= 3)
		{
			if (rsp_server1 == rsp_server2 || rsp_server1 == rsp_server3 || rsp_server2 == rsp_server3)
			{
				rsp_server2 = 0;
				rsp_server3 = 0;
				printf("TRUE\n");
			}
			else
			{
				rsp_server2 = 0;
				rsp_server3 = 0;
				printf("FALSE\n");
			}
			cnt = 0;
		}
		else
		{
			printf("Packet lost\n");
		}
	}

	close(sock_to_server[0]);
	close(sock_to_server[1]);
	close(sock_to_server[2]);
	return 0;
}
