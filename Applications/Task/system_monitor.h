#ifndef __SYSTEMMONITOR_H__
#define __SYSTEMMONITOR_H__

#define DECLARE_MONITOR(name)  \
    unsigned short name##_cnt; \
    unsigned short name##_fps;

typedef struct
{
    DECLARE_MONITOR(Led_Ctrl)
    DECLARE_MONITOR(Tim3)
    DECLARE_MONITOR(Tim4)
    DECLARE_MONITOR(Tim5)

    unsigned int TaskTotalTime;
    unsigned int TaskTotalTimeMost;
} SYSTEM_MONITOR;
extern SYSTEM_MONITOR system_monitor;
extern unsigned DelayCNT;

#define Calc_System_Monitor_fps(xx)                        \
    do                                                     \
    {                                                      \
        system_monitor.xx##_fps = system_monitor.xx##_cnt; \
        system_monitor.xx##_cnt = 0;                       \
    } while (0)

#define Calc_All_fps()                      \
    do                                      \
    {                                       \
        Calc_System_Monitor_fps(Led_Ctrl);  \
        Calc_System_Monitor_fps(Tim3);      \
        Calc_System_Monitor_fps(Tim4);      \
        Calc_System_Monitor_fps(Tim5);      \
    } while (0)

void SystemErrorDetect(void);

#endif
