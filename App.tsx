import React, { useEffect, useRef, useState } from 'react';
import { StyleSheet, View, StatusBar, Button, ScrollView } from 'react-native';
import { CanvasView } from './src/components/CanvasView';
import { SafeAreaProvider } from 'react-native-safe-area-context';
import { useCanvasCamera } from './src/hooks/useCanvasCamera';
import { installSceneGraph, useSceneGraph } from './src/hooks/useSceneGraph';
function App() {
  const [sceneGraphReady, setSceneGraphReady] = useState(false);
  useEffect(() => {
    installSceneGraph();
    setSceneGraphReady(true);
  }, []);
  return (
    <SafeAreaProvider>{sceneGraphReady && <AppContent />}</SafeAreaProvider>
  );
}
function AppContent(): React.JSX.Element {
  const canvasRef = useRef<React.ElementRef<typeof View> | null>(null);
  const canvasCamera = useCanvasCamera();
  const { sceneGraph, addNode, removeNode, canRedo, canUndo } = useSceneGraph();
  const [selectedNodeId, setSelectedNodeId] = useState<string | null>(null);

  const handleAddNode = () => {
    if (!canvasCamera) {
      console.error('[App] canvasCamera not available');
      return;
    }
    try {
      // Generate random position
      const x = Math.random() * 500;
      const y = Math.random() * 500;

      const worldPoint = canvasCamera.screenToWorld(x, y);

      addNode({
        x: worldPoint.x,
        y: worldPoint.y,
        width: 100,
        height: 100,
        fillColor: '#0000FF',
        zIndex: 0,
      });
    } catch (error) {
      console.error('[App] Error adding node:', error);
    }
  };
  const handleListNode = () => {
    const nodes = sceneGraph.getAllNodes();
    for (let node of nodes) {
      console.log(`[App] Node ${node.id} - ${JSON.stringify(node)}`);
    }
    if (nodes.length > 0) {
      setSelectedNodeId(nodes[0].id);
      console.log(`[App] Selected node with id - ${nodes[0].id} `);
    }
  };

  const handleGetNode = () => {
    if (!selectedNodeId) return;
    const node = sceneGraph.getNode(selectedNodeId);
    if (node) {
      console.log(`[App] Node ${node.id} - ${JSON.stringify(node)}`);
    } else {
      console.log(`[App] Could not find Node with id -${selectedNodeId}`);
    }
  };
  const handleRemoveNode = () => {
    if (!selectedNodeId) return;
    removeNode(selectedNodeId);
    setSelectedNodeId(null);
  };
  const handleClearGraph = () => {
    setSelectedNodeId(null);
  };
  return (
    <>
      <StatusBar barStyle="dark-content" />
      <View style={[styles.container]}>
        <CanvasView ref={canvasRef} style={styles.canvas} />

        <View style={{ height: 80 }}>
          <ScrollView horizontal>
            <View style={styles.controls}>
              {selectedNodeId ? (
                <>
                  <Button title="Get node" onPress={handleGetNode} />
                  <Button title="Remove node" onPress={handleRemoveNode} />
                </>
              ) : (
                <>
                  <Button title="Add random node" onPress={handleAddNode} />
                  <Button title="List nodes" onPress={handleListNode} />
                </>
              )}
              <Button title="Clear Graph" onPress={handleClearGraph} />
              {canRedo && <Button title="Redo" onPress={sceneGraph.redo} />}
              <Button title="Redo" onPress={sceneGraph.redo} />
              {canUndo && <Button title="Undo" onPress={sceneGraph.undo} />}
            </View>
          </ScrollView>
        </View>
      </View>
    </>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f5f5f5',
  },

  canvas: {
    flex: 1,

    backgroundColor: '#fff',
    borderRadius: 8,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 3,
  },

  controls: {
    flexDirection: 'row',
    justifyContent: 'space-around',
    padding: 16,
    backgroundColor: '#f0f0f0',
    borderTopWidth: 1,
    borderTopColor: '#ddd',
    gap: 20,
    alignItems: 'center',
  },
});

export default App;
