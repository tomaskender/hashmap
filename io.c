#include <stdio.h>
#include <ctype.h>

int get_word(char *s, int max, FILE *f) {
	char c;
	int length = 0;
	while((c = getc(f)) != EOF && !isspace(c)) {
		length++;
		if(length < max)
			s[length-1] = c;
		else if(length == max)
			fprintf(stderr, "Presiahol sa maximalny poceet znakov.\n");
	}

	if(c == EOF) {
		return EOF;
	} else {
		if(length < max) {
			// set EOS for strings shorter than MAX
			s[length] = '\0';
		} else {
			// set EOS for strings longer/equal
			s[max-1] = '\0';
		}
		return length;
	}
}
