#include "libretro.h"
#include "emul.h"
#include <string.h>

// Caractéristiques du système émulé
// Fréquence vidéo (60Hz)
#define VIDEO_FPS 60
// Fréquence audio (22kHz)
#define AUDIO_SAMPLE_RATE 22050
// Nombre de pixels X/Y de l'écran
#define XBITMAP 80
#define YBITMAP 50
// Pitch = longueur en octets entre 2 lignes du buffer vidéo
#define PITCH (2 * XBITMAP)
// Fréquence processeur
#define PROC_FREQ 1000000
// Nombre d'instructions exécutées pendant la durée d'une frame vidéo
#define NB_INSTR_PER_FRAME (PROC_FREQ / VIDEO_FPS)

// Callback d'accès à la configuration du frontend
static retro_environment_t environ_cb = NULL;
// Callback de mise à jour du buffer vidéo
static retro_video_refresh_t video_cb = NULL;
// Callbacks pour ajouter des échantillons au buffer audio
static retro_audio_sample_batch_t audio_batch_cb = NULL;
// Callback pour mettre à jour l'état des entrées (clavier, manette)
static retro_input_poll_t input_poll_cb = NULL;
// Callback pour lire l'état courant d'une touche/bouton
static retro_input_state_t input_state_cb = NULL;
// Buffer vidéo au format RGB565
static unsigned short *video_buffer = NULL;

unsigned retro_api_version(void)
{
  return RETRO_API_VERSION;
}

void retro_get_system_info(struct retro_system_info *info)
{
  memset(info, 0, sizeof(*info));
  // Nom de l'émulateur
  info->library_name = "mon_emu";
  // Version de l'émulateur
  info->library_version = "1.0";
  // Liste d'extensions de fichiers qui peuvent être chargés par l'émulateur
  info->valid_extensions = "bin";
  // Inutile d'avoir le chemin complet du fichier chargé, Libretro va le charger pour nous
  info->need_fullpath = false;
  // Autorise Libretro à extraire le fichier d'une archive avant son chargement
  info->block_extract = false;
}

void retro_set_environment(retro_environment_t env)
{
  // L'émulateur nécessite le chargement d'un support
  bool no_rom = false;
  env(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);

  environ_cb = env;
}

void retro_init(void)
{
  video_buffer = (unsigned short *)ram;
}

bool retro_load_game(const struct retro_game_info *game)
{
  // Configuration de la vidéo au format RGB565
  enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
  environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt);
  
  // Re-initialisation de l'état de la machine
  reset();
  
  // Chargement du programme en mémoire
  load_program(game->data, game->size);
  return true;
}

bool retro_load_game_special(unsigned game_type,
  const struct retro_game_info *info, size_t num_info)
{
  return false;
}

unsigned retro_get_region(void)
{
  return RETRO_REGION_PAL;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
  memset(info, 0, sizeof(*info));
  info->timing.fps = VIDEO_FPS;
  info->timing.sample_rate = AUDIO_SAMPLE_RATE;
  info->geometry.base_width = XBITMAP;
  info->geometry.base_height = YBITMAP;
  info->geometry.max_width = XBITMAP;
  info->geometry.max_height = YBITMAP;
  info->geometry.aspect_ratio = (float) XBITMAP / (float) YBITMAP;
}

void retro_set_video_refresh(retro_video_refresh_t video_refresh)
{
  video_cb = video_refresh;
}

void retro_set_audio_sample(retro_audio_sample_t audio_sample)
{
  // Inutilisé, on utilisera à la place retro_set_audio_sample_batch
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t audio_sample_batch)
{
  audio_batch_cb = audio_sample_batch;
}

void retro_set_input_poll(retro_input_poll_t input_poll)
{
  input_poll_cb = input_poll;
}

void retro_set_input_state(retro_input_state_t input_state)
{
  input_state_cb = input_state;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
}

size_t retro_serialize_size(void)
{
  return 0;
}

bool retro_serialize(void *data, size_t size)
{
  return false;
}

bool retro_unserialize(const void *data, size_t size)
{
  return false;
}

void retro_cheat_reset(void)
{
}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
}

void retro_run(void)
{
  int i;
  // Exécution du nombre d'instructions correspondant à la durée d'une frame vidéo
  for (i = 0; i < NB_INSTR_PER_FRAME; i++)
  {
    run_next_opcode();
  }

  // Mise à jour de l'image de l'écran
  video_cb(video_buffer, XBITMAP, YBITMAP, PITCH);
}

void retro_unload_game(void)
{
}

void retro_reset(void)
{
  reset();
}

void retro_deinit(void)
{
}

void *retro_get_memory_data(unsigned id)
{
  return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
  return 0;
}

