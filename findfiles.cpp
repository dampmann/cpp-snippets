#include <dirent.h>
#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <thread>

std::mutex output_mutex;
std::mutex mu;
std::vector<std::string> dirs;

template<typename Target = std::string, typename Source = std::string>
Target to(Source arg) {
    std::stringstream ip;
    Target result;

    if(!(ip << arg) || !(ip >> result) || !(ip >> std::ws).eof())
        throw std::runtime_error{"to<>() failed"};
    return(result);
}

void worker_thread(const std::string pattern) {
    const std::regex pat { pattern };
    const auto dc = [](DIR *d) { closedir(d); };
    std::string diritem;
    while(true) {
        {
            std::lock_guard<std::mutex> lck(mu);
            if(dirs.empty()) {
                return;
            } else {
                diritem = dirs.back();
                dirs.pop_back();
            }
        }

        const auto dp = std::unique_ptr<DIR, decltype(dc)>(opendir(
                    diritem.c_str()), dc);
        if(dp != nullptr) {
            struct dirent *de = nullptr;

            while((de = readdir(dp.get())) != nullptr) {
                const std::string entry_name { de->d_name };
                if(de->d_type != DT_DIR) {
                    std::smatch match;
                    const std::string se { de->d_name };
                    if(std::regex_search(se, match, pat)) {
                        std::lock_guard<std::mutex> lg(output_mutex);
                        std::cout << diritem + "/" + se << "\n";
                    }
                }
            }
        } else {
            std::lock_guard<std::mutex> lg(output_mutex);
            std::cerr << "Can't open " << diritem << "\n";
        }
    }
}

void traverse_directory(std::string dir) {
    const auto dc = [](DIR *d) { closedir(d); };
    const auto dp = std::unique_ptr<DIR, decltype(dc)>(opendir(
                                                    dir.c_str()), dc);
    if(dp != nullptr) {
        struct dirent *de = nullptr;

        while((de = readdir(dp.get())) != nullptr) {
            const std::string entry_name { de->d_name };
            if(de->d_type == DT_DIR && 
                    ((entry_name != ".") && (entry_name != ".."))) {
                std::string dir_name;
                if(dir.back() == '/') {
                    dir_name = dir + entry_name;
                } else {
                    dir_name = dir + "/" + entry_name;
                }

                dirs.push_back(dir_name);
                traverse_directory(dir_name);
            }
        }
    }
}

int main(int argc, char **argv) {
    if(argc != 4) {
        std::cerr << "Usage: " << 
            argv[0] << " <dir> <name_pattern> <#threads>\n";
        return(EXIT_FAILURE);
    }

	int number_of_threads = to<int>(argv[3]);
    std::vector<std::thread> workers;
    std::string base_dir { argv[1] };
    std::string pattern { argv[2] };
    dirs.push_back(base_dir);
    traverse_directory(base_dir);
    for(int i = 0; i < number_of_threads; ++i) {
        workers.emplace_back(worker_thread, pattern);
    }

    for(auto &t : workers) {
        t.join();
    }

    return(EXIT_SUCCESS);
}

