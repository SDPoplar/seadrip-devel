libseadrip: lib/libseadrip_config.a lib/libseadrip_shell.a lib/libseadrip_file.a lib/libseadrip_component.a
	ar rcs lib/libseadrip.a build/config/common-config.o build/config/socket-config.o build/config/daemon-config.o build/config/debug-config.o build/config/epoll-config.o build/file/ini-file-reader.o build/shell/shell-input.o build/component/socket-component.o build/component/epoll-component.o build/component/inet-socket-component.o build/component/work-process-component.o
	g++ -std=c++11 -fPIC -shared -o lib/libseadrip.so lib/libseadrip_config.a lib/libseadrip_shell.a lib/libseadrip_file.a lib/libseadrip_component.a

# ============================== package ==============================================================

lib/libseadrip_config.a: build/config/common-config.o build/config/socket-config.o build/config/daemon-config.o build/config/debug-config.o build/config/epoll-config.o
	ar rcs lib/libseadrip_config.a build/config/common-config.o build/config/socket-config.o build/config/daemon-config.o build/config/debug-config.o build/config/epoll-config.o
	g++ -fPIC -shared -o lib/libseadrip_config.so build/config/common-config.o build/config/socket-config.o build/config/daemon-config.o build/config/debug-config.o build/config/epoll-config.o

lib/libseadrip_file.a: build/file/pathlock build/file/ini-file-reader.o
	ar rcs lib/libseadrip_file.a build/file/ini-file-reader.o
	g++ build/file/ini-file-reader.o -shared -o lib/libseadrip_file.so

lib/libseadrip_shell.a: build/shell/pathlock build/shell/shell-input.o
	ar rcs lib/libseadrip_shell.a build/shell/shell-input.o
	g++ build/shell/shell-input.o -shared -o lib/libseadrip_shell.so

lib/libseadrip_component.a: build/component/pathlock build/component/socket-component.o build/component/inet-socket-component.o build/component/epoll-component.o build/component/work-process-component.o
	ar rcs lib/libseadrip_component.a build/component/socket-component.o build/component/epoll-component.o build/component/inet-socket-component.o build/component/work-process-component.o
	g++ -shared -o lib/libseadrip_component.so build/component/socket-component.o build/component/inet-socket-component.o build/component/epoll-component.o build/component/work-process-component.o

# ============================== atom =================================================================

# ============================== atom - config ========================================================
build/config/common-config.o: build/config/pathlock src/config/CommonConfig.cpp src/seadrip/config/base/BaseConfig.h src/seadrip/config/base/ConfigProperty.hpp
	g++ -c -std=c++11 src/config/CommonConfig.cpp -o build/config/common-config.o -fPIC

build/config/pathlock:
	mkdir -p build/config
	touch build/config/pathlock

build/config/debug-config.o: src/config/DebugConfig.cpp src/seadrip/config/atom/DebugConfig.h
	g++ -c -std=c++11 src/config/DebugConfig.cpp -o build/config/debug-config.o -fPIC

build/config/daemon-config.o: src/config/DaemonConfig.cpp src/seadrip/config/atom/DaemonConfig.h
	g++ -c -std=c++11 src/config/DaemonConfig.cpp -o build/config/daemon-config.o -fPIC

build/config/socket-config.o: src/config/SocketConfig.cpp src/seadrip/config/atom/SocketConfig.h
	g++ -c -std=c++11 src/config/SocketConfig.cpp -o build/config/socket-config.o -fPIC

build/config/epoll-config.o: src/config/EpollConfig.cpp src/seadrip/config/atom/EpollConfig.h
	g++ -c -std=c++11 src/config/EpollConfig.cpp -o build/config/epoll-config.o -fPIC

# ============================== atom - file ===========================================================

build/file/pathlock:
	mkdir -p build/file
	touch build/file/pathlock

build/file/ini-file-reader.o: src/file/IniReader.cpp src/seadrip/file/IniFileReader.h src/seadrip/file/KvFileReader.h
	mkdir -p build/file
	g++ -c -std=c++11 src/file/IniReader.cpp -lboost_regex -o build/file/ini-file-reader.o -fPIC


# ============================== atom - shell ==========================================================
build/shell/pathlock:
	mkdir -p build/shell
	touch build/shell/pathlock

build/shell/shell-input.o: src/ShellInput.cpp src/seadrip/shell/ShellInput.h
	mkdir -p build/shell
	g++ -c -std=c++11 src/ShellInput.cpp -o build/shell/shell-input.o -fPIC

build/shell/daemon-util.o: src/DaemonUtil.cpp src/seadrip/shell/Daemon.h
	g++ -c -std=c++11 src/DaemonUtil.cpp -o build/shell/daemon-util.o

# ============================== atom - component =======================================================
build/component/pathlock:
	mkdir -p build/component
	touch build/component/pathlock

build/component/socket-component.o: src/component/SocketComponent.cpp src/seadrip/component/SocketComponent.h
	g++ -c -std=c++11 src/component/SocketComponent.cpp -o build/component/socket-component.o -fPIC

build/component/inet-socket-component.o: src/component/InetSocketComponent.cpp src/seadrip/component/SocketComponent.h
	g++ -c -std=c++11 src/component/InetSocketComponent.cpp -o build/component/inet-socket-component.o -fPIC

build/component/epoll-component.o: src/component/EpollComponent.cpp src/seadrip/component/EpollComponent.h
	g++ -c -std=c++11 src/component/EpollComponent.cpp -o build/component/epoll-component.o -fPIC

build/component/work-process-component.o: src/component/WorkProcessComponent.cpp src/seadrip/component/WorkProcessComponent.h
	g++ -c -std=c++11 -fPIC -o build/component/work-process-component.o src/component/WorkProcessComponent.cpp

#build/daemon-log.o: src/DaemonLog.cpp src/seadrip/DaemonLog.h
#	g++ -c -std=c++11 src/DaemonLog.cpp -o build/daemon-log.o

# build/tick-core.o: src/TickCore.cpp src/seadrip/TickCore.h
#	g++ -c -std=c++11 src/TickCore.cpp -o build/tick-core.o

#build/singleton-core.o: src/SingletonCore.cpp src/seadrip/SingletonCore.h
#	g++ -c -std=c++11 src/SingletonCore.cpp -o build/singleton-core.o

#build/daemon-core.o: src/DaemonCore.cpp src/seadrip/DaemonCore.hpp
#	g++ -c -std=c++11 src/DaemonCore.cpp -o build/daemon-core.o

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
	rm -f lib/*.so

install:
	cp -rf src/seadrip include/
	cp -rf include/* /usr/include/
	cp -f lib/libseadrip.so /usr/lib/libseadrip.so

