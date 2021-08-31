#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include <linux_parser.h>

using std::string;
using std::to_string;
using std::vector;

//Constructor
Process::Process(int pid) : pid_(pid){
    CpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid() {
    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    long total_time = LinuxParser::ActiveJiffies(pid_);
    long seconds = LinuxParser::UpTime() - LinuxParser::UpTime(pid_);
    cpuUtilization_ = float(total_time) /  float(sysconf(_SC_CLK_TCK)) / float(seconds);
    return cpuUtilization_;
}

// TODO: Return the command that generated this process
string Process::Command() {
    command_ = LinuxParser::Command(pid_);
    unsigned int maxSize = 50;
    if (command_.length() > maxSize ) command_.replace(command_.begin() + maxSize, command_.end(), "...");
    return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime() - LinuxParser::UpTime(pid_);
    }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return cpuUtilization_ > a.cpuUtilization_;
}