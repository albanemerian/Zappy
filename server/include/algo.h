/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** algo
*/

#ifndef ALGO_H_
    #define ALGO_H_

typedef struct tiles_s {
    int x;
    int y;
} tiles_t;

/* Algo.c */
tiles_t *shuffle_fisher(int width, int heigth);

#endif /* !ALGO_H_ */
