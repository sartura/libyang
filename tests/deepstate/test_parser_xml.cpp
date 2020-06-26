extern "C" {
#include "context.h"
#include "printer.h"
#include "printer_data.h"
#include "tree_data_internal.h"
#include "tree_schema.h"
#include "tests/config.h"
}

#include <deepstate/DeepState.hpp>

class XMLParserTest : public deepstate::Test {
	public:
		struct ly_ctx *ctx = NULL;
		void SetUp(void) {
			const char *schema_a = "module a {namespace urn:tests:a;prefix a;yang-version 1.1;"
			    "list l1 { key \"a b c\"; leaf a {type string;} leaf b {type string;} leaf c {type int16;} leaf d {type string;}}"
			    "leaf foo { type string;}"
			    "container c {"
				"leaf x {type string;}"
				"action act { input { leaf al {type string;} } output { leaf al {type uint8;} } }"
				"notification n1 { leaf nl {type string;} }"
			    "}"
			    "container cp {presence \"container switch\"; leaf y {type string;} leaf z {type int8;}}"
			    "anydata any {config false;}"
			    "leaf foo2 { type string; default \"default-val\"; }"
			    "leaf foo3 { type uint32; }"
			    "notification n2;}";
			const struct lys_module *mod;

			ASSERT_EQ(LY_SUCCESS, ly_ctx_new(TESTS_DIR_MODULES_YANG, 0, &ctx));
			ASSERT_NE(ly_ctx_load_module(ctx, "ietf-netconf-with-defaults", "2011-06-01"), nullptr);
			ASSERT_NE((mod = ly_ctx_load_module(ctx, "ietf-netconf", "2011-06-01")), nullptr);
			ASSERT_EQ(LY_SUCCESS, lys_feature_enable(mod, "writable-running"));
			ASSERT_NE(lys_parse_mem(ctx, schema_a, LYS_IN_YANG), nullptr);
		}

		void TearDown(void) {
			ly_ctx_destroy(ctx, NULL);
		}
};

TEST_F(XMLParserTest, TestLeaf) {
	const char *data = "<foo xmlns=\"urn:tests:a\">foo value</foo>";
	struct lyd_node *tree;
	struct lyd_node_term *leaf;

	ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
	ASSERT_NE(tree, nullptr);
	ASSERT_EQ(LYS_LEAF, tree->schema->nodetype);
	ASSERT_EQ(strcmp("foo", tree->schema->name), 0);
	leaf = (struct lyd_node_term*)tree;
	ASSERT_EQ(strcmp("foo value", leaf->value.original), 0);

	ASSERT_EQ(LYS_LEAF, tree->next->next->schema->nodetype);
	ASSERT_EQ(strcmp("foo2", tree->next->next->schema->name), 0);
	leaf = (struct lyd_node_term*)tree->next->next;
	ASSERT_EQ(strcmp("default-val", leaf->value.original), 0);
	ASSERT_TRUE(leaf->flags & LYD_DEFAULT);

	lyd_free_all(tree);

	/* make foo2 explicit */
	data = "<foo2 xmlns=\"urn:tests:a\">default-val</foo2>";
	ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
	ASSERT_NE(tree, nullptr);
	ASSERT_EQ(LYS_LEAF, tree->schema->nodetype);
	ASSERT_EQ(0, strcmp("foo2", tree->schema->name));
	leaf = (struct lyd_node_term*)tree;
	ASSERT_EQ(0, strcmp("default-val", leaf->value.original));
	ASSERT_FALSE(leaf->flags & LYD_DEFAULT);

	lyd_free_all(tree);

	/* parse foo2 but make it implicit */
	data = "<foo2 xmlns=\"urn:tests:a\" xmlns:wd=\"urn:ietf:params:xml:ns:yang:ietf-netconf-with-defaults\" wd:default=\"true\">default-val</foo2>";
	ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
	ASSERT_NE(nullptr, tree);
	ASSERT_EQ(LYS_LEAF, tree->schema->nodetype);
	ASSERT_EQ(0, strcmp("foo2", tree->schema->name));
	leaf = (struct lyd_node_term*)tree;
	ASSERT_EQ(0, strcmp("default-val", leaf->value.original));
	ASSERT_TRUE(leaf->flags & LYD_DEFAULT);

	lyd_free_all(tree);
}

