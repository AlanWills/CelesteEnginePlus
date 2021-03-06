#include "Mocks/Objects/MockManagedComponent.h"
#include "UtilityHeaders/ComponentHeaders.h"


namespace CelesteMocks
{
  REGISTER_COMPONENT(MockManagedComponent, 10);

  MockManagedComponent::MockManagedComponent(Celeste::GameObject& gameObject) :
    Celeste::Component(gameObject)
  {
  }
}