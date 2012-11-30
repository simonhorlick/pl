#include <gtest/gtest.h>
#include "multicast_source.h"

TEST(MulticastTest, Connect) {
    MulticastSource source("239.255.0.54", 5004);
    ASSERT_TRUE(source.Initialise());
}

