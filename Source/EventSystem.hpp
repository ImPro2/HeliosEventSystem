#pragma once

/*
* Helios Event System
* This is a simple header-only library which you can use in your own projects!
* NOTICE: You need to compile this before using it somewhere
* You need to define HELIOS_IMPLEMENTATION in ONE cpp file before including it
* 
* // example
* #define HELIOS_IMPLEMENTATION
* #include "Helios.hpp"
* // end of example
* 
* Each frame you need to call helios::EventSystem::Dispatch() to dispatch all of the events
* Whenever an event occurs, you need to call helios::EventSystem::AddEvent() to add it to the queue
* Of course, you want to know when such an event happens, so you can also add a listener
* like so: helios::EventSystem::AddEventListener()
* 
* Here is an example of using the API:
* 
* int key = 69;
* helios::EventSystem::AddEvent(helios::KeyPressEvent(key));
* 
* helios::EventSystem::AddEventListener([](IEvent& event)
* {
*     EventDispatcher dispatcher(event);
*     dispatcher.Dispatch<KeyPressEvent>([](KeyPressEvent& e) {}); // this method calls a function if the event is a key press event
* });
* 
* // now inside the game loop
* 
* {
*     helios::EventSystem::Dispatch();
* }
* 
*/

#include <iostream>
#include <string>
#include <queue>
#include <any>
#include <variant>
#include <functional>
#include <vector>
#include <sstream>


