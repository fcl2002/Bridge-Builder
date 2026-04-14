# CLASS.md — Project Class Documentation

Bridge-building simulation game in C++ using SFML. The codebase is split into two layers:
- **Core physics** (`classes/`) — simulation logic, no rendering
- **Graphics & UI** (`graphics/`) — SFML rendering, scenes, levels, HUD

---

## Table of Contents

1. [Vec2](#1-vec2)
2. [Physics](#2-physics)
3. [Node](#3-node)
4. [Element](#4-element)
5. [WoodBeam](#5-woodbeam)
6. [Bridge](#6-bridge)
7. [Vehicle](#7-vehicle)
8. [Level (core)](#8-level-core)
9. [Button](#9-button)
10. [Car](#10-car)
11. [HUD](#11-hud)
12. [Scene](#12-scene)
13. [Level (graphics)](#13-level-graphics)
14. [Level1](#14-level1)
15. [Level2](#15-level2)
16. [Level3](#16-level3)

---

## 1. Vec2

**File**: [classes/Vec2.h](classes/Vec2.h)  
**Kind**: `struct`  
**Description**: 2D vector used throughout the physics layer for positions, velocities, and forces.

### Properties

| Name | Type | Description |
|------|------|-------------|
| `x` | `float` | X component |
| `y` | `float` | Y component |

### Constructors

| Signature | Description |
|-----------|-------------|
| `Vec2()` | Initializes to (0, 0) |
| `Vec2(float x, float y)` | Initializes with given components |

### Methods

| Signature | Description |
|-----------|-------------|
| `Vec2 operator+(const Vec2&)` | Vector addition |
| `Vec2 operator-(const Vec2&)` | Vector subtraction |
| `Vec2 operator*(float)` | Scalar multiplication |
| `Vec2 operator/(float)` | Scalar division (throws on zero) |
| `Vec2 operator-()` | Negation |
| `Vec2& operator+=(const Vec2&)` | Compound addition |
| `Vec2& operator-=(const Vec2&)` | Compound subtraction |
| `Vec2& operator*=(float)` | Compound scalar multiplication |
| `float dot(const Vec2&)` | Dot product |
| `float magnitude()` | Euclidean length |
| `float magnitudeSquared()` | Squared length (avoids sqrt) |
| `Vec2 normalized()` | Unit vector in same direction |
| `void zero()` | Resets to (0, 0) |
| `bool operator==(const Vec2&)` | Equality comparison |

---

## 2. Physics

**File**: [classes/Physics.h](classes/Physics.h)  
**Kind**: `static class`  
**Description**: Global constants for the physics simulation. Cannot be instantiated.

### Static Constants

| Name | Value | Description |
|------|-------|-------------|
| `GRAVITY` | `9.81f` | Gravitational acceleration (m/s²) |
| `DAMPING` | `0.99f` | Energy loss factor per step |
| `DT` | `0.016f` | Timestep for 60 FPS simulation |
| `K_STIFFNESS` | `1000.0f` | Default spring stiffness (N/px) |
| `FORCE_BEAM_MAX` | `2000.0f` | Default beam breaking threshold (N) |
| `MASS_PER_LENGTH` | `0.00f` | Beam mass per unit length (kg/px) |

> Constructor is deleted — use constants directly via `Physics::GRAVITY`, etc.

---

## 3. Node

**File**: [classes/Node.h](classes/Node.h) · [classes/Node.cpp](classes/Node.cpp)  
**Kind**: `class`  
**Description**: A point mass in the physics simulation. Stores position, velocity, and accumulated forces. Fixed nodes act as immovable anchors.

### Properties

| Name | Type | Description |
|------|------|-------------|
| `position` | `Vec2` | Current world position |
| `velocity` | `Vec2` | Current velocity |
| `forceAccumulator` | `Vec2` | Sum of all forces this frame |
| `mass` | `float` | Node mass (kg) |
| `isFix` | `bool` | If `true`, node is immovable |
| `initialPosition` | `Vec2` | Position at creation (used for reset) |
| `accumulatedMass` | `float` | Extra mass added by vehicles/beams |

### Constructor

```cpp
Node(Vec2 position, float mass = 1.0f, bool isFix = false)
```

### Methods

| Signature | Description |
|-----------|-------------|
| `void applyForce(const Vec2& force)` | Adds force to accumulator; no-op if fixed |
| `void update()` | Integrates velocity and position (Symplectic Euler) |
| `void clearForces()` | Resets force accumulator and accumulated mass |
| `void addMass(float m)` | Adds to the accumulated mass |

---

## 4. Element

**File**: [classes/Element.h](classes/Element.h) · [classes/Element.cpp](classes/Element.cpp)  
**Kind**: `abstract class`  
**Description**: Abstract base for all structural elements (beams, cables, etc.). Connects two nodes and manages their axial interaction.

### Properties

| Name | Type | Description |
|------|------|-------------|
| `nodeA` | `Node*` | First connected node |
| `nodeB` | `Node*` | Second connected node |
| `restLength` | `float` | Natural (unstressed) length |
| `isBroken` | `bool` | `true` after structural failure |

### Constructor

```cpp
Element(Node* nodeA, Node* nodeB, float restLength)
```

### Pure Virtual Methods (must override)

| Signature | Description |
|-----------|-------------|
| `virtual void applyForces() = 0` | Apply axial spring forces to nodes |
| `virtual void checkBreaking() = 0` | Set `isBroken` if failure threshold exceeded |

### Concrete Methods

| Signature | Description |
|-----------|-------------|
| `float currentLength()` | Distance between nodeA and nodeB |
| `float axialStrain()` | Normalized strain: `(L - L₀) / L₀` (positive = tension) |

---

## 5. WoodBeam

**File**: [classes/WoodBeam.h](classes/WoodBeam.h) · [classes/Woodbeam.cpp](classes/Woodbeam.cpp)  
**Kind**: `class` (inherits `Element`)  
**Description**: Wooden beam element. Uses Hooke's law for elastic forces and breaks when the axial force exceeds its threshold.

### Static Constants

| Name | Value | Description |
|------|-------|-------------|
| `ELASTIC_MODULUS` | `10e9f` | Young's modulus (Pa) |
| `SECTION_AREA` | `0.01f` | Cross-section area (m²) |

### Properties

| Name | Type | Description |
|------|------|-------------|
| `forceBeamMax` | `float` | Breaking force threshold (N) |
| `stiffness` | `float` | Spring constant k (N/m) |
| `colorFactor` | `float` | Stress visualization [0=compression, 0.5=neutral, 1=tension] |
| `isRoad` | `float` | `true` if part of the road surface |
| `cost` | `float` | Budget cost of this beam |

### Constructor

```cpp
WoodBeam(Node* nodeA, Node* nodeB, float restLength,
         float forceBeamMax = Physics::FORCE_BEAM_MAX,
         bool isRoad = false, float cost = 1.0f)
```

### Methods

| Signature | Description |
|-----------|-------------|
| `void applyForces() override` | Applies `F = k * (L - L₀)` along the beam axis |
| `void checkBreaking() override` | Sets `isBroken = true` if axial force > `forceBeamMax` |
| `void updateColorFactor()` | Recomputes stress color for visual feedback |

---

## 6. Bridge

**File**: [classes/Bridge.h](classes/Bridge.h) · [classes/Bridge.cpp](classes/Bridge.cpp)  
**Kind**: `class`  
**Description**: Owns and manages all nodes and elements. Orchestrates the full physics simulation pipeline each frame.

### Properties

| Name | Type | Description |
|------|------|-------------|
| `nodes` | `std::vector<std::unique_ptr<Node>>` | All nodes in the simulation |
| `elements` | `std::vector<std::unique_ptr<Element>>` | All beam elements |
| `hasCollapsed` | `bool` | `true` when all road beams have broken |
| `totalCost` | `float` | Sum of costs of all placed beams |

### Constructor

```cpp
Bridge()  // initializes empty simulation
```

### Public Methods

| Signature | Description |
|-----------|-------------|
| `Node* addNode(Vec2 position, float mass = 1.0f, bool isFix = false)` | Creates and registers a new node |
| `WoodBeam* addWoodBeam(Node* nodeA, Node* nodeB, float forceBeamMax, bool isRoad, float cost)` | Adds a wood beam between two nodes |
| `void step(const std::vector<Vehicle*>& vehicles)` | Runs one full simulation frame |

### Private Methods

| Signature | Description |
|-----------|-------------|
| `void clearForces()` | Resets all node accumulators |
| `void applyGravity()` | Applies gravity to all free nodes |
| `void applyElementForces()` | Calls `applyForces()` on every element |
| `void updateNodes()` | Integrates all nodes forward in time |
| `void checkCollapse()` | Sets `hasCollapsed` if all road beams are broken |
| `void checkAllBreaking()` | Calls `checkBreaking()` on every element |
| `void applyVehicleForces(const std::vector<Vehicle*>&)` | Distributes vehicle weights to nodes |

---

## 7. Vehicle

**File**: [classes/Vehicle.h](classes/Vehicle.h) · [classes/Vehicle.cpp](classes/Vehicle.cpp)  
**Kind**: `class`  
**Description**: A moving load that traverses road beams left to right. Distributes its weight to the two closest nodes via linear interpolation.

### Properties

| Name | Type | Access | Description |
|------|------|--------|-------------|
| `weight` | `float` | public | Downward force (N) |
| `speed` | `float` | public | Horizontal speed (px/s) |
| `currPos` | `float` | public | Current position along road |
| `hasFallen` | `bool` | public | `true` if vehicle fell off a broken beam |
| `currElem` | `WoodBeam*` | private | Current road element |
| `_localOffset` | `float` | private | Offset within current element |

### Constructor

```cpp
Vehicle(float weight, float speed)
```

### Methods

| Signature | Description |
|-----------|-------------|
| `void update(const std::vector<WoodBeam*>& roadElements)` | Advances position, detects beam breaks |
| `void applyWeightToNodes()` | Distributes weight between nodeA and nodeB of current beam |
| `WoodBeam* getCurrentElement()` | Returns the beam the vehicle is currently on |
| `Vec2 getPosition()` | Returns interpolated world position on current beam |

---

## 8. Level (core)

**File**: [classes/Level.h](classes/Level.h) · [classes/Level.cpp](classes/Level.cpp)  
**Kind**: `class`  
**Description**: Defines the structural rules and win/loss conditions for a level. Holds the `Bridge` instance and validates user actions against constraints.

### Properties

| Name | Type | Access | Description |
|------|------|--------|-------------|
| `name` | `std::string` | public | Level display name |
| `vehicleStart` | `float` | public | Vehicle starting X position |
| `vehicleEnd` | `float` | public | Vehicle goal X position |
| `maxBeamLength` | `float` | public | Maximum allowed beam length (px) |
| `budget` | `float` | public | Total cost budget |
| `bridge` | `Bridge` | public | Owned bridge simulation |
| `_anchorPositions` | `std::vector<Vec2>` | private | Fixed node coordinates |
| `_anchorNodes` | `std::vector<Node*>` | private | Pointers to anchor nodes |

### Constructor

```cpp
Level(const std::string& name,
      const std::vector<Vec2>& anchorPositions,
      float vehicleStart, float vehicleEnd,
      float maxBeamLength, float budget)
```

Creates the level and inserts fixed anchor nodes into the bridge.

### Methods

| Signature | Description |
|-----------|-------------|
| `const std::vector<Node*>& getAnchor()` | Returns anchor node pointers |
| `bool canAddBeam(const Node*, const Node*, float cost)` | Validates length and budget constraints |
| `WoodBeam* tryAddBeam(Node*, Node*, float cost, bool isRoad)` | Adds beam only if `canAddBeam` passes |
| `Vehicle createVehicle(float weight, float speed)` | Creates a vehicle at `vehicleStart` |
| `float remainingBudget()` | Returns `budget - totalCost` |
| `bool isVictory(const Vehicle&, const std::vector<WoodBeam*>&)` | Returns `true` when vehicle reaches goal |
| `bool isDefeat(const Vehicle&)` | Returns `true` when vehicle has fallen |

---

## 9. Button

**File**: [graphics/include/Button.h](graphics/include/Button.h) · [graphics/src/Button.cpp](graphics/src/Button.cpp)  
**Kind**: `class`  
**Description**: A clickable SFML UI button with hover effects and a callback.

### Static Constants

| Name | Value | Description |
|------|-------|-------------|
| `PADDING_X` | `15.0f` | Horizontal text padding |
| `PADDING_Y` | `10.0f` | Vertical text padding |

### Properties

| Name | Type | Description |
|------|------|-------------|
| `shape` | `sf::RectangleShape` | Button background |
| `text` | `sf::Text` | Button label |
| `defaultColor` | `sf::Color` | Normal fill color |
| `hoverColor` | `sf::Color` | Fill color on hover |
| `outlineColor` | `sf::Color` | Border color |
| `isHovered` | `bool` | Current hover state |
| `onClick` | `std::function<void()>` | Click callback |

### Constructor

```cpp
Button(const sf::Font& font, const std::string& label,
       sf::Vector2f position,
       sf::Color defaultColor = sf::Color(70, 130, 180),
       sf::Color hoverColor   = sf::Color(23, 61, 99))
```

### Methods

| Signature | Description |
|-----------|-------------|
| `void setCallback(std::function<void()>)` | Assign click handler |
| `void setText(const std::string&)` | Change label and resize button |
| `void setTextKeepSize(const std::string&)` | Change label without resizing |
| `void update(const sf::Vector2i& mousePos)` | Update hover state each frame |
| `void handleClick(const sf::Vector2i& mousePos)` | Fire callback if clicked |
| `void draw(sf::RenderWindow&)` | Render the button |
| `sf::FloatRect getBounds()` | Return bounding rectangle |
| `float getWidth()` | Return button width |

---

## 10. Car

**File**: [graphics/include/Car.h](graphics/include/Car.h) · [graphics/src/Car.cpp](graphics/src/Car.cpp)  
**Kind**: `class`  
**Description**: A pixel-art car sprite assembled from colored `sf::RectangleShape` blocks.

### Static Constants

| Name | Description |
|------|-------------|
| `P = 8.0f` | Pixel block size |
| `DARK_RED`, `MID_RED`, `LIGHT_RED`, `ORANGE_ACC` | Car body colors |
| `WINDOW_BLU`, `HEADLIGHT` | Window and headlight colors |
| `WHEEL_DARK`, `WHEEL_RIM`, `BLACK` | Wheel colors |

### Properties

| Name | Type | Description |
|------|------|-------------|
| `blocks` | `std::vector<sf::RectangleShape>` | All pixel blocks composing the sprite |

### Constructor

```cpp
Car(sf::Vector2f pos)  // bottom-left anchor position
```

### Methods

| Signature | Description |
|-----------|-------------|
| `void move(const sf::Vector2f& delta)` | Translate all blocks by delta |
| `void move(float dx)` | Translate all blocks horizontally |
| `void draw(sf::RenderWindow&)` | Render the car |
| `float getWidth()` | Returns `96.0f` |
| `float getHeight()` | Returns `64.0f` |

### Private Methods

| Signature | Description |
|-----------|-------------|
| `void addBlock(sf::Vector2f base, int col, int row, int w, int h, sf::Color)` | Append a pixel block at grid position |

---

## 11. HUD

**File**: [graphics/include/HUD.h](graphics/include/HUD.h) · [graphics/src/HUD.cpp](graphics/src/HUD.cpp)  
**Kind**: `class`  
**Description**: Head-up display. Renders the toolbar (play/pause, reset, hamburger menu, wood tool) and a level-selection overlay menu.

### Properties

| Name | Type | Description |
|------|------|-------------|
| `hudLevel` | `sf::Text` | Current level label |
| `hudScore` | `sf::Text` | Score display |
| `hudBudget` | `sf::Text` | Remaining budget display |
| `playPauseBg` | `sf::RectangleShape` | Play/pause button background |
| `playIcon` | `sf::ConvexShape` | Triangle play icon |
| `pauseBar1/2` | `sf::RectangleShape` | Pause bars |
| `resetBg` | `sf::RectangleShape` | Reset button background |
| `resetTexture` | `sf::Texture` | Reset icon texture |
| `resetSprite` | `std::unique_ptr<sf::Sprite>` | Reset icon |
| `hamburgerBg` | `sf::RectangleShape` | Hamburger menu background |
| `hBars` | `std::array<sf::RectangleShape, 3>` | Three hamburger bars |
| `woodBg` | `sf::RectangleShape` | Wood tool button background |
| `woodTexture` | `sf::Texture` | Wood tool icon texture |
| `woodSprite` | `std::unique_ptr<sf::Sprite>` | Wood tool icon |
| `woodModeActive` | `bool` | Whether wood placement mode is active |
| `menuOverlay` | `sf::RectangleShape` | Semi-transparent full-screen overlay |
| `menuPanel` | `sf::RectangleShape` | Menu popup panel |
| `level1Btn/2Btn/3Btn` | `sf::RectangleShape` | Level selection buttons |
| `level1Text/2Text/3Text` | `sf::Text` | Level button labels |
| `menuOpen` | `bool` | Whether the level menu is visible |

### Constructor

```cpp
HUD(const sf::Font& font, int level, int score, int budget)
```

### Methods

| Signature | Description |
|-----------|-------------|
| `void update(int score, int budget)` | Refresh displayed values |
| `std::string handleClick(sf::Vector2f pos)` | Handle click; returns action string (e.g. `"play_pause"`, `"reset"`, `"level1"`) |
| `void handleKeyEscape()` | Close menu if open |
| `void draw(sf::RenderWindow&, bool simRunning)` | Render the full HUD |
| `bool isMenuOpen()` | Returns menu visibility state |
| `bool isWoodModeActive()` | Returns wood placement mode state |
| `bool isPointInHUD(sf::Vector2f pos)` | Returns `true` if point overlaps a HUD element |
| `void clearToolSelection()` | Deactivates wood mode |

---

## 12. Scene

**File**: [graphics/include/Scene.h](graphics/include/Scene.h) · [graphics/src/Scene.cpp](graphics/src/Scene.cpp)  
**Kind**: `class`  
**Description**: The main game scene. Owns all visual and physics objects, handles user interaction for beam placement, and drives the simulation loop.

### Key Properties

| Name | Type | Description |
|------|------|-------------|
| `bridge` | `Bridge` | Physics simulation instance |
| `carVehicle` | `Vehicle` | Car physics object |
| `truckVehicle` | `Vehicle` | Truck physics object |
| `car` | `Car` | Car visual sprite |
| `woodSegments` | `std::vector<WoodSegment>` | All placed beams (visual + physics) |
| `maxWoodLength` | `float` | Maximum beam length constraint |
| `woodDragActive` | `bool` | Whether user is placing a beam |
| `currentStartNode` | `Node*` | Selected starting node for new beam |
| `extraFixedNodes` | `std::vector<Node*>` | Extra anchor nodes beyond the two defaults |
| `flagSprite` | `std::unique_ptr<sf::Sprite>` | Goal flag sprite |

### Constructor

```cpp
Scene(bool enableCarVehicle = true, bool enableTruckVehicle = false)
```

### Public Methods

| Signature | Description |
|-----------|-------------|
| `Node* addFixedSupportNode(const sf::Vector2f&)` | Add an extra immovable anchor node |
| `void setPreviewVehicleLayout(const sf::Vector2f& carPos, const sf::Vector2f& truckPos)` | Position vehicles for preview |
| `bool startWoodSegment(const sf::Vector2f& start, float maxLength)` | Begin beam placement from nearest node |
| `void updateWoodSegmentPreview(const sf::Vector2f& end)` | Update the beam ghost preview |
| `int commitWoodSegment(int& budget, float costPerPixel)` | Finalize beam and deduct cost from budget |
| `void cancelWoodSegmentPreview()` | Abort current beam placement |
| `bool isBuildingWood()` | Returns `true` while placing a beam |
| `void clearWoodSegments()` | Remove all user-placed beams and non-anchor nodes |
| `Bridge& getBridge()` | Direct access to the bridge |
| `Car& getCar()` | Direct access to the car sprite |
| `Node* findNode(const sf::Vector2f& pos, float threshold = 25.0f)` | Find the nearest node within threshold |
| `void resetSimulation()` | Reset vehicles to start positions |
| `void simulateStep()` | Advance physics by one frame |
| `void draw(sf::RenderWindow&, bool simRunning)` | Render the full scene |

### Private Methods

| Signature | Description |
|-----------|-------------|
| `void createTruck()` | Load and initialize truck sprite |
| `void createGrass()` | Build terrain shape |
| `void createSlopeLeft/Right()` | Build slope shapes |
| `void createDarkGrayBridgeLeft/Right()` | Build bridge anchor shapes |
| `void createRiver()` | Build river shape |
| `void createFlag()` | Load goal flag texture |
| `void createFixedNodes()` | Register the two default anchor nodes |
| `void rebuildExtraFixedNodes()` | Re-register extra anchor nodes after reset |
| `std::vector<WoodBeam*> getRoadBeams()` | Return road beams sorted by X position |
| `void setCarPosition(const sf::Vector2f&)` | Reposition car sprite |
| `void setTruckPosition(const sf::Vector2f&)` | Reposition truck sprite |

---

## 13. Level (graphics)

**File**: [graphics/include/Level.h](graphics/include/Level.h)  
**Kind**: `abstract class`  
**Description**: Base class for all game levels. Each level implements its own `run()` loop with a configured `Scene` and vehicle setup.

### Virtual Methods

| Signature | Description |
|-----------|-------------|
| `virtual ~Level() = default` | Virtual destructor |
| `virtual void run(sf::RenderWindow&, HUD&, int& score, int& budget, bool& simRunning, int& currentLevel) = 0` | Main level event/render loop |

---

## 14. Level1

**File**: [graphics/include/Level.h](graphics/include/Level.h) · [graphics/src/Level.cpp](graphics/src/Level.cpp)  
**Kind**: `class` (inherits `Level`)  
**Description**: First level — a car must cross a simple unsupported span.

| Parameter | Value |
|-----------|-------|
| Vehicle | Car only |
| Car weight | 780 N |
| Car speed | 1.2 px/s |
| Budget | 1000 |
| Extra supports | None |

### Methods

| Signature | Description |
|-----------|-------------|
| `void run(sf::RenderWindow&, HUD&, int& score, int& budget, bool& simRunning, int& currentLevel) override` | Level 1 main loop |

---

## 15. Level2

**File**: [graphics/include/Level.h](graphics/include/Level.h) · [graphics/src/Level2.cpp](graphics/src/Level2.cpp)  
**Kind**: `class` (inherits `Level`)  
**Description**: Second level — a heavier truck must cross with two mid-span support pillars available.

| Parameter | Value |
|-----------|-------|
| Vehicle | Truck only |
| Truck weight | 900 N |
| Truck speed | 0.6 px/s |
| Budget | 1200 |
| Extra supports | (420, 465) · (850, 465) |

### Methods

| Signature | Description |
|-----------|-------------|
| `void run(sf::RenderWindow&, HUD&, int& score, int& budget, bool& simRunning, int& currentLevel) override` | Level 2 main loop |

---

## 16. Level3

**File**: [graphics/include/Level.h](graphics/include/Level.h) · [graphics/src/Level3.cpp](graphics/src/Level3.cpp)  
**Kind**: `class` (inherits `Level`)  
**Description**: Third level — both the car and the truck must cross with four support pillars available.

| Parameter | Value |
|-----------|-------|
| Vehicles | Car + Truck |
| Budget | 1500 |
| Extra supports | (420, 465) · (850, 465) · (540, 500) · (755, 500) |

### Methods

| Signature | Description |
|-----------|-------------|
| `void run(sf::RenderWindow&, HUD&, int& score, int& budget, bool& simRunning, int& currentLevel) override` | Level 3 main loop |

---

## Inheritance Diagram

```
Element  (abstract)
└── WoodBeam

Level  (abstract, graphics)
├── Level1
├── Level2
└── Level3
```

## Class Dependency Overview

```
main
└── Level1 / Level2 / Level3
    ├── Scene
    │   ├── Bridge
    │   │   ├── Node
    │   │   └── WoodBeam  ──► Element
    │   ├── Vehicle
    │   └── Car
    └── HUD
        └── Button
```
