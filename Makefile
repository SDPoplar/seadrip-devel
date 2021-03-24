CC = g++ --std=c++11

SRCPATH = src
SRC_CFG = $(SRCPATH)/config
SRC_SHELL = $(SRCPATH)/shell
SRC_COMP = $(SRCPATH)/component
SRC_WP = $(SRCPATH)/work-process

OBJPATH = build
OBJ_CFG = $(OBJPATH)/config
OBJ_SHELL = $(OBJPATH)/shell
OBJ_COMP = $(OBJPATH)/component
OBJ_WP = $(OBJPATH)/work-process

libseadrip: lib/libseadrip_config.a lib/libseadrip_shell.a lib/libseadrip_file.a lib/libseadrip_component.a lib/libseadrip_workprocess.a
	ar rcs lib/libseadrip.a $(OBJ_CFG)/common-config.o $(OBJ_CFG)/socket-config.o $(OBJ_CFG)/daemon-config.o $(OBJ_CFG)/debug-config.o $(OBJ_CFG)/epoll-config.o $(OBJ_CFG)/work-process-config.o build/file/ini-file-reader.o build/shell/shell-input.o $(OBJ_COMP)/socket-component.o $(OBJ_COMP)/epoll-component.o $(OBJ_COMP)/inet-socket-component.o $(OBJ_COMP)/work-process-component.o $(OBJ_WP)/type.o $(OBJ_WP)/master.o $(OBJ_WP)/client.o
	g++ -std=c++11 -fPIC -shared -o lib/libseadrip.so lib/libseadrip_config.a lib/libseadrip_shell.a lib/libseadrip_file.a lib/libseadrip_component.a lib/libseadrip_workprocess.a

# ============================== package ==============================================================

lib/libseadrip_config.a: $(OBJ_CFG)/common-config.o $(OBJ_CFG)/socket-config.o $(OBJ_CFG)/daemon-config.o $(OBJ_CFG)/debug-config.o $(OBJ_CFG)/epoll-config.o $(OBJ_CFG)/work-process-config.o
	ar rcs lib/libseadrip_config.a $(OBJ_CFG)/common-config.o $(OBJ_CFG)/socket-config.o $(OBJ_CFG)/daemon-config.o $(OBJ_CFG)/debug-config.o $(OBJ_CFG)/epoll-config.o $(OBJ_CFG)/work-process-config.o
	$(CC) -fPIC -shared -o lib/libseadrip_config.so $(OBJ_CFG)/common-config.o $(OBJ_CFG)/socket-config.o $(OBJ_CFG)/daemon-config.o $(OBJ_CFG)/debug-config.o $(OBJ_CFG)/epoll-config.o $(OBJ_CFG)/work-process-config.o

lib/libseadrip_file.a: build/file/pathlock build/file/ini-file-reader.o
	ar rcs lib/libseadrip_file.a build/file/ini-file-reader.o
	g++ build/file/ini-file-reader.o -shared -o lib/libseadrip_file.so

lib/libseadrip_shell.a: build/shell/pathlock build/shell/shell-input.o
	ar rcs lib/libseadrip_shell.a build/shell/shell-input.o
	g++ build/shell/shell-input.o -shared -o lib/libseadrip_shell.so

lib/libseadrip_component.a: $(OBJ_COMP)/pathlock $(OBJ_COMP)/socket-component.o $(OBJ_COMP)/inet-socket-component.o $(OBJ_COMP)/epoll-component.o $(OBJ_COMP)/work-process-component.o
	ar rcs lib/libseadrip_component.a $(OBJ_COMP)/socket-component.o $(OBJ_COMP)/epoll-component.o $(OBJ_COMP)/inet-socket-component.o $(OBJ_COMP)/work-process-component.o
	g++ -shared -o lib/libseadrip_component.so $(OBJ_COMP)/socket-component.o $(OBJ_COMP)/inet-socket-component.o $(OBJ_COMP)/epoll-component.o $(OBJ_COMP)/work-process-component.o

lib/libseadrip_workprocess.a: $(OBJ_WP)/pathlock $(OBJ_WP)/type.o $(OBJ_WP)/master.o $(OBJ_WP)/client.o
	ar rcs lib/libseadrip_workprocess.a $(OBJ_WP)/type.o $(OBJ_WP)/master.o $(OBJ_WP)/client.o

# ============================== atom =================================================================

