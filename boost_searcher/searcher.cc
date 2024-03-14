#include "searcher.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>

const std::string input = "data/output/raw.bin";

int main()
{
  searcher::Searcher *search = new searcher::Searcher();
  search->InitSearcher(input);
  std::string seek;
  std::string json_string;
  char buffer[1024];
  while(true)
  {
    
    std::cout << "Enter Search# " << std::endl;
    fgets(buffer, sizeof(buffer) - 1, stdin);
    buffer[strlen(buffer) - 1] = 0;
    seek = buffer;
    search->Search(seek, &json_string);
    std::cout << json_string  << std::endl;

  }

  return 0;
}
