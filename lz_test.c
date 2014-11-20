#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include "lz.h"

int init_suite1(void)
{
    return 0;
}

int clean_suite1(void)
{
    return 0;
}

void testSUFFIX_ARRAY(void)
{
    char* string = "hello";
    char* array[5] = {"hello", "ello", "llo", "lo", "o"};

    Suffix_array* s_array = make_suffix_array(string);
    int i;

    for(i = 0; i < strlen(string); i++)
        CU_ASSERT_STRING_EQUAL(array[i], s_array[i]);
    CU_ASSERT(0 == 0);
}

int main()
{
    CU_pSuite pSuite = NULL;

    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("suite1", init_suite1, clean_suite1);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite,
                             "testing suffix array creation",
                             testSUFFIX_ARRAY)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
