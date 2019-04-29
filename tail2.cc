#include <string>
#include <queue>
#include <iostream>
#include <cstdio>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
    // not using stdio, so I disable syncing
    ios::sync_with_stdio(false);

    int line_cnt = 10;
    bool b_ReadFromBeginning = false;
    istream *in = NULL;
    ifstream ifn;

    if(argc > 4) {
        fprintf(stderr, "Max 3 arguments expected, which consist of \"FILENAME\" and \"-n NUMBER\" in random order\n");
        exit(1);
    }

    for(int i=1; i<argc; i++) {
        if((string)argv[i] == "-n") {
            // move to VALUE that is supposed to be found after -n
            i++;

            if(i >= argc) {
                fprintf(stderr, "-n VALUE not found.\n");
                return 1;
            }

            // VALUE argument format is -x, x, +x or -+x, where x is a number
            int p = 0;

            if(argv[i][p] == '-') {;
                p++;
            }

            if(argv[i][p] == '+') {
                b_ReadFromBeginning = true;
                p++;
            }

            char *end_ptr;
            line_cnt = strtol(&(argv[i][p]), &end_ptr, 10);

            if(*end_ptr != '\0') {
                fprintf(stderr, "-n VALUE is not a number.\n");
                exit(1);
            }

     
            // throws error for non-positive values
            if(line_cnt <= 0) {
                fprintf(stderr, "-n VALUE must be positive and it is %d.\n", line_cnt);
                exit(1);
            }
        } else {
            // error checking for file opening
            ifn.exceptions(ifstream::failbit | ifstream::badbit);
            try {
                ifn.open(argv[i]);
            } catch(system_error& e) {
                if(!in) {
                    fprintf(stderr, "File %s could not be loaded!\n", argv[i]);
                    exit(1);
                }
            }
            // reset
            ifn.exceptions(ifstream::goodbit);
            in=&ifn;
        }
    }

    // in not loaded -> load from stdin
    if(!in)
        in = &cin;

    queue<string> q;
    string buff;

    if(b_ReadFromBeginning) {
        // skip n-1 lines to start reading at n-th line
        for( int i=0; getline(*in, buff); i++) {
            if(i>=line_cnt-1)
                q.push(buff);
        }
    } else {
        while(getline(*in, buff)) {
            if(q.size() == (unsigned)line_cnt)
                q.pop();
            q.push(buff);
        }
    }

    // print queue
    while(!q.empty()) {
        cout << q.front() << endl;
        q.pop();
    }
    
    return 0;
}
