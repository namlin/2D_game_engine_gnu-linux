#include "../include/AssetManager.h"

AssetManager::AssetManager(void) {
  std::cout << "[AssetManager] Constructor Executing.\n";
}

AssetManager::~AssetManager(void) {
  std::cout << "[AssetManager] Destructor Executing.\n";
  this->clear_assets();
}

void AssetManager::clear_assets(void) {
  for (auto& [id, texture] : this->textures) {
    if (texture) {
      SDL_DestroyTexture(texture);
    }
  }

  this->textures.clear();
}

void AssetManager::add_texture(SDL_Renderer* renderer,
                  const std::string& texture_id, const std::string& file_path) {
  SDL_Surface* surface = IMG_Load(file_path.c_str());
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  this->textures.emplace(texture_id, texture);
}

SDL_Texture* AssetManager::get_texture(const std::string& id) const {
  auto it = this->textures.find(id);

  if (it != this->textures.end()) {
    return it->second;
  }

  return nullptr;
}