TEST_F(XMLParserTest, FuzzLeaf) {
	char *data = deepstate::DeepState_CStrUpToLen(2049, NULL);
	struct lyd_node *tree = NULL;

	lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree);
}

TEST_F(XMLParserTest, TestAnydata) {
    const char *data;
    char *str;
    struct lyd_node *tree;

    struct ly_out *out;
    ASSERT_EQ(LY_SUCCESS, ly_out_new_memory(&str, 0, &out));

    data =
    "<any xmlns=\"urn:tests:a\">"
        "<element1>"
            "<x:element2 x:attr2=\"test\" xmlns:a=\"urn:tests:a\" xmlns:x=\"urn:x\">a:data</x:element2>"
        "</element1>"
        "<element1a/>"
    "</any>";
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(LYS_ANYDATA, tree->schema->nodetype);
    ASSERT_EQ(0, strcmp("any", tree->schema->name));

    lyd_print(out, tree, LYD_XML, 0);
    ASSERT_EQ(0, strcmp(str,
        "<any xmlns=\"urn:tests:a\">"
            "<element1>"
                "<element2 xmlns=\"urn:x\" xmlns:x=\"urn:x\" x:attr2=\"test\" xmlns:a=\"urn:tests:a\">a:data</element2>"
            "</element1>"
            "<element1a/>"
        "</any>"
    ));
    ly_out_reset(out);

    lyd_free_all(tree);
    ly_out_free(out, NULL, 1);
}

TEST_F(XMLParserTest, TestList) {
    const char *data = "<l1 xmlns=\"urn:tests:a\"><a>one</a><b>one</b><c>1</c></l1>";
    struct lyd_node *tree, *iter;
    struct lyd_node_inner *list;
    struct lyd_node_term *leaf;

    /* check hashes */
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(LYS_LIST, tree->schema->nodetype);
    ASSERT_EQ(0, strcmp("l1", tree->schema->name));
    list = (struct lyd_node_inner*)tree;
    LY_LIST_FOR(list->child, iter) {
        ASSERT_NE(0, iter->hash);
    }
    lyd_free_all(tree);

    /* missing keys */
    data = "<l1 xmlns=\"urn:tests:a\"><c>1</c><b>b</b></l1>";
    ASSERT_EQ(LY_EVALID, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));

    data = "<l1 xmlns=\"urn:tests:a\"><a>a</a></l1>";
    ASSERT_EQ(LY_EVALID, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));

    data = "<l1 xmlns=\"urn:tests:a\"><b>b</b><a>a</a></l1>";
    ASSERT_EQ(LY_EVALID, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));

    /* key duplicate */
    data = "<l1 xmlns=\"urn:tests:a\"><c>1</c><b>b</b><a>a</a><c>1</c></l1>";
    ASSERT_EQ(LY_EVALID, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));

    /* keys order */
    data = "<l1 xmlns=\"urn:tests:a\"><d>d</d><a>a</a><c>1</c><b>b</b></l1>";
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(LYS_LIST, tree->schema->nodetype);
    ASSERT_EQ(0, strcmp("l1", tree->schema->name));
    list = (struct lyd_node_inner*)tree;
    ASSERT_NE(nullptr, leaf = (struct lyd_node_term*)list->child);
    ASSERT_EQ(0, strcmp("a", leaf->schema->name));
    ASSERT_NE(nullptr, leaf = (struct lyd_node_term*)leaf->next);
    ASSERT_EQ(0, strcmp("b", leaf->schema->name));
    ASSERT_NE(nullptr, leaf = (struct lyd_node_term*)leaf->next);
    ASSERT_EQ(0, strcmp("c", leaf->schema->name));
    ASSERT_NE(nullptr, leaf = (struct lyd_node_term*)leaf->next);
    ASSERT_EQ(0, strcmp("d", leaf->schema->name));
    lyd_free_all(tree);

    data = "<l1 xmlns=\"urn:tests:a\"><c>1</c><b>b</b><a>a</a></l1>";
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(LYS_LIST, tree->schema->nodetype);
    ASSERT_EQ(0, strcmp("l1", tree->schema->name));
    list = (struct lyd_node_inner*)tree;
    ASSERT_NE(nullptr, leaf = (struct lyd_node_term*)list->child);
    ASSERT_EQ(0, strcmp("a", leaf->schema->name));
    ASSERT_NE(nullptr, leaf = (struct lyd_node_term*)leaf->next);
    ASSERT_EQ(0, strcmp("b", leaf->schema->name));
    ASSERT_NE(nullptr, leaf = (struct lyd_node_term*)leaf->next);
    ASSERT_EQ(0, strcmp("c", leaf->schema->name));
    lyd_free_all(tree);

    ASSERT_EQ(LY_EVALID, lyd_parse_xml_data(ctx, data, LYD_OPT_STRICT, &tree));
}

