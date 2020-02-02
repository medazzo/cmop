######################################################
#  Created on: 29 nov. 2012
#      Author: Mohamed Azzouni
######################################################
BUILD_DIR=.build_tests/
BIN:=  $(BUILD_DIR)Tester
FOLDER_TEST := ./
FOLDER_SRC_SERVER:= ../src/Server/
BASE_DIR=../platinum/
ECHO=echo
RM=rm -fr
MKDIR=mkdir 
SCONS=scons -C $(BASE_DIR)Platinum/ 
CC=g++ 
############
FLAGS= -g -Wall -Wno-reorder -ansi -DNPT_CONFIG_ENABLE_LOGGING=1 -DNPT_CONFIG_HAVE_SYSTEM_LOG_CONFIG=1 -lpthread -fpermissive
INCLUDE=  -I $(BASE_DIR)/Neptune/Source/Core/ -I $(FOLDER_SRC_SERVER)
LIBS_PLAT=$(BASE_DIR)/Platinum/Build/Targets/x86-unknown-linux/Debug/libNeptune.a $(BASE_DIR)/Platinum/Build/Targets/x86-unknown-linux/Debug/libaxTLS.a $(BASE_DIR)/Platinum/Build/Targets/x86-unknown-linux/Debug/libZlib.a 
############
FILE_LIST := $(wildcard $(FOLDER_SRC_SERVER)*.cpp)
LOCAL_OBJS_SERVER_FILES := $(FILE_LIST:$(FOLDER_SRC_SERVER)%.cpp=$(BUILD_DIR)%.s.o)
####################"
FILE_LIST := $(wildcard $(FOLDER_TEST)*.cpp)
LOCAL_OBJS_TEST_FILES := $(FILE_LIST:$(FOLDER_TEST)%.cpp=$(BUILD_DIR)%.t.o)
###   ALL ####
OBJECTS_ALL := $(LOCAL_OBJS_SERVER_FILES) $(LOCAL_OBJS_TEST_FILES)  
###   RULES ####
$(BUILD_DIR)%.s.o: $(FOLDER_SRC_SERVER)%.cpp
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDE) $(FLAGS)
$(BUILD_DIR)%.t.o: $(FOLDER_TEST)%.cpp	
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDE) $(FLAGS)
$(BIN):$(OBJECTS_ALL)
	$(CC) $(FLAGS) -o $@ $^ $(INCLUDE) $(LIBS_PLAT)  $(FLAGS)
all: $(BIN)
run:
	$(BIN)
gdb:
	gdb $(BIN)
doc:
	cd ../doc && sudo doxygen ExpoDoxyConfig.conf  && cd -
val:
	valgrind --leak-check=full -v --show-reachable=yes  $(BIN)	
help:
	@$(ECHO) " Prepare,compile platinum for target linux  : prepare."
	@$(ECHO) " UnPrepare,clean platinum for target linux  : unprepare."
	@$(ECHO) " doc for generating doxygen documentation  : doc."
	@$(ECHO) " compile target : all."
	@$(ECHO) " clean   clean libdmc and test file:  clean."
	@$(ECHO) " Running run generated test binary: run."
	@$(ECHO) " Debug   run using gdb generated test binary: gdb."
	@$(ECHO) " Valgrind   run using valgrind tool: val."
unprepare:
	@$(ECHO) " removing objs directory "
	@$(RM)  $(BUILD_DIR)
	@$(ECHO) " cleaning Platinum for linux  "
	@$(SCONS) -c
	@$(RM)  $(BASE_DIR)Platinum/Targets/x86-unknown-linux
	@$(RM)  $(BASE_DIR)Platinum/Build/Targets/x86-unknown-linux/Debug
	@$(RM)  $(BASE_DIR)Platinum/Build/Targets/x86-unknown-linux/Neptune
	@$(RM)  $(BASE_DIR)Platinum/.sconsign.dblite
prepare:
	@$(ECHO) "preparing .. will create objs directory"
	@$(MKDIR) $(BUILD_DIR)
	@$(ECHO) "preparing .. will build Platinum for linux "
	@$(SCONS)
clean:	
	@$(RM)  $(OBJECTS_ALL)  $(BIN)	
