#include "dataProcess.hpp"

std::string JRLC::getCmd(std::string command)
{
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        syslog(LOG_ERR,"Error executing command.");
        std::cerr << "Error executing command." << std::endl;
        return "";
    }
    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}
std::string JRLC::read_file_contents(const std::string &file_name)
{
    std::ifstream file(file_name);
    if (!file.is_open()) {
        return "文件不存在";
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        (std::istreambuf_iterator<char>()));
    
    return content;
}

int JRLC::wirte_file_contents(const std::string &write_file_contents, const std::string &file_path)
{
    std::ofstream file(file_path);  // Open the file for writing
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return -1;  // Return an error code
    }

    // Write the content to the file
    file << write_file_contents;

    file.close();  // Close the file
    return 0;  // Return success
}

std::string JRLC::getLastLine(std::string input)
{
    std::istringstream stream(input);
    std::string line;
    std::string lastLine;

    while (std::getline(stream, line)) {
        if (!line.empty()) {
            lastLine = line;
        }
    }

    std::cout << "Last line: " << lastLine << std::endl;
    return lastLine;
}
std::string JRLC::JsonToString(const Json::Value& jsonObject)
{
    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, jsonObject);
    return jsonString;
}

Json::Value JRLC::StringToJson(const std::string& jsonString)
{
    Json::CharReaderBuilder readerBuilder;
    Json::Value root;
    std::istringstream jsonStream(jsonString);
    std::string errors;

    bool parsingSuccessful = Json::parseFromStream(readerBuilder, jsonStream, &root, &errors);
    if (!parsingSuccessful) {
        // Handle parsing error
        // For simplicity, here we're returning an empty JSON object
        return Json::Value();
    }

    return root;
}
void JRLC::replaceSpacesWithUnderscores(Json::Value& root)
{
    if (root.isObject()) {
        Json::Value::Members keys = root.getMemberNames();
        for (const std::string& key : keys) {
            Json::Value& value = root[key];
            if (key.find(' ') != std::string::npos && value.type() == Json::stringValue && !value.isNull() && value.asString() != " Not reported") {
                std::string newKey = key;
                std::replace(newKey.begin(), newKey.end(), ' ', '_');
                root[newKey] = value;
                std::replace(newKey.begin(), newKey.end(), '-', '_');
                root[newKey] = value;
                root.removeMember(key); // 删除原始的键
            }
            replaceSpacesWithUnderscores(value);
        }
    } else if (root.isArray()) {
        for (int i = 0; i < root.size(); ++i) {
            replaceSpacesWithUnderscores(root[i]);
        }
    }
}
void JRLC::replaceSpacesWithUnderscores(std::string& input) {
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == ' ') {
            input[i] = '_';
        }
        if (input[i]=='-')
        {
            input[i]='_';
        }
    }
}
std::vector<std::string> JRLC::split(const std::string& s,std::vector<std::string> &result)
{
  std::stringstream ss(s);
  std::string item;

  while (std::getline(ss, item, ' ')) 
  {
    result.push_back(item);
  }

  return result;
}

void JRLC::getCmd2(const std::string &cmd,std::vector<std::string> &result)
{
  std::array<char, 256> buffer;
  auto pipe = popen(cmd.c_str(), "r");

  if (!pipe) 
  {
    throw std::runtime_error("popen() failed!");
  }

  while (!feof(pipe)) 
  {
    if (fgets(buffer.data(), 256, pipe) != nullptr)
        result.push_back(buffer.data());
  }

  auto rc = pclose(pipe);

  if (rc != EXIT_SUCCESS)
  {
    std::cout<< "Command execution failed\n";
  } 
  return;
}

void JRLC::getLines(const std::string& s,std::vector<std::string> &result)
{
  std::istringstream stream(s);
  std::string line;

  while (std::getline(stream, line)) 
  {
    result.push_back(line);
  }
}

std::string JRLC::removeLineBreak(std::string &str)
{
  str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
  return str;
}

bool JRLC::check_and_create_directory(const std::string& dir_path) {
    size_t pos = 0;
    std::string current_path;
    struct stat info;

    while ((pos = dir_path.find('/', pos)) != std::string::npos) {
        current_path = dir_path.substr(0, pos++);
        if (current_path.empty()) continue;

        // Check if the directory exists
        if (stat(current_path.c_str(), &info) != 0) {
            // Directory does not exist; try to create it
            if (mkdir(current_path.c_str(), 0755) != 0 && errno != EEXIST) {
                return false; // Failed to create directory
            }
        } else if (!(info.st_mode & S_IFDIR)) {
            return false; // Path exists but is not a directory
        }
    }

    // Create the final directory in the path
    if (stat(dir_path.c_str(), &info) != 0) {
        return (mkdir(dir_path.c_str(), 0755) == 0 || errno == EEXIST);
    }

    return (info.st_mode & S_IFDIR);
}

std::string JRLC::getUserPath()
{
  std::string path = "/home/" + std::string(std::getenv("USER"));
  return path;
}

std::string JRLC::getSavePath()
{
  std::string file_path = JRLC::getUserPath() + "/dataacquisition/";
  JRLC::check_and_create_directory(file_path);
  file_path += "profile/";
  return file_path;
}
std::string JRLC::getLocalPath()
{
  std::string file_path = std::string(std::getenv("PWD"));
  return file_path;
}

std::string JRLC::getHomePath()
{
  std::string file_path = std::string(std::getenv("HOME"));
  return file_path;
}
/*获取毫秒时间*/
long long JRLC::getCurrentTimeMillis()
{
  auto now = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
  return duration.count();
}
std::string JRLC::getFormattedTime(long long millis)
{
   auto millis_duration = std::chrono::duration_cast<std::chrono::system_clock::duration>
                           (std::chrono::milliseconds(millis));
  auto time_point = std::chrono::system_clock::time_point(millis_duration);

  auto now_t = std::chrono::system_clock::to_time_t(time_point); // 转为std::time_t

  std::string formattedTime(ctime(&now_t));
  // 去掉末尾的换行符
  if (!formattedTime.empty() && formattedTime.back() == '\n') {
    formattedTime.pop_back();
  }

  return formattedTime;
}
/*获取微妙时间*/
long long JRLC::getCurrentTimeMicro()
{
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
    return duration.count();
}
/*微妙时间格式化*/
std::string JRLC::microsecondsToDateTime(long long microseconds) 
{
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(microseconds));
  auto micros = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::microseconds(microseconds) - seconds);
  
  std::time_t tt = seconds.count();
  std::tm* gm = std::localtime(&tt); 

  std::stringstream ss;
  ss << std::put_time(gm, "%Y-%m-%d %H:%M:%S"); 
  // ss << '.' << std::setfill('0') << std::setw(6) << micros.count();

  return ss.str();
}
/*毫秒时间格式化*/
std::string JRLC::millisecondsToDateTime(long long milliseconds) 
{
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(milliseconds));
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliseconds) - seconds);

  std::time_t tt = seconds.count();
  std::tm* gm = std::localtime(&tt); 

  std::stringstream ss;
  ss << std::put_time(gm, "%Y-%m-%d %H:%M:%S"); 
  ss << '.' << std::setfill('0') << std::setw(3) << millis.count();

  return ss.str();
}

bool JRLC::isExist(const std::string &filename)
{
  struct stat st;
  if (stat(filename.c_str(), &st) < 0)
  {
    std::cerr << "错误: 文件 " << filename << " 不存在或无法读取。" << std::endl;
    return false;
  }
  else
    return true;
}