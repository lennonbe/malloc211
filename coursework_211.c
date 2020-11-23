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
static void* endOfHeap;
Block* head;
Block* tail;

void* split(Block* inputBlock, size_t size) // these parameters are the block which we will be splitting and the size we will be using from this block
{
    Block* leftBlock = inputBlock;
    Block* rightBlock = inputBlock + size + sizeof(Block);

    leftBlock->nextBlock = rightBlock;
    leftBlock->prevBlock = inputBlock->prevBlock;
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

    return ((void*)(rightBlock + sizeof(Block)));
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
        endOfHeap = memory + 8192;
        //long length = endOfHeap - memory;

        printf("Memory starts at %p and ends at %p - length is 8192 \n", memory, endOfHeap);
        printf("%ld \n", sizeof(Block));

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
        //printf("%p is a used mem address\n", temp);

        if(temp->free == 0)
        {
            printf("%p is a free mem address\n", temp);

            if((temp->size) == (size + sizeof(Block)))
            {
                Block* next = temp + size + sizeof(Block);//by setting the pointer to a value in the heap C automatically stores the pointer variable in the heap as well

                temp->free = 1; //no longer free
                temp->nextBlock = NULL;
                temp->prevBlock = NULL;
                temp->size = size;

                result = (void*)(temp + sizeof(Block));

                printf("Exact fitting block allocated\n");

                return result;
            }
            else if((temp->size) > (size + sizeof(Block)))
            {
                result = split(temp, size);

                printf("Fitting block allocated with a split\n");

                return result;
            }
            else //if((temp->size) < size + sizeof(Block))
            {
                //create new block of size 8192 bytes - sizeof(Block) - which starts at the memory address returned by sbrk when called
                if(temp->nextBlock == NULL) //we are currently on last block
                {
                    printf("WORKING 1");

                    Block* next = sbrk(8192);
                    Block* next2 = next + size + sizeof(Block);

                    printf("WORKING 2");

                    next->size = size;
                    next->nextBlock = next2;
                    next->prevBlock = temp;
                    next->free = 1;

                    printf("WORKING 3");

                    //int tempInit = temp->size;

                    next2->size = 8192 - size - sizeof(Block) * 2;
                    next2->nextBlock = NULL;
                    next2->prevBlock = next;
                    next2->free = 0;

                    printf("WORKING 4");

                    temp->nextBlock = next;

                    printf("WORKING 5");

                    result = (void*)(next + sizeof(Block));

                    printf("WORKING 6");

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
            printf("%p is a used mem address\n", temp);
        }

        temp = temp->nextBlock;


    }
    
}

int main()
{
    //new_malloc(8192 - sizeof(Block) * 2);
    new_malloc(1120);
    new_malloc(1120);
    new_malloc(1120);
    new_malloc(1120);
}