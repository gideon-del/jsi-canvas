import React from 'react';
import { StyleSheet, Text, View, StatusBar } from 'react-native';
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
          <Text style={styles.title}>Canvas MVP - Sprint 1, Day 1</Text>
          <Text style={styles.subtitle}>iOS Native View Test</Text>
        </View>

        <CanvasView style={styles.canvas} />

        <View style={styles.footer}>
          <Text style={styles.instruction}>
            ✅ If you see a red rectangle above, the native view is working!
          </Text>
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
