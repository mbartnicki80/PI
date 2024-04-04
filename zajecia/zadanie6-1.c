#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(const pair* tab, int n) {

	for (int i=0; i<n; i++) {
		int znaleziono = 0;
		for (int j=0; j<n; j++) {
			if (tab[i].first==tab[j].first && tab[j].first==tab[j].second) {
				znaleziono = 1;
				break;
			}
		}
		if (znaleziono==0)
			return 0;
	}
	return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(const pair* tab, int n) {
	for (int i=0; i<n; i++) {
		int znaleziono = 0;
		for (int j=0; j<n; j++) {
			if (tab[i].first==tab[j].first && tab[j].first==tab[j].second) {
				znaleziono = 1;
				break;
			}
		}
		if (znaleziono==1)
			return 0;
	}
	return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(const pair* tab, int n) {
	for (int i=0; i<n; i++) {
		int znaleziono = 0;
		for (int j=0; j<n; j++) {
			if (tab[i].first==tab[j].second && tab[i].second==tab[j].first) {
				znaleziono = 1;
				break;
			}
		}
		if (znaleziono==0)
			return 0;
	}
	return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(const pair* tab, int n) {
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
			if (tab[i].first==tab[j].second && tab[i].second==tab[j].first)
				if (tab[i].first!=tab[i].second)
					return 0;
	return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(const pair* tab, int n) {
	for (int i=0; i<n; i++) {
		int znaleziono = 0;
		for (int j=0; j<n; j++) {
			if (tab[i].first==tab[j].second && tab[i].second==tab[j].first) {
				znaleziono = 1;
				break;
			}
		}
		if (znaleziono==1)
			return 0;
	}
	return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(const pair* tab, int n) {
	for (int i=0; i<n; i++) {
		int x = tab[i].first;
		int y = tab[i].second;
		for (int j=0; j<n; j++)
			if (tab[j].first==y) {
				int z = tab[j].second;
				int znaleziono = 0;
				for (int k=0; k<n; k++) {
					if (tab[k].first==x && tab[k].second==z) {
						znaleziono = 1;
						break;
					}
				}
				if (znaleziono==0)
					return 0;
			}
	}
	return 1;
}
// Case 2:

int insert_int (int *tab, int n, int new_element) {
	for (int i=0; i<n; i++)
		if (tab[i]==new_element)
			return 0;
	tab[n] = new_element;
	return 1;
}

int get_domain(const pair* tab, int n, int* elements) {
	int size = 0;
	for (int i=0; i<n; i++) {
		size += insert_int(elements, size, tab[i].first);
		size += insert_int(elements, size, tab[i].second);
	}
	return size;
}

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(const pair* tab, int n) {
	if (is_reflexive(tab, n)==1 && is_transitive(tab, n)==1 && is_antisymmetric(tab, n)==1)
		return 1;
	return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(const pair* tab, int n) {
	int all[MAX_REL_SIZE*2];
	int size = get_domain(tab, n, all);
	for (int i=0; i<size; i++) {
		int x = all[i];
		for (int j=i+1; j<size; j++) {
			int y = all[j];
			int znaleziono = 0;
			for (int k=0; k<n; k++)
				if ((tab[k].first==x && tab[k].second==y)||(tab[k].first==y && tab[k].second==x)) {
					znaleziono = 1;
					break;
				}
			if (znaleziono==0)
				return 0;
		}
	}
	return 1;
}

// A total order relation is a partial order relation that is connected
int is_total_order(const pair* tab, int n) {
	if (is_partial_order(tab, n)==1 && is_connected(tab, n)==1)
		return 1;
	else
		return 0;
}

int find_max_elements(const pair* tab, int n, int* elements) {
	int size = 0;
	for (int i=0; i<n; i++) {
		int g = tab[i].second;
		int znaleziono = 0;
		for (int j=0; j<n; j++) {
			if (tab[j].first==g && tab[j].second!=g) {
				znaleziono = 1;
				break;
			}
		}
		if (znaleziono==0)
			size += insert_int(elements, size, g);
	}
	return size;
}

int find_min_elements(const pair* tab, int n, int* elements) {
	int size = 0;
	for (int i=0; i<n; i++) {
		int m = tab[i].second;
		int znaleziono = 0;
		for (int j=0; j<n; j++) {
			if (tab[j].second==m && tab[j].first!=m) {
				znaleziono = 1;
				break;
			}
		}
		if (znaleziono==0)
			size += insert_int(elements, size, m);
	}
	return size;
}

//comparator for qsort
int cmp(const void* a, const void* b) {
   return ( *(int*)a - *(int*)b );
}

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
	for (int i=0; i<n; i++)
		if (tab[i].first==new_pair.first && tab[i].second==new_pair.second)
			return 0;
	tab[n] = new_pair;
	return 1;
}

void print_int_array(int *array, int n) {
	qsort(array, n, sizeof(int), cmp);
	for (int i=0;i<n;i++)
		printf("%d ", array[i]);
	printf("\n");
}

int composition (const pair* tab1, int n1, const pair* tab2, int n2, pair* res) {
	int size = 0;
	for (int i=0; i<n2; i++) {
		int x = tab1[i].second;
		for (int j=0; j<n1; j++) {
			if (tab2[j].first==x) {
				pair new_pair;
				new_pair.first = tab1[i].first;
				new_pair.second = tab2[j].second;
				size += add_relation(res, size, new_pair);
			}
		}
	}
	return size;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
	int n;
	int size = 0;
	scanf("%d", &n);
	for (int i=0; i<n; i++) {
		int a, b;
		scanf("%d %d", &a, &b);
		pair new_pair;
		new_pair.first = a;
		new_pair.second = b;
		size += add_relation(relation, i, new_pair);
	}
	return size;
}



int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;
	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			printf("%d\n", n_domain);
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			printf("%d\n", no_max_elements);
			print_int_array(max_elements, no_max_elements);
			printf("%d\n", no_min_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

