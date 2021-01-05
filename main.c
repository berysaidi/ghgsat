#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define USAGE "parse an S-Expression file.\n Usage: \n\t parser filename \n"
#define BUFFER_SIZE 512


enum node_t {
	ATOM,
	LIST,
	INTEGER,
	FLOAT,
	DATE
};

static const char * NODE_STR_TYPES[] = {
	"ATOM",
	"LIST",
	"INTEGER",
	"FLOAT",
	"DATE"
};;

struct snode {
	struct snode *next;
	enum node_t type;
	union {
		struct snode *list;
		char *value;
	};
};


static bool is_float(char *str);
static bool is_integer(char *str);
static bool is_date(char *str);

static struct snode *snode_parse(FILE *fp);
static void snode_free(struct snode *node);
static void snode_print(struct snode *node, int tab);


bool is_float(char *str) {
	char *ptr = NULL;
	(void)strtod(str, &ptr);
	return !*ptr;
}

bool is_integer(char *str) {
	char *ptr = NULL;
	(void)strtol(str, &ptr, 10);
	return !*ptr;
}

bool is_date(char *str) {
	struct tm tm;
	return strptime(str, "%Y-%m-%dT%H:%M:%S", &tm);
}


// parse an s-expression from a file
struct snode *snode_parse(FILE *fp) {
	// Using a linked list, nodes are appended to the list tail until we 
	// reach a list terminator at which point we return the list head.
	struct snode *tail, *head = NULL;
	int c;

	while ((c = fgetc(fp)) != EOF) {
		struct snode *node = NULL;

		if (c == ')') {
			// Terminate list recursion
			break;
		} else if (c == '(') {
			// Begin list recursion
			node = malloc(sizeof(struct snode));
			node->type = LIST;
			node->list = snode_parse(fp);
		} else if (!isspace(c)) {
			(void)ungetc(c, fp);
			node = malloc(sizeof(struct snode));
			int len = 0;
			char buffer[BUFFER_SIZE + 1];
			// Tokenize it
			while ((c = fgetc(fp)) != EOF) {
				if (c == EOF || isspace(c) || c == '(' || c == ')') break;
				if (len>=BUFFER_SIZE){
					fprintf(stderr, "error: token too long\n");
					exit(1);
				}
				buffer[len] = c;
				len++;
			}
			buffer[len] = '\0';
			char *str = malloc((len + 1) * sizeof(char));
			strcpy(str, buffer);
			node->value = str;
			// Parse it
			if (is_integer(node->value)) {
			node->type = INTEGER;
			} else if (is_float(node->value)) {
				node->type = FLOAT;
			} else if (is_date(node->value)) {
				node->type = DATE;
			} else {
				node->type = ATOM;
			}

			// Put the terminator back
			(void)ungetc(c, fp);
		}

		if (node != NULL) {
			// Terminate the node
			node->next = NULL;
			
			if (head == NULL) {
				// Initialize the list head
				head = tail = node;
			} else {
				// Append the node to the list tail
				tail = tail->next = node;
			}
		}
	}

	return head;
}

// free memory allocated by a node
void snode_free(struct snode *node) {
	while (node != NULL) {
		struct snode *tmp = node;

		if (node->type == LIST) {
			snode_free(node->list);
		} else {
			free(node->value);
			node->value = NULL;
		}

		node = node->next;

		free(tmp);
		tmp = NULL;
	}
}

// display tabulated nodes
void snode_print(struct snode *node, int tab) {
	while (node != NULL) {
		struct snode *tmp = node;

		if (node->type == LIST) {
			tab++;
			snode_print(node->list, tab);
			tab--;
			printf("\n");
		} else {
			for(int i=0;i<tab;i++){
				printf("\t");
			}
			printf("%s: %s\n",NODE_STR_TYPES[node->type], node->value);
		}

		node = node->next;
		tmp = NULL;
	}
}

int main(int argc, char *argv[]) {
	if(argc < 2){
		fprintf(stderr, USAGE);
		return 0;
	}
	char *fname = argv[1];
	// open the file
	FILE *fp = fopen(fname, "r");
	if(!fp){
		fprintf(stderr, "error opening the file\n");
		return 0;
	}
	// parse the the tokens
	struct snode *node = snode_parse(fp);
	// display in a nested fashion
	snode_print(node,0);
	// cleanup 
	snode_free(node);
	fclose(fp);

	return 0;
}
