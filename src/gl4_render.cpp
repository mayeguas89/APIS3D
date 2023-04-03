#include "gl4_render.h"

#include "gl_texture_framebuffer.h"
#include "system.h"
#include "vertex.h"

GL4Render::GL4Render(int width, int height): GL1Render{width, height} {}

GL4Render::~GL4Render()
{
  delete depth_texture_;
}

void GL4Render::Init()
{
  GL1Render::Init();
  depth_texture_ = new GLTextureFrameBuffer(GLTextureFrameBuffer::FBType::kDepthFb, width_, height_);
#ifdef IMGUI_ENABLE
  imgui_app_.Init(window_);
#endif
}

void GL4Render::SetupLight(Light* light)
{
#ifdef IMGUI_ENABLE
  imgui_app_.AddLight(std::move(light));
#endif
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
                 sizeof(Vertex) * mesh->GetVertList()->size(),
                 mesh->GetVertList()->data(),
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
#ifdef IMGUI_ENABLE
  imgui_app_.AddObject(std::move(object));
#endif
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
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.idxbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned int) * mesh->GetVertIndexesList()->size(),
                 mesh->GetVertIndexesList()->data(),
                 GL_STATIC_DRAW);
    buffer_object_list_[mesh->GetMeshId()] = vbo;
  }
}

void GL4Render::RemoveObject(Object* object) {}

void GL4Render::DrawLines(const std::vector<Line*>& lines)
{
  for (auto line: lines)
  {
    System::SetModelMatrix(&(line->GetModelMatrix()));
    for (auto mesh: line->GetMeshes())
    {
      auto it = buffer_object_list_.find(mesh->GetMeshId());
      if (it == buffer_object_list_.end())
      {
        throw std::runtime_error(
          "Mesh Id not binded in any buffer object, call Setup object on this object first");
      }

      auto buffer = it->second;
      glBindVertexArray(buffer.bo_id);
      glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.idxbo);
      mesh->GetMaterial()->Prepare();
      glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mesh->GetVertIndexesList()->size()));
    }
  }
}

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
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4 * sizeof(glm::vec4)));

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbm);
    glBufferData(GL_ARRAY_BUFFER,
                 emitter->GetModelMatrices().size() * sizeof(glm::mat4),
                 emitter->GetModelMatrices().data(),
                 GL_DYNAMIC_DRAW);

    mesh->GetMaterial()->Prepare();

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vba);
    glBufferData(GL_ARRAY_BUFFER,
                 emitter->GetParticleAlphaList().size() * sizeof(float),
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
#ifdef IMGUI_ENABLE
  imgui_app_.StartFrame();
  imgui_app_.Update();
#endif

  System::SetRenderType(System::RenderType::kColor);
  SetFrameBuffer(0);

  // Calculate shadowMap
  if (System::GetShadowsEnabled())
  {
    SetFrameBuffer(reinterpret_cast<GLTextureFrameBuffer*>(depth_texture_)->GetFrameBufferId());

    System::SetRenderType(System::RenderType::kShadow);
    for (size_t i = 0; i < objects->size(); i++)
      DrawObject(objects->at(i));
    System::SetRenderType(System::RenderType::kColor);

    SetFrameBuffer(0);
  }

  // Draw Mirrors
  if (auto mirrors = System::GetMirrors(); !mirrors.empty())
  {
    auto old_camera = System::GetCamera();
    for (auto mirror: mirrors)
    {
      auto mesh = mirror->GetMeshes().at(0);
      auto real_mirror = reinterpret_cast<Mirror*>(mirror);
      auto fb_texture = reinterpret_cast<GLTextureFrameBuffer*>(mesh->GetMaterial()->GetBaseTexture());
      SetFrameBuffer(fb_texture->GetFrameBufferId());
      System::SetCamera(real_mirror->GetCamera());
      for (size_t i = 0; i < objects->size(); i++)
        DrawObject(objects->at(i));
    }
    System::SetCamera(old_camera);
    SetFrameBuffer(0);
  }

  // Draw objects from camera

  for (size_t i = 0; i < objects->size(); i++)
  {
    if (System::GetShadowsEnabled())
      depth_texture_->Bind(3);
    DrawObject(objects->at(i));
  }

#ifdef IMGUI_ENABLE
  imgui_app_.EndFrame();
#endif
}

void GL4Render::DrawObject(Object* object)
{
  System::SetModelMatrix(&(object->GetModelMatrix()));
  System::SetAnimationEnabled(object->Animate());

  if (object->IsEnabled())
  {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  else
  {
    glBlendFunc(GL_ONE, GL_ZERO);
  }

  for (auto mesh: object->GetMeshes())
  {
    auto it = buffer_object_list_.find(mesh->GetMeshId());
    if (it == buffer_object_list_.end())
      throw std::runtime_error("Mesh Id not binded in any buffer object, call Setup object on this object first");
    auto buffer = it->second;
    glBindVertexArray(buffer.bo_id);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0x00, sizeof(Vertex), mesh->GetVertList()->data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.idxbo);

    mesh->GetMaterial()->Prepare();

    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(mesh->GetVertIndexesList()->size()),
                   GL_UNSIGNED_INT,
                   nullptr);
  }
}

void GL4Render::SetupAmbient()
{
#ifdef IMGUI_ENABLE
  imgui_app_.SetupAmbient();
#endif
}

void GL4Render::SetFrameBuffer(GLuint framebuffer)
{
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glViewport(0, 0, width_, height_);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  unsigned int draw_buf = GL_COLOR_ATTACHMENT0;
  glDrawBuffers(1, &draw_buf);
}

void GL4Render::SetSize(int width, int height)
{
  GL1Render::SetSize(width, height);
  depth_texture_ = new GLTextureFrameBuffer(GLTextureFrameBuffer::FBType::kDepthFb, width_, height_);
  if (auto mirrors = System::GetMirrors(); !mirrors.empty())
  {
    for (auto mirror: mirrors)
    {
      auto mesh = mirror->GetMeshes().at(0);
      auto material = mesh->GetMaterial();
      material->SetBaseTexture(new GLTextureFrameBuffer(GLTextureFrameBuffer::FBType::kColorFb, width_, height_));
    }
  }
}