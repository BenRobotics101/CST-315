# Fancy Shell

Creating a text-based graphical shell. 
Developed by Benjamin Carter and Trevor Pope

--- 

## How to build

Use make. You should just be able to run `make`.
This will output a executable called `FancyShell`.

## Features
FancyShell implements the following commands:
- ls
- df
- cd
- whoami
- pwd
- load*
- exit*
- clear*

Astrick denotes custom commands!

**Custom Commands:**

**load**

Load is a custom command for executing batch scripts. Run `load <filename>` which will run the batch file in the shell. You can also pass in a batch script when starting FancyShell: `./FancyShell test.sh`.

**exit**
To exit the program, type `exit`. You can also Ctrl+C or Ctrl+X.

**clear**
To clear console, type `clear`

It also supports other system commands. However, these commands must not be interactive and must not latch onto the console for rendering (as it will conflict with FancyShell).

> For instance, running `nano` will not work, nor will running `apt`. However, `echo`, `mkdir`, `touch` should work as they are non-interactive and do not require latching onto the console. Or like you can't run `./FancyShell` inside FancyShell.

Example Usage:
- `cd PrettyConsole` This will change into PrettyConsole
- `load script.sh` This will run script.sh


## Other Features
- Use up/down arrow to go back/forward in history
- blinking cursor
- use `;` to run multiple commands on input.
- color-coded feedback on command success

## Warnings
- It uses PrettyConsole for graphics. This means that you cannot resize the console window after starting the program! ***or else it will mess up the graphics display!***
- Make sure your console is at least 16 rows by 50 columns. *or else it will not work!!*