#define HELIOS_EVENT_CLASS_TYPE(type)\
static ::helios::EventType GetStaticType() { return ::helios::Type_##type; }\
virtual ::helios::EventType GetType() const override { return GetStaticType(); }

#define HELIOS_EVENT_CLASS_CATEGORY(category) virtual ::helios::EventCategory GetCategory() const override { return ::helios::Category_##category; }

namespace helios
{

    enum EventType
    {
        Type_None = -1,
        Type_WindowCreate, Type_WindowDestroy,  Type_WindowMove,       Type_WindowResize,
        Type_MouseMove,    Type_MouseScroll, Type_MouseButtonClick, Type_MouseButtonRelease,
        Type_KeyPress,     Type_KeyRelease,  Type_KeyType,
    };

    enum EventCategory
    {
        Category_None        = -1,
        Category_Window      = 1 << 0,
        Category_Mouse       = 1 << 1,
        Category_MouseButton = 1 << 2,
        Category_Keyboard    = 1 << 3
    };

    // an interface that all events will inherit from
    class IEvent
    {
    public:
        virtual EventType          GetType()     const { return Type_None;     }
        virtual EventCategory      GetCategory() const { return Category_None; }
        virtual const std::string& ToString()    const { return "";            }
    };

    class WindowEvent : public IEvent
    {
    public:
        HELIOS_EVENT_CLASS_CATEGORY(Window);
    };

    class WindowCreateEvent : public WindowEvent
    {
    public:
        HELIOS_EVENT_CLASS_TYPE(WindowCreate);

        // show mode is if it's fullscreen, minimized, maximized, etc
        WindowCreateEvent(int showMode) : mShowMode(showMode) {}
        int GetShowMode() const { return mShowMode; }

        virtual const std::string& ToString() const override
        {
            static std::ostringstream oss;
            oss << "[Event:WindowCreate]: ShowMode: (" << mShowMode << ")\n";
            static std::string s = oss.str();
            return s;
        }

    private:
        int mShowMode;
    };

    class WindowDestroyEvent : public WindowEvent
    {
    public:
        HELIOS_EVENT_CLASS_TYPE(WindowDestroy);

        WindowDestroyEvent() {}

        virtual const std::string& ToString() const override
        {
            static std::ostringstream oss;
            oss << "[Event:WindowDestroy]\n";
            static std::string s = oss.str();
            return s;
        }
    };

    class WindowMoveEvent : public WindowEvent
    {
    public:
        HELIOS_EVENT_CLASS_TYPE(WindowMove);

        WindowMoveEvent(int x, int y) : mX(x), mY(y) {}
        
        virtual const std::string& ToString() const override
        {
            static std::ostringstream oss;
            oss << "[Event:WindowMove]: XPos: (" << mX << "), YPos: (" << mY << ")\n";
            static std::string s = oss.str();
            return s;
        }

        int GetX() const { return mX; }
        int GetY() const { return mY; }

    private:
        int mX, mY;
    };

    class WindowResizeEvent : public WindowEvent
    {
    public:
        HELIOS_EVENT_CLASS_TYPE(WindowResize);

        WindowResizeEvent(int width, int height) : mWidth(width), mHeight(height) {}

        virtual const std::string& ToString() const override
        {
            static std::ostringstream oss;
            oss << "[Event:WindowResize]: Width: (" << mWidth << "), Height: (" << mHeight << ")\n";
            static std::string s = oss.str();
            return s;
        }

        int GetWidth()  const { return mWidth;  }
        int GetHeight() const { return mHeight; }

    private:
        int mWidth, mHeight;
    };

    class MouseEvent : public IEvent
    {
    public:
        HELIOS_EVENT_CLASS_CATEGORY(Mouse);
    };

    class MouseMoveEvent : public MouseEvent
    {
    public:
        HELIOS_EVENT_CLASS_TYPE(MouseMove);

        MouseMoveEvent(int x, int y) : mX(x), mY(y) {}
        
        virtual const std::string& ToString() const override
        {
            static std::ostringstream oss;
            oss << "[Event:MouseMove]: XPos: (" << mX << "), YPos: (" << mY << ")\n";
            static std::string s = oss.str();
            return s;
        }

        int GetX() const { return mX; }
        int GetY() const { return mY; }

    private:
        int mX, mY;
    };

    class MouseScrollEvent : public MouseEvent
    {
    public:
        HELIOS_EVENT_CLASS_TYPE(MouseScroll);

        MouseScrollEvent(int offset) : mOffset(offset) {}

        virtual const std::string& ToString() const override
        {
            static std::ostringstream oss;
            oss << "[Event:MouseScroll]: Offset: (" << mOffset << ")\n";
            static std::string s = oss.str();
            return s;
        }

        int GetOffset() const { return mOffset; }

    private:
        int mOffset;
    };
    class MouseButtonEvent : public IEvent
    {
    public:
        HELIOS_EVENT_CLASS_CATEGORY(MouseButton);

        MouseButtonEvent(int button, bool control, bool shift, bool alt)
            : mButton(button), mControl(control), mShift(shift), mAlt(alt) {}

        int  GetButton()  const { return mButton;  }
        bool IsControl() const { return mControl;  }
        bool IsShift()   const { return mShift;    }
        bool IsAlt()     const { return mAlt;      }

    private:
        int  mButton;
        bool mControl;
        bool mShift;
        bool mAlt;
    };

    class MouseButtonClickEvent : public MouseButtonEvent
    {
    public:
        HELIOS_EVENT_CLASS_TYPE(MouseButtonClick);

        MouseButtonClickEvent(int button, bool control = false, bool shift = false, bool alt = false)
            : MouseButtonEvent(button, control, shift, alt) {}

        virtual const std::string& ToString() const override
        {
            static std::ostringstream oss;
            oss << "[Event:MouseButtonClick]: Button: (" << GetButton() << "), Control: (" << IsControl() << "), Shift: (" << IsShift() << "), Alt: (" << IsAlt() << ")\n";
            static std::string s = oss.str();
            return s;
        }
    };

    class MouseButtonReleaseEvent : public MouseButtonEvent
    {
    public:
        HELIOS_EVENT_CLASS_TYPE(MouseButtonRelease);

        MouseButtonReleaseEvent(int button, bool control = false, bool shift = false, bool alt = false)
            : MouseButtonEvent(button, control, shift, alt) {}

        virtual const std::string& ToString() const override
        {
            static std::ostringstream oss;
            oss << "[Event:MouseButtonRelease]: Button: (" << GetButton() << "), Control: (" << IsControl() << "), Shift: (" << IsShift() << "), Alt: (" << IsAlt() << ")\n";
            static std::string s = oss.str();
            return s;
        }
    };

    class KeyEvent : public IEvent
    {
    public:
        HELIOS_EVENT_CLASS_CATEGORY(Keyboard);
    };

    class KeyPressEvent : public KeyEvent
    {
    public:
        HELIOS_EVENT_CLASS_TYPE(KeyPress);

        KeyPressEvent(int key, bool control = false, bool shift = false, bool alt = false)
            : mKey(key), mControl(control), mShift(shift), mAlt(alt) {}

        virtual const std::string& ToString() const override
        {
            static std::ostringstream oss;
            oss << "[Event:KeyPress]: Key: (" << GetKey() << "), Control: (" << IsControl() << "), Shift: (" << IsShift() << "), Alt: (" << IsAlt() << ")\n";
            static std::string s = oss.str();
            return s;
        }

        int  GetKey()    const { return mKey;     }
        bool IsControl() const { return mControl; }
        bool IsShift()   const { return mShift;   }
        bool IsAlt()     const { return mAlt;     }

    private:
        int  mKey;
        bool mControl, mShift, mAlt;
    };

    class KeyReleaseEvent : public KeyEvent
    {
    public:
        HELIOS_EVENT_CLASS_TYPE(KeyRelease);

        KeyReleaseEvent(int key, bool control = false, bool shift = false, bool alt = false)
            : mKey(key), mControl(control), mShift(shift), mAlt(alt) {}

        virtual const std::string& ToString() const override
        {
            static std::ostringstream oss;
            oss << "[Event:KeyRelease]: Key: (" << GetKey() << "), Control: (" << IsControl() << "), Shift: (" << IsShift() << "), Alt: (" << IsAlt() << ")\n";
            static std::string s = oss.str();
            return s;
        }

        int  GetKey()    const { return mKey;     }
        bool IsControl() const { return mControl; }
        bool IsShift()   const { return mShift;   }
        bool IsAlt()     const { return mAlt;     }

    private:
        int  mKey;
        bool mControl, mShift, mAlt;
    };

    class KeyTypeEvent : public KeyEvent
    {
    public:
        HELIOS_EVENT_CLASS_TYPE(KeyType);

        KeyTypeEvent(char c) : mChar(c) {}

        virtual const std::string& ToString() const override
        {
            static std::ostringstream oss;
            oss << "[Event:KeyType]: Char: (" << mChar << ")\n";
            static std::string s = oss.str();
            return s;
        }

        char GetChar() const { return mChar; }

    private:
        char mChar;
    };

    //
    ///////////////////////////////////////////////////////////////////////
    // Event System Implementation
    ///////////////////////////////////////////////////////////////////////
    //

    using EventBus       = std::queue<std::pair<std::any, EventType>>;
    using EventListener  = std::function<void(const IEvent&)>;

    class EventSystem
    {
    public:
        static void Init();                            // No need to call this, just reserves capacity for the event bus

        template<typename T>
        static void AddEvent(T e);                     // Adds an event to the queue

        template<typename T>
        static void AddCustomEvent(T e);               // Adds an event that you made :)

        static void AddEventListener(EventListener e); // Add an event listener to a queue
        static void Dispatch();                        // Dispatch all events

    private:
        template<typename T>
        static void IterateThroughEventListeners(const T& e);

    private:
        static EventBus sEventBus;
        static std::vector<EventListener> sEventListeners;
    };

    template<typename T>
    inline void EventSystem::AddEvent(T e)
    {
        static_assert(std::is_base_of<IEvent, decltype(e)>::value);
        sEventBus.push({ e, e.GetType() });
    }

    //
    ///////////////////////////////////////////////////////////////////////
    // Utility
    ///////////////////////////////////////////////////////////////////////
    //

    // helper class to use in an event listener to call functions if the event is a certain type

    class EventDispatcher
    {
    public:
        EventDispatcher(const IEvent& e)
            : mEvent((IEvent&)e)
        {
        }

        // T is the type of event
        // F gets deduced by the compiler

        template<typename T, typename F> 
        void Dispatch(F& func)
        {
            if (mEvent.GetType() == T::GetStaticType())
                func(*static_cast<T*>(&mEvent));
        }

    private:
        IEvent& mEvent;
    };

}

#ifdef HELIOS_IMPLEMENTATION
namespace helios
{

    EventBus                   EventSystem::sEventBus;
    std::vector<EventListener> EventSystem::sEventListeners;

    void EventSystem::Init()
    {
        sEventListeners.reserve(69); // I don't think there will be more event listeners than this

    }

    void EventSystem::AddEventListener(EventListener e)
    {
        sEventListeners.push_back(e);
    }

    void EventSystem::Dispatch()
    {
        for (int i = 0; i <= sEventBus.size() + 1; i++)
        {
            auto& event = sEventBus.front();

            switch (event.second)
            {
                case Type_WindowCreate:         IterateThroughEventListeners(std::any_cast<WindowCreateEvent&>      (event.first)); break;
                case Type_WindowDestroy:        IterateThroughEventListeners(std::any_cast<WindowDestroyEvent&>     (event.first)); break;
                case Type_WindowMove:           IterateThroughEventListeners(std::any_cast<WindowMoveEvent&>        (event.first)); break;
                case Type_WindowResize:         IterateThroughEventListeners(std::any_cast<WindowResizeEvent&>      (event.first)); break;
                case Type_KeyPress:             IterateThroughEventListeners(std::any_cast<KeyPressEvent&>          (event.first)); break;
                case Type_KeyRelease:           IterateThroughEventListeners(std::any_cast<KeyReleaseEvent&>        (event.first)); break;
                case Type_KeyType:              IterateThroughEventListeners(std::any_cast<KeyTypeEvent&>           (event.first)); break;
                case Type_MouseScroll:          IterateThroughEventListeners(std::any_cast<MouseScrollEvent&>       (event.first)); break;
                case Type_MouseMove:            IterateThroughEventListeners(std::any_cast<MouseMoveEvent&>         (event.first)); break;
                case Type_MouseButtonClick:     IterateThroughEventListeners(std::any_cast<MouseButtonClickEvent&>  (event.first)); break;
                case Type_MouseButtonRelease:   IterateThroughEventListeners(std::any_cast<MouseButtonReleaseEvent&>(event.first)); break;
                default: __debugbreak();
            }

            sEventBus.pop();
        }
    }

    template<typename T>
    void EventSystem::IterateThroughEventListeners(const T& e)
    {
        for (EventListener listener : sEventListeners)
        {
            listener(e);
        }
    }

}
#endif