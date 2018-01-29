/*
 * This example reads standard from input and writes to
 * the default PCM device for 5 seconds of data.
 */
/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
int main()
{
    long loops;
    int rc;
    int size;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;
    snd_pcm_uframes_t frames;
    char *buffer;

    /* Open PCM device for playback */
     rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    // rc = snd_pcm_open(&handle, "hw:0,0", SND_PCM_STREAM_PLAYBACK, 0);
    // rc = snd_pcm_open(&handle, "plughw:", SND_PCM_STREAM_PLAYBACK, 0);

    if (rc < 0) {
        printf("unable to open pcm device: %s\n", snd_strerror(rc));
        exit(1);
    }

    snd_pcm_hw_params_alloca(&params);

    snd_pcm_hw_params_any(handle, params);

    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

    snd_pcm_hw_params_set_channels(handle, params, 2);

    val = 44100;
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
    printf("rate : %u\n", val);

    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
    printf("frames : %d\n", frames);

    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        printf("unable to set hw parameters: %s\n", snd_strerror(rc));
        exit(1);
    }

    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    printf("period size : %d\n", frames);

    size = frames * 4;
    buffer = (char *)malloc(size);

    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    printf("val : %u\n", val);
    loops = 5000000 / val;

    while (loops > 0) {
        loops--;
        rc = read(0, buffer, size);

        rc = snd_pcm_writei(handle, buffer, frames);
        if (rc == -EPIPE) {
            printf("underrun occured\n");
        }
        else if (rc < 0) {
            printf("error from writei: %s\n", snd_strerror(rc));
        }
    }

    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);

    return 0;
}
