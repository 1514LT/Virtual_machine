#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "sqlite3.h"

int sockfd = 0;
int main(int argc, char const *argv[])
{
    
    printf("content-type:textml\n\n");
    char *data = getenv("QUERY_STRING");
    char usr[64]="";
    char text[128] = "";
    //获取收到的数据(url中？后面的数据)
    sscanf(data, "%[^:]:%s", usr, text);
    //printf("%s\n",usr);
#if 1
    //获取数据库中的数据
    sqlite3 *db = NULL;
    int ret = sqlite3_open("../door.db", &db);
    if (ret != SQLITE_OK)
    {
        perror("sqlite3_open");
        return 0;
    }

    //查询lucy的信息
    char sql[128] = "";
    sprintf(sql, "select user,user.card,time from user,data where user.card=data.card and user.user=\'%s\';", usr);
    //sprintf(sql, "select * from user where user=\'%s\';", usr);
    // sqlite3_exec(db, sql, my_fun, NULL, NULL);
    char **result = NULL;
    int row = 0, col = 0;
    sqlite3_get_table(db, sql, &result, &row, &col, NULL);
    int i = 0, j = 0;
    char buf[1024] = "<table border='2'>";
    for (i = 0; i < row + 1; i++)
    {
        strcat(buf, "<tr>");
        for (j = 0; j < col; j++)
        {
            strcat(buf, "<td>");
            strcat(buf, result[i * col + j]); //存放的就是查找结果
            strcat(buf, "</td>");
        }
        strcat(buf, "</tr>");
    }
    strcat(buf, "</table>");

    printf("%s", buf);

    //关闭数据库
    sqlite3_close(db);
#endif
    close(sockfd);
    return 0;
}
