#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
	struct ListElement *next;
	void *data;
} ListElement;

typedef struct {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	DataFp modify_data;
} List;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	printf("malloc error\n");
	exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
	void *ptr = strdup(string);
	if(ptr) return ptr;
	printf("strdup error\n");
	exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
		CompareDataFp compare_data, DataFp modify_data) {
	p_list->head = NULL;
    p_list->tail = NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->modify_data = modify_data;
}

// Print elements of the list
void dump_list(const List* p_list) {
    for (ListElement *i=p_list->head; i!=NULL; i=i->next)
        p_list->dump_data(i->data);
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
    for (ListElement *i=p_list->head; i!=NULL; i=i->next)
		if (p_list->compare_data(i->data, data)==0)
        	p_list->dump_data(i->data);
}

// Free all elements of the list
void free_list(List* p_list) {
	p_list->tail = NULL;
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data) {
	ListElement *element = safe_malloc(sizeof(ListElement));
    element->data = data;
    element->next = p_list->head;
    if(!p_list->head)
        p_list->tail = element;
    p_list->head = element;
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
	ListElement *element = safe_malloc(sizeof(ListElement));
    element->data = data;
    element->next = NULL;
    if (p_list->tail)
        p_list->tail->next = element;
    p_list->tail = element;
    if(!p_list->head)
        p_list->head = p_list->tail;
}

// Remove the first element
void pop_front(List *p_list) {
	if (p_list->head!=NULL) {
        ListElement *element = p_list->head;
        if (p_list->head == p_list->tail) {
            p_list->head = NULL;
            p_list->tail = NULL;
        }
        else
            p_list->head = element->next;
        p_list->free_data(element->data);
        free(element);
    }
}

// Reverse the list
void reverse(List *p_list) {
	ListElement *prev = p_list->head;
    if (prev!=NULL) {
        ListElement *curr = prev->next;
        prev->next = NULL;
        ListElement *tmp;
        while (curr!=NULL) {
            tmp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = tmp;
        }
        tmp = p_list->head;
        p_list->head = p_list->tail;
        p_list->tail = tmp;
    }
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
    ListElement *prev = NULL;
	for(ListElement *i=p_list->head; i!=NULL; i=i->next) {
		int w = p_list->compare_data(i->data, p_element->data);
		if (w>0)
        	break;
		else if (w==0)
			return p_element;
		prev = i;
	}	
    return prev;
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
	ListElement *element = safe_malloc(sizeof(ListElement));
	element->data = data;

    if(previous==NULL) {
        element->next = p_list->head;
        p_list->head = element;
    }
    else {
        element->next = previous->next;
        previous->next = element;
    }
	if(element->next==NULL)
        p_list->tail = element;
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
	ListElement * element = safe_malloc(sizeof(ListElement));
    element->data = p_data;
    ListElement * prev = find_insertion_point(p_list, element);
    if(prev!=element)
        push_after(p_list, p_data, prev);
    free(element);
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element
typedef struct IntData{
    int data;
} IntData;

void dump_int(const void *d) {
	IntData * v = (IntData *)d;
    printf("%d ", v->data);
}

void free_int(void *d) {
	free(d);
}

int cmp_int(const void *a, const void *b) {
	IntData * v1 = (IntData*)a;
    IntData * v2 = (IntData*)b;
    return v1->data-v2->data;
}

IntData *create_data_int(int v) {
	IntData *element = safe_malloc(sizeof(IntData));
    element->data = v;
    return element;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word (const void *d) {
    DataWord *w = (DataWord *)d;
    printf("%s ", w->word);
}

void dump_word_lowercase (const void *d) {
    DataWord *w = (DataWord *)d;
    for(int i=0; w->word[i]; i++)
        printf("%c", tolower(w->word[i]));
    printf(" ");
}

void free_word(void *d) {
	free(d);
}

int str_cmp(char const *a, char const *b)
{
	while (1) {
		int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
		a++; b++;
	}
}

int cmp_word_alphabet(const void *a, const void *b) {
    DataWord *w1 = (DataWord *)a;
    DataWord *w2 = (DataWord *)b;
    return str_cmp(w1->word, w2->word);
}

int cmp_word_counter(const void *a, const void *b) {
	return ((DataWord*)a)->counter-((DataWord*)b)->counter;
}

void modify_word(void *p) {
	((DataWord*)p)->counter++;
}

void *create_data_word(char *string, int counter) {
    DataWord *newDataWord = (DataWord *)malloc(sizeof(DataWord));
    newDataWord->word = (char *)malloc((strlen(string) + 1) * sizeof(char));
    strcpy(newDataWord->word, string);
    newDataWord->counter = counter;
    return (void *)newDataWord;
}
void insert_elem_in_order(List *p_list, void *data) {

    ListElement *list_element = p_list->head;
	for(ListElement *i=p_list->head; i!=NULL; i=i->next) {
		if(p_list->compare_data(i->data, data)==0) {
            DataWord *word = ( DataWord *)i->data;
            word->counter++;
            return;
        }
	}

    ListElement *element = safe_malloc(sizeof(ListElement));
    element->data = data;
    ListElement *insert = find_insertion_point(p_list, element);

	
    if (insert) {
		element->next = insert->next;
        insert->next = element;
		if(element->next==NULL) 
			p_list->tail = element;
    }
    else {
        element->next = p_list->head;
        p_list->head = element;
    }

    DataWord *word = (DataWord *)element->data;
    word->counter = 1;
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
    DataWord *data;
    char *p, buf[BUFFER_SIZE];
    char delimits[] = "\t\n\v\f\r .,?!:;-.";
    if (cmp) 
		p_list->compare_data = cmp;
    while (fgets(buf, BUFFER_SIZE, stream)!=NULL) {
        p = strtok(buf, delimits);
        while (p!=NULL) {
            data = create_data_word(p, 1);
            if (cmp)
				insert_elem_in_order(p_list, data);
            else
				push_back(p_list, data);
            p = strtok(NULL, delimits);
        }
    }
}

// test integer list
void list_test(List *p_list, int n) {
	char op;
	int v;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		switch (op) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int, cmp_int, NULL);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word, NULL, NULL);
			stream_to_list(&list, stdin, NULL);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word_lowercase, free_word, NULL, modify_word);
			stream_to_list(&list, stdin, cmp_word_alphabet);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
			dump_list_if(&list, &data);
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

