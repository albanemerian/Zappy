/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** fake_malloc
*/

#ifndef FAKE_MALLOC_H_
    #define FAKE_MALLOC_H_


void enable_malloc_failure(int after_calls);
void disable_malloc_failure(void);
void reset_malloc_counter(void);
void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);

#endif /* !FAKE_MALLOC_H_ */
