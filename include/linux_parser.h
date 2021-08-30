#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
// CPU Process
enum CPUProcessStates {
  kpid_= 0,
  kcomm_,
  kstate_,
  kppid_,
  kpgrp_,
  ksession_,
  ktty_nr_,
  ktpgid_,
  kflags_,
  kminflt_,
  kcminflt_,
  kmajflt_,
  kcmajflt_,
  kutime_,
  kstime_,
  kcutime_,
  kcstime_,
  kpriority_,
  knice_,
  knum_threads_,  
  kitrealvalue_,
  kstarttime_,
  kvsize_,
  krss_,
  krsslim_,  
  kstartcode_,    
  kendcode_,
  kstartstack_,    
  kkstkesp_,
  kkstkeip_,  
  ksignal_,
  kblocked_, 
  ksigignore_,  
  ksigcatch_,
  kwchan_,
  knswap_,
  kcnswap_, 
  kexit_signal_,
  kprocessor_,
  krt_priority_,
  kpolicy_,
  kdelayacct_blkio_ticks_,
  kguest_tim_,
  kcguest_tim_,
  kstart_data_,
  kend_data_,
  kstart_brk_, 
  karg_start_,
  karg_end_,
  kenv_start_,  
  kenv_end_,
  kexit_code_, 
};
float CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif