#include <iostream>
#include <memory>
using namespace std;
// Productos abstractos
class Window {
public:
    virtual ~Window() {}
    virtual void draw() const = 0;
};
class Button {
public:
    virtual ~Button() {}
    virtual void draw() const = 0;
};
class Shader {
public:
    virtual ~Shader() {}
    virtual void compile() const = 0;
};
// Fabrica abstracta
class RenderFactory {
public:
    virtual ~RenderFactory() {}
    virtual unique_ptr<Window> createWindow() const = 0;
    virtual unique_ptr<Button> createButton() const = 0;
    virtual unique_ptr<Shader> createShader() const = 0;
};
// Productos OpenGL
class OpenGLWindow:public Window {
public:
    void draw() const override {
        cout<<"Dibujando ventana con OpenGL"<<endl;
    }
};
class OpenGLButton:public Button {
public:
    void draw() const override {
        cout<<"Dibujando boton con OpenGL"<<endl;
    }
};
class OpenGLShader:public Shader {
public:
    void compile() const override {
        cout<<"Compilando shader GLSL para OpenGL"<<endl;
    }
};
// Productos Vulkan
class VulkanWindow:public Window {
public:
    void draw() const override {
        cout<<"Dibujando ventana con Vulkan"<<endl;
    }
};
class VulkanButton:public Button {
public:
    void draw() const override {
        cout<<"Dibujando boton con Vulkan"<<endl;
    }
};
class VulkanShader:public Shader {
public:
    void compile() const override {
        cout<<"Compilando shader SPIR-V para Vulkan"<<endl;
    }
};
// Fabrica OpenGL
class OpenGLFactory:public RenderFactory {
public:
    unique_ptr<Window> createWindow() const override {
        return make_unique<OpenGLWindow>();
    }
    unique_ptr<Button> createButton() const override {
        return make_unique<OpenGLButton>();
    }
    unique_ptr<Shader> createShader() const override {
        return make_unique<OpenGLShader>();
    }
};
// Fabrica Vulkan
class VulkanFactory:public RenderFactory {
public:
    unique_ptr<Window> createWindow() const override {
        return make_unique<VulkanWindow>();
    }
    unique_ptr<Button> createButton() const override {
        return make_unique<VulkanButton>();
    }
    unique_ptr<Shader> createShader() const override {
        return make_unique<VulkanShader>();
    }
};
int main() {
    unique_ptr<RenderFactory> factory=make_unique<OpenGLFactory>();
    auto window=factory->createWindow();
    auto button=factory->createButton();
    window->draw();
    button->draw();
    factory=make_unique<VulkanFactory>();
    auto shader=factory->createShader();
    shader->compile();
    auto windowVulkan=factory->createWindow();
    auto buttonVulkan=factory->createButton();
    windowVulkan->draw();
    buttonVulkan->draw();
    return 0;
}