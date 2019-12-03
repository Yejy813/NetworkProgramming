#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char* message);

int main(int argc, char* argv[])
{
    int sock;
    char opmsg[BUF_SIZE];
    int result, opnd_cnt, i;
    struct sockaddr_in serv_addr;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        error_handling("scok() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect() error");
    }
    else
    {
        puts("connected.........");
    }
    
    fputs("Operand Counts: ", stdout);
    scanf("%d", &opnd_cnt); 

    opmsg[0] = (char)opnd_cnt; // 第一个字符为计算个数
    for(i = 0; i < opnd_cnt; ++i)
    {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int*)&opmsg[i*OPSZ + 1]); // 将每个计算单位加入
    }

    fgetc(stdin);
    fputs("Operator: ", stdout);
	scanf("%c", &opmsg[opnd_cnt*OPSZ+1]); // 最后加入 加减乘除 符号
    write(sock, opmsg, opnd_cnt*OPSZ + 2);
    read(sock, &result, RLT_SIZE);

	printf("Operation result:%d \n",result);
	close(sock);

    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}