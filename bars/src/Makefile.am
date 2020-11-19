## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = bars

bars_SOURCES = \
	common/basecandle.h \
	common/datetime.h \
	bars.cc \
	common/datetime.cpp

bars_CXXFLAGS = -I/home/aleksandr/Work/cpp/bars/src/common 

bars_LDFLAGS = 

bars_LDADD = 


if NATIVE_WIN32
bars_LDFLAGS += -mwindows
endif

SUBDIRS = \
	test \
	common \
	common
