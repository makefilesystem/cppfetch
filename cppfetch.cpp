#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <map>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>  
#include <array>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <cstdlib>
#include "config.use.h"

namespace cppfetch {

    struct Config {
        std::string ascii_logo_path = ASCII_LOGO_PATH;
        std::string color_primary = COLOR_PRIMARY;
        std::string color_secondary = COLOR_SECONDARY;
        std::string color_reset = COLOR_RESET;
        std::map<std::string, std::function<std::string()>> fetch_functions;
        std::map<std::string, std::string> labels;
        bool position_side = true; 
        bool position_below = false;
        bool enable_logo = true;

        void loadConfig();
    };

    Config config;

    std::string getMemoryUsage() {
#ifdef ENABLE_MEMORY
        std::ifstream file("/proc/meminfo");
        if (!file.is_open()) {
            return "";
        }
        std::string line;
        long total = 0, free = 0;
        while (std::getline(file, line)) {
            if (line.find("MemTotal:") == 0) {
                total = std::stol(line.substr(line.find(":") + 1)) / 1024;
            } else if (line.find("MemAvailable:") == 0) {
                free = std::stol(line.substr(line.find(":") + 1)) / 1024;
            }
            if (total && free) break;
        }
        std::ostringstream result;
        result << (total - free) << "M / " << total << "M";
        return result.str();
#else
        return "";
#endif
    }

    std::string getCPUInfo() {
#ifdef ENABLE_CPU
        std::ifstream cpuinfo("/proc/cpuinfo");
        std::string line;
        int cores = 0;
        std::string cpu_model;
        while (std::getline(cpuinfo, line)) {
            if (line.find("model name") != std::string::npos) {
                cpu_model = line.substr(line.find(":") + 2);
            } else if (line.find("cpu cores") != std::string::npos) {
                cores = std::stoi(line.substr(line.find(":") + 2));
            }
        }
        std::ostringstream result;
        result << cpu_model << " (" << cores << " cores)";
        return result.str();
#else
        return "";
#endif
    }

    std::string getOSName() {
#ifdef ENABLE_OS
        std::ifstream os_release("/etc/os-release");
        std::string line;
        while (std::getline(os_release, line)) {
            if (line.find("PRETTY_NAME") != std::string::npos) {
                return line.substr(line.find("=") + 2, line.length() - line.find("=") - 3);
            }
        }
        return "";
#else
        return "";
#endif
    }

    std::string getKernelVersion() {
#ifdef ENABLE_KERNEL
        struct utsname buffer;
        if (uname(&buffer) == 0) {
            return buffer.release;
        }
        return "";
#else
        return "";
#endif
    }

std::string getPackageCount() {
#ifdef ENABLE_PACKAGES
        std::string command;
        #if defined(__linux__)
            if (system("which dpkg > /dev/null 2>&1") == 0) {
                command = "dpkg-query -f '.\\n' -W | wc -l";
            } else if (system("which rpm > /dev/null 2>&1") == 0) {
                command = "rpm -qa | wc -l";
            } else if (system("which pacman > /dev/null 2>&1") == 0) {
                command = "pacman -Q | wc -l";
            } else if (system("which emerge > /dev/null 2>&1") == 0) {
                command = "qlist -I | wc -l";
            } else {
                return "";
            }
        #elif defined(__APPLE__)
            if (system("which brew > /dev/null 2>&1") == 0) {
                command = "brew list --formula | wc -l";
            } else {
                return "";
            }
        #elif defined(_WIN32) || defined(_WIN64)
            if (system("where choco > NUL 2>&1") == 0) {
                command = "choco list --local-only | find /c \"\"";
            } else if (system("where winget > NUL 2>&1") == 0) {
                command = "winget list | find /c \"\"";
            } else {
                return "";
            }
        #else
            return "";
        #endif

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            return "";
        }

        std::array<char, 128> buffer;
        std::ostringstream result;
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result << buffer.data();
        }
        fclose(pipe);

        return result.str();
#else
        return "";
#endif
    }

    std::string getHostname() {
#ifdef ENABLE_HOSTNAME
        char hostname[1024];
        if (gethostname(hostname, sizeof(hostname)) == 0) {
            return std::string(hostname);
        }
        return "";
#else
        return "";
#endif
    }

    std::string getUptime() {
#ifdef ENABLE_UPTIME
        struct sysinfo info;
        if (sysinfo(&info) == 0) {
            long uptime = info.uptime;
            long hours = uptime / 3600;
            long minutes = (uptime % 3600) / 60;
            return std::to_string(hours) + "h " + std::to_string(minutes) + "m";
        }
        return "";
#else
        return "";
#endif
    }

    std::string getUsername() {
#ifdef ENABLE_USERNAME
        const char *username = getenv("USER");
        return username ? username : "";
#else
        return "";
#endif
    }

    std::string getIP() {
#ifdef ENABLE_IP
        char buffer[128];
        FILE *fp = popen("ip route get 1.2.3.4 | awk '{print $7}'", "r");
        if (fp == nullptr) return "";
        fgets(buffer, sizeof(buffer) - 1, fp);
        fclose(fp);
        return std::string(buffer);
#else
        return "";
#endif
    }

    std::string getDiskUsage() {
#ifdef ENABLE_DISK
        struct statvfs fs;
        if (statvfs("/", &fs) == 0) {
            unsigned long total = fs.f_blocks * fs.f_frsize / 1024 / 1024;
            unsigned long free = fs.f_bfree * fs.f_frsize / 1024 / 1024;
            unsigned long used = total - free;
            return std::to_string(used) + "M / " + std::to_string(total) + "M";
        }
        return "";
#else
        return "";
#endif
    }

    void loadConfig() {
        config.fetch_functions = {
            {"memory", getMemoryUsage},
            {"cpu", getCPUInfo},
            {"os", getOSName},
            {"kernel", getKernelVersion},
            {"packages", getPackageCount},
            {"hostname", getHostname},
            {"uptime", getUptime},
            {"username", getUsername},
            {"ip", getIP},
            {"disk", getDiskUsage}
        };

        config.labels = {
            {"memory", LABEL_MEMORY},
            {"cpu", LABEL_CPU},
            {"os", LABEL_OS},
            {"kernel", LABEL_KERNEL},
            {"packages", LABEL_PACKAGES},
            {"hostname", LABEL_HOSTNAME},
            {"uptime", LABEL_UPTIME},
            {"username", LABEL_USERNAME},
            {"ip", LABEL_IP},
            {"disk", LABEL_DISK}
        };
    }

    void printData(const std::string& key, const std::string& data) {
        if (!data.empty()) {
            std::cout << config.color_secondary << key << ": " << config.color_reset << data << std::endl;
        }
    }

    void runFetch() {
        loadConfig();
        
        if (config.enable_logo) {
            std::ifstream logo_file(config.ascii_logo_path);
            std::string line;
            while (std::getline(logo_file, line)) {
                std::cout << config.color_primary << line << config.color_reset << "\n";
            }
        }

        
        if (config.position_side) {
            
            for (const auto& entry : config.fetch_functions) {
                printData(config.labels[entry.first], entry.second());
            }
        } else {
            
            for (const auto& entry : config.fetch_functions) {
                printData(config.labels[entry.first], entry.second());
            }
        }
    }
}

int main() {
    cppfetch::runFetch();
    return 0;
}

