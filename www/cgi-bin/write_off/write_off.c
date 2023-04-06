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
int main(int argc, char const *argv[])
{
    printf("content-type:textml\n\n");
    char *data = getenv("QUERY_STRING");
    sqlite3 *db = NULL;
    int ret = sqlite3_open("../door.db", &db);
    if (ret != SQLITE_OK)
    {
        perror("sqlite3_open");
        return 0;
    }
    char sql[128] = "";
    sprintf(sql, "select * from user where user =\'%s\';", data);
    char **result = NULL;
    int row = 0, col = 0;
    sqlite3_get_table(db, sql, &result, &row, &col, NULL);
    if (row >= 1)
    {
        sprintf(sql, "delete from user where user=\'%s\';", data);
        sqlite3_exec(db, sql, NULL, NULL, NULL);
        printf("1");
    }
    else
    {
        printf("0");
    }
    sqlite3_close(db);
    return 0;
}
