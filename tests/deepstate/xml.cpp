extern "C" {
#include "xml.h"
#include "context.h"

LY_ERR lyxml_ns_add(struct lyxml_ctx *xmlctx, const char *prefix, size_t prefix_len, char *uri);
LY_ERR lyxml_ns_rm(struct lyxml_ctx *xmlctx);
}

#include <deepstate/DeepState.hpp>

class XMLTest : public deepstate::Test {
	public:
		struct ly_ctx *ctx;
		void SetUp(void) {
			ly_ctx_new(NULL, 0, &ctx);
		}

		void TearDown(void) {
			ly_ctx_destroy(ctx, NULL);
		}
};

TEST_F(XMLTest, TestElement) {
    struct lyxml_ctx *xmlctx;
    const char *str;

    /* empty */
    str = "";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_END, xmlctx->status);
    ASSERT_TRUE(xmlctx->input[0] == '\0');
    lyxml_ctx_free(xmlctx);

    /* end element */
    str = "</element>";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_new(ctx, str, &xmlctx));

    /* no element */
    str = "no data present";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_new(ctx, str, &xmlctx));

        /* not supported DOCTYPE */
    str = "<!DOCTYPE greeting SYSTEM \"hello.dtd\"><greeting/>";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_new(ctx, str, &xmlctx));

    /* invalid XML */
    str = "<!NONSENSE/>";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_new(ctx, str, &xmlctx));

    /* unqualified element */
    str = "  <  element/>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
    ASSERT_EQ(xmlctx->prefix, nullptr);
    ASSERT_TRUE(!strncmp("element", xmlctx->name, xmlctx->name_len));
    ASSERT_EQ(1, xmlctx->elements.count);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("", xmlctx->value, xmlctx->value_len));

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CLOSE, xmlctx->status);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_END, xmlctx->status);
    lyxml_ctx_free(xmlctx);

    /* element with attribute */
    str = "  <  element attr=\'x\'/>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("element", xmlctx->name, xmlctx->name_len));
    ASSERT_EQ(xmlctx->prefix, nullptr); 

    ASSERT_EQ(1, xmlctx->elements.count);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ATTRIBUTE, xmlctx->status);
    ASSERT_TRUE(!strncmp("attr", xmlctx->name, xmlctx->name_len));
    ASSERT_EQ(xmlctx->prefix, nullptr);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ATTR_CONTENT, xmlctx->status);
    ASSERT_EQ(1, xmlctx->elements.count);
    ASSERT_TRUE(!strncmp("x", xmlctx->value, xmlctx->value_len));

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CLOSE, xmlctx->status);
    ASSERT_EQ(0, xmlctx->elements.count);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_END, xmlctx->status);
    lyxml_ctx_free(xmlctx);

    /* headers and comments */
    str = "<?xml version=\"1.0\"?>  <!-- comment --> <![CDATA[<greeting>Hello, world!</greeting>]]> <?TEST xxx?> <element/>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("element", xmlctx->name, xmlctx->name_len));
    ASSERT_EQ(xmlctx->prefix, nullptr);
    ASSERT_EQ(1, xmlctx->elements.count);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CLOSE, xmlctx->status);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_END, xmlctx->status);
    lyxml_ctx_free(xmlctx);

    /* separate opening and closing tags, neamespaced parsed internally */
    str = "<element xmlns=\"urn\"></element>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("element", xmlctx->name, xmlctx->name_len));
    ASSERT_EQ(xmlctx->prefix, nullptr);
    ASSERT_EQ(1, xmlctx->elements.count);
    ASSERT_EQ(1, xmlctx->ns.count);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CLOSE, xmlctx->status);
    ASSERT_EQ(0, xmlctx->elements.count);
    ASSERT_EQ(0, xmlctx->ns.count);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_END, xmlctx->status);
    lyxml_ctx_free(xmlctx);

    /* qualified element */
    str = "  <  yin:element/>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("element", xmlctx->name, xmlctx->name_len));
    ASSERT_TRUE(!strncmp("yin", xmlctx->prefix, xmlctx->prefix_len));

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CLOSE, xmlctx->status);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_END, xmlctx->status);
    lyxml_ctx_free(xmlctx);

    /* invalid UTF-8 characters */
    str = "<¢:element>";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_new(ctx, str, &xmlctx));

    str = "<yin:c⁐element>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LY_EVALID, lyxml_ctx_next(xmlctx));
    lyxml_ctx_free(xmlctx);

    /* mixed content */
    str = "<a>text <b>x</b></a>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("a", xmlctx->name, xmlctx->name_len));
    ASSERT_EQ(xmlctx->prefix, nullptr);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("text ", xmlctx->value, xmlctx->value_len));

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("b", xmlctx->name, xmlctx->name_len));
    ASSERT_EQ(xmlctx->prefix, nullptr);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("x", xmlctx->value, xmlctx->value_len));

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CLOSE, xmlctx->status);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CLOSE, xmlctx->status);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_END, xmlctx->status);
    lyxml_ctx_free(xmlctx);

    /* tag mismatch */
    str = "<a>text</b>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("a", xmlctx->name, xmlctx->name_len));
    ASSERT_EQ(xmlctx->prefix, nullptr);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("text", xmlctx->value, xmlctx->value_len));

    ASSERT_EQ(LY_EVALID, lyxml_ctx_next(xmlctx));
    lyxml_ctx_free(xmlctx);
}

