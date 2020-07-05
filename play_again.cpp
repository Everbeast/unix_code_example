//
// Created by ChanChill on 2020-07-05.
//

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define ASK      "Do you want another transaction"
#define TRIES     3
#define SLEEPTIME 2
#define BEEP      putchar('\a')

int main()
{
    int response;
    void ctrl_c_handler(int);

    tty_mode(0);    //save current mode
    set_cr_noecho_mode();  //单字符  无回显
    set_nodelay_mode();  //noinput =>eof
    signal(SIGINT, ctrl_c_handler);
    signal(SIGQUIT, SIG_IGN);
    response = get_response(ASK, TRIES);
    tty_mode(1);  //reset orign mode
    return response;
}

int get_response(char *question, int maxtries)
{
    int input;
    printf("%s(y/n)?", question);
    fflush(stdout);
    while(1){
        sleep(SLEEPTIME);
        input = tolower(get_ok_char());
        if(input == 'y') return 0;
        if(input == 'n') return 1;
        if(maxtries-- == 0) return 2;  //outatime
        BEEP;
    }
}

char get_ok_char()
{
    int c;
    while((c=getchar())!=EOF && strchr("yYnN", c)==NULL);
    return c;
}

void set_cr_noecho_mode()
{
    struct termios ttystate;

    tcgetattr(0, &ttystate);   //read current tty setting
    ttystate.c_lflag &= ~ICANON;  // set no buffering
    ttystate.c_lflag &=~ECHO;     //no echo either
    ttystate.c_cc[VMIN] = 1;   //1 char at a time
    tcsetattr(0, TCSANOW, &ttystate);  //install setting
}

void set_nodelay_mode()
{
    int termflags;
    termflags = fcntl(0, F_GETEL);
    termflags |=O_NDELAY;
    fcntl(0, F_SETFL, termflags);
}

void tty_mode(int how)
{
    static struct termios original_mode;
    static int original_flags;
    static int stored = 0;

    if(how == 0){
        tcgetattr(0, &original_mode);
        original_flags = fcntl(0, F_GETEL);
        stored = 1;
    }
    else if(stored){
        tcsetattr(0, TCSANOW, &original_mode);
        fcntl(0, F_SETEL, original_flags);
    }
}

void ctrl_c_handler(int signum){
    tty_mode(1);
    exit(1)
}