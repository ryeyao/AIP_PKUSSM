/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/19
 */
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
//#include <libavdevice/alsa-audio.h>
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include <mp3player.h>

void ffmpeg_fmt_to_alsa_fmt(AVCodecContext *pCodecCtx, snd_pcm_t *pcm, snd_pcm_hw_params_t *params) {
    switch (pCodecCtx->sample_fmt) {
        case AV_SAMPLE_FMT_U8:
            snd_pcm_hw_params_set_format(pcm, params, SND_PCM_FORMAT_S8);
            break;
        case AV_SAMPLE_FMT_S16:
            snd_pcm_hw_params_set_format(pcm, params, SND_PCM_FORMAT_S16);
            break;
        default:
            break;
    }
}

int cal_wav_frame_byte (AVCodecContext* pCodecCtx) {
    switch (pCodecCtx->sample_fmt) {
        case AV_SAMPLE_FMT_U8:
            return pCodecCtx->channels;
        case AV_SAMPLE_FMT_S16:
            return 2 * pCodecCtx->channels;
        default:
            return -1;
    }
}

void display_codec_ctx (AVCodecContext* pCodecCtx) {
    printf("fmt: %d\n", pCodecCtx->sample_fmt);
    printf("rate: %d\n", pCodecCtx->sample_rate);
}

long vol;

void *thrHdlr (void *arg) {
    char c;
    snd_mixer_elem_t *elem = (snd_mixer_elem_t *) arg;
    
    while (1) {
        c = getchar();
        if (c == 119) {
            snd_mixer_selem_set_playback_volume_all (elem, vol += 2000);
            snd_mixer_selem_set_playback_volume_all (elem, vol -= 2000);
        }
    }

    return ((void *) 0);
}
int main (int argc, char** argv) {

    /*ffmpeg parameters*/
    const char *fileName = "~/Music/the_sun_also_rises.mp3";

    AVFormatContext *pFmtCtx = NULL;
    int i, audioStream = -1;
    AVCodec *pCodec;
    AVPacket packet;
    AVFrame *pFrame;
    AVCodecContext *pCodecCtx;
    int len;
    int sel;
    int gotFrames;

    /*alsa parameters*/
    snd_pcm_t *pcm;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;
    snd_pcm_uframes_t buffer_size;
    snd_pcm_sw_params_t *swParams;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_id_t *sid;
    snd_mixer_t *mixer;
    long int min;
    long int max;
    int ret = 0;
    int frameByte = 0;
    pthread_t tid;

    av_register_all();
    
    avformat_open_input (&pFmtCtx, fileName, NULL, NULL);

    avformat_find_stream_info (pFmtCtx, NULL);

    for (i = 0; i < pFmtCtx->nb_streams; ++i) {
        if (( *(pFmtCtx->streams + i))->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStream = i;
            break;
        }
    }

    pCodecCtx = pFmtCtx->streams[audioStream]->codec;
    pCodec = avcodec_find_decoder (pFmtCtx->streams[audioStream]->codec->codec_id);
    avcodec_open2 (pCodecCtx, pCodec, NULL);
    pFrame = avcodec_alloc_frame();
    display_codec_ctx(pCodecCtx);

    snd_pcm_open (&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_hw_params_alloca (&params);
    snd_pcm_hw_params_any (pcm, params);
    snd_pcm_hw_params_set_access (pcm, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    ffmpeg_fmt_to_alsa_fmt (pCodecCtx, pcm, params);
    snd_pcm_hw_params_set_channels (pcm, params, pCodecCtx->channels);
    val = pCodecCtx->sample_rate;
    snd_pcm_hw_params_set_rate_near (pcm, params, &val, &dir);
    if (strcmp(pFmtCtx->iformat->name, "mp3") && strcmp(pFmtCtx->iformat->name, "ape")) {
        frameByte = cal_wav_frame_byte(pCodecCtx);
    }

    snd_pcm_hw_params (pcm, params);
    snd_pcm_sw_params_alloca (&swParams);
    snd_pcm_sw_params_current (pcm, swParams);
    snd_pcm_hw_params_get_buffer_size (params, &buffer_size);
    snd_pcm_sw_params_set_start_threshold (pcm, swParams, buffer_size);
    snd_pcm_sw_params (pcm, swParams);

    snd_mixer_selem_id_alloca (&sid);
    snd_mixer_selem_id_set_index (sid, 0);
    snd_mixer_selem_id_set_name (sid, "Master");
    snd_mixer_open (&mixer, 0);
    snd_mixer_attach (mixer, "default");
    snd_mixer_selem_register (mixer, NULL, NULL);
    snd_mixer_load (mixer);
    elem = snd_mixer_find_selem (mixer, sid);

    if (elem != NULL) {
        snd_mixer_selem_get_playback_volume (elem, 1, &vol);
        snd_mixer_selem_get_playback_volume_range (elem, &min, &max);
        pthread_create (&tid, NULL, thrHdlr, (void *)elem);
    }

    while (av_read_frame (pFmtCtx, &packet) >= 0) {
        if (packet.stream_index == audioStream) {
            gotFrames = 0;
            len = avcodec_decode_audio4 (pCodecCtx, pFrame, &gotFrames, &packet);
            if (len > 0) {
                if (gotFrames) {
                    ret = snd_pcm_writei (pcm, pFrame->data[0], pCodecCtx->frame_size);
                    if (ret < 0) {
                        ret = snd_pcm_recover (pcm, ret, 0);
                    } else {
                        int data_size = av_samples_get_buffer_size (
                                NULL, 
                                pCodecCtx->channels,
                                pFrame->nb_samples,
                                pCodecCtx->sample_fmt,
                                1
                                );
                        if ((ret = snd_pcm_writei (pcm, pFrame->data[0], data_size / frameByte)) < 0) {
                            snd_pcm_recover (pcm, ret, 0);
                        }
                    }

                }
            }
        }

        av_free_packet (&packet);
    }

    av_free (pFrame);
    avformat_close_input (&pFmtCtx);
    snd_mixer_close (mixer);
    snd_pcm_close (pcm);
    return 0;
}

