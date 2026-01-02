# Component Responsibilities

## CanvasView (Native - iOS/Android)

### ✅ Responsibilities:

- Gesture recognition (pan, pinch, tap)
- 60fps rendering loop
- Query scene graph for visible nodes
- Transform world coords to screen coords
- Draw shapes using native graphics APIs
- Emit events to JS (tap, selection)

### ❌ Does NOT:

- Store node data (that's SceneGraph)
- Create/delete nodes (that's JSI commands)
- Make business logic decisions (that's JS)
- Parse user input (that's JS)

### 🔌 Interfaces:

- **Reads from:** SceneGraph (via C++ pointer), CameraState (via C++ pointer)
- **Writes to:** CameraState (updates offset/zoom from gestures)
- **Emits to:** JS via event emitters (Sprint 4)
- **Called by:** OS (draw loop), Gestures (user input)

### 🧵 Threading:

- **Runs on:** UI/Main thread ONLY
- **Locks needed:** None (single-threaded)
- **Synchronization:** Not needed within native view

---

## SceneGraph (C++)

### ✅ Responsibilities:

- Store all nodes in memory
- Provide O(1) lookup by ID
- Maintain spatial index for fast queries
- Return visible nodes in viewport
- Hit testing (find node at point)
- Keep nodes sorted by z-index
- Manage node lifecycle (add/remove)

### ❌ Does NOT:

- Render anything (that's Native views)
- Handle gestures (that's Native views)
- Communicate with JS (that's JSI layer)
- Make decisions about what to create (that's JS)

### 🔌 Interfaces:

- **Accessed by:** CanvasView (read-only for rendering), CanvasDocument JSI (read/write)
- **Owns:** All Node objects via `unique_ptr`
- **Thread-safe:** NO - must be accessed from same thread

### 🧵 Threading:

- **Accessed from:** UI thread (for rendering), Native thread (for JSI commands)
- **Locks needed:** YES - mutex around all operations (Sprint 3)
- **Why:** Multiple threads can access simultaneously

---

## CameraState (C++)

### ✅ Responsibilities:

- Store camera transform (offsetX, offsetY, zoom)
- Convert world coords ↔ screen coords
- Clamp zoom to valid range (0.1x - 10x)
- Provide transform matrices (if using Metal/OpenGL later)

### ❌ Does NOT:

- Handle gestures (that's Native views)
- Decide when to pan/zoom (that's Native views)
- Store any node data

### 🔌 Interfaces:

- **Read by:** CanvasView (for rendering transforms)
- **Written by:** CanvasView (from gesture updates)
- **Owned by:** CanvasView

### 🧵 Threading:

- **Accessed from:** UI thread ONLY
- **Locks needed:** None (single-threaded access)

---

## CanvasDocument (JSI HostObject - Sprint 3)

### ✅ Responsibilities:

- Expose JS API for creating/modifying nodes
- Validate input from JS
- Call SceneGraph methods
- Return node IDs to JS
- Trigger redraws when scene changes
- Handle errors and convert to JS exceptions

### ❌ Does NOT:

- Render anything
- Handle gestures
- Store nodes (delegates to SceneGraph)
- Know about React lifecycle

### 🔌 Interfaces:

- **Called by:** JS via JSI
- **Calls:** SceneGraph methods
- **Notifies:** Native views to redraw

### 🧵 Threading:

- **Called from:** JS thread
- **Executes on:** Native thread (JSI switches threads)
- **Locks needed:** YES - SceneGraph access must be synchronized

---

## React Components (App.tsx, etc.)

### ✅ Responsibilities:

- UI layout and styling
- User input (buttons, forms)
- Business logic (when to create/delete)
- App state management
- Validation before calling native
- Display node properties
- Handle navigation

### ❌ Does NOT:

- Store scene data (that's SceneGraph)
- Handle gestures on canvas (that's Native)
- Render shapes (that's Native)
- Calculate transforms (that's Camera/Native)

### 🔌 Interfaces:

- **Uses:** CanvasView component (renders native view)
- **Calls:** JSI commands (via CanvasDocument)
- **Receives:** Events from native (via onNodeSelected, etc.)

### 🧵 Threading:

- **Runs on:** JS thread
- **Async:** All native calls are async from JS perspective

---

## Decision Matrix

When implementing a feature, ask:

| Question                     | If YES                   | If NO               |
| ---------------------------- | ------------------------ | ------------------- |
| Does it need 60fps?          | → Native                 | → Could be JS       |
| Does it access node data?    | → C++ SceneGraph         | → Could be anywhere |
| Does it need user input?     | → React or Native        | → Not React         |
| Does it involve gestures?    | → Native ONLY            | → Could be JS       |
| Does it need business logic? | → React                  | → Not React         |
| Is it platform-specific?     | → Native (separate impl) | → C++ or JS         |

---

## Examples

### ❓ Where should "create rectangle on double-tap" go?

**Answer:**

- **Gesture detection:** Native (detect double-tap)
- **Decision to create:** Native emits event → React decides
- **Creation:** React calls JSI command → C++ creates node

**Why split?**

- Native knows about taps (60fps)
- React knows if we're in "create mode"
- C++ manages the data

---

### ❓ Where should "snap to grid" go?

**Answer:**

- **Grid logic:** C++ helper function
- **When to snap:** React (user preference) or Native (always)
- **Execution:** C++ (when creating/moving node)

**Why C++?**

- Math should be fast (C++)
- Needs access to world coordinates (C++)
- Reusable (both platforms)

---

### ❓ Where should "undo/redo" go?

**Answer:**

- **Command pattern:** JS manages undo stack
- **Each command:** Calls JSI to C++
- **Undo:** JS calls inverse JSI command

**Why JS?**

- Undo is application state, not canvas state
- Easier to serialize/persist
- Easier to implement command pattern in JS

---

### ❓ Where should "selection rectangle" (marquee selection) go?

**Answer:**

- **Draw rectangle:** Native (overlay on canvas)
- **Track drag:** Native gesture
- **Find nodes in rect:** C++ SceneGraph query
- **Update selection:** C++ (mark nodes as selected)
- **Notify JS:** Native emits event

**Why this split?**

- Fast dragging needs native
- Spatial query needs SceneGraph
- JS might want to know selection changed
