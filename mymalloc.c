#include<stdio.h>
#include<stdlib.h>
#include"assignment_1.h"


typedef struct book{
    int size;
    struct book * next;                                                    //to keep link to the next book keeping structure                                             
    int encode;
}book;



//Global declarations
char * p;                                                                 //Gobal array
book * block;                                                            //Global pointer
long int size_of_structure = sizeof(book);                                 //sizeof book keeping structure





void allocate(int n){
    p = (char*)malloc(n*sizeof(char));                                    //allocate n bytes
    block = (void *)p;
    block->encode =2;      
    block->next=NULL;
    block->size = n - sizeof(book);                                        //size = total size - sizeof(book keeping)
}



static void makeblock(book * node,int size){                           //function to split the block for required size
    book * new = (void *)((void*)node+size+sizeof(book));
    //  printf("%d\n",node->size);
    int remaining = (node->size-(size));
  

    if(remaining<=sizeof(book)){
        node->encode=1;
        
    }else{
        new->encode=2;
        new->next=node->next;
        node->next = new;
        node->encode=1;
        new->size = (node->size)-(size+sizeof(book));
        node->size = size;
        
    }



    

}


static void goodallocate(){                                                 //function to merge continuous free blocks
    book * curr = block;                    
    book* prev =NULL;
    while(curr->next!=NULL){
        if((curr->encode==2)&&(curr->next->encode==2)){                      //check encode value
            curr->size = curr->size+curr->next->size+sizeof(book);
            curr->next = curr->next->next;
        }
        prev = curr;
        if(curr->next!=NULL){
        curr =curr->next;
        }
    }



    


}


void * mymalloc(int size){                                                        //my malloc
  
    if(size<0||size==0){
        return NULL;
    }

    book * curr,*prev,*best;
    int bestfit=0;
    int new_size = size+sizeof(book);                                                 //size+sizeof(book keeping)
    void * result;      
    curr = block;



while(curr!=NULL){
    if(curr->encode ==2){   
        if((curr->size>=size)&&((bestfit>curr->size)||bestfit==0)){               //finding best bit for the size
            bestfit= curr->size;
            best = curr;
            if(curr->size==size){
                break;
            }
        }
    }
    prev = curr;
    curr = curr->next;
}

//printf("%d\n",bestfit);
if(curr!=NULL){
    curr->encode =1;
    result = (void*)(++prev);
    return result;
    }else if(bestfit!=0){
       // printf("%d\n\n",best->size);
            makeblock(best,size);
            result = (void *)(++best);
            return result;
    }else{
        return NULL;
    }
}






void myfree(void * b){                                                                //myfree
    book * node = b;        
    // printf("%d\n",node->size);
    --node;
    node->encode = 2;
    goodallocate();
}

void print_book(){                                                                  //print Book function to print size of book keeping structure
    printf("%ld\n",size_of_structure);
}


void display_mem_map(){                                                                //to display current memory allocations
    goodallocate();
    book * node = block;

    int index =0;                                                                       //index :to point  next block

    while(node!=NULL){
        printf("%d\t%ld\t%d\n",index,sizeof(book),0);               
        index = index+sizeof(book);
        printf("%d\t%d\t%d\n",index,node->size,node->encode);
        index =index+node->size;
        node= node->next;
    }
}
