/**
 * @file main.cpp
 * @author Benjamin Carter and Trevor Pope
 * @brief The FancyShell Program.
 * @version 0.1
 * @date 2024-02-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <PrettyConsole/console.h>
#include <PrettyConsole/inputHandler.h>
#include <PrettyConsole/rectangle.h>
#include <PrettyConsole/line.h>
#include <PrettyConsole/point.h>
#include <stack>
#include <string>
#include <queue>
#include <thread>
#include <shell.h>
#include <ctime>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fileLoader.h>

volatile bool displayUpdated = false;
volatile bool stopProgram = false;

std::thread* processingThread;

struct CommandStatus
{
    std::string command;
    std::string response;
    bool isDone = true;
    bool isDisplay = true;
    int status = 0;
};

std::queue<CommandStatus*> commandQueue;
std::queue<CommandStatus*> displayQueue;
std::vector<std::string> inputHistory;

int currentRow = 0;
int history = -1;

/**
 * @brief Clear the console and repaint the UI for the response box
 * 
 * @param myConsole 
 */
void clear(Console &myConsole)
{
    // clear!
    Style backgroundColor = Style();
    backgroundColor.setBackgroundColor(20, 20, 20);
    backgroundColor.turnOffBlink();
    Rectangle2D background = Rectangle2D(1, 1,myConsole.getWidth() - 2,myConsole.getHeight() - 7);
    background.setFill(backgroundColor);
    
    myConsole.addShape(&background);
}

/**
 * @brief The command queue consumer. Executes the Shell commands
 * 
 * @param myConsole 
 */
void commandProcessor(Console* myConsole)
{
    while(!commandQueue.empty())
    {
        CommandStatus* curCommand = commandQueue.front();

        curCommand->response = "";
        Shell sh = Shell();
        if(curCommand->command == "clear")
        {
            clear(*(myConsole));
            currentRow = 0;
            curCommand->status = 0;
        }
        else if(curCommand->command.find("load") == 0)
        {
            if(curCommand->status >= 0)
            {
                curCommand->response = "Running program: " + std::to_string(curCommand->status) + " commands.";
                curCommand->status = 0;
            }
            else
            {
                curCommand->response = "Script not found!";
            }
        }
        else if(curCommand->command.find("exit") == 0)
        {
            stopProgram = true;
            curCommand->status = 0;
            curCommand->response = "Press any key to exit program!";
        }
        else
        {
            CommandResponse cr = sh.callCommand(curCommand->command);
            curCommand->response = cr.response;
            curCommand->status = cr.returnCode;
        }

        curCommand->isDisplay = false;
        curCommand->isDone = true;
        displayQueue.push(curCommand);
        commandQueue.pop();
    }

}

/**
 * @brief Display Queue consumer. Displays command results onto console
 * 
 * @param myConsole 
 * @param repeat 
 */
