#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "util.hpp"

namespace no_index
{
  struct BoostDocInfo
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
    BoostDocInfo *GetForwardIndex(uint64_t doc_id)
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
        BoostDocInfo *bdi = BuildForwarIndex(line_file);
        if (nullptr == bdi)
        {
          std::cerr << "BuildForwarIndex fail!"
                    << "    filename: " << line_file << std::endl;
          continue;
        }
        BuildInvertedIndex(bdi);
      }
      return true;
    }

  private:
    BoostDocInfo *BuildForwarIndex(const std::string &line)
    {
      //split string
      std::string SEP = "\3";
      std::vector<std::string> results;
      util::StringUtil::SplitString(line, &results, SEP);
      if(results.size() != 3)
      {
        return nullptr;
      }

      //fill BoostDocInfo
      BoostDocInfo doc;
      doc.title = results[0];
      doc.content = results[1];
      doc.url = results[2];
      doc.doc_id = forward_index.size();
      
      //insert forward_index
      forward_index.emplace_back(doc);
      return &forward_index.back();

    }

    bool BuildInvertedIndex(const BoostDocInfo *doc)
    {
        
      return true;
    }

  public:
    index()
    {
    }
    ~index()
    {
    }

  private:
    // 正排用数组，下标表示ID
    std::vector<BoostDocInfo> forward_index;
    // 倒排HASH表，关键字和倒排拉链的映射关系
    std::unordered_map<std::string, Inverted> inverted_index;
  };

}