# ============================== atom - config ========================================================
$(OBJ_CFG)/pathlock:
	mkdir -p $(OBJ_CFG)
	touch $(OBJ_CFG)/pathlock

$(OBJ_CFG)/common-config.o: $(OBJ_CFG)/pathlock $(SRC_CFG)/CommonConfig.cpp src/seadrip/config/base/BaseConfig.h src/seadrip/config/base/ConfigProperty.hpp
	$(CC) -c -o $(OBJ_CFG)/common-config.o $(SRC_CFG)/CommonConfig.cpp -fPIC

$(OBJ_CFG)/debug-config.o: $(SRC_CFG)/DebugConfig.cpp src/seadrip/config/atom/DebugConfig.h
	$(CC) -c -o $(OBJ_CFG)/debug-config.o $(SRC_CFG)/DebugConfig.cpp -fPIC

$(OBJ_CFG)/daemon-config.o: $(SRC_CFG)/DaemonConfig.cpp src/seadrip/config/atom/DaemonConfig.h
	$(CC) -c -o $(OBJ_CFG)/daemon-config.o $(SRC_CFG)/DaemonConfig.cpp -fPIC

$(OBJ_CFG)/socket-config.o: src/config/SocketConfig.cpp src/seadrip/config/atom/SocketConfig.h
	$(CC) -c -o $(OBJ_CFG)/socket-config.o src/config/SocketConfig.cpp -fPIC

$(OBJ_CFG)/epoll-config.o: src/config/EpollConfig.cpp src/seadrip/config/atom/EpollConfig.h
	g++ -c -std=c++11 src/config/EpollConfig.cpp -o $(OBJ_CFG)/epoll-config.o -fPIC

$(OBJ_CFG)/work-process-config.o: $(SRC_CFG)/WorkProcessConfig.cpp src/seadrip/config/atom/WorkProcessConfig.h
	$(CC) -c -o $(OBJ_CFG)/work-process-config.o $(SRC_CFG)/WorkProcessConfig.cpp -fPIC

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
$(OBJ_COMP)/pathlock:
	mkdir -p $(OBJ_COMP)
	touch $(OBJ_COMP)/pathlock

$(OBJ_COMP)/socket-component.o: $(SRC_COMP)/SocketComponent.cpp src/seadrip/component/SocketComponent.h
	$(CC) -c -o $(OBJ_COMP)/socket-component.o $(SRC_COMP)/SocketComponent.cpp -fPIC

$(OBJ_COMP)/inet-socket-component.o: $(SRC_COMP)/InetSocketComponent.cpp src/seadrip/component/SocketComponent.h
	$(CC) -c -o $(OBJ_COMP)/inet-socket-component.o $(SRC_COMP)/InetSocketComponent.cpp -fPIC

$(OBJ_COMP)/epoll-component.o: $(SRC_COMP)/EpollComponent.cpp src/seadrip/component/EpollComponent.h
	$(CC) -c -o $(OBJ_COMP)/epoll-component.o $(SRC_COMP)/EpollComponent.cpp -fPIC

$(OBJ_COMP)/work-process-component.o: $(SRC_COMP)/WorkProcessComponent.cpp src/seadrip/component/WorkProcessComponent.h
	$(CC) -c -o $(OBJ_COMP)/work-process-component.o $(SRC_COMP)/WorkProcessComponent.cpp -fPIC

# ============================== work process =============================================================
$(OBJ_WP)/pathlock:
	mkdir -p $(OBJ_WP)
	touch $(OBJ_WP)/pathlock

$(OBJ_WP)/type.o: $(SRC_WP)/type.cpp
	$(CC) -c -o $(OBJ_WP)/type.o $(SRC_WP)/type.cpp -fPIC

$(OBJ_WP)/master.o: $(SRC_WP)/master.cpp
	$(CC) -c -o $(OBJ_WP)/master.o $(SRC_WP)/master.cpp -fPIC

$(OBJ_WP)/client.o: $(SRC_WP)/client.cpp
	$(CC) -c -o $(OBJ_WP)/client.o $(SRC_WP)/client.cpp -fPIC

clean:
	rm -f build/*.o
	rm -f build/*/*.o
	rm -f lib/*.a
	rm -f lib/*.so

install:
	cp -rf src/seadrip include/
	cp -rf include/* /usr/include/
	cp -f lib/libseadrip.so /usr/lib/libseadrip.so

