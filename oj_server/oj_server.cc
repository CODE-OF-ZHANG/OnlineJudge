#include <iostream>
#include "../comm/httplib.h"
#include "oj_model.hpp"
#include "oj_control.hpp"
using namespace httplib;
using namespace ns_model;
using namespace ns_control;

int main()
{
    // 用户请求的服务路由功能
    Server svr;
    Control ctrl;

    // 获取所有题目的列表
    svr.Get("/all_questions", [&ctrl](const Request& req, Response& resp){
        std::string html;
        ctrl.AllQuestions(&html);
        resp.set_content(html, "text/html;charset=utf-8");
    });

    // 根据题目编码, 获取题目内容
    svr.Get(R"(/question/(\d+))", [&ctrl](const Request& req, Response& resp){
        std::string number = req.matches[1];
        std::string html;
        ctrl.Question(number, &html);
        resp.set_content(html, "text/html;charset=utf-8");
    });

    // 用户提交代码, 判题功能
    svr.Post(R"(/judge/(\d+))", [&ctrl](const Request &req, Response &resp){
        std::string number = req.matches[1];
        std::string result_json;
        ctrl.Judge(number, req.body, &result_json);
        resp.set_content(result_json, "application/json;charset=utf-8");
        // resp.set_content("指定题目的判题: " + number, "text/plain; charset=utf-8");
    });

    svr.set_base_dir("./wwwroot");
    // 监听
    svr.listen("0.0.0.0", 8080);
    return 0;
}