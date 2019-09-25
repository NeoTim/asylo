/*
 *
 * Copyright 2017 Asylo authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef ASYLO_IDENTITY_SGX_CODE_IDENTITY_UTIL_H_
#define ASYLO_IDENTITY_SGX_CODE_IDENTITY_UTIL_H_

#include <string>

#include "asylo/identity/identity.pb.h"
#include "asylo/identity/sgx/code_identity.pb.h"
#include "asylo/identity/sgx/code_identity_constants.h"
#include "asylo/identity/sgx/identity_key_management_structs.h"
#include "asylo/identity/sgx/sgx_identity.pb.h"
#include "asylo/util/status.h"
#include "asylo/util/statusor.h"

namespace asylo {
namespace sgx {

// A "legacy" identity, match spec, or expectation is one that does not contain
// any SgxMachineConfiguration-related fields. When the |is_legacy| parameter on
// any of these methods is set to true, all checks on SgxMachineConfiguration-
// related fields are skipped. The rationale behind introducing an explicit
// parameter to control this rather than simply checking for the presence of
// the |machine_configuration| field within a match spec is that it *should* be
// invalid for this field to be missing on non-legacy match specs.

// Matches |identity| to given |expectation|. Returns true if there is no error
// encountered, and if the match is successful. Else returns false and populates
// |explanation| with an explanation of why the match failed.
StatusOr<bool> MatchIdentityToExpectation(
    const SgxIdentity &identity, const SgxIdentityExpectation &expectation,
    std::string *explanation, bool is_legacy = false);

// Sets |expectation| based on |identity| and |match_spec|. Returns true on
// success (no error), else returns false.
Status SetExpectation(const CodeIdentityMatchSpec &match_spec,
                      const CodeIdentity &identity,
                      CodeIdentityExpectation *expectation);
Status SetExpectation(const SgxIdentityMatchSpec &match_spec,
                      const SgxIdentity &identity,
                      SgxIdentityExpectation *expectation);

// Checks if a signer-assigned identity is valid.
bool IsValidSignerAssignedIdentity(const SignerAssignedIdentity &identity);

// Checks if an enclave identity is valid.
bool IsValidSgxIdentity(const SgxIdentity &identity);

// Checks if a match specification is valid.
bool IsValidMatchSpec(const SgxIdentityMatchSpec &match_spec,
                      bool is_legacy = false);

// Checks if an identity expectation is valid.
bool IsValidExpectation(const CodeIdentityExpectation &expectation);
bool IsValidExpectation(const SgxIdentityExpectation &expectation,
                        bool is_legacy = false);

// Parses SgxIdentity from |report| and places the result in |identity|.
// Does not verify |report|.
Status ParseIdentityFromHardwareReport(const Report &report,
                                       SgxIdentity *identity);

// Sets |spec| to the default SGX match spec, which requires a match on
// MRSIGNER, MISCSELECT, and all ATTRIBUTES that do not fall into the default
// "do not care" set.
Status SetDefaultMatchSpec(SgxIdentityMatchSpec *spec);

// Sets |spec| to the strictest SGX match spec, which requires a match on
// MRENCLAVE, MRSIGNER, MISCSELECT, and all ATTRIBUTES bits.
void SetStrictMatchSpec(CodeIdentityMatchSpec *spec);
void SetStrictMatchSpec(SgxIdentityMatchSpec *spec);

// Sets |identity| to the current enclave's identity.
void SetSelfSgxIdentity(SgxIdentity *identity);

// Sets |expectation| to default expectation, which is defined as the pair
// <self identity, default match spec>.
Status SetDefaultSelfSgxIdentityExpectation(
    SgxIdentityExpectation *expectation);

// Sets |expectation| to the strictest self identity expectation, which is
// defined as the pair <self identity, strict match spec>.
Status SetStrictSelfSgxIdentityExpectation(SgxIdentityExpectation *expectation);

// Parses an SGX identity from an EnclaveIdentity proto.
Status ParseSgxIdentity(const EnclaveIdentity &generic_identity,
                        SgxIdentity *sgx_identity);

// Parses SGX match spec |sgx_match_spec| from a string (which is how it is
// stored in the EnclaveIdentityExpectation proto).
Status ParseSgxMatchSpec(const std::string &generic_match_spec,
                         SgxIdentityMatchSpec *sgx_match_spec,
                         bool is_legacy = false);

// Parses SGX code identity expectation |sgx_expectation| from
// |generic_expectation|.
Status ParseSgxExpectation(
    const EnclaveIdentityExpectation &generic_expectation,
    SgxIdentityExpectation *sgx_expectation, bool is_legacy = false);

// Serializes SGX code identity |sgx_identity| into the identity field of
// |generic_identity|. Sets the description field of |generic_identity| to
// indicate the identity type CODE_IDENTITY and the authority type "SGX".
Status SerializeSgxIdentity(const CodeIdentity &sgx_identity,
                            EnclaveIdentity *generic_identity);
Status SerializeSgxIdentity(const SgxIdentity &sgx_identity,
                            EnclaveIdentity *generic_identity);

// Serializes SGX match spec to a string that is suitable for use in a
// EnclaveIdentityExpectation proto.
Status SerializeSgxMatchSpec(const CodeIdentityMatchSpec &sgx_match_spec,
                             std::string *generic_match_spec);
Status SerializeSgxMatchSpec(const SgxIdentityMatchSpec &sgx_match_spec,
                             std::string *generic_match_spec);

// Serializes reference_identity and match_spec portions of |sgx_expectation|
// into the appropriate fields of |generic_expectation|. Sets the description
// field of |generic_expectation| to indicate the identity type CODE_IDENTITY
// and the authority type "SGX".
Status SerializeSgxExpectation(const CodeIdentityExpectation &sgx_expectation,
                               EnclaveIdentityExpectation *generic_expectation);
Status SerializeSgxExpectation(const SgxIdentityExpectation &sgx_expectation,
                               EnclaveIdentityExpectation *generic_expectation);

// Sets |tinfo| to match this enclave's identity. Any reports generated using
// this TARGETINFO are targeted at this enclave.
void SetTargetinfoFromSelfIdentity(Targetinfo *tinfo);

// Verifies the hardware report |report|.
Status VerifyHardwareReport(const Report &report);

namespace internal {

// Verifies whether |identity| is compatible with |spec|. This function is
// exposed through this header for testing purposes only.
bool IsIdentityCompatibleWithMatchSpec(const CodeIdentity &identity,
                                       const CodeIdentityMatchSpec &spec);
bool IsIdentityCompatibleWithMatchSpec(const SgxIdentity &identity,
                                       const SgxIdentityMatchSpec &spec);
}  // namespace internal
}  // namespace sgx
}  // namespace asylo

#endif  // ASYLO_IDENTITY_SGX_CODE_IDENTITY_UTIL_H_
