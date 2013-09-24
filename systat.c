#include "fcgi_stdio.h" /* fcgi library; put it first*/

#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <utmp.h>

#define LINELEN 256

void initialize(void)
{
}
int usercount() {
    int numuser = 0;
    struct utmp *utmpstruct;
    setutent();
    while ((utmpstruct = getutent())) {
        if ((utmpstruct->ut_type == USER_PROCESS) &&
            (utmpstruct->ut_name[0] != '\0'))
            numuser++;
    }
    endutent();

    return numuser;
}

int main(void)
{
    /* Initialization. */  
    initialize();

    /* Response loop. */
    while (FCGI_Accept() >= 0)   {
        printf("\n");

        char up[1024];
        char users[1024];
        char load[1024];

        struct sysinfo info;
        sysinfo(&info);

        long minutes = info.uptime / 60;
        long hours = minutes / 60;
        if ( hours >= 48 )
            sprintf(up, "up: %ld days, %ld:%02ld,", hours / 24, hours % 24, minutes % 60);
        else if ( hours >= 24 )
            sprintf(up, "up: 1 day, %ld:%02ld,", hours % 24, minutes % 60);
        else
            sprintf(up, "up: %ld:%02ld,", hours % 24, minutes % 60);

        int numuser = usercount();
        if (numuser == 1) 
            sprintf(users, "%d user, ", numuser);
        else
            sprintf(users, "%d users,", numuser);

        double div = (float)(1 << SI_LOAD_SHIFT);
        sprintf(load, "load average: %.2f, %.2f, %.2f", info.loads[0] / div, info.loads[1] / div, info.loads[2] / div);

        printf(" %-21s  %9s  %s\n", up, users, load);
    }
}

