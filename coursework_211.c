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
    //left = (void*)((void*)fitting_slot + sizeof(Block));
    //right = (void*)((void*)fitting_slot + size + 2 * sizeof(Block));

    Block* leftBlock = inputBlock;
    Block* rightBlock = inputBlock + size + sizeof(Block);

    leftBlock->nextBlock = rightBlock;
    leftBlock->size = size;
    leftBlock->free = 1;

    /*
    These lines above show how the new block of memory in the structure is allocated. A new block of memory is created and its parameters are filled.
    This new Block struct will serve as the new free part of the structure, since its free variable is set to 1.
    */

    rightBlock->size = (inputBlock->size) - size - sizeof(Block);
    rightBlock->free = 0;
    rightBlock->nextBlock = inputBlock->nextBlock;
    rightBlock->prevBlock = leftBlock;

    tail = rightBlock;

    /*printf("%p is free %d\n", leftBlock, leftBlock->free);
    printf("%p is free %d\n", rightBlock, rightBlock->free);*/

    return ((void*)rightBlock);

    //fitting_slot->nextBlock = left;
    //inputBlock = leftBlock;

    /*
    Fitting slot will be used to store the used storage area. It will also connect with the new block through a nextBlock pointer in the structure.
    */
}

/*static void* memory = 0;
Block* head;
Block* tail;*/

void* new_malloc(size_t size)
{
    /*Block* head = memory;
    Block* tail; */
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

        printf("Memory initialized\n");

        //return (void*)freeList;
    }
    
    
    Block* temp = head;
    while(temp/*->nextBlock*/ != NULL)
    {
        printf("%p \n", temp);

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

                printf("Exact fitting block allocated\n");

                return result;
            }
            else if((temp->size) > size + sizeof(Block))
            {
                result = split(temp, size);

                printf("Fitting block allocated with a split\n");

                return result;
            }
            else //if((temp->size) < size + sizeof(Block))
            {
                //create new block of size 8192 bytes - sizeof(Block) - which starts at the memory address returned by sbrk when called

                if(temp->nextBlock == NULL)
                {
                    Block* new = sbrk(8192);

                    result = sbrk(8192) + sizeof(Block) + size + 1; 

                    printf("Adding new memory to the heap \n");

                    return result;
                }
                else
                {
                    //if not the final block of current allocated heap memory do nothing 
                    //because it would not allow for fragmentation avoidance
                }

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
    /*Block* head = memory;
    Block* tail;*/

    new_malloc(8192 - sizeof(Block) * 2);
    //new_malloc(120);
    //new_malloc(10);

}