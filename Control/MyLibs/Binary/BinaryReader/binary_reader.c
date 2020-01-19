#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "binary_reader.h"

BinaryReaderStruct* BinaryReader_Init(uint8_t* binaryData)
{
  BinaryReaderStruct* new_reader = malloc(sizeof(BinaryReaderStruct));
  memset(new_reader, 0, sizeof(BinaryReaderStruct));
  new_reader->CurrentReadPosition = 0;
  new_reader->BinaryDataArray = binaryData;
  return new_reader;
}

void BinaryReader_IncrementPosition(BinaryReaderStruct* reader, int increment)
{
  reader->CurrentReadPosition = reader->CurrentReadPosition + increment;
}

char* BinaryReader_ReadString(BinaryReaderStruct* reader)
{
  uint8_t stringSizeInBytes = BinaryReader_ReadByte(reader);
  char* string = (char*)BinaryReader_ReadBytes(reader, sizeof(char) * stringSizeInBytes);
  return string;
}

uint8_t BinaryReader_ReadByte(BinaryReaderStruct* reader)
{
  uint8_t byte = reader->BinaryDataArray[reader->CurrentReadPosition];
  BinaryReader_IncrementPosition(reader, sizeof(uint8_t));
  return byte;
}

bool BinaryReader_ReadBoolean(BinaryReaderStruct* reader)
{
  return (bool)BinaryReader_ReadByte(reader);
}

double BinaryReader_ReadDouble(BinaryReaderStruct* reader)
{
  BinaryReader_ReadBytes(reader, 8);
  return 0;
}

uint16_t BinaryReader_ReadUInt16(BinaryReaderStruct* reader)
{
  uint16_t result = BinaryReader_ReadByte(reader);
  result |= (BinaryReader_ReadByte(reader) << 8);
  return result;
}

uint8_t* BinaryReader_ReadBytes(BinaryReaderStruct* reader, int count)
{
  uint8_t* new_array = malloc(count* sizeof(uint8_t));
  memcpy(new_array, reader->BinaryDataArray + reader->CurrentReadPosition, count);
  BinaryReader_IncrementPosition(reader, count);
  return new_array;
}