#include <obs-module.h>

#include <vlc/libvlc.h>

#ifdef _MSC_VER
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif

#include <vlc/libvlc_media.h>
#include <vlc/libvlc_events.h>
#include <vlc/libvlc_media_list.h>
#include <vlc/libvlc_media_player.h>
#include <vlc/libvlc_media_list_player.h>

extern libvlc_instance_t *libvlc;
extern uint64_t time_start;

extern bool load_libvlc(void);

/* libvlc core */
typedef libvlc_instance_t *(*LIBVLC_NEW)(int argc, const char *const *argv);
typedef void (*LIBVLC_RELEASE)(libvlc_instance_t *p_instance);
typedef int64_t (*LIBVLC_CLOCK)(void);
typedef int (*LIBVLC_EVENT_ATTACH)(libvlc_event_manager_t *p_event_manager,
		libvlc_event_type_t i_event_type,
		libvlc_callback_t f_callback,
		void *user_data);

/* libvlc media */
typedef libvlc_media_t *(*LIBVLC_MEDIA_NEW_PATH)(
		libvlc_instance_t *p_instance, const char *path);
typedef void (*LIBVLC_MEDIA_RETAIN)(libvlc_media_t *p_md);
typedef void (*LIBVLC_MEDIA_RELEASE)(libvlc_media_t *p_md);

/* libvlc media player */
typedef libvlc_media_player_t *(*LIBVLC_MEDIA_PLAYER_NEW)(
		libvlc_instance_t *p_libvlc);
typedef libvlc_media_player_t *(*LIBVLC_MEDIA_PLAYER_NEW_FROM_MEDIA)(
		libvlc_media_t *p_md);
typedef void (*LIBVLC_MEDIA_PLAYER_RELEASE)(
		libvlc_media_player_t *p_mi);
typedef void (*LIBVLC_VIDEO_SET_CALLBACKS)(
		libvlc_media_player_t *mp,
		libvlc_video_lock_cb lock,
		libvlc_video_unlock_cb unlock,
		libvlc_video_display_cb display,
		void *opaque);
typedef void (*LIBVLC_VIDEO_SET_FORMAT_CALLBACKS)(
		libvlc_media_player_t *mp,
		libvlc_video_format_cb setup,
		libvlc_video_cleanup_cb cleanup);
typedef void (*LIBVLC_AUDIO_SET_CALLBACKS)(
		libvlc_media_player_t *mp,
		libvlc_audio_play_cb play,
		libvlc_audio_pause_cb pause,
		libvlc_audio_resume_cb resume,
		libvlc_audio_flush_cb flush,
		libvlc_audio_drain_cb drain,
		void *opaque);
typedef void (*LIBVLC_AUDIO_SET_FORMAT_CALLBACKS)(
		libvlc_media_player_t *mp,
		libvlc_audio_setup_cb setup,
		libvlc_audio_cleanup_cb cleanup);
typedef int (*LIBVLC_MEDIA_PLAYER_PLAY)(
		libvlc_media_player_t *p_mi);
typedef void (*LIBVLC_MEDIA_PLAYER_STOP)(
		libvlc_media_player_t *p_mi);
typedef libvlc_time_t (*LIBVLC_MEDIA_PLAYER_GET_TIME)(
		libvlc_media_player_t *p_mi);
typedef int (*LIBVLC_VIDEO_GET_SIZE)(
		libvlc_media_player_t *p_mi,
		unsigned num,
		unsigned *px,
		unsigned *py);
typedef libvlc_event_manager_t *(*LIBVLC_MEDIA_PLAYER_EVENT_MANAGER)(
		libvlc_media_player_t *p_mp);

/* libvlc media list */
typedef libvlc_media_list_t *(*LIBVLC_MEDIA_LIST_NEW)(
		libvlc_instance_t *p_instance);
typedef void (*LIBVLC_MEDIA_LIST_RELEASE)(libvlc_media_list_t *p_ml);
typedef int (*LIBVLC_MEDIA_LIST_ADD_MEDIA)(libvlc_media_list_t *p_ml,
		libvlc_media_t *p_md);
typedef void (*LIBVLC_MEDIA_LIST_LOCK)(libvlc_media_list_t *p_ml);
typedef void (*LIBVLC_MEDIA_LIST_UNLOCK)(libvlc_media_list_t *p_ml);
typedef libvlc_event_manager_t *(*LIBVLC_MEDIA_LIST_EVENT_MANAGER)(
		libvlc_media_list_t *p_ml);

/* libvlc media list player */
typedef libvlc_media_list_player_t *(*LIBVLC_MEDIA_LIST_PLAYER_NEW)(
		libvlc_instance_t * p_instance);
typedef void (*LIBVLC_MEDIA_LIST_PLAYER_RELEASE)(
		libvlc_media_list_player_t *p_mlp);
