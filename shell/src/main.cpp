
#include <iostream>
#include <PrettyConsole/console.h>
#include <PrettyConsole/inputHandler.h>
#include <PrettyConsole/rectangle.h>
#include <PrettyConsole/line.h>
#include <stack>
#include <string>

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

    Style backgroundColor = Style();
    backgroundColor.setBackgroundColor(20, 20, 20);
    backgroundColor.turnOffBlink();
    Rectangle2D background = Rectangle2D(1, 1,width - 2,height - 2);
    background.setFill(backgroundColor);
    
    myConsole.addShape(&background);

    Style commandColor = Style();
    commandColor.setBackgroundColor(60, 60, 60);
    Rectangle2D commandBox = Rectangle2D(1,height- 4, width-2, 4);
    commandBox.setFill(commandColor);
    myConsole.addShape(&commandBox);
    
    Style commandDividerLineColor = Style();
    commandDividerLineColor.setBackgroundColor(60, 90, 60);
    commandDividerLineColor.setTextColor(255, 255, 200);
    Line2D dividerLine = Line2D(1,height - 5, width-2, height - 5);
    dividerLine.setFill(commandDividerLineColor);
    myConsole.addShape(&dividerLine);

    Style userBoxColor = Style();
    userBoxColor.setBackgroundColor(30, 40, 190);
    userBoxColor.setTextColor(255, 255, 255);
    Line2D userLine = Line2D(1,0, 10, 0);
    userLine.setFill(userBoxColor);
    myConsole.addShape(&userLine);


    int typerX = 1;
    int typerY = height - 4;
    int chars = 0;
    myConsole.putString("$ ",typerX, typerY);

    myConsole.render();

    stack<char> inputCommand = stack<char>();
        
    // start keyboard listener
    keyboard.startListening();
    int debugRow = 5;
    typerX ++;
    while(true)
    {
        if(keyboard.isAvailable())
        {
            char inc = keyboard.read();
            if(inc == 24)
            {
                break;
            }
            else if(inc > 31 && inc < 127)
            {
                // letter!
                typerX ++;
                if(typerX > width - 2)
                {
                    typerX = 1;
                    typerY += 1;
                } 
                if(typerY > height)
                {
                    break;
                }
                myConsole.putString(std::string(1,inc),typerX, typerY);
                chars++;
                inputCommand.push(inc);
            }
            else if(inc == 127 && chars > 0)
            {
                myConsole.putString(" ",typerX, typerY);
                typerX--;
                if(typerX < 1)
                {
                    typerX = width - 2;
                    typerY -= 1;
                } 
                if(typerY < height - 5)
                {
                    // error! Out of bounds!
                    continue;
                }
                chars--;
                inputCommand.pop();
            }
            else if(inc == 10)
            {
                std::string command = std::string();
                while(!inputCommand.empty())
                {
                    // convert stack to string!
                    char c = inputCommand.top();
                    inputCommand.pop();
                    command = std::string(1, c) + command;
                }
                myConsole.addShape(&dividerLine); // clears the box.
                myConsole.putString(command,dividerLine.getAnchorX(),dividerLine.getAnchorY());
                
                    

                myConsole.addShape(&commandBox);
                typerX = 1;
                typerY = height - 4;
                chars = 0;
                myConsole.putString("$ ",typerX, typerY);
                typerX++;

            }
            myConsole.putString(std::to_string(typerX) + std::string("  "), 2, 2);
            myConsole.putString(std::to_string(typerY) + std::string("  "), 2, 3);

            myConsole.putString(std::string("Press ") + std::to_string(inc), 2, debugRow);
            debugRow++;
            if(debugRow > height - 10)
            {
                debugRow = 5;
            }
            myConsole.putString("-------- ", 2, debugRow);
            myConsole.render();
        }
    }

    keyboard.stopListening();


    
}