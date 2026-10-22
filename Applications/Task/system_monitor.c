#include "system_monitor.h"
SYSTEM_MONITOR system_monitor = {0}; // 系统监视器

void SystemErrorDetect(void)
{
	Calc_All_fps(); // 统计各个任务的帧率
}
