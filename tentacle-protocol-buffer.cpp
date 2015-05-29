#include "tentacle-protocol-buffer.hpp"
namespace tentacle {
  TentacleProtoBuf::TentacleProtoBuf(uint8_t *buffer, size_t bufferLength) {
    this->buffer = buffer;
    this->bufferLength = bufferLength;
  }

  unsigned int TentacleProtoBuf::writeStateMessage(const std::vector<Pin> &pins) {
    std::cout << "BUFFER: BEFORE " << buffer << std::endl;
    protobuf::MicrobluState message = {};
    // message.has_pins = true;
    message.pins.funcs.encode = &TentacleProtoBuf::pinEncode;
    message.pins.arg = (void*) &pins;
    // message.pins.funcs.decode = NULL;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferLength);
    bool status = pb_encode(&stream, protobuf::MicrobluState_fields, &message);
    unsigned int messageSize = stream.bytes_written;
    std::cout << "status: " << status << " " << PB_GET_ERROR(&stream) << std::endl;
    std::cout << "BUFFER: AFTER " << buffer << std::endl;
    std::cout << "MESSAGE SIZE " << messageSize << std::endl;
    return messageSize;
  }

  const std::vector<Pin> TentacleProtoBuf::readStateMessage(unsigned int messageSize) {
    protobuf::MicrobluState message = {};
    // message.pins.funcs.encode = NULL;
    std::cout << "IN READSTATEMESSAGE" << std::endl;
    message.pins.funcs.decode = &TentacleProtoBuf::pinDecode;
    pb_istream_t stream = pb_istream_from_buffer(buffer, messageSize);
    bool status = pb_decode(&stream, protobuf::MicrobluState_fields, &message);
    std::cout << "status: " << status << " " << PB_GET_ERROR(&stream) << std::endl;
    return std::vector<Pin>();
  }

  bool TentacleProtoBuf::pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
    std::cout << "PIN ENCODING IS HAPPENING" << std::endl;
    std::vector<Pin> *pins = (std::vector<Pin>*) *arg;
    bool fail = false;
    for(Pin pin : *pins) {
      protobuf::Pin protoBufPin;

      protoBufPin.number = pin.getPin();
      protoBufPin.value = pin.getState();
      std::cout << "PIN NUMBER: " << protoBufPin.number << " VALUE " << protoBufPin.value << std::endl;
      if (!pb_encode_tag_for_field(stream, field)) {
        return false;
      }

      if(!pb_encode_submessage(stream, protobuf::Pin_fields, &protoBufPin)) {
        return false;
      }
    }
    return true;
  }

  bool TentacleProtoBuf::pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg)
  {
    std::cout << "PIN DECODING IS HAPPENING" << std::endl;
    protobuf::Pin pin;

    if (!pb_decode(stream, protobuf::Pin_fields, &pin)) {
        return false;
    }
    std::cout << "PIN NUMBER " << pin.number << " VALUE " << pin.value << std::endl;
    return true;
  }
}
