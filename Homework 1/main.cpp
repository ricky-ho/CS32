#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int frobcmp(char const* a, char const*b);
int callFrob(const void* a, const void* b);
void checkIOError(FILE* fp, const char* msg);
void memError(void* ptr);
void skipSpaces(char* a, char* b);
void outputWords(char const* words);

int main()
{
    int z = EOF;
    unsigned int y = z ^ 42;
    printf("%d", y);
    
    /* DECLARE VARIABLES */
    char* curr_word;
    char** all_words;
    char curr_letter, next_letter;
    int is_EOF, num_word, num_letter;
   
    /* INITIALIZE VARIABLES */
    curr_word = (char*)malloc(sizeof(char));
    all_words = (char**)malloc(sizeof(char*));
    is_EOF = feof(stdin);
    num_word = 0;
    num_letter = 0;
    
    curr_letter = getchar();
    checkIOError(stdin, "Input");
    next_letter = getchar();
    checkIOError(stdin, "Input");
    
    while (! is_EOF)
    {
        /* Add the current letter into the current word and allocate more memory for the
           next set of characters */
        curr_word[num_letter] = curr_letter;
        char* new_letter = (char*) realloc(curr_word, (num_letter + 2) * sizeof(char));
        if (new_letter == NULL)
        {
            memError(curr_word);
        }
        else
        {
            num_letter += 1;
            curr_word = new_letter;
        }
        
        /* If the letter added was a space char, this indicates the end of a word thus
           we add that word into our array of words and allocate more memory for a new word
           and reset current word */
        if (curr_letter == ' ')
        {
            all_words[num_word] = curr_word;
            char** new_word = (char**) realloc(all_words, (num_word + 2) * sizeof(char*));
            if (new_word == NULL)
            {
                memError(all_words);
            }
            else
            {
                num_word += 1;
                all_words = new_word;
                curr_word = NULL;
                curr_word = (char*)malloc(sizeof(char));
                num_letter = 0;
            }
        }
        
        is_EOF = feof(stdin);
        if (curr_letter == ' ' && is_EOF)
            break;
        else if (curr_letter == ' ' && next_letter == ' ')
        {
            skipSpaces(&curr_letter, &next_letter);
            num_letter += 1;
            continue;
        }
        else if (is_EOF)
        {
            curr_letter = ' ';
            curr_word[num_letter] = curr_letter;
            all_words[num_word] = curr_word;
            
            char** new_word = (char**) realloc(all_words, (num_word + 2) * sizeof(char*));
            if (new_word == NULL)
            {
                memError(all_words);
            }
            else
            {
                num_word += 1;
                all_words = new_word;
                break;
            }
        }
        
        curr_letter = next_letter;
        next_letter = getchar();
        checkIOError(stdin, "Input");
    }
    
    qsort(all_words, num_word, sizeof(char*), callFrob);
    
    int i;
    for (i = 0; i < num_word; i++)
        outputWords(all_words[i]);
    
    int x = 0;
    for (x = 0; x < num_word; x++) {
        free(all_words[x]);
    }
    free(all_words);
    
    return 0;
}

void outputWords(char const* words)
{
    while (true)
    {
        putchar(*words);
        checkIOError(stdout, "Output");
        if (*words++ == ' ')
            return;
    }
}

/* Compares two words lexicographically by unfrobnicating letter by letter and comparing these
 letters. Returns negative, zero, or positive if a < b, a == b, a > b, respectively. */
int frobcmp(char const* a, char const*b)
{
    while (*a == *b)
    {
        if (*a == ' ')
            return 0;
        a += 1;
        b += 1;
    }
    if ((*a ^ 42) < (*b ^ 42))
        return -1;
    else
        return 1;
}

/* Used to pass frobcmp to qsort */
int callFrob(const void* a, const void* b)
{
    int value = frobcmp(*(char**) a, *(char**) b);
    return value;
}

/* If we received an input or output error, output to stderr and exit with status 1 */
void checkIOError(FILE* fp, const char* msg)
{
    if (ferror(fp) != 0)
    {
        fprintf(stderr, "%s Error!\n", msg);
        exit(1);
    }
}

/* We received a memory allocation error, thus we free all memory, output to stderr,
 and exit with status 1 */
void memError(void* ptr)
{
    free(ptr);
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
}

void skipSpaces(char* a, char* b)
{
    while (*a == ' ')
    {
        *a = getchar();
        checkIOError(stdin, "Input");
    }
    *b = getchar();
    checkIOError(stdin, "Input");
}
