#ifndef UNIVERSAL_H
#define UNIVERSAL_H

#include <stdint.h>

enum log_Type {
  LOG_TRACE = 0,
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL
};

typedef void (*log_func_t)(enum log_Type level, const char *file, int line,
                           const char *func, const char *msg);

typedef struct {
  uint32_t version;
  log_func_t log_func;
} __attribute__((packed)) driver_params_t;

extern log_func_t main_log;

#define log_t(...)                                                             \
  main_log(LOG_TRACE, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_d(...)                                                             \
  main_log(LOG_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_i(...) main_log(LOG_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_w(...) main_log(LOG_WARN, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_e(...)                                                             \
  main_log(LOG_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_f(...)                                                             \
  main_log(LOG_FATAL, __FILE__, __LINE__, __func__, __VA_ARGS__)

#endif /* UNIVERSAL_H */