void displayCommandResult(Console &myConsole, int repeat=0)
{
    while(!displayQueue.empty())
    {
        CommandStatus* curCommand = displayQueue.front();
        Style commandBarColor = Style();
        if(curCommand->status == 0)
        {
            commandBarColor.setBackgroundColor(60, 255, 60);
            commandBarColor.setTextColor(0,0,0);
        }
        else if(curCommand->status == 1)
        {
            commandBarColor.setBackgroundColor(255, 255, 60);
            commandBarColor.setTextColor(0,0,0);
        }
        else
        {
            commandBarColor.setBackgroundColor(255, 60, 60);
            commandBarColor.setTextColor(0,0,0);
        }

        Style error = Style();
        error.setBackgroundColor(255,200,60);
        error.setTextColor(255,0,0);
        
        if(currentRow > myConsole.getHeight() - 10)
        {
            clear(myConsole);
            currentRow = 0;
        }

        int lastRowPaint = -1;
        int offset = 0;
        for(int i = 0; i < curCommand->command.size(); i++)
        {
            int row = i / (myConsole.getWidth() - 3);
            int col = i % (myConsole.getWidth() - 3);
            char inc = curCommand->command.at(i);
            
            if(row + 1 + currentRow > myConsole.getHeight() - 10)
            {
                clear(myConsole);
                currentRow = 0;
                return displayCommandResult(myConsole, 1);
            }
            if(lastRowPaint != row)
            {
                int barWidth = myConsole.getWidth() - 3;
                if(curCommand->command.size() + 2 < myConsole.getWidth() - 3)
                {
                    barWidth = curCommand->command.size() + 1;
                }
                Line2D commandBar = Line2D(1, row + 1 + currentRow, 1 + barWidth, row + 1 + currentRow);
                commandBar.setFill(commandBarColor);
                myConsole.addShape(&commandBar);
                lastRowPaint = row;
            }
            myConsole.putString(std::string(1,inc),col + 2,row + 1 + currentRow);
        }
        int textRow = lastRowPaint + 2 + currentRow;
        int textCol = 0;

        for(int i = 0; i < curCommand->response.size(); i++)
        {
            char inc = curCommand->response.at(i);
            if(inc == 10)
            {
                textCol = 0;
                textRow += 1;
                continue;
            }
            if(inc == 9)
            {
                inc = ' ';
                for(int i = 0; i < 4; i++)
                {
                    myConsole.putString(std::string(1, inc),textCol + 1, textRow);
                    textCol++;
                    if(textCol > myConsole.getWidth() - 3)
                    {
                        textCol = 0;
                        textRow++;
                    }
                }
            }
            if(textRow > myConsole.getHeight() - 10)
            {
                Line2D errorLine = Line2D(textCol + 1,textRow,textCol + 1 + 19, textRow);
                errorLine.setFill(error);
                myConsole.addShape(&errorLine);
                myConsole.putString("<<< Output cut >>>>",textCol + 1, textRow);

                break;
            }
            myConsole.putString(std::string(1, inc),textCol + 1, textRow);
            textCol++;
            if(textCol > myConsole.getWidth() - 3)
            {
                textCol = 0;
                textRow++;
            }
            if(textRow > myConsole.getHeight() - 10)
            {
                Line2D errorLine = Line2D(textCol + 1,textRow,textCol + 1 + 19, textRow);
                errorLine.setFill(error);
                myConsole.addShape(&errorLine);
                myConsole.putString("<<< Output cut >>>>",textCol + 1, textRow);

                break;
            }
        }
        currentRow = textRow;
        displayQueue.pop();
    }

}

Console* exitConsole;
InputHandler* exitKeyboard;

/**
 * @brief Push command onto the command queue for execution
 * 
 * @param command 
 */
void manuallyPushCommand(std::string command)
{
    CommandStatus* currentCommand = new CommandStatus();
    currentCommand->command = command;
    currentCommand->isDone = false;
    currentCommand->status = 0;
    commandQueue.push(currentCommand);
}

/**
 * @brief Called when Ctrl+C is pressed. Runs exit command.
 * 
 * @param s 
 */
void exitHandler(int s)
{
    manuallyPushCommand("exit");
    commandProcessor(exitConsole);
}

