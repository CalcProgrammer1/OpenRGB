# Shared state cache {#shared-state}

[TOC]

## What shared state means
There are two ways in which the API state (internally JSON) can be handled:
1. Every resource instance holds its own cache of the state (default).
2. All instances share the cache of the entire bridge.

### Advantages of shared state
* Different resources are always consistent on the library level.
If one part of the code uses the light with id 1 and turns it off, 
light 1 is also off when using a different variable to access it.
* The number of requests can be reduced, because they can be bundled together on a higher cache level.

### Disadvantages of shared state
* Different objects are no longer thread safe, you cannot use **any** parts of the library 
from multiple threads (without locking).
* Changes are not transparent. For example, a `const Light` can suddenly change its name, because the
name was changed somewhere else in the code.

Because of these considerations, shared state is disabled by default.

## Enabling shared state
Shared state can be configured when the bridge is first constructed, either in [getBridge()](@ref hueplusplus::BridgeFinder::getBridge)
or in the [constructor](@ref hueplusplus::Bridge::Bridge). Set `sharedState` to `true` to keep all resources
connected to the bridge cache.
\snippet Snippets.cpp shared-bridge-1
\snippet Snippets.cpp shared-bridge-2

## Shared state and refreshing
When shared cache is used, refreshes use a hierarchichal structure to determine how much should be requested from the bridge.
Every level has its own last update time and refresh duration.
First, it is checked whether the higher level is up to date and refresh everything if not.
Otherwise, only the lowest necessary level is requested from the bridge to be more efficient.

### Example:

\snippet Snippets.cpp refresh-example
[isOn()](@ref hueplusplus::Light::isOn) is a non-const method (in this case). That means it will refresh the
state if it is outdated. The default refresh time is inherited from `bridge.lights()`, so it is 30 seconds.
After 30 seconds, the state of `light` *and* `bridge.lights()` is outdated. Therefore, the entire list of lights is
updated at this point.

After more than one minute, the bridge state is considered outdated. This means that `isOn()` causes an update of
the entire bridge.
