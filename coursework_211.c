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

void split(Block* fitting_slot, size_t size) // these parameters are the block which we will be splitting and the size we will be using from this block
{
    Block* left = (void*)((void*)fitting_slot + sizeof(Block));
    Block* right = (void*)((void*)fitting_slot + size + 2 * sizeof(Block));

    fitting_slot->nextBlock = left;

    left->size = size;
    left->free = 0;
    left->nextBlock = right;
    left->prevBlock = fitting_slot;

    /*
    These lines above show how the new block of memory in the structure is allocated. A new block of memory is created and its parameters are filled.
    This new Block struct will serve as the new free part of the structure, since its free variable is set to 1.
    */

    right->size = (fitting_slot->size) - size - sizeof(Block);
    right->free = 1;
    right->nextBlock = fitting_slot->nextBlock;
    right->prevBlock = left;

    /*
    Fitting slot will be used to store the used storage area. It will also connect with the new block through a nextBlock pointer in the structure.
    */
}

void* new_malloc(size_t size)
{
    Block* freeList = memory;
    void* result;
    int memoryAmmount;

    if(memory == 0)
    {
        Block* next;
        memory = sbrk(8192);

        freeList = memory;
        next = memory + sizeof(Block)*2 + size;

        //printf("%p %p is freeList & next \n", freeList, next);

        freeList->size = size; 
        freeList->free = 0;
        freeList->nextBlock = next;
        freeList->prevBlock = NULL; 

        //printf("%p %p is freeList & next \n", freeList, freeList->nextBlock);

        next->free = 1;
        next->nextBlock = NULL;
        next->prevBlock = freeList;
        next->size = 8192 - size - sizeof(Block)*2;

        printf("Memory initialized\n");

        return (void*)freeList;
    }
    else
    {
        while(freeList/*->nextBlock*/ != NULL)
        {
            printf("%p \n", freeList);
            //printf("%ld is freeList size \n", freeList->size);

            if(freeList->free == 1)
            {

                if((freeList->size) == size + sizeof(Block))
                {
                    freeList->free = 0;
                    result = (void*)(++freeList);

                    printf("Exact fitting block allocated\n");

                    return result;
                }
                else if((freeList->size) > size + sizeof(Block))
                {
                    split(freeList, size);
                    result = (void*)(++freeList);

                    printf("%p\n", result);
                    printf("Fitting block allocated with a split\n");

                    return result;
                }
                else if((freeList->size) < size + sizeof(Block))
                {
                    //create new block of size 8192 bytes - sizeof(Block) - which starts at the memory address returned by sbrk when called

                    if(freeList->nextBlock != NULL)
                    {

                    }
                    else
                    {
                        Block* new = sbrk(8192);

                        /*freeList->nextBlock = new;

                        new->free = 1;
                        new->size = 8192 - sizeof(Block);
                        new->prevBlock = freeList;
                        new->nextBlock = NULL;*/

                        result = sbrk(8192) + sizeof(Block) + size + 1; 

                        printf("Adding new memory to the heap \n");

                        return result;
                    }

                }
                else
                {
                    printf("ERROR");
                }
                
            }
            else
            {
                /* code */
            }

            freeList = freeList->nextBlock;


        }
    }
}

int main()
{
    new_malloc(920);
    new_malloc(10);

}