import React, { useRef } from 'react';
import { StyleSheet, View, StatusBar, Button } from 'react-native';
import { CanvasView } from './src/components/CanvasView';
import { Commands } from './src/specs/CanvasViewNativeComponent';
import { SafeAreaProvider } from 'react-native-safe-area-context';

function App() {
  return (
    <SafeAreaProvider>
      <AppContent />
    </SafeAreaProvider>
  );
}
function AppContent(): React.JSX.Element {
  const canvasRef = useRef<React.ElementRef<typeof View> | null>(null);
  const createScene = (count: number) => {
    if (!canvasRef.current) return;
    Commands.createTestScene(canvasRef.current, count);
  };
  return (
    <>
      <StatusBar barStyle="dark-content" />
      <View style={[styles.container]}>
        <CanvasView ref={canvasRef} style={styles.canvas} />

        <View style={styles.controls}>
          <Button title="100 Nodes" onPress={() => createScene(100)} />
          <Button title="500 Nodes" onPress={() => createScene(500)} />
          <Button title="1000 Nodes" onPress={() => createScene(1000)} />
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
  },
});

export default App;
