#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <mutex>
#include <unordered_map>
#include "util.hpp"

namespace ns_index
{
  struct DocInfo
  {
    std::string title;   // 文档标题
    std::string content; // 内容
    std::string url;     // 官网URL
    int doc_id;          // 文档ID
  };

  struct InvertedElem
  {
    uint64_t doc_id;
    std::string keyword;
    int weight;
  };

  typedef std::vector<InvertedElem> Inverted;
  class index
  {

  public:
  
    static index* GetInstance()
    {
      if(nullptr == instance)
      {
        _lock.lock();
        if(nullptr == instance)
        {
          instance = new index();
        }
        _lock.unlock();
      }
      return instance;
    }

    DocInfo *GetForwardIndex(uint64_t doc_id)
    {
      if (doc_id >= forward_index.size())
      {
        std::cerr << "doc id out range, error " << std::endl;
      }
      return &forward_index[doc_id];
    }

    // 倒排拉链
    Inverted *GetInvertedList(const std::string &key)
    {
      std::unordered_map<std::string, Inverted>::iterator it = inverted_index.find(key);
      if (it == inverted_index.end())
      {
        std::cerr << key << "not find" << std::endl;
        return nullptr;
      }
      return &(it->second);
    }

    // 构建倒排正排索引
    bool BuildIndex(const std::string &input)
    {
      std::ifstream in(input, std::ios::in | std::ios::binary);
      if (in.is_open())
      {
        std::cerr << input << "  open error" << std::endl;
        return false;
      }

      std::string line_file;
      while (std::getline(in, line_file))
      {
        DocInfo *di = BuildForwarIndex(line_file);
        if (nullptr == di)
        {
          std::cerr << "BuildForwarIndex fail!"
                    << "    filename: " << line_file << std::endl;
          continue;
        }
        BuildInvertedIndex(*di);
      }
      return true;
    }

  private:
    DocInfo *BuildForwarIndex(const std::string &line)
    {
      // split string
      std::string SEP = "\3";
      std::vector<std::string> results;
      util::StringUtil::SplitString(line, &results, SEP);
      if (results.size() != 3)
      {
        return nullptr;
      }

      // fill DocInfo
      DocInfo doc;
      doc.title = results[0];
      doc.content = results[1];
      doc.url = results[2];
      doc.doc_id = forward_index.size();

      // insert forward_index
      forward_index.emplace_back(std::move(doc));
      return &forward_index.back();
    }

    bool BuildInvertedIndex(const DocInfo &doc)
    {
      struct word_cnt
      {

        word_cnt()
            : title_cnt(0), content_cnt(0)
        {
        }

        int title_cnt;
        int content_cnt;
      };

      std::unordered_map<std::string, word_cnt> word_map;
      
      //标题分词
      std::vector<std::string> title_words;
      util::JiebaUtil::CutString(doc.title, &title_words);
      for(auto s : title_words)
      {
        boost::to_lower(s);
        word_map[s].title_cnt++;
      }

      //内容分词
      std::vector<std::string> content_cnt;
      util::JiebaUtil::CutString(doc.content, &content_cnt);
      for(auto s : content_cnt)
      {
        boost::to_lower(s);
        word_map[s].content_cnt++;
      }
      
#define W1 2
#define W2 1

      //填充
      for(auto &pair : word_map)
      {
        InvertedElem elem;
        elem.doc_id = doc.doc_id;
        elem.keyword = pair.first;
        elem.weight = pair.second.title_cnt * W1 + pair.second.content_cnt * W2; //权重
        inverted_index[pair.first].push_back(std::move(elem));
        
      }

      
      return true;
    }

  public:
    ~index()
    {}
    
  private:
    index()
    {}
    index(const index&) = delete;
    index& operator=(const index&) = delete;

  private:
    static index *instance;
    static std::mutex _lock;
    // 正排用数组，下标表示ID
    std::vector<DocInfo> forward_index;
    // 倒排HASH表，关键字和倒排拉链的映射关系
    std::unordered_map<std::string, Inverted> inverted_index;
  };

}
