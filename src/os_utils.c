#include "os_utils.h"

#include <string.h>
#include <errno.h>

#ifdef __linux__
#include <unistd.h>
#include <time.h>
#include <libgen.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#else
#include <windows.h>
#include <direct.h>
#include <io.h>
#endif

extern void os_set_error(const char * fmt, ...);

bool os_utils_work_path(char * path, size_t len)
{
    return true;
}

bool os_utils_create_directory(const char * path)
{
    if (NULL == path)
    {
        os_set_error("Invalid param");
        return false;
    }

    if (os_utils_file_exist(path))
        return true;

    size_t len = strlen(path);
#ifdef __linux__
    char * tmp_path = strdup(path);
#else
    char * tmp_path = _strdup(path);
#endif
    if (NULL == tmp_path)
    {
        os_set_error("strdup %zu bytes failed", len + 1);
        return false;
    }

    for (size_t i = 1; i < len + 1; i++)
    {
        if (tmp_path[i] != OS_UTILS_DISK_SEP)
            continue;

        tmp_path[i] = '\0';
        if (!os_utils_file_exist(tmp_path))
        {
#ifdef __linux__
            if (0 != mkdir(tmp_path, S_IRWXU | S_IRWXG | S_IRWXO))
#else
            if (0 != _mkdir(tmp_path))
#endif
            {
                const int code = errno;
                if (errno == EEXIST)
                {
                    tmp_path[i] = OS_UTILS_DISK_SEP;
                    continue;
                }
                os_set_error("%s", strerror(code));
                free(tmp_path);
                return false;
            }
        }
        tmp_path[i] = OS_UTILS_DISK_SEP;
    }
    free(tmp_path);
    return true;
}

bool os_utils_file_exist(const char * path)
{
#ifdef __linux__
    return (0 == access(path, F_OK));
#else
    return (0 == _access(path, 0));
#endif
}

bool os_utils_file_name(const char * path, char * file, const size_t len)
{
    if (NULL == path || NULL == file)
    {

        return false;
    }

#ifdef __linux__
    char * file_name = basename((char *)path);
    strncpy(file, file_name, len);
#else
    char tmp[OS_UTILS_FILE_MAX] = { 0 };
    char ext[OS_UTILS_FILE_MAX] = { 0 };
    _splitpath(path, NULL, NULL, tmp, ext);
    snprintf(file, len, "%s%s", tmp, ext);
#endif

    return true;
}

long os_utils_tid()
{
#ifdef __linux__
    return syscall(SYS_gettid);
#else
    return (long)GetCurrentThreadId();
#endif
}

os_time_t * os_utils_time(os_time_t * tv)
{
    if (NULL == tv)
        return NULL;

#ifdef __linux__
    struct timeval ts;
    gettimeofday(&ts, NULL);
    struct tm * sys_tv = localtime(&ts.tv_sec);
    tv->year = sys_tv->tm_year + 1900;
    tv->month = sys_tv->tm_mon + 1;
    tv->day = sys_tv->tm_mday;
    tv->hour = sys_tv->tm_hour;
    tv->minute = sys_tv->tm_min;
    tv->second = sys_tv->tm_sec;
    tv->millisecond = (int)ts.tv_usec / 1000;
#else
    SYSTEMTIME sys_tv = { 0 };
    GetLocalTime(&sys_tv);
    tv->year = sys_tv.wYear;
    tv->month = sys_tv.wMonth;
    tv->day = sys_tv.wDay;
    tv->hour = sys_tv.wHour;
    tv->minute = sys_tv.wMinute;
    tv->second = sys_tv.wSecond;
    tv->millisecond = sys_tv.wMilliseconds;
#endif
    return tv;
}
