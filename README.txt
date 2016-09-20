Changes made to implement test-mega:

-Added file test-mega.ck to pintos/src/tests/threads, based on test-multiple.ck
-Added function test_alarm_mega to alarm_wait.c in pintos/src/tests/threads, based on function test_alarm_multiple in the same file
-Added line "extern test_func test_alarm_mega" to tests.h in pintos/src/tests/threads
-Added alarm-mega to list of test names in Make.tests in pintos/src/tests/threads
-Added line "4	alarm-mega" to Rubric.alarm
-Added line "{"alarm-mega", test_alarm_mega}" to tests[] field in tests.c in pintos/src/tests/threads