typedef void (*LIBVLC_MEDIA_LIST_PLAYER_PLAY)(
		libvlc_media_list_player_t *p_mlp);
typedef void (*LIBVLC_MEDIA_LIST_PLAYER_PAUSE)(
		libvlc_media_list_player_t *p_mlp);
typedef void (*LIBVLC_MEDIA_LIST_PLAYER_STOP)(
		libvlc_media_list_player_t *p_mlp);
typedef void (*LIBVLC_MEDIA_LIST_PLAYER_SET_MEDIA_PLAYER)(
		libvlc_media_list_player_t *p_mlp,
		libvlc_media_player_t *p_mp);
typedef void (*LIBVLC_MEDIA_LIST_PLAYER_SET_MEDIA_LIST)(
		libvlc_media_list_player_t *p_mlp,
		libvlc_media_list_t *p_mlist);
typedef libvlc_event_manager_t *(*LIBVLC_MEDIA_LIST_PLAYER_EVENT_MANAGER)(
		libvlc_media_list_player_t *p_mlp);
typedef void (*LIBVLC_MEDIA_LIST_PLAYER_SET_PLAYBACK_MODE)(
		libvlc_media_list_player_t *p_mlp,
		libvlc_playback_mode_t e_mode);

/* -------------------------------------------------------------------- */

/* libvlc core */
extern LIBVLC_NEW libvlc_new_;
extern LIBVLC_RELEASE libvlc_release_;
extern LIBVLC_CLOCK libvlc_clock_;
extern LIBVLC_EVENT_ATTACH libvlc_event_attach_;

/* libvlc media */
extern LIBVLC_MEDIA_NEW_PATH libvlc_media_new_path_;
extern LIBVLC_MEDIA_RELEASE libvlc_media_release_;
extern LIBVLC_MEDIA_RETAIN libvlc_media_retain_;

/* libvlc media player */
extern LIBVLC_MEDIA_PLAYER_NEW libvlc_media_player_new_;
extern LIBVLC_MEDIA_PLAYER_NEW_FROM_MEDIA libvlc_media_player_new_from_media_;
extern LIBVLC_MEDIA_PLAYER_RELEASE libvlc_media_player_release_;
extern LIBVLC_VIDEO_SET_CALLBACKS libvlc_video_set_callbacks_;
extern LIBVLC_VIDEO_SET_FORMAT_CALLBACKS libvlc_video_set_format_callbacks_;
extern LIBVLC_AUDIO_SET_CALLBACKS libvlc_audio_set_callbacks_;
extern LIBVLC_AUDIO_SET_FORMAT_CALLBACKS libvlc_audio_set_format_callbacks_;
extern LIBVLC_MEDIA_PLAYER_PLAY libvlc_media_player_play_;
extern LIBVLC_MEDIA_PLAYER_STOP libvlc_media_player_stop_;
extern LIBVLC_MEDIA_PLAYER_GET_TIME libvlc_media_player_get_time_;
extern LIBVLC_VIDEO_GET_SIZE libvlc_video_get_size_;
extern LIBVLC_MEDIA_PLAYER_EVENT_MANAGER libvlc_media_player_event_manager_;

/* libvlc media list */
extern LIBVLC_MEDIA_LIST_NEW libvlc_media_list_new_;
extern LIBVLC_MEDIA_LIST_RELEASE libvlc_media_list_release_;
extern LIBVLC_MEDIA_LIST_ADD_MEDIA libvlc_media_list_add_media_;
extern LIBVLC_MEDIA_LIST_LOCK libvlc_media_list_lock_;
extern LIBVLC_MEDIA_LIST_UNLOCK libvlc_media_list_unlock_;
extern LIBVLC_MEDIA_LIST_EVENT_MANAGER libvlc_media_list_event_manager_;

/* libvlc media list player */
extern LIBVLC_MEDIA_LIST_PLAYER_NEW libvlc_media_list_player_new_;
extern LIBVLC_MEDIA_LIST_PLAYER_RELEASE libvlc_media_list_player_release_;
extern LIBVLC_MEDIA_LIST_PLAYER_PLAY libvlc_media_list_player_play_;
extern LIBVLC_MEDIA_LIST_PLAYER_PAUSE libvlc_media_list_player_pause_;
extern LIBVLC_MEDIA_LIST_PLAYER_STOP libvlc_media_list_player_stop_;
extern LIBVLC_MEDIA_LIST_PLAYER_SET_MEDIA_PLAYER libvlc_media_list_player_set_media_player_;
extern LIBVLC_MEDIA_LIST_PLAYER_SET_MEDIA_LIST libvlc_media_list_player_set_media_list_;
extern LIBVLC_MEDIA_LIST_PLAYER_EVENT_MANAGER libvlc_media_list_player_event_manager_;
extern LIBVLC_MEDIA_LIST_PLAYER_SET_PLAYBACK_MODE libvlc_media_list_player_set_playback_mode_;
