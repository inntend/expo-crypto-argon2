# expo-crypto-argon2

Argon2id bindings for Expo apps — native (iOS/Android) implementation of the [phc-winner-argon2](https://github.com/P-H-C/phc-winner-argon2) reference implementation via Expo Modules.

> **NOTE:** This repo is a very early experiment and everything in here is fragile and subject to change.

## Key points

- **Argon2id only** — the variant recommended for password hashing and key derivation
- **Native execution** — runs C argon2 directly on iOS (Swift/ObjC++) and Android (Kotlin/JNI), off the JS thread
- **Three output formats** — `hex` string, raw `binary` (Uint8Array), or PHC-encoded string
- **String or binary inputs** — `password` and `salt` accept `string` (UTF-8) or `Uint8Array`
- **Web not supported** — iOS and Android only
- **`secret` parameter not yet implemented** — throws if provided

## Installation

```sh
npx expo install expo-crypto-argon2
```

Requires a [development build](https://docs.expo.dev/develop/development-builds/introduction/) — not compatible with Expo Go.

## Usage

```typescript
import { argon2id } from 'expo-crypto-argon2';

// Hex output (default)
const hash = await argon2id({
  password: 'correct horse battery staple',
  salt: 'unique-per-credential-salt',
  iterations: 3,
  memorySize: 65536,  // KiB — 64 MiB
  parallelism: 1,
  hashLength: 32,
  outputType: 'hex',  // → string
});

// Binary output
const bytes = await argon2id({
  password: new Uint8Array([...]),
  salt: new Uint8Array([...]),
  iterations: 3,
  memorySize: 65536,
  parallelism: 1,
  hashLength: 32,
  outputType: 'binary',  // → Uint8Array
});

// PHC-encoded string ($argon2id$v=19$m=65536,t=3,p=1$...)
const encoded = await argon2id({
  password: 'myPassword',
  salt: 'mySalt',
  iterations: 3,
  memorySize: 65536,
  parallelism: 1,
  hashLength: 32,
  outputType: 'encoded',  // → string
});
```

## API

```typescript
function argon2id(params: Argon2Params): Promise<string | Uint8Array>
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `password` | `string \| Uint8Array` | The password or data to hash |
| `salt` | `string \| Uint8Array` | Salt — should be unique per credential (min 16 bytes recommended) |
| `iterations` | `number` | Number of passes over memory |
| `memorySize` | `number` | Memory cost in KiB (e.g. `65536` = 64 MiB) |
| `parallelism` | `number` | Number of parallel threads |
| `hashLength` | `number` | Output length in bytes |
| `outputType` | `'hex' \| 'binary' \| 'encoded'` | Output format (default: `'hex'`) |

Return type is narrowed by `outputType`: `'binary'` → `Uint8Array`, otherwise `string`.

## Platform support

| Platform | Min version | Backend |
|----------|-------------|---------|
| iOS | 13.4+ | Swift + ObjC++ (background queue) |
| Android | API 23+ | Kotlin + JNI C++ (background executor) |
| Web | — | Not supported |
