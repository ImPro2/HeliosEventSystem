# Helios Event System

_A simplistic event system in C++17_

## Table of Contents
- [Usage](/#usage)
  - [Adding Event Listeners](#/adding-event-listeners)
  - [Adding Events](#/adding-events)
  - [Dispatching Events](#/dispatching-events)
- [Cloning](/#cloning)

## Usage

To use the API, you first need to define the following in a C++ implementation file (with a .cpp extension) and build it.
```cpp
#define HELIOS_IMPLEMENTATION
#include "HeliosEventSystem.hpp"
```

### Adding Event Listeners

You need to add event listeners to the event system somewhere at the start of your program to know when events occur.

```cpp
void EventCallback(const helios::IEvent& event)
{
    // do whatever
}

// somewhere in code at the start

helios::EventSystem::AddEventListener(EventCallback);
```

### Adding Events

When events are polled, you can add them to the event system using the following API.

```cpp
// an example of a key press event happening
int key = 69;
helios::EventSystem::AddEvent(KeyPressEvent(key));
```

### Dispatching Events

You need to dispatch events in the game loop (or main loop, you know what I'm talking about), usually at the end of each frame.

```cpp
// somewhere in the game loop
helios::EventSystem::Dispatch();
```

## Cloning

So you decided to use the library? Awesome!
Type the following command into a console:

`git clone github.com/ImPro2/HeliosEventSystem`

Now that you have the repository on your machine, you can follow the steps [described here](/#usage), or you can double click the `GenProjects.bat` file to generate project files.