

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum {
  T_NUMBER,
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_END,
  T_INVALID
} TokenType;

typedef struct {
  TokenType type;
  double value;
  size_t pos;  
} Token;

static const char* src;
static size_t src_pos = 0;
static Token current;
static int had_error = 0;
static size_t error_pos = 0;

static void set_error(size_t pos) {
  if (!had_error) {
    had_error = 1;
    error_pos = pos + 1;  
  }
}

static void next_token() {
  while (isspace(src[src_pos])) src_pos++;
  char c = src[src_pos];

  if (c == '\0') {
    current.type = T_END;
    return;
  }

  current.pos = src_pos;

  // Parse number
  if (isdigit(c) || c == '.') {
    char* end;
    errno = 0;
    double val = strtod(src + src_pos, &end);
    if (errno != 0) {
      current.type = T_INVALID;
      return;
    }
    current.type = T_NUMBER;
    current.value = val;
    src_pos = end - src;
    return;
  }

  // Parse operators
  switch (c) {
    case '+':
      current.type = T_PLUS;
      src_pos++;
      return;
    case '-':
      current.type = T_MINUS;
      src_pos++;
      return;
    case '*':
      current.type = T_STAR;
      src_pos++;
      return;
    case '/':
      current.type = T_SLASH;
      src_pos++;
      return;
    default:
      current.type = T_INVALID;
      src_pos++;
      return;
  }
}

static double parse_expr();

static double parse_factor() {
  double value = 0;
  if (current.type == T_NUMBER) {
    value = current.value;
    next_token();
  } else {
    set_error(current.pos);
    next_token();
  }
  return value;
}

static double parse_term() {
  double left = parse_factor();

  while (current.type == T_STAR || current.type == T_SLASH) {
    TokenType op = current.type;
    size_t op_pos = current.pos;
    next_token();
    double right = parse_factor();

    if (op == T_SLASH) {
      if (fabs(right) < 1e-15) {  // division by zero
        set_error(op_pos);
        return 0;
      }
      left /= right;
    } else {
      left *= right;
    }
  }
  return left;
}

static double parse_expr() {
  double left = parse_term();

  while (current.type == T_PLUS || current.type == T_MINUS) {
    TokenType op = current.type;
    next_token();
    double right = parse_term();

    if (op == T_PLUS)
      left += right;
    else
      left -= right;
  }
  return left;
}

// Evaluate expression from string
static int evaluate(const char* text, double* result) {
  src = text;
  src_pos = 0;
  had_error = 0;
  error_pos = 0;

  next_token();
  double val = parse_expr();

  if (current.type != T_END && !had_error) set_error(current.pos);

  *result = val;
  return !had_error;
}

static char* read_file(const char* filename) {
  FILE* f = fopen(filename, "r");
  if (!f) return NULL;
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);
  char* buf = malloc(size + 1);
  fread(buf, 1, size, f);
  buf[size] = '\0';
  fclose(f);
  return buf;
}

static void write_result(const char* outpath, double value, int ok) {
  FILE* f = fopen(outpath, "w");
  if (!f) return;

  if (ok) {
    long long iv = (long long)llround(value);
    if (fabs(value - iv) < 1e-12)
      fprintf(f, "%lld\n", iv);
    else
      fprintf(f, "%.15g\n", value);
  } else {
    fprintf(f, "ERROR:%zu\n", error_pos);
  }
  fclose(f);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s input.txt\n", argv[0]);
    return 1;
  }

  const char* input = argv[1];
  char output[256];
  snprintf(output, sizeof(output), "result_Aditya_Panwar_241ADB112.txt");


  char* content = read_file(input);
  if (!content) {
    fprintf(stderr, "Cannot read input file: %s\n", input);
    return 1;
  }

  // Skip comment lines (#)
  char* filtered = malloc(strlen(content) + 1);
  filtered[0] = '\0';
  char* line = strtok(content, "\n");

  while (line) {
    const char* p = line;
    while (isspace(*p)) p++;
    if (*p != '#' && *p != '\0') {
      strcat(filtered, line);
      strcat(filtered, " ");
    }
    line = strtok(NULL, "\n");
  }

  double result;
  int ok = evaluate(filtered, &result);
  write_result(output, result, ok);

  free(content);
  free(filtered);

  return 0;
}
