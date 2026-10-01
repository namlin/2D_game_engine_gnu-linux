#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <map>
#include <string>

class AssetManager {
 private:
  std::map<std::string, SDL_Texture*> textures;
  std::map<std::string, TTF_Font*> fonts;

 public:
  AssetManager(void);
  ~AssetManager(void);

  void clear_assets(void);
  void add_texture(SDL_Renderer* renderer, const std::string& asset_id, const char* file_path);

  SDL_Texture* get_texture(const std::string& id) const;

  void add_font(const std::string& id, const std::string file_path, size_t font_size);

  TTF_Font* get_font(const std::string& id) const;

  void clear_fonts(void);
};

#endif  // ASSETMANAGER_H
