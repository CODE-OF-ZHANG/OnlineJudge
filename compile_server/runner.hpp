#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "../comm/log.hpp"
#include "../comm/util.hpp"

namespace ns_runner
{
    using namespace ns_log;
    using namespace ns_util;
    class Runner
    {
    public:
        Runner() {}
        ~Runner() {}

    public:
        // 提供设置该进程占用资源大小的接口
        static void SetProcLimit(int _cpu_limit, int _mem_limit)
        {
            // 设置cpu运行时长
            struct rlimit cpu_rlimit;
            cpu_rlimit.rlim_cur = _cpu_limit;
            cpu_rlimit.rlim_max = RLIM_INFINITY;

            setrlimit(RLIMIT_CPU, &cpu_rlimit);

            // 设置内存大小
            struct rlimit mem_rlimit;
            mem_rlimit.rlim_cur = _mem_limit * 1024;  // 转化成KB
            mem_rlimit.rlim_max = RLIM_INFINITY;

            setrlimit(RLIMIT_AS, &mem_rlimit);
        }

        /**********************************
         * 返回值 > 0: 程序异常, 收到了信号, 返回值就是信号的编号
         * 返回值 = 0: 正常运行完毕, 结果保存到对应的临时文件之中
         * 返回值 < 0: 内部错误  如: 创建子进程失败
         * 
         * cpu_limit: 该程序运行时, 可以使用的最大cpu资源的上限
         * mem_limit: 该程序运行时, 可以使用的最大的内存的大小(KB)
         ***********************************/
        static int Run(const std::string &file_name, int cpu_limit, int mem_limit)
        {
            /*************************************
             * 程序运行:
             * 1. 代码跑完, 结果正确
             * 2. 代码跑完, 结果不正确
             * 3. 代码没跑完, 出现异常
             * Run 不需要考虑代码是否跑完, 结果是否正确
             * 结果正确与否, 由测试用例决定！
             * 只考虑是否正确运行完毕
             *
             * 1. 需要知道可执行程序是谁
             * 2. 一个程序在默认启动的时候
             * 标准输入: 不做处理
             * 标准输出: 程序运行完成, 输出的结果是什么
             * 标准错误: 运行时错误信息
             **************************************/

            std::string _execute = PathUtil::Exe(file_name);
            std::string _stdin = PathUtil::Stdin(file_name);
            std::string _stdout = PathUtil::Stdout(file_name);
            std::string _stderr = PathUtil::Stderr(file_name);

            umask(0);
            int _stdin_fd = open(_stdin.c_str(), O_CREAT | O_RDONLY, 0644);
            int _stdout_fd = open(_stdout.c_str(), O_CREAT | O_WRONLY, 0644);
            int _stderr_fd = open(_stderr.c_str(), O_CREAT | O_WRONLY, 0644);

            if (_stdin_fd < 0 || _stdout_fd < 0 || _stderr_fd < 0)
            {
                LOG(ERROR) << "运行时打开标准文件失败" << "\n";
                return -1;
            }

            pid_t pid = fork();
            if (pid < 0)
            {
                // 创建子进程失败
                close(_stdin_fd);
                close(_stdout_fd);
                close(_stderr_fd);

                LOG(ERROR) << "运行时创建子进程失败" << "\n";
                return -2;
            }
            else if (pid == 0)
            {
                // child
                dup2(_stdin_fd, 0);
                dup2(_stdout_fd, 1);
                dup2(_stderr_fd, 2);

                SetProcLimit(cpu_limit, mem_limit);

                execl(_execute.c_str(), _execute.c_str(), nullptr);
                exit(1);
            }
            else
            {
                // parent
                close(_stdin_fd);
                close(_stdout_fd);
                close(_stderr_fd);

                int status = 0;
                waitpid(pid, &status, 0);

                LOG(INFO) << "运行完毕, info: " << (status & 0x7F) << "\n";
                return status & 0x7F;
            }
        }
    };
}