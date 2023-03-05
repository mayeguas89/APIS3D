#include "imgui_app.h"

#include "system.h"

ImguiApp::ImguiApp()
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
}

ImguiApp::~ImguiApp()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImguiApp::Init(GLFWwindow* window)
{
  const char* glsl_version = "#version 130";

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  ambient_light_data_.color.x = System::GetAmbient().r;
  ambient_light_data_.color.y = System::GetAmbient().g;
  ambient_light_data_.color.z = System::GetAmbient().b;
  ambient_light_data_.color.w = 1.f;
}

void ImguiApp::Update()
{
  SceneMenu();
  CameraMenu();
  ObjectsMenu();
  LightsMenu();
}

void ImguiApp::AddObject(Object* object)
{
  objects_.push_back(std::move(object));
  ObjectData data;
  auto object_position = object->GetPosition();
  data.translation[0] = object_position.x;
  data.translation[1] = object_position.y;
  data.translation[2] = object_position.z;
  auto object_rotation = object->GetRotation();
  data.rotation[0] = object_rotation.x;
  data.rotation[1] = object_rotation.y;
  data.rotation[2] = object_rotation.z;
  auto object_scale = object->GetScaling();
  data.scale[0] = object_scale.x;
  data.scale[1] = object_scale.y;
  data.scale[2] = object_scale.z;
  data.enabled = object->IsEnabled();
  objects_data_.push_back(data);
}

void ImguiApp::AddLight(Light* light)
{
  lights_.push_back(std::move(light));
  LightData data;
  auto light_position = light->GetPosition();
  data.position[0] = light_position.x;
  data.position[1] = light_position.y;
  data.position[2] = light_position.z;
  // auto light_direction = light->GetDirection();
  // data.direction[0] = light_direction.x;
  // data.direction[1] = light_direction.y;
  // data.direction[2] = light_direction.z;
  auto light_color = light->GetColor();
  data.color = ImVec4(light_color.x, light_color.y, light_color.z, 1.0f);
  data.cut_off_angle = light->GetCutOff();
  data.linear_attenuation = light->GetLinearAttenuation();
  data.ambient_contribution = light->GetAmbientContribution();
  data.difuse_contribution = light->GetDifuseContribution();
  data.specular_contribution = light->GetSpecularContribution();
  data.enabled = light->IsEnabled();
  light_data_.push_back(data);
}

