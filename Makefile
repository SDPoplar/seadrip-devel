libseadrip: build/daemon-log.o
	ar build/daemon-log.o -r lib/libseadrip.a

build/daemon-log.o: src/DaemonLog.cpp src/seadrip/DaemonLog.h
	g++ -c -std=c++11 src/DaemonLog.cpp -o build/daemon-log.o

build/common-config.o: src/CommonConfig.cpp src/seadrip/ConfigProperty.hpp
	g++ -c -std=c++11 src/CommonConfig.cpp -o build/common-config.o

build/ini-file-reader.o: src/IniReader.cpp src/seadrip/IniFileReader.h src/seadrip/KvFileReader.hpp
	g++ -c -std=c++11 src/IniReader.cpp -lboost_regex -o build/ini-file-reader.o

pre:
	chmod +x checkpath.sh
	./checkpath.sh build
	./checkpath.sh include
	echo date >> build-log

clean:
	rm -f build/*.o
	rm -f lib/*.a

install:
	cp -f src/seadrip include/
	cp -f include/* /usr/include/
	cp -f lib/*.a /usr/lib/seadrip/
