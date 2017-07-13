/**
 * @file xpath.h
 * @author Michal Vasko <mvasko@cesnet.cz>
 * @brief YANG XPath evaluation functions header
 *
 * Copyright (c) 2015 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef _XPATH_H
#define _XPATH_H

#include <stdint.h>

#include "tree_schema.h"
#include "tree_data.h"

/*
 * XPath evaluator fully compliant with http://www.w3.org/TR/1999/REC-xpath-19991116/
 * except the following restrictions in the grammar.
 *
 * PARSED GRAMMAR
 *
 * Full axes are not supported, abbreviated forms must be used,
 * variables are not supported, "id()" function is not supported,
 * and processing instruction and comment nodes are not supported,
 * which is also reflected in the grammar. Undefined rules and
 * constants are tokens.
 *
 * Modified full grammar:
 *
 * [1] LocationPath ::= RelativeLocationPath | AbsoluteLocationPath
 * [2] AbsoluteLocationPath ::= '/' RelativeLocationPath? | '//' RelativeLocationPath
 * [3] RelativeLocationPath ::= Step | RelativeLocationPath '/' Step | RelativeLocationPath '//' Step
 * [4] Step ::= '@'? NodeTest Predicate* | '.' | '..'
 * [5] NodeTest ::= NameTest | NodeType '(' ')'
 * [6] Predicate ::= '[' Expr ']'
 * [7] PrimaryExpr ::= '(' Expr ')' | Literal | Number | FunctionCall
 * [8] FunctionCall ::= FunctionName '(' ( Expr ( ',' Expr )* )? ')'
 * [9] PathExpr ::= LocationPath | PrimaryExpr Predicate*
 *                 | PrimaryExpr Predicate* '/' RelativeLocationPath
 *                 | PrimaryExpr Predicate* '//' RelativeLocationPath
 * [10] Expr ::= AndExpr | Expr 'or' AndExpr
 * [11] AndExpr ::= EqualityExpr | AndExpr 'and' EqualityExpr
 * [12] EqualityExpr ::= RelationalExpr | EqualityExpr '=' RelationalExpr
 *                     | EqualityExpr '!=' RelationalExpr
 * [13] RelationalExpr ::= AdditiveExpr
 *                       | RelationalExpr '<' AdditiveExpr
 *                       | RelationalExpr '>' AdditiveExpr
 *                       | RelationalExpr '<=' AdditiveExpr
 *                       | RelationalExpr '>=' AdditiveExpr
 * [14] AdditiveExpr ::= MultiplicativeExpr
 *                     | AdditiveExpr '+' MultiplicativeExpr
 *                     | AdditiveExpr '-' MultiplicativeExpr
 * [15] MultiplicativeExpr ::= UnaryExpr
 *                     | MultiplicativeExpr '*' UnaryExpr
 *                     | MultiplicativeExpr 'div' UnaryExpr
 *                     | MultiplicativeExpr 'mod' UnaryExpr
 * [16] UnaryExpr ::= UnionExpr | '-' UnaryExpr
 * [17] UnionExpr ::= PathExpr | UnionExpr '|' PathExpr
 */

/* expression tokens allocation */
#define LYXP_EXPR_SIZE_START 10
#define LYXP_EXPR_SIZE_STEP 5

/* XPath matches allocation */
#define LYXP_SET_SIZE_START 2
#define LYXP_SET_SIZE_STEP 2

/* building string when casting */
#define LYXP_STRING_CAST_SIZE_START 64
#define LYXP_STRING_CAST_SIZE_STEP 16

/**
 * @brief Tokens that can be in an XPath expression.
 */
enum lyxp_token {
    LYXP_TOKEN_NONE = 0,
    LYXP_TOKEN_PAR1,          /* '(' */
    LYXP_TOKEN_PAR2,          /* ')' */
    LYXP_TOKEN_BRACK1,        /* '[' */
    LYXP_TOKEN_BRACK2,        /* ']' */
    LYXP_TOKEN_DOT,           /* '.' */
    LYXP_TOKEN_DDOT,          /* '..' */
    LYXP_TOKEN_AT,            /* '@' */
    LYXP_TOKEN_COMMA,         /* ',' */
    /* LYXP_TOKEN_DCOLON,      * '::' * axes not supported */
    LYXP_TOKEN_NAMETEST,      /* NameTest */
    LYXP_TOKEN_NODETYPE,      /* NodeType */
    LYXP_TOKEN_FUNCNAME,      /* FunctionName */
    LYXP_TOKEN_OPERATOR_LOG,  /* Operator 'and', 'or' */
    LYXP_TOKEN_OPERATOR_COMP, /* Operator '=', '!=', '<', '<=', '>', '>=' */
    LYXP_TOKEN_OPERATOR_MATH, /* Operator '+', '-', '*', 'div', 'mod', '-' (unary) */
    LYXP_TOKEN_OPERATOR_UNI,  /* Operator '|' */
    LYXP_TOKEN_OPERATOR_PATH, /* Operator '/', '//' */
    /* LYXP_TOKEN_AXISNAME,    * AxisName * axes not supported */
    LYXP_TOKEN_LITERAL,       /* Literal - with either single or double quote */
    LYXP_TOKEN_NUMBER         /* Number */
};

