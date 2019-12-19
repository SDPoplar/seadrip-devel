libseadrip: build/daemon-log.o build-log
	ar build/daemon-log.o -r lib/libseadrip.a

build/daemon-log.o: src/DaemonLog.cpp src/seadrip/DaemonLog.h build-log
	g++ -c -std=c++11 src/DaemonLog.cpp -lboost_regex -o build/daemon-log.o

build-log:
	chmod +x checkpath.sh
	./checkpath.sh build
	./checkpath.sh include
	echo date >> build-log

install:
	cp -f src/seadrip include/
	cp -f include/* /usr/include/
	cp -f lib/*.a /usr/lib/seadrip/

