#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    string HH = std::to_string(seconds/3600);
    seconds = seconds%3600;
    string MM = std::to_string(seconds/60);
    seconds = seconds%60;
    string SS = std::to_string(seconds);
    HH.insert(0,2 - HH.length(), '0');
    MM.insert(0,2 - MM.length(), '0');
    SS.insert(0,2 - SS.length(), '0');
    return HH + ":" + MM + ":" + SS;
}