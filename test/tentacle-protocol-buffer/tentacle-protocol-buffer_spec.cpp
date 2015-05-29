#include <limits.h>
#include <vector>

#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"
#include "tentacle-protocol-buffer.hpp"

using namespace std;
using namespace testing;
using namespace tentacle;

TEST(TentacleProtoBufTest, writeStateMessage_1) {
  uint8_t buffer[128];
  vector<tentacle::Pin> pins;
  pins.push_back(Pin(4, 0, 1));
  pins.push_back(Pin(40, 0, 0));
  TentacleProtoBuf tentacleProtoBuf(buffer, 128);
  unsigned int messageSize = tentacleProtoBuf.writeStateMessage(pins);
  vector<tentacle::Pin> pins2 = tentacleProtoBuf.readStateMessage(messageSize);

  EXPECT_EQ(pins2.size(), 2);
}

TEST(TentacleProtoBufTest, writeStateMessage_2) {
  uint8_t buffer[128];
  vector<tentacle::Pin> pins;
  pins.push_back(Pin(5, 0, 0));
  pins.push_back(Pin(8, 0, 1));
  TentacleProtoBuf tentacleProtoBuf(buffer, 128);
  unsigned int messageSize = tentacleProtoBuf.writeStateMessage(pins);
  vector<tentacle::Pin> pins2 = tentacleProtoBuf.readStateMessage(messageSize);

  EXPECT_EQ(pins2[0].getNumber(), pins[0].getNumber());
  EXPECT_EQ(pins2[0].getValue(), pins[0].getValue());

  EXPECT_EQ(pins2[1].getNumber(), pins[1].getNumber());
  EXPECT_EQ(pins2[1].getValue(), pins[1].getValue());
}
