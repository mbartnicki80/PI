#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day; 
	int month; 
	int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
	char name[20];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2) {
	Date date1 = *(Date *)d1;
    Date date2 = *(Date *)d2;
	if (date1.year != date2.year)
        return date1.year - date2.year;
    if (date1.month != date2.month)
        return date1.month - date2.month;
    return date1.day - date2.day;
}

// compare foods
int cmp_qss(const void *d1, const void *d2){
    Food p1 = *(Food*)d1;
    Food p2 = *(Food*)d2;
    if(p1.price == p2.price)
        return cmp_date(d1, d2);
	else {
		if (p1.price-p2.price>0)
			return 1;
		else
			return -1;
	}
}

int cmp(const void *a, const void *b) {
	Food pa = *(Food*)a;
    Food pb = *(Food*)b;
	int art_cmp = strncmp(pa.name, pb.name, ART_MAX + 1);
	if (art_cmp) return art_cmp;
	double price_cmp = pa.price - pb.price;
	if (price_cmp<0) return 1;
	else if (price_cmp>0) return -1;
	return cmp_date(&pa.valid_date, &pb.valid_date);
}

// bsearch returning address where to insert new element
void *bsearch2 (const void *key, const void *base, size_t nmemb, size_t size, ComparFp compar, char *result) {
	size_t start = 0;
	size_t end = nmemb;
	char *blk = (char *)base;
	
	while (start<end) {
		size_t sr = (start+end)/2;
		char *element = blk+(sr*size);
		int cmp = compar(key, element);
		if (cmp==0) {
			*result = 1;
			return element;
		}
		if (cmp<0) 
			end = sr;
		else
			start = sr+1;
	}
	*result = 0;
	return blk+(start*size);
}

// print goods of given name
void print_art(Food *p, int n, char *art) {
	qsort(p, n, sizeof (Food), cmp_qss);
	for (int i = 0; i <n; i++) {
        if (strcmp(p[i].name, art) == 0) {
            if(p[i].valid_date.month < 10 && p[i].valid_date.day <10)
                printf("%.2f %d 0%d.0%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
			else if(p[i].valid_date.month < 10 && p[i].valid_date.day >10)
                printf("%.2f %d %d.0%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
			else if (p[i].valid_date.month > 10 && p[i].valid_date.day < 10)
                printf("%.2f %d 0%d.%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
			else
                printf("%.2f %d %d.%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
        }
    }
}

// add record to table if absent
Food* add_record(Food *tab, int *np, ComparFp compar, Food *new) {
	char found;
	Food *food = bsearch2(new, tab, *np, sizeof(Food), compar, &found);
	if (found) {
		food->amount += new->amount;
		return food;
	}
	else {
		int index = food - tab;
		for (int i = *np; i > index; i--)
			tab[i] = tab[i - 1];
		tab[index] = *new;
		(*np)++;
		return &tab[index];
	}
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {

	int count = 0;
	switch (sorted) {
		case 0:
			for (int i=0; i<no; i++) {
				Food c;
				fscanf(stream, "%s %f %d %d.%d.%d", c.name, &c.price, &c.amount, &c.valid_date.day, &c.valid_date.month, &c.valid_date.year);
				tab[count] = c;
				count++;
			}
			break;
		case 1:
			for (int i=0; i<no; i++) {
				Food c;
				fscanf(stream, "%s %f %d %d.%d.%d", c.name, &c.price, &c.amount, &c.valid_date.day, &c.valid_date.month, &c.valid_date.year);
				add_record(tab, &count, cmp, &c);	
			}
			break;
		default:
			return 0;
	}
	return count;
}

int cmp_qs(const void *a, const void *b) {
	Food *fa = (Food*)a;
	Food *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
	Date *pd = (Date*)a;
	Food *fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}


double date_diff(Date d1, Date d2) {
    struct tm t1 = {0};
    t1.tm_year = d1.year - 1900;
    t1.tm_mon = d1.month - 1;
    t1.tm_mday = d1.day;
    struct tm t2 = {0};
    t2.tm_year = d2.year - 1900;
    t2.tm_mon = d2.month - 1;
    t2.tm_mday = d2.day;
    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);
    double diff = difftime(time1, time2) / (60 * 60 * 24);
    return diff;
}

// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days) {
	qsort(food_tab, n, sizeof(Food), cmp_qs);
	float sum = 0;
	char found;
	Date exp_date = curr_date;
    exp_date.day += days;
	Food *food = bsearch2(&exp_date, food_tab, n, sizeof(Food), cmp_bs, &found);
	int i = 0;
	while (i<n) {
		if (abs(date_diff( food_tab[i].valid_date, exp_date)==0)) {
			sum += (food_tab[i].price*(float)food_tab[i].amount);
		}
			
		i++;
	}
	return sum;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
	enum Sex sex:1;
	enum BOOL in_line:1;
};

typedef struct {
	char *name;
	struct Bit_data bits;
	Date born;
	char *parent;
} Person;

typedef struct {
	char *par_name;
	int index;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
	if (person1->bits.sex == person2->bits.sex) return 0;
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
	return person2->bits.sex - person1->bits.sex;
}

int cmpp(Person *person1) {
	if (person1->bits.in_line == yes)
		return 1;
	return 0;
}
// compare persons
int cmp_person(const void *a, const void *b) {
	const Person *pa = a;
	const Person *pb = b;
	return cmp_date(&pa->born, &pb->born);
}

void print_person(const Person *p) {
	printf("%s\n", p->name);
}

int create_list(Person *person_tab, int n) {
	Person new_tab[n];
	Person temp[n];
	int czy[n];
	for (int i=0; i<n; i++)
		czy[i] = 0;

	int tmp = 0;
	int count = 1;
	int firsst = 0;
	Person first = person_tab[18];
	czy[18] = 1;
	new_tab[0] = first;
	while (count<n) {
		for (int i=0; i<n; i++) {
			if ((person_tab[i].parent==first.name)&&(czy[i]==0)) {
				temp[tmp++] = person_tab[i];
				czy[i] = 1;
			}
		}
		qsort(temp, tmp, sizeof(Person), cmp_person);

		//tu naprawic
		/*for (Person *c = new_tab + firsst; c >= new_tab; c--)
			memcpy(c+tmp, c, sizeof(Person));*/
		for (int j=0; j<tmp; j++)
			new_tab[count++] = temp[j];
		//count += tmp;
		tmp = 0;
		first = new_tab[++firsst];
	}

	/*for (int i=0; i<n; i++)
		if (cmpp(&new_tab[i]))
			person_tab[counter++] = new_tab[i];*/
	/*for (int i=0; i<n; i++)
		printf("%s\n", person_tab[i].name);*/
	return 0;
}

int main(void) {
	Person person_tab[] = {
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[ART_MAX];
	fgets(buff, ART_MAX, stdin);
	sscanf(buff, "%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 0);
			Date curr_date;
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d",&no);
			int no_persons = sizeof(person_tab) / sizeof(Person);
			create_list(person_tab,no_persons);
			print_person(person_tab + no - 1);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
