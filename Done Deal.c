#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

typedef struct tree_node_s {
    float frequency;
    char c;
    char code[128];
    struct tree_node_s *left;
    struct tree_node_s *right;
} tree_node_t;
tree_node_t *arr[26], *letters[26];
void findMinAndSecondMin(tree_node_t **, float *, int *, float *, int *);
void printTree(tree_node_t *);
void interpret(char *, int *, tree_node_t *);
void printTree(tree_node_t *);
void encode(tree_node_t *, tree_node_t **, char, short, char*);
int main() {
    char str[128];
    int i, j,k, index, n,r;
    float min, secondMin;
    int minIndex, secondMinIndex;
    int numberOfCharacters = 0;
    tree_node_t *tree;
    FILE *fp = fopen("sdlab.txt", "r");
    if ( fp == NULL ) {
        printf("\nFile not found");
        return 0;
    } else {
        for (i = 'A'; i <= 'Z'; i++) {
            index = i - 'A';
            arr[index] = NULL;
        }//For having none values in each index;arr[26==NULL];
        numberOfCharacters = 0;
        r = fgets(str, 128, fp) != NULL;
        while(!feof(fp) || r) {
            n = strlen(str);
            printf("\n%s", str);
            for (i = 0; i < n ; i ++ ) {
                str[i] = toupper(str[i]);//Uppercase Conversion for any char
                if (str[i] >= 'A' && str[i] <= 'Z') {
                    numberOfCharacters =numberOfCharacters+1;
                    index = str[i] - 'A';
                    if (arr[index] == NULL) {
                        arr[index] =  malloc(sizeof(tree_node_t));
                        (*arr[index]).c = str[i];
                        (*arr[index]).frequency = 1;
                        (*arr[index]).left = (*arr[index]).right = NULL;
                    } else {
                        (*arr[index]).frequency ++ ;
                    }
                }
            }
            if (r==1) {
                r = fgets(str, 128, fp) != NULL;
            }
        }
    }
    fclose(fp);

    for ( i = 0, n = 0 ; i < 26 ; i ++ ) {
        letters[i] = arr[i];
        if (arr[i] != NULL) {
            //(*arr[i]).frequency = (*arr[i]).frequency/numberOfCharacters;    // Computing the frequency.
            n ++;                                       // n is the number of involved letters which is going to be consumed in the do while loop's condition
        }
    }

    j = 1;
    do {
        findMinAndSecondMin(arr, &min, &minIndex, &secondMin, &secondMinIndex);

        if (minIndex != -1 && secondMinIndex != -1 && minIndex != secondMinIndex) {
            tree_node_t *temp;
            tree = malloc(sizeof(tree_node_t));
            (*tree).frequency = (*arr[minIndex]).frequency + (*arr[secondMinIndex]).frequency;
            (*tree).c = j;
            (*tree).left = arr[minIndex];
            temp =malloc(sizeof(tree_node_t));
            (*temp).c = (*arr[secondMinIndex]).c;
            (*temp).frequency = (*arr[secondMinIndex]).frequency;
            (*temp).left = (*arr[secondMinIndex]).left;
            (*temp).right = (*arr[secondMinIndex]).right;
            (*tree).right = temp;

            arr[minIndex] = tree;

            arr[secondMinIndex] = NULL;
        }
        j ++;
    } while( j < n );

    for ( i = 0 ; i < 26 ; i ++ ) {
        if (arr[i] != NULL)  {
            char code[128];
            strcpy(code, "");
            encode(tree = arr[i], letters, 0, 0, code);
            puts("\nSuccessful encoding");
            printTree(arr[i]);
            break;
        }
    }
    fp = fopen("sdlab.txt", "r");

    r = fgets(str, 128, fp) != NULL;
    while(!feof(fp) || r) {
        n = strlen(str);
        for (i = 0; i < n ; i ++ ) {
            str[i] = toupper(str[i]);
            if (str[i] >= 'A' && str[i] <= 'Z') {
                index = str[i] - 'A';

            }
        }
        if (r) {
            r = fgets(str, 128, fp) != NULL;
        }
    }

    fclose(fp);


    printf("\nFile size (only letters) of the input file:%d bits", numberOfCharacters * 8);
    numberOfCharacters = 0;
    return 0;
}
//LEFT RIGHT
void encode(tree_node_t *node, tree_node_t **letters, char direction, short level, char* code) {
    int n;
    if ( node != NULL ) {
        if ((n = strlen(code)) < level) {
            if (direction == 'R') {
                strcat(code, "1");
            } else {
                if (direction == 'L') {
                    strcat(code, "0");
                }
            }
        } else {
            if (n >= level) {
                code[n - (n - level) - 1] = 0;
                if (direction == 'R') {
                    strcat(code, "1");
                } else {
                    if (direction == 'L') {
                        strcat(code, "0");
                    }
                }
            }
        }
        if ((*node).c >= 'A' && (*node).c <= 'Z') {
            strcpy((*node).code, code);
            strcpy((*letters[(*node).c - 'A']).code, code);
        }
        encode((*node).left, letters, 'L', level + 1, code);
        encode((*node).right, letters, 'R', level + 1, code);
    }
}

void printTree(tree_node_t *node) {
    int n;
    if ( node != NULL ) {
        if ((*node).c >= 'A' && (*node).c <= 'Z') {
            printf("\t%c - frequency: %.0f\tencoding: %s\n", (*node).c, (*node).frequency, (*node).code);
        }
        printTree((*node).left);
        printTree((*node).right);
    }
}

void findMinAndSecondMin(tree_node_t *arr[], float *min, int *minIndex, float *secondMin, int *secondMinIndex) {
    int i, k;
    k = 0;
    *minIndex = -1;
    while (k < 26 && arr[k] == NULL) k++;

    *minIndex = k;
    *min = (*arr[k]).frequency;

    for ( i = k ; i < 26; i ++ ) {
        if ( arr[i] != NULL && (*arr[i]).frequency < *min ) {
            *min = (*arr[i]).frequency;
            *minIndex = i;
        }
    }

    k = 0;
    *secondMinIndex = -1;
    while ((k < 26 && arr[k] == NULL) || (k == *minIndex && arr[k] != NULL)) k++;

    *secondMin = (*arr[k]).frequency;
    *secondMinIndex = k;

    if (k == *minIndex) k ++;

    for ( i = k ; i < 26; i ++ ) {
        if ( arr[i] != NULL && (*arr[i]).frequency < *secondMin && i != *minIndex ) {
            *secondMin = (*arr[i]).frequency;
            *secondMinIndex = i;
        }
    }
}

void interpret(char *str, int *index, tree_node_t *tree) {
    int n = strlen(str);
    if ((*tree).c >= 'A' && (*tree).c <= 'Z') {
        printf("%c ", (*tree).c);
        return ;
    } else {
        if ( *index < n ) {
            if (str[*index] == '0') {
                (*index) ++;
                interpret(str, index, (*tree).left);
            } else {
                if (str[*index] == '1') {
                    (*index) ++;
                    interpret(str, index, (*tree).right);
                }
            }
        }
    }
}