/**
 * @brief Structure holding a parsed XPath expression.
 */
struct lyxp_expr {
    enum lyxp_token *tokens; /* array of tokens */
    uint16_t *expr_pos;      /* array of pointers to the expression in expr (idx of the beginning) */
    uint8_t *tok_len;        /* array of token lengths in expr */
    uint16_t **repeat;       /* array of the operator token indices that succeed this expression ended with 0,
                                more in the comment after this declaration */
    uint16_t used;           /* used array items */
    uint16_t size;           /* allocated array items */

    char *expr;              /* the original XPath expression */
};

/*
 * lyxp_expr repeat
 *
 * This value is NULL for all the tokens that do not begin an
 * expression which can be repeated. Otherwise it is an array
 * of indices in the tokens array that are an operator for
 * which the current expression is an operand. These values
 * are used during evaluation to know whether we need to
 * duplicate the current context or not. Examples:
 *
 * Expression: "/ *[key1 and key2 or key1 < key2]"
 * Tokens: '/',  '*',  '[',  NameTest,  'and', NameTest, 'or', NameTest, '<',  NameTest, ']'
 * Repeat: NULL, NULL, NULL, [4, 6, 0], NULL,  NULL,     NULL, [8, 0],   NULL, NULL,     NULL
 *
 * Expression: "//node[key and node2]/key | /cont"
 * Tokens: '//',   'NameTest', '[',  'NameTest', 'and', 'NameTest', ']',  '/',  'NameTest', '|',  '/',  'NameTest'
 * Repeat: [9, 0], NULL,       NULL, [4, 0],     NULL,  NULL,       NULL, NULL, NULL,       NULL, NULL, NULL
 *
 * Operators between expressions which this concerns:
 *     'or', 'and', '=', '!=', '<', '>', '<=', '>=', '+', '-', '*', 'div', 'mod', '|'
 */

/**
 * @brief Supported types of (partial) XPath results.
 */
enum lyxp_set_type {
    LYXP_SET_EMPTY = 0,
    LYXP_SET_NODE_SET,
    LYXP_SET_SNODE_SET,
    LYXP_SET_BOOLEAN,
    LYXP_SET_NUMBER,
    LYXP_SET_STRING
};

/**
 * @brief XPath set - (partial) result.
 */
struct lyxp_set {
    enum lyxp_set_type type;
    union {
        struct lyxp_set_nodes {
            struct lyd_node *node;
            enum lyxp_node_type type;
            uint32_t pos;
        } *nodes;
        struct lyxp_set_snodes {
            struct lys_node *snode;
            enum lyxp_node_type type;
            /* 0 - snode was traversed, but not currently in the context,
             * 1 - snode currently in context,
             * 2 - snode in context and just added, so skip it for the current operation,
             * >=3 - snode is not in context because we are in a predicate and this snode was used/will be used later */
            uint32_t in_ctx;
        } *snodes;
        struct lyxp_set_attrs {
            struct lyd_attr *attr;
            enum lyxp_node_type type;
            uint32_t pos; /* if node_type is LYXP_SET_NODE_ATTR, it is the parent node position */
        } *attrs;
        char *str;
        long double num;
        int bool;
    } val;

    /* this is valid only for type LYXP_SET_NODE_SET and LYXP_SET_SNODE_SET */
    uint32_t used;
    uint32_t size;
    /* this is valid only for type LYXP_SET_NODE_SET */
    uint32_t ctx_pos;
    uint32_t ctx_size;
};

/**
 * @brief Evaluate the XPath expression \p expr on data. Be careful when using this function, the result can often
 * be confusing without thorough understanding of XPath evaluation rules defined in RFC 6020.
 *
 * @param[in] expr XPath expression to evaluate. Must be in JSON format (prefixes are model names).
 * @param[in] cur_node Current (context) data node. If the node has #LYD_VAL_INUSE flag, it is considered dummy (intended
 * for but not restricted to evaluation with the LYXP_WHEN flag).
 * @param[in] cur_node_type Current (context) data node type. For every standard case use #LYXP_NODE_ELEM. But there are
 * cases when the context node \p cur_node is actually supposed to be the XML root, there is no such data node. So, in
 * this case just pass the first top-level node into \p cur_node and use an enum value for this kind of root
 * (#LYXP_NODE_ROOT_CONFIG if \p cur_node has config true, otherwise #LYXP_NODE_ROOT). #LYXP_NODE_TEXT and #LYXP_NODE_ATTR can also be used,
 * but there are no use-cases in YANG.
 * @param[in] local_mod Local module relative to the \p expr. Used only to determine the internal canonical value for identities.
 * @param[out] set Result set. Must be valid and in the same libyang context as \p cur_node.
 * To be safe, always either zero or cast the \p set to empty. After done using, either cast
 * the \p set to empty (if allocated statically) or free it (if allocated dynamically) to
 * prevent memory leaks.
 * @param[in] options Whether to apply some evaluation restrictions.
 * LYXP_MUST - apply must data tree access restrictions.
 * LYXP_WHEN - apply when data tree access restrictions and consider LYD_WHEN flags in data nodes.
 *
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on unresolved when dependency, -1 on error.
 */
