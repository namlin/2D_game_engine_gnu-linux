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
  this->clear_fonts();
}

void AssetManager::clear_fonts(void) {
  for (auto font : this->fonts) {
    TTF_CloseFont(font.second);
  }

  this->fonts.clear();
}

void AssetManager::add_texture(SDL_Renderer* renderer,
                  const std::string& texture_id, const char* file_path) {
  SDL_Surface* surface = IMG_Load(file_path);

  if (surface == nullptr) {
    std::cerr << "surface == nullptr.\n";
    std::exit(EXIT_FAILURE);
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  if (texture == nullptr) {
    std::cerr << "texture == nullptr.\n";
    std::exit(EXIT_FAILURE);
  }

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

// Fonts:

void AssetManager::add_font(const std::string& id, const std::string file_path,
                            size_t font_size) {
  TTF_Font* font = TTF_OpenFont(file_path.c_str(), font_size);

  if (font == NULL) {
    std::string error = TTF_GetError();
    std::cerr << "[AssetManager] " << error << "\n";
    return;
  }

  this->fonts.emplace(id, font);
}

TTF_Font* AssetManager::get_font(const std::string& id) const {
  return this->fonts.at(id);
}
