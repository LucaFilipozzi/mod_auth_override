// Copyright (C) 2014 The University of British Columbia

#include "httpd.h"
#include "http_config.h"
#include "http_log.h"

module AP_MODULE_DECLARE_DATA auth_override_module;

typedef struct {
    char* header_key;
} auth_override_config_t;

static const char* auth_override_command_handler(cmd_parms* cmd, void* cfg, const char* arg)
{
    auth_override_config_t* config = (auth_override_config_t*)cfg;
    config->header_key = (char*)apr_pstrdup(cmd->pool, arg);
    return NULL;
}

static int auth_override_auth_checker(request_rec* request)
{
    auth_override_config_t* config = ap_get_module_config(request->per_dir_config, &auth_override_module);
    if (config) {
        const char* header_val = apr_table_get(request->headers_in, config->header_key);
        if (header_val) {
            request->user = (char*)apr_pstrdup(request->pool, header_val);
            return OK;
        } else {
            ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, request, "mod_auth_override: header '%s' not found", config->header_key);
            return HTTP_INTERNAL_SERVER_ERROR;
        }
    }
    return HTTP_UNAUTHORIZED;
}

static void auth_override_create_directory_config(apr_pool_t* pool, char* directory) {
    auth_override_config_t* config = apr_pcalloc(pool, sizeof(auth_override_config_t));
    config->header_key = NULL;
    return config;
}

static const command_rec auth_override_module_commands[] = {
    AP_INIT_TAKE1("AuthOverride", auth_override_command_handler, NULL, ACCESS_CONF, "AuthOverride <header>"),
    {NULL}
};

static void auth_override_register_module_hooks(apr_pool_t* pool)
{
    ap_hook_auth_checker(auth_override_auth_checker, NULL, NULL, APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA auth_override_module = {
    STANDARD20_MODULE_STUFF,
    auth_override_create_directory_config,  // directory config - pointer to create function
    NULL,                                   // directory config - pointer to merge function
    NULL,                                   // virt_host config - pointer to create function
    NULL,                                   // virt_host config - pointer to merge function
    auth_override_module_commands,          // module commands - pointer to array
    auth_override_register_module_hooks     // module hooks - pointer to register function
};

// vim: set ts=4 sw=4 et ai si:
