/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
 */
#pragma once

#include <Atom/Feature/Utils/FrameCaptureBus.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Script/ScriptTimePoint.h>
#include <ImGuiBus.h>
#include <AzCore/Component/TickBus.h>

namespace TestScene
{
//! Component responsible for storing counters of apples gathered by Kraken.
class CameraTestComponent
    : public AZ::Component
    , public ImGui::ImGuiUpdateListenerBus::Handler
    , public AZ::TickBus::Handler
{
public:
  AZ_COMPONENT(CameraTestComponent, "{a0b93eea-8111-11ed-a1bf-0242ac430002}", AZ::Component);
  CameraTestComponent() = default;
  ~CameraTestComponent() = default;
  void Activate() override;
  void Deactivate() override;
  static void Reflect(AZ::ReflectContext* context);

private:
  void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
  bool m_tickingCamera{false};
  AZStd::string m_pipelineName;
  AZStd::vector<AZStd::string> m_passHierarchy;
  AZ::RPI::RenderPipelinePtr m_pipeline;
  AZ::RPI::ViewPtr m_view; //!< The view associated with the render pipeline.
  AZ::RPI::ViewPtr m_targetView; //!< The view that this render pipeline will mimic.
  AZ::RPI::RenderPipelinePtr m_renderPipeline;
  AZ::RPI::Scene* m_scene = nullptr;

  void OnImGuiUpdate() override;
  };
} // namespace AppleKraken