#include <iostream>
#include <string>
#include <future>
#include <functional>
#include <thread>
#include <experimental/filesystem>

// g++ -std=c++17 -Wall -o fs_01 fs_01.cpp -lpthread -lstdc++fs

std::uintmax_t du(const char *path) {
  namespace fs = std::experimental::filesystem;
  fs::path p { path };
  if(!fs::is_directory(p)) {
    std::cerr << path << " is not a directory.\n";
    return(0);
  }

  std::uintmax_t sz = 0;
  for(auto& entry: fs::recursive_directory_iterator(p)) {
    if(fs::is_regular_file(entry)) {
      std::error_code ec;
      sz += fs::file_size(entry, ec);
      if(ec) {
        std::cerr << "Error " << ec.message() << "\n";
      }
    }
  }

  return(sz);
}

int main(int argc, char **argv) {
  if(argc < 2) {
    std::cerr << "Usage " << argv[0] << " <dir>\n";
    return(127);
  }

  using task_type = std::uintmax_t(const char *);
  std::packaged_task<task_type> pt {du};
  std::future<std::uintmax_t> fut { pt.get_future() } ;
  std::thread t1 { std::move(pt), argv[1] };
  std::cout << "The size of " << argv[1] << " in bytes is " << fut.get() << "\n";
  t1.join();
  return(0);
}

