#include "gl4_render.h"

#include "common.h"
#include "system.h"
#include "vertex.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

GL4Render::GL4Render(int width, int height): GL1Render{width, height}
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
}

GL4Render::~GL4Render()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void GL4Render::Init()
{
  GL1Render::Init();

  const char* glsl_version = "#version 130";

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
}

void GL4Render::SetupObject(Object* object)
{
  for (auto* mesh: object->GetMeshes())
  {
    VBO vbo;
    glGenVertexArrays(1, &vbo.bo_id);
    glGenBuffers(1, &vbo.vbo);
    glGenBuffers(1, &vbo.idxbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo.vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertex) * mesh->GetVertList()->size(),
                 mesh->GetVertList()->data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.idxbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned int) * mesh->GetVertIndexesList()->size(),
                 mesh->GetVertIndexesList()->data(),
                 GL_STATIC_DRAW);
    buffer_object_list_[mesh->GetMeshId()] = vbo;
  }
}

void GL4Render::RemoveObject(Object* object) {}

void GL4Render::DrawObjects(const std::vector<Object*>* objects)
{
  static float rotation[] = {0.0, 0.0, 0.0};
  static float translation[] = {0.0, 0.0};
  static float camera_speed = 0.1f;
  static float near_plane = 0.1f;
  static float far_plane = 100.f;
  static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  System::GetCamera()->SetSpeed(camera_speed);
  // IMGUI
  clear_color_ = glm::vec4(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  System::SetFarPlane(far_plane);
  System::SetNearPlane(near_plane);
  for (auto i = 0; i < objects->size(); i++)
  {
    // IMGUI
    glm::vec4 rotation_vec(rotation[0], rotation[1], rotation[2], 0.f);
    objects->at(i)->SetRotation(rotation_vec);
    glm::vec4 position_vec(translation[0], translation[1], 0.f, 0.f);
    objects->at(i)->SetPosition(position_vec);

    // OPENGL
    System::SetModelMatrix(&(objects->at(i)->GetModelMatrix()));
    for (auto* mesh: objects->at(i)->GetMeshes())
    {
      auto buffer = buffer_object_list_[mesh->GetMeshId()];
      glBindVertexArray(buffer.bo_id);
      glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.idxbo);

      mesh->GetMaterial()->Prepare();

      if (auto* texture = mesh->GetMaterial()->GetTexture(); texture)
      {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());
      }

      glDrawElements(GL_TRIANGLES,
                     static_cast<GLsizei>(mesh->GetVertIndexesList()->size()),
                     GL_UNSIGNED_INT,
                     nullptr);
    }
  }

  // render your GUI
  ImGui::Begin("Triangle Position/Color");

  ImGui::SliderFloat("CameraSpeed", &camera_speed, 0.0, 5.0);
  ImGui::SliderFloat("NearPlane", &near_plane, 0.001f, 10.0f);
  ImGui::SliderFloat("FarPlane", &far_plane, 100.0f, 200.f);
  ImGui::SliderFloat3("Rotation", rotation, 0, 2 * glm::pi<float>());
  ImGui::SliderFloat2("Position", translation, -1.0, 1.0);
  ImGui::ColorEdit3("Clear color", (float*)&clear_color);
  ImGui::End();

  // Render dear imgui into screen
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
