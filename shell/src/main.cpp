
#include <iostream>
#include <PrettyConsole/console.h>
#include <PrettyConsole/inputHandler.h>
#include <PrettyConsole/rectangle.h>
#include <PrettyConsole/line.h>
#include <stack>
#include <string>
#include <queue>
#include <thread>
#include <shell.h>

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
        else if(curCommand->command == "exit")
        {
            stopProgram = true;
            curCommand->response = "Press any key to exit program!";
        }
        else
        {
            CommandRespoonse cr = sh.callCommand(curCommand->command);
            curCommand->response = cr.response;
            curCommand->status = cr.returnCode;
            curCommand->isDisplay = false;
            curCommand->isDone = true;
        }

        curCommand->isDisplay = false;
        curCommand->isDone = true;
        displayQueue.push(curCommand);
        commandQueue.pop();
    }

}

void displayCommandResult(Console &myConsole, int repeat=0)
{
    while(!displayQueue.empty())
    {
        CommandStatus* curCommand = displayQueue.front();
        displayQueue.pop();
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
        
        commandBarColor.turnOffBlink();
        
        int lastRowPaint = -1;
        int offset = 0;
        for(int i = 0; i < curCommand->command.size(); i++)
        {
            int row = i / (myConsole.getWidth() - 3);
            int col = i % (myConsole.getWidth() - 3);
            char inc = curCommand->command.at(i);
            
            if(row + 1 + currentRow > myConsole.getHeight() - 9)
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
            if(textRow > myConsole.getHeight() - 9)
            {
                clear(myConsole);
                currentRow = 0;
                if(repeat == 1)
                {
                    curCommand->response = "too big!";
                    return;
                }
                return displayCommandResult(myConsole, 1);
            }
            myConsole.putString(std::string(1, inc),textCol + 1, textRow);
            textCol++;
            if(textCol > myConsole.getWidth() - 3)
            {
                textCol = 0;
                textRow++;
            }
            if(textRow > myConsole.getHeight() - 9)
            {
                clear(myConsole);
                currentRow = 0;
                if(repeat == 1)
                {
                    curCommand->response = "too big!";
                    return;
                }
                return displayCommandResult(myConsole, 1);
            }
        }
        currentRow = textRow;
    }

}



int main()
{
    commandQueue = queue<CommandStatus*>();
    displayQueue = queue<CommandStatus*>();
    inputHistory = std::vector<std::string>();

    Console myConsole = Console();
    
    if(!myConsole.getSupport())
    {
        std::cout << "Platform not supported!" << std::endl;
    }

    InputHandler keyboard;
    int height = myConsole.getHeight();
    int width = myConsole.getWidth();

    // Clear the screen
    myConsole.clear(); 
    myConsole.setTitle("Fancy Shell - By Benjamin Carter and Trevor Pope");

    // 0, 0 is top left    
    myConsole.render();

    clear(myConsole);

    Style commandColor = Style();
    commandColor.setBackgroundColor(60, 60, 60);
    Rectangle2D commandBox = Rectangle2D(1,height- 4, width-2, 4);
    commandBox.setFill(commandColor);
    myConsole.addShape(&commandBox);
    
    Style pathLineColor = Style();
    pathLineColor.setBackgroundColor(60, 90, 60);
    pathLineColor.setTextColor(255, 255, 200);
    Line2D pathLine = Line2D(1,height - 5, width-2, height - 5);
    pathLine.setFill(pathLineColor);
    myConsole.addShape(&pathLine);

    Style userBoxColor = Style();
    userBoxColor.setBackgroundColor(30, 40, 190);
    userBoxColor.setTextColor(255, 255, 255);
    Line2D userLine = Line2D(1,0, 20, 0);
    userLine.setFill(userBoxColor);
    myConsole.addShape(&userLine);

    Line2D userLine2 = Line2D(1,height - 6, 20, height - 6);
    userLine2.setFill(userBoxColor);
    myConsole.addShape(&userLine2);

    myConsole.putString(" username",userLine.getAnchorX(),userLine.getAnchorY());
    myConsole.putString(" username",userLine2.getAnchorX(),userLine2.getAnchorY());

    myConsole.putString(" your/current/path",pathLine.getAnchorX(), pathLine.getAnchorY());

    int typerX = 1;
    int typerY = height - 4;
    myConsole.putString("$ ",typerX, typerY);

    myConsole.render();

      
    /*****************
     * Main Loop!
     ****************/
    
    typerX++;
    stack<char> textbox = stack<char>();


    keyboard.startListening();
    while(true)
    {
        if(keyboard.isAvailable())
        {
            char inc = keyboard.read();
            if(inc == 24 | inc == 3) // Ctrl+X
            {
                break;
            }
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
                myConsole.putString(std::string(1,inc),typerX, typerY);
                displayUpdated = true;
            }
            else if(inc == 127 && !textbox.empty()) // backspace
            {
                myConsole.putString(" ",typerX, typerY);
                typerX--;
                if(typerX < 1)
                {
                    typerY--;
                    typerX = width - 2;
                }
                textbox.pop();
                displayUpdated = true;
            }
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
                std::string dump;
                for(int i = 0; i < command.size(); i++)
                {
                    if(command.at(i) == ';')
                    {
                        semicolonSections.push_back(dump);
                        dump = "";
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
                    CommandStatus* currentCommand = new CommandStatus();
                    currentCommand->command = section;
                    currentCommand->isDone = false;
                    currentCommand->status = 0;
                    commandQueue.push(currentCommand);
                }
                
                inputHistory.insert(inputHistory.begin(), command);
                history = -1;

                processingThread = new std::thread(commandProcessor,&myConsole);
                myConsole.addShape(&commandBox);
                typerX = 1;
                typerY = height - 4;
                myConsole.putString("$ ",typerX, typerY);
                typerX++;
                displayUpdated = true;
                
            }
        } 

        if(commandQueue.empty() && !displayQueue.empty())
        {
            displayCommandResult(myConsole);
            displayUpdated = true;
        }       
        

        if(displayUpdated && !keyboard.isAvailable())
        {
            myConsole.render();    
            displayUpdated = false;
        }
        if(stopProgram)
        {
            break;
        }
    }

    keyboard.stopListening();
    myConsole.clear();
    system("clear");
    std::cout << "exit!";

    
}

