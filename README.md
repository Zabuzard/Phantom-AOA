# Phantom-AOA

This project simulates the **Angle Of Attack** (AOA) system of
the **Phantom F-4E** fighter jet.

![demo](https://i.imgur.com/2f0AXtm.gif)

It comes as an interactive console application with a simple text-based GUI.
For the best experience, it should be viewed on an ANSI-compatible console.
As of now, only Windows is supported.

## Controls

The simulation can be interacted with, by using keys. The controls are as follows:

### Aircraft

The aircraft's position in relation to the flight path (which is kept stationary in the simulation),
can be changed by holding down the following keys

* <kbd>↑</kbd> pitch down (drop nose)
* <kbd>↓</kbd> pitch up (lift nose)
* <kbd>→</kbd> rudder right (changes yaw, not roll)
* <kbd>←</kbd> rudder left (changes yaw, not roll)

### Power

#### Bus

All buses are powered by default already, their state can be toggled with

* <kbd>Q</kbd> Essential 28V DC
* <kbd>W</kbd> Right Main 115/200V AC
* <kbd>E</kbd> Instrument 115/200V AC
* <kbd>R</kbd> Left Main 115/200V AC

#### Circuit Breaker

Circuit breakers are, by default, pushed in. Their state can be toggled with

* <kbd>A</kbd> AOA PROBE HTR PWR (power to the AOA sensor)
* <kbd>S</kbd> AOA PROBE HTR CONT (power to the other components of the AOA system)
* <kbd>D</kbd> ESS DC CADC (whether the central computer receives power through the essential bus)
* <kbd>F</kbd> INSTR AC CADC (whether the central computer receives power through the instrument bus)
* <kbd>G</kbd> RM VAC AC CADC PWR 1 (whether the central computer receives power through the right main bus)
* <kbd>H</kbd> RM VAC AC CADC PWR 2 (whether the central computer receives power through the right main bus)
* <kbd>J</kbd> RM VAC AC CADC PWR 3 (whether the central computer receives power through the right main bus)

### Knobs

The aircraft has a few knobs that can be rotated to decrease or increase their value respectively

* <kbd>T</kbd><kbd>Z</kbd> Instrument Panel light intensity
* <kbd>U</kbd><kbd>I</kbd> Emergency Floodlight (technically a 3-way switch, increase beyond 0.3 and 0.6 respectively)
* <kbd>O</kbd><kbd>P</kbd> AOA indexer light intensity
* <kbd>K</kbd><kbd>L</kbd> Aural Stall Warning Tone volume

### Flag

Further, there are certain states, provided as simple boolean flags, which can be toggled.

* <kbd>Y</kbd> Nose-Wheel (extend/retract)
* <kbd>X</kbd> Weight On Wheel (on-ground/in-flight) 
* <kbd>C</kbd> Gear (extend/retract)
* <kbd>V</kbd> Slats (in/out)
* <kbd>B</kbd> AGM-45 mode (select/unselect)
* <kbd>N</kbd> AOA sensor icing (freeze/not frozen)

For simplicity, the gear flags all act independently of each other.
For example, applying the gear flag will  not apply the nose-wheel flag as well.

## Features

The aircraft's AOA system consists of the following components:

* sensor
* indicator
* indexer
* stall warning vibrator
* aural tone system

For which, the simulated features are:

### Sensor

* measure AOA based on aircraft's 3 axis (pitch, roll, yaw) and the flight path vector
* (also measure side-slip angle in a similar way)
* power based on bus and circuit breakers
* warmup-time based on outside temperature
* error induced by an extended nose-wheel
* error induced by side-slip
* error induced by a frozen sensor

### Indicator

* display the AOA received by sensor
* power based on bus and circuit breakers
    * needle is stuck if no power
    * shows OFF-flag if no power
* shows AOA within limits of 0-30°
* needle dampening to prevent jitter
* needle lag (does not catch up to the sensor reading immediately)
* has 3 lamps
    * illuminate based on AOA reading
    * either red or white, depending on knobs
    * power based on bus and circuit breakers
        * white, red BRT and red DIM are energized differently
    * light intensity based on knob

### Indexer

* illuminate 3 lamps based on AOA received by sensor
* power based on bus and circuit breakers
* lamp status based on AOA ranges
* light intensity based on knob
    * power based on bus and circuit breakers
    * knob can not turn them off, only dim them
* special AGM-45 mode
    * lamps then indicate AGM-45 cues instead of AOA

### Stall Warning Vibrator

* vibrates based on AOA received by sensor (vibration is just visualized)
* power based on bus and circuit breakers
* only active if no weight on wheels (in-air)

### Aural Tone System

* emits tones based on AOA received by sensor (tones are just visualized)
* power based on bus and circuit breakers
* different tone profiles based on slats and gear status
    * tones have a frequency, volume and can be either
      steady or interrupted (pulse per second)
* volume based on knob
    * tone profile can prevent muting in certain situations

### Illustrations

#### Sensor

Sensor measurements:
![sensor](https://i.imgur.com/2QfuBRp.png)

Warmup:
![warmup](https://i.imgur.com/pRYiWs3.png)

Nose-wheel error:
![nose-wheel](https://i.imgur.com/lpVEByU.png)

Side-slip error:
![side-slip](https://i.imgur.com/4kZu0KW.png)

Frozen sensor error:
![frozen](https://i.imgur.com/EknxikY.png)

#### Indicator

Indicator:
![indicator](https://i.imgur.com/YKVA9Ag.png)

Power off, needle stuck:
![power off](https://i.imgur.com/uwGijkV.png)

Indicator is limited:
![limit](https://i.imgur.com/t5Vp0K4.png)

Needle lags:
![lag](https://i.imgur.com/2UCMK70.png)

Medium-AOA white lamp with intensity
![white lamp](https://i.imgur.com/aqJGqW0.png)

High-AOA red lamp with intensity
![red lamp](https://i.imgur.com/QPNMIBd.png)

#### Indexer

Indexer lamps:
![indexer](https://i.imgur.com/V4gQrfv.png)

No power:
![no power](https://i.imgur.com/uHSdpEG.png)

Light intensity:
![light intensity](https://i.imgur.com/fT2wLbf.png)

AGM-45 mode:
![agm mode](https://i.imgur.com/DgWxn5B.png)

#### Stall Warning Vibrator

Stall-warning:
![stall warning](https://i.imgur.com/ePKpHRH.png)

Inactive on ground:
![on ground](https://i.imgur.com/021tJl7.png)

#### Aural Tone System

Emits tones:
![tones](https://i.imgur.com/EVM7ZkA.png)

No power:
![no power](https://i.imgur.com/SxtF8tn.png)

Muted with knob:
![muted](https://i.imgur.com/FnBgnsi.png)

Prevents muting in critical situation:
![prevented muting](https://i.imgur.com/wj4laNg.png)

## Code Base

At the moment, only Windows is supported. This is due to how the system reads keyboard input.

The flow starts with `main.cpp`, which simply starts `Simulation`.

### Simulation

`Simulation` consists of a simple _game loop_ that controls logical updates and rendering using `TPS` and `FPS` respectively.
It maintains a list of `Entity`, for which it will call `update(double deltaTime)` during a logical update (tick)
and `render()` when rendering the next frame.

As of now, `update` and `render` can not be called in-parallel, since state changes in `update`
are not all synchronized/guarded yet.

The list of entities consists of `Engine` and `Phantom` (the aircraft and all its components).

### Engine

The `Engine` class represents all external inputs which are out of the scope for this simulation.
For example measuring the outside temperature or maintaining the aircraft's position.

It also handles all user inputs and maintains the states of all corresponding knobs and similar.

Other classes will use it as main source for external input.

#### Input

The way the engine handles input, right now, is a bit over the place and grew organically. It consists of a few enums:

* `Bus`
* `CircuitBreaker`
* `Flag`
* `Knob`

and `Controls`. Since enums can not hold other state or properties, they are represented with multiple maps.
Each of them has a map to represent:

* `entry` to `name` for visual display (`render`)
* `key` to `entry` (to map controls to the corresponding system)

and further possible extra state, such as `activatedFlags`, `poweredBuses`, `knobToValue`, `pushedCircuitBreakers`.

### Phantom

`Phantom` represents the aircraft itself. It maintains a list of all its own entities,
such as the `AOASensor`, `AOAIndicator` and more.

It essentially just forwards all calls, like `update` or `render` to all its components.

### Math

All relevant logic for math and physics can be found in `Math` and `Vector3`.

## Q&A

### How to simulate

> Given that the atmosphere is already fully simulated, how would you simulate the AOA system?

#### Pressure

In reality, the sensor measures the AOA with a slotted probe. The probe has two holes that allow air to stream in.
Difference in air pressure between the areas behind those two holes, created by a change of AOA, physically move a surface within the probe.
This movement then induces changes in electrical resistance, which is translated into the sensors signal.

Simulating it that way requires that the engine is capable of providing precise atmospheric pressure readings
for a given 3D position. The advantage of that would be that the system behaves as close to reality as possible,
and would naturally support edge cases that can occur when, for example flying through clouds or other
environments with rapidly changing air pressure.

In practice, however, such a deep simulation is often not applicable and for most engines likely also not feasible.

#### Position

Another option is to just measure the AOA based on the aircraft's position and attitude in space,
together with its flight path vector. This information can typically be provided by engines more easily.

The AOA can then be computed by basic means of 3D geometry:

![geometry](https://i.imgur.com/kcjbwRu.png)

This approach will provide an accurate AOA reading in all situations, attitudes and motions. While this sounds great,
it is also a disadvantage at the same time. It implies that more effects have to be simulated manually,
such as errors induced by side-slip, or by an extended gear that changes the air-flow in front of the probe.

Once the sensor is simulated, it can simply forward its readings to all other components that rely on it.

#### Components

Code-wise, it is likely meaningful to employ OOP here and represent each component in the system as its own
individual class, which possibly has dependencies on other classes. For example, the AOA system consists of components, like:
* sensor
* heaters
* circuit breakers
* indicator
* indexer
* stall warning vibrator
* aural tone system

That way, each component can simulate its own quirks individually. It also allows us to create multiple, individual
instances per component. For example, the aircraft has 2 indicators and 4 indexers. Each of them could then exist individually,
allowing individual interaction.

Some things likely do not need to be represented as individual components in the simulation, such as the heaters. The heaters
can not be interacted with individually and only influence the sensors capability to measure accurately.
It may or may not be feasible to just represent this effect within the sensor itself, depending on outside conditions,
such as the temperature. This could change if the environment allows for more detailed simulation,
such as combat-damage of the heater unit itself.

### Who depends on it?

> What simulation outputs are necessary and for which systems are they critical?

#### Systems

The AOA system is a core component of the aircraft system. Its data is not only relevant for its own components, such as:

* indicator
* indexer
* stall warning vibrator
* aural tone system

But is also fed directly into the Central Aircraft Data Computer (CADC), from which it is forwarded to all other systems that need it.

Some systems that rely on it are:

* HUD
* navigation system (INS, ...)
* flight control system (slats, engine inlets, ...)
* weapon system (lead computation, CCIP, AGM-45, ...)
* tests (BITs and test switches might spin the needles around and light all lamps)

Further, there are also systems that should be given access to the true physical AOA, without any simulation effects applied by the sensor.
The simulation of the flight model or engine performance needs access to the real AOA in order to simulate changes during extreme conditions.
As an example, the aircraft starts rolling to the side under very high AOAs, or the engines may flame-out. 

#### Exposed data

The core data that has to be exposed to other systems is essentially the AOA reading of the sensor itself.

Further, some systems, such as the AGM-45, requires control of the AOA indexer. It can override the lamp states to indicate cues.

Apart from that, the AOA system mostly just has to expose its data for display/rendering in the cockpit.
For example, the AOA indicator has to move the needle to the indicated position.

The components also need to be able to read certain data from the system, such as the state of relevant buses or circuit
breakers, as well as all knobs and switches that can be used to control them.
For example the light intensity knobs for the AOA indexers.

### Which failures?

> What kind of basic failures in the AOA system would you simulate?

The most obvious _failures_ are related to the system not having power. Some reasons could be:
* engines out, battery out, no external power
* circuit breaker pulled
* generator out
* combat damage

Further, the sensor can become iced if flying in cold conditions and the heaters are turned off or damaged.

But there are also failures that are just unrealistic to happen within the context of the simulation.
It is not necessary to simulate those. For example:
* mean operating time between failures (1500 hours)
* minimum operating life (6000 hours)
* error induced by voltage spikes
* errors when applying very high or low temperatures
* applying force to the probe until it bends

Relevant failures that could potentially be simulated are combat damage in the corresponding areas on the aircraft
where the systems are located at. For example, when receiving gun fire near the probe, it could get damaged,
disabling the sensor and everything that relies on it.
