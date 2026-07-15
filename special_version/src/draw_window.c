/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdaniel- <tdaniel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:09:36 by tdaniel-          #+#    #+#             */
/*   Updated: 2026/07/15 17:12:46 by tdaniel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void open_game_window(t_game *game)
{

    game->window_ptr = mlx_new_window(
        game->mlx_ptr,
        game->columns * SIZE,
        game->rows    * SIZE,
        "so_long"
    );
    if (!game->window_ptr)
        ft_quit_game(game);

    draw_map(game);
    mlx_hook(game->window_ptr, KeyPress,       KeyPressMask,
             &process_player_input, game);
    mlx_hook(game->window_ptr, DestroyNotify,  StructureNotifyMask,
             &ft_quit_game, game);
}

bool display_window(t_game *game)
{
    int w, h;

    if (!game->mlx_ptr)
        return false;

    game->start_img[0] = mlx_xpm_file_to_image(
        game->mlx_ptr, "textures/start.xpm",
        &w, &h
    );
    game->start_img[1] = mlx_xpm_file_to_image(
        game->mlx_ptr, "textures/start1.xpm",
        &w, &h
    );
    if (!game->start_img[0] || !game->start_img[1])
        return false;

    game->img_size_x = w;
    game->img_size_y = h;

    game->window_ptr = mlx_new_window(
        game->mlx_ptr,
        game->img_size_x,
        game->img_size_y,
        "so_long"
    );
    if (!game->window_ptr)
        return false;

    game->start_frame = 0;
    game->animating   = true;
    return true;
}

int animate_start(void *param)
{
    t_game *g = param;
    static int  tick = 0;

    if (!g->animating)
        return 0;

    if (++tick < 2000)
    {
        mlx_put_image_to_window(
            g->mlx_ptr, g->window_ptr,
            g->start_img[g->start_frame],
            0, 0
        );
        return 0;
    }
    tick = 0;
    g->start_frame ^= 1;
    mlx_put_image_to_window(
        g->mlx_ptr, g->window_ptr,
        g->start_img[g->start_frame],
        0, 0
    );
    return 0;
}

int start_keypress(int keycode, void *param)
{
    t_game *g = param;

    if (g->animating && keycode == XK_space)
    {
        g->animating = false;

        mlx_destroy_image(g->mlx_ptr, g->start_img[0]);
        mlx_destroy_image(g->mlx_ptr, g->start_img[1]);
        mlx_destroy_window(g->mlx_ptr, g->window_ptr);

        open_game_window(g);
    }
    return 0;
}

void open_window(t_game *game)
{
    if (!display_window(game))
        ft_quit_game(game);

    mlx_loop_hook(game->mlx_ptr, &animate_start, game);
    mlx_hook     (game->window_ptr, KeyPress, KeyPressMask,
                  &start_keypress, game);
    mlx_loop(game->mlx_ptr);
}
