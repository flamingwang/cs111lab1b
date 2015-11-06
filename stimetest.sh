#Simple Case
time sleep 0.5

#After a semicolon command
echo hi; time ls -l | od -c | sort > /dev/null

#No time test
echo hello1; echo hello2

#Inside a subshell command
(time sleep 0.5)

#After && command
ls -al | od -c | od -c | sort > /dev/null && time sleep 0.5

#After || command
false || time sleep 0.5

#Special Case 1
time (ls -al | od -c | od -c | sort > /dev/null && sleep 0.5)

#Special Case 2
time (sleep 1)

#Funny Case
time time time sleep 1

#Funny Case v2
time ( time ( time ( time ( sleep 1) ) ) )