#include "Render.h"
#include "render/object/Primitive.h"
#include "render/object/Triangle.h"
#include "render/object/Quad.h"
#include "render/shader/Shader.h"
#include "render/texture/Texture.h"
#include "render/vertexArray/VertexArray.h"
#include <SDL_video.h>
#include <iostream>
#include <string>



CoreObject::Primitive *t;
Core::Texture* text;

namespace Core {
Render::Render() {}

bool Render::Init(SDL_Window *window) {
  this->SetAttributes();
  mWindow = window;
  mContext = SDL_GL_CreateContext(mWindow);
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    SDL_Log("Failed to initialize GLEW.");
    return false;
  }
  mShader = new Core::Shader();
  mShader->Load("../shaders/Sprite.vert", "../shaders/Sprite.frag");
  t = {new CoreObject::Quad()};
  text = new Texture();
  text->Load("../assets/Plane.png");
  return true;
}

void Render::SetAttributes(SDL_GLprofile profile,
                           std::pair<short, short> version, short sizeColor,
                           short sizeDepth) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version.first);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, version.second);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, sizeColor);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, sizeColor);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, sizeColor);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, sizeColor);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, sizeDepth);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
}

void Render::Draw() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  // Clear the color buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
  // text->SetActive();
  // mShader->SetActive();

  // glCheckError();
  // mvba->SetActive();
  // glCheckError();
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // t->Draw();

  SDL_GL_SwapWindow(mWindow);
}

void Render::SetVertexArray(Core::VertexArray *vba) { mvba = vba; }
} // namespace Core