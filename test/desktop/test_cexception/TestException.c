#include "unity.h"
#include "CException.h"

volatile int TestingTheFallback;
volatile int TestingTheFallbackId;
typedef int esp_err_t;

void setUp(void)
{
    CExceptionFrames[0].pFrame = NULL;
    TestingTheFallback = 0;
}

void tearDown(void)
{
}

void test_BasicTryDoesNothingIfNoThrow(void)
{
  int i = 0;
  CEXCEPTION_T e = 0x5a;

  Try
  {
    i += 1;
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Enter Catch If Not Thrown");
  }

  //verify that e was untouched
  TEST_ASSERT_EQUAL(0x5a, e);

  // verify that i was incremented once
  TEST_ASSERT_EQUAL(1, i);
}

void test_BasicThrowAndCatch(void)
{
  CEXCEPTION_T e;

  Try
  {
    Throw(0xBE);
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e)
  {
    //verify that e has the right data
    TEST_ASSERT_EQUAL(0xBE, e);
  }

  //verify that e STILL has the right data
  TEST_ASSERT_EQUAL(0xBE, e);
}

void test_BasicThrowAndCatch_WithMiniSyntax(void)
{
  CEXCEPTION_T e;

  //Mini Throw and Catch
  Try
    Throw(0xEF);
  Catch(e)
    TEST_ASSERT_EQUAL(0xEF, e);
  TEST_ASSERT_EQUAL(0xEF, e);

  //Mini Passthrough
  Try
    e = 0;
  Catch(e)
    TEST_FAIL_MESSAGE("I shouldn't be caught because there was no throw");

  TEST_ASSERT_EQUAL(0, e);
}

void test_VerifyVolatilesSurviveThrowAndCatch(void)
{
  volatile unsigned int VolVal = 0;
  CEXCEPTION_T e;

  Try
  {
    VolVal = 2;
    Throw(0xBF);
    TEST_FAIL_MESSAGE("Should Have Thrown An Error");
  }
  Catch(e)
  {
    VolVal += 2;
    TEST_ASSERT_EQUAL(0xBF, e);
  }

  TEST_ASSERT_EQUAL(4, VolVal);
  TEST_ASSERT_EQUAL(0xBF, e);
}

void HappyExceptionThrower(unsigned int ID)
{
  if (ID != 0)
  {
    Throw(ID);
  }
}

void test_ThrowFromASubFunctionAndCatchInRootFunc(void)
{
  volatile  unsigned int ID = 0;
  CEXCEPTION_T e;

  Try
  {

    HappyExceptionThrower(0xBA);
    TEST_FAIL_MESSAGE("Should Have Thrown An Exception");
  }
  Catch(e)
  {
    ID = e;
  }

  //verify that I can pass that value to something else
  TEST_ASSERT_EQUAL(0xBA, e);
  //verify that ID and e have the same value
  TEST_ASSERT_EQUAL(ID, e);
}

void HappyExceptionRethrower(unsigned int ID)
{
  CEXCEPTION_T e;

  Try
  {
    Throw(ID);
  }
  Catch(e)
  {
    switch (e)
    {
    case 0xBD:
      Throw(0xBF);
      break;
    default:
      break;
    }
  }
}

void test_ThrowAndCatchFromASubFunctionAndRethrowToCatchInRootFunc(void)
{
  volatile  unsigned int ID = 0;
  CEXCEPTION_T e;

  Try
  {
    HappyExceptionRethrower(0xBD);
    TEST_FAIL_MESSAGE("Should Have Rethrown Exception");
  }
  Catch(e)
  {
    ID = 1;
  }

  TEST_ASSERT_EQUAL(0xBF, e);
  TEST_ASSERT_EQUAL(1, ID);
}

void test_ThrowAndCatchFromASubFunctionAndNoRethrowToCatchInRootFunc(void)
{
  CEXCEPTION_T e = 3;

  Try
  {
    HappyExceptionRethrower(0xBF);
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Have Re-thrown Error (it should have already been caught)");
  }

  //verify that THIS e is still untouched, even though subfunction was touched
  TEST_ASSERT_EQUAL(3, e);
}

void test_ThrowAnErrorThenEnterATryBlockFromWithinCatch_VerifyThisDoesntCorruptExceptionId(void)
{
  CEXCEPTION_T e;

  Try
  {
    HappyExceptionThrower(0xBF);
    TEST_FAIL_MESSAGE("Should Have Thrown Exception");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(0xBF, e);
    HappyExceptionRethrower(0x12);
    TEST_ASSERT_EQUAL(0xBF, e);
  }
  TEST_ASSERT_EQUAL(0xBF, e);
}

void test_ThrowAnErrorThenEnterATryBlockFromWithinCatch_VerifyThatEachExceptionIdIndependent(void)
{
  CEXCEPTION_T e1, e2;

  Try
  {
    HappyExceptionThrower(0xBF);
    TEST_FAIL_MESSAGE("Should Have Thrown Exception");
  }
  Catch(e1)
  {
    TEST_ASSERT_EQUAL(0xBF, e1);
    Try
    {
      HappyExceptionThrower(0x12);
    }
    Catch(e2)
    {
      TEST_ASSERT_EQUAL(0x12, e2);
    }
    TEST_ASSERT_EQUAL(0x12, e2);
    TEST_ASSERT_EQUAL(0xBF, e1);
  }
  TEST_ASSERT_EQUAL(0x12, e2);
  TEST_ASSERT_EQUAL(0xBF, e1);
}