TEST_F(XMLTest, TestAttr) {
    const char *str;
    struct lyxml_ctx *xmlctx;
    struct lyxml_ns *ns;

    /* not an attribute */
    str = "<e unknown/>";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_new(ctx, str, &xmlctx));

        str = "<e xxx=/>";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_new(ctx, str, &xmlctx));

    str = "<e xxx\n = yyy/>";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_new(ctx, str, &xmlctx));

    /* valid attribute */
    str = "<e attr=\"val\"";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ATTRIBUTE, xmlctx->status);
    ASSERT_TRUE(!strncmp("attr", xmlctx->name, xmlctx->name_len));
    ASSERT_EQ(xmlctx->prefix, nullptr);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ATTR_CONTENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("val", xmlctx->name, xmlctx->name_len));
    ASSERT_EQ(xmlctx->ws_only, 0);
    ASSERT_EQ(xmlctx->dynamic, 0);
    lyxml_ctx_free(xmlctx);

    /* valid namespace with prefix */
    str = "<e xmlns:nc\n = \'urn\'/>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
    ASSERT_EQ(1, xmlctx->ns.count);
    ns = (struct lyxml_ns *)xmlctx->ns.objs[0];
    ASSERT_EQ(strcmp(ns->prefix, "nc"), 0);
    ASSERT_EQ(strcmp(ns->uri, "urn"), 0);
    lyxml_ctx_free(xmlctx);
}

