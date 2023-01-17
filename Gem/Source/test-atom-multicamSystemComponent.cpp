
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>

#include "test-atom-multicamSystemComponent.h"

namespace test_atom_multicam
{
    void test_atom_multicamSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<test_atom_multicamSystemComponent, AZ::Component>()
                ->Version(0)
                ;

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<test_atom_multicamSystemComponent>("test_atom_multicam", "[Description of functionality provided by this System Component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }
    }

    void test_atom_multicamSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("test_atom_multicamService"));
    }

    void test_atom_multicamSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("test_atom_multicamService"));
    }

    void test_atom_multicamSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void test_atom_multicamSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    test_atom_multicamSystemComponent::test_atom_multicamSystemComponent()
    {
        if (test_atom_multicamInterface::Get() == nullptr)
        {
            test_atom_multicamInterface::Register(this);
        }
    }

    test_atom_multicamSystemComponent::~test_atom_multicamSystemComponent()
    {
        if (test_atom_multicamInterface::Get() == this)
        {
            test_atom_multicamInterface::Unregister(this);
        }
    }

    void test_atom_multicamSystemComponent::Init()
    {
    }

    void test_atom_multicamSystemComponent::Activate()
    {
        test_atom_multicamRequestBus::Handler::BusConnect();
    }

    void test_atom_multicamSystemComponent::Deactivate()
    {
        test_atom_multicamRequestBus::Handler::BusDisconnect();
    }
}
