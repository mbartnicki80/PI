#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
	int znak=fgetc(stdin);
	int nlc=0; int nwc=0; int ncc=0;
	int czyslowo = 0;
	while (znak!=EOF) {
		if (znak=='\n') {
		    nlc++;
		}
		if (isspace(znak)) {
            czyslowo = 0;
        } else if (czyslowo==0) {
            czyslowo = IN_WORD;
            nwc++;
        }
        ncc++;
        znak=fgetc(stdin);
	}
	*nl = nlc;
	*nw = nwc;
	*nc = ncc;
}

void char_count(int char_no, int *n_char, int *cnt) {
	int znak=fgetc(stdin);
	while (znak!=EOF) {
		if (znak>=FIRST_CHAR && znak<LAST_CHAR) {
			count[znak-FIRST_CHAR]++;
		}
		znak=fgetc(stdin);
	}
	int indeks[MAX_CHARS];
    for (int i=0; i<MAX_CHARS; i++)
        indeks[i] = i;

    qsort(indeks, MAX_CHARS, sizeof(int), cmp);
	*cnt = count[indeks[char_no-1]];
	for (int i=0; i<MAX_CHARS; i++)
		if (count[i]==*cnt) {
			*n_char = FIRST_CHAR+i;
			break;
		}

}

void bigram_count(int bigram_no, int bigram[]) {
	int prev=fgetc(stdin);
	int curr=fgetc(stdin);
	while (curr!=EOF) {
		if (curr>=FIRST_CHAR && curr<LAST_CHAR && prev>=FIRST_CHAR && prev<LAST_CHAR) {
			count[(prev-FIRST_CHAR)*(MAX_CHARS+1)+(curr-FIRST_CHAR)]++;
		}
		prev = curr; curr = fgetc(stdin);
	}
    
	int indeks[MAX_BIGRAMS];
    for (int i=0; i<MAX_BIGRAMS; i++)
        indeks[i] = i;

    qsort(indeks, MAX_BIGRAMS, sizeof(int), cmp_di);
    
	bigram[2] =  count[indeks[bigram_no-1]];
	for (int i=0; i<MAX_BIGRAMS; i++)
		if (count[i]==bigram[2]) {
			bigram[0] = (int)(i/(MAX_CHARS+1))+FIRST_CHAR;
			bigram[1] = (i%(MAX_CHARS+1))+FIRST_CHAR;
			break;
		}
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
	int prev=fgetc(stdin);
	int curr=fgetc(stdin);
	int linec=0; int blockc=0;
	int czyblok=0; int czykom=0;
	while (curr!=EOF && prev!=EOF) {
		if (czyblok) {
			if (prev=='*' && curr=='/') {
				czyblok = 0;
				blockc++;
			}
		}
		else if (czykom) {
			if (curr=='\n') {
				czykom = 0;
				linec++;
			}
		}
		else if (curr=='*' && prev=='/') {
			prev = curr; curr = fgetc(stdin);
			czyblok = IN_BLOCK_COMMENT;
		}
			

		else if (curr=='/' && prev=='/') {
			prev = curr; curr = fgetc(stdin);
			czykom = IN_LINE_COMMENT;
		}
		prev = curr; curr = fgetc(stdin);
	}
	if (czykom)
		linec++;
	*line_comment_counter = linec;
	*block_comment_counter = blockc;

}

#define MAX_LINE 128

int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int bigram[3];

	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

