#include <cstdio>
#include <SDL.h>

#include <atomic>
#include <chrono>
#include <thread>

#include <stdexcept>

namespace JumpGame {

class Main {
  public:
  void run(int numberOfArguments, char ** arguments) {

    std::printf("Hello, World!\n");
    readCommandLineArguments(numberOfArguments, arguments);
    
    initializeSdl();

    printRenderDrivers();

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
  static constexpr int N_BLOCKS = 40; 
  /*
  Number of blocks that fit in the width of one screen. I have set it to
  40 as you initially wanted blocks of 16x16 pixels in a screen of width
  640.
  */

  double x{0.0}; //Boskabouter: remove this later.

  int displayNumber{0}; //Indicates the monitor you want to use.

  int blockSize, renderWidth,renderHeight;

  SDL_Window * window;
  SDL_Renderer * renderer;
  std::atomic<bool> 
    stopBoolean{false}, //Used to stop the program and all non-main threads.
    threadError{false}; //If this becomes true, it means at least one of the
      //non-main threads has encountered an error.
  bool sdlHasBeenInitialized{false};

  std::thread updateAndRenderThread;

  void readCommandLineArguments(int numberOfArguments, char** arguments) {
    if(numberOfArguments > 1) {
      readDisplayNumber(arguments[1]);
    }
    else {
      std::printf(
        "Display number is 0 (primary display) by default, but it can be "
        "changed by providing a command line argument.\n");
    }
  }

  void readDisplayNumber(std::string text) {
    try {
      displayNumber = std::stoi(text);
    }
    catch(std::exception& e) {
      std::printf(
        "Could not read the display number from the text \"%s\".\n" 
        "Error message: %s\n."
        "The display number will be set to 0.\n"
        ,text.c_str(), e.what() );

      displayNumber = 0;
    }

    if(displayNumber < 0) {
      throw std::runtime_error("The display number cannot be negative");
    }
  }
  
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

      blockSize = renderWidth/N_BLOCKS;
    
      while(!stopBoolean) {
        update();
        render();
      }
    }
    catch(std::exception& e) {
      std::printf("Exception in updateRender thread: %s\n", e.what() );
      threadError = true;
      stopBoolean = true;
    }
  }
  
  void createWindow() {
    if(displayNumber >= SDL_GetNumVideoDisplays() ) {
      std::printf(
        "The display number %d is larger than the number of monitors - 1\n",
        displayNumber);
      throw std::runtime_error("Display number is too large.");
    }

    SDL_Rect rectangle{0,0,0,0};
    SDL_GetDisplayBounds(displayNumber,&rectangle);
    
    window = SDL_CreateWindow("Window title",
      rectangle.x,rectangle.y,
      0,0,
      SDL_WINDOW_FULLSCREEN_DESKTOP|
      SDL_WINDOW_ALLOW_HIGHDPI|
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
    
    SDL_GetRendererOutputSize(renderer,&renderWidth,&renderHeight);
    std::printf("Window size in pixels: (%d,%d)\n",renderWidth,renderHeight);
  }

  
  void update() {
    x += 0.6666;
    if(x > N_BLOCKS) {
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
    int xRender = (int) (x*blockSize);
    
    SDL_Rect rectangle{xRender,renderHeight/4,blockSize,blockSize};
    
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
    main.run(numberOfArguments,arguments);
  }
  catch(std::exception& e) {
    std::printf( "Error: %s\n",e.what() );
    main.close();
    return 1;
  }

  return 0;
}

