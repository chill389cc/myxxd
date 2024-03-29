#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *bits) {
  if (argc > 2) {
    printf("Usage: %s [-b|-bits]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2 &&
      (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
    *bits = TRUE;
  } else {
    *bits = FALSE;
  }

  return stdin;
}

/**
 * Writes data to stdout in hexadecimal.
 *
 * data: an array of no more than {rollover} characters
 * size: the size of the array
 * rollover: how many characters per line
 **/
void printDataAsHex(unsigned char *data, size_t size, size_t rollover) {
  int bytesPrinted = 0;
  printf(" ");
  for (size_t i = 0; i < rollover; i++) {
    if (i < size) {
      if (bytesPrinted < size) {
        printf("%02x",data[i]);
      }
      bytesPrinted++;
    } else {
      printf("  ");
    }
    if (i % 2) printf(" ");
  }
}

/**
 * Writes data to stdout in binary.
 *
 * data: an array of no more than {rollover} characters
 * size: the size of the array
 * rollover: how many characters per line
 **/
void printDataAsBits(unsigned char *data, size_t size, size_t rollover) {
  int bitsPrinted = 0;
  printf(" ");
  for (size_t i = 0; i < rollover; i++) {
    if (i < size) {
      if (bitsPrinted < size) {
        int temp = data[i];
        char s2[10];
        for (int j = 0; j < 9; j++) {
          if ( temp % 2 == 1) {
            s2[j] = '1';
          } else {
            s2[j] = '0';
          }
          temp /= 2;
        }
        s2[9] = 0;
        for (int j = 7; j >=0; j--) { //this loop prints the binary digits out backwards
          printf("%c", s2[j]);
        }
      }
      bitsPrinted++;
    } else {
      printf("        ");
    }
    printf(" ");
  }
}

/**
 * Writes data to stdout as characters.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (data[i] == 0x0a) {
      printf(".");
    } else {
      printf("%c",data[i]);
    }
  }
}

void readAndPrintInputAsHex(FILE *input) {
  const size_t ROLLOVER_CAP = 16;
  unsigned char data[ROLLOVER_CAP];
  int numBytesRead = fread(data, 1, ROLLOVER_CAP, input);
  unsigned int offset = 0;
  while (numBytesRead != 0) {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsHex(data, numBytesRead, (size_t) ROLLOVER_CAP);
    printf(" ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, ROLLOVER_CAP, input);
  }
}

/**
 * Bits output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input) {
  const int ROLLOVER_CAP = 6;
  unsigned char data[ROLLOVER_CAP];
  size_t numBytesRead = fread(data, 1, ROLLOVER_CAP, input);
  unsigned int offset = 0;
  while (numBytesRead != 0) {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsBits(data, numBytesRead, (size_t) ROLLOVER_CAP);
    printf(" ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, ROLLOVER_CAP, input);
  }
}

int main(int argc, char **argv) {
  int bits = FALSE;
  FILE *input = parseCommandLine(argc, argv, &bits);

  if (bits == FALSE) {
    readAndPrintInputAsHex(input);
  } else {
    readAndPrintInputAsBits(input);
  }
  return 0;
}
