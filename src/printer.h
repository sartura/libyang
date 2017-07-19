/**
 * @file printer.h
 * @author Radek Krejci <rkrejci@cesnet.cz>
 * @brief Printers for libyang
 *
 * Copyright (c) 2015 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef LY_PRINTER_H_
#define LY_PRINTER_H_

#include "libyang.h"
#include "tree_schema.h"
#include "tree_internal.h"

typedef enum LYOUT_TYPE {
    LYOUT_FD,          /**< file descriptor */
    LYOUT_STREAM,      /**< FILE stream */
    LYOUT_MEMORY,      /**< memory */
    LYOUT_CALLBACK     /**< print via provided callback */
} LYOUT_TYPE;

struct lyout {
    LYOUT_TYPE type;
    union {
        int fd;
        FILE *f;
        struct {
            char *buf;
            size_t len;
            size_t size;
        } mem;
        struct {
            ssize_t (*f)(void *arg, const void *buf, size_t count);
            void *arg;
        } clb;
    } method;
};

struct ext_substmt_info_s {
    const char *name;
    const char *arg;
    int flags;
#define SUBST_FLAG_YIN 0x1 /**< has YIN element */
#define SUBST_FLAG_ID 0x2  /**< the value is identifier -> no quotes */
};

/* filled in printer.c */
extern struct ext_substmt_info_s ext_substmt_info[];

/**
 * @brief Generic printer, replacement for printf() / write() / etc
 */
int ly_print(struct lyout *out, const char *format, ...);
void ly_print_flush(struct lyout *out);
int ly_write(struct lyout *out, const char *buf, size_t count);
/* module_name_or_prefix: 1 - print module names for foreign if-features, 0 - print import prefixes */
int ly_print_iffeature(struct lyout *out, const struct lys_module *module, struct lys_iffeature *expr, int module_name_or_prefix);

int yang_print_model(struct lyout *out, const struct lys_module *module);
int yin_print_model(struct lyout *out, const struct lys_module *module);
int tree_print_model(struct lyout *out, const struct lys_module *module, int groupings);
int info_print_model(struct lyout *out, const struct lys_module *module, const char *target_node);

int json_print_data(struct lyout *out, const struct lyd_node *root, int options);
int xml_print_data(struct lyout *out, const struct lyd_node *root, int options);
int xml_print_node(struct lyout *out, int level, const struct lyd_node *node, int toplevel, int options);

/**
 * get know if the node is supposed to be printed according to the specified with-default mode
 * return 1 - print, 0 - do not print
 */
int lyd_wd_toprint(const struct lyd_node *node, int options);

/* 0 - same, 1 - different */
int nscmp(const struct lyd_node *node1, const struct lyd_node *node2);

#endif /* LY_PRINTER_H_ */
