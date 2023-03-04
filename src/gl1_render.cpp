#include "gl1_render.h"

#include "system.h"

static void glfw_error_callback(int error, const char* description)
{
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

GL1Render::GL1Render(int width, int height): Render{width, height}
{
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
  {
    // Error del so de versionado de librerias
    throw std::runtime_error("ERROR AT INIT GLFWINIT\n");
  }
  window_ = glfwCreateWindow(width_, height_, "MyWindow", nullptr, nullptr);
}

GL1Render::~GL1Render()
{
  glfwDestroyWindow(window_);
  glfwTerminate();
}

void GL1Render::Init()
{
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);
  gladLoadGL(glfwGetProcAddress);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  glfwSetWindowSizeCallback(window_, &System::WindowResizeCallback);

  // Set Mouse in middle on the screen
  // glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // glfwSetCursorPos(window_, width_ / 2.f, height_ / 2.f);
}

void GL1Render::SetupObject(Object* object) {}
void GL1Render::SetupParticle(Emitter* emitter) {}

void GL1Render::SetupLight(Light* light) {}

void GL1Render::RemoveObject(Object* object) {}

void GL1Render::DrawObjects(const std::vector<Object*>* objects)
{
  glm::vec4 color(1.f, 1.f, 1.f, 1.f);
  glBegin(GL_TRIANGLES);
  glColor3f(color.r, color.g, color.b);
  for (Object* object: *(objects))
  {
    DrawObject(object);
  }
  glEnd();
}

void GL1Render::DrawObject(Object* object)
{
  for (auto* mesh: object->GetMeshes())
  {
    for (Vertex vertex: *(mesh->GetVertList()))
    {
      vertex.position = object->GetModelMatrix() * vertex.position;
      glColor3f(vertex.color.r, vertex.color.g, vertex.color.b);
      glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
    }
  }
}

void GL1Render::DrawParticles(Emitter* emitter) {}

bool GL1Render::IsClosed()
{
  return glfwWindowShouldClose(window_);
}

void GL1Render::SetSize(int width, int height)
{
  Render::SetSize(width, height);
  glViewport(0, 0, width, height);
  glfwSetWindowSize(window_, width, height);
}

void GL1Render::SetKeyCallback(void (*callback)(GLFWwindow* window, int key, int scancode, int action, int mods))
{
  glfwSetKeyCallback(window_, callback);
}

void GL1Render::SetCursorPositionCallback(void (*callback)(GLFWwindow* window, double x_key, double y_pos))
{
  glfwSetCursorPosCallback(window_, callback);
}

void GL1Render::SetMouseButtonCallback(void (*callback)(GLFWwindow* window, int button, int action, int mods))
{
  glfwSetMouseButtonCallback(window_, callback);
}

void GL1Render::SetScrollCallback(void (*callback)(GLFWwindow* window, double x_offset, double y_offset))
{
  glfwSetScrollCallback(window_, callback);
}

void GL1Render::Clear()
{
  glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GL1Render::SwapBuffers()
{
  glfwSwapBuffers(window_);
}
