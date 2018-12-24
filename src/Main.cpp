#include <cstdio>
#include <SDL.h>

#include <atomic>
#include <chrono>
#include <thread>

#include <stdexcept>

namespace JumpGame {

class Main {
  public:
  void run() {
    std::printf("Hello, World!\n");
    
    initializeSdl();

    createWindow();
    createRenderer();
    
    drawSquare();
    
    processEvents();

    close();

  }

  void close() {
    if(sdlHasBeenInitialized) {
      SDL_Quit();
      sdlHasBeenInitialized = false;
    }
  }
  
  private:
  static constexpr int WIDTH = 640, HEIGHT = 480;

  SDL_Window * window;
  SDL_Renderer * renderer;
  std::atomic<bool> stopBoolean{false};
  bool sdlHasBeenInitialized{false};

  void initializeSdl() {
    int status = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);

    if(status != 0 ) {
      std::printf("SDL could not be initialized: %s\n",SDL_GetError());
      throw std::runtime_error("");
    }
    sdlHasBeenInitialized = true;
    std::printf("SDL has been initialized\n");
  }
  void createWindow() {
    window = SDL_CreateWindow("Window title"
      ,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
      WIDTH,HEIGHT,
      0);
    
    if(window == NULL) {
      std::printf("Could not create a window: %s",SDL_GetError());
      throw std::runtime_error("Window creation error");
    }
  }

  void createRenderer() {
    renderer = SDL_CreateRenderer(window,-1,
      SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    
    if(renderer == NULL) {
      std::printf("Could not create a renderer: %s",SDL_GetError());
      throw std::runtime_error("Renderer creation error");
    }
  }

  void drawSquare() {
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    
   // SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderPresent(renderer);
  }

  void processEvents() {
    while(!stopBoolean) {
      SDL_Event event;
      while( SDL_PollEvent(&event) ) {
        processEvent(event);
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  void processEvent(const SDL_Event& event) {
    switch(event.type) {
      case SDL_QUIT: stopBoolean = true; break;
    }
  }


};

}

int main(int numberOfArguments, char** arguments) {
  JumpGame::Main main;
  try {
    main.run();
  }
  catch(std::exception& e) {
    std::printf( "Uncaught Exception in main thread: %s\n",e.what() );
    main.close();
    return 1;
  }

  return 0;
}

