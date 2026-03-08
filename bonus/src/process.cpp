#include "process.hpp"
#include <stdexcept>
#include <cstring>
#include <sys/wait.h>

Process::Process(const std::string &binary, const std::vector<std::string> &args)
{
    int to_child[2];
    int from_child[2];

    if (pipe(to_child) < 0 || pipe(from_child) < 0)
        throw std::runtime_error("pipe() failed");

    _pid = fork();
    if (_pid < 0)
        throw std::runtime_error("fork() failed");

    if (_pid == 0) {
        dup2(to_child[0], STDIN_FILENO);
        dup2(from_child[1], STDOUT_FILENO);
        close(to_child[1]);
        close(from_child[0]);

        std::vector<char *> argv;
        argv.push_back(const_cast<char *>(binary.c_str()));
        for (const auto &a : args)
            argv.push_back(const_cast<char *>(a.c_str()));
        argv.push_back(nullptr);

        execvp(binary.c_str(), argv.data());
        _exit(1);
    }

    close(to_child[0]);
    close(from_child[1]);
    _stdin_fd  = to_child[1];
    _stdout_fd = from_child[0];

    fcntl(_stdout_fd, F_SETFL, O_NONBLOCK);
}

Process::~Process()
{
    if (_stdin_fd != -1)
        close(_stdin_fd);
    if (_stdout_fd != -1)
        close(_stdout_fd);
    if (_pid > 0)
        waitpid(_pid, nullptr, WNOHANG);
}

void Process::send(const std::string &cmd)
{
    std::string line = cmd + "\n";
    write(_stdin_fd, line.c_str(), line.size());
}

std::string Process::read_available()
{
    std::string result;
    char buf[256];
    ssize_t n;
    while ((n = read(_stdout_fd, buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';
        result += buf;
    }
    return result;
}

bool Process::is_alive() const
{
    if (_pid <= 0) return false;
    return waitpid(_pid, nullptr, WNOHANG) == 0;
}
