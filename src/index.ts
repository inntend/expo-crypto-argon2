import { requireNativeModule } from 'expo-modules-core';

/** Mirrors hash-wasm's IDataType. Strings are UTF-8 encoded before hashing. */
export type IDataType = string | Uint8Array;

export interface Argon2Params {
  password: IDataType;
  salt: IDataType;
  /** Optional secret key for keyed hashing. */
  secret?: IDataType;
  iterations: number;
  parallelism: number;
  /** Memory in KiB — e.g. 65536 = 64 MiB. */
  memorySize: number;
  hashLength: number;
  /** Output format. Defaults to 'hex'. */
  outputType?: 'hex' | 'binary' | 'encoded';
}

/** Mirrors hash-wasm's conditional return type. */
type Argon2ReturnType<T extends Argon2Params> =
  T extends { outputType: 'binary' } ? Uint8Array : string;

const ExpoCryptoArgon2 = requireNativeModule('ExpoCryptoArgon2');

export async function argon2id<T extends Argon2Params>(
  params: T,
): Promise<Argon2ReturnType<T>> {
  if (params.secret !== undefined) {
    // Requires argon2_ctx (lower-level API) — not yet implemented
    throw new Error('expo-crypto-argon2: secret is not yet supported');
  }

  const outputType = params.outputType ?? 'hex';

  const result: string = await ExpoCryptoArgon2.argon2id({
    password: toHex(params.password),
    salt: toHex(params.salt),
    iterations: params.iterations,
    memory: params.memorySize,  // native record field is still named 'memory'
    parallelism: params.parallelism,
    hashLength: params.hashLength,
    outputType,
  });

  // Native returns a hex string for 'hex'/'binary', PHC string for 'encoded'
  if (outputType === 'binary') {
    return hexToUint8Array(result) as Argon2ReturnType<T>;
  }
  return result as Argon2ReturnType<T>;
}

function toHex(data: IDataType): string {
  if (typeof data === 'string') {
    return uint8ArrayToHex(new TextEncoder().encode(data));
  }
  return uint8ArrayToHex(data);
}

function uint8ArrayToHex(arr: Uint8Array): string {
  return Array.from(arr, b => b.toString(16).padStart(2, '0')).join('');
}

function hexToUint8Array(hex: string): Uint8Array {
  const bytes = new Uint8Array(hex.length >> 1);
  for (let i = 0; i < bytes.length; i++) {
    bytes[i] = parseInt(hex.slice(i * 2, i * 2 + 2), 16);
  }
  return bytes;
}
