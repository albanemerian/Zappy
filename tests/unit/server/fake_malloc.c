/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** fake_malloc
*/

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <string.h>

static bool malloc_failure_enabled = false;
static int call_count = 0;
static int malloc_fail_after = 1000; // Default high threshold
static void *(*real_malloc)(size_t) = NULL;
static void *(*real_calloc)(size_t, size_t) = NULL;

static void init_real_functions(void)
{
    if (!real_malloc) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
    }
    if (!real_calloc) {
        real_calloc = dlsym(RTLD_NEXT, "calloc");
    }
}

// Control functions for tests
void enable_malloc_failure(int after_calls)
{
    malloc_failure_enabled = true;
    malloc_fail_after = after_calls;
    call_count = 0;
    fprintf(stderr, "[fake malloc] Enabled failure after %d calls\n", after_calls);
}

void disable_malloc_failure(void)
{
    malloc_failure_enabled = false;
    call_count = 0;
    fprintf(stderr, "[fake malloc] Disabled failure mode\n");
}

void reset_malloc_counter(void)
{
    call_count = 0;
}

void *malloc(size_t size)
{
    init_real_functions();
    
    if (malloc_failure_enabled) {
        call_count++;
        if (call_count > malloc_fail_after) {
            fprintf(stderr, "[fake malloc] Simulating malloc failure (call %d)\n", call_count);
            return NULL;
        }
    }
    
    return real_malloc(size);
}

void *calloc(size_t nmemb, size_t size)
{
    init_real_functions();
    
    if (malloc_failure_enabled) {
        call_count++;
        if (call_count > malloc_fail_after) {
            fprintf(stderr, "[fake calloc] Simulating calloc failure (call %d)\n", call_count);
            return NULL;
        }
    }
    
    return real_calloc(nmemb, size);
}
