#include "fcgi_stdio.h" /* fcgi library; put it first*/

#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <utmp.h>
#include <errno.h>
#include <syslog.h>

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

    char up[1024];
    char users[1024];
    char load[1024];
    char error[1024];

    /* Response loop. */
    while (FCGI_Accept() >= 0)   {
        printf("\n");

        struct sysinfo info;
        if (sysinfo(&info) == -1) {
            sprintf(error, "sysinfo(2) call failed: (%d) %s", errno, strerror(errno));
            syslog(LOG_ERR, error);
            printf("Error.");
            continue;
        }

        long minutes = info.uptime / 60;
        long hours = minutes / 60;
        if (hours >= 48)
            sprintf(up, "up: %ld days, %ld:%02ld,", hours / 24, hours % 24, minutes % 60);
        else if (hours >= 24)
            sprintf(up, "up: 1 day, %ld:%02ld,", hours % 24, minutes % 60);
        else
            sprintf(up, "up: %ld:%02ld,", hours % 24, minutes % 60);

        int numuser = usercount();
        if (numuser == 1) 
            sprintf(users, "%3d user, ", numuser);
        else
            sprintf(users, "%3d users,", numuser);

        double div = (float)(1 << SI_LOAD_SHIFT);
        sprintf(load, "load average: %.2f, %.2f, %.2f", info.loads[0] / div, info.loads[1] / div, info.loads[2] / div);

        printf(" %-21s  %10s %s\n", up, users, load);
    }
}

