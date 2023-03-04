#include "gl4_render.h"

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

void GL4Render::SetupParticle(Emitter* emitter)
{
  for (auto* mesh: emitter->GetModelParticle()->GetMeshes())
  {
    if (buffer_object_list_.find(mesh->GetMeshId()) != buffer_object_list_.end())
      continue;
    VBO vbo;
    glGenVertexArrays(1, &vbo.bo_id);
    glGenBuffers(1, &vbo.vbo);
    glGenBuffers(1, &vbo.idxbo);
    glGenBuffers(1, &vbo.vbm);
    glGenBuffers(1, &vbo.vba);

    glBindBuffer(GL_ARRAY_BUFFER, vbo.vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 mesh->GetVertList()->size(),
                 emitter->GetParticleVertexPosList().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.vbt);
    glBufferData(GL_ARRAY_BUFFER,
                 mesh->GetVertList()->size(),
                 emitter->GetParticleVertexTextList().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.idxbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned int) * mesh->GetVertIndexesList()->size(),
                 mesh->GetVertIndexesList()->data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo.vbm);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.vba);

    buffer_object_list_[mesh->GetMeshId()] = vbo;
  }
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

void GL4Render::DrawParticles(Emitter* emitter)
{
  for (auto mesh: emitter->GetModelParticle()->GetMeshes())
  {
    auto it = buffer_object_list_.find(mesh->GetMeshId());
    if (it == buffer_object_list_.end())
    {
      throw std::runtime_error("Mesh Id not binded in any buffer object, call Setup object on this object first");
    }
    // System::SetModelMatrix(emitter->GetModelMatrices().data());
    auto buffer = it->second;
    glBindVertexArray(buffer.bo_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.idxbo);
    // Tiene que ir aqui con las localizaciones hardcodeadas porque primero hay que hacer el binbuffer de los vertices y luego el bind buffer de la matriz
    // al meter todos los datos en prepare se sobreescribe el buffer donde se escriben los datos de posicion, color y textura
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0x00);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(glm::vec4)));

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbm);
    glBufferData(GL_ARRAY_BUFFER,
                 emitter->GetModelMatrices().size() * sizeof(glm::mat4),
                 emitter->GetModelMatrices().data(),
                 GL_DYNAMIC_DRAW);

    mesh->GetMaterial()->Prepare();

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vba);
    glBufferData(GL_ARRAY_BUFFER,
                 emitter->GetParticleAlphaList().size() * sizeof(glm::mat4),
                 emitter->GetParticleAlphaList().data(),
                 GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawElementsInstanced(GL_TRIANGLES,
                            static_cast<GLsizei>(mesh->GetVertIndexesList()->size()),
                            GL_UNSIGNED_INT,
                            0,
                            static_cast<GLsizei>(emitter->GetModelMatrices().size()));
    // glBindVertexArray(0);
  }
}

void GL4Render::DrawObjects(const std::vector<Object*>* objects)
{
  static float look_at[] = {System::GetCamera()->GetLookAt().x,
                            System::GetCamera()->GetLookAt().y,
                            System::GetCamera()->GetLookAt().z};
  static float translation[] = {System::GetCamera()->GetPosition().x,
                                System::GetCamera()->GetPosition().y,
                                System::GetCamera()->GetPosition().z};
  static float camera_speed = 0.1f;
  static float near_plane = 0.1f;
  static float far_plane = 100.f;
  static float linear_attenuation = System::GetLights()[0]->GetLinearAttenuation();
  static ImVec4 clear_color = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
  static ImVec4 light_color = ImVec4(System::GetLights()[0]->GetColor().r,
                                     System::GetLights()[0]->GetColor().g,
                                     System::GetLights()[0]->GetColor().b,
                                     1.f);
  static float light_position[] = {
    System::GetLights()[0]->GetPosition().x,
    System::GetLights()[0]->GetPosition().y,
    System::GetLights()[0]->GetPosition().z,
  };
  static float light_direction[] = {
    System::GetLights()[0]->GetDirection().x,
    System::GetLights()[0]->GetDirection().y,
    System::GetLights()[0]->GetDirection().z,
  };

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  // IMGUI
  clear_color_ = glm::vec4(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  System::GetCamera()->SetSpeed(camera_speed);
  System::GetCamera()->SetLookAt(glm::vec3(look_at[0], look_at[1], look_at[2]));
  // System::GetCamera()->SetPosition(glm::vec4(translation[0], translation[1], translation[2], 1.f));
  System::SetFarPlane(far_plane);
  System::SetNearPlane(near_plane);
  System::GetLights()[0]->SetColor(glm::vec3{light_color.x, light_color.y, light_color.z});
  System::GetLights()[0]->SetPosition(glm::vec4{light_position[0], light_position[1], light_position[2], 1.f});
  System::GetLights()[0]->SetRotation(glm::vec4{light_direction[0], light_direction[1], light_direction[2], 1.f});
  System::GetLights()[0]->SetLinearAttenuation(linear_attenuation);

  // OPENGL
  for (size_t i = 0; i < objects->size(); i++)
    DrawObject(objects->at(i));

  // render your GUI
  ImGui::Begin("Triangle Position/Color");
  ImGui::SliderFloat("Camera Speed", &camera_speed, 0.0, 5.0);
  ImGui::SliderFloat("Near Plane", &near_plane, 0.001f, 10.0f);
  ImGui::SliderFloat("Far Plane", &far_plane, 100.0f, 200.f);
  ImGui::SliderFloat3("Look At", look_at, -10, 10);
  ImGui::SliderFloat3("Camera Position", translation, -20, 20);
  ImGui::ColorEdit3("Clear color", (float*)&clear_color);
  ImGui::Separator();
  ImGui::SliderFloat3("Light Position", light_position, -30, 30);
  ImGui::SliderFloat3("Light Rotation", light_direction, -glm::two_pi<float>(), glm::two_pi<float>());
  ImGui::SliderFloat("Linear Attenuation", &linear_attenuation, 0.0f, 1.0f);
  ImGui::ColorEdit3("Light Color", (float*)&light_color);
  ImGui::End();

  // Render dear imgui into screen
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GL4Render::DrawObject(Object* object)
{
  // OPENGL
  System::SetModelMatrix(&(object->GetModelMatrix()));
  for (auto mesh: object->GetMeshes())
  {
    auto it = buffer_object_list_.find(mesh->GetMeshId());
    if (it == buffer_object_list_.end())
      throw std::runtime_error("Mesh Id not binded in any buffer object, call Setup object on this object first");
    auto buffer = it->second;
    glBindVertexArray(buffer.bo_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.idxbo);

    mesh->GetMaterial()->Prepare();

    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(mesh->GetVertIndexesList()->size()),
                   GL_UNSIGNED_INT,
                   nullptr);
  }
}