/**
 * @brief Main function. Contains initial draw and main loop.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char** argv)
{
    // exit section. from https://stackoverflow.com/questions/1641182/how-can-i-catch-a-ctrl-c-event
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = exitHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
   
    // end exit section. ^^^^^^^^^^^^^^^^^^

    commandQueue = queue<CommandStatus*>();
    displayQueue = queue<CommandStatus*>();
    inputHistory = std::vector<std::string>();

    Console myConsole = Console();
    exitConsole = &myConsole;
    
    if(!myConsole.getSupport())
    {
        std::cout << "Platform not supported!" << std::endl;
    }

    InputHandler keyboard;
    exitKeyboard = &keyboard;
    int height = myConsole.getHeight();
    int width = myConsole.getWidth();

    // Clear the screen
    myConsole.clear(); 
    myConsole.setTitle("Fancy Shell - By Benjamin Carter and Trevor Pope");

    // 0, 0 is top left    
    myConsole.render();

    clear(myConsole);

    /* UI CREATION SECTION */

    Style commandColor = Style();
    commandColor.setBackgroundColor(60, 60, 60);
    Rectangle2D commandBox = Rectangle2D(1,height- 4, width-2, 4);
    commandBox.setFill(commandColor);
    myConsole.addShape(&commandBox);
    
    Style pathLineColor = Style();
    pathLineColor.setBackgroundColor(30, 54, 96);
    pathLineColor.setTextColor(200, 200, 0);
    Line2D pathLine = Line2D(1,height - 5, width-2, height - 5);
    pathLine.setFill(pathLineColor);
    myConsole.addShape(&pathLine);

    Style userBoxColor = Style();
    userBoxColor.setBackgroundColor(146, 93, 22);
    userBoxColor.setTextColor(255, 255, 255);
    Line2D userLine = Line2D(1,0, 20, 0);
    userLine.setFill(userBoxColor);
    myConsole.addShape(&userLine);

    Line2D userLine2 = Line2D(1,height - 6, 20, height - 6);
    userLine2.setFill(userBoxColor);
    myConsole.addShape(&userLine2);

    Shell sh = Shell();
    std::string username = sh.whoami().response;
    myConsole.putString(username,userLine.getAnchorX() + 1,userLine.getAnchorY());
    myConsole.putString(username,userLine2.getAnchorX() + 1,userLine2.getAnchorY());

    std::string path = sh.getCurrentDirectory().response;
    myConsole.setTitle("Fancy Shell - By Benjamin Carter and Trevor Pope - " + path);
    while(path.size() > width - 5)
    {
        path = path.substr(1);
    }
    myConsole.putString(path,pathLine.getAnchorX()+1, pathLine.getAnchorY());

    int typerX = 1;
    int typerY = height - 4;
    myConsole.putString("$ ",typerX, typerY);

    myConsole.render();

    /* END UI CREATION SECTION */

    /* PROCESS ARGUMENTS SECTION */
    std::string arguments = "load ";
      
    keyboard.startListening();
    keyboard.clear();
    for(int i = 1; i < argc; i++)
    {
        char* arg = argv[i];
        arguments += arg;
        arguments += ' ';
    }
    if(argc > 1)
    {
        for(char x : arguments.substr(0,arguments.size()-1))
        {
            keyboard.simPress(x);
        }
        keyboard.simPress('\n');
    }

    /* END PROCESS ARGUMENTS SECTION */

    Style cursorColor = Style();
    cursorColor.setBackgroundColor(0,255,0);

    /*****************
     * Main Loop!
     ****************/
    
    typerX++;
    stack<char> textbox = stack<char>();

    unsigned long timestamp = time(NULL);
    bool drawCursor = false;
    while(true)
    {
        /* Blinking Cursor section. */
        if(time(NULL) != timestamp)
        {
            timestamp = time(NULL);
            drawCursor = !drawCursor;
            
            int typerX_TEMP = typerX;
            int typerY_TEMP = typerY;
            typerX_TEMP++;
            if(typerX_TEMP > width - 2)
            {
                typerX_TEMP = 1;
                typerY_TEMP += 1;
            } 
            if(typerY_TEMP >= height - 1)
            {
                // FULL
            }
            else
            {
                Point2D cursor = Point2D(typerX_TEMP, typerY_TEMP);
                if(!drawCursor)
                {
                    cursor.setFill(commandColor);
                }
                else
                {
                    cursor.setFill(cursorColor);
                }
                myConsole.addShape(&cursor);
                displayUpdated = true;
            }
        }
        
        /* Keyboard section. */
        if(keyboard.isAvailable())
        {
            char inc = keyboard.read();
            if(inc == 24 | inc == 3) // Ctrl+X
            {
                exitHandler(0);
            }
            /* user presses arrows */
            else if(inc == 27 && inputHistory.size() > 0)
            {
                bool stop = false;
                while(keyboard.isAvailable() < 2)
                {
                    if(keyboard.isAvailable() && keyboard.peek() != 91)
                    {
                        stop = true;
                        break;
                    }
                }
                if(stop)
                {
                    continue;
                }
                char inc2 = keyboard.read();
                char inc3 = keyboard.read();
                if(inc3 == 65) // down arrow
                {
                    if(history + 1 >= inputHistory.size())
                    {
                        continue;
                    }
                    history++;

                }
                else if(inc3 == 66)
                {
                    // up! arrow
                    if(history <= 0)
                    {
                        continue;
                    }
                    history--;
                }
                else
                {
                    // left and right
                    continue;
                }
                // clear!
                myConsole.addShape(&commandBox);
                typerX = 1;
                typerY = height - 4;
                myConsole.putString("$ ",typerX, typerY);
                typerX++;

                // "retype it"
                keyboard.clear();
                while(!textbox.empty())
                {
                    textbox.pop();
                }
                std::string s = inputHistory[history];
                for(int i = 0; i< s.size(); i++)
                {
                    keyboard.simPress(s.at(i));
                }
            }
            /* user presses a letter/number */
            else if(inc > 31 && inc < 127) // letter/number
            {
                int originalX = typerX;
                int originalY = typerY;
                typerX++;
                if(typerX > width - 2)
                {
                    typerX = 1;
                    typerY += 1;
                } 
                if(typerY >= height - 1)
                {
                    typerX = originalX;
                    typerY = originalY;
                    continue; // FULL
                }
                textbox.push(inc);

                Point2D cursor = Point2D(typerX, typerY);
                cursor.setFill(commandColor);
                cursor.putChar(inc);                
                myConsole.addShape(&cursor);

                // myConsole.putString(std::string(1,inc),typerX, typerY, commandColor);
                displayUpdated = true;
            }
            /* user presses backspace */
            else if(inc == 127 && !textbox.empty()) // backspace
            {
               
                int typerX_TEMP = typerX;
                int typerY_TEMP = typerY;
                typerX_TEMP++;
                if(typerX_TEMP > width - 2)
                {
                    typerX_TEMP = 1;
                    typerY_TEMP += 1;
                } 
                if(typerY_TEMP >= height - 1)
                {
                    // FULL
                }
                else
                {
                    Point2D cursor = Point2D(typerX_TEMP, typerY_TEMP);
                    cursor.setFill(commandColor);
                    myConsole.addShape(&cursor);
                }
                Point2D cursor = Point2D(typerX, typerY);
                cursor.setFill(commandColor);               
                myConsole.addShape(&cursor);
                typerX--;
                if(typerX < 1)
                {
                    typerY--;
                    typerX = width - 2;
                }
                textbox.pop();
                displayUpdated = true;
            }
            /* user presses enter. push command to queue */
            else if(inc == 10 && commandQueue.empty() && displayQueue.empty()) // enter!
            {
                // process command!
                std::vector<std::string> semicolonSections;
                std::string command = std::string();
                while(!textbox.empty())
                {
                    // convert stack to string!
                    char c = textbox.top();
                    textbox.pop();
                    command = std::string(1, c) + command;
                    
                }
                std::string dump = "";
                for(int i = 0; i < command.size(); i++)
                {
                    if(command.at(i) == ';')
                    {
                        semicolonSections.push_back(dump);
                        dump = "";
                        continue;
                    }
                    if(dump == "" && command.at(i) == ' ')
                    {
                        continue;
                    }
                    dump += command.at(i);
                }
                semicolonSections.push_back(dump);
                for(int i = 0; i < semicolonSections.size(); i++)
                {
                    std::string section = semicolonSections[i];
                    if(section == "" || section == " ")
                    {
                        continue;
                    }
                    if(section.find("load") == 0)
                    {
                        std::string fname = section.substr(5);
                        FileLoader fload = FileLoader(fname.c_str());
                        int feedback = fload.load();
                        std::vector<std::string> cmds = fload.getCommands();
                        
                        CommandStatus* currentCommand = new CommandStatus();
                        currentCommand->command = section;
                        currentCommand->isDone = false;
                        currentCommand->status = feedback;
                        commandQueue.push(currentCommand);

                        for(int c = 0; c < cmds.size(); c++)
                        {
                            manuallyPushCommand(cmds[c]);
                        }
                    }
                    else
                    {
                        manuallyPushCommand(section);
                    }
                }
                
                inputHistory.insert(inputHistory.begin(), command);
                history = -1;

                processingThread = new std::thread(commandProcessor,&myConsole);
                myConsole.addShape(&commandBox);

                myConsole.addShape(&pathLine);
                std::string username = sh.whoami().response;
                myConsole.putString(username,userLine.getAnchorX() + 1,userLine.getAnchorY());
                myConsole.putString(username,userLine2.getAnchorX() + 1,userLine2.getAnchorY());

                std::string path = sh.getCurrentDirectory().response;
                myConsole.setTitle("Fancy Shell - By Benjamin Carter and Trevor Pope - " + path);
                while(path.size() > width - 5)
                {
                    path = path.substr(1);
                }
                myConsole.putString(path,pathLine.getAnchorX()+1, pathLine.getAnchorY());
                
                typerX = 1;
                typerY = height - 4;
                myConsole.putString("$ ",typerX, typerY);
                typerX++;
                displayUpdated = true;
                
            }
        } 

        /* if no commands, run display queue consumer */
        if(commandQueue.empty() && !displayQueue.empty())
        {
            displayCommandResult(myConsole);
            displayUpdated = true;
        }       
        
        /* if display updated and keyboard is done, render! */
        if(displayUpdated && !keyboard.isAvailable())
        {
            myConsole.render();    
            displayUpdated = false;
        }
        /* if program stopped, quit main loop*/
        if(stopProgram)
        {
            break;
        }
    }

    keyboard.stopListening();
    myConsole.clear();
    myConsole.setTitle("Ubuntu");
    myConsole.render();
    /* reset ascii console handling and reenable cursor */
    std::cout << "exit!\033[0m\033[?25h";
    system("clear");

    
}

