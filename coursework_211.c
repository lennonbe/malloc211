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

void split(Block *fitting_slot, size_t size) // these parameters are the block which we will be splitting and the size we will be using from this block
{
    Block* new = (void*)((void*)fitting_slot + size + sizeof(Block));
    Block* new = (void*)((void*)fitting_slot + size + sizeof(Block));

    new->size = (fitting_slot->size) - size - sizeof(Block);
    new->free = 1;
    new->nextBlock = fitting_slot->nextBlock;
    new->prevBlock = fitting_slot;

    /*
    These lines above show how the new block of memory in the structure is allocated. A new block of memory is created and its parameters are filled.
    This new Block struct will serve as the new free part of the structure, since its free variable is set to 1.
    */

    fitting_slot->size = size;
    fitting_slot->free = 0;
    fitting_slot->nextBlock = new;

    /*
    Fitting slot will be used to store the used storage area. It will also connect with the new block through a nextBlock pointer in the structure.
    */
}

void* new_malloc(size_t size)
{
    Block* freeList = memory;

    //printf("%p", (void*) freeList);

    if(memory == 0)
    {
        memory = sbrk(8192);

        freeList = memory;

        freeList->size = 8192 - sizeof(Block); 
        freeList->free = 1;
        freeList->nextBlock = NULL;
        freeList->prevBlock = NULL; 

        printf("Memory initialized\n");
    }
    else
    {
        do
        {
            //printf("Itterating through the loop \n");
            void* result;
            //printf("%p \n", result);

            printf("%ld is freeList size \n", freeList->size);
            printf("%ld is size\n", size);

            if((freeList->size) == size && ((freeList->free) == 1))
            {
                freeList->free = 0;
                result = (void*)(++freeList);

                printf("Exact fitting block allocated\n");

                return result;
            }
            else if((freeList->size) > (size + sizeof(Block)) && ((freeList->free) == 1))
            {
                split(freeList, size);
                result = (void*)(++freeList);

                printf("Fitting block allocated with a split\n");

                return result;
            }
            else if((freeList->nextBlock != NULL) && ((freeList->free) == 1))
            {
                //create new block of size 8192 bytes - sizeof(Block) - which starts at the memory address returned by sbrk when called

                Block* new = sbrk(8192);

                freeList->nextBlock = new;

                new->free = 1;
                new->size = 8192 - sizeof(Block);
                new->prevBlock = freeList;
                new->nextBlock = NULL;

                result = sbrk(8192); 

                printf("Adding new memory to the stack \n");

                return result;
            }
            else{}

            freeList = freeList->nextBlock;

        }while((((freeList->size) < size) || ((freeList->free)==0)) && (freeList->nextBlock != NULL));
    }
    
}

int main()
{
    new_malloc(10);
    new_malloc(900);
    new_malloc(900);
    new_malloc(900);

    /*for(int i = 0; i < 12; i++)
    {
        new_malloc(900);
    }*/
    
}


