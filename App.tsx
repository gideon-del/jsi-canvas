import React, { useState } from 'react';
import { StyleSheet, Text, View, StatusBar, Alert, Button } from 'react-native';
import { CanvasView } from './src/components/CanvasView';
import {
  SafeAreaProvider,
  useSafeAreaInsets,
} from 'react-native-safe-area-context';

function App() {
  return (
    <SafeAreaProvider>
      <AppContent />
    </SafeAreaProvider>
  );
}
function AppContent(): React.JSX.Element {
  const { top, bottom } = useSafeAreaInsets();
  const [freezing, setFreezing] = useState(false);
  const freezeJS = () => {
    setFreezing(true);
    Alert.alert(
      'JS thread test',
      'About to freeze JS for 5 seconds. Try panning the canvas',
      [
        {
          text: 'OK',
          onPress: () => {
            const start = Date.now();

            while (Date.now() - start < 5000) {}

            setFreezing(false);
            Alert.alert('Done', 'JS thread unfreeze.');
          },
        },
      ],
    );
  };
  return (
    <>
      <StatusBar barStyle="dark-content" />
      <View
        style={[
          styles.container,
          { paddingTop: top + 20, paddingBottom: bottom + 20 },
        ]}
      >
        <View style={styles.header}>
          <Text style={styles.title}>Canvas MVP - Sprint 1, Day 4</Text>
          <Text style={styles.subtitle}>Pan Gesture test</Text>
        </View>

        <CanvasView style={styles.canvas} />

        <View style={styles.footer}>
          <Button
            title={freezing ? 'JS Freezes' : 'Test: Freeze JS Thread'}
            onPress={freezeJS}
            disabled={freezing}
          />
          <Text style={styles.instruction}>Testing</Text>
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
  header: {
    padding: 16,
    backgroundColor: '#fff',
    borderBottomWidth: 1,
    borderBottomColor: '#e0e0e0',
  },
  title: {
    fontSize: 18,
    fontWeight: 'bold',
    color: '#333',
  },
  subtitle: {
    fontSize: 14,
    color: '#666',
    marginTop: 4,
  },
  canvas: {
    flex: 1,
    margin: 16,
    backgroundColor: '#fff',
    borderRadius: 8,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 3,
  },
  footer: {
    padding: 16,
    backgroundColor: '#fff',
    borderTopWidth: 1,
    borderTopColor: '#e0e0e0',
  },
  instruction: {
    fontSize: 14,
    color: '#666',
    textAlign: 'center',
  },
});

export default App;
