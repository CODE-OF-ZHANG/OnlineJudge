#include <iostream>
#include <vector>
#include <string>
#include <ctemplate/template.h>
// #include <boost/algorithm/string.hpp>
// #include <cstdlib>
// #include <unistd.h>
// #include <signal.h>
// #include <sys/time.h>
// #include <sys/resource.h>
// #include <jsoncpp/json/json.h>

int main()
{
    std::string in_html = "./test.html";
    std::string value = "hello world";

    // 形成字典数据
    ctemplate::TemplateDictionary root("test");
    root.SetValue("key", value);
    ctemplate::Template *tpl = ctemplate::Template::GetTemplate(in_html, ctemplate::DO_NOT_STRIP);
    std::string out_html;
    tpl->Expand(&out_html, &root);
    std::cout << out_html << std::endl;
    return 0;
}


// // 测试json
// int main()
// {
//     std::vector<std::string> tokens;
//     const std::string str = "1 判断回文数 简单 1 30000";
//     const std::string sep = " ";
//     boost::split(tokens, str, boost::is_any_of(sep), boost::algorithm::token_compress_on);
//     for(auto &iter : tokens)
//     {
//         std::cout << iter << std::endl;
//     }

//     // Json::Value root;
//     // root["code"] = "mycode";
//     // root["name"] = "zhangxu";
//     // root["age"] = "21";

//     // Json::StyledWriter writer1;
//     // std::string str1 = writer1.write(root);

//     // Json::FastWriter writer2;
//     // std::string str2 = writer2.write(root);

//     // std::cout << str1 << std::endl;
//     // std::cout << str2 << std::endl;
//     return 0;
// }



// void hander(int signo)
// {
//     std::cout << " signo : " << signo << std::endl;
//     exit(1);
// }

// int main()
// {
//     for(int i = 1; i <= 31; i++)
//     {
//         signal(i, hander);
//     }

//     // 限制运行时长
//     // struct rlimit r;
//     // r.rlim_cur = 1;
//     // r.rlim_max = RLIM_INFINITY;
//     // setrlimit(RLIMIT_CPU, &r);
//     // while(1);

//     // 超出内存
//     struct rlimit r;
//     r.rlim_cur = 1024 * 1024 * 40;
//     r.rlim_max = RLIM_INFINITY;
//     setrlimit(RLIMIT_AS, &r);

//     int count = 0;
//     while (true)
//     {
//         /* code */
//         int* p = new int[1024 * 1024];
//         count++;
//         std::cout << "size: " << count << std::endl;
//         sleep(1);
//     }
    
//     return 0;
// }