#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

char identifiers[MAX_IDS][MAX_ID_LEN];
int ind = 0;

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};


void add_identifier(char* id){
	for(int i=0;i<32;i++)
		if(strcmp(id, keywords[i])==0) 
			return;
	for(int i=0;i<ind;i++)
		if(strcmp(id, identifiers[i])==0)
			return;
	strcpy(identifiers[ind++], id);
}

int find_idents() {
	int status=0;
	char id[MAX_ID_LEN]; int id_ind = 0;
	int prev='\n'; int curr;

	while ((curr=getchar())!=EOF) {
		switch (status) {
			case IN_LINE_COMMENT:
				if (curr=='\n') {
					status = 0;
					curr = 'a';
				}
				prev = curr;
				break;
			case IN_BLOCK_COMMENT:
				if (prev=='*' && curr=='/') {
					status = 0;
					curr = 'a';
				}
				prev = curr;
				break;
			case IN_STRING:
				if (prev!='\\' && (curr=='\"'||curr=='\''))
					status = 0;
				prev = curr;
                break;
			case IN_ID:
			    if (curr=='_'||(curr>='a'&&curr<='z')||(curr>='A'&&curr<='Z')||(id_ind>0&&(curr>='0'&&curr<='9')))
					id[id_ind++] = curr;
				else {
					status = 0;
					id[id_ind] = '\0';
					add_identifier(id);
					id_ind = 0;
					ungetc(curr, stdin);
				}
				prev=curr;
			    break;
			default:
				if (prev=='/' && curr=='/')
					status = IN_LINE_COMMENT;
				else if (prev=='/' && curr=='*') {
					status = IN_BLOCK_COMMENT;
					curr = 'a';
				}
				else if (curr=='_'||(curr>='a' && curr<='z')||(curr>='A' && curr<='Z')) {
					status = IN_ID;
					id[id_ind++] = curr;
				}
				else if (curr=='\"'||curr=='\'')
					status = IN_STRING;
				prev=curr;
				break;
		}
	}
	return ind;
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}
