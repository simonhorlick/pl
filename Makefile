CXX = clang++
CXXFLAGS += -g -Wall

GTEST_DIR = /usr/include/gtest-1.6.0

all:

clean:
	rm -f *.o *.a

libav_demuxer.o: libav_demuxer.cc libav_demuxer.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

libav_demuxer_test.o: libav_demuxer_test.cc libav_demuxer.h
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR)/include -c $< -o $@

libav_demuxer_test: libav_demuxer_test.o libav_demuxer.o gtest_main.a
	$(CXX) $(CXXFLAGS) $^ -lavformat -lpthread -o $@


# Google Test rules

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) -I$(GTEST_DIR)/include -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) -I$(GTEST_DIR)/include -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

