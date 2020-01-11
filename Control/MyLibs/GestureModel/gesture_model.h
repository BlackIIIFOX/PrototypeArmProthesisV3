#ifndef __GESTURE_MODEL_H
#define __GESTURE_MODEL_H

#include "stdbool.h"
#include "stdint.h"
#include "list.h"
#include <stdio.h>

/* ���������� � �����. */
typedef struct {
  
  /* ����� ��������� �����. */
  long TimeChange;
  
  /* ��������� ������������� �����. */
  bool IterableGesture;
  
  /* ���������� ���������� �����. */
  uint8_t NumberOfGestureRepetitions;
  
  /* ���-�� �������� � �����. */
  uint8_t NumberOfMotions;
  
} InfoGestureModel;

/* ��������� �������� �����. �������� ����� ������� � �����, � ��� �� ��������, 
����������� ����� ��������� ���������. */
typedef struct {
  
  /* ��������� ������������� ������ � �������� (0-180). */
  uint8_t PointerFinger;
  
  /* ��������� �������� ������ � �������� (0-180). */
  uint8_t MiddleFinger;
  
  /* ��������� ����������� ������ � �������� (0-180). */
  uint8_t RingFinder;
  
  /* ��������� ������� � �������� (0-180). */
  uint8_t LittleFinger;
  
  /* ��������� �������� ������ � �������� (0-180). */
  uint8_t ThumbFinger;
  
  /* ��������� ����� � �������� (0-350). */
  uint16_t StatePosBrush;
  
  /* �������� ����� ���������� � ������������. */
  uint16_t DelMotion;
  
} MotionModel;

/* ����, ����������� �������� */
typedef struct {
  
  /* Id ����� � Guid. */
  uint8_t Id[16];
  
  /* ��� ����� */
  char* Name;
  
  /* ���������� � �����. */
  InfoGestureModel InfoGesture;
  
  /* ��������� �������� �����. */
  list* ListMotions;
  
} GestureModel;

GestureModel* GestureModel_Init();

GestureModel* GestureModel_InitFromBinary(uint8_t* binaryData);

MotionModel* MotionModel_Init();

void MotionModel_Destroy(void *motion);

uint8_t* GestureModel_SerializeGesture(GestureModel* gesture);

GestureModel* GestureModel_DeserializeGesture(uint8_t* binaryData);

void GestureModel_Destroy(GestureModel* gesture);

 
#endif