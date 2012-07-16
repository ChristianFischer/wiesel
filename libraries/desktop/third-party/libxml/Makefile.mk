# libxml2 library

# add the include path returned by xml2-config to the C_FLAGS variable
C_FLAGS		+= `xml2-config --cflags`
LD_FLAGS	+= `xml2-config --libs`
