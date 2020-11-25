#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>

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

void removeFromFreeList(Block* input)
{
    input->prevBlock->nextBlock = input->nextBlock;
    input->nextBlock->prevBlock = input->prevBlock;
    input->nextBlock = NULL;
    input->prevBlock = NULL; 
}

Block* bestFit(size_t size)
{
    Block* temp = head;
    Block* smallest = head;

    while(temp != NULL)
    {
        if((long)temp->size >= (long)(size + sizeof(Block)) && (long)temp->size < (long)smallest->size && temp->free == 0)
        {
            smallest = temp;
        }

        temp = temp->nextBlock;
    }

    return smallest;
}

void* result;
Block* temp;
Block* smallest;

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
    smallest = head;
    while(temp != NULL)
    {    
        if(temp->size >= size + sizeof(Block) && temp->size < smallest->size && temp->free == 0)
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

    if((smallest->size) == (size + sizeof(Block)))
    {
        smallest->free = 1;
        smallest->nextBlock = NULL;
        smallest->prevBlock = NULL;
        smallest->size = size;

        result = ((void*)((long)smallest + sizeof(Block)));

        return result;
    }
    else if((smallest->size) > (size + sizeof(Block)))
    {
        split(smallest, size);
        result = smallest;

        return (void*)((long) result + sizeof(Block));
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

            split(new, size);

            result = new;

            //removeFromFreeList(new);

            return (void*)((long) result + sizeof(Block));
        }
    }
}

void new_free(void* address)
{
    Block* temp1;
    Block* temp2;
    Block* curr = address;
    --curr;
    curr->free = 0;

    //this is where the normal implementation ends

    temp1 = head;
    temp2 = curr;
    
    head = curr;
    head->nextBlock = temp1;
    head->size = curr->size;
    head->free = 0;

    if(curr->nextBlock != NULL)
    {
        curr->prevBlock->nextBlock ==
    }

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

/*void userInterface()
{
    int flag = 0;
    while(flag != 1)
    {
        char input;
        char input2[70];

        scanf("%c%ld", &input, &input2);


        if(input == 'F')
        {
            new_free((void*)input2);
        }
        else if(input == 'A')
        {
            new_malloc(((long)input2));
        }
        else
        {
            printf("INVALID INPUT \n");
        }
        
    }
}*/

/*void my_free(void *ptr)
{
    Block* current = (void*)((long)ptr - sizeof(Block));
    current->free = 0;
    void* top = sbrk(0);
    if(current >= head && (void*) current <= top)
    {
        if(current->nextBlock != NULL && current->nextBlock->free == 0)
        {
            current->size = current->size + current->nextBlock->size + sizeof(Block);
            current->nextBlock = current->nextBlock->nextBlock;
            if(current->nextBlock != NULL && current->nextBlock->nextBlock != NULL)
            {
                current->nextBlock->nextBlock->prevBlock = current;
            }
        }
        if(current->prevBlock != NULL && current->prevBlock->free == 0)
        {
            current->prevBlock->nextBlock = current->nextBlock;
            current->prevBlock->size = current->prevBlock->size + current->size + sizeof(Block);
            current->prevBlock->nextBlock = current->nextBlock;
            if(current->prevBlock->prevBlock != NULL)
            {
                current->prevBlock->prevBlock->nextBlock = current;
            }

        }

    }
}*/

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

    //my_free(addr4);
    //my_free(addr5);

    //new_malloc(50);

    debugPrint();
}