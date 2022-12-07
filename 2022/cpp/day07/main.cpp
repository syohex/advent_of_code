#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <cstring>
#include <map>

struct BaseFile {
    virtual ~BaseFile() = default;

    virtual size_t Size() const = 0;
};

struct File : public BaseFile {
    explicit File(size_t size) : size_(size) {
    }

    size_t Size() const override {
        return size_;
    }

    size_t size_;
};

struct Directory : public BaseFile {
    ~Directory() {
        for (auto it : files_) {
            delete it.second;
        }
    }

    size_t Size() const override {
        size_t ret = 0;
        for (const auto &it : files_) {
            ret += it.second->Size();
        }
        return ret;
    }

    void AddFile(const std::string &name, BaseFile *file) {
        files_[name] = file;
    }

    std::map<std::string, BaseFile *> files_;
};

template <typename T>
std::map<std::vector<std::string>, Directory *> ParseInput(T &it) {
    std::vector<std::string> dirs;
    std::string line;
    bool read_line = true;

    std::map<std::vector<std::string>, Directory *> file_system;
    file_system[{"/"}] = new Directory();

    Directory *current_dir = nullptr;
    while (true) {
        if (read_line) {
            if (!std::getline(it, line)) {
                break;
            }
            if (line.empty()) {
                break;
            }
        }

        read_line = true;
        if (line.find("$ cd") == 0) {
            std::string dir = line.substr(5);
            if (dir == "/") {
                dirs = {"/"};
            } else if (dir == "..") {
                dirs.pop_back();
            } else {
                dirs.push_back(dir);
            }

            current_dir = file_system.at(dirs);
        } else if (line.find("$ ls") == 0) {
            while (std::getline(it, line)) {
                if (line[0] == '$') {
                    read_line = false;
                    break;
                }

                if (line[0] == 'd') {
                    std::string subdir = line.substr(4);
                    Directory *dir = new Directory;
                    dirs.push_back(subdir);
                    file_system[dirs] = dir;
                    dirs.pop_back();

                    current_dir->AddFile(subdir, dir);
                } else {
                    size_t size;
                    char name[256];
                    sscanf(line.c_str(), "%zd %s", &size, name);
                    current_dir->AddFile(name, new File(size));
                }
            }
        } else {
            // never reach here
            assert(!"unexpected input");
        }
    }

    return file_system;
}

size_t Problem1(const std::map<std::vector<std::string>, Directory *> data) {
    size_t ret = 0;
    for (const auto &it : data) {
        size_t size = it.second->Size();
        if (size < 100000) {
            ret += size;
        }
    }

    return ret;
}

void Test() {
    std::string input(R"($ cd /
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k)");

    std::stringstream ss(input);
    auto data = ParseInput(ss);
    size_t ret1 = Problem1(data);
    assert(ret1 == 95437);

    delete data[{"/"}];
}

int main() {
    Test();

    auto data = ParseInput(std::cin);

    int ret1 = Problem1(data);

    std::cout << "Problem1: " << ret1 << std::endl;

    delete data[{"/"}];
    return 0;
}
