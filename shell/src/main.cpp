
#include <iostream>
#include <PrettyConsole/console.h>
#include <PrettyConsole/inputHandler.h>
#include <PrettyConsole/rectangle.h>
#include <PrettyConsole/line.h>
#include <stack>
#include <string>
#include <queue>
#include <thread>

volatile bool displayUpdated = false;

std::thread* processingThread;

struct CommandAndResponse
{
    std::string command;
    std::string response;
    bool isDone = true;
    bool isDisplay = true;
    int status = 0;
};

CommandAndResponse currentCommand;

int currentRow = 0;

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

void commandProcessor()
{
    currentCommand.response = "This is the response to: " + currentCommand.command;
    currentCommand.status = 1;
    currentCommand.isDisplay = false;
    currentCommand.isDone = true;
}

void displayCommandResult(Console &myConsole, int repeat=0)
{
    Style commandBarColor = Style();
    if(currentCommand.status == 0)
    {
        commandBarColor.setBackgroundColor(60, 255, 60);
        commandBarColor.setTextColor(0,0,0);
    }
    else if(currentCommand.status == 1)
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
    for(int i = 0; i < currentCommand.command.size(); i++)
    {
        int row = i / (myConsole.getWidth() - 3);
        int col = i % (myConsole.getWidth() - 3);
        char inc = currentCommand.command.at(i);
        
        if(row + 1 + currentRow > myConsole.getHeight() - 9)
        {
            clear(myConsole);
            currentRow = 0;
            return displayCommandResult(myConsole, 1);
        }
        if(lastRowPaint != row)
        {
            int barWidth = myConsole.getWidth() - 3;
            if(currentCommand.command.size() + 2 < myConsole.getWidth() - 3)
            {
                barWidth = currentCommand.command.size() + 1;
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

    for(int i = 0; i < currentCommand.response.size(); i++)
    {
        char inc = currentCommand.response.at(i);
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
                currentCommand.response = "too big!";
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
                currentCommand.response = "too big!";
                return;
            }
            return displayCommandResult(myConsole, 1);
        }
    }
    currentRow = textRow;

}



int main()
{
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
            else if(inc == 10 && currentCommand.isDone && currentCommand.isDisplay) // enter!
            {
                // process command!
                std::string command = std::string();
                while(!textbox.empty())
                {
                    // convert stack to string!
                    char c = textbox.top();
                    textbox.pop();
                    command = std::string(1, c) + command;
                }

                currentCommand.command = command;
                currentCommand.isDone = false;

                processingThread = new std::thread(commandProcessor);
                myConsole.addShape(&commandBox);
                typerX = 1;
                typerY = height - 4;
                myConsole.putString("$ ",typerX, typerY);
                typerX++;
                
            }
        } 

        if(currentCommand.isDone && !currentCommand.isDisplay)
        {
            displayCommandResult(myConsole);
            displayUpdated = true;
            currentCommand.isDisplay = true;
        }       
        

        if(displayUpdated && !keyboard.isAvailable())
        {
            myConsole.render();    
            displayUpdated = false;
        }
    }

    keyboard.stopListening();
    
}


/****************
 *  OLD CODE
 * 
 * **************/

// start keyboard listener
    // stack<char> inputCommand = stack<char>();
    // keyboard.startListening();
    // int debugRow = 5;
    // typerX ++;
    // while(true)
    // {
    //     if(keyboard.isAvailable())
    //     {
    //         char inc = keyboard.read();
    //         if(inc == 24)
    //         {
    //             break;
    //         }
    //         else if(inc > 31 && inc < 127)
    //         {
    //             // letter!
    //             typerX ++;
    //             if(typerX > width - 2)
    //             {
    //                 typerX = 1;
    //                 typerY += 1;
    //             } 
    //             if(typerY > height)
    //             {
    //                 break;
    //             }
    //             myConsole.putString(std::string(1,inc),typerX, typerY);
    //             chars++;
    //             inputCommand.push(inc);
    //         }
    //         else if(inc == 127 && chars > 0)
    //         {
    //             myConsole.putString(" ",typerX, typerY);
    //             typerX--;
    //             if(typerX < 1)
    //             {
    //                 typerX = width - 2;
    //                 typerY -= 1;
    //             } 
    //             if(typerY < height - 5)
    //             {
    //                 // error! Out of bounds!
    //                 continue;
    //             }
    //             chars--;
    //             inputCommand.pop();
    //         }
    //         else if(inc == 10)
    //         {
    //             std::string command = std::string();
    //             while(!inputCommand.empty())
    //             {
    //                 // convert stack to string!
    //                 char c = inputCommand.top();
    //                 inputCommand.pop();
    //                 command = std::string(1, c) + command;
    //             }
    //             myConsole.addShape(&dividerLine); // clears the box.
    //             myConsole.putString(command,dividerLine.getAnchorX(),dividerLine.getAnchorY());

    //             myConsole.addShape(&commandBox);
    //             typerX = 1;
    //             typerY = height - 4;
    //             chars = 0;
    //             myConsole.putString("$ ",typerX, typerY);
    //             typerX++;

    //         }
    //         myConsole.putString(std::to_string(typerX) + std::string("  "), 2, 2);
    //         myConsole.putString(std::to_string(typerY) + std::string("  "), 2, 3);

    //         myConsole.putString(std::string("Press ") + std::to_string(inc), 2, debugRow);
    //         debugRow++;
    //         if(debugRow > height - 10)
    //         {
    //             debugRow = 5;
    //         }
    //         myConsole.putString("-------- ", 2, debugRow);
    //         myConsole.render();
    //     }
    // }

    // keyboard.stopListening();
