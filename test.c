#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

typedef struct Block
{
    size_t size; //size of this block
    struct Block* nextBlock; //pointer to next meta-data block
    struct Block* prevBlock; //pointer to the previous meta-data block
    int free; //used as a flag to know wether block is free or not (set to 1 if free, 0 otherwise)
    int prevFree;
}Block;

static void* memory = 0;
Block* head;
Block* tail;

void* split(Block* inputBlock, size_t size) // these parameters are the block which we will be splitting and the size we will be using from this block
{

    Block* leftBlock = inputBlock;
    Block* rightBlock = inputBlock + size + sizeof(Block);

    leftBlock->nextBlock = rightBlock;
    leftBlock->size = size;
    leftBlock->free = 1;

    rightBlock->size = (inputBlock->size) - size - sizeof(Block);
    rightBlock->free = 0;
    rightBlock->nextBlock = inputBlock->nextBlock;
    rightBlock->prevBlock = leftBlock;

    tail = rightBlock;

    return ((void*)rightBlock);
    
}

void* new_malloc(size_t size)
{
    void* result;
    head = memory;

    if(memory == 0)
    {
        memory = sbrk(8192);

        head = memory;
        tail = memory;

        head->free = 0;
        head->size = 8192 - sizeof(Block);
        head->nextBlock = NULL;
        head->prevBlock = NULL;

    }
    
    
    Block* temp = head;
    while(temp != NULL)
    {

        if(temp->free == 0)
        {

            if((temp->size) == size + sizeof(Block))
            {
                Block* next = temp + size + sizeof(Block);//by setting the pointer to a value in the heap C automatically stores the pointer variable in the heap as well

                temp->free = 1; //no longer free
                temp->nextBlock = NULL;
                temp->prevBlock = NULL;
                temp->size = size;

                result = (void*)(temp + 1);

                return result;
            }
            else if((temp->size) > size + sizeof(Block))
            {
                result = split(temp, size);

                return result;
            }
            else //if((temp->size) < size + sizeof(Block))
            {
        

            }
            
        }
        else
        {
            
        }

        temp = temp->nextBlock;


    }
    
}

int main()
{
    

    void* addr1;
    void* addr2;
    void* addr3;

    addr1 = new_malloc(100); 
    addr2 = new_malloc(100);
    //new_free(addr2); 
    //addr3 = new_malloc(100);
    printf( "Addr1 = %p, Addr2 = %p, Addr3 = %p \n", addr1, addr2, addr3);
    printf("%ld \n", sizeof(Block));

}