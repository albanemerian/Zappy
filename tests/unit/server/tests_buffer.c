/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Unit tests for buffer functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "zappy.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(buffer, basic_test, .init = redirect_all_std)
{
    // Basic test to ensure file compiles
    cr_assert(true, "Buffer tests placeholder");
}
