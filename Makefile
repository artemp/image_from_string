all: image-test


image-test: main.cpp
	clang++ -o test main.cpp `mapnik-config --cflags --ldflags` ../node-blend/src/reader.cpp -I../node-blend/src -std=c++11 -stdlib=libc++ -lboost_system -lpng -ljpeg -lboost_timer -lmapnik