TEST_F(XMLParserTest, TestOpaq) {
    const char *data;
    char *str;
    struct lyd_node *tree;

    struct ly_out *out;
    ASSERT_EQ(LY_SUCCESS, ly_out_new_memory(&str, 0, &out));

    /* invalid value, no flags */
    data = "<foo3 xmlns=\"urn:tests:a\"/>";
    ASSERT_EQ(LY_EVALID, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_EQ(nullptr, tree);

    /* opaq flag */
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_OPT_OPAQ | LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(nullptr, tree->schema);
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)tree)->name, "foo3"));
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)tree)->value, ""));

    lyd_print(out, tree, LYD_XML, 0);
    ASSERT_EQ(0, strcmp(str, "<foo3 xmlns=\"urn:tests:a\"/>"));
    ly_out_reset(out);
    lyd_free_all(tree);

    /* missing key, no flags */
    data = "<l1 xmlns=\"urn:tests:a\"><a>val_a</a><b>val_b</b><d>val_d</d></l1>";
    ASSERT_EQ(LY_EVALID, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_EQ(nullptr, tree);

    /* opaq flag */
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_OPT_OPAQ | LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(nullptr, tree->schema);
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)tree)->name, "l1"));
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)tree)->value, ""));

    lyd_print(out, tree, LYD_XML, 0);
    ASSERT_EQ(0, strcmp(str, data));
    ly_out_reset(out);
    lyd_free_all(tree);

    /* invalid key, no flags */
    data = "<l1 xmlns=\"urn:tests:a\"><a>val_a</a><b>val_b</b><c>val_c</c></l1>";
    ASSERT_EQ(LY_EVALID, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_EQ(nullptr, tree);

    /* opaq flag */
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_OPT_OPAQ | LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(nullptr, tree->schema);
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)tree)->name, "l1"));
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)tree)->value, ""));

    lyd_print(out, tree, LYD_XML, 0);
    ASSERT_EQ(0, strcmp(str, data));
    ly_out_reset(out);
    lyd_free_all(tree);

    /* opaq flag and fail */
    ASSERT_EQ(LY_EVALID, lyd_parse_xml_data(ctx, "<a xmlns=\"ns\"><b>x</b><c xml:id=\"D\">1</c></a>",
            LYD_OPT_OPAQ | LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_EQ(nullptr, tree);

    ly_out_free(out, NULL, 1);
}

TEST_F(XMLParserTest, TestRPC) {
    const char *data;
    char *str;
    struct lyd_node *tree, *op;
    const struct lyd_node *node;

    struct ly_out *out;
    ASSERT_EQ(LY_SUCCESS, ly_out_new_memory(&str, 0, &out));

    data =
        "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\" msgid=\"25\" custom-attr=\"val\">"
            "<edit-config>"
                "<target>"
                    "<running/>"
                "</target>"
                "<config xmlns:nc=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
                    "<l1 xmlns=\"urn:tests:a\" nc:operation=\"replace\">"
                        "<a>val_a</a>"
                        "<b>val_b</b>"
                        "<c>val_c</c>"
                    "</l1>"
                    "<cp xmlns=\"urn:tests:a\">"
                        "<z nc:operation=\"delete\"/>"
                    "</cp>"
                "</config>"
            "</edit-config>"
        "</rpc>";
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_rpc(ctx, data, &tree, &op));

    ASSERT_NE(nullptr, op);
    ASSERT_EQ(0, strcmp(op->schema->name, "edit-config"));

    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(nullptr, tree->schema);
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)tree)->name, "rpc"));
    ASSERT_NE(nullptr, ((struct lyd_node_opaq *)tree)->attr);
    node = lyd_node_children(tree, 0);
    ASSERT_EQ(0, strcmp(node->schema->name, "edit-config"));
    node = lyd_node_children(node, 0)->next;
    ASSERT_EQ(0, strcmp(node->schema->name, "config"));
    node = ((struct lyd_node_any *)node)->value.tree;
    /* l1 key c has invalid value */
    ASSERT_EQ(nullptr, node->schema);
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)node)->name, "l1"));
    node = node->next;
    ASSERT_NE(nullptr, node->schema);
    ASSERT_EQ(0, strcmp(node->schema->name, "cp"));
    node = lyd_node_children(node, 0);
    /* z has no value */
    ASSERT_EQ(nullptr, node->schema);
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)node)->name, "z"));

    lyd_print(out, tree, LYD_XML, 0);
    ASSERT_EQ(0, strcmp(str,
        "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\" msgid=\"25\" custom-attr=\"val\">"
            "<edit-config>"
                "<target>"
                    "<running/>"
                "</target>"
                "<config>"
                    "<l1 xmlns=\"urn:tests:a\" xmlns:nc=\"urn:ietf:params:xml:ns:netconf:base:1.0\" nc:operation=\"replace\">"
                        "<a>val_a</a>"
                        "<b>val_b</b>"
                        "<c>val_c</c>"
                    "</l1>"
                    "<cp xmlns=\"urn:tests:a\">"
                        "<z xmlns:nc=\"urn:ietf:params:xml:ns:netconf:base:1.0\" nc:operation=\"delete\"/>"
                    "</cp>"
                "</config>"
            "</edit-config>"
        "</rpc>"));
    ly_out_reset(out);
    lyd_free_all(tree);

    /* wrong namespace, element name, whatever... */
    /* TODO */

    ly_out_free(out, NULL, 1);
}

