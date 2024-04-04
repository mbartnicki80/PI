#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list node
typedef struct Node {
	int *data;
	size_t array_size;
	struct Node* next;
	struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
	Node *head;
	Node *tail;
} List;

// iterator
typedef struct iterator {
	struct Node* node_ptr;
	size_t position;
} iterator;

// forward initialization
iterator begin(Node* head) {
	iterator it = { head, 0 };
	return it;
}

// backward initialization;
// points to the element following the last one
iterator end(Node* tail) {
	iterator it = { tail, tail->array_size };
	return it;
}

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
	void *ptr = realloc(old_ptr, size);
	if (ptr) return ptr;
	free(old_ptr);
	exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t array_size, Node *next, Node *prev) {
	Node *node = safe_malloc(sizeof(Node));
	node->data = data;
	node->array_size = array_size;
	node->next = next;
	node->prev = prev;
	return node;
}

// initialize list
// creates the front and back sentinels
void init(List *list) {
	list->head = create_node(NULL, 0, NULL, NULL);
	list->tail = create_node(NULL, 0, NULL, list->head);
	list->head->next = list->tail;
}

// to implement ...
Node *create_node_s(int *data, size_t array_size, Node *next, Node *prev) {
	Node *node = safe_malloc(sizeof(Node));
	node->data = data;
	node->array_size = array_size;
	next->prev = node;
	node->next = next;
	prev->next = node;
	node->prev = prev;
	return node;
}
// append node to the list
void push_back(List *list, int *data, size_t array_size) {
	Node *push = create_node_s(data, array_size, list->tail, list->tail->prev);
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, size_t n) {
	Node *ele = itr->node_ptr;
    while (ele->array_size<n) {
        n -= ele->array_size;
        ele = ele->next;
    }
    itr->node_ptr = ele;
    itr->position = n;
}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n) {
	iterator itr = begin(list->head);
	skip_forward(&itr, n);
	if (itr.node_ptr!=list->tail)
		return itr.node_ptr->data[itr.position-1];
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, size_t n) {
	Node *ele = itr->node_ptr;
    while (ele->array_size<n) {
        n -= ele->array_size;
        ele = ele->prev;
    }
    itr->node_ptr = ele;
    itr->position = ele->array_size-n;
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n) {
	iterator itr = end(list->tail);
	skip_backward(&itr, n);
	if (itr.node_ptr!=list->head)
		return itr.node_ptr->data[itr.position];
}

void remove_node(Node *node_ptr) {
	Node *prev = node_ptr->prev;
	Node *next = node_ptr->next;
	prev->next = next;
	next->prev = prev;
	free(node_ptr);
}

// remove n-th element; if array empty remove node
void remove_at(List *list, size_t n) {
	iterator itr = begin(list->head);
    skip_forward(&itr, n);

	if (itr.node_ptr->array_size==1)
		remove_node(itr.node_ptr);
	else {
		int *tab = malloc((itr.node_ptr->array_size-1)*sizeof(int));
        memcpy(tab, itr.node_ptr->data, (itr.position-1)*sizeof(int));
        memcpy(&(tab[itr.position-1]), &(itr.node_ptr->data[itr.position]), (itr.node_ptr->array_size-1-(itr.position-1))*sizeof(int));
        free(itr.node_ptr->data);
        itr.node_ptr->data = tab;
        itr.node_ptr->array_size -= 1;
	}
}

// return the number of digits of number n
size_t digits(int n) {
	size_t sum = 0;
	if (n==0)
		return 1;
	else if (n<0)
		n = -n;
	while (n>0) {
		n /= 10;
		sum++;
	}
	return sum;
}

// inserts 'value' to the node with the same digits' count
// otherwise insert new node
void put_in_order(List *list, int value) {
	Node *ele = list->head;
	size_t valsize = digits(value);
	if (ele->next==list->tail) {
		int *tab = safe_malloc(sizeof(int));
		tab[0] = value;
		Node *new = create_node_s(tab, 1, list->tail, list->head);
		return;
	}
	while (ele!=list->tail) {
		if (ele->array_size>0) {
			if (digits(ele->data[0])==valsize) {
				size_t index = 0;
				for (size_t i=0; i<ele->array_size; i++) {
					if (value<=ele->data[i])
						break;
					index++;
				}
				ele->data = safe_realloc(ele->data, (ele->array_size+1)*sizeof(int));
				for (size_t i = ele->array_size; i > index; i--)
            		ele->data[i] = ele->data[i - 1];

				ele->data[index] = value;
				ele->array_size++;
				return;
			}
			else if (digits(ele->data[0])>valsize) {
				int *tab = safe_malloc(sizeof(int));
				tab[0] = value;
				Node *new = create_node_s(tab, 1, ele, ele->prev);
				return;
			}
		}
		ele = ele->next;
	}
	int *tab = safe_malloc(sizeof(int));
	tab[0] = value;
	Node *new = create_node_s(tab, 1, list->tail, list->tail->prev);

}

// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
	for(Node *node = list->head->next; node != list->tail; node = node->next) {
		printf("-> ");
		for (int k = 0; k < node->array_size; k++) {
			printf("%d ", node->data[k]);
		}
		printf("\n");
	}
}

// free list
void freeList(List *list) {
	Node *to_delete = list->head->next, *next;
	while(to_delete != list->tail) {
		next = to_delete->next;
		remove_node(to_delete);
		to_delete = next;
	}
}

// read int vector
void read_vector(int tab[], size_t n) {
	for (size_t i=0; i<n; ++i)
		scanf("%d", tab + i);
}

// initialize the list and push data
void read_list(List *list) {
	int n;
	size_t size;
	scanf("%d", &n); // number of nodes
	for (int i = 0; i < n; i++) {
		scanf("%zu", &size); // length of the array in i-th node
		int *tab = (int*) safe_malloc(size * sizeof(int));
		read_vector(tab, size);
		push_back(list, tab, size);
	}
}

int main() {
	int to_do, value;
	size_t size, m;
	List list;
	init(&list);

	scanf("%d", &to_do);
	switch (to_do) {
		case 1:
			read_list(&list);
			dumpList(&list);
			break;
		case 2:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_forward(&list, m));
			}
			printf("\n");
			break;
		case 3:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_backward(&list, m));
			}
			printf("\n");
			break;
		case 4:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				remove_at(&list, m);
			}
			dumpList(&list);
			break;
		case 5:
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%d", &value);
				put_in_order(&list, value);
			}
			dumpList(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	freeList(&list);

	return 0;
}
