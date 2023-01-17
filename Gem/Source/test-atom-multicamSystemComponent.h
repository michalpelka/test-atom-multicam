
#pragma once

#include <AzCore/Component/Component.h>

#include <test-atom-multicam/test-atom-multicamBus.h>

namespace test_atom_multicam
{
    class test_atom_multicamSystemComponent
        : public AZ::Component
        , protected test_atom_multicamRequestBus::Handler
    {
    public:
        AZ_COMPONENT(test_atom_multicamSystemComponent, "{4697BBAD-8316-4520-AF18-4081F6409078}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        test_atom_multicamSystemComponent();
        ~test_atom_multicamSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // test_atom_multicamRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
