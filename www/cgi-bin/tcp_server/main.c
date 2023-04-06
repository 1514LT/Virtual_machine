#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include "sqlite3.h"
#include <time.h>
int sockfd = 0;
typedef struct
{
    int device_id; //设备ID
    int cli_fd;    //服务器与设备ID通信的已连接套接字
} DEVICE;
DEVICE array[128];
char *get_now_time(char *time_now);
int insert_card_from_user(char *card);
// echo服务
void *deal_fun(void *arg)
{
    int cli_fd = (int)arg;
    while (1)
    {
        unsigned char buf[256] = "";
        //上线信息："1:设备号"
        int len = recv(cli_fd, buf, sizeof(buf), 0);
        printf("buf=%s\n", buf);
        if (len == 0) //说明对方关闭了连接
        {
            close(cli_fd);
            break;
        }

        if (buf[0] == '1') //上线信息
        {
            int device_id = 0;
            sscanf(buf, "1:%d", &device_id);
            printf("%d号设备 通信文件描述符%d上线\n", device_id, cli_fd);
            array[device_id].device_id = device_id;
            array[device_id].cli_fd = cli_fd;
            continue;
        }
        else if (strncmp(buf, "get_card", 8) == 0) // cgi需要获取card
        {
            int device_id = 0;
            sscanf(buf, "get_card:%d", &device_id);
            send(array[device_id].cli_fd, "get_card:1", 10, 0);
            printf("cgi希望和%d号文件描述通信\n", array[device_id].cli_fd);
        }
        else if (buf[0] == '2') // A53回复card
        {
            int device_id = 0;
            char card[32] = "";
            //"2:0:card"
            sscanf(buf, "2:%d:%s", &device_id, card);
            send(array[device_id].cli_fd, card, strlen(card), 0);
            printf("A53希望和%d号文件描述通信\n", array[device_id].cli_fd);
        }
        else if (buf[0] == '3') //普通打卡
        {
            char card[32] = "";
            sscanf(buf, "3:%s", card);

            //从user表中查找有无此卡
            int ret = insert_card_from_user(card); // ret==0 未找到  ret==1找到
            if (ret == 1)
            {
                send(cli_fd, "3:1", 3, 0);
            }
            else
            {
                send(cli_fd, "3:0", 3, 0);
            }
        }
    }
    printf("有客户端退出了\n");
}
//回收子进程资源
void exit_func(int sig)
{
    //关闭监听套接字
    close(sockfd);
    _exit(-1);
}
int main(int argc, char const *argv[])
{
    //./a.out 8000
    if (argc != 2)
    {
        printf("./a.out 8000\n");
        return 0;
    }

    // 1、创建TCP套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return 0;
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2、bind绑定固定端口和IP
    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(argv[1]));
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if (ret != 0)
    {
        perror("bind");
        return 0;
    }

    // 3、监听
    ret = listen(sockfd, 10);
    if (ret != 0)
    {
        perror("listen");
        return 0;
    }

    //注册 SIGINT 处理函数（目的 回收子进程资源 避免僵尸进程出现）
    signal(SIGINT, exit_func);

    while (1)
    {
        // 4、从连接队列中提取客户端 得到与客户端通信的 已连接套接字
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);

        int cli_fd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
        if (cli_fd < 0)
            continue;

        //查看客户端的信息
        unsigned short port = ntohs(cli_addr.sin_port);
        char ip_str[16] = "";
        inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, ip_str, 16);
        printf("%s:%hu的连接已到来.......\n", ip_str, port);

        //来一个客户端创建一个线程
        pthread_t tid;
        pthread_create(&tid, NULL, deal_fun, (void *)cli_fd);
        pthread_detach(tid);
    }

    //关闭监听套接字
    close(sockfd);
    return 0;

    return 0;
}
int insert_card_from_user(char *card)
{
    // 1、查找有无次卡
    //定义数据库句柄
    sqlite3 *db = NULL;
    int ret = sqlite3_open("../door.db", &db);
    if (ret != SQLITE_OK)
    {
        perror("sqlite3_open");
        return 0;
    }

    char sql[128] = "";
    sprintf(sql, "select * from user where card=\'%s\';", card);
    char **result = NULL;
    int row = 0, col = 0;
    sqlite3_get_table(db, sql, &result, &row, &col, NULL);
    if (row >= 1)
    {
        //获取当前时间
        char time_buf[64] = "";
        get_now_time(time_buf);

        //插入考勤数据库
        char sql[128] = "";
        sprintf(sql, "insert into data values(\'%s\', \'%s\');", card, time_buf);
        sqlite3_exec(db, sql, NULL, NULL, NULL);

        //关闭数据库
        sqlite3_close(db);
        return 1;
    }
    //关闭数据库
    sqlite3_close(db);
    return 0;
}
//获取时间
char *get_now_time(char *time_now)
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep);
    sprintf(time_now, "%d-%d-%d %d:%d:%d\n", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday,
            (p->tm_hour), p->tm_min, p->tm_sec);
    return time_now;
}