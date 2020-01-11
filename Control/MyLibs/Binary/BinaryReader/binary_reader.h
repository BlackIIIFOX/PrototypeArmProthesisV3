#ifndef __BINARY_READER_H
#define __BINARY_READER_H

#include "stdbool.h"
#include "stdint.h"
#include "stdbool.h"

/* TODO: ����� �������� ���������� ������, ���� ���� ����� �� ����������� ������ ��� ������. */

typedef struct {
  unsigned int CurrentReadPosition;
  uint8_t* BinaryDataArray;
} BinaryReaderStruct;
  

/**
* @brief �������������� ����� ��������� BinaryReaderStruct.
* @param binaryData ������� ����� � ���� ������� ����.
*/
BinaryReaderStruct* BinaryReader_Init(uint8_t* binaryData);

/**
* @brief ����������� ��� �������, ������������ ���������� BinaryReaderStruct.
* @param reader ������������ BinaryReaderStruct.
*/
void BinaryReader_Destroy(BinaryReaderStruct* reader);

/**
* @brief ��������� �������� Boolean �� �������� ������ � ���������� ������� ������� �
* ������ �� ���� ���� ������.
* @param reader ������, �������� ����� ��� ������.
* @retval �������� true, ���� ���� �� ����� ����; � ��������� ������ � �������� false.
*/
bool BinaryReader_ReadBoolean(BinaryReaderStruct* reader);

/**
* @brief ��������� �� �������� ������ ��������� ���� � ���������� ������� ������� � ������
* �� ���� ���� ������.
* @param reader ������, �������� ����� ��� ������.
* @retval ��������� ����, ��������� �� �������� ������.
*/
uint8_t BinaryReader_ReadByte(BinaryReaderStruct* reader);

/**
* @brief ��������� ��������� ���������� ������ �� �������� ������ � ������ ������ � ����������
* ������� ������� �� ��� ���������� ������.
* @param reader ������, �������� ����� ��� ������.
* @retval ������ ������, � ������� ���������� ������, ��������� �� �������� ������.
*/
uint8_t BinaryReader_ReadBytes(BinaryReaderStruct* reader, int count);

/**
* @brief ��������� ��������� ���� �� �������� ������ � �������� ������� ������� � ������
* �� ���� ���� ������. �������� ������ ����� �������� ������ ���� ����.
* @param reader ������, �������� ����� ��� ������.
* @retval ������, ��������� �� �������� ������.
*/
char BinaryReader_ReadChar(BinaryReaderStruct* reader);

/**
* @brief ��������� ������ �� �������� ������. ������ ������������ ��������� ����� ������,
* ������� ������������ ��� ������� ����� ����������� �����.
* @param reader ������, �������� ����� ��� ������.
* @retval ����������� ������.
*/
char* BinaryReader_ReadString(BinaryReaderStruct* reader);

/**
* @brief ��������� ����� ����� ��� ����� ������ 2 ����� � ������� � ������ �������� ������,
* �� �������� ������ � ���������� ������� ������� � ������ �� ��� ����� ������.
* @param reader ������, �������� ����� ��� ������.
* @retval ����� ����� ��� ����� ������ 2 �����, ��������� �� �������� ������.
*/
char BinaryReader_ReadUInt16(BinaryReaderStruct* reader);

/**
* @brief ��������� ����� ����� ��� ����� ������ 4 ����� � ������� � ������ �������� ������,
* �� �������� ������ � ���������� ������� ������� � ������ �� ������ ����� ������.
* @param reader ������, �������� ����� ��� ������.
* @retval ����� ����� ��� ����� ������ 4 �����, ��������� �� �������� ������.
*/
char BinaryReader_ReadUInt32(BinaryReaderStruct* reader);

/**
* @brief ��������� ����� ����� �� ������ ������ 2 ����� � ������� � ������ �������� ������,
* �� �������� ������ � ���������� ������� ������� � ������ �� ��� ����� ������.
* @param reader ������, �������� ����� ��� ������.
* @retval ����� ����� �� ������ ������ 2 �����, ��������� �� �������� ������.
*/
char BinaryReader_BinaryReader_ReadInt16(BinaryReaderStruct* reader);

/**
* @brief ��������� ����� ����� �� ������ ������ 4 ����� �� �������� ������ � ����������
* �� �������� ������ � ���������� ������� ������� � ������ �� ������ ����� ������.
* @param reader ������, �������� ����� ��� ������.
* @retval ����� ����� �� ������ ������ 2 �����, ��������� �� �������� ������.
*/
char BinaryReader_ReadInt32(BinaryReaderStruct* reader);

/**
* @brief ��������� ����� � ��������� ������� ������ 8 ���� �� �������� ������ � ����������
* ������� ������� � ������ �� ������ ���� ������.
* @param reader ������, �������� ����� ��� ������.
* @retval ����� � ��������� ������� ������ 8 ����, ��������� �� �������� ������.
*/
char BinaryReader_ReadDouble(BinaryReaderStruct* reader);

/**
* @brief ��������� ����� � ��������� ������� ������ 4 ����� �� �������� ������ � ����������
* ������� ������� � ������ �� ������ ����� ������.
* @param reader ������, �������� ����� ��� ������.
* @retval ����� � ��������� ������� ������ 4 �����, ��������� �� �������� ������.
*/
char BinaryReader_ReadSingle(BinaryReaderStruct* reader);

#endif