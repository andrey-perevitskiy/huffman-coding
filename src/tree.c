#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "tree.h"

void
t_get_codes (struct node * root, struct c_block * c_bl, int index, int h)
{
    if (root->left) {
        c_bl->bytes[index] = '0';

        t_get_codes(root->left, c_bl, index + 1, h);
    }

    if (root->right) {
        c_bl->bytes[index] = '1';

        t_get_codes(root->right, c_bl, index + 1, h);
    }

    if (n_is_leaf(root)) {
        /*   abcd
         *  /    \
         * a     bcd
         *      /   \
         *     b     cd
         *          /  \
         *         c    d <- let's say, we are here
         *
         * 'd' has code 111. I. e., c_bl->bytes = "111".
         *
         * root->data.s[0] is 'd';
         * root->data.f is frequency of 'd'.
         *
         * Append it into the c_bl->c_l, including the 'd' code. */
        struct c_data * data = malloc(sizeof(struct c_data));

        data->c = root->data.s[0];
        data->f = root->data.f;
        data->code = calloc(index + 1, sizeof(char));
        strncpy(data->code, c_bl->bytes, index);

        c_l_append(c_bl->c_l, data);

        free(data->code);
        free(data);
    }
}

int
t_get_height (struct node * root)
{
    int h_l;
    int h_r;

    if (!root)
        return 0;

    h_l = t_get_height(root->left);
    h_r = t_get_height(root->right);

    return h_l > h_r ? h_l + 1 : h_r + 1;
}

void
t_free (struct node * root)
{
    if (!root)
        return;

    t_free(root->left);
    t_free(root->right);

    n_free(root);
}

void
t_print (struct node * n, int y, int x, bool codes)
{
    int x_left = x;
    int x_right = x;

    move(y, x);
    printw("%s", n->data.s);

    if (n->left) {
        move(++y, --x_left);
        addch('/');

        if (codes)
            mvaddch(y, x_left - 1, '0');
    }

    if (n->right) {
        x_right += strlen(n->data.s);

        move(y, x_right);

        addch('\\');

        if (codes)
            mvaddch(y, x_right + 1, '1');
    }

    if (n->left)
        t_print(n->left, ++y, x_left -= strlen(n->left->data.s), codes);

    if (n->right)
        t_print(n->right, y, ++x_right, codes);
}
