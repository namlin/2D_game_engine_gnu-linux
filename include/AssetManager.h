#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <map>
#include <string>

class AssetManager {
 private:
  std::map<std::string, SDL_Texture*> textures;

 public:
  AssetManager(void);
  ~AssetManager(void);

  void clear_assets(void);
  void add_texture(SDL_Renderer* renderer, const std::string& asset_id, const char* file_path);

  SDL_Texture* get_texture(const std::string& id) const;
};

#endif  // ASSETMANAGER_H
