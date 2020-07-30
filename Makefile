#libseadrip: build/daemon-log.o build/common-config.o build/ini-file-reader.o build/tick-core.o build/daemon-core.o build/daemon-config.o build/socket-daemon-config.o #build/singleton-core.o
#	ar rcs lib/libseadrip.a build/daemon-log.o build/common-config.o build/ini-file-reader.o build/tick-core.o build/daemon-core.o build/daemon-config.o build/socket-daemon-config.o

libseadrip: build/config/common-config.o build/config/daemon-config.o build/shell/shell-input.o
	ar rcs lib/seadrip.a build/config/common-config.o build/config/daemon-config.o build/shell/shell-input.o

#build/daemon-log.o: src/DaemonLog.cpp src/seadrip/DaemonLog.h
#	g++ -c -std=c++11 src/DaemonLog.cpp -o build/daemon-log.o

build/config/common-config.o: src/CommonConfig.cpp src/seadrip/config/ConfigProperty.hpp
	mkdir -p build/config
	g++ -c -std=c++11 src/CommonConfig.cpp -o build/config/common-config.o

build/shell/shell-input.o: src/ShellInput.cpp src/seadrip/shell/ShellInput.h
	mkdir -p build/shell
	g++ -c -std=c++11 src/ShellInput.cpp -o build/shell/shell-input.o

#build/ini-file-reader.o: src/IniReader.cpp src/seadrip/IniFileReader.h src/seadrip/KvFileReader.h
#	g++ -c -std=c++11 src/IniReader.cpp -lboost_regex -o build/ini-file-reader.o

# build/tick-core.o: src/TickCore.cpp src/seadrip/TickCore.h
#	g++ -c -std=c++11 src/TickCore.cpp -o build/tick-core.o

#build/singleton-core.o: src/SingletonCore.cpp src/seadrip/SingletonCore.h
#	g++ -c -std=c++11 src/SingletonCore.cpp -o build/singleton-core.o

#build/daemon-core.o: src/DaemonCore.cpp src/seadrip/DaemonCore.hpp
#	g++ -c -std=c++11 src/DaemonCore.cpp -o build/daemon-core.o

build/config/daemon-config.o: src/DaemonConfig.cpp src/seadrip/config/DaemonConfig.h
	g++ -c -std=c++11 src/DaemonConfig.cpp -o build/config/daemon-config.o

#build/socket-daemon-config.o: src/SocketDaemonConfig.cpp src/seadrip/SocketDaemonConfig.h
#	g++ -c -std=c++11 src/SocketDaemonConfig.cpp -o build/socket-daemon-config.o

pre:
	chmod +x checkpath.sh
	./checkpath.sh build
	./checkpath.sh include
	./checkpath.sh lib
	./checkpath.sh /usr/lib/seadrip
	touch build-prepare

clean:
	rm -f build/*.o
	rm -f build/*/*.o
	rm -f lib/*.a

install:
	cp -rf src/seadrip include/
	cp -rf include/* /usr/include/
	cp -rf lib/*.a /usr/lib/seadrip/
	echo "/usr/lib/seadrip" > /etc/ld.so.conf.d/seadrip.conf
