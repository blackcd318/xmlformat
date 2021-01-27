#include <stdio.h>

#define size 128

typedef enum bool\
{\
  false,\
  true}\
bool;

int addSpace(int n, FILE *out)
{
  if(n>0)
    for(int t=0; t<n; t++)
      fputs("  ", out);
  return n;
}

int main (int argc, char *argv[])
{
  FILE *in, *out;
  char buf[size];
  char outname[size] = "out_";
  int read_size;
  int num_space = 0;
  bool lf    = false; // '<'
  bool div   = false; // '/'
  bool rf    = false; // '>'
  bool first = false; // >[^\n]<
  if(argc !=2) {
    printf("usage :\n \
      %s <xml file>\n", argv[0]);
    return 1;
  }
  if(!(in = fopen(argv[1], "r+"))) {
    printf("Can't open in %s", argv[1]);
    return 2;
  }
  for(int t=0; t<size-4; t++) {
    if(argv[1][t]) outname[t+4] = argv[1][t];
    else {
      outname[t+4] = '\0';
      break;
    }
  }

  if(!(out = fopen(outname, "w+"))) {
    printf("Can't create output file");
    return 3;
  }

  while(read_size = fread(buf, 1, size, in )) {
    for(int t=0;t<read_size;t++)
    {
      if(buf[t] == '<') {
        lf = true;
        rf = false;
        if(first) { fputc('\n', out); first = false;}
        continue;
      }
      else if(buf[t] == '/') {
        if(lf) {
          addSpace(-- num_space, out);
          lf = false;
          fputc('<', out);
        }
        else div = true;
      }
      else if(buf[t] == '>') {
        if(div) num_space --;
        div = false;
        rf = true;
      } else if(buf[t] == '\n') continue ;
      else {
        if(lf) {
          if(buf[t] == '?') {addSpace(num_space, out); fputc('<',out); lf = false; }
          else { addSpace(num_space ++, out); fputc('<',out); lf = false; }
        }
        if(rf) {
          addSpace(num_space, out);
          rf = false;
          first = true;
        }
        div = false;
      }
      fputc(buf[t], out);
      if(buf[t] == '>') fputc('\n', out);
    }
  }  // while

  return 0;
}
