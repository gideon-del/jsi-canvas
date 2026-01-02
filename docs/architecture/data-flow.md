# Data Flow Documentation

## Flow 1: User Pans Canvas

**Scenario:** User drags finger across canvas

**Step-by-step flow:**

1. **User touches screen**

   - iOS: `touchesBegan` called
   - Android: `onTouchEvent(ACTION_DOWN)` called
   - Thread: UI thread (main thread)

2. **Gesture recognizer activates**

   - iOS: `UIPanGestureRecognizer` state = `.began`
   - Android: Track `lastTouchX`, `lastTouchY`

3. **User moves finger**

   - iOS: `UIPanGestureRecognizer` state = `.changed`
   - Android: `onTouchEvent(ACTION_MOVE)`
   - Happens 60-120 times per second!

4. **Calculate delta**

   - iOS: `translation - lastTranslation`
   - Android: `event.x - lastTouchX`

5. **Update camera state (C++)**

```cpp
   camera.offsetX += deltaX / camera.zoom;
   camera.offsetY += deltaY / camera.zoom;
```

- Thread: Still UI thread
- No JS involved!

6. **Trigger redraw**

   - iOS: `[canvasLayer setNeedsDisplay]`
   - Android: `invalidate()`

7. **Draw method called by OS**

   - iOS: `drawRect:` called
   - Android: `onDraw()` called
   - Thread: UI thread

8. **Query visible nodes**

```cpp
   Rect viewport = screenToWorld(viewBounds);
   vector<Node*> visible = sceneGraph.queryVisible(viewport);
```

9. **Transform and render each node**

```cpp
   for (Node* node : visible) {
       Rect screenBounds = worldToScreen(node->bounds);
       drawRect(screenBounds, node->fillColor);
   }
```

10. **Present to screen**
    - OS composites and displays
    - Next frame starts

**Total time: ~10-16ms (60fps)**

**JS involvement: ZERO** ← This is critical!

---

## Flow 2: JS Creates a Rectangle

**Scenario:** User taps "Create Rectangle" button in React

**Step-by-step flow:**

1. **User taps button**

   - React `onPress` handler called
   - Thread: JS thread

2. **Button handler calls JSI command**

```typescript
const id = canvasDocument.createRectangle(100, 100, 200, 150);
```

- Thread: Switches from JS thread to...

3. **JSI bridge executes (Sprint 3)**

   - Marshals arguments
   - Thread: Calls C++ on native thread

4. **C++ command executes**

```cpp
   std::string CanvasDocument::createRectangle(
       double x, double y, double w, double h
   ) {
       auto node = std::make_unique<Node>(
           generateId(), NodeType::RECTANGLE, x, y, w, h
       );
       std::string id = node->id;
       sceneGraph_->addNode(std::move(node));

       // Trigger redraw
       notifyNeedsRedraw();

       return id;
   }
```

- Thread: C++ (not JS!)

5. **Node added to scene graph**

   - Stored in `unordered_map<string, unique_ptr<Node>>`
   - Added to spatial index
   - Added to sorted list

6. **Notify native view to redraw**

```objc
   // iOS
   dispatch_async(dispatch_get_main_queue(), ^{
       [self->canvasView setNeedsDisplay];
   });
```

```kotlin
   // Android
   runOnUiThread {
       canvasView.invalidate()
   }
```

7. **Native draw method called**

   - Same as Flow 1 steps 7-10
   - New rectangle appears!

8. **Return ID to JS**

```typescript
   const id = canvasDocument.createRectangle(...);
   console.log('Created:', id); // "node_123456"
```

**Total time: ~1-5ms**

**What if creation fails?**

- C++ can throw exception
- JSI catches and converts to JS Error
- JS can handle with try/catch

---

## Flow 3: User Selects a Node

**Scenario:** User taps on a shape

**Step-by-step flow:**

1. **User taps screen**

   - iOS: `touchesEnded` called
   - Android: `onTouchEvent(ACTION_UP)`
   - Thread: UI thread

2. **Tap recognizer (distinct from pan)**

   - iOS: `UITapGestureRecognizer`
   - Android: Check if not dragging (distance < threshold)

3. **Get tap location**

```objc
   CGPoint tapLocation = [gesture locationInView:self];
   float screenX = tapLocation.x;
   float screenY = tapLocation.y;
```

4. **Convert to world coordinates**

```cpp
   float worldX = camera.screenToWorldX(screenX);
   float worldY = camera.screenToWorldY(screenY);
```

5. **Hit test**

```cpp
   Node* hitNode = sceneGraph.hitTest(worldX, worldY);
```

- Queries spatial index
- Checks candidates from highest to lowest z-index
- Returns first node containing point

6. **Update selection state**

```cpp
   if (hitNode) {
       // Deselect all
       for (Node* node : sceneGraph.getAllNodes()) {
           node->selected = false;
       }
       // Select hit node
       hitNode->selected = true;
   }
```

7. **Trigger redraw**

   - Selected nodes draw with highlight

8. **Emit event to JS (Sprint 4)**

```objc
   // iOS
   if (hitNode) {
       [self emitNodeSelected:@{
           @"nodeId": @(hitNode->id.c_str())
       }];
   }
```

9. **JS receives event**

```typescript
<CanvasView
  onNodeSelected={event => {
    console.log('Selected:', event.nativeEvent.nodeId);
    setSelectedNodeId(event.nativeEvent.nodeId);
  }}
/>
```

10. **JS can update UI**
    - Show property panel
    - Display node info
    - Enable edit/delete buttons

**Total time: ~1-3ms**

---

## Critical Insights from These Flows

**Insight 1: Gestures NEVER touch JS**

- Pan/Pinch → Native → Camera → Render
- JS is not involved
- This is why 60fps is possible!

**Insight 2: Scene graph is source of truth**

- Not React state
- Not native view properties
- C++ SceneGraph owns all nodes

**Insight 3: JS is for commands and events**

- Commands: JS → Native (create, delete, move)
- Events: Native → JS (selected, tapped)
- Not for every frame updates

**Insight 4: Three threads involved**

- JS thread: React, business logic
- Native thread: C++ commands execute here
- UI thread: Gestures, rendering (60fps)
