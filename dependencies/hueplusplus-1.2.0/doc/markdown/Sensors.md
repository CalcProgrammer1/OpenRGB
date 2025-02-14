# Sensors {#sensors}

[TOC]

## Sensor support
The library supports the sensor types listed on the Hue developer documentation.
Include `hueplusplus/ZLLSensors.h` for ZigBee sensors and `hueplusplus/CLIPSensors.h` for CLIP sensors.
Other sensors can be used with the generic [Sensor](@ref hueplusplus::Sensor) class.

### Working with a known sensor
In most cases, the type of the sensors is known in advance, such as a switch.
The classes in the [sensors](@ref hueplusplus::sensors) namespace provide the documented
functionality. The type can be specified when accessing the sensor. When it does not match,
an exception is thrown.
\snippet Snippets.cpp known-sensor-1

You can also get all sensors of a specified type by using [getAllByType<T>()](@ref hueplusplus::SensorList::getAllByType).
\snippet Snippets.cpp known-sensor-2

### Working with an unknown sensor
When the sensor type is not known, use the generic sensor class. In this case, some attributes might not
exist, so they have to be checked first. This applies to all attributes that have a `hasXXX` method.
\snippet Snippets.cpp generic-sensor-1

It is easiest to compare the sensor type to the existing ones (`typeStr` on the specific sensor classes)
and then convert the sensor to that type.
\snippet Snippets.cpp generic-sensor-2

## ZLL sensors vs. CLIP sensors
ZLL sensors (defined in `ZLLSensors.h`) are physical device sensors which send their data
to the bridge using ZigBee. They are added in the same way as lights are, using [search()](@ref hueplusplus::SearchableResourceList::search).

CLIP sensors (in `CLIPSensors.h`) are added using [create()](@ref hueplusplus::CreateableResourceList::create) with [CreateSensor](@ref hueplusplus::CreateSensor)
for parameters. In general, which config and state attributes exist is specified when the sensor is created.
The values of CLIP sensors can be changed using requests, unlike ZLL sensors. They can also have a URL to query from.

## Creating conditions
The most important use for sensors is in [Rules](@ref hueplusplus::Rule), to trigger changes.
Conditions can be created from the specific sensor types using `makeCondition()`.

These functions return a helper class with methods for the [possible operators](@ref hueplusplus::Condition::Operator) valid for the state.

For some sensors, which have multiple possible states, there exist multiple variations of makeCondition.
\snippet Snippets.cpp sensor-conditions

For generic sensors, the conditions must be created manually using the [Condition](@ref hueplusplus::Condition::Condition)
constructor with a proper address to the sensor state.
