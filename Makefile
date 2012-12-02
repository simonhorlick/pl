CXX = clang++
CXXFLAGS += -g -Wall -D__STDC_CONSTANT_MACROS

GTEST_DIR = /usr/include/gtest-1.6.0

all:

clean:
	rm -f *.o *.a libav_demuxer_unittest

bob.o: bob.cc bob.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

bob_unittest.o: bob_unittest.cc bob.h
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR)/include -c $< -o $@

bob_unittest: bob_unittest.o bob.o file_renderer.o libav_demuxer.o libav_video_decoder.o gtest_main.a
	$(CXX) $(CXXFLAGS) $^ -lz -lavcodec -lavformat -lavutil -lpthread -o $@

libav_demuxer.o: libav_demuxer.cc libav_demuxer.h elementary_stream.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

libav_demuxer_unittest.o: libav_demuxer_unittest.cc libav_demuxer.h
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR)/include -c $< -o $@

libav_demuxer_unittest: libav_demuxer_unittest.o libav_demuxer.o gtest_main.a
	$(CXX) $(CXXFLAGS) $^ -lavcodec -lavformat -lavutil -lpthread -o $@

libdvbpsi_demuxer.o: libdvbpsi_demuxer.cc libdvbpsi_demuxer.h data_source.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

file_source.o: file_source.cc file_source.h data_source.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

libav_stream.o: libav_stream.cc libav_stream.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

libav_video_decoder.o: libav_video_decoder.cc libav_video_decoder.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

libav_video_decoder_unittest.o: libav_video_decoder_unittest.cc libav_video_decoder.h
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR)/include -c $< -o $@

libav_video_decoder_unittest: libav_video_decoder_unittest.o libav_video_decoder.o libav_demuxer.o gtest_main.a
	$(CXX) $(CXXFLAGS) $^ -lz -lavutil -lavcodec -lavformat -lpthread -o $@

# Google Test rules

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) -I$(GTEST_DIR)/include -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) -I$(GTEST_DIR)/include -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

