CFLAGS= -O3 
CPPFLAGS=$(CLFAGS) -O3
LDFLAGS= -lpng -lm
LM	:= libMatch/
LN	:= libNumerics/
BIN	:= bin/

# C source
CSRC	= extern_files_used/io_png.c
# C++ source
CPP_match	= match_surf.cpp extern_files_used/orsa.cpp lib_match_surf.cpp extern_files_used/libMatch/match.cpp
CPP_display = image.cpp display_images_surf.cpp extern_files_used/libMatch/match.cpp
CPP_extract = extract_surf.cpp descriptor.cpp image.cpp keypoint.cpp

# objects
COBJ	= $(CSRC:.c=.o)
CPPOBJ_match_surf	= $(CPP_match:.cpp=.o)
CPPOBJ_display_surf =  $(CPP_display:.cpp=.o)
CPPOBJ_extract_surf =  $(CPP_extract:.cpp=.o)
CPPOBJ = $(COBJ) $(CPPOBJ_extract_surf) $(CPPOBJ_match_surf) $(CPPOBJ_display_surf)

# binaries
EXEC	= extract_surf match_surf display_surf


#all : $(EXEC)

default	: $(BIN) $(EXEC)

$(BIN):
	mkdir -p $(BIN)

# Create objects
%.o	: %.c %.h
	$(CC)  $(CFLAGS) -c -o $@ $<

%.o	: %.cpp %.h
	$(CXX) $(CPPFLAGS) -c -o $@  $<


# Create executives

#extract_surf :

$(EXEC)	: $(COBJ) $(CPPOBJ)
	$(CXX) $(COBJ) $(CPPOBJ_$@)  -o $(BIN)$@  $(LDFLAGS)
#    $(CXX) $(CPPOBJ_$@) -o $(BIN)$@ $(OBJ) $(LDFLAGS)

.PHONY	: clean

clean	:
	$(RM) $(CPPOBJ)

release :
	git archive --format=tar --prefix=surf/ HEAD \
		| gzip > ../surf.tar.gz
