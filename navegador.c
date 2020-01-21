#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 518401 //the max number that the int time (in minutes) can assume
//it is equal to 12 months * 30 days * 24 hours * 60 minutes + 1 minute =  518401


typedef struct Node{ //node that contains the relevant information of the site
	char tittle[32];
	char url[1026];
	int day, month, h, min, time;
	struct Node *next;
}node;


int time_convert (node *no){ //function that converts all the dates in to minutes
	int n;
	n = no->month*(30*24*60) + no->day*(24*60) + no->h*(60) + no->min;
	return n;
}


node *creat_node (char *tittle, char *url, int day, int month, int h, int min){
//dinamically allocates the memory and also converts the time into minutes


	node *no;
	no = (node*)malloc(sizeof(node)); //allocates the memory for the site
	strcpy(no->tittle, tittle);
	strcpy(no->url, url);
	no->day = day;
	no->month = month;
	no->h = h;
	no->min = min;
	no->time = time_convert(no);//converts the date to minutes
	return no;
}

node *remove_node (node **head, node **tail, char *tittle){
	//search a node, unlink it from the list and returns it

    node *temp = *head;
    node *prev;
    node *removed;


    if (temp != NULL && !strcmp(temp->tittle, tittle)){ //if the removed node is the first on the list
    	removed = *head;
        *head = temp->next;
        return removed;
    }

    while (temp != NULL && !!strcmp(temp->tittle, tittle)){//searches for the node
        prev = temp;
        temp = temp->next;
    }


    if (temp == NULL){ //if the node is not found
            return NULL;
    }
    // Unlink the node from linked list
    prev->next = temp->next;
    temp->next = NULL;
    removed = temp;
    if(!strcmp((*tail)->tittle, removed->tittle)){ //last node is removed
        *tail = prev;
    }
    //return the removed node
    return removed;
}

void insert_node_end (node **tail, node *no){//inserts a node in the end of the list
	node *temp = *tail;
	temp->next = no;
	no->next = NULL;
	(*tail) = no;
	return;
}

void insert_node_begin (node **head, node *no){//inserts a node at the begining of the list
	node *temp = *head;
	no->next = temp;
	(*head) = no;
	return;
}


void insert_node_pos(node **head, node **tail, node *no, int pos, int size){//insert a node in a position (pos)
//check in the case if there is only one element on the list

    node *temp = *head;
    int i;

    if(pos==1){//insert at the begin
        insert_node_begin(head, no);


    }else if(pos>size){
        insert_node_end(tail, no);
    }else{//insert in the middle of the list

        for(i=1; i<(pos-1); i++){
            temp = temp->next;
            //find the previous node of the node to be inserted
        }
        no->next = temp->next;
        temp->next = no;
    }

    return;
}

void insert(node** head, node** tail) {//reads the data and insert it into the list
    if (*head == NULL) { //if the list is empty it creats a list
        *head = (node*) malloc(sizeof(node));
        (*head)->next = NULL;
        scanf(" %[^\n]", (*head)->tittle);
        scanf(" %s", (*head)->url);
        scanf(" %d %d", &(*head)->day, &(*head)->month);
        scanf(" %d %d", &(*head)->h, &(*head)->min);
        (*tail) = (*head);
        (*head)->time = time_convert((*head));
    }
    else {//if the list is not empty, inserts at the end of the list
        node* temp = *head;
        while(temp->next != NULL) temp = temp->next;
        temp->next = (node*) malloc(sizeof(node));
        temp->next->next = NULL;
        scanf(" %[^\n]", temp->next->tittle);
        scanf(" %s", temp->next->url);
        scanf("%d %d", &temp->next->day, &temp->next->month);
        scanf("%d %d", &temp->next->h, &temp->next->min);
        temp->next->time = time_convert(temp->next);
        (*tail) = (temp->next);
    }

    return;
}



void delete_list(node **head, node **tail){//delets the list and desalocate all the memory dinamically alocated on the list

	if((*head) == NULL){ //empty list
		return;
	}

	node *temp = *head;
	node *next;

	while(temp != NULL){//deliting the nodes
		next = temp->next;
		free(temp);
		temp = next;
	}
	head = NULL;
}



void print_list(node *head){//prints the list

	if(head == NULL) return; //empty list;

	node *temp = head;

	while(temp != NULL){ //while the end of the list
		printf("%s ", temp->tittle);
		printf("%s ", temp->url);
		printf("%02d/%02d %02d:%02d\n", temp->day, temp->month, temp->h, temp->min);
		temp=temp->next;
	}
	printf("\n");
	return;
}


void radix (node **head, node **tail, int exp){//ordenates the list
	//10 pointers to 10 buckets one for each decimal digit
	node *h[10];
	node *t[10];
	int i;
	node *temp = *head;
	node *begin;
	node *end;

	for(i=0;i<10;i++){//inciates the pointers to the buckets
		h[i]=NULL;
		t[i]=NULL;
	}

	while(temp!=NULL){//creat the buckets
		i = (int) (temp->time%(exp*10))/exp;//selects the exp decimal digit
		//if exp = 1 selects last digit, if it is equal to 10 selects the second and so on

		if(h[i]==NULL){//if the bucket is empty
			h[i] = temp;
			t[i] = temp;
			temp=temp->next;
			t[i]->next=NULL;
		}else{
			//if the bucket is not empty the next element of the bucket is inserted
			t[i]->next = temp;
			t[i] = temp;
			temp=temp->next;
		}
	}

	i=0;

	while(h[i]==NULL && i<10){//finds the first bucket that is not empty
		i++;
	}

	begin = h[i];
	end = t[i];

	i++;

	while(i<10){//links the end of one bucket with the beggining of the next creating a new list sorted by the decimal digit
		if(h[i]!= NULL){
            end->next = h[i];
            end = t[i];
		}
		i++;
	}

	end->next = NULL;
	*head = begin;
	(*tail) = end;

	if((exp*10)<MAX){
		exp = exp*10;
		radix(&(*head), &(*tail), exp);//recursevely calls the radix fuction to ordenate the next digit
	}
	return;
}


int main(){

	node *head;
	node *tail;
	head = NULL;
	tail = NULL;
	
    node* new_node;
	int op;
	int pos;

	int size;
	size=0;

	char tittle[32];

	do{
		scanf("%d", &op);

		switch (op){
			case 1: //insert tab
                insert(&head, &tail);
				size++;
			break;

			case 2: //alterates the tab position

				scanf(" %[^\n]", tittle);
				scanf(" %d ", &pos);

				if(size>1){ //if there is one tab or with the list is empty we do nothing
                    new_node = (node *)remove_node(&head, &tail, tittle);
                    size--;
                    if(new_node != NULL){
                        insert_node_pos(&head, &tail, new_node, pos, size);
                    }
                    size++;
				}

			break;

			case 3: //ordenates the list sorted by the time previously calcullated
				if(size>1){
                    radix(&head, &tail, 1);
				}

			break;


			case 4: //exib the list
                if(head!=NULL){
                    print_list(head);
                }

			break;


			default:
				//printf("deafult\n");
			break;
		}
	}while(op!=5);

    delete_list(&head, &tail);
	//delete linked list and free allocated memory
	return 0;
}