TEST_F(XMLTest, TestText) {
    const char *str;
    struct lyxml_ctx *xmlctx;

    /* empty attribute value */
    str = "<e a=\"\"";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ATTRIBUTE, xmlctx->status);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ATTR_CONTENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("", xmlctx->value, xmlctx->value_len));
    ASSERT_EQ(xmlctx->ws_only, 1);
    ASSERT_EQ(xmlctx->dynamic, 0);

    /* empty value but in single quotes */
    xmlctx->status = LYXML_ATTRIBUTE;
    xmlctx->input = "=\'\'";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ATTR_CONTENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("", xmlctx->value, xmlctx->value_len));
    ASSERT_EQ(xmlctx->ws_only, 1);
    ASSERT_EQ(xmlctx->dynamic, 0);

    /* empty element content - only formating before defining child */
    xmlctx->status = LYXML_ELEMENT;
    xmlctx->input = ">\n  <y>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("\n  ", xmlctx->value, xmlctx->value_len));
    ASSERT_EQ(xmlctx->ws_only, 1);
    ASSERT_EQ(xmlctx->dynamic, 0);

        /* empty element content is invalid - missing content terminating character < */
    xmlctx->status = LYXML_ELEM_CONTENT;
    xmlctx->input = "";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_next(xmlctx));

    xmlctx->status = LYXML_ELEM_CONTENT;
    xmlctx->input = "xxx";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_next(xmlctx));

    lyxml_ctx_free(xmlctx);

    /* valid strings */
    str = "<a>€𠜎Øn \n&lt;&amp;&quot;&apos;&gt; &#82;&#x4f;&#x4B;</a>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));
    ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);

    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("€𠜎Øn \n<&\"\'> ROK", xmlctx->value, xmlctx->value_len));
    ASSERT_EQ(xmlctx->ws_only, 0);
    ASSERT_EQ(xmlctx->dynamic, 1);
    free((char *)xmlctx->value);

    /* test using n-bytes UTF8 hexadecimal code points */
    xmlctx->status = LYXML_ATTRIBUTE;
    xmlctx->input = "=\'&#x0024;&#x00A2;&#x20ac;&#x10348;\'";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LYXML_ATTR_CONTENT, xmlctx->status);
    ASSERT_TRUE(!strncmp("$¢€𐍈", xmlctx->value, xmlctx->value_len));
    ASSERT_EQ(xmlctx->ws_only, 0);
    ASSERT_EQ(xmlctx->dynamic, 1);
    free((char *)xmlctx->value);

    /* invalid characters in string */
    xmlctx->status = LYXML_ATTRIBUTE;

    xmlctx->input = "=\'&#x52\'";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_next(xmlctx));

    xmlctx->status = LYXML_ATTRIBUTE;
    xmlctx->input = "=\"&#82\"";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_next(xmlctx));

    xmlctx->status = LYXML_ATTRIBUTE;
    xmlctx->input = "=\"&nonsense;\"";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_next(xmlctx));

    xmlctx->status = LYXML_ELEMENT;
    xmlctx->input = ">&#o122;";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_next(xmlctx));

    xmlctx->status = LYXML_ATTRIBUTE;
    xmlctx->input = "=\'&#x06;\'";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_next(xmlctx));

    xmlctx->status = LYXML_ATTRIBUTE;
    xmlctx->input = "=\'&#xfdd0;\'";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_next(xmlctx));

    xmlctx->status = LYXML_ATTRIBUTE;
    xmlctx->input = "=\'&#xffff;\'";
    ASSERT_EQ(LY_EVALID, lyxml_ctx_next(xmlctx));

    lyxml_ctx_free(xmlctx);
}

TEST_F(XMLTest, TestNS) {
    const char *str;
    struct lyxml_ctx *xmlctx;
    const struct lyxml_ns *ns;

    /* opening element1 */
    str = "<element1/>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));

    /* processing namespace definitions */
    ASSERT_EQ(LY_SUCCESS, lyxml_ns_add(xmlctx, NULL, 0, strdup("urn:default")));
    ASSERT_EQ(LY_SUCCESS, lyxml_ns_add(xmlctx, "nc", 2, strdup("urn:nc1")));
    /* simulate adding open element2 into context */
    xmlctx->elements.count++;
    /* processing namespace definitions */
    ASSERT_EQ(LY_SUCCESS, lyxml_ns_add(xmlctx, "nc", 2, strdup("urn:nc2")));
    ASSERT_EQ(3, xmlctx->ns.count);
    ASSERT_NE(0, xmlctx->ns.size);

    ns = lyxml_ns_get(xmlctx, NULL, 0);
    ASSERT_NE(ns, nullptr);
    ASSERT_EQ(ns->prefix, nullptr);
    ASSERT_EQ(strcmp("urn:default", ns->uri), 0);

    ns = lyxml_ns_get(xmlctx, "nc", 2);
    ASSERT_NE(ns, nullptr);
    ASSERT_EQ(strcmp("nc", ns->prefix), 0);
    ASSERT_EQ(strcmp("urn:nc2", ns->uri), 0);

    /* simulate closing element2 */
    xmlctx->elements.count--;
    lyxml_ns_rm(xmlctx);
    ASSERT_EQ(2, xmlctx->ns.count);

    ns = lyxml_ns_get(xmlctx, "nc", 2);

    ASSERT_NE(ns, nullptr);
    ASSERT_EQ(strcmp("nc", ns->prefix), 0);
    ASSERT_EQ(strcmp("urn:nc1", ns->uri), 0);

    /* close element1 */
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(0, xmlctx->ns.count);

    ASSERT_EQ(lyxml_ns_get(xmlctx, "nc", 2), nullptr);
    ASSERT_EQ(lyxml_ns_get(xmlctx, NULL, 0), nullptr);

    lyxml_ctx_free(xmlctx);
}

