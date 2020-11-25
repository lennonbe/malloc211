#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

typedef struct Block
{
    size_t size; //size of this block
    struct Block* nextBlock; //pointer to next meta-data block
    struct Block* prevBlock; //pointer to the previous meta-data block
    int free; //used as a flag to know wether block is free or not (set to 0 if free, 1 otherwise)

}Block;

Block* head = NULL;
Block* tail = NULL;

void split(Block* inputBlock, size_t size) // these parameters are the block which we will be splitting and the size we will be using from this block
{
    Block* new = (void*)inputBlock + size + sizeof(Block);

    new->size = (inputBlock->size) - size - sizeof(Block);
    new->free = 0;
    new->prevBlock = inputBlock;

    inputBlock->nextBlock = new;
    inputBlock->size = size;
    inputBlock->free = 1;
}

void* result;
Block* temp;

void* new_malloc(size_t size)
{
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
            else
            {
                if(temp->nextBlock == NULL)
                {
                    Block* new;
                    new = sbrk(8192);

                    temp->nextBlock = new;

                    new->prevBlock = temp;
                    new->nextBlock = NULL;
                    new->size = 8192 - sizeof(Block);
                    new->free = 0;

                    split(new, size);

                    result = new;

                    return (void*)((long) result + sizeof(Block));
                }
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
    void* addr4;
    void* addr5;
    void* addr6;
    

    addr1 = new_malloc(100); 
    addr2 = new_malloc(100);
    addr3 = new_malloc(100);
    addr4 = new_malloc(100);
    addr5 = new_malloc(100);
    addr6 = new_malloc(8000);

    printf( "Addr1 = %p, Addr2 = %p, Addr3 = %p, Addr4 = %p, Addr5 = %p, Addr6 = %p\n", addr1, addr2, addr3, addr4, addr5, addr6);
    printf("%ld \n", sizeof(Block));


}