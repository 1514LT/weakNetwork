#ifndef __DATAPROCESS_HPP
#define __DATAPROCESS_HPP
#include <iostream>
#include <limits>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include "json/json.h"
#include <regex>
#include <vector>
#include <map>
#include <unistd.h>
#include <regex>
#include <thread>
#include <chrono>
#include <syslog.h>
#include <string>
#include <ctime>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
namespace JRLC
{
    std::string read_file_contents(const std::string &file_name);
    int wirte_file_contents(const std::string &write_file_contents, const std::string &file_path);
    std::string getLastLine(std::string input);
    std::string JsonToString(const Json::Value& jsonObject);
    Json::Value StringToJson(const std::string& jsonString);
    void replaceSpacesWithUnderscores(Json::Value& root);
    void replaceSpacesWithUnderscores(std::string& input);
    std::string getCmd(std::string command);
    void getCmd2(const std::string &cmd,std::vector<std::string> &result);
    std::vector<std::string> split(const std::string& s,std::vector<std::string> &result);
    void getLines(const std::string& s,std::vector<std::string> &result);
    std::string removeLineBreak(std::string &str);
    bool check_and_create_directory(const std::string& dir_path);
    std::string getUserPath();
    std::string getSavePath();
    std::string getLocalPath();
    std::string getHomePath();
    long long getCurrentTimeMillis(); // 获取当前毫秒时间
    std::string getFormattedTime(long long millis);
    long long getCurrentTimeMicro(); // 获取当前微妙秒时间
    std::string microsecondsToDateTime(long long microseconds);
    std::string millisecondsToDateTime(long long milliseconds);
    bool isExist(const std::string &filename);
      
} // namespace JRLC


#endif