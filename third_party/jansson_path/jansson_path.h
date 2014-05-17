#ifndef JANSSON_PATH_H
#define JANSSON_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

/* path */
json_t *json_path_get(const json_t *json, const char *path);

#ifdef __cplusplus
}
#endif

#endif
