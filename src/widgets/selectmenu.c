#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "keycodes.h"
#include "selectmenu.h"

struct selectmenu menu_create(int y, int x, int height, int width, char *label)
{
    WINDOW *win = newwin(height, width, y, x);
    refresh();

    box(win, 0, 0);
    mvwprintw(win, 0, 3, "%s", label);
    wrefresh(win);

    return (struct selectmenu){
        .raw = win, .height = height, .width = width, .y = y, .x = x, .active = -1, .count = 0, .focus = false};
}

void menu_add(struct selectmenu *menu, char *label, char *url)
{
    struct selectitem *item = (struct selectitem *)malloc(sizeof(struct selectitem));
    strcpy(item->label, label);
    item->length = strlen(label);
    strcpy(item->url, url);

    item->order = menu->count + 1;
    if (menu->active == -1)
    {
        menu->active = 0;
    }

    menu->items[menu->count] = item;
    menu->count++;
}

void menu_focus(struct selectmenu *menu)
{
    menu->focus = true;
}

struct selectitem *menu_update(struct selectmenu *menu, char key, char *filter)
{
    switch (key)
    {
    case 'k':
        menu->active--;
        if (menu->active < 0)
        {
            menu->active = 0;
        }
        break;
    case 'j':
        menu->active++;
        if (menu->active >= menu->count)
        {
            menu->active = menu->count - 1;
        }
        break;
    case KEY_CONFIRM:
        return menu->items[menu->active];
    default:
        break;
    }

    for (int i = 0; i < menu->count; i++)
    {
        if (i == menu->active && menu->focus)
        {
            wattron(menu->raw, A_REVERSE);
        }
        mvwprintw(menu->raw, i + 1, 1, "%s", menu->items[i]->label);
        wattroff(menu->raw, A_REVERSE);
    }

    struct selectitem *selected = menu->items[menu->active];
    int posX = 1 + selected->length;
    int posY = 1 + menu->active;
    wmove(menu->raw, posY, posX);
    wrefresh(menu->raw);

    return NULL;
}

void menu_destroy(struct selectmenu *menu)
{
    delwin(menu->raw);
}
