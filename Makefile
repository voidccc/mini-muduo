#####################################
# Copyright (c) 1997 George Foot (george.foot@merton.ox.ac.uk)
# # All rights reserved.
# ######################################
# #目标（可执行文档）名称，库（譬如stdcx,iostr,mysql等），头文件路径
DESTINATION := mini-muduo
LIBS := pthread
INCLUDES := .


RM := rm -f
#C,CC或CPP文件的后缀
PS=cc
# GNU Make的隐含变量定义
CC=g++
CPPFLAGS = -g -Wall -O3 -march=native
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += -MMD

#以下部分无需修改
SOURCE := $(wildcard *.$(PS))
OBJS := $(patsubst %.$(PS),%.o,$(SOURCE))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.$(PS),$(MISSING_DEPS)))

.PHONY : all deps objs clean rebuild

all : $(DESTINATION)

deps : $(DEPS)
		$(CC) -MM -MMD $(SOURCE)


objs : $(OBJS)

clean :
		@$(RM) *.o
		@$(RM) *.d
		@$(RM) $(DESTINATION)

rebuild: clean all 

ifneq ($(MISSING_DEPS),)
$(MISSING_DEPS) :
		@$(RM) $(patsubst %.d,%.o,$@)
endif

-include $(DEPS)

$(DESTINATION) : $(OBJS)
		$(CC) -o $(DESTINATION) $(OBJS) $(addprefix -l,$(LIBS))
#结束