void test_CanHaveMultipleTryBlocksInASingleFunction(void)
{
  CEXCEPTION_T e;

  Try
  {
    HappyExceptionThrower(0x01);
    TEST_FAIL_MESSAGE("Should Have Thrown Exception");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(0x01, e);
  }

  Try
  {
    HappyExceptionThrower(0xF0);
    TEST_FAIL_MESSAGE("Should Have Thrown Exception");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(0xF0, e);
  }
}

void test_CanHaveNestedTryBlocksInASingleFunction_ThrowInside(void)
{
  int i = 0;
  CEXCEPTION_T e;

  Try
  {
    Try
    {
      HappyExceptionThrower(0x01);
      i = 1;
      TEST_FAIL_MESSAGE("Should Have Rethrown Exception");
    }
    Catch(e)
    {
      TEST_ASSERT_EQUAL(0x01, e);
    }
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Have Been Caught By Inside Catch");
  }

  // verify that i is still zero
  TEST_ASSERT_EQUAL(0, i);
}

void test_CanHaveNestedTryBlocksInASingleFunction_ThrowOutside(void)
{
  int i = 0;
  CEXCEPTION_T e;

  Try
  {
    Try
    {
      i = 2;
    }
    Catch(e)
    {
      TEST_FAIL_MESSAGE("Should Not Be Caught Here");
    }
    HappyExceptionThrower(0x01);
    TEST_FAIL_MESSAGE("Should Have Rethrown Exception");
  }
  Catch(e)
  {
    TEST_ASSERT_EQUAL(0x01, e);
  }

  // verify that i is 2
  TEST_ASSERT_EQUAL(2, i);
}

void test_AbilityToExitTryWithoutThrowingAnError(void)
{
    int i=0;
    CEXCEPTION_T e;

    Try
    {
        ExitTry();
        i = 1;
        TEST_FAIL_MESSAGE("Should Have Exited Try Before This");
    }
    Catch(e)
    {
        i = 2;
        TEST_FAIL_MESSAGE("Should Not Have Been Caught");
    }

    // verify that i is still zero
    TEST_ASSERT_EQUAL(0, i);
}

void test_AbilityToExitTryWillOnlyExitOneLevel(void)
{
    int i=0;
    CEXCEPTION_T e;
    CEXCEPTION_T e2;

    Try
    {
        Try
        {
            ExitTry();
            TEST_FAIL_MESSAGE("Should Have Exited Try Before This");
        }
        Catch(e)
        {
            TEST_FAIL_MESSAGE("Should Not Have Been Caught By Inside");
        }
        i = 1;
    }
    Catch(e2)
    {
        TEST_FAIL_MESSAGE("Should Not Have Been Caught By Outside");
    }

    // verify that we picked up and ran after first Try
    TEST_ASSERT_EQUAL(1, i);
}

void test_function_should_check_if_cexceptions_work_with_esp_err_type (void) {
  //given
  esp_err_t esp_err = -1;
  int i = 0;

  //when
  Try 
  {
    i += 1;
    Throw(esp_err);
  }
  Catch (esp_err) {
    i += 1;
  }

  //then

  TEST_ASSERT_EQUAL_INT(2, i);
  TEST_ASSERT_EQUAL_INT(-1, esp_err);
}

void test_function_should_check_if_cexceptions_work_with_esp_err_type_and_dont_catch (void) {
  //given
  esp_err_t esp_err = 123;
  int i = 0;

  //when
  Try 
  {
    i += 1;
  }
  Catch (esp_err) {
    i += 1;
    TEST_FAIL_MESSAGE("Should Not Enter Catch If Not Thrown");
  }

  //then

  TEST_ASSERT_EQUAL_INT(1, i);
  TEST_ASSERT_EQUAL_INT(123, esp_err);
} 

void test_function_should_check_if_cexceptions_catch_in_try_block(void) {
  //given
  esp_err_t esp_err = 0;
  volatile int i = 0;

  //when
  Try 
  {
    i += 1;
    esp_err = 123;
  }
  Catch (esp_err) {
    i += 1;
    TEST_FAIL_MESSAGE("Should Not Enter Catch If Not Thrown");
  }

  //then

  TEST_ASSERT_EQUAL_INT(1, i);
  TEST_ASSERT_EQUAL_INT(123, esp_err);
}

/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_AbilityToExitTryWillOnlyExitOneLevel);
  RUN_TEST(test_AbilityToExitTryWithoutThrowingAnError);
  RUN_TEST(test_CanHaveNestedTryBlocksInASingleFunction_ThrowOutside);
  RUN_TEST(test_CanHaveMultipleTryBlocksInASingleFunction);
  RUN_TEST(test_ThrowAnErrorThenEnterATryBlockFromWithinCatch_VerifyThatEachExceptionIdIndependent);
  RUN_TEST(test_ThrowAnErrorThenEnterATryBlockFromWithinCatch_VerifyThisDoesntCorruptExceptionId);
  RUN_TEST(test_ThrowAndCatchFromASubFunctionAndNoRethrowToCatchInRootFunc);
  RUN_TEST(test_ThrowFromASubFunctionAndCatchInRootFunc);
  RUN_TEST(test_VerifyVolatilesSurviveThrowAndCatch);
  RUN_TEST(test_BasicThrowAndCatch_WithMiniSyntax);
  RUN_TEST(test_BasicThrowAndCatch);
  RUN_TEST(test_BasicTryDoesNothingIfNoThrow);
  RUN_TEST(test_function_should_check_if_cexceptions_work_with_esp_err_type);
  RUN_TEST(test_function_should_check_if_cexceptions_work_with_esp_err_type_and_dont_catch);
  RUN_TEST(test_function_should_check_if_cexceptions_catch_in_try_block);

  return UNITY_END();
}


/**
  * main function for native dev-platform
  */
int main(void) {
  runUnityTests();
}
