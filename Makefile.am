AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
AM_CXXFLAGS = ${OPENCV_CFLAGS}
AM_CXXFLAGS += -Isrc/



bin_PROGRAMS = tracker
tracker_SOURCES = 	src/main.cpp src/ROI.cpp 
include_HEADERS =  src/defines.hpp src/ROI.hpp 
tracker_CPPFLAGS = 	 -Isrc/
tracker_CPPFLAGS += -DINSTALLDIR='"$(pkgdatadir)"'
tracker_LDADD = ${OPENCV_LIBS}
