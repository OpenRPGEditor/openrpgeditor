#ifndef IMAGE_LOADER_H_
#define IMAGE_LOADER_H_

#ifdef __cplusplus
extern "C" {
#endif
bool LoadTextureFromFile(const char* filename, struct SDL_Texture** texture_ptr, int* width, int* height, struct SDL_Renderer* renderer);

bool LoadTextureFromMemory(const void* data, int length, struct SDL_Texture** texture_ptr, int* width, int* height, struct SDL_Renderer* renderer);

#ifdef __cplusplus
}
#endif
#endif // IMAGE_LOADER
