
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "test-atom-multicamSystemComponent.h"
#include "CameraTestComponent.h"

namespace test_atom_multicam
{
    class test_atom_multicamModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(test_atom_multicamModule, "{A3F9BCFA-0429-4875-84A1-CA72C45DD94F}", AZ::Module);
        AZ_CLASS_ALLOCATOR(test_atom_multicamModule, AZ::SystemAllocator, 0);

        test_atom_multicamModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                test_atom_multicamSystemComponent::CreateDescriptor(),
                TestScene::CameraTestComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<test_atom_multicamSystemComponent>(),
            };
        }
    };
}// namespace test_atom_multicam

AZ_DECLARE_MODULE_CLASS(Gem_test_atom_multicam, test_atom_multicam::test_atom_multicamModule)