int lyxp_eval(const char *expr, const struct lyd_node *cur_node, enum lyxp_node_type cur_node_type,
              const struct lys_module *local_mod, struct lyxp_set *set, int options);

/**
 * @brief Get all the partial XPath nodes (atoms) that are required for \p expr to be evaluated.
 *
 * @param[in] expr XPath expression to be evaluated. Must be in JSON format (prefixes are model names).
 * @param[in] cur_snode Current (context) schema node.
 * @param[in] cur_snode_type Current (context) schema node type.
 * @param[out] set Result set. Must be valid and in the same libyang context as \p cur_snode.
 * To be safe, always either zero or cast the \p set to empty. After done using, either cast
 * the \p set to empty (if allocated statically) or free it (if allocated dynamically) to
 * prevent memory leaks.
 * @param[in] options Whether to apply some evaluation restrictions, one flag must always be used.
 * LYXP_SNODE - no special data tree access modifiers.
 * LYXP_SNODE_MUST - apply must data tree access restrictions.
 * LYXP_SNODE_WHEN - apply when data tree access restrictions.
 * LYXP_SNODE_OUTPUT - search RPC/action output instead input
 * @param[out] ctx_snode Actual context node for the expression (it often changes for "when" expressions).
 *
 * @return EXIT_SUCCESS on success, -1 on error.
 */
int lyxp_atomize(const char *expr, const struct lys_node *cur_snode, enum lyxp_node_type cur_snode_type,
                 struct lyxp_set *set, int options, const struct lys_node **ctx_snode);

/* these are used only internally */
#define LYXP_SNODE 0x04
#define LYXP_SNODE_MUST 0x08
#define LYXP_SNODE_WHEN 0x10
#define LYXP_SNODE_OUTPUT 0x20

#define LYXP_SNODE_ALL 0x1C

/**
 * @brief Works like lyxp_atomize(), but it is executed on all the when and must expressions
 * which the node has.
 *
 * @param[in] node Node to examine.
 * @param[in,out] set Resulting set of atoms merged from all the expressions.
 * Will be cleared before use.
 * @param[in] warn_on_fwd_ref Setting this flag causes no errors to be printed and
 * only warning is printed on forward reference paths (addressing a non-existing node).
 *
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on forward reference, -1 on error.
 */
int lyxp_node_atomize(const struct lys_node *node, struct lyxp_set *set, int warn_on_fwd_ref);

/**
 * @brief Check syntax of all the XPath expressions of the node.
 *
 * @param[in] node Node to examine.
 *
 * @return EXIT_SUCCESS on success, -1 on error.
 */
int lyxp_node_check_syntax(const struct lys_node *node);

/**
 * @brief Cast XPath set to another type.
 *        Indirectly context position aware.
 *
 * @param[in] set Set to cast.
 * @param[in] target Target type to cast \p set into.
 * @param[in] cur_node Current (context) data node. Cannot be NULL.
 * @param[in] local_mod Local expression module.
 * @param[in] options Whether to apply some evaluation restrictions.
 *
 * @return EXIT_SUCCESS on success, -1 on error.
 */
int lyxp_set_cast(struct lyxp_set *set, enum lyxp_set_type target, const struct lyd_node *cur_node,
                  const struct lys_module *local_mod, int options);

/**
 * @brief Free contents of an XPath \p set.
 *
 * @param[in] set Set to free.
 */
void lyxp_set_free(struct lyxp_set *set);

/**
 * @brief Parse an XPath expression into a structure of tokens.
 *        Logs directly.
 *
 * http://www.w3.org/TR/1999/REC-xpath-19991116/ section 3.7
 *
 * @param[in] expr XPath expression to parse. It is duplicated.
 *
 * @return Filled expression structure or NULL on error.
 */
struct lyxp_expr *lyxp_parse_expr(const char *expr);

/**
 * @brief Frees a parsed XPath expression. \p expr should not be used afterwards.
 *
 * @param[in] expr Expression to free.
 */
void lyxp_expr_free(struct lyxp_expr *expr);

#endif /* _XPATH_H */
