#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

typedef struct Block
{
    size_t size; //size of this block
    struct Block* nextBlock; //pointer to next meta-data block
    struct Block* prevBlock; //pointer to the previous meta-data block
    int free; //used as a flag to know wether block is free or not (set to 0 if free, 1 otherwise)
    int prevFree;

}Block;

Block* head = NULL;
Block* tail = NULL;

void split(Block* inputBlock, size_t size) // these parameters are the block which we will be splitting and the size we will be using from this block
{
    //Block* leftBlock = inputBlock;
    Block* rightBlock = inputBlock + size + sizeof(Block);

    rightBlock->size = (inputBlock->size) - size - sizeof(Block);
    rightBlock->free = 0;
    rightBlock->prevBlock = inputBlock;

    inputBlock->nextBlock = rightBlock;
    inputBlock->size = size;
    inputBlock->free = 1;

    //return ((void*)(rightBlock + sizeof(Block)) + 1);
}

void* new_malloc(size_t size)
{
    void* result;
    Block* temp;

    if(head == NULL)
    {
        head = sbrk(8192);

        head->free = 0;
        head->size = 8192 - sizeof(Block);
        head->nextBlock = NULL;
        head->prevBlock = NULL;

        split(head, size);
        result = head;

        return result;
    }
    
    temp = head;
    while(temp != NULL)
    {        
        //printf("%p - %ld - %d - %p \n", temp, temp->size,temp->free, temp->nextBlock);
        if(temp->free == 0)
        {          
            if((temp->size) == (size + sizeof(Block)))
            {
                temp->free = 1;
                temp->nextBlock = NULL;
                temp->prevBlock = NULL;
                temp->size = size;

                result = ((void*)((long)temp + sizeof(Block)));

                return result;
            }
            else if((temp->size) > (size + sizeof(Block)))
            {
                split(temp, size);
                result = temp;

                return (void*)((long) result + sizeof(Block));
            }                        
        }

        temp = temp->nextBlock;
    }
    
}

int main()
{
    //new_malloc(8192 - sizeof(Block) * 2);
    //new_malloc(500);
    //new_malloc(500);

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