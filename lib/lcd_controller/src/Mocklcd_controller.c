/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "Mocklcd_controller.h"

static const char* CMockString_cmock_arg1 = "cmock_arg1";
static const char* CMockString_init_lcd = "init_lcd";
static const char* CMockString_lcd_reset = "lcd_reset";

typedef struct _CMOCK_init_lcd_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  int Expected_cmock_arg1;

} CMOCK_init_lcd_CALL_INSTANCE;

typedef struct _CMOCK_lcd_reset_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  int ReturnVal;
  int Expected_cmock_arg1;

} CMOCK_lcd_reset_CALL_INSTANCE;

static struct Mocklcd_controllerInstance
{
  CMOCK_MEM_INDEX_TYPE init_lcd_CallInstance;
  CMOCK_MEM_INDEX_TYPE lcd_reset_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mocklcd_controller_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_MEM_INDEX_TYPE call_instance;
  call_instance = Mock.init_lcd_CallInstance;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_init_lcd);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  call_instance = Mock.lcd_reset_CallInstance;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_lcd_reset);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
}

void Mocklcd_controller_Init(void)
{
  Mocklcd_controller_Destroy();
}

void Mocklcd_controller_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

void init_lcd(int cmock_arg1)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_init_lcd_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_init_lcd);
  cmock_call_instance = (CMOCK_init_lcd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.init_lcd_CallInstance);
  Mock.init_lcd_CallInstance = CMock_Guts_MemNext(Mock.init_lcd_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_SET_DETAILS(CMockString_init_lcd,CMockString_cmock_arg1);
    UNITY_TEST_ASSERT_EQUAL_INT(cmock_call_instance->Expected_cmock_arg1, cmock_arg1, cmock_line, CMockStringMismatch);
  }
  UNITY_CLR_DETAILS();
}

void CMockExpectParameters_init_lcd(CMOCK_init_lcd_CALL_INSTANCE* cmock_call_instance, int cmock_arg1);
void CMockExpectParameters_init_lcd(CMOCK_init_lcd_CALL_INSTANCE* cmock_call_instance, int cmock_arg1)
{
  cmock_call_instance->Expected_cmock_arg1 = cmock_arg1;
}

void init_lcd_CMockExpect(UNITY_LINE_TYPE cmock_line, int cmock_arg1)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_init_lcd_CALL_INSTANCE));
  CMOCK_init_lcd_CALL_INSTANCE* cmock_call_instance = (CMOCK_init_lcd_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.init_lcd_CallInstance = CMock_Guts_MemChain(Mock.init_lcd_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_init_lcd(cmock_call_instance, cmock_arg1);
}

int lcd_reset(int cmock_arg1)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_lcd_reset_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_lcd_reset);
  cmock_call_instance = (CMOCK_lcd_reset_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.lcd_reset_CallInstance);
  Mock.lcd_reset_CallInstance = CMock_Guts_MemNext(Mock.lcd_reset_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_SET_DETAILS(CMockString_lcd_reset,CMockString_cmock_arg1);
    UNITY_TEST_ASSERT_EQUAL_INT(cmock_call_instance->Expected_cmock_arg1, cmock_arg1, cmock_line, CMockStringMismatch);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_lcd_reset(CMOCK_lcd_reset_CALL_INSTANCE* cmock_call_instance, int cmock_arg1);
void CMockExpectParameters_lcd_reset(CMOCK_lcd_reset_CALL_INSTANCE* cmock_call_instance, int cmock_arg1)
{
  cmock_call_instance->Expected_cmock_arg1 = cmock_arg1;
}

void lcd_reset_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_arg1, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_lcd_reset_CALL_INSTANCE));
  CMOCK_lcd_reset_CALL_INSTANCE* cmock_call_instance = (CMOCK_lcd_reset_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.lcd_reset_CallInstance = CMock_Guts_MemChain(Mock.lcd_reset_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_lcd_reset(cmock_call_instance, cmock_arg1);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

