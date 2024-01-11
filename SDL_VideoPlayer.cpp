#include "opencv2/core/core_c.h"
#include "opencv2/videoio/legacy/constants_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/opencv.hpp>
#include "SDL2-2.0.4/include/SDL.h"

using namespace cv;

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    // Initialize SDL for video processing.
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;

    // Initialize video capture using OpenCV.
    VideoCapture vc;
    vc.open("path/to/your/video.mp4");

    // Check if the video file was opened successfully.
    if (!vc.isOpened())
    {
        std::cout << "Error loading video" << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Get the width and height of the video.
    int video_width = static_cast<int>(vc.get(CAP_PROP_FRAME_WIDTH));
    int video_height = static_cast<int>(vc.get(CAP_PROP_FRAME_HEIGHT));

    // Create an SDL window.
    window = SDL_CreateWindow("SDL_RenderClear",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              video_width, video_height, 0);

    // Create a renderer for the SDL window.
    renderer = SDL_CreateRenderer(window, -1, 0);

    // Retrieve the frames per second of the video.
    int fps_of_video = static_cast<int>(vc.get(CAP_PROP_FPS));
    int time_to_wait = 1000 / fps_of_video;

    // Create a Mat object to store the current video frame.
    Mat image;

    SDL_Event event;
    bool running = true;
    while (running)
    {
        double time_start = static_cast<double>(getTickCount());

        // Handle SDL events.
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
            }
        }

        // Read the current frame into the 'image' Mat object.
        if (!vc.read(image))
            running = false;

        // Convert cv::Mat to IplImage format for SDL compatibility.
        IplImage opencvimg2 = cvIplImage(image);
        IplImage* opencvimg = &opencvimg2;
        
        // Convert IplImage to an SDL_Surface.
        SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void*)opencvimg->imageData, 
                                                        opencvimg->width, 
                                                        opencvimg->height, 
                                                        opencvimg->depth*opencvimg->nChannels, 
                                                        opencvimg->widthStep, 
                                                        0xff0000, 0x00ff00, 0x0000ff, 0);

        // Create an SDL texture from the surface.
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        // Clear the renderer to the background color.
        SDL_RenderClear(renderer);

        // Copy the texture to the renderer.
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        // Update the screen with the rendering.
        SDL_RenderPresent(renderer);

        // Clean up the texture.
        SDL_DestroyTexture(texture);

        // Delay to maintain the original FPS of the video.
        while (time_to_wait > ((static_cast<double>(getTickCount()) - time_start) / getTickFrequency() * 1000))
        {
            SDL_Delay(1);
        }
    }

    // Clean up SDL components and quit.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}




