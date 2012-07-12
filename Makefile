CXX = clang++
CXXFLAGS += -g -Wall -D__STDC_CONSTANT_MACROS

GTEST_DIR = /usr/include/gtest-1.6.0

all:

clean:
	rm -f *.o *.a libav_demuxer_unittest

libav_demuxer.o: libav_demuxer.cc libav_demuxer.h data_source.h libav_stream.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

libav_demuxer_unittest.o: libav_demuxer_unittest.cc libav_demuxer.h
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR)/include -c $< -o $@

libav_demuxer_unittest: libav_demuxer_unittest.o libav_stream.o file_source.o libav_demuxer.o gtest_main.a
	$(CXX) $(CXXFLAGS) $^ -lavformat -lpthread -o $@

libdvbpsi_demuxer.o: libdvbpsi_demuxer.cc libdvbpsi_demuxer.h data_source.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

file_source.o: file_source.cc file_source.h data_source.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

libav_stream.o: libav_stream.cc libav_stream.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

libav_video_decoder.o: libav_video_decoder.cc libav_video_decoder.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cc file_source.h libdvbpsi_demuxer.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

pl: main.o file_source.o libdvbpsi_demuxer.o
	$(CXX) $(CXXFLAGS) $^ -ldvbpsi -lavformat -lpthread -o $@

# Google Test rules

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) -I$(GTEST_DIR)/include -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) -I$(GTEST_DIR)/include -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

