import React from 'react';
import { StyleSheet } from 'react-native';
import type { ViewStyle } from 'react-native';
import NativeCanvasView from '../specs/CanvasViewNativeComponent';

interface CanvasViewProps {
  style?: ViewStyle;
}

export const CanvasView: React.FC<CanvasViewProps> = ({ style }) => {
  return <NativeCanvasView style={[styles.default, style]} />;
};

const styles = StyleSheet.create({
  default: {
    flex: 1,
  },
});
