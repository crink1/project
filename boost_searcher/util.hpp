#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "cppjieba/Jieba.hpp"
namespace util
{
  class FileUtil
  {
  public:
    static bool ReadFile(const std::string &file_path, std::string *out)
    {
      std::ifstream in(file_path, std::ios::in);
      if (!in.is_open())
      {
        std::cerr << "open file  " << file_path << "  err" << std::endl;
        return false;
      }
      std::string line;
      while (std::getline(in, line)) // operator bool
      {
        *out += line;
      }
      in.close();

      return true;
    }
  };

  class StringUtil
  {
  public:
    static void SplitString(const std::string &in, std::vector<std::string> *out, const std::string &sep)
    {
      boost::split(*out, in, boost::is_any_of(sep), boost::token_compress_on);
    }
  };

  const char *const DICT_PATH = "./dict/jieba.dict.utf8";
  const char *const HMM_PATH = "./dict/hmm_model.utf8";
  const char *const USER_DICT_PATH = "./dict/user.dict.utf8";
  const char *const IDF_PATH = "./dict/idf.utf8";
  const char *const STOP_WORD_PATH = "./dict/stop_words.utf8";

  class JiebaUtil
  {
  public:
    static void CutString(const std::string &src, std::vector<std::string> *out)
    {
      jieba.CutForSearch(src, *out);
    }

  private:
    static cppjieba::Jieba jieba;
  };

  cppjieba::Jieba JiebaUtil::jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH);
}
