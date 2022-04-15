#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "player.h"
#include "interface.h"

static bool mm_player_exit = false;
static bool mm_player_working = false;

static int g_width;
static int g_height;

static void * mm_player_thread(void *args)
{
    int ret;

    while (!mm_player_exit)
    {
        if (!mm_player_working)
        {
            sleep(1);
            continue;
        }

        ret = mm_player_get_status();
        if (ret < 0)
        {
            printf("mmplayer has been closed!\n");
            mm_player_working = false;
            continue;
        }

        if (ret & AV_PLAY_ERROR)
        {
            mm_player_close();
            mm_player_exit = true;
        }
        else if (ret & AV_PLAY_LOOP)
        {
        }
        else if ((ret & AV_PLAY_COMPLETE) == AV_PLAY_COMPLETE)
        {
            mm_player_working = false;
            mm_player_close();
            printf("over\n");
            /*ret = mm_player_open(filename, 0, 0, g_width, g_height);
            if (ret < 0)
            {
                mm_player_exit = true;
            }*/
            mm_player_working = true;
        }
        av_usleep(50 * 1000);
    }

    return NULL;
}

void mmplayer_set_rotate(int mode)
{
    mm_player_set_opts("video_rotate", "", mode);
}

int mmplayer_start(int width, int height)
{
    pthread_t mm_thread = NULL;
    int ret;
    mm_player_set_opts("audio_device", "", 0);
    mm_player_set_opts("video_rotate", "", AV_ROTATE_NONE);
    mm_player_set_opts("video_only", "", 0);
    mm_player_set_opts("video_ratio", "", AV_SCREEN_MODE);
    mm_player_set_opts("enable_scaler", "", 0);
    mm_player_set_opts("resolution", "8294400", 0);
    mm_player_set_opts("play_mode", "", AV_LOOP);
    mm_player_set_opts("decoder_type", "", AV_SOFT_DECODING);

    g_width = width;
    g_height = height;
    mm_player_working = false;
    ret = pthread_create(&mm_thread, NULL, mm_player_thread, NULL);
    if (ret != 0) {
        goto exit;
    }

    mm_player_exit = false;

    return 0;

exit:
    mm_player_close();

    return ret;
}

int mmplayer_open(char *filename)
{
    mm_player_open(filename, 0, 0, g_width, g_height);
}

int mmplayer_stop(void)
{
    mm_player_exit = true;
    mm_player_close();
}
