
#include <iostream>
#include <fstream>
using namespace std;

#define LOGICAL_MEMORY_LENGTH 4
#define PAGE_TABLE_LENGTH 4
#define PHYSICAL_MEMORY_LENGTH 8
#define NUM_OF_PAGES 4
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

void readFromFile(char words[NUM_OF_PAGES][PAGE_LENGTH])
{
    ifstream input("logMem.txt");

    char inputChar;
    //char words[NUM_OF_PAGES][PAGE_LENGTH];

    int i = 0;
    int j = 0;
    while (!input.eof() && j < NUM_OF_PAGES)
    {
        inputChar = input.get();
        if(inputChar == 10)
        {
            j++;
            i=0;
            continue; // skip the '\n'
        }
        words[i][j] = inputChar;
        i++;
        if(i == PAGE_LENGTH)
        {
            i = i % PAGE_LENGTH;
            j++; 
        }
    }
    input.close();
}

int main()
{
    Page logicalMemory[LOGICAL_MEMORY_LENGTH];  // This is the logical memory space. It has a length of 4 pages.
    PageTableEntry pageTable[PAGE_TABLE_LENGTH]; // This is the page table. It has the same length as the number of pages in logical memory.     
    Page physicalMemory[PHYSICAL_MEMORY_LENGTH]; // This is the physical memory space. It has a length of 8 pages.

    // I'm confused at this for loop. Shouldn't the logical memory be filled with pages of chars not integers?
    for(int i = 0; i < LOGICAL_MEMORY_LENGTH; i++) // fill the logical memory with data. This is the difference between C and C++. (because 4 bytes is technically an "int")
    {
        logicalMemory[i].byte1 = i % 26 + 65;   // we are just putting in random data right now. We don't really care what the numbers are.
        logicalMemory[i].byte2 = (i * 2) % 26 + 65;   // because later we will be reading from a file real data.
        logicalMemory[i].byte3 = (i * 3) % 26 + 65;   
        logicalMemory[i].byte4 = (i * 4) % 26 + 65;
    }

    // what is going on here: create a "table" (really an array) where each item is two pointers. One to the logical memory and one to the physical memory.
    // so, what is the best way to create this "Page Table"?


    // logical memory has 4 elements.
    // physical memory has 8 elements.
    // mapping:
    //    logical   |    physical
    //       2              3
    //       1              0
    //       0              6
    //       3              1

    // Do we need an empty constructor for Page
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

    for (int i = 0; i < PAGE_TABLE_LENGTH; i++)
    {
        // logical index. 
        Page* logical = pageTable[i].logicalAddress;
        // physical index.
        Page* physical = pageTable[i].physicalAddress;

        cout << "i: " << i << " | " 
             << "Logical: " << (logical - logicalMemory) % sizeof(*logical) << " | "
             << "Physical: " << (physical - physicalMemory) % sizeof(*logical)
             << "\n";
    }

    cout << "===================\n" << "\n";

    // print out the physical memory.

    cout << "===================\n"
         << "PHYSICAL" << "\n";

    for (int i = 0; i < PHYSICAL_MEMORY_LENGTH; i++)
    {
        cout << "i: " << i << " | " 
             << hex << physicalMemory[i].byte1 
             << hex << physicalMemory[i].byte2
             << hex << physicalMemory[i].byte3 
             << hex << physicalMemory[i].byte4 
             << "\n";
    }
    cout << "===================" << "\n";
}