#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../comm/util.hpp"
#include "../comm/log.hpp"

namespace ns_compiler
{
    // 引入路径拼接功能
    using namespace ns_util;
    using namespace ns_log;

    class Compiler
    {
    public:
        Compiler()
        {}

        // 返回值: 编译成功: true   否则: false
        // 输入的参数: 编译的文件名
        // file_name: test
        // test  -> ./temp/test.cpp
        // test  -> ./temp/test.exe
        // test -> ./temp/test.stderr
        static bool Compile(const std::string &file_name)
        {
            pid_t pid = fork();
            // 创建子进程失败
            if (pid < 0)
            {
                LOG(ERROR) << "内部错误, 创建子进程失败" << "\n";
                return false;
            }
            else if (pid == 0)
            {
                umask(0);
                int _stderr = open(PathUtil::CompilerError(file_name).c_str(), O_CREAT | O_WRONLY, 0644);
                if (_stderr < 0)
                {
                    LOG(WARNING) << "没有成功形成 stderr 文件" << "\n";
                    exit(1);
                }
                // 重定向标准错误到 _stderr
                dup2(_stderr, 2);

                // child: 调用编译器, 完成对代码的编译工作
                // g++ -o target src -std=c++11
                execlp("g++", "g++", "-o", PathUtil::Exe(file_name).c_str(),
                       PathUtil::Src(file_name).c_str(), "-std=c++11", "-D", "COMPILER_ONLINE", nullptr);
                LOG(ERROR) << "启动编译器g++启动失败, 可能是参数错误" << "\n";
                exit(2);
            }
            else
            {
                waitpid(pid, nullptr, 0);
                // 判断文件是否编译成功
                if(FileUtil::IsFileExists(PathUtil::Exe(file_name)))
                {
                    LOG(INFO) << PathUtil::Src(file_name) << " 编译成功!" << "\n";
                    return true;
                }
            }
            LOG(ERROR) << "编译失败, 没有形成可执行程序" << "\n";
            return false;
        }

        ~Compiler() 
        {}
    };
}
