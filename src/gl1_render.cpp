#include "gl1_render.h"

GL1Render::GL1Render(int width, int height): Render{width, height}
{
  if (!glfwInit())
  {
    // Error del so de versionado de librerias
    throw std::runtime_error("ERROR AT INIT GLFWINIT\n");
  }
  window_ = glfwCreateWindow(width_, height_, "MyWindow", nullptr, nullptr);
}

GL1Render::~GL1Render()
{
  glfwTerminate();
}

void GL1Render::Init()
{
  glfwMakeContextCurrent(window_);
  gladLoadGL(glfwGetProcAddress);
}

void GL1Render::SetupObject(Object* object) {}

void GL1Render::RemoveObject(Object* object) {}

void GL1Render::DrawObjects(const std::vector<Object*>* objects)
{
  glm::vec4 color(1.f, 1.f, 1.f, 1.f);
  glBegin(GL_TRIANGLES);
  glColor3f(color.r, color.g, color.b);
  for (Object* object: *(objects))
  {
    for (Vertex vertex: *(object->GetMesh()->GetVertList()))
    {
      vertex.position = object->GetModelMatrix() * vertex.position;
      glColor3f(vertex.color.r, vertex.color.g, vertex.color.b);
      glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
    }
  }
  glEnd();
}

bool GL1Render::IsClosed()
{
  return glfwWindowShouldClose(window_);
}

void GL1Render::SetKeyCallback(void (*callback)(GLFWwindow* window, int key, int scancode, int action, int mods))
{
  glfwSetKeyCallback(window_, callback);
}

void GL1Render::Clear()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void GL1Render::SwapBuffers()
{
  glfwSwapBuffers(window_);
}
