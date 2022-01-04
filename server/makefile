# puglife makefile
CXX = gcc
DEBUG = -g -Wall -Wextra

.PHONY: main
main: puglife.c 
	$(CXX) src/puglife.c -o puglife_webserver

.PHONY: debug
debug: puglife.c
	$(CXX) $(DEBUG) src/puglife.c -o debug_puglife 2>debug_puglife.log && gdb debug_puglife

.PHONY: run
run: puglife.c
	$(CXX) src/puglife.c -o puglife_webserver && ./puglife_webserver

.PHONY: test
test: puglife_webserver
	curl -X POST http://localhost:80/\?wifi_ssd\=test\&wifi_passwd\=pass

.PHONY: clean
clean:
	rm -f puglife_webserver
	rm -f debug_puglife
	rm -f debug_puglife.log
