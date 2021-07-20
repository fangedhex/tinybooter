#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <config/JobConfig.h>
#include <config/RunOnceJobConfig.h>

TEST(Convert, JobKind)
{
  YAML::Node node;
  node = "init";

  auto kind = node.as<JobKind>();
  EXPECT_EQ(kind, JobKind::INIT);

  EXPECT_EQ(ToString(kind), "init");
}

TEST(YamlDecode, JobConfig)
{
  YAML::Node node;
  node["kind"] = "init";
  node["name"] = "test";
  node["cmd"] = "echo";
  std::vector<std::string> args = {"hello world!"};
  node["args"] = args;

  auto config = node.as<JobConfig*>();

  EXPECT_NE(config, nullptr);

  EXPECT_EQ(config->kind, JobKind::INIT);
  EXPECT_EQ(config->name, "test");
  EXPECT_EQ(config->cmd, "echo");
  EXPECT_THAT(config->args, ::testing::ElementsAre("hello world!"));

  delete config;
}

TEST(YamlDecode, RunOnceJobConfig)
{
  YAML::Node node;
  node["kind"] = "init";
  node["name"] = "test";
  node["cmd"] = "echo";
  std::vector<std::string> args = {"hello world!"};
  node["args"] = args;

  auto config = node.as<RunOnceJobConfig*>();

  EXPECT_NE(config, nullptr);

  EXPECT_EQ(config->kind, JobKind::INIT);
  EXPECT_EQ(config->name, "test");
  EXPECT_EQ(config->cmd, "echo");
  EXPECT_THAT(config->args, ::testing::ElementsAre("hello world!"));
}
