libseadrip: build/daemon-log.o build/common-config.o build/ini-file-reader.o
	ar rcs lib/libseadrip.a build/daemon-log.o build/common-config.o build/ini-file-reader.o

build/daemon-log.o: src/DaemonLog.cpp src/seadrip/DaemonLog.h
	g++ -c -std=c++11 src/DaemonLog.cpp -o build/daemon-log.o

build/common-config.o: src/CommonConfig.cpp src/seadrip/ConfigProperty.hpp
	g++ -c -std=c++11 src/CommonConfig.cpp -o build/common-config.o

build/ini-file-reader.o: src/IniReader.cpp src/seadrip/IniFileReader.h src/seadrip/KvFileReader.h
	g++ -c -std=c++11 src/IniReader.cpp -lboost_regex -o build/ini-file-reader.o

build/common-core.o: src/CommonCore.cpp src/seadrip/GlobalCore.hpp
	g++ -c -std=c++11 src/CommonCore.cpp -o build/common-core.o

pre:
	chmod +x checkpath.sh
	./checkpath.sh build
	./checkpath.sh include
	./checkpath.sh lib
	./checkpath.sh /usr/lib/seadrip
	touch build-prepare

clean:
	rm -f build/*.o
	rm -f lib/*.a

install:
	cp -rf src/seadrip include/
	cp -rf include/* /usr/include/
	cp -rf lib/*.a /usr/lib/seadrip/
	echo "/usr/lib/seadrip" > /etc/ld.so.conf.d/seadrip.conf
