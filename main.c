#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <curses.h>

#define DIM(arr) (sizeof(arr) / sizeof(arr[0]))

const char* hiragana[][2] = {
    { u8"あ", "a" },
    { u8"い", "i" },
    { u8"う", "u" },
    { u8"え", "e" },
    { u8"お", "o" }
};

typedef enum {
    ResultErr,
    ResultOk
} ResultVariant;

typedef struct {
    ResultVariant var;
    void* val;
} Result;

typedef struct {
    int len;
    char* str;
} GetStrHeapVal;

bool result_is_ok(Result* res) {
    bool is_ok = res->var == ResultOk;
    return is_ok;
}

char* append_buf(int outlen, int buflen, char* out, char* buf) {
    // no need to add + 1 since buflen already accounts for null terminator
    char* newout = calloc(outlen + buflen, sizeof(char));
    if (outlen > 0) {
        strcpy(newout, out);
        free(out);
        strcat(newout, buf);
    } else {
        strcpy(newout, buf);
    }
    return newout;
}

Result getstrheap() {
    int len = 0;
    char* out = (char*) 0;
    char buf[5] = { 0 };
    int i = 0;
    
    while (true) {
        int ch = getch();

        if (ch == ERR) {
            Result res = {
                .var = ResultErr,
                .val = (void*) 0
            };
            return res;
        }

        if (ch == '\n' || ch == '\r') {
            break;
        }

        buf[i++] = (char) ch;

        if (i >= DIM(buf) - 1) {
        //                ^^^ leave null terminator
            out = append_buf(len, DIM(buf), out, &buf);
            len += DIM(buf) - 1;
            //              ^^^ leave null terminator

            memset(&buf, 0, DIM(buf));
            i = 0;
        }
    }

    out = append_buf(len, DIM(buf), out, &buf);
    len += strlen(&buf);
    
    GetStrHeapVal* val = malloc(sizeof(GetStrHeapVal));
    *val = (GetStrHeapVal) {
        .len = len,
        .str = out
    };
    Result res = {
        .var = ResultOk,
        .val = val
    };
    return res;
}

int main() {
   //  srand(time(0));
   // 
   //  while (true) {
   //      int i = rand() % DIM(hiragana);
   //      const char* kanachar = hiragana[i][0];
   //      const char* romanjistr = hiragana[i][1];
   //
   //      printf("%s\n", kanachar);
   //
   //      char in[2];
   //      int result;
   //      printf("romanji? ");
   //      result = scanf("%1s", in);
   //
   //      result = strcmp(romanjistr, in);
   //      
   //      printf("You answered \"%s\"\n", in);
   //      if (result == 0) {
   //          printf("CORRECT!\n");
   //      } else {
   //          printf("WRONG!\ncorrect answer: %s\n", romanjistr);
   //      }
   //  }

    // initscr();
    //
    // while (true) {
    //     int in = getch();
    //     char* str1 = "Hello, world! pressed: ";
    //     char* str2 = &in;
    //     char* str = malloc(strlen(str1) + 2);
    //     
    //     strcpy(str, str1);
    //     strcat(str, str2);
    //     printw(str);
    //     refresh();
    // }
    //
    // endwin();
    
    initscr();
    
    Result res = getstrheap();

    endwin();

    if (!result_is_ok(&res)) {
        printf("ERR");
    }

    GetStrHeapVal* val = (GetStrHeapVal*) res.val;
    char* str = val->str;

    printf(str);

    return 0;
}
