#include "texture2d.h"

#include "../core/logging.h"

#include "../platform/opengl/ext/glcorearb.h"
#include "../platform/opengl/gl_functions.h"

#include "../../../vendor/stb_image/stb_image.h"

#include <stdlib.h>

texture2d* texture2d_create(const char* filepath) {
    texture2d* ret = malloc(sizeof(texture2d));

    ret->filepath = filepath;
    i32 width, height, number_of_channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = nullptr;

    data = stbi_load(filepath, &width, &height, &number_of_channels, 0);

    ASSERT_MSG(data, "Failed to load texture.");

    ret->width = width;
    ret->height = height;

    u32 internal_format = 0, data_format = 0;
    if (number_of_channels == 4) {
        internal_format = GL_RGBA8;
        data_format = GL_RGBA;
    } else if (number_of_channels == 3) {
        internal_format = GL_RGB8;
        data_format = GL_RGB;
    }

    ret->internal_format = internal_format;
    ret->data_format = data_format;

    ASSERT_MSG(internal_format & data_format, "Texture uses a unsupported format.");

    glCreateTextures(GL_TEXTURE_2D, 1, &ret->id);
    glTextureStorage2D(ret->id, 1, internal_format, ret->width, ret->height);

    glTextureParameteri(ret->id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(ret->id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(ret->id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(ret->id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureSubImage2D(ret->id, 0, 0, 0, ret->width, ret->height, data_format, GL_UNSIGNED_BYTE, data);

    LOG_INFO("Loaded texture file at location '%s'", filepath);

    return ret;
}

texture2d* texture2d_create_by_size(u32 width, u32 height) {
    texture2d* ret = malloc(sizeof(texture2d));

    ret->width = width;
    ret->height = height;

    ret->internal_format = GL_RGBA8;
    ret->data_format = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &ret->id);
    glTextureStorage2D(ret->id, 1, ret->internal_format, ret->width, ret->height);

    glTextureParameteri(ret->id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(ret->id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(ret->id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(ret->id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return ret;
}

void texture2d_bind(texture2d* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void texture2d_bind_unit(texture2d* texture, u32 slot) {
    glBindTextureUnit(slot, texture->id);
}

void texture2d_unbind(texture2d* texture) {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture2d_delete(texture2d* texture) {
    glDeleteTextures(1, &texture->id);
    free(texture);
}
