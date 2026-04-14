// Minimal ambient declaration so TypeScript resolves expo-modules-core without
// it being installed in this package's own node_modules.
// The real implementation is provided at runtime by the consuming app.
declare module 'expo-modules-core' {
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  export function requireNativeModule(name: string): any;
}
