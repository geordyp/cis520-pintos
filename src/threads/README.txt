
Changes made for Project 0:
1. The first task was to track down alarm-multiple by running 
~/Documents/cis520/pintos/src/tests$ grep -rni test_alarm_multiple . | sort
It will list where the files are at and the line number
./threads/alarm-wait.c:22:test_alarm_multiple (void) 
./threads/tests.c:15:    {"alarm-multiple", test_alarm_multiple},
./threads/tests.h:9:extern test_func test_alarm_multiple;

2. Then, I add a new definition of alarm_mega alarm-wait.c, tests.c, tests.h 
   For the function definition at alarm-wait.c: looking at the pattern, I did (5,70) 

3. Once all that is done, I verified it by running grep -rni test_alarm_mega  . | sort
./alarm-wait.c:34:test_alarm_mega (void)
./tests.c:17:    {"alarm-mega", test_alarm_mega},
./tests.h:10:extern test_func test_alarm_mega;
   Looks good!

4. Then go back to pintos/src/threads, run the make command to rebuild it, then run.
title: Project 0
author: geordyp
desc: The changes required to incorporate test_alarm_mega

The following files were changed in the src/tests/threads/ directory:
- tests.c
  'alarm-mega' was added to the 'tests' list in this class. The name,
  'alarm-mega', was mapped to the test function, 'test_alarm_mega'.
- tests.h
  Contains the definitions of the different test functions. There's
  'msg', 'fail', 'pass' then all the external test functions like
  'test_alarm_multiple' and 'test_alarm_mega'. 'test_alarm_mega'
  needed to be added.
- alarm-wait.c
  Added the 'test_alarm_mega' function which is structured just like
  'test_alarm_multiple' only 'test_alarm_mega' creates 5 threads to
  sleep 70 times each instead of just 7 [test_sleep(5, 70)].
- Make.tests
  'alarm-mega' was added to the list of test names. The test would
  still run even without this change.
