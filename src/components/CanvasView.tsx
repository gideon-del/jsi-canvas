import React, { forwardRef } from 'react';
import { StyleSheet } from 'react-native';
import type { ViewStyle } from 'react-native';
import NativeCanvasView from '../specs/CanvasViewNativeComponent';

interface CanvasViewProps {
  style?: ViewStyle;
}

export const CanvasView = forwardRef(({ style }: CanvasViewProps, ref) => {
  return <NativeCanvasView style={[styles.default, style]} ref={ref} />;
});

const styles = StyleSheet.create({
  default: {
    flex: 1,
  },
});
