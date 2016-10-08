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