void ImguiApp::StartFrame()
{
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImguiApp::EndFrame()
{
  // Render dear imgui into screen
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiApp::CameraMenu()
{
  static float camera_speed = System::GetCamera()->GetSpeed();
  static float near_plane = System::GetNearPlane();
  static float far_plane = System::GetFarPlane();
  static float look_at[] = {System::GetCamera()->GetLookAt().x,
                            System::GetCamera()->GetLookAt().y,
                            System::GetCamera()->GetLookAt().z};
  static float translation[] = {System::GetCamera()->GetPosition().x,
                                System::GetCamera()->GetPosition().y,
                                System::GetCamera()->GetPosition().z};
  if (ImGui::CollapsingHeader("Camera"))
  {
    System::GetCamera()->SetSpeed(camera_speed);
    System::SetNearPlane(near_plane);
    System::SetFarPlane(far_plane);
    System::GetCamera()->SetLookAt(glm::vec3(look_at[0], look_at[1], look_at[2]));
    System::GetCamera()->SetPosition(glm::vec4(translation[0], translation[1], translation[2], 1.f));

    ImGui::SliderFloat("CameraSpeed", &camera_speed, 0.0, 5.0);
    ImGui::SliderFloat("NearPlane", &near_plane, 0.001f, 10.0f);
    ImGui::SliderFloat("FarPlane", &far_plane, 100.0f, 500.f);
    ImGui::SliderFloat3("Look At", look_at, -10, 10);
    ImGui::SliderFloat3("Camera Position", translation, -20, 20);
  }
}

void ImguiApp::ObjectsMenu()
{
  if (ImGui::CollapsingHeader("Objects"))
  {
    for (size_t i = 0; i < objects_.size(); i++)
    {
      auto object_txt = "Object" + std::to_string(i);
      if (ImGui::TreeNode(object_txt.c_str()))
      {
        auto object = objects_.at(i);
        auto& data = objects_data_.at(i);
        glm::vec4 position_vec(data.translation[0], data.translation[1], data.translation[2], 1.f);
        object->SetPosition(position_vec);
        glm::vec4 rotation_vec(data.rotation[0], data.rotation[1], data.rotation[2], 1.f);
        object->SetRotation(rotation_vec);
        glm::vec4 scaling_vec(data.scale[0], data.scale[1], data.scale[2], 1.f);
        object->SetScaling(scaling_vec);
        object->SetEnabled(data.enabled);

        ImGui::Checkbox("Toggle ON/OFF", &data.enabled);
        ImGui::SliderFloat3("Position", &data.translation[0], -100.0, 100.0);
        ImGui::SliderFloat3("Rotation", &data.rotation[0], -2 * glm::pi<float>(), 2 * glm::pi<float>());
        ImGui::SliderFloat3("Scale", &data.scale[0], -10.0, 10.0);
        ImGui::TreePop();
      }
    }
  }
}

void ImguiApp::LightsMenu()
{
  static float value = 0.5f;
  if (ImGui::BeginPopupContextItem("LightMenu"))
  {
    if (ImGui::Selectable("Set to zero"))
      value = 0.0f;
    if (ImGui::Selectable("Set to PI"))
      value = 3.1415f;
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
    ImGui::EndPopup();
  }

  if (ImGui::CollapsingHeader("Lights"))
  {
    ImGui::OpenPopupOnItemClick("LightMenu", ImGuiPopupFlags_MouseButtonLeft);
    if (ImGui::TreeNode("Ambient"))
    {
      System::SetAmbient(
        glm::vec3(ambient_light_data_.color.x, ambient_light_data_.color.y, ambient_light_data_.color.z));
      ImGui::ColorEdit3("Color", (float*)&ambient_light_data_.color);
      ImGui::SliderFloat("Intensity", &ambient_light_data_.intensity, 0.f, 1.f);
      ImGui::TreePop();
    }

    for (size_t i = 0; i < lights_.size(); i++)
    {
      auto light_txt_ = "Light" + std::to_string(i);
      if (ImGui::TreeNode(light_txt_.c_str()))
      {
        auto light = lights_.at(i);
        auto& data = light_data_.at(i);

        glm::vec4 position_vec(data.position[0], data.position[1], data.position[2], 1.f);
        light->SetPosition(position_vec);
        glm::vec4 rotation_vec(data.rotation[0], data.rotation[1], data.rotation[2], 1.f);
        light->SetRotation(rotation_vec);
        glm::vec4 color(data.color.x, data.color.y, data.color.z, 1.f);
        light->SetColor(color);

        light->SetCutOff(data.cut_off_angle);
        light->SetLinearAttenuation(data.linear_attenuation);
        light->SetAmbientContribution(data.ambient_contribution);
        light->SetDifuseContribution(data.difuse_contribution);
        light->SetSpecularContribution(data.specular_contribution);
        light->SetEnabled(data.enabled);

        std::string light_type = "Light type -> ";
        if (light->GetType() == (int)Light::Type::kDirectional)
          light_type += "Directional";
        else if (light->GetType() == (int)Light::Type::kPoint)
          light_type += "Point";
        else if (light->GetType() == (int)Light::Type::kFocal)
          light_type += "Focal";

        ImGui::TextColored(data.color, light_type.c_str());
        ImGui::Checkbox("Toggle ON/OFF", &data.enabled);

        if (light->GetType() != (int)Light::Type::kDirectional)
          ImGui::SliderFloat3("Position", &data.position[0], -100.f, 100.f);
        ImGui::SliderFloat3("Rotation", &data.rotation[0], -2 * glm::pi<float>(), 2 * glm::pi<float>());
        ImGui::ColorEdit3("Color", (float*)&data.color);
        if (light->GetType() == (int)Light::Type::kFocal)
          ImGui::SliderFloat("CutOffAngle", &data.cut_off_angle, 0.f, 360.f);
        ImGui::SliderFloat("LinearAttenuation", &data.linear_attenuation, 0.f, 1.f);
        ImGui::SliderFloat("AmbienContribution", &data.ambient_contribution, 0.f, 1.f);
        ImGui::SliderFloat("DifuseContribution", &data.difuse_contribution, 0.f, 1.f);
        ImGui::SliderFloat("SpecularContribution", &data.specular_contribution, 0.f, 1.f);
        ImGui::TreePop();
      }
    }
  }
}

void ImguiApp::SceneMenu()
{
  static ImVec4 clear_color = ImVec4(System::GetClearColor().r,
                                     System::GetClearColor().g,
                                     System::GetClearColor().b,
                                     System::GetClearColor().a);
  System::SetClearColor(glm::vec4{clear_color.x, clear_color.y, clear_color.z, clear_color.w});

  ImGui::ColorEdit3("Clear color", (float*)&clear_color);
}

void ImguiApp::AddMenu() {}
