#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iostream>

const char *title = "web test";
int width = 700, height = 500;
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Texture *screen;
SDL_Rect screensize;
int frameCount, timerFPS, lastFrame, fps, lastTime;
int setFPS = 60;
SDL_Point mouse;
const Uint8 *keystates;
Uint32 mousestate;
SDL_Event event;
bool running;
SDL_Color bkg = {0,255,255,255};

int logo;

std::vector<SDL_Texture*> images;
int loadImage(std::string filename) {
 images.push_back(IMG_LoadTexture(renderer, filename.c_str()));
 return images.size()-1;
}
void drawImage(int img, SDL_Rect dest, SDL_Rect src) {
 SDL_RenderCopy(renderer, images[img], &src, &dest);
}

int font_size = 32;
TTF_Font *font;
SDL_Color font_color = {255,255,255,255};
void writeText(std::string t, int x, int y) {
 const char *text = t.c_str();
 SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, font_color);
 SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
 SDL_Rect wrect = {x, y, text_surface->w, text_surface->h};
 SDL_FreeSurface(text_surface);
 SDL_RenderCopy(renderer, text_texture, NULL, &wrect);
 SDL_DestroyTexture(text_texture);
}

void updateKeys() {
  keystates = SDL_GetKeyboardState(NULL);
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT)
      running = false;
  }
  mousestate = SDL_GetMouseState(&mouse.x, &mouse.y);
}

void setDrawColor(SDL_Color c) { SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a); }

void begin_render() {
  SDL_SetRenderTarget(renderer, screen);
  setDrawColor(bkg);
  SDL_RenderClear(renderer);
  frameCount++;
  timerFPS = SDL_GetTicks()-lastFrame;
  if(timerFPS<(1000/setFPS)) {
   SDL_Delay((1000/setFPS)-timerFPS);
  }
}

void end_render() {
  SDL_SetRenderTarget(renderer, NULL);
  SDL_RenderCopy(renderer, screen, &screensize, &screensize);
  SDL_RenderPresent(renderer);
}

void init() {
 SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
 SDL_Init(SDL_INIT_EVERYTHING);
 window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
 renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
 SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
 running = 1;
 screensize.x=screensize.y=0;
 screensize.w=width;screensize.h=height;
 screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

 logo = loadImage("res/logo-amg.png");
 TTF_Init();
 font = TTF_OpenFont("res/Peepo.ttf", font_size);
}
void quit() {
 running = 0;
 SDL_DestroyRenderer(renderer);
 SDL_DestroyWindow(window);
 SDL_Quit();
}

void loop() {
  end_render();
  lastFrame = SDL_GetTicks();
  if (lastFrame >= (lastTime + 1000)) {
    lastTime = lastFrame;
    fps = frameCount;
    frameCount = 0;
  }
  updateKeys();
  begin_render();
}

int main() {
 init();
 while(running) {
  loop();
  if(keystates[SDL_SCANCODE_ESCAPE]) running=false;
  drawImage(logo, {mouse.x, mouse.y, 38*4, 28*4}, {0,0,38,28});
  writeText(std::to_string(fps), 10, 0);
 }
 quit();
 return 0;
}
