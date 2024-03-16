#include "cpp-http/httplib.h"
#include "searcher.hpp"


const std::string input = "data/output/raw.bin";
const std::string root_dir = "./rootdir";
int main()
{
  lg.changemod(2);
  searcher::Searcher search;
  search.InitSearcher(input);

  httplib::Server svr;
  svr.set_base_dir(root_dir.c_str());
  svr.Get("/s", [&search](const httplib::Request &req, httplib::Response &res)
        {
        
        if(!req.has_param("word"))
        {
          
          res.set_content("请输入关键字！", "text/plain;charset=utf-8");
          return;
        }

        std::string word = req.get_param_value("word");
       // std::cout << "搜索: " << word <<std::endl;
        lg(Debug, "用户搜索：%s", word.c_str());
        std::string json_string;
        search.Search(word, &json_string);
        res.set_content(json_string, "application/json"); 
        
        });
  svr.listen("0.0.0.0", 8080);
  return 0;
}
