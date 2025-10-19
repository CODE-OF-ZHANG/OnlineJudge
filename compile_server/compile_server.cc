#include "compile_run.hpp"
#include "../comm/httplib.h"
using namespace ns_compile_and_run;
using namespace httplib;

void Usage(std::string proc)
{
    std::cerr << "Usage: " << "\n\t" << proc << " port" << std::endl;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }
    Server svr;
    svr.Get("/hello", [](const Request& req, Response& resp){
        resp.set_content("hello httplib", "content-type: text/plain;charset=utf-8");
    });
    svr.listen("0.0.0.0", std::atoi(argv[1]));

    svr.Post("/compile_and_run", [](const Request& req, Response& resp){
        std::string in_json = req.body;
        std::string out_json;
        if(!in_json.empty())
        {
            CompileAndRun::Start(in_json, &out_json);
            resp.set_content(out_json, "application/json;charset=utf-8");
        }
    });


    // // in_json: {"code": "include<...>...", "input": "...", }
    // // out_json: {"status":"0", "reason":"", "stdout":"", "stderr":"",}
    // std::string in_json;
    // Json::Value in_value;
    // in_value["code"] = R"(#include<iostream>
    // int main()
    // {
    //     std::cout << "你可以看见我了" << std::endl;
    //     // while(1);
    //     // int* p = new int[1024 * 1024 * 50];
    //     // int a = 10;
    //     // a /= 0;

    //     sdvsdv
    //     return 0;
    // })";
    // in_value["input"] = "";
    // in_value["cpu_limit"] = 1;
    // in_value["mem_limit"] = 10240 * 3;

    // Json::FastWriter writer;
    // in_json = writer.write(in_value);

    // std::cout << in_json << std::endl;
    // std::string out_json;
    // CompileAndRun::Start(in_json, &out_json);
    // std::cout << out_json << std::endl;
    return 0;
}