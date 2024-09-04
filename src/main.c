#include <mpv/client.h>
#include <ncurses.h>
#include <string.h>

#include "utils/mainwindow.h"
#include "widgets/inputbox.h"
#include "widgets/playerbox.h"
#include "widgets/selectmenu.h"

void clean();

int main()
{
    struct mainwindow main = main_init();
    struct inputbox input = input_create(0, 0, main.width, "Search");
    struct selectmenu menu = menu_create(input.height, 0, main.height - input.height - 3, main.width, "Stations");
    struct playerbox player = player_create(menu.y + menu.height, 0, main.width);

    menu_add(&menu, "Code Radio", "https://coderadio-admin-v2.freecodecamp.org/listen/coderadio/radio.mp3");
    menu_add(&menu, "Chillofi", "http://streams.dez.ovh:8000/radio.mp3");
    menu_add(&menu, "Box Lofi", "https://play.streamafrica.net/lofiradio");
    menu_add(&menu, "Planet Lofi",
             "https://www.internet-radio.com/servers/tools/playlistgenerator/?u=http://198.245.60.88:8080/"
             "listen.pls?sid=1&t=.m3u");
    menu_focus(&menu);

    int in = ' ';
    char filter[1024] = "";
    while (true)
    {
        if (in != ERR)
        {
            if (input.focus)
            {
                enum instatus state = input_capture(&input, in);

                if (state == Enter)
                {
                    strcpy(filter, input.content);
                }
            }
            else
            {
                if (in == 'q')
                {
                    break;
                }
                else if (in == 'f')
                {
                    input_focus(&input);
                }

                struct selectitem *item = menu_update(&menu, in, filter);

                if (item != NULL)
                {
                    player_play(&player, item);
                }

                player_update(&player, in);
            }
        }

        in = getch();
    }

    menu_destroy(&menu);
    player_destroy(&player);
    return main_close();
}
