#include "index.hpp"

namespace searcher
{
  class Searcher
  {
   public:
     Searcher(){}
     ~Searcher(){}
  
   public:
     void InitSearcher(const std::string &input)
     {
        //创建index对象
        index = ns_index::index::GetInstance();
        //使用index对象建立引索
        index->BuildIndex(input);
     }

      //seek用户搜索关键字，json_string给用户返回的结果
     void Search(const std::string &seek, std::string *json_string)
     {
        //分词
       std::vector<std::string> words;
       util::JiebaUtil::CutString(seek, &words);
        //搜索
       ns_index::Inverted inverted_list_all;
        for(std::string s : words)
        {
          boost::to_lower(s);
          ns_index::Inverted *inverted_list = index->GetInvertedList(s);
          if(nullptr == inverted_list)
          {
            continue;
          }
          inverted_list_all.insert(inverted_list_all.end(), inverted_list.begin(), inverted_list.end());

        }

        //合并排序
        std::sort(inverted_list_all.begin(), inverted_list_all.end(), \
            [](const ns_index::InvertedElem &elem, const ns_index::InvertedElem &elem1){
             return  elem.weight > elem1.weight;
            });
        //构建json
     }

   private:
     ns_index::index *index;

  };
};
