
#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace test_atom_multicam
{
    class test_atom_multicamRequests
    {
    public:
        AZ_RTTI(test_atom_multicamRequests, "{874F1091-96DF-4354-AA61-3F534D36BA70}");
        virtual ~test_atom_multicamRequests() = default;
        // Put your public methods here
    };

    class test_atom_multicamBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using test_atom_multicamRequestBus = AZ::EBus<test_atom_multicamRequests, test_atom_multicamBusTraits>;
    using test_atom_multicamInterface = AZ::Interface<test_atom_multicamRequests>;

} // namespace test_atom_multicam
