#include "sdli.hpp"

#include <string>
#include <SDL.h>

#include "log.hpp"

struct SDLI::InnerData
{
    std::string window_name;

    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};

};

SDLI::SDLI()
{
    data_ = std::make_shared<InnerData>();

    data_->window_name = std::string("ffplay_demo");
}

SDLI::~SDLI()
{
    deinit();
}

int SDLI::init()
{
    auto &window = data_->window;
    auto &window_name = data_->window_name;
    auto &renderer = data_->renderer;

    if (nullptr != window)
    {
        xerror("window is not null");
        return -1;
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);

    window = SDL_CreateWindow(
        window_name.c_str(), 
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        DEFAULT_WIDTH,
        DEFAULT_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );

    if (nullptr == window)
    {
        xerror("create window failed");
        return -1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    if (nullptr != renderer)
    {
        xerror("renderer is not null");
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (nullptr == renderer)
    {
        xerror("create renderer failed");
    }

    return 0;
}

int SDLI::deinit()
{
    auto &window = data_->window;

    if (nullptr != window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    return 0;
}

int SDLI::exec()
{
    while (true)
    {
        SDL_PumpEvents();
    }
    
    return 0;
}