
#定义项目编译的根目录,通过export把某个变量声明为全局的[其他文件中可以用]，这里获取当前这个文件所在的路径作为根目录；
#BUILD_ROOT = /mnt/hgfs/linux/nginx
export BUILD_ROOT = $(shell pwd)

#定义头文件的路径变量

export INCLUDE_PATH = -I$(BUILD_ROOT)/_include           \
					  -I$(BUILD_ROOT)/_include/net          \
					  -I$(BUILD_ROOT)/_include/database  \
					  -I$(BUILD_ROOT)/_include/logic/Gateway  \
					  -I$(BUILD_ROOT)/_include/logic/Login     \
					  -I$(BUILD_ROOT)/_include/lua/LuaBridge     \
					  -I$(BUILD_ROOT)/_include/lua/LuaBridge/detail     \
					  -I$(BUILD_ROOT)/_include/lua

#定义我们要编译的目录
BUILD_DIR = $(BUILD_ROOT)/signal/ \
			$(BUILD_ROOT)/proc/   \
			$(BUILD_ROOT)/net/    \
			$(BUILD_ROOT)/misc/   \
			$(BUILD_ROOT)/logic/GatewayServer/ \
			$(BUILD_ROOT)/logic/LoginServer/ \
			$(BUILD_ROOT)/logic/  \
			$(BUILD_ROOT)/database/  \
			$(BUILD_ROOT)/lua/    \
			$(BUILD_ROOT)/app/ 

#编译时是否生成调试信息。GNU调试器可以利用该信息
#很多调试工具，包括Valgrind工具集都会因为这个为true能够输出更多的调试信息；
export DEBUG = true

