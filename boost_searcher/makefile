SSVER=ssver
PSER=parser
HSER=http_server
cc=g++

.PHONY:all
all:$(PSER) $(SSVER) $(HSER)

$(PSER):Parser.cc
	$(cc) -o $@ $^ -lboost_system -lboost_filesystem -std=c++11

$(SSVER):searcher.cc
	$(cc) -o $@ $^ -ljsoncpp -std=c++11

$(HSER):http_server.cc
	$(cc) -o $@ $^ -lpthread -ljsoncpp -std=c++11

.PHONY:clean
clean:
	rm -f $(SSVER) $(PSER) $(HSER)
