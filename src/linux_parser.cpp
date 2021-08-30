#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  float value;
  float MemTotal = 0;
  float MemFree = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") MemTotal = value;
        if (key == "MemFree") MemFree = value;;
      }
    }
  }
  return(MemTotal - MemFree)/ MemTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  float value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        return value;  // return the first number
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string value;
  vector<string> values;
  int numberOfValues = 52;
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open()){
  	while(std::getline(stream, line)){
    	std::istringstream linestream(line);
      for (int i = 0; i < numberOfValues; i++)
      {
        linestream >> value;
        values.emplace_back(value);
      }
    }
  }
long utime = std::stoi(values[CPUProcessStates::kutime_]);
long stime = std::stoi(values[CPUProcessStates::kstime_]);
long cutime = std::stoi(values[CPUProcessStates::kcutime_]);
long cstime = std::stoi(values[CPUProcessStates::kcstime_]);
return utime + stime + cutime + cstime;
}
// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line;
  string key;
  long user, nice, system, idle, iowait, irq, softirq, steal;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
  	while(std::getline(stream, line)){
    	std::istringstream linestream(line);
    	while(linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal)
      {
        if (key == "cpu") {
          return (user + nice + system + irq + softirq + steal)/sysconf(_SC_CLK_TCK);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{
  string line;
  string key;
  long user, nice, system, idle, iowait;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
  	while(std::getline(stream, line)){
    	std::istringstream linestream(line);
    	while(linestream >> key >> user >> nice >> system >> idle >> iowait)
      {
        if (key == "cpu") {
          return (idle + iowait)/sysconf(_SC_CLK_TCK);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return CPU utilization
//I have changed return type from vector<string> to float
float LinuxParser::CpuUtilization() {
  return float(ActiveJiffies())/float(Jiffies());
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") return value;
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") return value;
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
     return line;
    }
  }
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        value=value*0.001;
        if (key == "VmSize") return std::to_string(value);
      }
    }
  }
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") return std::to_string(value);
      }
    }
  }
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string user;
  string uid = Uid(pid);
  string x;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> value) {
        if (value == uid) return user;
      }
    }
  }
  return "Error";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    string line;
    vector<string> values;
    string value;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        for (int i = 0 ; i < 22; i++){
        linestream >> value;
        values.emplace_back(value);
        }
      }
    }
  return std::stoi(values[21])/sysconf(_SC_CLK_TCK);
}