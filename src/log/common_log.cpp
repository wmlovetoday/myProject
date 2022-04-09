#include "common_log.h"

namespace pts {
class ComLog *com_log = ComLog::GetLogInstance();
zlog_category_t *com_log_handle = com_log->GetHandler();
}  // namespace pts
// #include "common_log.h"

// namespace wm
// {
// class Pzlog *Pzlog = Pzlog::GetLogInstance();
// zlog_category_t *com_log_handle = Pzlog->GetHandle();
// }  // namespace wm

/*
[global]
strict init = true
buffer min = 1024
buffer max = 2MB

# 1k => 1000 bytes
# 1kb => 1024 bytes
# 1m => 1000000 bytes
# 1mb => 1024*1024 bytes
# 1g => 1000000000 bytes
# 1gb => 1024*1024*1024 byte

[formats]
file_func_ms= "%d(%T) %ms %m [%U:%L]%n"
file_func_us = "%d(%T) %us %m [%U:%L]%n"
file_line_ms= "%d(%T) %ms %m [%f:%L]%n"
file_line_us = "%d(%T) %us %m [%f:%L]%n"

[rules]
# *.*   >stdout;normal
sys_log.=INFO   "./log/sys_log.log",100M * 100;file_line_us
sys_log.=ERROR  "./log/sys_err.log",100M * 100;file_line_us
usr_log.*       "./log/usr_log.log",100M * 100;file_line_us

DEBUG”, “INFO”, “NOTICE”, “WARN”, “ERROR”和”FATAL”
表达式	含义
*	所有等级
aa.debug	代码内等级>=debug
aa.=debug	代码内等级==debug
aa.!debug	代码内等级!=debug
typedef enum {
    ZLOG_LEVEL_DEBUG = 20,
    ZLOG_LEVEL_INFO = 40,
    ZLOG_LEVEL_NOTICE = 60,
    ZLOG_LEVEL_WARN = 80,
    ZLOG_LEVEL_ERROR = 100,
    ZLOG_LEVEL_FATAL = 120
} zlog_level;

zlog_fatal(cat, format, ...)
zlog_error(cat, format, ...)
zlog_warn(cat, format, ...)
zlog_notice(cat, format, ...)
zlog_info(cat, format, ...)
zlog_debug(cat, format, ...)
ZL_BUF(handle, cc, sizeof(cc));
*/
