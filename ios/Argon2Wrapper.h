#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * Thin ObjC++ wrapper around argon2id_hash_raw / argon2id_hash_encoded.
 * Called from Swift via the auto-generated module map (same pod, no explicit import needed).
 *
 * outputType:
 *   @"binary"  — runs argon2id_hash_raw,     returns lowercase hex of the raw hash bytes
 *   @"encoded" — runs argon2id_hash_encoded,  returns the PHC string ($argon2id$v=19$...)
 */
@interface Argon2Wrapper : NSObject

+ (nullable NSString *)argon2idWithPassword:(NSString *)passwordHex
                                   salt:(NSString *)saltHex
                             iterations:(NSInteger)iterations
                                 memory:(NSInteger)memory
                            parallelism:(NSInteger)parallelism
                             hashLength:(NSInteger)hashLength
                             outputType:(NSString *)outputType
                                  error:(NSError *_Nullable *_Nullable)error;

@end

NS_ASSUME_NONNULL_END
