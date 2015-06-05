#include "BufferStream.hpp"

BufferStream::BufferStream(uint8_t *buffer, int bufferLength) {
  this->buffer = buffer;
  this->bufferLength = bufferLength;
  this->readden = 0;
  this->written = 0;
}

int BufferStream::available() {
  return written - readden;
}

int BufferStream::read() {
  if (available() < 1) {
    return -1;
  }

  return buffer[readden++];
}

int BufferStream::peek() {
  return buffer[readden];
}

void BufferStream::flush() {
}

bool BufferStream::find(char *target) {
  return false;
}

bool BufferStream::find(uint8_t *target) {
  return false;
}

bool BufferStream::find(char *target, size_t length) {
  return false;
}

bool BufferStream::find(uint8_t *target, size_t length) {
  return false;
}

size_t BufferStream::readBytes( char *buffer, size_t length) {
  return readBytes( (uint8_t *) buffer, length);
}

size_t BufferStream::readBytes( uint8_t *buffer, size_t length) {
  if (available() < 1) {
    return 0;
  }

  memcpy(buffer, &this->buffer[readden], length);
  size_t newRead = readden + length;
  size_t written = length;

  if (readden + length > this->written) {
    written = this->written - readden;
    readden = this->written;
  } else {
    readden += length;
  }

  return written;
}

size_t BufferStream::write(const uint8_t *buffer, size_t length) {
  memcpy(&this->buffer[written], buffer, length);

  written += length;
  
  return length;
}