TEST_F(XMLParserTest, TestAction) {
    const char *data;
    char *str;
    struct lyd_node *tree, *op;
    const struct lyd_node *node;

    struct ly_out *out;
    ASSERT_EQ(LY_SUCCESS, ly_out_new_memory(&str, 0, &out));

    data =
        "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\" msgid=\"25\" custom-attr=\"val\">"
            "<action xmlns=\"urn:ietf:params:xml:ns:yang:1\">"
                "<c xmlns=\"urn:tests:a\">"
                    "<act>"
                        "<al>value</al>"
                    "</act>"
                "</c>"
            "</action>"
        "</rpc>";
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_rpc(ctx, data, &tree, &op));

    ASSERT_NE(nullptr, op);
    ASSERT_EQ(0, strcmp(op->schema->name, "act"));

    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(nullptr, tree->schema);
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)tree)->name, "rpc"));
    ASSERT_NE(nullptr, ((struct lyd_node_opaq *)tree)->attr);
    node = lyd_node_children(tree, 0);
    ASSERT_EQ(nullptr, node->schema);
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)node)->name, "action"));
    ASSERT_EQ(nullptr, ((struct lyd_node_opaq *)node)->attr);

    lyd_print(out, tree, LYD_XML, 0);
    ASSERT_EQ(0, strcmp(str,
        "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\" msgid=\"25\" custom-attr=\"val\">"
            "<action xmlns=\"urn:ietf:params:xml:ns:yang:1\">"
                "<c xmlns=\"urn:tests:a\">"
                    "<act>"
                        "<al>value</al>"
                    "</act>"
                "</c>"
            "</action>"
        "</rpc>"));
    ly_out_reset(out);
    lyd_free_all(tree);

    /* wrong namespace, element name, whatever... */
    /* TODO */

    ly_out_free(out, NULL, 1);
}

TEST_F(XMLParserTest, TestContainer) {
    const char *data = "<c xmlns=\"urn:tests:a\"/>";
    struct lyd_node *tree;
    struct lyd_node_inner *cont;

    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(LYS_CONTAINER, tree->schema->nodetype);
    ASSERT_EQ(0, strcmp("c", tree->schema->name));
    cont = (struct lyd_node_inner*)tree;
    ASSERT_TRUE(cont->flags & LYD_DEFAULT);
    lyd_free_all(tree);

    data = "<cp xmlns=\"urn:tests:a\"/>";
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_data(ctx, data, LYD_VALOPT_DATA_ONLY, &tree));
    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(LYS_CONTAINER, tree->schema->nodetype);
    ASSERT_EQ(0, strcmp("cp", tree->schema->name));
    cont = (struct lyd_node_inner*)tree;
    ASSERT_FALSE(cont->flags & LYD_DEFAULT);
    lyd_free_all(tree);
}

