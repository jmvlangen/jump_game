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

    printRenderDrivers();

    //createWindow();
    //createRenderer();
    
    startUpdateAndRenderThread();
    

    
    processEvents();

    checkThreadErrors();

    close();
  }

  void close() {
    stopThreads();
    if(sdlHasBeenInitialized) {
      SDL_Quit();
      sdlHasBeenInitialized = false;
    }
  }
  
  private:
  static constexpr int WIDTH = 640, HEIGHT = 480;

  double x{0.0}; //Boskabouter: remove this later.

  SDL_Window * window;
  SDL_Renderer * renderer;
  std::atomic<bool> 
    stopBoolean{false}, //Used to stop the program and all non-main threads.
    threadError{false}; //If this becomes true, it means at least one of the
      //non-main threads has encountered an error.
  bool sdlHasBeenInitialized{false};

  std::thread updateAndRenderThread;

  void initializeSdl() {
    int status = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);

    if(status != 0 ) {
      std::printf("SDL could not be initialized: %s\n",SDL_GetError());
      throw std::runtime_error("");
    }
    sdlHasBeenInitialized = true;
    std::printf("SDL has been initialized\n");
  }

  void printRenderDrivers() {
    SDL_RendererInfo info;

    std::printf("Available render drivers:\n");

    for( int i = 0;  i < SDL_GetNumRenderDrivers(); ++i ) {
      std::printf("  %2d: ",i);
      SDL_GetRenderDriverInfo(i,&info);
      printRenderDriver(info);
    }
  }

  void printRenderDriver(const SDL_RendererInfo& info) {
    std::printf("%s\n",info.name);
  }

  void startUpdateAndRenderThread() {
    updateAndRenderThread = std::thread( [this]() { updateAndRender(); } );
  }

  void updateAndRender() {
    try {
      createWindow();
      createRenderer();
    
      while(!stopBoolean) {
        update();
        render();
      }
    }
    catch(std::exception& e) {
      std::printf("Exception in updateRender thread: %s\n", e.what() );
      stopBoolean = true;
      
    }
  }
  
  void createWindow() {
    window = SDL_CreateWindow("Window title"
      ,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
      WIDTH,HEIGHT,
      SDL_WINDOW_FULLSCREEN_DESKTOP|
      SDL_WINDOW_BORDERLESS);
    
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

  
  void update() {
    x += 0.6666;
    if(x > 40) {
      x = 0;
    }
  }

  void render() {
    clearScreen();
    drawSquare();
    SDL_RenderPresent(renderer);
  }

  void clearScreen() {
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
  }

  void drawSquare() {
    int xRender = (int) (x*16);
    
    //SDL_Rect rectangle{WIDTH/2,HEIGHT/2,16,16};
    SDL_Rect rectangle{xRender,HEIGHT/2,16,16};
    
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer,&rectangle);
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

  void checkThreadErrors() {
    if(threadError) {
      std::printf("One of the non-main threads has encountered an error!\n");
      throw std::runtime_error("Error in at least one non-main thread.");
    }
  }

  void processEvent(const SDL_Event& event) {
    switch(event.type) {
      case SDL_QUIT: handleQuitEvent(); break;
    }
  }

  void handleQuitEvent() {
    std::printf("Received the quit signal\n");
    stopBoolean = true;
  }

  void stopThreads() {
    stopBoolean = true;
    waitForThread(updateAndRenderThread);
  }

  void waitForThread(std::thread& t) {
    if( t.joinable() ) {
      t.join();
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

