#ifndef ASCII_RENDERER_H
#define ASCII_RENDERER_H

#include <iostream>

template <typename Display, typename Frame>
class Renderer
{
public:
    Display* display_;
    virtual ~Renderer() = default;
    virtual void render(const Frame& frame) {};
    explicit Renderer(Display* display) : display_(display) {};
};

class CommandLineRenderer final : public Renderer<std::ostream, std::string>
{
public:
    void render(const std::string& frame) override { *(this->display_) << frame << "\n"; };
    ~CommandLineRenderer() override = default;

    explicit
    CommandLineRenderer(std::ostream* output_stream) : Renderer<std::ostream, std::string>(output_stream) {};
};

#endif //ASCII_RENDERER_H
