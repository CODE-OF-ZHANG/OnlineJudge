#pragma once
#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstdlib>

namespace ns_model
{
    using namespace ns_log;
    using namespace ns_util;
    struct Question
    {
        std::string number; // 题目编号
        std::string title;  // 题目标题
        std::string desc;   // 题目描述
        std::string star;   // 题目难度 --> 简单 中等  困难
        int cpu_limit;      // 时间限制
        int mem_limit;      // 空间限制
        std::string header; // 题目预设代码
        std::string tail;   // 测试用例, 拼接header形成完整代码
    };

    const std::string questions_list = "./questions/questions.list";
    const std::string questions_path = "./questions/";

    class Model
    {
    private:
        // 题号 : 题目细节
        std::unordered_map<std::string, Question> questions;

    public:
        Model()
        {
            assert(LoadQuestionList(questions_list));
        }

        // 加载配置文件 questions/questions.list + 题目编号文件
        bool LoadQuestionList(const std::string &question_list)
        {
            std::ifstream in(question_list);
            if (!in.is_open())
            {
                LOG(FATAL) << " 加载题库失败, 请检查题库文件是否存在" << "\n";
                return false;
            }
            std::string line;
            while (std::getline(in, line))
            {
                std::vector<std::string> tokens;
                StringUtil::SplitString(line, &tokens, " ");
                if (tokens.size() != 5)
                {
                    LOG(WARNING) << "加载部分题目失败, 请检查文件格式" << "\n";
                    continue;
                }
                Question q;
                // 1 判断回文数 简单 1 30000
                q.number = tokens[0];
                q.title = tokens[1];
                q.star = tokens[2];
                q.cpu_limit = std::atoi(tokens[3].c_str());
                q.mem_limit = std::atoi(tokens[4].c_str());

                std::string path = questions_path;
                path += q.number;
                path += "/";

                FileUtil::ReadFile(path + "desc.txt", &(q.desc), true);
                FileUtil::ReadFile(path + "header.cpp", &(q.header), true);
                FileUtil::ReadFile(path + "tail.cpp", &(q.tail), true);

                questions.insert({q.number, q});
            }
            LOG(INFO) << "加载题库...成功" << "\n";
            in.close();

            return true;
        }

        // 获取所有题目
        bool GetAllQuestions(std::vector<Question> *out)
        {
            if (questions.size() == 0)
            {
                LOG(ERROR) << "用户获取所有题目失败" << "\n";
                return false;
            }
            for (const auto &q : questions)
            {
                out->push_back(q.second);
            }
            return true;
        }

        // 获取一个题目
        bool GetOneQuestion(const std::string &number, Question *q)
        {
            const auto &iter = questions.find(number);
            // 没有
            if (iter == questions.end())
            {
                LOG(ERROR) << "用户获取题目失败, 题目编号: " << number << "\n";
                return false;
            }
            // 找到了
            (*q) = iter->second;
            return true;
        }

        ~Model() {}
    };
}