#ifndef XERXES_LOGGING_H
#define XERXES_LOGGING_H

#include <iostream>

#define RED   "\x1B[31m" // Color Terminal Red
#define YEL   "\x1B[33m" // Color Terminal Yellow
#define GRN   "\x1B[32m" // Color Terminal Green
#define RESET "\x1B[0m" // Color Terminal Reset color

#define print_error(err)  fprintf(stderr, "%s%s %s%s\n", RED, "[ERROR]:", (err), RESET);
#define print_info(inf)  fprintf(stdout, "%s%s %s%s\n", GRN, "[INFO]:", (inf), RESET);
#define print_warning(warn)  fprintf(stdout, "%s%s %s%s\n", YEL, "[WARNING]:", (warn), RESET);

#endif //XERXES_LOGGING_H
