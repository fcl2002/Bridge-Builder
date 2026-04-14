# Bridge Builder — Physics Modeling

This document covers the physical simulation layer of the Bridge Builder project: mathematical foundations, implementation decisions, formulas, and the `Vec2` primitive that underpins all of it.

---

## Table of Contents

1. [Overview](#1-overview)
2. [Vec2 — 2D Vector Primitive](#2-vec2--2d-vector-primitive)
3. [Physics Constants & Material Properties](#3-physics-constants--material-properties)
4. [Point-Mass Nodes](#4-point-mass-nodes)
5. [Structural Elements & Hooke's Law](#5-structural-elements--hookes-law)
6. [Axial Strain & Fracture](#6-axial-strain--fracture)
7. [Numerical Integration — Symplectic Euler](#7-numerical-integration--symplectic-euler)
8. [Dynamic Loads — Vehicle System](#8-dynamic-loads--vehicle-system)
9. [Simulation Loop](#9-simulation-loop)
10. [Collapse Criterion](#10-collapse-criterion)
11. [Force Summary](#11-force-summary)
12. [Coordinate System & Sign Convention](#12-coordinate-system--sign-convention)
13. [Assumptions & Limitations](#13-assumptions--limitations)
14. [Bibliography](#14-bibliography)

---

## 1. Overview

The engine models a planar bridge as a **mass-spring system**: structural members are linear springs connecting point-mass joints. At every simulation tick the following pipeline executes:

```
clear forces
    → apply gravity to every free node
    → apply vehicle loads (distributed)
    → apply spring forces from beams (Hooke's law)
    → integrate (Symplectic Euler)
    → check beam fracture
    → check global collapse
```

The simulation runs at a fixed time step `dt = 0.016 s` (60 Hz).

---

## 2. Vec2 — 2D Vector Primitive

All spatial quantities (position, velocity, acceleration, force) are represented as `Vec2`, a two-component floating-point vector defined in [classes/Vec2.h](classes/Vec2.h).

### 2.1 Definition

```cpp
struct Vec2 {
    float x;
    float y;

    Vec2();                        // default: (0, 0)
    Vec2(float x, float y);        // component constructor
};
```

### 2.2 Arithmetic operators

| Expression | Result |
|---|---|
| `u + v` | Component-wise addition: `(u.x + v.x, u.y + v.y)` |
| `u - v` | Component-wise subtraction: `(u.x − v.x, u.y − v.y)` |
| `v * s` | Scalar multiplication: `(v.x·s, v.y·s)` |
| `v / s` | Scalar division (guards against division by zero): `(v.x/s, v.y/s)` |
| `-v` | Negation: `(−v.x, −v.y)` |
| `u += v`, `u -= v`, `v *= s` | In-place variants |

### 2.3 Vector operations

#### Dot product

$$\mathbf{u} \cdot \mathbf{v} = u_x v_x + u_y v_y$$

```cpp
float dot(const Vec2& other) const;
```

Used to project a force onto an axis or to compare directions.

#### Euclidean magnitude (L2 norm)

$$|\mathbf{v}| = \sqrt{v_x^2 + v_y^2}$$

```cpp
float magnitude() const;
```

Used to compute beam lengths and force magnitudes.

#### Squared magnitude

$$|\mathbf{v}|^2 = v_x^2 + v_y^2$$

```cpp
float magnitudeSquared() const;
```

Avoids the `sqrt` when only a length *comparison* is needed — pure performance optimization.

#### Normalization (unit vector)

$$\hat{\mathbf{v}} = \frac{\mathbf{v}}{|\mathbf{v}|}$$

```cpp
Vec2 normalized() const;   // throws if |v| == 0
```

Produces the axial direction of a beam, onto which the scalar spring force is later projected.

#### Zero / reset

```cpp
void zero();   // sets x = y = 0.0f
```

Called at the start of every frame to clear force accumulators.

---

## 3. Point-Mass Nodes

Nodes (`Node`) are the degrees of freedom of the structure. Each node is a **lumped point mass** characterized by:

| State variable | Type | Meaning |
|---|---|---|
| `position` | `Vec2` | Current world-space position (px) |
| `velocity` | `Vec2` | Current velocity (px s⁻¹) |
| `forceAccumulator` | `Vec2` | Sum of forces applied this tick (N) |
| `mass` | `float` | Intrinsic node mass (kg) |
| `accumulatedMass` | `float` | Extra mass contributed by connected beams (kg) |
| `isFix` | `bool` | Whether the node is a fixed support |

Fixed nodes (`isFix = true`) are **boundary conditions**: they accept no forces and never move. All reaction forces are implicitly absorbed by them.

### 3.1 Force accumulation

Every force source calls:

```cpp
void Node::applyForce(const Vec2& force) {
    if (isFix) return;
    forceAccumulator += force;
}
```

This is the standard *force accumulator* pattern: during a tick, forces are summed; integration is deferred until all forces have been collected.

---

## 4. Structural Elements & Hooke's Law

### 4.1 Element model

Each structural member (beam) is a **linear axial spring** — it resists only elongation and compression along its axis, with no bending stiffness. This corresponds to a **truss element** assumption.

The current length of beam connecting nodes A and B is:

$$L = |\mathbf{p}_B - \mathbf{p}_A|$$

The deformation (signed elongation) is:

$$\Delta L = L - L_0$$

where $L_0$ is the **rest length** — the distance between A and B at construction time.

### 4.2 Hooke's Law

The restoring force magnitude follows Hooke's law:

$$F_\text{spring} = k \cdot \Delta L$$

where $k$ = `K_STIFFNESS` = 1 000 N px⁻¹.

The force is vectorized along the beam axis:

$$\hat{\mathbf{d}} = \frac{\mathbf{p}_B - \mathbf{p}_A}{|\mathbf{p}_B - \mathbf{p}_A|}$$

$$\mathbf{F}_\text{spring} = k \cdot \Delta L \cdot \hat{\mathbf{d}}$$

### 4.3 Newton's Third Law — action–reaction pair

By Newton's third law, the spring exerts equal and opposite forces on its two endpoint nodes:

$$\mathbf{F}_A = +\mathbf{F}_\text{spring} \qquad \mathbf{F}_B = -\mathbf{F}_\text{spring}$$

When $\Delta L > 0$ (extension), the beam is in **tension**: it pulls both nodes toward each other.  
When $\Delta L < 0$ (compression), the beam is in **compression**: it pushes both nodes apart.

---

## 5. Axial Strain & Fracture

### 5.1 Axial strain

The normalized axial strain measures deformation relative to rest length:

$$\varepsilon = \frac{L - L_0}{L_0} = \frac{\Delta L}{L_0}$$

- $\varepsilon > 0$: tension (member is elongated)
- $\varepsilon < 0$: compression (member is shortened)
- $\varepsilon = 0$: unstressed

### 5.2 Stress indicator (`colorFactor`)

A dimensionless stress ratio maps the current axial force to the fracture threshold:

$$\eta = \frac{k \cdot |\Delta L|}{F_\text{max}} = \frac{|\mathbf{F}_\text{spring}|}{F_\text{max}}$$

clamped to $[0, 1]$. This value drives the visual colour of each beam (blue → green → red) so the user can read stress distribution at a glance.

### 5.3 Fracture criterion

A beam fractures irreversibly when:

$$k \cdot |\Delta L| \geq F_\text{max}$$

i.e. when $\eta \geq 1$. After fracture, the beam stops applying forces and is marked `isBroken = true`. The model does not distinguish tension fracture from compression buckling — both modes are captured by the same threshold.

---

## 6. Numerical Integration — Symplectic Euler

### 6.1 Why Symplectic Euler?

At each frame the simulation must move every node from its current position to its next one. The simplest way to do this is **forward Euler**: compute acceleration, add it to velocity, add velocity to position. The problem is that in spring systems forward Euler leaks energy — nodes drift further and further from their rest positions until the simulation explodes.

**Symplectic Euler** fixes this with a single change in the order of operations: velocity is updated *before* position, so position already "knows" the corrected velocity. This keeps the total mechanical energy bounded over time without adding any computational cost.

### 6.2 Integration equations

Each tick, for every free node with effective mass $m$ and accumulated force $\mathbf{F}$:

**Step 1 — Acceleration:**

$$\mathbf{a} = \frac{\mathbf{F}}{m}$$

*How fast the node is accelerating right now, given all forces acting on it.*

**Step 2 — Velocity update:**

$$\mathbf{v}_{n+1} = \left(\mathbf{v}_n + \mathbf{a} \cdot \Delta t\right) \cdot \gamma$$

*The velocity changes due to acceleration, then is multiplied by $\gamma = 0.99$ to bleed off a small amount of energy (damping).*

**Step 3 — Position update:**

$$\mathbf{p}_{n+1} = \mathbf{p}_n + \mathbf{v}_{n+1} \cdot \Delta t$$

*Position advances using the velocity that was just updated — not the old one. This is the key step that separates symplectic Euler from forward Euler.*

### 6.3 Damping

The multiplier $\gamma = 0.99$ removes 1 % of the velocity every tick. Over time this makes the bridge settle into a static equilibrium instead of vibrating forever. After one second of simulation (≈ 62 ticks) the speed of any node has dropped to roughly half its initial value:

$$|\mathbf{v}(1\,\text{s})| \approx |\mathbf{v}(0)| \cdot 0.99^{62} \approx 0.54 \cdot |\mathbf{v}(0)|$$

### 6.4 Stability condition

A spring-mass system becomes unstable if the time step is too large relative to how stiff the spring is. The safe upper bound is:

$$\Delta t \leq \frac{2}{\omega_\text{max}} \qquad \text{where} \qquad \omega_\text{max} = \sqrt{\frac{k}{m}}$$

For $k = 1000$ N px⁻¹ and $m = 1$ kg this gives $\Delta t \leq 0.063$ s. The chosen $\Delta t = 0.016$ s is well within that limit.

---

## 7. Dynamic Loads — Vehicle System

### 7.1 Vehicle model

The vehicle is an abstraction for a **moving point load** of constant weight $W$ (units: N). It traverses the road surface at constant horizontal speed $v_\text{veh}$, advancing each tick by:

$$\Delta s = v_\text{veh} \cdot \Delta t$$

### 7.2 Linear load distribution

When the vehicle is at position $s$ within a beam whose endpoints have accumulated road-path offsets $[s_0,\ s_0 + L_0]$, the normalized position within the beam is:

$$t = \frac{s - s_0}{L_0} \in [0, 1]$$

The weight is distributed to the two endpoint nodes via **linear (barycentric) interpolation**:

$$F_A = W \cdot (1 - t) \qquad F_B = W \cdot t$$

This is the 1D equivalent of a simply supported beam with a point load: when $t = 0$ the full load goes to A; when $t = 1$ it goes to B; at midspan it splits equally. Equilibrium is preserved: $F_A + F_B = W$.

### 7.3 Failure propagation

If the beam the vehicle currently occupies is `isBroken`, the vehicle is immediately marked `hasFallen = true` and no further forces are applied. This models the physical scenario of the vehicle falling through a collapsed span.

---

## 8. Simulation Loop

One call to `Bridge::step()` constitutes a single simulation tick of duration $\Delta t = 0.016$ s.

```
Bridge::step(vehicles)
│
├─ clearForces()
│     For every node: forceAccumulator = (0, 0),  accumulatedMass = 0
│
├─ applyGravity()
│     For every free node:
│         F += (0,  m · g)          [g = 9.81 m s⁻²]
│
├─ applyVehicleForces(vehicles)
│     For every vehicle that has not fallen:
│         update vehicle position along road
│         distribute weight W to current beam endpoints (§ 8.2)
│
├─ applyElementForces()
│     For every non-broken beam:
│         compute F_spring = k · ΔL · d̂  (Hooke's law, § 5)
│         nodeA.F += F_spring
│         nodeB.F -= F_spring
│         distribute half beam mass to each node (if ρ > 0)
│
├─ updateNodes()
│     For every free node:
│         a = F / m_eff
│         v = (v + a·Δt) · γ
│         p = p + v·Δt
│         clearForces()
│
├─ checkAllBreaking()
│     For every non-broken beam:
│         if  k·|ΔL| ≥ F_max  →  isBroken = true
│
└─ checkCollapse()
      if all road beams broken  →  hasCollapsed = true
```

---

## 9. Collapse Criterion

The bridge is declared **collapsed** when every beam marked `isRoad = true` has `isBroken = true`. Road beams form the traversable deck; losing all of them means there is no longer a continuous load path for the vehicle, which maps intuitively to a total bridge failure.

Truss-only (non-road) members can break without triggering collapse — they model bracing and secondary structure whose failure degrades but does not immediately destroy the deck.

---

## 10. Force Summary

| Source | Expression | Direction |
|---|---|---|
| Gravity | $\mathbf{F}_g = m \cdot g$ | $+Y$ (downward) |
| Spring (tension) | $\mathbf{F} = k \cdot \Delta L \cdot \hat{\mathbf{d}}$ | Along beam axis (inward) |
| Spring (compression) | $\mathbf{F} = k \cdot \Delta L \cdot \hat{\mathbf{d}}$ | Along beam axis (outward) |
| Vehicle on node A | $F_A = W \cdot (1-t)$ | $+Y$ (downward) |
| Vehicle on node B | $F_B = W \cdot t$ | $+Y$ (downward) |
| Damping | $\mathbf{v} \leftarrow \mathbf{v} \cdot 0.99$ | Opposed to motion |

Total force at node $i$ per tick:

$$\mathbf{F}_i = \mathbf{F}_{g,i} + \sum_j \mathbf{F}_{\text{spring},j}^{(i)} + \mathbf{F}_{\text{vehicle},i}$$

---

## 11. Coordinate System & Sign Convention

- **Origin**: top-left corner of the simulation canvas.
- **X axis**: points right.
- **Y axis**: points *down* (screen convention).
- Gravity therefore acts in the **positive Y** direction.
- A beam that connects a lower-left node to an upper-right node will, when extended, pull the lower-left node up-and-right and the upper-right node down-and-left.

---

## 12. Assumptions & Limitations

| Assumption | Implication |
|---|---|
| **Truss (axial-only) elements** | No bending moments or shear at joints; each beam carries only tension or compression |
| **Pin joints** | Nodes are frictionless hinges; no rotational stiffness |
| **Linear elastic material** | Hooke's law applies up to the fracture limit; no plasticity or nonlinear response |
| **Small deformations** | Geometry updates use current positions, but the rest length is fixed at construction |
| **Lumped mass** | All inertia is concentrated at nodes; beams are massless by default |
| **2D plane problem** | Out-of-plane effects (lateral buckling, torsion) are ignored |
| **Single fracture threshold** | Tension and compression failure are treated identically |
| **No contact / self-collision** | Nodes and beams do not interact geometrically after fracture |
| **Constant vehicle speed** | The vehicle does not accelerate, brake, or respond to deck deformation |

---
## 13. Bibliography

1. Catto, E. (2009). *Numerical Integration*. GDC 2009. [PDF](https://box2d.org/files/ErinCatto_NumericalIntegration_GDC2009.pdf)
2. Catto, E. (2015). *Numerical Methods*. GDC 2015. [PDF](https://box2d.org/files/ErinCatto_NumericalMethods_GDC2015.pdf)

---
