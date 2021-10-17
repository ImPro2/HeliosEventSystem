#include "EventSystem.hpp"

int main()
{
    helios::EventSystem::AddEvent(helios::KeyPressEvent('a'));
    helios::EventSystem::AddEvent(helios::MouseButtonClickEvent(2));

    helios::EventSystem::AddEventListener([](const helios::IEvent& e)
    {
        std::cout << e.ToString();

        helios::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<helios::KeyPressEvent>([](const helios::KeyPressEvent& keypress)
        { 
            std::cout << static_cast<char>(keypress.GetKey()) << "\n";
        });
    });

    helios::EventSystem::Dispatch();
    return 0;
}