/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Unit tests for checkers
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "zappy.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(checkers, check_port_valid_range, .init = redirect_all_std)
{
    params_t params = {0};
    
    cr_assert_eq(check_port("-p", "1024", &params), true);
    cr_assert_eq(params.port, 1024);
    
    cr_assert_eq(check_port("-p", "65535", &params), true);
    cr_assert_eq(params.port, 65535);
}

Test(checkers, check_port_invalid_range, .init = redirect_all_std)
{
    params_t params = {0};
    
    cr_assert_eq(check_port("-p", "0", &params), false);
    cr_assert_eq(check_port("-p", "70000", &params), false);
    cr_assert_eq(check_port("-p", "-1", &params), false);
}

Test(checkers, check_dimensions_valid, .init = redirect_all_std)
{
    params_t params = {0};
    
    cr_assert_eq(check_width("-x", "10", &params), true);
    cr_assert_eq(params.x, 10);
    
    cr_assert_eq(check_height("-y", "20", &params), true);
    cr_assert_eq(params.y, 20);
}

Test(checkers, check_dimensions_invalid, .init = redirect_all_std)
{
    params_t params = {0};
    
    cr_assert_eq(check_width("-x", "0", &params), false);
    cr_assert_eq(check_height("-y", "-5", &params), false);
    cr_assert_eq(check_width("-x", "abc", &params), false);
}

Test(checkers, check_client_count, .init = redirect_all_std)
{
    params_t params = {0};
    
    cr_assert_eq(check_client("-c", "5", &params), true);
    cr_assert_eq(params.nb_client, 5);
    
    cr_assert_eq(check_client("-c", "0", &params), false);
    cr_assert_eq(check_client("-c", "xyz", &params), false);
}

Test(checkers, check_frequency, .init = redirect_all_std)
{
    params_t params = {0};
    
    cr_assert_eq(check_freq("-f", "100", &params), true);
    cr_assert_eq(params.freq, 100);
    
    cr_assert_eq(check_freq("-f", "1", &params), true);
    cr_assert_eq(params.freq, 1);
    
    cr_assert_eq(check_freq("-f", "0", &params), false);
    cr_assert_eq(check_freq("-f", "-10", &params), false);
}
