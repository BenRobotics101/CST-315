/**
 * @file paging.cpp
 * @author Benjamin Carter and Trevor Pope
 * @brief Simulate a Page Table with logical and physical memory.
 * @version 0.1
 * @date 2024-03-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <fstream>
using namespace std;

#define LOGICAL_MEMORY_LENGTH 4
#define PAGE_TABLE_LENGTH 4
#define PHYSICAL_MEMORY_LENGTH 8
#define PAGE_LENGTH 4

struct Page
{
    unsigned char byte1 = 0; // 'T' // so this is the "page". It has four letters, or four characters. 
    unsigned char byte2 = 0; // 'A'
    unsigned char byte3 = 0; // 'N'
    unsigned char byte4 = 0; // 'K'  Storing "TANK" for instance.
};

struct PageTableEntry
{
    Page* logicalAddress; // This is the page table. You have a pointer to the "logical address", which is just an item in the logical memory array
    Page* physicalAddress; // This is the pointer to the "physical address"
    bool isValid; 
    PageTableEntry() {}
    PageTableEntry(Page* logical, Page* physical)
    {
        logicalAddress = logical;
        physicalAddress = physical;
        isValid = physicalAddress != nullptr;
    }
};

void readFromFile(char words[LOGICAL_MEMORY_LENGTH][PAGE_LENGTH])
{
    ifstream input("logMem.txt");

    char inputChar;
    //char words[NUM_OF_PAGES][PAGE_LENGTH];

    int i = 0; // COLUMN.
    int j = 0; // ROW
    while (!input.eof() && j < LOGICAL_MEMORY_LENGTH)
    {
        inputChar = input.get();
        if(inputChar == 10)
        {
            j++;
            i = 0;
            continue; // skip the '\n'
        }
        words[j][i] = inputChar;
        i++;
        if(i == PAGE_LENGTH)
        {
            i = 0;
        }
    }
    input.close();
}

int main()
{
    Page logicalMemory[LOGICAL_MEMORY_LENGTH];  // This is the logical memory space. It has a length of 4 pages.
    PageTableEntry pageTable[PAGE_TABLE_LENGTH]; // This is the page table. It has the same length as the number of pages in logical memory.     
    Page physicalMemory[PHYSICAL_MEMORY_LENGTH]; // This is the physical memory space. It has a length of 8 pages.


    char inputWords[LOGICAL_MEMORY_LENGTH][PAGE_LENGTH];

    readFromFile(inputWords);

    for(int i = 0; i < LOGICAL_MEMORY_LENGTH; i++)
    {
        char* letters = inputWords[i];
        logicalMemory[i].byte1 = letters[0];
        logicalMemory[i].byte2 = letters[1];
        logicalMemory[i].byte3 = letters[2];
        logicalMemory[i].byte4 = letters[3];
    }

    // logical memory has 4 elements.
    // physical memory has 8 elements.
    // mapping:
    //    logical   |    physical
    //       2              3
    //       1              0
    //       0              6
    //       3              1

    pageTable[0] = PageTableEntry(&logicalMemory[0], &physicalMemory[3]);
    pageTable[1] = PageTableEntry(&logicalMemory[1], &physicalMemory[0]);
    pageTable[2] = PageTableEntry(&logicalMemory[2], &physicalMemory[6]);
    pageTable[3] = PageTableEntry(&logicalMemory[3], &physicalMemory[1]);
    
    // now, using the page table, populate the physical memory.
    for (PageTableEntry p : pageTable)
    {
        // set the value at p->physicalAddress to the value at p->localAddress. * for value, & for address.
        if(p.isValid)
        {
            *(p.physicalAddress) = *(p.logicalAddress);
        }
    }

    cout << "===================" << "\n"; 
    cout << "LOGICAL" << "\n";
    // could you add a loop to print out the logical memory?
    for (int i = 0; i < LOGICAL_MEMORY_LENGTH; i++)
    {
        cout << "i: " << i << " | " 
             << hex << logicalMemory[i].byte1 
             << hex << logicalMemory[i].byte2 
             << hex << logicalMemory[i].byte3 
             << hex << logicalMemory[i].byte4 
             << "\n";
    }
    cout << "===================\n" << "\n";

    // then page table

    cout << "===================\n"
         << "PAGE TABLE" << "\n";


    cout << logicalMemory << endl;
    cout << pageTable[2].logicalAddress << endl;
    cout << pageTable[2].logicalAddress - logicalMemory << endl;

    for (int i = 0; i < PAGE_TABLE_LENGTH; i++)
    {
        // logical index. 
        Page* logical = pageTable[i].logicalAddress;
        // physical index.
        Page* physical = pageTable[i].physicalAddress;

        cout << "i: " << i << " | " 
             << "Logical: " << (logical - logicalMemory) % sizeof(logical) << " | "
             << "Physical: " << (physical - physicalMemory) % sizeof(physical)       
             << "\n";
    }

    cout << "===================\n" << "\n";

    // print out the physical memory.

    cout << "===================\n"
         << "PHYSICAL" << "\n";

    ofstream output("outfile.txt");
    output << "PHYSICAL" << "\n";

    for (int i = 0; i < PHYSICAL_MEMORY_LENGTH; i++)
    {
        cout << "i: " << i << " | " << (char)(physicalMemory[i].byte1 == 0 ? '-' : physicalMemory[i].byte1);
        cout << (char)(physicalMemory[i].byte2 == 0 ? '-' : physicalMemory[i].byte2);
        cout << (char)(physicalMemory[i].byte3 == 0 ? '-' : physicalMemory[i].byte3);
        cout << (char)(physicalMemory[i].byte4 == 0 ? '-' : physicalMemory[i].byte4);
        cout << "\n";

        output << "i: " << i << " | " << (char)(physicalMemory[i].byte1 == 0 ? '-' : physicalMemory[i].byte1);
        output << (char)(physicalMemory[i].byte2 == 0 ? '-' : physicalMemory[i].byte2);
        output << (char)(physicalMemory[i].byte3 == 0 ? '-' : physicalMemory[i].byte3);
        output << (char)(physicalMemory[i].byte4 == 0 ? '-' : physicalMemory[i].byte4);
        output << "\n";
    }
    cout << "===================" << "\n";
    output.close();
}