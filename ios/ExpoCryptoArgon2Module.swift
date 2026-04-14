import ExpoModulesCore

// Typed record — Expo deserialises the JS object into this struct automatically.
struct Argon2Params: Record {
    @Field var password: String = ""
    @Field var salt: String = ""
    @Field var iterations: Int = 3
    @Field var memory: Int = 65536
    @Field var parallelism: Int = 1
    @Field var hashLength: Int = 32
    @Field var outputType: String = "binary"   // "binary" | "encoded"
}

public class ExpoCryptoArgon2Module: Module {
    public func definition() -> ModuleDefinition {
        Name("ExpoCryptoArgon2")

        // AsyncFunction with an explicit Promise so we can dispatch to a global
        // concurrent queue — Argon2 is CPU-heavy and must not block the JS thread.
        AsyncFunction("argon2id") { (params: Argon2Params, promise: Promise) in
            DispatchQueue.global(qos: .userInitiated).async {
                // Swift bridges ObjC `NSError **` as a throwing function — use try/catch.
                do {
                    let hex = try Argon2Wrapper.argon2id(
                        withPassword: params.password,
                        salt: params.salt,
                        iterations: params.iterations,
                        memory: params.memory,
                        parallelism: params.parallelism,
                        hashLength: params.hashLength,
                        outputType: params.outputType
                    )
                    promise.resolve(hex)
                } catch {
                    promise.reject(error)
                }
            }
        }
    }
}
