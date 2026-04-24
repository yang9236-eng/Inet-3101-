# Module 6: Modifying ls and whoami

## Problem Statement 

So the goal of this assignment was to modify the source code of two Unix commands, 'ls' and 'whoami', from GNU coreutils.
For the first part, the ls command needed to be modified so that it displays output in long format by default, instead of the normal column format. This means it should show file permissions, number of links, owner, group, file size and last modified date without using the '-l' option. 
For the second part, the whoami command needed to be modified so that it prints the message "You are:" before displaying the current users username. 

### Describe the solution
To modify the ls command, i edits the `ls.c` file and forced the output format to always use the long format. This ensures that whenever the `ls` command is run, it behaves like `ls -l` by default.
For the `whoami` command, I modified the output line in `whoami.c`. Instead of printing only the username, I changed it to use `printf` so that it displays "You are:" followed by the username.
After these changes, I rebuilt the coreutils package and tested both commands to confirm that the new behavior worked.

## Pros and Cons of your solution
One advantaghe of this solution is that it directly changes the default behavior of the commands, so the user does not need to remeber additional flags like '-l' for ls. It also helps demonstrate how system level utilities can be modified and recompiled.
Another pro is that the changes are simple but effective, making it easy to understand and implement. 
A con is that changing the default behavior of common commands like 'ls' could confuse users who are used to the original output format. Also these changes only apply to the modified version of coreutils and do not affect the systems default commands. 
