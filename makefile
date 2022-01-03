# puglife makefile
CXX = gcc
DEBUG = -g -Wall -Wextra

.PHONY: main
main: puglife.c 
	$(CXX) puglife.c -o puglife_webserver

.PHONY: debug
debug: puglife.c
	$(CXX) $(DEBUG) puglife.c -o debug_puglife 2>debug_puglife.log && gdb debug_puglife

.PHONY: run
run: puglife_webserver
	$(CXX) puglife.c -o puglife_webserver && ./puglife_webserver

.PHONY: clean
clean:
	cd src/ && rm -f puglife_webserver
	cd src/ && rm -rf puglife_webserver.dSYM
	cd src/ && rm -f debug_puglife
	cd src/ && rm -rf debug_puglife.dSYM	
	cd src/ && rm -f debug_puglife.log
	cd src/ && rm -f *.h.gch
