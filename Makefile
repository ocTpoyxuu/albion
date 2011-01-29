#
# Взято отсюда: http://habrahabr.ru/blogs/development/111691/
#

root_source_dir  := src
source_subdirs   := . ./world
compile_flags    := -Wall -MD -pipe
link_flags       := -s -pipe
libraries        := -ldl -lX11 -lglut -lGL -lGLU -lm

relative_include_dirs := $(addprefix ../../, $(root_include_dir))
relative_source_dirs  := $(addprefix ../../$(root_source_dir)/, $(source_subdirs))
objects_dirs          := $(addprefix $(root_source_dir)/, $(source_subdirs))
objects               := $(patsubst ../../%, %, $(wildcard $(addsuffix /*.c*, $(relative_source_dirs))))
objects               := $(objects:.cpp=.o)
objects               := $(objects:.c=.o)

all : $(program_name)

$(program_name) : obj_dirs $(objects)
	gcc -o $@ $(objects) $(link_flags) $(libraries) $(build_flags)

obj_dirs :
	mkdir -p $(objects_dirs)

VPATH := ../../

%.o : %.cpp
	gcc -o $@ -c $< $(compile_flags) $(build_flags)

%.o : %.c
	gcc -o $@ -c $< $(compile_flags) $(build_flags)

.PHONY : clean

clean :
	rm -rf bin obj

include $(wildcard $(addsuffix /*.d, $(objects_dirs)))
