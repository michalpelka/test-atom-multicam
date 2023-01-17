/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
 */

#include "CameraTestComponent.h"

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzFramework/Components/TransformComponent.h>

#include <AzFramework/Physics/RigidBodyBus.h>

#include <imgui/imgui.h>

#include <AzCore/Math/MatrixUtils.h>

#include <Atom/RPI.Public/Base.h>
#include <Atom/RPI.Public/Pass/Specific/RenderToTexturePass.h>
#include <Atom/RPI.Public/RPISystemInterface.h>
#include <Atom/RPI.Public/RenderPipeline.h>
#include <Atom/RPI.Public/Scene.h>
#include <Atom/RPI.Public/View.h>

#include <AzFramework/Components/TransformComponent.h>
#include <AzFramework/Scene/SceneSystemInterface.h>

#include <Atom/RPI.Public/FeatureProcessorFactory.h>
#include <Atom/RPI.Public/Pass/PassSystemInterface.h>

#include <Atom/Feature/Utils/FrameCaptureBus.h>
#include <PostProcess/PostProcessFeatureProcessor.h>
#include <Atom/RPI.Public/Pass/PassFactory.h>
#include <Atom/RPI.Public/Pass/AttachmentReadback.h>


namespace TestScene
{
void CameraTestComponent::Activate() {
  ImGui::ImGuiUpdateListenerBus::Handler::BusConnect();

  m_pipelineName = AZStd::string ::format("FooBar_pipeline_%s", GetEntityId().ToString().c_str());

  AZ::RPI::RenderPipelineDescriptor pipelineDesc;
  pipelineDesc.m_mainViewTagName = "MainCamera"; // must be "MainCamera"
  pipelineDesc.m_name = m_pipelineName;
  pipelineDesc.m_rootPassTemplate = "MainPipelineRenderToTexture";
  pipelineDesc.m_renderSettings.m_multisampleState =
      AZ::RPI::RPISystemInterface::Get()->GetApplicationMultisampleState();
  m_renderPipeline =
      AZ::RPI::RenderPipeline::CreateRenderPipeline(pipelineDesc);
  AZ::RPI::Scene *scene =
      AZ::RPI::RPISystemInterface::Get()->GetSceneByName(AZ::Name("Main"));
  AZ_Assert(scene, "No Scene");
  scene->AddRenderPipeline(m_renderPipeline);

  m_passHierarchy.push_back(m_pipelineName);
  m_passHierarchy.push_back("CopyToSwapChain");

  AZ::Name viewName = AZ::Name("MainCamera");
  m_view = AZ::RPI::View::CreateView(viewName, AZ::RPI::View::UsageCamera);
  m_renderPipeline->SetDefaultView(m_view);
  m_targetView = scene->GetDefaultRenderPipeline()->GetDefaultView();
  if (AZ::Render::PostProcessFeatureProcessor *fp =
          scene->GetFeatureProcessor<
              AZ::Render::PostProcessFeatureProcessor>()) {
    // This will be set again to mimic the active camera in UpdateView
    fp->SetViewAlias(m_view, m_targetView);
  }

  AZ::TickBus::Handler::BusConnect();

}

void CameraTestComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
{

  const auto transform = GetEntity()->GetTransform()->GetWorldTM();
  const AZ::Transform inverse = transform.GetInverse();
  m_view->SetWorldToViewMatrix(AZ::Matrix4x4::CreateFromQuaternionAndTranslation(inverse.GetRotation(), inverse.GetTranslation()));

  if (m_tickingCamera) {
    AZ::Render::FrameCaptureId frameCaptureId{};
    auto attachmentReadbackCallback =
        [=](const AZ::RPI::AttachmentReadback::ReadbackResult &result) {
          AZ_Printf(m_pipelineName.c_str(), "name -> %s \n",
                    result.m_name.GetCStr());
          AZ_Printf(m_pipelineName.c_str(), "size -> %d %d \n",
                    result.m_imageDescriptor.m_size.m_height,
                    result.m_imageDescriptor.m_size.m_width);
        };
    AZ::Render::FrameCaptureRequestBus::BroadcastResult(
        frameCaptureId,
        &AZ::Render::FrameCaptureRequestBus::Events::
            CapturePassAttachmentWithCallback,
        m_passHierarchy, AZStd::string("Output"), attachmentReadbackCallback,
        AZ::RPI::PassAttachmentReadbackOption::Output);
  }
}


void CameraTestComponent::Deactivate()
{
  ImGui::ImGuiUpdateListenerBus::Handler::BusDisconnect();
  AZ::TickBus::Handler::BusDisconnect();
  AZ::RPI::Scene* scene = AZ::RPI::RPISystemInterface::Get()->GetSceneByName(AZ::Name("Main"));
  AZ_Assert(scene, "No Scene");
  if (AZ::Render::PostProcessFeatureProcessor* fp = scene->GetFeatureProcessor<AZ::Render::PostProcessFeatureProcessor>())
  {
    // Remove view alias introduced in CreatePipeline and UpdateView
    fp->RemoveViewAlias(m_view);
  }
  scene->RemoveRenderPipeline(m_renderPipeline->GetId());
  m_passHierarchy.clear();
  m_renderPipeline.reset();
  m_view.reset();
  m_targetView.reset();
}


void CameraTestComponent::Reflect(AZ::ReflectContext* context)
{
  if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
  {
    serialize->Class<CameraTestComponent, AZ::Component>()
        ->Version(1);

    if (AZ::EditContext* ec = serialize->GetEditContext())
    {
      ec->Class<CameraTestComponent>("CameraTestComponent", "CameraTestComponent")
          ->ClassElement(AZ::Edit::ClassElements::EditorData, "CameraTestComponent")
          ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game"))
          ->Attribute(AZ::Edit::Attributes::Category, "TestScene");
    }
  }
}


void CameraTestComponent::OnImGuiUpdate()
{
  ImGui::Begin(m_pipelineName.c_str());
  if(ImGui::Button("Save frame")){
    AZStd::string fileName = "/tmp/test.png";
    AZ::Render::FrameCaptureId frameCaptureId {};
    AZ::Render::FrameCaptureRequestBus::BroadcastResult(
        frameCaptureId, &AZ::Render::FrameCaptureRequestBus::Events::CapturePassAttachment, m_passHierarchy,
        AZStd::string("Output"),fileName, AZ::RPI::PassAttachmentReadbackOption::Output);
    AZ_Printf("OnImGuiUpdate", "Frame Id %d \n",frameCaptureId );
  }
  ImGui::Checkbox("Tick Camera", &m_tickingCamera);
  ImGui::End();

}
}