#include "index.hpp"
#include "util.hpp"
#include <algorithm>
#include <jsoncpp/json/json.h>
namespace searcher
{
  struct UltInvertedElem
  {
      uint64_t doc_id;
      int weight;
      std::vector<std::string> keys;
      UltInvertedElem()
      :doc_id(0)
      ,weight(0)
      {}
  };

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
        std::cout << "获取index单例成功" << std::endl;
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
         //  ns_index::Inverted inverted_list_all;
       std::vector<UltInvertedElem> inverted_list_all;

       std::unordered_map<uint64_t, UltInvertedElem> tokens_map;

        for(std::string s : words)
        {
          boost::to_lower(s);
          ns_index::Inverted *inverted_list = index->GetInvertedList(s);
         
          if(nullptr == inverted_list)
          {
            continue;
          }
          
          //inverted_list_all.insert(inverted_list_all.end(), inverted_list->begin(), inverted_list->end());
          for(auto &it : *inverted_list)
          {
            auto &item = tokens_map[it.doc_id];
            item.doc_id = it.doc_id;
            item.weight += it.weight;
            item.keys.push_back(it.keyword);
          }
        }
        for(auto &item : tokens_map)
        {
          inverted_list_all.push_back(std::move(item.second));
        }

        //合并排序
        //std::sort(inverted_list_all.begin(), inverted_list_all.end(), \
        //    [](const ns_index::InvertedElem &elem, const ns_index::InvertedElem &elem1){
        //     return  elem.weight > elem1.weight;
        //    });
        std::sort(inverted_list_all.begin(), inverted_list_all.end(), [](const UltInvertedElem &e1, const UltInvertedElem &e2){
            return e1.weight > e2.weight;

            });
        //构建json
        Json::Value root;
        for(auto &it : inverted_list_all)
        {
          ns_index::DocInfo *doc = index->GetForwardIndex(it.doc_id);
          if(nullptr == doc)
          {
            continue;

          }
          Json::Value elem;
          elem["title"] = doc->title;
          elem["desc"] = getdesc(doc->content, it.keys[0]);
          elem["url"] = doc->url;
          elem["Weight"] = it.weight;
          root.append(elem);
        }
        Json::StyledWriter writer;
        *json_string = writer.write(root);
     }

     std::string getdesc(const std::string &html_content, const std::string &key)
     {
        const size_t psep = 50;
        const size_t nsep = 100;

      //  size_t pos = html_content.find(key);
      //  if(pos == std::string::npos)
      //  {
      //    return "None";
      //  }
       //找到首次出现位置 
        auto it = search(html_content.begin(), html_content.end(), key.begin(), key.end(), [](char x, char y){
              return (std::tolower(x) == std::tolower(y));
            });
        size_t pos = std::distance(html_content.begin(), it);
        
        //截取子串
        size_t begin = 0;
        size_t end = html_content.size() - 1;
        
        if(pos > begin + psep)
        {
          begin = pos - psep;
        }

        if(pos + nsep < end)
        {
          end = pos + nsep;
        }
        
        if(begin >= end)
        {
          return "None";
        }

        return html_content.substr(begin, end - begin);

     }

   private:
     ns_index::index *index;

  };
};
