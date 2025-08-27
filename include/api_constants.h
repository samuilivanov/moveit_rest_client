#ifndef MOVEIT_ENDPOINTS_H
#define MOVEIT_ENDPOINTS_H
#include <string_view>

namespace moveit::api {

constexpr auto authenticate = "/api/v1/token";
constexpr auto get_current_user_details = "/api/v1/users/self";
constexpr auto upload = "/api/v1/folders/";

constexpr auto AUTH_HEADER = "Authorization";
constexpr auto CONTENT_TYPE_HEADER = "Content-Type";
constexpr auto TRANSFER_ENCODING_HEADER = "Transfer-Encoding";

constexpr auto urlencoded_content_type = "application/x-www-form-urlencoded";
constexpr auto chunked = "chunked";

} // namespace moveit::api

#endif