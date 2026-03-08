#pragma once

#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>

class Process {
public:
    Process(const std::string &binary, const std::vector<std::string> &args);
    ~Process();

    void send(const std::string &cmd);
    std::string read_available();

    bool is_alive() const;

private:
    pid_t _pid = -1;
    int _stdin_fd = -1;
    int _stdout_fd = -1;
};
