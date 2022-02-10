#include "sys_resourse.h"
namespace sys_res {
SysRes
}

// static int32_t ReadCmd(const char *cmd, std::string &ack, uint32_t try_count)
// {
//   uint32_t tmp = try_count;
//   char recv[1024];
//   if (cmd == nullptr) return -1;
//   ack.clear();
//   FILE *pp = popen(cmd, "r");
//   if (pp == nullptr) {
//     // PRINT_ERRNO("popen failed");
//     return -2;
//   }
//   while (tmp) {
//     char *s = fgets(recv, sizeof(recv), pp);
//     if (s == nullptr) {
//       //   PRINT_ERRNO("%d fgets failed", tmp);
//       pclose(pp);
//       return -1;
//     }
//     ack += recv;
//     break;
//     --tmp;
//     // }
//   }
//   pclose(pp);
//   if (tmp) {
//     return 0;
//   }
//   return -4;
// }

// struct StatData {
//   void parse(const std::string &content) {
//     size_t rp = content.rfind(')');
//     std::istringstream iss(content.data() + rp + 1);

//     //            0    1    2    3     4    5       6   7 8 9  11  13   15
//     // 3770 (cat) R 3718 3770 3718 34818 3770 4202496 214 0 0 0 0 0 0 0 20
//     // 16  18     19      20 21                   22      23      24 25
//     //  0 1 0 298215 5750784 81 18446744073709551615 4194304 4242836
//     //  140736345340592
//     //              26
//     // 140736066274232 140575670169216 0 0 0 0 0 0 0 17 0 0 0 0 0 0

//     iss >> state;
//     iss >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags;
//     iss >> minflt >> cminflt >> majflt >> cmajflt;
//     iss >> utime >> stime >> cutime >> cstime;
//     iss >> priority >> nice >> num_threads >> itrealvalue >> starttime;
//   }
//   std::string name;
//   char state;
//   int ppid;
//   int pgrp;
//   int session;
//   int tty_nr;
//   int tpgid;
//   int flags;

//   long minflt;
//   long cminflt;
//   long majflt;
//   long cmajflt;

//   long utime;
//   long stime;
//   long cutime;
//   long cstime;

//   long priority;
//   long nice;
//   long num_threads;
//   long itrealvalue;
//   long starttime;
// };

// static bool running = true;
// inline void stop_handler(int) {
//   running = false;
//   std::cout << "preparing to shut down..." << std::endl;
// }

// inline void setup_signal_handlers() {
//   signal(SIGINT, stop_handler);
//   signal(SIGTERM, stop_handler);
// }

// int32_t main(int32_t argc, char **argv) {
//   if (argc != 3) {
//     printf("ERROR: must input process id, interval secone\n");
//     return 0;
//   }
//   int clockTicks = static_cast<int>(::sysconf(_SC_CLK_TCK));
//   int process_id = atoi(argv[1]);
//   int interval = atoi(argv[2]);
//   std::string page_cmd{"getconf PAGESIZE"};
//   std::string page_ack{};
//   int32_t ret = ReadCmd(page_cmd.data(), page_ack, 2);
//   int32_t pagesize = std::stoi(page_ack.data());
//   while (running) {
//     std::string cmd{"cat /proc/" + std::to_string(process_id) + "/stat"};
//     std::string ack{};
//     setup_signal_handlers();

//     int32_t ret = ReadCmd(cmd.data(), ack, 2);
//     StatData sta{};
//     sta.parse(ack);

//     long old_cutime = sta.utime;
//     long old_cstime = sta.stime;
//     timespec old_tv{};
//     ret = clock_gettime(CLOCK_REALTIME, &old_tv);
//     int64_t old_ms = old_tv.tv_sec * 1000 + old_tv.tv_nsec / 1000000;

//     std::this_thread::sleep_for(std::chrono::seconds(interval));

//     ack.clear();
//     ReadCmd(cmd.data(), ack, 2);
//     sta.parse(ack);

//     long new_cutime = sta.utime;
//     long new_cstime = sta.stime;

//     timespec new_tv{};
//     clock_gettime(CLOCK_REALTIME, &new_tv);

//     int64_t process_ticks = new_cutime + new_cstime - old_cutime -
//     old_cstime; int64_t new_ms = new_tv.tv_sec * 1000 + new_tv.tv_nsec /
//     1000000; float period_sec = (new_ms - old_ms) / 1000.0;
//     // printf("old %ld new %ld  period_sec %.2f\n", old_ms, new_ms,
//     period_sec); float cpu_usage = process_ticks / (period_sec * clockTicks);
//     float usr_usage = (new_cutime - old_cutime) / (period_sec * clockTicks);
//     float sys_usage = (new_cstime - old_cstime) / (period_sec * clockTicks);
//     printf("cpu_usage total: %.2lf; usr: %.2lf; sys: %.2lf;\n", cpu_usage *
//     100, usr_usage * 100, sys_usage * 100);

//     std::string mem_cmd{"cat /proc/" + std::to_string(process_id) +
//     "/statm"}; std::string mem_ack{}; ret = ReadCmd(mem_cmd.data(), mem_ack,
//     2); if (ret == 0) {
//       if (!ack.empty()) {
//         char *p = strtok(const_cast<char *>(mem_ack.data()), " ");
//         p = strtok(nullptr, " ");
//         int32_t mem = std::stoi(p);
//         mem = (mem * pagesize) / 1024;
//         if (mem > 0) {
//           float mm = mem / 1024.0;
//           printf("process mem %.2f MB\n", mm);
//         } else {
//           printf("process mem %d KB\n", mem);
//         }
//       }
//     }
//   }
//   return 0;
// }
