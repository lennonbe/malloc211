#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct Block
{
    size_t size; //size of this block
    struct Block* nextBlock; //pointer to next meta-data block
    struct Block* prevBlock; //pointer to the previous meta-data block
    int free; //used as a flag to know wether block is free or not (set to 0 if free, 1 otherwise)

}Block;

typedef struct allocatedBlock
{
    size_t size; //size of this block
    int free; //used as a flag to know wether block is free or not (set to 0 if free, 1 otherwise)

}allocatedBlock;

Block* head = NULL;
Block* tail = NULL;

allocatedBlock* split(Block* inputBlock, size_t size) // these parameters are the block which we will be splitting and the size we will be using from this block
{
    Block* new = (void*)inputBlock + size + sizeof(allocatedBlock);
    allocatedBlock* allocated;

    if(inputBlock == head)
    {
        new->size = (inputBlock->size) - size - sizeof(Block);
        new->free = 0;
        new->prevBlock = NULL;
        new->nextBlock = inputBlock->nextBlock;

        if(head->nextBlock != NULL)
        {
            head->nextBlock->prevBlock = new;
        }

        allocated = (void*)inputBlock;
        
        allocated->size = size;
        allocated->free = 1;

        inputBlock = (allocatedBlock*)allocated;

        inputBlock = NULL;

        head = new;

        return allocated;
    }
    else
    {
        new->size = (inputBlock->size) - size - sizeof(Block);
        new->free = 0;
        new->prevBlock = inputBlock->prevBlock;
        new->nextBlock = inputBlock->nextBlock;
        
        inputBlock->prevBlock->nextBlock = new;        
        inputBlock->nextBlock->prevBlock = new;

        allocated = (void*)inputBlock;

        allocated->size = size;
        allocated->free = 1;

        inputBlock = NULL;

        return allocated;
    }
    
    //inputBlock -= (sizeof(inputBlock->nextBlock) + sizeof(inputBlock->prevBlock));
}

void* result;
Block* temp;
Block* smallest;
void* programStart;

void* new_malloc(size_t size)
{
    if(head == NULL)
    {
        head = sbrk(8192);
        programStart = head;
        tail = head;

        head->free = 0;
        head->size = 8192 - sizeof(Block);
        head->nextBlock = NULL;
        head->prevBlock = NULL;

        result = split(head, size);
        //result = head;

        return((void*)((long)result + sizeof(allocatedBlock)));
    }

    temp = head;
    smallest = head;
    while(temp != NULL)
    {    
        if(temp->size >= size + sizeof(allocatedBlock) && temp->size < smallest->size && temp->free == 0)
        {          
            smallest = temp;  
        }
        else if(smallest->free == 1 && temp->free == 0)
        {
            smallest = temp;
        }
        else
        {

        }

        temp = temp->nextBlock;
    }

    if((smallest->size) == (size + sizeof(allocatedBlock)))
    {
        smallest->free = 1;
        smallest->nextBlock = NULL;
        smallest->prevBlock = NULL;
        smallest->size = size;

        result = ((void*)((long)smallest + sizeof(allocatedBlock)));

        return result;
    }
    else if((smallest->size) > (size + sizeof(allocatedBlock)))
    {
        allocatedBlock* resultTemp = split(smallest, size);
        result = (void*)resultTemp;

        allocatedBlock* resultTemp2 = result;

        return ((void*)((long)result + sizeof(allocatedBlock)));
    }
    else
    {
        if(smallest->nextBlock == NULL)
        {
            Block* new;
            new = sbrk(8192);

            smallest->nextBlock = new;

            new->prevBlock = smallest;
            new->nextBlock = NULL;
            new->size = 8192 - sizeof(Block);
            new->free = 0;

            allocatedBlock* resultTemp = split(new, size);
            result = resultTemp;

            return ((void*)((long) result + sizeof(allocatedBlock)));
        }
    }
}

void new_free(void* address)
{
    allocatedBlock* curr = address;
    Block* new;

    if(curr != programStart)
    {
        --curr;
    }

    new = (Block*)(address - sizeof(allocatedBlock));

    new->free = 0;
    new->nextBlock = head;
    new->prevBlock = NULL;
    new->size = (curr->size) + sizeof(allocatedBlock) - sizeof(Block);

    head->prevBlock = new;

    head = new;
    /*
    temp = head;
    while(temp != NULL)
    {


        temp = temp->nextBlock;
    }*/
}

void debugPrint()
{
    Block *temp = head;
    int i = 0;
    while(temp != NULL)
    {
        printf("%d - %p | previous - %p | next - %p | size - %zu | flag - %d ", i,temp,temp->prevBlock,temp->nextBlock, temp->size, temp->free);

        unsigned char *debug = (unsigned char*)temp;

        for (int j = 0; j < temp->size + sizeof(Block); j++)
        {
            printf("%02x",debug[j]);
        }

        temp = temp->nextBlock;
        i++;

        printf("\n");
    }
}

void userInterface()
{
    int flag = 1;
    printf("Enter A (malloc) or F (free) and the size or address: \n");

    while(flag == 1)
    {
        char input[20];
        scanf("\n%s", input);

        if(input[0] == 'A')
        {
            char* final = input + 1;
            printf("%p\n", new_malloc(atoi(final)));
        }
        else if(input[0] == 'F')
        {
            char* final = input + 1;
            unsigned long pointer = strtoul(final,NULL,16);
            new_free(pointer);
        }
        else
        {
            flag = 0;
        }
        
    }
}

int main()
{
    void* addr1;
    void* addr2;
    void* addr3;
    void* addr4;
    void* addr5;
    void* addr6;    

    //printf("%ld", sizeof(allocatedBlock));
    addr1 = new_malloc(30); 
    printf("%p \n", addr1);
    addr2 = new_malloc(100);
    printf("%p \n", addr2);
    addr3 = new_malloc(101);
    printf("%p \n", addr3);
    addr4 = new_malloc(102);
    addr5 = new_malloc(103);
    printf("%p \n", addr5);

    //debugPrint();

    new_free(addr1);

    //debugPrint();

    new_free(addr3);
    new_free(addr5);

    debugPrint();

    //new_free(addr1);

    //userInterface();
}