#pragma once
#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include "include/mysql.h"
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
        std::string header; // 题目预设代码
        std::string tail;   // 测试用例, 拼接header形成完整代码
        int cpu_limit;      // 时间限制
        int mem_limit;      // 空间限制
    };

    const std::string oj_questions = "oj_questions";
    const std::string host = "127.0.0.1";
    const std::string user = "oj_client";
    const std::string passwd = "123456";
    const std::string db = "oj";
    const int port = 3306;

    class Model
    {
    public:
        Model()
        {
        }

        bool QueryMySql(const std::string &sql, std::vector<Question> *out)
        {
            // 连接数据库
            MYSQL *my = mysql_init(nullptr);
            if (nullptr == mysql_real_connect(my, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0))
            {
                LOG(FATAL) << "数据库连接失败! "
                           << "\n";
                return false;
            }
            LOG(INFO) << "数据库连接成功! "
                      << "\n";

            // 设置编码格式
            mysql_set_character_set(my, "utf8");

            // 执行sql语句
            if (0 != mysql_query(my, sql.c_str()))
            {
                LOG(WARNING) << sql << " execute error! "
                             << "\n";
                return false;
            }

            // 提取结果
            MYSQL_RES *res = mysql_store_result(my);

            // 分析结果 --> 获得行列数
            int rows = mysql_num_rows(res);
            int cols = mysql_num_fields(res);
            struct Question q;

            for (int i = 0; i < rows; i++)
            {
                MYSQL_ROW row = mysql_fetch_row(res);
                q.number = row[0];
                q.title = row[1];
                q.star = row[2];
                q.desc = row[3];
                q.header = row[4];
                q.tail = row[5];
                q.cpu_limit = std::atoi(row[6]);
                q.mem_limit = std::atoi(row[7]);

                out->push_back(q);
            }

            // 释放空间
            free(res);

            // 关闭数据库连接
            mysql_close(my);
            return true;
        }

        // 获取所有题目
        bool GetAllQuestions(std::vector<Question> *out)
        {
            std::string sql = "select * from ";
            sql += oj_questions;
            return QueryMySql(sql, out);
        }

        // 获取一个题目
        bool GetOneQuestion(const std::string &number, Question *q)
        {
            bool res = false;
            std::string sql = "select * from ";
            sql += oj_questions;
            sql += " where number=";
            sql += number;
            std::vector<Question> result;
            if (QueryMySql(sql, &result))
            {
                if (result.size() == 1)
                {
                    *q = result[0];
                    res = true;
                }
            }
            return res;
        }

        ~Model() {}
    };
}