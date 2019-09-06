#include <gtest/gtest.h>

#include "config/ConfigLoader.hpp"

TEST(ConfigLoaderTest, load)
{
    ConfigLoader loader;
    auto configs = loader.getServicesConfigFromIni("tinybooter.d");

    // Check service1
    EXPECT_EQ(configs[0].name, "service1");
    EXPECT_EQ(configs[0].command, "sleep 10");
    EXPECT_EQ(configs[0].restart, true);
    EXPECT_EQ(configs[0].max_retry, 3);    

    // Check service2
    EXPECT_EQ(configs[1].name, "service2");
    EXPECT_EQ(configs[1].command, "sleep 2");
    EXPECT_EQ(configs[1].restart, false);
    EXPECT_EQ(configs[1].max_retry, 0);
}
