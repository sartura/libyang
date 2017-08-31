/**
 * @file extensions_config.h
 * @author Radek Krejci <rkrejci@cesnet.cz>
 * @brief libyang support for YANG extension implementations - internal configuration values.
 *
 * Copyright (c) 2017 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef LY_EXTENSIONS_CONFIG_H_
#define LY_EXTENSIONS_CONFIG_H_

#define LYEXT_PLUGINS_DIR "/usr/local/lib/libyang" /**< directory with YANG extension plugins */

#if defined __linux__ || defined __unix__
#  define LYEXT_PLUGIN_SUFFIX ".so"
#  define LYEXT_PLUGIN_SUFFIX_LEN 3
#elif defined __APPLE__
#  define LYEXT_PLUGIN_SUFFIX ".dylib"
#  define LYEXT_PLUGIN_SUFFIX_LEN 6
#endif

#endif /* LY_EXTENSIONS_CONFIG_H_ */
