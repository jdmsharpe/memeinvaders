#ifndef MACROS_H
#define MACROS_H

// Return if pointer is null
#define RETURN_IF_NULL(ptr)                                                    \
  if (!ptr) {                                                                  \
    return;                                                                    \
  }

// Continue loop if pointer is null
#define CONTINUE_IF_NULL(ptr)                                                  \
  if (!ptr) {                                                                  \
    continue;                                                                  \
  }

// Execute function if pointer is not null
#define EXECUTE_IF_VALID(ptr, routine)                                         \
  if (ptr) {                                                                   \
    (routine);                                                                 \
  }

// Execute function if two pointers are not null
#define EXECUTE_IF_BOTH_VALID(ptr1, ptr2, routine)                             \
  if (ptr1 && ptr2) {                                                          \
    (routine);                                                                 \
  }

#endif // MACROS_H