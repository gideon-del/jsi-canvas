import { StyleSheet, ViewStyle } from 'react-native';
import NativeColorBox from '../specs/ColorBoxNativeComponent';
interface ColorBoxProps {
  color?: string;
  style?: ViewStyle;
}

export const ColorBox = ({ color, style }: ColorBoxProps) => {
  return <NativeColorBox color={color} style={[styles.box, style]} />;
};

const styles = StyleSheet.create({
  box: {
    width: 100,
    height: 100,
  },
});
