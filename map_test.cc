//
// Created by jacob on 12/17/21.
//

#include "map.h"
#include "gtest/gtest.h"

namespace {

class MapTest : public testing::Test {
 protected:
  void SetUp() override {
    std::vector<int> nums = {10, 5, 56, 16, 30, 2};
    for (int i = 0; i < nums.size(); i++) {
      map_.Insert(std::make_pair(i, nums[i]));
    }
  }

  Map<int, int> map_;
};

TEST_F(MapTest, DefaultConstructor) {
  Map<int, int> empty_map;
  EXPECT_TRUE(empty_map.Empty());
  EXPECT_EQ(empty_map.Size(), 0);
}

TEST_F(MapTest, CopyConstructor) {
  Map<int, int> copy_map = map_;
  EXPECT_FALSE(copy_map.Empty());
  EXPECT_EQ(copy_map.Size(), 6);

  Map<int, int> copy_map_two(copy_map);
  EXPECT_FALSE(copy_map.Empty());
  EXPECT_EQ(copy_map.Size(), 6);
}

TEST_F(MapTest, AssignmentOperator) {
  Map<int, int> copy_map;
  copy_map = map_;
  EXPECT_FALSE(copy_map.Empty());
  EXPECT_EQ(copy_map.Size(), 6);
}

TEST_F(MapTest, Insert) {
  EXPECT_EQ(map_.Size(), 6);
  map_.Insert(std::make_pair(0, 1));
  EXPECT_EQ(map_.Size(), 6);
  auto [it, flag] = map_.Insert(std::make_pair(10, 1));
  EXPECT_TRUE(flag);
  EXPECT_EQ(map_.Size(), 7);
  EXPECT_EQ(it->first, 10);
  EXPECT_EQ(it->second, 1);
  flag = map_.Insert(std::make_pair(10, 1)).second;
  EXPECT_FALSE(flag);
}

TEST_F(MapTest, SquareBracketOperator) {
  EXPECT_EQ(map_.Size(), 6);
  auto it = map_.Find(0);
  EXPECT_EQ(it->second, 10);
  map_[0] = 1;
  it = map_.Find(0);
  EXPECT_EQ(it->second, 1);
  EXPECT_EQ(map_.Size(), 6);
  map_[10] = 1;
  EXPECT_EQ(map_.Size(), 7);
}

TEST_F(MapTest, Find) {
  Map<int, int> empty_map;
  EXPECT_TRUE(empty_map.Empty());
  EXPECT_EQ(empty_map.Size(), 0);
}

TEST_F(MapTest, Size) {
  EXPECT_EQ(map_.Size(), 6);
  map_.Insert(std::make_pair(0, 1));
  EXPECT_EQ(map_.Size(), 6);
  map_.Insert(std::make_pair(10, 1));
  EXPECT_EQ(map_.Size(), 7);
  map_.Clear();
  EXPECT_EQ(map_.Size(), 0);
  map_[0] = 1;
  EXPECT_EQ(map_.Size(), 1);
}

TEST_F(MapTest, Clear) {
  EXPECT_FALSE(map_.Empty());
  map_.Clear();
  EXPECT_TRUE(map_.Empty());
  map_.Insert({0, 1});
  EXPECT_FALSE(map_.Empty());
  map_.Clear();
  EXPECT_TRUE(map_.Empty());
  map_.Clear();
  EXPECT_TRUE(map_.Empty());
}

TEST_F(MapTest, Empty) {
  EXPECT_FALSE(map_.Empty());
  map_.Clear();
  EXPECT_TRUE(map_.Empty());
  map_.Insert({0, 1});
  EXPECT_FALSE(map_.Empty());
  map_.Clear();
  EXPECT_TRUE(map_.Empty());
  map_.Clear();
  EXPECT_TRUE(map_.Empty());
}

TEST_F(MapTest, Begin) {
  auto it = map_.Begin();
  EXPECT_EQ(it->first, 0);
  EXPECT_EQ(it->second, 10);
}

TEST_F(MapTest, End) {

}


TEST_F(MapTest, CBegin) {
  auto it = map_.CBegin();
  EXPECT_EQ(it->first, 0);
  EXPECT_EQ(it->second, 10);
}

TEST_F(MapTest, CEnd) {

}

}
