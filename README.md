# jsi-canvas

A high-performance, cross-platform canvas engine for React Native built with **JSI**, **C++**, and **native rendering**.

This project explores how far React Native can be pushed by moving core rendering, spatial indexing, and interaction logic into C++, while exposing a minimal, synchronous API to JavaScript via JSI.

---

## Motivation

Most canvas and graphics solutions in React Native rely heavily on the JS thread or bridge-based communication.

`jsi-canvas` takes a different approach:

- Core data structures live in **C++**
- JavaScript communicates via **JSI HostObjects** (no bridge)
- Gestures and rendering are handled **natively**
- JS focuses on orchestration, tooling, and UI

This mirrors how editors, engines, and high-performance native systems are typically built.

---

## Architecture Overview

### Core (C++)

- **SceneGraph**

  - Node storage and ownership
  - Spatial indexing
  - Visibility queries
  - Selection state

- **CameraState**

  - Pan / zoom
  - Screen ↔ world coordinate transforms
  - Shared across JS, JNI, and native views

---

### JSI Layer

- `SceneGraphJSI`
- `CameraJSI`
- Synchronous APIs exposed as global HostObjects
- Shared runtime-backed state (no serialization)

---

### Native Views (Android / iOS)

- Native rendering
- Gesture handling (pan, pinch, hit testing)
- Reads from the same C++ SceneGraph and CameraState

---

## Current Features

- ✅ SceneGraph implemented in C++
- ✅ Camera state shared across JSI + JNI
- ✅ Add and clear nodes via JSI
- ✅ Native pan and pinch zoom
- ✅ Viewport-based visibility queries
- ✅ Android native rendering prototype
- ✅ Cross-layer state consistency (JS ↔ C++ ↔ Native)

---

## Example (JavaScript)

```ts
const graph = global.sceneGraph;
const camera = global.canvasCamera;

const worldPoint = camera.screenToWorld(200, 300);

graph.addNode({
  x: worldPoint.x,
  y: worldPoint.y,
  width: 100,
  height: 100,
  fillColor: '#ff0000',
});
```

---

## Roadmap

- [ ] Full CRUD operations
- [ ] Command system (undo / redo)
- [ ] Event system (selection, updates)
- [ ] iOS renderer parity
- [ ] Toolbar and property panel UI
- [ ] Performance benchmarks

---

## Why this project exists

This is a research-driven project focused on:

- React Native internals
- JSI architecture
- Cross-platform C++ engines
- Native-first interaction models
- Editor-style system design

---

## Status

🚧 **Experimental / Work in progress**

This is not a production-ready library.
It is an exploration of architecture, performance, and native integration.
