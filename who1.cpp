//
// Created by ChanChill on 2020-06-30.
//

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

//#define SHOWHOST

void showtime(long);
void show_info(struct utmp *);


int main()
{
    struct utmp utbuf;
    int         utmpfd;

    if( (utmpfd == open(UTMP_FILE, O_RDONLY))==-1){
        perror(UTMP_FILE);
        exit(1);
    }
    while( read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))  //参数 fd buf 读取字节数 返回值 numread  错误返回-1
        show_info(&utbuf);
    close(utmpfd);
    return 0;
}


void show_info(struct utmp *utbufp)
{
    if( utbufp->ut_type!=USER_PROCESS) return ;
    print("%-8.8s", utbufp->ut_name);
    print(" ");
    print("%-8.8s", utbufp->ut_line);
    print(" ");
    showtime(utbufp->ut_time);
#ifdef  SHOWHOST
    if(utbufp->ut_host[0]!='\0')
        printf("(%s)", utbufp->ut_host);
#endif
    printf("\n");
}

void showtime(long timeval)
{
    char *cp
    cp = ctime(&timeval);
    printf("%12.12s", cp+4);
}