TEST_F(XMLParserTest, TestNotification) {
    const char *data;
    char *str;
    struct lyd_node *tree, *ntf;
    const struct lyd_node *node;

    struct ly_out *out;
    ASSERT_EQ(LY_SUCCESS, ly_out_new_memory(&str, 0, &out));

    data =
        "<notification xmlns=\"urn:ietf:params:xml:ns:netconf:notification:1.0\">"
            "<eventTime>2037-07-08T00:01:00Z</eventTime>"
            "<c xmlns=\"urn:tests:a\">"
                "<n1>"
                    "<nl>value</nl>"
                "</n1>"
            "</c>"
        "</notification>";
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_notif(ctx, data, &tree, &ntf));

    ASSERT_NE(nullptr, ntf);
    ASSERT_EQ(0, strcmp(ntf->schema->name, "n1"));

    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(nullptr, tree->schema);
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)tree)->name, "notification"));
    ASSERT_EQ(nullptr, ((struct lyd_node_opaq *)tree)->attr);
    node = lyd_node_children(tree, 0);
    ASSERT_EQ(nullptr, node->schema);
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)node)->name, "eventTime"));
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)node)->value, "2037-07-08T00:01:00Z"));
    ASSERT_EQ(nullptr, ((struct lyd_node_opaq *)node)->attr);
    node = node->next;
    ASSERT_NE(nullptr, node->schema);
    ASSERT_EQ(0, strcmp(node->schema->name, "c"));

    lyd_print(out, tree, LYD_XML, 0);
    ASSERT_EQ(0, strcmp(str, data));
    ly_out_reset(out);
    lyd_free_all(tree);

    /* top-level notif without envelope */
    data = "<n2 xmlns=\"urn:tests:a\"/>";
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_notif(ctx, data, &tree, &ntf));

    ASSERT_NE(nullptr, ntf);
    ASSERT_EQ(0, strcmp(ntf->schema->name, "n2"));

    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(ntf, tree);

    lyd_print(out, tree, LYD_XML, 0);
    ASSERT_EQ(0, strcmp(str, data));
    ly_out_reset(out);
    lyd_free_all(tree);

    /* wrong namespace, element name, whatever... */
    /* TODO */

    ly_out_free(out, NULL, 1);
}

TEST_F(XMLParserTest, TestReply) {
    const char *data;
    char *str;
    struct lyd_node *request, *tree, *op;
    const struct lyd_node *node;

    struct ly_out *out;
    ASSERT_EQ(LY_SUCCESS, ly_out_new_memory(&str, 0, &out));

    data =
        "<c xmlns=\"urn:tests:a\">"
            "<act>"
                "<al>value</al>"
            "</act>"
        "</c>";
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_rpc(ctx, data, &request, NULL));
    data =
        "<rpc-reply xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\" msgid=\"25\">"
            "<al xmlns=\"urn:tests:a\">25</al>"
        "</rpc-reply>";
    ASSERT_EQ(LY_SUCCESS, lyd_parse_xml_reply(request, data, &tree, &op));
    lyd_free_all(request);

    ASSERT_NE(nullptr, op);
    ASSERT_EQ(0, strcmp(op->schema->name, "act"));
    node = lyd_node_children(op, 0);
    ASSERT_NE(nullptr, node->schema);
    ASSERT_EQ(0, strcmp(node->schema->name, "al"));
    ASSERT_TRUE(node->schema->flags & LYS_CONFIG_R);

    ASSERT_NE(nullptr, tree);
    ASSERT_EQ(nullptr, tree->schema);
    ASSERT_EQ(0, strcmp(((struct lyd_node_opaq *)tree)->name, "rpc-reply"));
    ASSERT_NE(nullptr, ((struct lyd_node_opaq *)tree)->attr);
    node = lyd_node_children(tree, 0);
    ASSERT_NE(nullptr, node->schema);
    ASSERT_EQ(0, strcmp(node->schema->name, "c"));

    /* TODO print only rpc-reply node and then output subtree */
    lyd_print(out, lyd_node_children(op, 0), LYD_XML, 0);
    ASSERT_EQ(0, strcmp(str,
        "<al xmlns=\"urn:tests:a\">25</al>"));
    ly_out_reset(out);
    lyd_free_all(tree);

    /* wrong namespace, element name, whatever... */
    /* TODO */

    ly_out_free(out, NULL, 1);
}