TEST_F(XMLTest, TestNS2) {
    const char *str;
    struct lyxml_ctx *xmlctx;

    /* opening element1 */
    str = "<element1/>";
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, str, &xmlctx));

    /* default namespace defined in parent element1 */
    ASSERT_EQ(LY_SUCCESS, lyxml_ns_add(xmlctx, NULL, 0, strdup("urn:default")));
    ASSERT_EQ(1, xmlctx->ns.count);
    /* going into child element1 */
    /* simulate adding open element1 into context */
    xmlctx->elements.count++;
    /* no namespace defined, going out (first, simulate closing of so far open element) */
    xmlctx->elements.count--;
    lyxml_ns_rm(xmlctx);
    ASSERT_EQ(1, xmlctx->ns.count);

    /* nothing else, going out of the parent element1 */
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
    ASSERT_EQ(0, xmlctx->ns.count);

    lyxml_ctx_free(xmlctx);
}

TEST_F(XMLTest, TestSimpleXML) {
	struct lyxml_ctx *xmlctx;

	const char *test_input = "<elem1 attr1=\"value\"> <elem2 attr2=\"value\" /> </elem1>";
	ASSERT_EQ(LY_SUCCESS, lyxml_ctx_new(ctx, test_input, &xmlctx));
	ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
	ASSERT_EQ(strcmp(xmlctx->input, "attr1=\"value\"> <elem2 attr2=\"value\" /> </elem1>"), 0);

	ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
	ASSERT_EQ(LYXML_ATTRIBUTE, xmlctx->status);
	ASSERT_EQ(strcmp(xmlctx->input, "=\"value\"> <elem2 attr2=\"value\" /> </elem1>"), 0);

	ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
	ASSERT_EQ(LYXML_ATTR_CONTENT, xmlctx->status);
	ASSERT_EQ(strcmp(xmlctx->input, "> <elem2 attr2=\"value\" /> </elem1>"), 0);

	ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
	ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);
	ASSERT_EQ(strcmp(xmlctx->input, "<elem2 attr2=\"value\" /> </elem1>"), 0);

	ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
	ASSERT_EQ(LYXML_ELEMENT, xmlctx->status);
	ASSERT_EQ(strcmp(xmlctx->input, "attr2=\"value\" /> </elem1>"), 0);

	ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
	ASSERT_EQ(LYXML_ATTRIBUTE, xmlctx->status);
	ASSERT_EQ(strcmp(xmlctx->input, "=\"value\" /> </elem1>"), 0);

	ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
	ASSERT_EQ(LYXML_ATTR_CONTENT, xmlctx->status);
	ASSERT_EQ(strcmp(xmlctx->input, " /> </elem1>"), 0);

	ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
	ASSERT_EQ(LYXML_ELEM_CONTENT, xmlctx->status);
	ASSERT_EQ(strcmp(xmlctx->input, "/> </elem1>"), 0);

	ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
	ASSERT_EQ(LYXML_ELEM_CLOSE, xmlctx->status);
	ASSERT_EQ(strcmp(xmlctx->input, " </elem1>"), 0);

	ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
	ASSERT_EQ(LYXML_ELEM_CLOSE, xmlctx->status);
	ASSERT_EQ(strcmp(xmlctx->input, ""), 0);

	ASSERT_EQ(LY_SUCCESS, lyxml_ctx_next(xmlctx));
	ASSERT_EQ(LYXML_END, xmlctx->status);
	ASSERT_EQ(strcmp(xmlctx->input, ""), 0);

	lyxml_ctx_free(xmlctx);
}
