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
