#include "mime.h"

static bool endsWith(const std::string& text,
                     const std::string& suffix)
{
    if (suffix.size() > text.size())
        return false;

    return text.compare(
        text.size() - suffix.size(),
        suffix.size(),
        suffix
    ) == 0;
}

std::string Mime::getContentType(const std::string& filePath)
{
    if (endsWith(filePath, ".html"))
        return "text/html";

    if (endsWith(filePath, ".css"))
        return "text/css";

    if (endsWith(filePath, ".js"))
        return "application/javascript";

    if (endsWith(filePath, ".png"))
        return "image/png";

    if (endsWith(filePath, ".jpg") ||
        endsWith(filePath, ".jpeg"))
        return "image/jpeg";

    if (endsWith(filePath, ".ico"))
        return "image/x-icon";

    return "application/octet-stream";
}