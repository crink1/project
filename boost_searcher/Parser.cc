#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include "util.hpp"
// using namespace std;

// html路径
const std::string input = "data/input";
const std::string output = "data/output/raw.bin";

typedef struct HtmlInfo
{
  std::string title;
  std::string content;
  std::string url;

} HtmlInfo_t;

bool EnumFile(const std::string &input, std::vector<std::string> *files_list);

bool Parse(const std::vector<std::string> &files_list, std::vector<HtmlInfo_t> *results);

bool Save(const std::vector<HtmlInfo_t> &results, const std::string &output);

int main()
{
  // 文件带路径存到file_list
  std::vector<std::string> files_list;

  if (!EnumFile(input, &files_list))
  {
    std::cerr << "enum file name err" << std::endl;
    return 1;
  }

  // 解析file_list里面的内容
  std::vector<HtmlInfo> results;

  if (!Parse(files_list, &results))
  {
    std::cerr << "parse err" << std::endl;
    return 2;
  }

  // 解析完把结果存到output里面，使用\3作为分隔符
  if (!Save(results, output))
  {
    std::cerr << "save err" << std::endl;
    return 3;
  }

  return 0;
}

bool EnumFile(const std::string &input, std::vector<std::string> *files_list)
{
  namespace fs = boost::filesystem;

  // 路径文件
  fs::path root_path(input);
  // 判断路径文件是否存在
  if (!fs::exists(root_path))
  {
    std::cerr << input << "path err" << std::endl;
    return false;
  }
  // 空迭代器用于递归结束的判断
  fs::recursive_directory_iterator end;
  for (fs::recursive_directory_iterator it(root_path); it != end; it++)
  {
    // 判断是否是普通文件
    if (!fs::is_regular_file(*it))
    {
      continue;
    }
    if (it->path().extension() != ".html")
    {
      continue;
    }

    //   std::cout << "bebug" << it->path().string() << std::endl;;
    files_list->push_back(it->path().string());
  }

  return true;
}

static bool ParseTitle(const std::string &file, std::string *title)
{
  size_t begin = file.find("<title>");
  if (begin == std::string::npos)
  {
    return false;
  }

  size_t end = file.find("</title>");
  if (end == std::string::npos)
  {
    return false;
  }

  begin += std::string("<title>").size();
  if (begin > end)
  {
    return false;
  }

  *title = file.substr(begin, end - begin);
  return true;
}
static bool ParseContent(const std::string &file, std::string *content)
{
  // 去除html标签
  enum status
  {
    LAB,
    CONTENT
  };

  enum status st = LAB;
  for (char c : file)
  {
    switch (st)
    {
    case LAB:
      if (c == '>')
      {
        st = CONTENT;
      }
      break;

    case CONTENT:
      if (c == '<')
      {
        st = LAB;
      }
      else
      {
        if (c == '\n') //'\n'作为解析后的文本分隔符，所以这里替换掉
        {
          c = ' ';
        }
        content->push_back(c);
      }
      break;

    default:
      break;
    }
  }
  return true;
}

static bool ParseUrl(const std::string &file_path, std::string *url)
{
  std::string head = "https://www.boost.org/doc/libs/1_84_0/doc/html";
  std::string tail = file_path.substr(input.size());
  *url = head + tail;

  return true;
}

static void printdoc(const HtmlInfo_t &doc)
{
  std::cout << "title: " << doc.title << std::endl;
  std::cout << "content: " << doc.content << std::endl;
  std::cout << "url: " << doc.url << std::endl;
}

bool Parse(const std::vector<std::string> &files_list, std::vector<HtmlInfo_t> *results)
{
  for (const std::string &file : files_list)
  {
    // 读文件Read()
    std::string result;
    if (!util::FileUtil::ReadFile(file, &result))
    {
      continue;
    }
    HtmlInfo_t info;
    // 解析title
    if (!ParseTitle(result, &info.title))
    {
      continue;
    }
    // 解析content
    if (!ParseContent(result, &info.content))
    {
      continue;
    }
    // 构建url
    if (!ParseUrl(file, &info.url))
    {
      continue;
    }

    results->emplace_back(std::move(info));

    // printdoc(info);
  }
  return true;
}

bool Save(const std::vector<HtmlInfo_t> &results, const std::string &output)
{
#define SEP '\3'

  std::ofstream out(output, std::ios::out | std::ios::binary);
  if (!out.is_open())
  {
    std::cerr << "open " << output << " err" << std::endl;
    return false;
  }

  for (auto &it : results)
  {
    std::string outstr;
    outstr = it.title;
    outstr += SEP;
    outstr += it.content;
    outstr += SEP;
    outstr += it.url;
    outstr += '\n';

    out.write(outstr.c_str(), outstr.size());
  }

  out.close();

  return true;
}
