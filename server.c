#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PAIRS 10000 // The maximum number of key-value pairs
#define MAX_STRLEN 256  // The maximum length of key and value strings
/* KVS structure */
typedef struct
{
    char key[MAX_STRLEN];
    char value[MAX_STRLEN];
} KeyValuePair;

KeyValuePair kv_store[MAX_PAIRS];
int kv_count = 0;

// Function to put a key-value pair into the dictionary
void put(const char *key, const char *value)
{
    int i;
    for (i = 0; i < kv_count; i++)
    {
        if (strcmp(kv_store[i].key, key) == 0)
        {
            strcpy(kv_store[i].value, value);
            return;
        }
    }

    if (kv_count < MAX_PAIRS)
    {
        strcpy(kv_store[kv_count].key, key);
        strcpy(kv_store[kv_count].value, value);
        kv_count++;
    }
}

// Function to get the value for a key from the dictionary
const char *get(const char *key)
{
    int i;
    for (i = 0; i < kv_count; i++)
    {
        if (strcmp(kv_store[i].key, key) == 0)
        {
            return kv_store[i].value;
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (argc < 2)
    {
        printf("Input : %s port number\n", argv[0]);
        return 1;
    }

    int SERVER_PORT = atoi(argv[1]);

    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SERVER_PORT);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    const char *server_name = "localhost"; // 127.0.0.1

    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("Could not create socket\n");
        exit(1);
    }

    if ((bind(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr))) < 0)
    {
        printf("Could not bind socket\n");
        exit(1);
    }

    struct sockaddr_in cli_addr;
    int cli_addr_len = sizeof(cli_addr);

    int maxlen = 1024;
    int n = 0;
    char RecvBuffer[maxlen];
    char SendBuffer[maxlen];
    if (SERVER_PORT == 5001)
    {
        while (1)
        {
            n = recvfrom(sock, &RecvBuffer, sizeof(RecvBuffer), MSG_DONTWAIT, (struct sockaddr *)&cli_addr, &cli_addr_len);
            if (n > 0)
            {
                RecvBuffer[n] = '\0'; // Null-terminate the received string
                // Parse input command
                char command[MAX_STRLEN], key[MAX_STRLEN], value[MAX_STRLEN];
                int parsed = sscanf(RecvBuffer, "%s %s %s", command, key, value);

                if (strcmp(command, "get") == 0 && parsed == 2)
                {
                    const char *result = "1";
                    printf("1 returned\n");
                    strcpy(SendBuffer, result);
                    sendto(sock, &SendBuffer, sizeof(SendBuffer), 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));
                }
                else if (strcmp(command, "put") == 0 && parsed == 3)
                {
                    put(key, value);
                }

                else
                {
                    printf("Invalid command\n");
                    strcpy(SendBuffer, "Invalid command");
                    sendto(sock, &SendBuffer, sizeof(SendBuffer), 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));
                }
            }
        }
    }
    else
    {
        while (1)
        {
            n = recvfrom(sock, &RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr *)&cli_addr, &cli_addr_len);
            if (n > 0)
            {
                RecvBuffer[n] = '\0'; // Null-terminate the received string
                // Parse input command
                char command[MAX_STRLEN], key[MAX_STRLEN], value[MAX_STRLEN];
                int parsed = sscanf(RecvBuffer, "%s %s %s", command, key, value);
                if (strcmp(command, "get") == 0 && parsed == 2)
                {
                    int i;
                    i = rand() % 3 + 1;
                    char result[10];
                    sprintf(result, "%d", i);
                    printf("%s returned\n", result);
                    strcpy(SendBuffer, result);
                    sendto(sock, &SendBuffer, sizeof(SendBuffer), 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));
                }
            }
        }
    }

    close(sock);

    return 0;
}
