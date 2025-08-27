#ifndef MOVEIT_UTIL_H
#define MOVEIT_UTIL_H

#include <memory>
#include <openssl/evp.h>
#include <stdexcept>

namespace moveit::util {

inline std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)>
make_evp_md_ctx() {
  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  if (!ctx)
    throw std::bad_alloc();
  return {ctx, &EVP_MD_CTX_free};
}
} // namespace moveit::util

#endif